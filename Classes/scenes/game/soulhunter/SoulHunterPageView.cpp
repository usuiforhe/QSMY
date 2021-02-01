//
//  SoulHunterPageView.cpp
//  狩魂一页6个弟子的视图
//
//  Created by liuxiaogang on 13-10-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoulHunterPageView.h"
#include "SoulHunterWarriorView.h"

#define SNATCH_LOWEST_POWER 0
USING_NS_CC;
USING_NS_CC_EXT;

SoulHunterPageView::SoulHunterPageView()
:m_pData(NULL)
,m_pDelegate(NULL)
{
    D_RESET_C_ARRAY(m_pSoulHunterWarriorViews,HEAD_COUNT);
}


SoulHunterPageView::~SoulHunterPageView()
{
    D_SAFE_RELEASE_C_ARRAY(m_pSoulHunterWarriorViews,HEAD_COUNT);
    CC_SAFE_RELEASE(m_pData);
}

SoulHunterPageView* SoulHunterPageView::create()
{
	SoulHunterPageView* pRet = new SoulHunterPageView();
	if(pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}

bool SoulHunterPageView::init()
{
	if(!CCNode::init())
	{
		return false;
	}
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("SoulHunterWarriorView", SoulHunterWarriorViewLoader::loader());
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/soulHunter/soulHunterPage",pNodeLoaderLibrary);

	return true; 
}





bool SoulHunterPageView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "SoulHunterNode1",  SoulHunterWarriorView*, this->m_pSoulHunterWarriorViews[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "SoulHunterNode2",  SoulHunterWarriorView*, this->m_pSoulHunterWarriorViews[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "SoulHunterNode3", SoulHunterWarriorView *, this->m_pSoulHunterWarriorViews[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "SoulHunterNode4", SoulHunterWarriorView*, this->m_pSoulHunterWarriorViews[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "SoulHunterNode5", SoulHunterWarriorView *, this->m_pSoulHunterWarriorViews[4]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "SoulHunterNode6", SoulHunterWarriorView *, this->m_pSoulHunterWarriorViews[5]);
    
    return false;
}

void SoulHunterPageView::setData(cocos2d::CCArray* pData)
{
    CC_SAFE_RELEASE(m_pData);
    CC_SAFE_RETAIN(pData);
    m_pData = pData;
    
    for (uint8_t i(0); i<HEAD_COUNT; ++i) {
        if(i>=pData->count())
        {
            m_pSoulHunterWarriorViews[i]->setData(NULL);
        }else
        {
            m_pSoulHunterWarriorViews[i]->setData(static_cast<warrior::WarriorCurInfo *>(pData->objectAtIndex(i)));

            m_pSoulHunterWarriorViews[i]->getSnatchMenuItem()->setTarget(this, menu_selector(SoulHunterPageView::SnatchCallBack));
            m_pSoulHunterWarriorViews[i]->getSnatchMenuItem()->setTag(i);
        }
    }
}


void SoulHunterPageView::SnatchCallBack(cocos2d::CCObject* sender)
{
	soulhunter::SoulHunterInfo * info = DM_GET_SOUL_HUNTER_MODEL->getSoulHunterInfo();
	if(info->getPower() <= SNATCH_LOWEST_POWER)
	{
		TIPSMANAGER->warning(D_LOCAL_STRING("SoulHunterNotEnoughPower"));
		return;
	}

    CCMenuItem* pItem = static_cast<CCMenuItem*>(sender);
    int tag = pItem->getTag();
    warrior::WarriorCurInfo* pWarrior = static_cast<warrior::WarriorCurInfo *>(m_pData->objectAtIndex(tag));
    if(m_pDelegate) getDelegate()->sendRivalRequst(pWarrior);
}
