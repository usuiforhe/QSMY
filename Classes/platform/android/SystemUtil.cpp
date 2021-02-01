//
//  SystemUtil.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "utils/SystemUtil.h"
#include <string>
#include <jni.h>
#include "jni/JniHelper.h"
#include <android/log.h>

#define  LOG_TAG    "Java_org_cocos2dx_lib_QsmyHelper.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,"%S",__VA_ARGS__)

#define  HELPER_NAME "org/cocos2dx/lib/QsmyHelper"

USING_NS_CC;
static std::string m_sVersionName = "";
std::string SystemUtil::getUDID()
{
	JniMethodInfo t;
	std::string ret("");

	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "getDeviceId", "()Ljava/lang/String;")) {
		jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		ret = JniHelper::jstring2string(str);
		t.env->DeleteLocalRef(str);
	}
	return ret;
}

void SystemUtil::openURL(const char* url)
{
	if (url == NULL) {
		return;
	}

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "openUrl", "(Ljava/lang/String;)V")) {
		jstring stringArg = t.env->NewStringUTF(url);

		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg);
		t.env->DeleteLocalRef(stringArg);
		t.env->DeleteLocalRef(t.classID);
	}
}

std::string SystemUtil::getDeviceInfo()
{
    JniMethodInfo t;
	std::string ret("");
    
	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "getDeviceInfo", "()Ljava/lang/String;")) {
		jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		ret = JniHelper::jstring2string(str);
		t.env->DeleteLocalRef(str);
	}
    return ret;
}

bool SystemUtil::isNBDevice()
{
	double totalMem = getTotalMemory();
	CCLOG("isNBDevice--totalMem: %d, isNb:%d", totalMem, totalMem > 1024.00);
    return true;
}

void SystemUtil::setIdleTimerDisabled(bool val)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "setScreenLock", "(Z)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, val);
		t.env->DeleteLocalRef(t.classID);
	}
}

void SystemUtil::sendLocalNotification(const char*message,double delaySecond)
{
	if (message == NULL) {
		return;
	}

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "customizeNotification", "(Ljava/lang/String;D)V")) {
		jstring stringArg = t.env->NewStringUTF(message);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg, delaySecond);
		t.env->DeleteLocalRef(stringArg);
		t.env->DeleteLocalRef(t.classID);
	}
}


void SystemUtil::cancelNotification()
{
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "cancelNotificationTask", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

std::string SystemUtil::getVersion()
{
    CCLOG("in jstring2string--m_sVersionName: %s", m_sVersionName.c_str());
	if(!m_sVersionName.empty()) return m_sVersionName;
	JniMethodInfo t;
	std::string ret("");

	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "getVerSionName", "()Ljava/lang/String;")) {
		jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
        //CCLOG("enter jstring2string--");
		ret = JniHelper::jstring2string(str);
        CCLOG("out jstring2string:%s", ret.c_str());
		t.env->DeleteLocalRef(str);
	}
	m_sVersionName = ret;
    return ret;
}

NetworkStatus SystemUtil::getNetworkStatus()
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "getNetworkType", "()I")) {
        jint ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
        
        t.env->DeleteLocalRef(t.classID);

        return (NetworkStatus)ret;
	}
}

void SystemUtil::hideMouseCursor()
{
    
}

void SystemUtil::updateGame(const char* url,const char* content)
{
    if (url == NULL) {
		return;
	}
	/*JniMethodInfo t;
    
	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "showUpadateDialog", "(Ljava/lang/String;Ljava/lang/String;)V")) {
		jstring stringArg1;
        
        if (!content) {
            stringArg1 = t.env->NewStringUTF("");
        } else {
            stringArg1 = t.env->NewStringUTF(content);
        }
		jstring stringArg2 = t.env->NewStringUTF(url);

		t.env->CallStaticVoidMethod(t.classID,t.methodID, stringArg1, stringArg2);

		t.env->DeleteLocalRef(stringArg1);
		t.env->DeleteLocalRef(stringArg2);
		t.env->DeleteLocalRef(t.classID);
	}*/
	openURL(url);
}

double SystemUtil::availableMemory()
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "getAvailMemory", "()D")) {
		jdouble ret = t.env->CallStaticIntMethod(t.classID, t.methodID);

		t.env->DeleteLocalRef(t.classID);

		return ret;
	}else{
		 return 0;
	}  
}

double SystemUtil::usedMemory()
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "getUseMemory", "()D")) {
		jdouble ret = t.env->CallStaticIntMethod(t.classID, t.methodID);

		t.env->DeleteLocalRef(t.classID);

		return ret;
	}else{
		return 0;
	}
}

double SystemUtil::getTotalMemory()
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "getTotalMemory", "()D")) {
		jdouble ret = t.env->CallStaticIntMethod(t.classID, t.methodID);

		t.env->DeleteLocalRef(t.classID);

		return ret;
	}else{
		return 0;
	}  
}

void SystemUtil::quitGame()
{
    JniMethodInfo t;
    
	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "quitGame", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

bool SystemUtil::addSkipBackupAttributeToItemAtPath(std::string path)
{
	return true;
}

bool SystemUtil::isFileSkipedBackUp(std::string path)
{
	return true;
}

bool SystemUtil::copyStringToPasteboard(std::string content)
{
	JniMethodInfo t;
	CCLOG("enter copyStringToPasteboard--%s",content.c_str());
	if (JniHelper::getStaticMethodInfo(t, HELPER_NAME, "copyStringToClipBoard", "(Ljava/lang/String;)V")) {
		jstring stringArg1;

		stringArg1 = t.env->NewStringUTF(content.c_str());
		t.env->CallStaticVoidMethod(t.classID,t.methodID, stringArg1);

		t.env->DeleteLocalRef(stringArg1);
		t.env->DeleteLocalRef(t.classID);
	}
    return true;
}

#endif /*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)*/