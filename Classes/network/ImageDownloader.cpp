//
//  ImageDownloader.cpp
//  QSMY
//
//  Created by wanghejun on 14-2-13.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "ImageDownloader.h"
#include "utils/FileUtil.h"



// #include "platform/CCThread.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)

#include <queue>
#include <pthread.h>
#include <errno.h>

#include "curl/curl.h"

USING_NS_CC;
USING_NS_CC_EXT;


const char* ImageRequest::DOWNLOAD_COMPLETE = "DOWNLOAD_COMPLETE";
ImageRequest::ImageRequest(){}
ImageRequest::~ImageRequest(){}




static pthread_t        s_networkThread;
static pthread_mutex_t  s_requestQueueMutex;
static pthread_mutex_t  s_responseQueueMutex;

static pthread_mutex_t		s_SleepMutex;
static pthread_cond_t		s_SleepCondition;

static unsigned long    s_asyncRequestCount = 0;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
typedef int int32_t;
#endif

static bool need_quit = false;

static CCArray* s_requestQueue = NULL;
static CCArray* s_responseQueue = NULL;

static ImageDownloader *s_pHttpClient = NULL; // pointer to singleton

static char s_errorBuffer[CURL_ERROR_SIZE];

typedef size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *stream);

// Callback function used by libcurl for collect response data
static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}

// Callback function used by libcurl for collect header data
static size_t writeHeaderData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}


static int processGetTask(ImageRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream);
static int processPostTask(ImageRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream);
static int processPutTask(ImageRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream);
static int processDeleteTask(ImageRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream);

// Worker thread
static void* networkThread(void *data)
{
    ImageRequest *request = NULL;
    
    while (true)
    {
        if (need_quit)
        {
            break;
        }
        
        // step 1: send http request if the requestQueue isn't empty
        request = NULL;
        
        pthread_mutex_lock(&s_requestQueueMutex); //Get request task from queue
        if (0 != s_requestQueue->count())
        {
            request = dynamic_cast<ImageRequest*>(s_requestQueue->objectAtIndex(0));
            s_requestQueue->removeObjectAtIndex(0);
            // request's refcount = 1 here
        }
        pthread_mutex_unlock(&s_requestQueueMutex);
        
        if (NULL == request)
        {
        	// Wait for http request tasks from main thread
        	pthread_cond_wait(&s_SleepCondition, &s_SleepMutex);
            continue;
        }
        
        // step 2: libcurl sync access
        
        // Create a HttpResponse object, the default setting is http access failed
        CCHttpResponse *response = new CCHttpResponse(request);
        
        // request's refcount = 2 here, it's retained by HttpRespose constructor
        request->release();
        // ok, refcount = 1 now, only HttpResponse hold it.
        
        long responseCode = -1;
        int retValue = 0;
        
        // Process the request -> get response packet
        switch (request->getRequestType())
        {
            case ImageRequest::kHttpGet: // HTTP GET
                retValue = processGetTask(request,
                                          writeData,
                                          response->getResponseData(),
                                          &responseCode,
                                          writeHeaderData,
                                          response->getResponseHeader());
                break;
                
            case ImageRequest::kHttpPost: // HTTP POST
                retValue = processPostTask(request,
                                           writeData,
                                           response->getResponseData(),
                                           &responseCode,
                                           writeHeaderData,
                                           response->getResponseHeader());
                break;
                
            case ImageRequest::kHttpPut:
                retValue = processPutTask(request,
                                          writeData,
                                          response->getResponseData(),
                                          &responseCode,
                                          writeHeaderData,
                                          response->getResponseHeader());
                break;
                
            case ImageRequest::kHttpDelete:
                retValue = processDeleteTask(request,
                                             writeData,
                                             response->getResponseData(),
                                             &responseCode,
                                             writeHeaderData,
                                             response->getResponseHeader());
                break;
                
            default:
                CCAssert(true, "ImageDownloader: unkown request type, only GET and POSt are supported");
                break;
        }
        
        // write data to HttpResponse
        response->setResponseCode(responseCode);
        
        if (retValue != 0)
        {
            response->setSucceed(false);
            response->setErrorBuffer(s_errorBuffer);
        }
        else
        {
            response->setSucceed(true);
        }
        
        
        // add response packet into queue
        pthread_mutex_lock(&s_responseQueueMutex);
        s_responseQueue->addObject(response);
        pthread_mutex_unlock(&s_responseQueueMutex);
        
        // resume dispatcher selector
        CCDirector::sharedDirector()->getScheduler()->resumeTarget(ImageDownloader::getInstance());
    }
    
    // cleanup: if worker thread received quit signal, clean up un-completed request queue
    pthread_mutex_lock(&s_requestQueueMutex);
    s_requestQueue->removeAllObjects();
    pthread_mutex_unlock(&s_requestQueueMutex);
    s_asyncRequestCount -= s_requestQueue->count();
    
    if (s_requestQueue != NULL) {
        
        pthread_mutex_destroy(&s_requestQueueMutex);
        pthread_mutex_destroy(&s_responseQueueMutex);
        
        pthread_mutex_destroy(&s_SleepMutex);
        pthread_cond_destroy(&s_SleepCondition);
        
        s_requestQueue->release();
        s_requestQueue = NULL;
        s_responseQueue->release();
        s_responseQueue = NULL;
    }
    
    pthread_exit(NULL);
    
    return 0;
}

