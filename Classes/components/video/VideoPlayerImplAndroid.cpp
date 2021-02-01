//
//  VideoPlayerImplAndroid.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "VideoPlayerImplAndroid.h"
#include <jni.h>
#include "jni/JniHelper.h"
#include "utils/Notification.h"
#define ClASS_NAME "org/cocos2dx/lib/QsmyHelper"


#define D_EVENT_VIDEO_PLAYEND "android video play end"

#define  LOG_TAG    "VideoPlayerImplAndroid.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,"%s",__VA_ARGS__)

USING_NS_CC;

extern "C" {
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_QsmyHelper_nativeOnVideoPlayedFinished(JNIEnv * env, jobject obj) {
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_VIDEO_PLAYEND);
    }
    
}

VideoPlayerImplAndroid::VideoPlayerImplAndroid(VideoPlayer* pVideoPlayer)
:VideoPlayerImpl(pVideoPlayer)
{
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(VideoPlayerImplAndroid::playEnd),D_EVENT_VIDEO_PLAYEND);
}

void VideoPlayerImplAndroid::playEnd()
{
    LOGD("video play end");
    if(m_pDelegate)
    {
        m_pDelegate->playEnd();
    }
}

VideoPlayerImplAndroid::~VideoPlayerImplAndroid()
{
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,ClASS_NAME,"finishVideoEnforce","()V"))
	{
		t.env->CallStaticVoidMethod(t.classID,t.methodID);
	}

}

void VideoPlayerImplAndroid::playVideo(const char*_url)
{
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,ClASS_NAME,"playVideo","(Ljava/lang/String;)V"))
	{
		jstring stringArg = t.env->NewStringUTF(_url);
		t.env->CallStaticVoidMethod(t.classID,t.methodID,stringArg);
        t.env->DeleteLocalRef(stringArg);
	}
}


VideoPlayerImpl* __createSystemVideoPlayer(VideoPlayer* pVideoPlayer)
{
    return new VideoPlayerImplAndroid(pVideoPlayer);
}

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)*/