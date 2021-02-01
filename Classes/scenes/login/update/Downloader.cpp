//
//  Downloader.cpp
//  QSMY
//
//  Created by wanghejun on 13-10-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Downloader.h"
#include "utils/FileUtil.h"
#include "utils/SystemUtil.h"


enum MessageType {
    kComplete = 1,
    kError    = 2,
    kCancel   = 3,
};

void* Downloader::doDownload(void *context)
{
    Downloader* handle = (Downloader*) context;
    while (!handle->m_bNeedQuit) {
        handle->download();
    }
    pthread_exit(NULL);
    return NULL;
}


size_t Downloader::process_data(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

int Downloader::progress(void *context, double dlTotal, double dlNow, double upTotal, double upNow)
{
    Downloader* handle = (Downloader*) context;
    if(handle)
    {
        handle->m_fCurSize = dlNow;
    }
	return 0;
}

struct CompleteMessage
{
    version::FileInfo *file;
    Downloader* handle;
};


struct ErrorMessage
{
    Downloader::ErrorCode  code;
    version::FileInfo *file;
    Downloader* handle;
    uint32_t http_code;
};

struct CancelMessage
{
    version::FileInfo *file;
    Downloader* handle;
};



Downloader::Downloader()
:m_fCurSize(0.0)
,_tid(NULL)
,m_bNeedQuit(false)
{
    pthread_mutex_init(&m_queueMutex, NULL);
    m_queue = new std::vector<version::FileInfo*>();
    _schedule = new Helper();
}

Downloader::~Downloader()
{
    m_bNeedQuit = true;
    if(_tid) pthread_join(*_tid, NULL);
    m_Delegate = NULL;
    while (true){
        pthread_mutex_lock(&m_queueMutex);
        if(m_queue->empty())
        {
            pthread_mutex_unlock(&m_queueMutex);
            break;
        }
        version::FileInfo* pFile= m_queue->front();
        m_queue->erase(m_queue->begin());
        pthread_mutex_unlock(&m_queueMutex);
        Downloader::Message *msg = new Downloader::Message();
        CancelMessage *pMsg = new CancelMessage();
        pMsg->handle = this;
        pMsg->file = pFile;
        msg->what = kCancel;
        msg->obj = pMsg;
        this->_schedule->sendMessage(msg);
    }
    CC_SAFE_DELETE(_tid);
    CC_SAFE_DELETE(m_queue);
    if(_schedule) _schedule->destory();
    CC_SAFE_RELEASE(_schedule);
}

void Downloader::addFile(version::FileInfo* pInfo)
{
    if(_tid==NULL)
    {
        _tid = new pthread_t();
        pthread_create(_tid, NULL, Downloader::doDownload, this);
    }
    
    CC_SAFE_RETAIN(pInfo);
    
    pthread_mutex_lock(&m_queueMutex);
    m_queue->push_back(pInfo);
    pthread_mutex_unlock(&m_queueMutex);
}


void Downloader::download()
{
    pthread_mutex_lock(&m_queueMutex);
    if(m_queue->empty())
    {
        pthread_mutex_unlock(&m_queueMutex);
        return;
    }
    version::FileInfo* pFile= m_queue->front();
    m_queue->erase(m_queue->begin());
    pthread_mutex_unlock(&m_queueMutex);
    
    m_fCurSize = 0.0;
    
    const char* file = pFile->getLocalURL().c_str();
    const char* url = pFile->getRemoteURL().c_str();
    
    CCLOG("download %s\n%s",file,url);

    
    FILE* fp = fopen(file, "wb+");
    if (!fp)
    {
        // build path directory, try again!
        FileUtil::builddir(file);
        fp = fopen(file, "wb+");
        if ( !fp )
        {
            Downloader::Message *msg = new Downloader::Message();
            ErrorMessage *pMsg = new ErrorMessage();
            pMsg->handle = this;
            pMsg->file = pFile;
            pMsg->code = kCreateFile;
            msg->what = kError;
            msg->obj = pMsg;
            this->_schedule->sendMessage(msg);
            return;
        }
    }
    
    CURL *m_handle = curl_easy_init();
    curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION,Downloader::process_data);
    curl_easy_setopt(m_handle, CURLOPT_CONNECTTIMEOUT, 30l);
    curl_easy_setopt(m_handle, CURLOPT_TIMEOUT, 60l);
    curl_easy_setopt(m_handle, CURLOPT_NOSIGNAL, 1L);
    
    curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, fp);
    
	curl_easy_setopt(m_handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(m_handle, CURLOPT_SSL_VERIFYHOST, 0L);

	curl_easy_setopt(m_handle, CURLOPT_FOLLOWLOCATION, true);
    curl_easy_setopt(m_handle, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(m_handle, CURLOPT_PROGRESSFUNCTION, Downloader::progress);
    curl_easy_setopt(m_handle, CURLOPT_PROGRESSDATA, this);
    
    curl_easy_setopt(m_handle, CURLOPT_URL, url);

    int retv = curl_easy_perform(m_handle);
    long retcode = 0;
    if ( retv == CURLE_OK )
        retv = curl_easy_getinfo(m_handle, CURLINFO_RESPONSE_CODE , &retcode);
    curl_easy_cleanup(m_handle);
    
    //用于比较文件大小
    long downloadedSize = 0;
    fseek(fp, 0, SEEK_END);
    downloadedSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    fclose(fp);
    SystemUtil::addSkipBackupAttributeToItemAtPath(file);
    //bool skiped = SystemUtil::isFileSkipedBackUp(file);
    //CCLOG("%s, skiped:%d", file, (int)skiped);
    
    if(retv != CURLE_OK || (retcode >= 400 && retcode!=404) || downloadedSize != pFile->getSize())
    {
        FileUtil::remove(file); //下载文件出错 删除本地文件
        
        Downloader::Message *msg = new Downloader::Message();
        ErrorMessage *pMsg = new ErrorMessage();
        pMsg->handle = this;
        pMsg->file = pFile;
        pMsg->code = kNetwork;
        pMsg->http_code = retcode;
        msg->what = kError;
        msg->obj = pMsg;
        this->_schedule->sendMessage(msg);
        return;
    }
    
    if(retcode == 404)
    {
        FileUtil::remove(file); //找不到网络数据 删除本地文件
    }
    
    this->m_fCurSize = 0.0;
    Downloader::Message *msg = new Downloader::Message();
    CompleteMessage *pMsg = new CompleteMessage();
    pMsg->handle = this;
    pMsg->file = pFile;
    msg->what = kComplete;
    msg->obj = pMsg;
    this->_schedule->sendMessage(msg);
    
}

size_t Downloader::getWaitingCount()
{
    pthread_mutex_lock(&m_queueMutex);
    size_t size = m_queue->size();
    pthread_mutex_unlock(&m_queueMutex);
	return size;
}


void Downloader::Helper::destory()
{
    cocos2d::CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    Message *msg = NULL;
    while (true)
    {
        pthread_mutex_lock(&_messageQueueMutex);
        if(_messageQueue->empty())
        {
            pthread_mutex_unlock(&_messageQueueMutex);
            break;
        }
        
        msg = *(_messageQueue->begin());
        _messageQueue->pop_front();
        
        pthread_mutex_unlock(&_messageQueueMutex);
        
        switch (msg->what) {
            case kComplete:
                handleComplete(msg);
                break;
            //case kProgress:
            //    handleProgress(msg);
            //    break;
            case kError:
                handleError(msg);
                break;
            case kCancel:
                handleCancel(msg);
                break;
        }
        delete msg;
    }
    CC_SAFE_DELETE(_messageQueue);
}

Downloader::Helper::Helper()
{
    _messageQueue = new std::list<Message*>();
    pthread_mutex_init(&_messageQueueMutex, NULL);
    cocos2d::CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

Downloader::Helper::~Helper()
{
    
}

void Downloader::Helper::sendMessage(Message *msg)
{
    pthread_mutex_lock(&_messageQueueMutex);
    _messageQueue->push_back(msg);
    pthread_mutex_unlock(&_messageQueueMutex);
}

void Downloader::Helper::update(float dt)
{
    Message *msg = NULL;
    static int max_count = 2;
    int count = 0;
    // Returns quickly if no message
    
    while (true)
    {
        CC_BREAK_IF(++count>max_count);
        pthread_mutex_lock(&_messageQueueMutex);
        if(!_messageQueue || _messageQueue->empty())
        {
            pthread_mutex_unlock(&_messageQueueMutex);
            break;
        }
        
        msg = *(_messageQueue->begin());
        _messageQueue->pop_front();
        
        pthread_mutex_unlock(&_messageQueueMutex);
        
        switch (msg->what) {
            case kComplete:
                handleComplete(msg);
                break;
            //case kProgress:
            //    handleProgress(msg);
            //    break;
            case kError:
                handleError(msg);
                break;
            case kCancel:
                handleCancel(msg);
                break;
        }
        delete msg;
    }
    
}

void Downloader::Helper::handleProgress(Message *msg)
{
    Downloader* handle = (Downloader*)msg->obj;
    if(handle && handle->getDelegate())
    {
        handle->getDelegate()->onProgress();
    }
}

void Downloader::Helper::handleError(Message *msg)
{
    ErrorMessage* pMsg = static_cast<ErrorMessage*>(msg->obj);
    if(pMsg->handle)
    {
        if(pMsg->handle->getDelegate())
        {
            pMsg->handle->getDelegate()->onError(pMsg->file,pMsg->code,pMsg->http_code);
        }
        CC_SAFE_RELEASE(pMsg->file);
    }
    CC_SAFE_DELETE(pMsg);
}

void Downloader::Helper::handleComplete(Message *msg)
{
    CompleteMessage* pMsg = static_cast<CompleteMessage*>(msg->obj);
    if(pMsg->handle)
    {
        if(pMsg->handle->getDelegate())
        {
            pMsg->handle->getDelegate()->onComplete(pMsg->file);
        }
        CC_SAFE_RELEASE(pMsg->file);
    }
    CC_SAFE_DELETE(pMsg);
}

void Downloader::Helper::handleCancel(Message *msg)
{
    CancelMessage* pMsg = static_cast<CancelMessage*>(msg->obj);
    if(pMsg->handle)
    {
        if(pMsg->handle->getDelegate())
        {
            pMsg->handle->getDelegate()->onCancel(pMsg->file);
        }
        CC_SAFE_RELEASE(pMsg->file);
    }
    CC_SAFE_DELETE(pMsg);
}