//Configure curl's timeout property
static bool configureCURL(CURL *handle)
{
    if (!handle) {
        return false;
    }
    
    int32_t code;
    code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, s_errorBuffer);
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, ImageDownloader::getInstance()->getTimeoutForRead());
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, ImageDownloader::getInstance()->getTimeoutForConnect());
    if (code != CURLE_OK) {
        return false;
    }
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);
    
    // FIXED #3224: The subthread of ImageDownloader interrupts main thread if timeout comes.
    // Document is here: http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTNOSIGNAL
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);
    
    if(ImageDownloader::getInstance()->getCookieFile().compare("")!=0)
    {
        CCLOG(ImageDownloader::getInstance()->getCookieFile().c_str());
        code = curl_easy_setopt(handle, CURLOPT_COOKIEFILE, ImageDownloader::getInstance()->getCookieFile().c_str());
        if (code != CURLE_OK) {
            return false;
        }
        
        code = curl_easy_setopt(handle, CURLOPT_COOKIEJAR, ImageDownloader::getInstance()->getCookieFile().c_str());
        if (code != CURLE_OK) {
            return false;
        }
        
    }
    
    return true;
}

class CURLRaii
{
    /// Instance of CURL
    CURL *m_curl;
    /// Keeps custom header data
    curl_slist *m_headers;
public:
    CURLRaii()
    : m_curl(curl_easy_init())
    , m_headers(NULL)
    {
    }
    
    ~CURLRaii()
    {
        if (m_curl)
            curl_easy_cleanup(m_curl);
        /* free the linked list for header data */
        if (m_headers)
            curl_slist_free_all(m_headers);
    }
    
    template <class T>
    bool setOption(CURLoption option, T data)
    {
        return CURLE_OK == curl_easy_setopt(m_curl, option, data);
    }
    
    /**
     * @brief Inits CURL instance for common usage
     * @param request Null not allowed
     * @param callback Response write callback
     * @param stream Response write stream
     */
    bool init(ImageRequest *request, write_callback callback, void *stream, write_callback headerCallback, void *headerStream)
    {
        if (!m_curl)
            return false;
        if (!configureCURL(m_curl))
            return false;
        
        return setOption(CURLOPT_URL, request->getUrl())
        && setOption(CURLOPT_WRITEFUNCTION, callback)
        && setOption(CURLOPT_WRITEDATA, stream)
        && setOption(CURLOPT_HEADERFUNCTION, headerCallback)
        && setOption(CURLOPT_HEADERDATA, headerStream);
        
    }
    
    /// @param responseCode Null not allowed
    bool perform(long *responseCode)
    {
        if (CURLE_OK != curl_easy_perform(m_curl))
            return false;
        CURLcode code = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, responseCode);
        if (code != CURLE_OK || *responseCode != 200)
            return false;
        
        // Get some mor data.
        
        return true;
    }
};

//Process Get Request
static int processGetTask(ImageRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
    && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
    && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process POST Request
static int processPostTask(ImageRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
    && curl.setOption(CURLOPT_POST, 1)
    && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
    && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
    && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process PUT Request
static int processPutTask(ImageRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
    && curl.setOption(CURLOPT_CUSTOMREQUEST, "PUT")
    && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
    && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
    && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process DELETE Request
static int processDeleteTask(ImageRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
    && curl.setOption(CURLOPT_CUSTOMREQUEST, "DELETE")
    && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
    && curl.perform(responseCode);
    return ok ? 0 : 1;
}

// HttpClient implementation
ImageDownloader* ImageDownloader::getInstance()
{
    if (s_pHttpClient == NULL) {
        s_pHttpClient = new ImageDownloader();
    }
    
    return s_pHttpClient;
}

void ImageDownloader::destroyInstance()
{
    CCAssert(s_pHttpClient, "");
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImageDownloader::dispatchResponseCallbacks), s_pHttpClient);
    s_pHttpClient->release();
}

ImageDownloader::ImageDownloader()
: _timeoutForConnect(30)
, _timeoutForRead(60)
{
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                                                                   schedule_selector(ImageDownloader::dispatchResponseCallbacks), this, 0, false);
    CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
}

