//
//  SceneUtil.cpp
//  QSMY
//
//  Created by mzp on 14-04-16.
//  Copyright (c) 2013ƒÍ thedream. All rights reserved.
//
#include "SceneUtil.h"


USING_NS_CC;


cocos2d::CCImage* SceneUtil::printScreen()
{
	cocos2d::CCSize size = CCDirector::sharedDirector()->getWinSize();
	cocos2d::CCRenderTexture* screen = cocos2d::CCRenderTexture::create(size.width, size.height);
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	screen->begin();
	scene->visit();
	screen->end();

	cocos2d::CCImage* pRet = screen->newCCImage(true);
	if (pRet)
	{
		pRet->autorelease();
	}

	return pRet;
}
