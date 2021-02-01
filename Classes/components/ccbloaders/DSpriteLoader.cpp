//
//  DSpriteLoader.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-5.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DSpriteLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PROPERTY_TEXTURE_REPEAT "textureRepeat"
#define PROPERTY_PREFEREDSIZE "preferedSize"
#define PROPERTY_TEXTURE_UV "UV"



void DSpriteLoader::onHandlePropTypeCheck(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, bool pCheck, cocos2d::extension::CCBReader * pCCBReader)
{
    if(strcmp(pPropertyName, PROPERTY_TEXTURE_REPEAT) == 0)
    {
        ((DSprite *)pNode)->setTextureRepeat(pCheck);
    } else {
        CCSpriteLoader::onHandlePropTypeCheck(pNode, pParent, pPropertyName, pCheck, pCCBReader);
    }
}


void DSpriteLoader::onHandlePropTypePoint(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, cocos2d::CCPoint pPoint, cocos2d::extension::CCBReader * pCCBReader)
{
    if(strcmp(pPropertyName, PROPERTY_TEXTURE_UV)==0)
    {
        m_Rect.origin = pPoint;
        if(m_bRectReaded) setRect((DSprite *)pNode);
        m_bRectReaded = true;
    }else
    {
        CCSpriteLoader::onHandlePropTypePoint(pNode, pParent, pPropertyName, pPoint, pCCBReader);
    }
}

void DSpriteLoader::onHandlePropTypeSize(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, cocos2d::CCSize pSize, cocos2d::extension::CCBReader * pCCBReader)
{
    if(strcmp(pPropertyName, PROPERTY_PREFEREDSIZE)==0)
    {
        m_Rect.size = pSize;
        if(m_bRectReaded) setRect((DSprite *)pNode);
        m_bRectReaded = true;
    }else
    {
        CCSpriteLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName, pSize, pCCBReader);
    }
}

void DSpriteLoader::setRect(DSprite * pNode)
{
    pNode->setTextureRect(m_Rect);
}