ImageDownloader::~ImageDownloader()
{
    need_quit = true;
    
    if (s_requestQueue != NULL) {
    	pthread_cond_signal(&s_SleepCondition);
    }
    
    s_pHttpClient = NULL;
}

//Lazy create semaphore & mutex & thread
bool ImageDownloader::lazyInitThreadSemphore()
{
    if (s_requestQueue != NULL) {
        return true;
    } else {
        
        s_requestQueue = new CCArray();
        s_requestQueue->init();
        
        s_responseQueue = new CCArray();
        s_responseQueue->init();
        
        pthread_mutex_init(&s_requestQueueMutex, NULL);
        pthread_mutex_init(&s_responseQueueMutex, NULL);
        
        pthread_mutex_init(&s_SleepMutex, NULL);
        pthread_cond_init(&s_SleepCondition, NULL);
        
        pthread_create(&s_networkThread, NULL, networkThread, NULL);
        pthread_detach(s_networkThread);
        
        need_quit = false;
    }
    
    return true;
}

//Add a get task to queue
ImageRequest* ImageDownloader::download(const char* url, const char* localPath)
{
    if (false == lazyInitThreadSemphore())
    {
        return NULL;
    }
    
    CCObject* obj = NULL;
    ImageRequest* request = NULL;
    ImageRequest* rt = NULL;
    {
        pthread_mutex_lock(&s_requestQueueMutex);
        CCARRAY_FOREACH(s_requestQueue, obj)
        {
            request = static_cast<ImageRequest*>(obj);
            if(request && request->getLocalFile().compare(localPath)==0)
            {
                rt = request;
                break;
            }
        }
        pthread_mutex_unlock(&s_requestQueueMutex);
        if(rt) return rt;
    }
    
    {
        pthread_mutex_lock(&s_responseQueueMutex);
        CCHttpResponse* response = NULL;
        CCARRAY_FOREACH(s_responseQueue, obj)
        {
            response = static_cast<CCHttpResponse*>(obj);
            if(response)
            {
                ImageRequest *request = static_cast<ImageRequest*>(response->getHttpRequest());
                if(request && request->getLocalFile().compare(localPath)==0)
                {
                    rt = request;
                    break;
                }
            }
        }
        pthread_mutex_unlock(&s_responseQueueMutex);
        if(rt) return rt;
    }
    
    
    ++s_asyncRequestCount;
    request = new ImageRequest(); //后面会release
    request->init();
    request->setUrl(url);
    request->setRequestType(ImageRequest::kHttpGet);
    request->setLocalFile(localPath);
    
    pthread_mutex_lock(&s_requestQueueMutex);
    s_requestQueue->addObject(request);
    pthread_mutex_unlock(&s_requestQueueMutex);
    
    // Notify thread start to work
    pthread_cond_signal(&s_SleepCondition);
    
    return request;
}

// Poll and notify main thread if responses exists in queue
void ImageDownloader::dispatchResponseCallbacks(float delta)
{
    // CCLog("ImageDownloader::dispatchResponseCallbacks is running");
    
    CCHttpResponse* response = NULL;
    
    pthread_mutex_lock(&s_responseQueueMutex);
    if (s_responseQueue->count())
    {
        response = dynamic_cast<CCHttpResponse*>(s_responseQueue->objectAtIndex(0));
        s_responseQueue->removeObjectAtIndex(0);
    }
    pthread_mutex_unlock(&s_responseQueueMutex);
    
    if (response)
    {
        --s_asyncRequestCount;
        
        ImageRequest *request = static_cast<ImageRequest*>(response->getHttpRequest());
        //CCObject *pTarget = request->getTarget();
        //SEL_HttpResponse pSelector = request->getSelector();
        
//        if (pTarget && pSelector)
//        {
//            (pTarget->*pSelector)(this, response);
//        }
        
        if(response->isSucceed())
        {
            CCImage* pImage = new CCImage();
            std::string img(response->getResponseData()->begin(),response->getResponseData()->end());
            pImage->initWithImageData((void*)img.c_str(), img.length());
            CCTextureCache::sharedTextureCache()->addUIImage(pImage, request->getLocalFile().c_str());
            //pImage->saveToFile(request->getLocalFile().c_str(),false);
            pImage->release();
            
            FileUtil::builddir(request->getLocalFile().c_str());
            FILE* fp = fopen(request->getLocalFile().c_str(), "wb+");
            fwrite(img.c_str(), sizeof(char), img.length(), fp);
            fclose(fp);
        }
        
        request->postNotification(ImageRequest::DOWNLOAD_COMPLETE);
        response->release();
    }
    
    if (0 == s_asyncRequestCount)
    {
        CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
    }
    
}



#endif // CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)