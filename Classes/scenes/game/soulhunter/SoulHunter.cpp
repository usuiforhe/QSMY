
//
//  NavSoul.cpp
//  魂魄模块
//
//  Created by liuxiaogang on 13-10-18.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoulHunter.h"
#include "SoulHunterView.h"
#include "SoulHunterRivalView.h"
#include "utils/TimerUtil.h"

USING_NS_CC;


SoulHunter::SoulHunter()
:m_pContainer(NULL)
,m_pModel(NULL)
,m_pRemainChTimes(NULL)
,m_pHunter(NULL)
,m_pTargetWarrior(NULL)
,m_pPopupLayer(NULL)
,m_pChooseRivalViewIsShowing(false)
{
}


SoulHunter::~SoulHunter()
{
    removeAllChildren();
	CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pRemainChTimes);
	CC_SAFE_RELEASE(m_pHunter);
	CC_SAFE_RELEASE(m_pTargetWarrior);
	CC_SAFE_RELEASE(m_pPopupLayer);
	SAFE_RELEASE_UNBINDING_ALL(m_pModel, this);

}

SoulHunter* SoulHunter::create(NavigationDelegate* pDelegate)
{
	SoulHunter *pRet = new SoulHunter();
	if (pRet && pRet->init(pDelegate))
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		CC_SAFE_DELETE(pRet); 
		return NULL; 
	} 
}

bool SoulHunter::init(NavigationDelegate* pDelegate)
{
	if(!DLayer::init())
	{
		return false;
	}
    
    m_pModel = DM_GET_SOUL_HUNTER_MODEL;
    CC_SAFE_RETAIN(m_pModel);
    
	CCB_READER_AND_ADDCHILD("ui/ccb/soulHunter/soulHunter");

	m_pPopupLayer = CCNode::create();
	CC_SAFE_RETAIN(m_pPopupLayer);
	addChild(m_pPopupLayer,1);

    BINDING_EXEC(m_pModel, this, SoulHunter::updateView, D_EVENT_SOULHUNTER_UPDATED);
	BINDING_ONLY(m_pModel, this, SoulHunter::showHunterFinish, D_EVENT_SOULHUNTER_SNATCH_FINISH);
    
	showHunterView();
	
    return true;
}

void SoulHunter::onEnter()
{
	BINDING_ONLY(m_pModel, this, SoulHunter::showChooseRivalView, D_EVENT_SOULHUNTER_RIVALS_UPDATED);
    DLayer::onEnter();
    update(0.f);
    TimerUtil::sharedTimerUtil()->schedule(schedule_selector(SoulHunter::update), this, 600.f);
}

void SoulHunter::update(float dt)
{
    HTTP_CLIENT->huntGet();
}

void SoulHunter::onExit()
{
    TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
    DLayer::onExit();
}

void SoulHunter::changeRinalViewClosed(){
	m_pChooseRivalViewIsShowing = false;
}

void SoulHunter::sendRivalRequst(warrior::WarriorCurInfo * pWarrior){
	CC_SAFE_RELEASE_NULL(m_pTargetWarrior);
	m_pTargetWarrior = pWarrior;
	CC_SAFE_RETAIN(m_pTargetWarrior);

	if(m_pModel->cannotSnatchWarroir(pWarrior)){
        DM_GET_ERRORTIP_MODEL->showErrorTips(SOUL_HUNTER_NO_MORE_SNATCH);
		return;
	}

	m_pModel->seTargetWarrior(pWarrior);
	uint32_t wid = pWarrior->getWid();
	HTTP_CLIENT->huntTargets(wid);	//每次都会刷新对手
}

void SoulHunter::showChooseRivalView()
{
	if(m_pChooseRivalViewIsShowing)return;
	removeAllPopupLayer();

	m_pChooseRivalViewIsShowing = true;
    SoulHunterRivalView *pRival=SoulHunterRivalView::create();
    pRival->setData(m_pTargetWarrior);
	pRival->setDelegate(this);
	m_pPopupLayer->addChild(pRival);

}

void SoulHunter::updateView()
{
    m_pRemainChTimes->setString(CCString::createWithFormat("%d/%d",m_pModel->getSoulHunterInfo()->getPower(),m_pModel->getSoulHunterInfo()->getMaxPower())->getCString());
}


bool SoulHunter::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRemainChTimes", CCLabelBMFont *, this->m_pRemainChTimes);
	return false;
}


void SoulHunter::showHunterView()
{
	m_pContainer->setVisible(true);
	m_pContainer->removeAllChildren();
	removeAllPopupLayer();
	if(m_pHunter == NULL)
	{
		m_pHunter = SoulHunterView::create();
		m_pHunter->setTransferDelegate(this);
		CC_SAFE_RETAIN(m_pHunter);
        
	}
	m_pContainer->addChild(m_pHunter);
}


void SoulHunter::showHunterFinish()
{
	switch(m_pModel->getCurSoulHunterType())
	{
	case SOUL_HUNTER_TYPE_SUCCESS:
		showHunterSuccess();	
		break;
	case SOUL_HUNTER_TYPE_FAIL:
		showHunterFail();		
		break;
	case SOUL_HUNTER_TYPE_DEFEATE:
		showHunterDefeate();
		break;
	default:
		removeAllPopupLayer();
		break;
	}
}

void SoulHunter::removeAllPopupLayer()
{
	m_pChooseRivalViewIsShowing = false;
	m_pPopupLayer->removeAllChildren();
}


void SoulHunter::showHunterSuccess()
{
	removeAllPopupLayer();
	SoulHunterSuccess *pSuccess=SoulHunterSuccess::create();
	m_pPopupLayer->addChild(pSuccess);
}


void SoulHunter::showHunterFail()
{
	removeAllPopupLayer();
	SoulHunterFail *pFail=SoulHunterFail::create();
	m_pPopupLayer->addChild(pFail);
}

void SoulHunter::showHunterDefeate()
{
	removeAllPopupLayer();
	SoulHunterDefeate *pDefeate=SoulHunterDefeate::create();
	m_pPopupLayer->addChild(pDefeate);
}

void SoulHunter::onBackKey()
{
	if (m_pPopupLayer && m_pPopupLayer->getChildrenCount())
	{
		removeAllPopupLayer();
		return;
	}
	BackKeyInterface::onBackKey();
}