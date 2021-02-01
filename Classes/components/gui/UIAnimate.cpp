//
//  UIAnimate.cpp
//  QSMY
//
//  Created by wanghejun on 13-12-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "UIAnimate.h"
#include "managers/ResourceManager.h"
#include "utils/OpacityUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace sp;

#define PROPERTY_NAME "animateName"
#define PROPERTY_AUTO "autoPlay"
#define PROPERTY_BATCH "usingBatchNode"
#define PROPERTY_ZORDER "maxZOrder"



UIAnimate::UIAnimate()
	: m_pArmature(NULL)
	, m_bAutoPlay(true)
	, m_pDelegate(NULL)
	, m_bRunningAnimation(false)
	, m_bUsingBatchNode(true)
	, m_bMaxZOrder(false)
{
    
}

UIAnimate::~UIAnimate()
{
	if (m_pArmature)
	{
		m_pArmature->getAnimation()->removeObserver(this, COMPLETE);
	}
}

void UIAnimate::setOpacity(GLubyte opacity)
{
    cocos2d::CCNodeRGBA::setOpacity(opacity);
    ::setOpacity(this, opacity);
}
// 
// void UIAnimate::setAnimateName(const char* name)
// {
//     removeAllChildren();
// 	m_pArmature = ResourceManager::sharedResourceManager()->getUIAnimateOnly(name, sp::ZORDER);
//     addChild(m_pArmature);
// 	m_pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(UIAnimate::animationComplete), COMPLETE);
// 	if (m_bAutoPlay)
// 	{
// 		playAtIndex(0);
// 	}
// 	
// }

void UIAnimate::playAtIndex(uint32_t idx)
{
	if (m_pArmature)
	{
		m_pArmature->getAnimation()->playByIndex(idx, -1, -1, true);
		m_bRunningAnimation = true;
	}
}

void UIAnimate::playWithName(const char *name)
{
	if (m_pArmature)
	{
		m_pArmature->getAnimation()->play(name, -1, -1, true);
		m_bRunningAnimation = true;
	}
}

void UIAnimate::playRandomly()
{
	if (m_pArmature)
	{
		int count = m_pArmature->getAnimation()->getMovementCount();
		int idx = count * CCRANDOM_0_1();
		m_pArmature->getAnimation()->playByIndex(idx, -1, -1, true);
		m_bRunningAnimation = true;
	}
	
}

bool UIAnimate::isRunningAnimation()
{
	return m_bRunningAnimation;
}

void UIAnimate::animationComplete(sp::Armature * target, const char* event,const char* data)
{
	m_bRunningAnimation = false;
	if (m_pDelegate)
	{
		m_pDelegate->animationComplete(this);
	}
	
}

// void UIAnimate::setAutoPlay(bool autoPlay)
// {
// 	m_bAutoPlay = autoPlay;
// 	if (m_bAutoPlay)
// 	{
// 		playAtIndex(0);
// 	}
// 	else if (m_bRunningAnimation)
// 	{
// 		m_pArmature->getAnimation()->stop();
// 		m_bRunningAnimation = false;
// 	}
// }

void UIAnimate::replaceNodeInBone(CCNode *pNode, const char *pBoneName, uint32_t idx /* = 0 */)
{
	CCAssert(!m_bUsingBatchNode, "Animation should not use batch node. Edit CCB file!!!");
	m_pArmature->getBone(pBoneName)->replaceDisplayAt(pNode, idx);
}

void UIAnimate::stop()
{
	if (m_pArmature)
	{
		m_pArmature->getAnimation()->stop();
	}
}

void UIAnimate::playStill()
{
	if (m_pArmature)
	{
		m_pArmature->getAnimation()->playByIndex(0, 0, 0, true);
	}
}

void UIAnimate::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{
	sp::RENDER_TYPE eType = sp::ZORDER;
	if (m_bUsingBatchNode && m_bMaxZOrder)
	{
		eType = sp::BATCHNODE_VERTEXZ;
	}
	else if (m_bUsingBatchNode)
	{
		eType = sp::BATCHNODE_ZORDER;
	}
	else if (m_bMaxZOrder)
	{
		eType = sp::RENDER_MAX;
	}

	
	m_pArmature = ResourceManager::sharedResourceManager()->getUIAnimateOnly(m_sAnimateName.c_str(), eType);
	addChild(m_pArmature);
	m_pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(UIAnimate::animationComplete), COMPLETE);
	if (m_bAutoPlay)
	{
		playAtIndex(0);
	}

}

void UIAnimateLoader::onHandlePropTypeString(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, const char * pString, cocos2d::extension::CCBReader * pCCBReader)
{
    if(strcmp(pPropertyName, PROPERTY_NAME) == 0) {
        //((UIAnimate *)pNode)->setAnimateName(pString
		((UIAnimate *)pNode)->m_sAnimateName = pString;
    }
	else
	{
        cocos2d::extension::CCNodeLoader::onHandlePropTypeString(pNode, pParent, pPropertyName,pString,pCCBReader);
    }
}

void UIAnimateLoader::onHandlePropTypeCheck(CCNode * pNode, CCNode * pParent, const char* pPropertyName, bool pCheck, CCBReader * pCCBReader)
{
	if (strcmp(pPropertyName, PROPERTY_AUTO) == 0)
	{
		//((UIAnimate *)pNode)->setAutoPlay(pCheck);
		((UIAnimate *)pNode)->m_bAutoPlay = pCheck;
	}
	else if (strcmp(pPropertyName, PROPERTY_BATCH) == 0)
	{
		((UIAnimate *)pNode)->m_bUsingBatchNode = pCheck;
	}
	else if (strcmp(pPropertyName, PROPERTY_ZORDER) == 0)
	{
		((UIAnimate *)pNode)->m_bMaxZOrder = pCheck;
	}
	else
	{
		cocos2d::extension::CCNodeLoader::onHandlePropTypeCheck(pNode, pParent, pPropertyName,pCheck,pCCBReader);
	}
}