//
//  Match.cpp
//  杯赛模块
//
//  Created by cll on 13-4-16.
//
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Match.h"
#include "MatchView.h"
#include "MatchGuessView.h"
#include "MatchRuleView.h"
#include "MatchBetView.h"
#include "managers/GameManager.h"
#include "components/ccbloaders/DSpriteLoader.h"
#include "utils/TimerUtil.h"
#include "../Game.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  MATCH_TAB_RECT    CCRectMake(0, 0, 620, 820)


/************************ match class begin*****************************/

Match::Match()
:m_pTabView(NULL)
,m_pMatchModel(NULL)
,m_pTabContainer(NULL)
,m_pPopupLayer(NULL)
,isFirstEnter(true)
,m_pContainerSize(NULL)
{
    
}


Match::~Match()
{
    removeAllChildren();
	CC_SAFE_RELEASE(m_pTabView);
    CC_SAFE_RELEASE(m_pTabContainer);
    CC_SAFE_RELEASE(m_pPopupLayer);
	CC_SAFE_RELEASE(m_pContainerSize);
}


bool Match::init()
{
	if(!DLayer::init())
	{
		return false;
	}
    
	m_pMatchModel = DM_GET_MATCH_MODEL;

    
    
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beisai/PL_beisai_saichang");
    //CCB_READER_AND_ADDCHILD("ui/ccb/match/match");
    
    m_pPopupLayer = CCNode::create();
    CC_SAFE_RETAIN(m_pPopupLayer);
    addChild(m_pPopupLayer);
	//popup自适应
	PL_MOVE_NODE_BELOW_CENTER_Y(m_pPopupLayer);

    //m_pTabView = DTabView::create(MATCH_TAB_RECT,350);

	float posY = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainerSize->getPosition().y, PL_MAX_HEIGHT_DIFF / 2);
	float height = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainerSize->getContentSize().height + 18, -PL_MAX_HEIGHT_DIFF);

	m_pTabView = DTabView::create(CCRectMake(m_pContainerSize->getPosition().x, m_pContainerSize->getPosition().y, m_pContainerSize->getContentSize().width, height),50);
	//m_pTabView->setPadding(15.0);

	m_pTabView->retain();
    m_pTabContainer->addChild(m_pTabView);
    
	MatchView * matchView = MatchView::create(this);
	//m_pTabView->addTabWithFontImg(matchView,"PL_ui/beisai/pl_bs_font_saichang.png");
	m_pTabView->addTab(matchView,"","PL_ui/all/all_btn_fenye_lan.png","PL_ui/all/all_btn_fenye_lv.png",NULL,"PL_ui/beisai/pl_bs_font_saichang.png"); 
	MatchGuessListView * guessView = MatchGuessListView::create(this);
    //m_pTabView->addTabWithFontImg(guessView,"PL_ui/beisai/pl_bs_font_jingcai.png");
    m_pTabView->addTab(guessView,"","PL_ui/all/all_btn_fenye_lan.png","PL_ui/all/all_btn_fenye_lv.png",NULL,"PL_ui/beisai/pl_bs_font_jingcai.png"); 
	return true;
}


void Match::showMatchBetView(uint32_t finalNum,uint32_t mid)
{
    m_pPopupLayer->removeAllChildren();
	MatchBetView* pBetView = MatchBetView::create();
	pBetView->setVSInfo(m_pMatchModel->getVSInfo(finalNum,mid));
    m_pPopupLayer->addChild(pBetView);
}

void Match::showMatchRule()
{
    m_pPopupLayer->removeAllChildren();
    MatchRuleView* pRuleView = MatchRuleView::create();
	pRuleView->setDelegate(this);
	m_pPopupLayer->addChild(pRuleView);
}


void Match::signUpCup()
{
    if(GameManager::sharedGameManager()->getServerTime()<TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),m_pMatchModel->getMatchConfig()->m_nSignStartTime))
    {
        Tips_Alert(D_LOCAL_STRING("EnrollTips").c_str());
        return;
    }
    DMessageBox::showMessageBox(D_LOCAL_STRING("matchTips").c_str(), this, messageBox_callfunc_selector(Match::doSignUpCup));
}

void Match::doSignUpCup(MessageType type,DMessageBox*)
{
    if(type == kMessageTypeOk)
    {
        HTTP_CLIENT->signUpCup();
    }
    
}

void Match::onEnter()
{
    DLayer::onEnter();
    BINDING_ONLY(m_pMatchModel, this, Match::updateView, D_EVENT_MATCHINFO_UPDATE);
    BINDING_ONLY(m_pMatchModel, this, Match::updateView, D_EVENT_MATCHLIST_UPDATE);
    updateView();
}


void Match::onExit()
{
    DLayer::onExit();
    TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(Match::intervalCallBack), this);
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(Match::refreshCallBack), this);
    UNBINDING_ALL(m_pMatchModel, this);
}


void Match::checkUpdate()
{
    TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(Match::intervalCallBack), this);
    int t = m_pMatchModel->checkTime();
	CCLOG("update:%d",t);
    if(t==0)//立即刷新
    {
        intervalCallBack(0.f);
    }
    else if(t>0) //倒计时刷新
    {
        TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(Match::intervalCallBack), this, t + 1);
    }else
    {
        //t<0 没动作
    }

	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(Match::refreshCallBack), this);
	t = m_pMatchModel->refreshTime();
	CCLOG("refresh:%d",t);
	if(t==0)//立即刷新
	{
		refreshCallBack(0.f);
	}
	else if(t>0) //倒计时刷新
	{
		TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(Match::refreshCallBack), this, t + 1);
	}else
	{
		//t<0 没动作
	}
}


void Match::intervalCallBack(float dt)
{
	m_pMatchModel->isSynced = false;
	HTTP_CLIENT->initCup();
}

void Match::refreshCallBack(float dt)
{
	m_pMatchModel->refreshWatchedMatch();
}

void Match::updateView()
{
    checkUpdate();
    if(!m_pMatchModel->isSynced) return;
    if(isFirstEnter && !m_pMatchModel->getIsSigned() && m_pMatchModel->getMatchInfoCount()==0)
    {
        isFirstEnter = false;
        showMatchRule();
    }
}


bool Match::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTabContainer", CCNode *, this->m_pTabContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainerSize", CCNode *, this->m_pContainerSize);
    return false;
}

SEL_MenuHandler Match::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "competeCallBack", Match::signUpCup);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "ruleCallBack", Match::menuCallBack);
    return NULL;
}


void Match::menuCallBack(CCObject * obj)
{
	showMatchRule();
}


void Match::onBackKey()
{
	if (m_pPopupLayer && m_pPopupLayer->getChildrenCount())
	{
		m_pPopupLayer->removeAllChildren();
		return;
	}
	
	BackKeyInterface::onBackKey();
}