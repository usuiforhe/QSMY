//
//  CCWebVIewTODO.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "CCWebView.h"
#include "AppMacros.h"
#include "cocos2d.h"
#include <jni.h>
#include "jni/JniHelper.h"
#define ClASS_NAME "org/cocos2dx/lib/QsmyHelper"

#define  LOG_TAG    "CCWebView.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,"%s",__VA_ARGS__)
USING_NS_CC;


CCWebView::CCWebView()
{

}

CCWebView::~CCWebView()
{

}
bool CCWebView::init()
{
	return true;
}

void CCWebView::showWebView(const char* url, cocos2d::CCNode* node)
{
    cocos2d::CCPoint p = node->convertToWorldSpaceAR(cocos2d::CCPointZero);
	
	cocos2d::CCEGLViewProtocol* eglView = cocos2d::CCEGLView::sharedOpenGLView();
    
    cocos2d::CCPoint visiblePos = ccp(p.x * eglView->getScaleX(), p.y * eglView->getScaleY());
    cocos2d::CCPoint screenGLPos = ccpAdd(visiblePos, eglView->getViewPortRect().origin);
    
    cocos2d::CCPoint screenPos = CCPointMake(screenGLPos.x, screenGLPos.y);
	     
    CCSize controlSize = CCSizeMake(node->getContentSize().width * eglView->getScaleX(),node->getContentSize().height * eglView->getScaleY());
		
    this->showWebView(url,screenPos.x, screenPos.y, controlSize.width , controlSize.height);
}
void CCWebView::showWebView(const char* url, float x, float y, float width, float height)
{
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,ClASS_NAME,"displayWebView","(IIII)V"))
	{
		jint jX = (int)x;
		jint jY = (int)y;
		jint jWidth = (int)width;
		jint jHeight = (int)height;
		t.env->CallStaticVoidMethod(t.classID,t.methodID,jX,jY,jWidth,jHeight);
	}
	if(JniHelper::getStaticMethodInfo(t, ClASS_NAME, "updateURL", "(Ljava/lang/String;)V"))
	{
		jstring jUrl = t.env->NewStringUTF(url);
		t.env->CallStaticVoidMethod(t.classID,t.methodID,jUrl);
        t.env->DeleteLocalRef(jUrl);
	}
}


void CCWebView::updateURL(const char* url)
{
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,ClASS_NAME,"updateURL","(Ljava/lang/String;)V"))
	{
		jstring jUrl = t.env->NewStringUTF(url);
		t.env->CallStaticVoidMethod(t.classID,t.methodID,jUrl);
        t.env->DeleteLocalRef(jUrl);
	}
}

void CCWebView::removeWebView()
{
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,ClASS_NAME,"removeWebView","()V"))
	{
		t.env->CallStaticVoidMethod(t.classID,t.methodID);
	}
}
#endif /*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)*/