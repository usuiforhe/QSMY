//
//  Downloader.h
//  QSMY
//
//  Created by wanghejun on 13-10-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Downloader__
#define __QSMY__Downloader__

#include <curl/curl.h>
#include <curl/easy.h>
#include "cocos2d.h"
#include "model/VersionModel.h"
#include <pthread.h>

class DownloaderDelegate;

class Downloader
{
    class Helper;
public:
    
    enum ErrorCode
    {
        kCreateFile,
        kNetwork,
    };

    
    enum edownloaderr_t
	{
		e_downloaderr_ok = 0,
		e_downloaderr_connect,
		e_downloaderr_timeout,
		e_downloaderr_notfound,
		e_downloaderr_other_nobp,
        e_loaderr_openfile_failed,
	};
    
    CC_SYNTHESIZE_READONLY(double, m_fCurSize, CurSize);
    
    
public:
    
	Downloader();
	~Downloader();
    void addFile(version::FileInfo* pInfo);
    
    static void* doDownload(void *context);
    
    void download();
    size_t getWaitingCount();
private:
	static size_t process_data(void* buffer, size_t size, size_t nmemb, void* context);
	static int progress(void *context, double dlTotal, double dlNow, double upTotal, double upNow);

private:    
    pthread_t                       *_tid;
    std::vector<version::FileInfo*> *m_queue;
    pthread_mutex_t                 m_queueMutex;
    Helper                          *_schedule;
    bool                            m_bNeedQuit;
    CC_SYNTHESIZE(DownloaderDelegate*, m_Delegate, Delegate);
    
private:
    typedef struct _Message
    {
    public:
        _Message() : what(0), obj(NULL){}
        unsigned int what; // message type
        void* obj;
    } Message;
    
    class Helper : public cocos2d::CCObject
    {
    public:
        Helper();
        ~Helper();
        
        virtual void update(float dt);
        void sendMessage(Message *msg);
        void destory();
    private:
        void handleProgress(Message *msg);
        void handleComplete(Message *msg);
        void handleError(Message *msg);
        void handleCancel(Message *msg);
        std::list<Message*> *_messageQueue;
        pthread_mutex_t _messageQueueMutex;
    };
};


class DownloaderDelegate
{
public:
    virtual void onError(version::FileInfo*,Downloader::ErrorCode errorCode,uint32_t httpCode) {};
    virtual void onProgress() {};
    virtual void onComplete(version::FileInfo*) {};
    virtual void onCancel(version::FileInfo*) {};

};

#endif /* defined(__QSMY__Downloader__) */
