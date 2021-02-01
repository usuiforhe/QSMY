//
//  PLAdaptiveScale9Sprite.cpp
//  QSMY
//
//  Created by Eci on 14-7-1.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "PLAdaptiveScale9Sprite.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PROPERTY_FLEXIBLE_UP "flexibleHeightUp"
#define PROPERTY_FLEXIBLE_DOWN "flexibleHeightDown"
#define PROPERTY_DESIGN_SIZE	"designSize"
#define PROPERTY_FLEXIBLE_POSY	"flexibleHeight"


PLAdaptiveScale9Sprite::PLAdaptiveScale9Sprite()
	: m_fFlexibleHeightDown(0.f)
    , m_fFlexibleHeightUp(0.f)
	, m_fFlexibleHeight(0.f)
{
    
}

PLAdaptiveScale9Sprite::~PLAdaptiveScale9Sprite()
{
}

void PLAdaptiveScale9Sprite::setFlexibleHeightUp(float var)
{
	m_fFlexibleHeightUp = var;
}

void PLAdaptiveScale9Sprite::setFlexibleHeightDown(float var)
{
	m_fFlexibleHeightDown = var;
}

void PLAdaptiveScale9Sprite::setFlexibleHeight(float var)
{
	m_fFlexibleHeight = var;
}

void PLAdaptiveScale9Sprite::onNodeLoaded(cocos2d::CCNode *pNode, cocos2d::extension::CCNodeLoader *pNodeLoader)
{
	PL_RESIZE_WITH_FLEXIBLE_HEIGHT(this, (m_fFlexibleHeightUp + m_fFlexibleHeightDown));
	PL_MOVE_WITH_FLEXIBLE_HEIGHT(this, (m_fFlexibleHeightDown + m_fFlexibleHeight));
}

void PLAdaptiveScale9SpriteLoader::onHandlePropTypeFloat(CCNode * pNode, CCNode * pParent, const char* pPropertyName, float pFloat, CCBReader * pCCBReader)
{
    if(strcmp(pPropertyName, PROPERTY_FLEXIBLE_UP) == 0) {
		((PLAdaptiveScale9Sprite *)pNode)->setFlexibleHeightUp(pFloat);
    }
    else if(strcmp(pPropertyName, PROPERTY_FLEXIBLE_DOWN) == 0)
    {
		((PLAdaptiveScale9Sprite *)pNode)->setFlexibleHeightDown(pFloat);
    }
	else if (strcmp(pPropertyName, PROPERTY_FLEXIBLE_POSY) == 0)
	{
		((PLAdaptiveScale9Sprite *)pNode)->setFlexibleHeight(pFloat);
	}
	else
	{
        cocos2d::extension::CCScale9SpriteLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName,pFloat,pCCBReader);
    }
}

void PLAdaptiveScale9SpriteLoader::onHandlePropTypeSize(CCNode * pNode, CCNode * pParent, const char* pPropertyName, CCSize pSize, CCBReader * pCCBReader)
{
	if(strcmp(pPropertyName, PROPERTY_DESIGN_SIZE) == 0) {
	}
	else
	{
		cocos2d::extension::CCScale9SpriteLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName,pSize,pCCBReader);
	}
}