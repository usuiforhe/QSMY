//
//  DMenuItemSound.cpp
//  QSMY
//
//  Created by wanghejun on 13-11-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DMenuItemSound.h"

USING_NS_CC;

#define PROPERTY_SOUND "sound"

DMenuItemImageSound::DMenuItemImageSound()
	:m_sSound("")
{
    
}

DMenuItemImageSound::~DMenuItemImageSound()
{
    
}

bool DMenuItemImageSound::init()
{
    if(!CCMenuItemImage::init()) return false;
    return true;
}

void DMenuItemImageSound::activate()
{
    if (m_bEnabled)
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX,CCString::create(m_sSound));
		
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
        
        if (kScriptTypeNone != m_eScriptType)
        {
            CCScriptEngineManager::sharedManager()->getScriptEngine()->executeMenuItemEvent(this);
        }
    }
}


void DMenuItemImageSoundLoader::onHandlePropTypeString(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, const char * pString, cocos2d::extension::CCBReader * pCCBReader)
{
    if(strcmp(pPropertyName, PROPERTY_SOUND) == 0) {
        ((DMenuItemImageSound *)pNode)->setSound(pString);
    } else {
        cocos2d::extension::CCMenuItemImageLoader::onHandlePropTypeString(pNode, pParent, pPropertyName,pString,pCCBReader);
    }
}


DMenuItemImageSound * DMenuItemImageSound::create(const char *normalImage, const char *selectedImage)
{
	return DMenuItemImageSound::create(normalImage, selectedImage, NULL, NULL, NULL);
}

DMenuItemImageSound * DMenuItemImageSound::create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector)
{
	return DMenuItemImageSound::create(normalImage, selectedImage, NULL, target, selector);
}

DMenuItemImageSound * DMenuItemImageSound::create(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector)
{
	DMenuItemImageSound *pRet = new DMenuItemImageSound();
	if (pRet && pRet->initWithNormalImage(normalImage, selectedImage, disabledImage, target, selector))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

DMenuItemImageSound * DMenuItemImageSound::create(const char *normalImage, const char *selectedImage, const char *disabledImage)
{
	DMenuItemImageSound *pRet = new DMenuItemImageSound();
	if (pRet && pRet->initWithNormalImage(normalImage, selectedImage, disabledImage, NULL, NULL))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

DMenuItemImageSound * DMenuItemImageSound::create(CCObject *rec, cocos2d::SEL_MenuHandler selector)
{
	DMenuItemImageSound *pRet = new DMenuItemImageSound();
	if (pRet && pRet->initWithNormalImage(NULL, NULL, NULL, rec, selector))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}




