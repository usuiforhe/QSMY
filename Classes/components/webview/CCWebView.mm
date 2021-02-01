//
//  CCWebView.cpp
//  CCXWebview
//
//  Created by Vincent on 12-11-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CCWebView.h"
#import "CCWebView_iOS.h"
#import "CCDirectorCaller.h"

USING_NS_CC;

static CCWebView_iOS* m_webViewiOS;

CCWebView::CCWebView()
{
    
}

CCWebView::~CCWebView()
{
    [m_webViewiOS removeWebView];
    [m_webViewiOS release];
    m_webViewiOS = nil;
}

bool CCWebView::init()
{
    m_webViewiOS = [[CCWebView_iOS alloc] init];
	return true;
}

void CCWebView::showWebView(const char* url, float x, float y, float width, float height)
{
    [m_webViewiOS showWebView_x:x y:y width:width height:height];
    [[CCDirectorCaller sharedDirectorCaller] setRunLoopMode:NSRunLoopCommonModes];
    this->updateURL(url);
}

void CCWebView::showWebView(const char* url, cocos2d::CCNode* node)
{
    cocos2d::CCPoint p = node->convertToWorldSpaceAR(cocos2d::CCPointZero);
    this->showWebView(url,p.x,p.y,node->getContentSize().width,node->getContentSize().height);
}

void CCWebView::updateURL(const char* url)
{
    [m_webViewiOS updateURL:url];
}

void CCWebView::removeWebView()
{
    [m_webViewiOS removeWebView];
    [[CCDirectorCaller sharedDirectorCaller] setRunLoopMode:NSDefaultRunLoopMode];
}
