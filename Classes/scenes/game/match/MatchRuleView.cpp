//
//  MatchRuleView.cpp
//  比赛规则
//
//  Created by cll on 13-4-16
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MatchRuleView.h"
#include "Match.h"
#include "utils/TimerUtil.h"
#include "utils/TimeUtil.h"
#include "managers/GameManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

/************************ match rule view begin*******************/


MatchRuleView::MatchRuleView()
:m_pMatchModel(NULL)
,m_pDelegate(NULL)
,m_pSignMenu(NULL)
,m_pPage(NULL)
,m_pTimeLabel(NULL)
,m_pTime(NULL)
,m_pVipLabel(NULL)
,m_fTimer(0.f)
,m_pCloseButton(NULL)
,m_pCompete(NULL)
,m_pCompeted(NULL)
,m_pCompeteMenu(NULL)
{
    
}

MatchRuleView::~MatchRuleView()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pSignMenu);
    CC_SAFE_RELEASE(m_pPage);
    CC_SAFE_RELEASE(m_pTime);
	CC_SAFE_RELEASE(m_pVipLabel);
    CC_SAFE_RELEASE(m_pTimeLabel);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pCompete);
	CC_SAFE_RELEASE(m_pCompeted);
	CC_SAFE_RELEASE(m_pCompeteMenu);
}


bool MatchRuleView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

    m_pMatchModel = DM_GET_MATCH_MODEL;
    //CCB_READER_AND_ADDCHILD("ui/ccb/match/rule");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beisai/PL_beisai_guize");
    //m_pTime->setVisible(false);
	//m_pVipLabel->setVisible(false);
	m_pCloseButton->getButton()->setTarget(this, menu_selector(MatchRuleView::closeCallBack));

	m_pPage->setViewSize(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pPage->getViewSize(), -PL_MAX_HEIGHT_DIFF));

	return true;

}


SEL_MenuHandler MatchRuleView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", MatchRuleView::closeCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "signCallBack", MatchRuleView::signCallBack);
    return NULL;
}


bool MatchRuleView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSignMenu", CCMenuItem*, this->m_pSignMenu);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPage", CCScrollView*, this->m_pPage);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTime", CCNode*, this->m_pTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipLabel", CCNode*, this->m_pVipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTimeLabel", CCLabelBMFont*, this->m_pTimeLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton*, this->m_pCloseButton);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCompete", CCSprite *, m_pCompete);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCompeted", CCSprite *, m_pCompeted);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCompeteMenu", CCMenuItemImage *, m_pCompeteMenu);
    
    return false;
}

void MatchRuleView::closeCallBack(CCObject * obj)
{
	removeFromParent();
}

void MatchRuleView::signCallBack(cocos2d::CCObject *obj)
{
    if(m_pDelegate)
    {
        m_pDelegate->signUpCup();
    }
}


void MatchRuleView::onEnter()
{
    DLayer::onEnter();
    m_pPage->setContentOffset(m_pPage->minContainerOffset());
    BINDING_ONLY(m_pMatchModel, this, MatchRuleView::updateView, D_EVENT_MATCHINFO_UPDATE);
    updateView();
}

void MatchRuleView::onExit()
{
    DLayer::onExit();
    TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
    UNBINDING_ALL(m_pMatchModel, this);
}

void MatchRuleView::update(float dt)
{
    m_fTimer-=dt;
    if(m_fTimer<0.f) m_fTimer=0.f;
    m_pTimeLabel->setString(TimeUtil::getSeconds2String(m_fTimer));
}


void MatchRuleView::updateView()
{
	if(!m_pMatchModel->isSynced) return;

	m_pCompete->setVisible(!m_pMatchModel->getIsSigned());
	m_pCompeted->setVisible(m_pMatchModel->getIsSigned());
	if(m_pMatchModel->getIsSigned()){
		m_pCompeteMenu->setShaderProgram(D_SHADER_GARY);
		m_pCompeteMenu->setEnabled(false);
	}

    /*if(!m_pMatchModel->isSynced) return;
    m_pSignMenu->setEnabled(!m_pMatchModel->getIsSigned());
    m_pTime->setVisible(m_pMatchModel->getIsSigned());
	m_pVipLabel->setVisible(!m_pMatchModel->getIsSigned());
    if(m_pTime->isVisible())
    {
        m_fTimer = TimeUtil::getDiff(GameManager::sharedGameManager()->getServerTime(),
                                     TimeUtil::getTimeToday(TimeUtil::getStartTimeOfTomorrow(GameManager::sharedGameManager()->getServerTime()),8));
        TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
        TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
        update(0.f);
    }*/
}

/************************ match rule view end*******************/



