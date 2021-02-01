//
//  ImageDownloader
//  QSMY
//
//  Created by wanghejun on 14-2-13.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__ImageDownloader__
#define __QSMY__ImageDownloader__

#include "cocos2d.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "utils/Notification.h"

/**
 * @addtogroup Network
 * @{
 */


class ImageRequest : public cocos2d::extension::CCHttpRequest,public Notification {
    
public:
    static const char* DOWNLOAD_COMPLETE;
    ImageRequest();
    ~ImageRequest();
    CC_SYNTHESIZE(std::string, m_sLocalFile, LocalFile);

private:
    void setResponseCallback(cocos2d::CCObject* pTarget, cocos2d::extension::SEL_HttpResponse pSelector){}

};


/** @brief Singleton that handles asynchrounous http requests
 * Once the request completed, a callback will issued in main thread when it provided during make request
 * @js NA
 * @lua NA
 */
class ImageDownloader : public cocos2d::CCObject
{
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_Cookie, CookieFile);
    
public:
    
    void setCookie(std::string val)
    {
        m_Cookie=val;
    }
    
    /** Return the shared instance **/
    static ImageDownloader *getInstance();
    
    /** Relase the shared instance **/
    static void destroyInstance();
    
   
    ImageRequest* download(const char* url, const char* localPath);
    
    
    /**
     * Change the connect timeout
     * @param timeout
     * @return NULL
     */
    inline void setTimeoutForConnect(int value) {_timeoutForConnect = value;};
    
    /**
     * Get connect timeout
     * @return int
     *
     */
    inline int getTimeoutForConnect() {return _timeoutForConnect;}
    
    
    /**
     * Change the download timeout
     * @param value
     * @return NULL
     */
    inline void setTimeoutForRead(int value) {_timeoutForRead = value;};
    
    
    /**
     * Get download timeout
     * @return int
     */
    inline int getTimeoutForRead() {return _timeoutForRead;};
    
private:
    ImageDownloader();
    virtual ~ImageDownloader();
    bool init(void);
    
    /**
     * Init pthread mutex, semaphore, and create new thread for http requests
     * @return bool
     */
    bool lazyInitThreadSemphore();
    /** Poll function called from main thread to dispatch callbacks when http requests finished **/
    void dispatchResponseCallbacks(float delta);
    
private:
    int _timeoutForConnect;
    int _timeoutForRead;
    
    // std::string reqId;
};

// end of Network group
/// @}



#endif /* defined(__QSMY__ImageImageDownloader__) */
