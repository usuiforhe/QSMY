//
//  SoulHunterDefeate.cpp
//  狩魂战斗失败界面
//
//  Created by liuxiaogang  on 13-10-28
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoulHunterDefeate.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define SNATCH_LOWEST_POWER 0

SoulHunterDefeate::SoulHunterDefeate()
:m_pBackButton(NULL)
,m_pAgainButton(NULL)
{
}


SoulHunterDefeate::~SoulHunterDefeate()
{
	CC_SAFE_RELEASE(m_pBackButton);
	CC_SAFE_RELEASE(m_pAgainButton);
}


SoulHunterDefeate*  SoulHunterDefeate::create()
{
	SoulHunterDefeate* pRet = new SoulHunterDefeate();
	if(pRet&& pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}


bool SoulHunterDefeate::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !DLayer::init());
		CCB_READER_AND_ADDCHILD("ui/ccb/soulHunter/soulHunterDefeat");
		bRet = true;
	}while(0);
    
	return bRet;
}


void SoulHunterDefeate::onEnter()
{
	BINDING_ONLY(DM_GET_SOUL_HUNTER_MODEL, this, SoulHunterDefeate::removeFromParent, D_EVENT_SOULHUNTER_NOT_EXIST);
	m_pBackButton->getButton()->setTarget(this, menu_selector(SoulHunterDefeate::closeCallBack));
	m_pAgainButton->getButton()->setTarget(this, menu_selector(SoulHunterDefeate::againCallBack));
	DLayer::onEnter();
}



void SoulHunterDefeate::onExit()
{
	UNBINDING_ALL(DM_GET_SOUL_HUNTER_MODEL, this);
	DLayer::onExit();
}


bool SoulHunterDefeate::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackButton", DButton *, this->m_pBackButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAgainButton", DButton *, this->m_pAgainButton);
	return false;
}


void SoulHunterDefeate::closeCallBack(cocos2d::CCObject* sender)
{
	removeFromParent();
}


void SoulHunterDefeate::againCallBack(cocos2d::CCObject* sender)
{
	soulhunter::SoulHunterInfo * info = DM_GET_SOUL_HUNTER_MODEL->getSoulHunterInfo();
	if(info->getPower() <= SNATCH_LOWEST_POWER)
	{
		TIPSMANAGER->warning(D_LOCAL_STRING("SoulHunterNotEnoughPower"));
	}
	else
	{
		DM_GET_SOUL_HUNTER_MODEL->doLastBattleRequest();
	}
	removeFromParent();
}

