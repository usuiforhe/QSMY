//
//  GuildBossView.h
//  公会主页
//
//  Created by Eci on 14-01-26.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossView.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBossPoint.h"
#include "GuildBoss.h"
#include "network/HttpHandler.h"

using namespace sp;

GuildBossView::GuildBossView()
	: m_pDelegate(NULL)
	, m_pBtnDeclare(NULL)
	, m_pDeclare(NULL)
	, m_pBtnRule(NULL)
	, m_pTTFName(NULL)
	, m_pBMFTime(NULL)
	, m_pNodeDay(NULL)
	, m_pBtnSettle(NULL)
	, m_pBtnPrev(NULL)
	, m_pBtnCurr(NULL)
	, m_pClose(NULL)
	, m_pOpen(NULL)
	, m_pNodeDragon(NULL)
	, m_pNodeFight(NULL)
	, m_pNodeDragonBg(NULL)
	, m_pNodeFightBg(NULL)
	, m_pNodeWaiting(NULL)
	, m_pNodeRunning(NULL)
	, m_pBMFRunningTime(NULL)
	, m_pBMFCurrTime(NULL)
	, m_pBMFNextTime(NULL)
	, m_pNodeTime(NULL)
	, m_pNodeToday(NULL)
	, m_pNodeYesterday(NULL)
	, m_pModel(NULL)
	, m_pBoss(NULL)
	, m_fWaitTime(0.f)
	, m_eLastState(guildboss::GuildBossStateClose)
	, m_pBtnGuildInfo(NULL)
	, m_pBtnPersonInfo(NULL)
{
	D_RESET_C_ARRAY(m_pBtnPoint, MAX_GUILD_BOSS_POINT);
	D_RESET_C_ARRAY(m_pBtnWeek, MAX_GUILD_BOSS_POINT);
	D_RESET_C_ARRAY(m_pNodeWeek, MAX_GUILD_BOSS_POINT);
}

GuildBossView::~GuildBossView()
{
	D_SAFE_RELEASE_C_ARRAY(m_pBtnPoint, MAX_GUILD_BOSS_POINT);
	CC_SAFE_RELEASE(m_pBtnDeclare);
	CC_SAFE_RELEASE(m_pDeclare);
	CC_SAFE_RELEASE(m_pBtnRule);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pBMFTime);
	CC_SAFE_RELEASE(m_pNodeDay);
	CC_SAFE_RELEASE(m_pBtnSettle);
	CC_SAFE_RELEASE(m_pBtnPrev);
	CC_SAFE_RELEASE(m_pBtnCurr);
	CC_SAFE_RELEASE(m_pOpen);
	CC_SAFE_RELEASE(m_pClose);
	CC_SAFE_RELEASE(m_pNodeDragon);
	CC_SAFE_RELEASE(m_pNodeFight);
	CC_SAFE_RELEASE(m_pNodeDragonBg);
	CC_SAFE_RELEASE(m_pNodeFightBg);
	CC_SAFE_RELEASE(m_pNodeWaiting);
	CC_SAFE_RELEASE(m_pNodeRunning);
	CC_SAFE_RELEASE(m_pBMFRunningTime);
	CC_SAFE_RELEASE(m_pBMFCurrTime);
	CC_SAFE_RELEASE(m_pBMFNextTime);
	CC_SAFE_RELEASE(m_pNodeTime);
	CC_SAFE_RELEASE(m_pNodeToday);
	CC_SAFE_RELEASE(m_pNodeYesterday);
	D_SAFE_RELEASE_C_ARRAY(m_pBtnWeek, MAX_GUILD_BOSS_POINT);
	D_SAFE_RELEASE_C_ARRAY(m_pNodeWeek, MAX_GUILD_BOSS_POINT);
	CC_SAFE_RELEASE(m_pBtnGuildInfo);
	CC_SAFE_RELEASE(m_pBtnPersonInfo);
}

GuildBossView* GuildBossView::create()
{
	GuildBossView *pRet = new GuildBossView();
	if (pRet && pRet->init())
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

bool GuildBossView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILDBOSS_MODEL;
	m_pBoss = DM_GET_GUILDBOSS_MODEL->getBoss();

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("GuildBossMenu", GuildBossMenuLoader::loader());
	//CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/guildBoss/guildBoss", pNodeLoaderLibrary);
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_banghui/pl_banghui_jianghu", pNodeLoaderLibrary);

	m_pBtnDeclare->setTarget(this, menu_selector(GuildBossView::callBackTarget));
	m_pBtnRule->getButton()->setTarget(this, menu_selector(GuildBossView::callBackRule));
	m_pTTFName->setString("");
	//m_pNodeDay->setVisible(false);

	m_pBtnSettle->getButton()->setTarget(this, menu_selector(GuildBossView::callBackSettle));
	m_pBtnPrev->getButton()->setTarget(this, menu_selector(GuildBossView::callBackPrev));
	m_pBtnCurr->setTarget(this, menu_selector(GuildBossView::callBackCurr));

	//m_pNodeRunning->setVisible(false);
	//m_pNodeYesterday->setVisible(false);
	
	m_pBtnGuildInfo->getButton()->setTarget(this, menu_selector(GuildBossView::callBackRank));
	m_pBtnPersonInfo->getButton()->setTarget(this, menu_selector(GuildBossView::callBackContribution));

	for (int i = 0; i < MAX_GUILD_BOSS_POINT; i++)
	{
		//m_pNodeWeek[i]->setVisible(false);
		m_pBtnWeek[i]->setTarget(this, menu_selector(GuildBossView::callBackWeek));
		m_pBtnWeek[i]->setTag(i);
	}

	return true;
}

void GuildBossView::onEnter()
{
	BINDING_EXEC(m_pModel, this, GuildBossView::updateView, D_EVENT_GUILDBOSS_REFRESH);
	BINDING_ONLY(DM_GET_GUILD_MODEL, this, GuildBossView::updateView, D_EVENT_GUILD_GET);
	DLayer::onEnter();
}

void GuildBossView::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
	UNBINDING_ALL(DM_GET_GUILD_MODEL, this);
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
}

bool GuildBossView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	D_CCB_ASSIGN_ARRAY("m_pBtnPoint", CCMenuItem *, m_pBtnPoint, MAX_GUILD_BOSS_POINT);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnDeclare", CCMenuItemImage *, m_pBtnDeclare);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDeclare", CCNode *, m_pDeclare);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRule", DButton *, m_pBtnRule);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFTime", CCLabelBMFont *, m_pBMFTime);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDay", CCNode *, m_pNodeDay);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnSettle", DButton *, m_pBtnSettle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnPrev", DButton *, m_pBtnPrev);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnCurr", CCMenuItem *, m_pBtnCurr);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOpen", CCNode *, m_pOpen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pClose", CCNode *, m_pClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDragon", CCNode *, m_pNodeDragon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFight", CCNode *, m_pNodeFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDragonBg", CCNode *, m_pNodeDragonBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFightBg", CCNode *, m_pNodeFightBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeWaiting", CCNode *, m_pNodeWaiting);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRunning", CCNode *, m_pNodeRunning);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRunningTime", CCLabelBMFont *, m_pBMFRunningTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCurrTime", CCLabelBMFont *, m_pBMFCurrTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFNextTime", CCLabelBMFont *, m_pBMFNextTime);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeTime", CCNode *, m_pNodeTime);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeToday", CCNode *, m_pNodeToday);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeYesterday", CCNode *, m_pNodeYesterday);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pBtnWeek", CCMenuItem *, m_pBtnWeek, MAX_GUILD_BOSS_POINT);
	//D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pNodeWeek", CCNode *, m_pNodeWeek, MAX_GUILD_BOSS_POINT);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnPersonInfo", DButton *, m_pBtnPersonInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnGuildInfo", DButton *, m_pBtnGuildInfo);
	
	return false;
}

cocos2d::SEL_MenuHandler GuildBossView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackRank", GuildBossView::callBackRank);
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackContribution", GuildBossView::callBackContribution);
	return NULL;
}


void GuildBossView::setDelegate(GuildBossViewDelegate *pDelegate)
{
	m_pDelegate = pDelegate;
}

void GuildBossView::callBackTarget(CCObject *pSender)
{
	if (!m_pModel->checkCanDeclare())
	{
		//CCLOG(m_pModel->getConfig()->getDeclareError().c_str());
		Tips_Alert(m_pModel->getConfig()->getDeclareError().c_str());
		return;
	}
	GuildModel *pGuild = DM_GET_GUILD_MODEL;
	guild::GuildPosition ePos = pGuild->getSelfInfo()->getPosition();
	if (ePos != guild::GuildPositionAssistant && ePos != guild::GuildPositionPresident)
	{
		Tips_Alert(D_LOCAL_STRING("GuildErrorPermissionDenied").c_str());
		return;
	}
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavTarget);
	}
}

void GuildBossView::callBackRule(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavRule);
	}
}

void GuildBossView::callBackRank(CCObject *pSender)
{
	if (!m_pModel->canShowRank())
	{
		Tips_Alert(m_pModel->getConfig()->getRankError().c_str());
		return;
	}
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavRankGuild);
	}
}

void GuildBossView::callBackContribution(CCObject *pSender)
{
	if (!m_pModel->canShowRank())
	{
		Tips_Alert(m_pModel->getConfig()->getRankError().c_str());
		return;
	}
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavRankContribution);
	}
}

void GuildBossView::updateView()
{
	//默认（未开启时），隐藏倒计时
	m_pNodeWaiting->setVisible(false);

	if (!DM_GET_GUILD_MODEL->getGuildInfo()->getGuildID())
	{
		return;
	}
	if (DM_GET_GUILD_MODEL->getGuildInfo()->getIsCreatedToday())
	{
		return;
	}
	if (!m_pModel->getIsOpen())
	{
		uint32_t openTs = m_pModel->getOpenTs() - TimeUtil::getStartTimeOfToday(GameManager::sharedGameManager()->getServerTime());
		int dayCount = atoi(TimeUtil::getSeconds2String(openTs, "%D"));
		m_pBMFTime->setString(D_CSTRING_FROM_INT(dayCount));
		//m_pNodeDay->setVisible(true);
		m_pBtnPrev->getParent()->setVisible(false);
		return;
	}
	m_pBtnPrev->getParent()->setVisible(true);
	//m_pNodeDay->setVisible(false);
	m_fWaitTime = m_pModel->getCountingDownOfLivingPoint();
	if (m_fWaitTime > 0)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossView::updateDeadLine), this);
		TimerUtil::sharedTimerUtil()->scheduleSelector(schedule_selector(GuildBossView::updateDeadLine), this, 1,false);
	}
	m_eLastState = m_pModel->getStateOfLivingPoint();
	guildboss::GuildBossState eState = m_pModel->getStateOfLivingPoint();
	if (eState != guildboss::GuildBossStateClose)
	{
		TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
		TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
	}
	if (m_pModel->getBoss()->getIsDeclaredToday() || eState != guildboss::GuildBossStateClose)
	{
		m_pTTFName->setString(m_pBoss->getTargetName().c_str());
	}
	m_pDeclare->setVisible(!m_pBoss->getIsDeclaredToday());
	updatePoints();
	if (m_fWaitTime < 0.f)
	{
		m_pBMFTime->setString("00:00:00");
	}
	else
	{
		m_pBMFTime->setString(TimeUtil::getSeconds2String(m_fWaitTime, "%H:%M:%S"));
	}
	m_pNodeWaiting->setVisible(m_fWaitTime > 0.f);
	//m_pNodeRunning->setVisible(m_fWaitTime <= 0.f);

	uint32_t uServerTime = GameManager::sharedGameManager()->getServerTime();
	float fStartCountDown = -1;
	if (m_pModel->getConfig()->getStartTime() > uServerTime)
	{
		fStartCountDown = m_pModel->getConfig()->getStartTime() - uServerTime;
	}
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossView::updateStart), this);
	if (fStartCountDown >= 0.f)
	{
		TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(GuildBossView::updateStart), this, fStartCountDown + 1);
	}

	m_pNodeDragon->setVisible(eState != guildboss::GuildBossStateRob);
	m_pNodeFight->setVisible(eState == guildboss::GuildBossStateRob);
	m_pNodeDragonBg->setVisible(eState != guildboss::GuildBossStateRob);
	m_pNodeFightBg->setVisible(eState == guildboss::GuildBossStateRob);

	m_pBtnCurr->setEnabled(eState == guildboss::GuildBossStateBoss || eState == guildboss::GuildBossStateRob);
	m_pOpen->setVisible(m_pBtnCurr->isEnabled());
	m_pClose->setVisible(!m_pBtnCurr->isEnabled());

 	if (eState == guildboss::GuildBossStateRob)
 	{
		m_pBMFCurrTime->setString(m_pModel->getConfig()->getRobPeriod().c_str());
		m_pBMFNextTime->setString(m_pModel->getConfig()->getBossPeriod().c_str());
 	}
	else
	{
		m_pBMFCurrTime->setString(m_pModel->getConfig()->getBossPeriod().c_str());
		m_pBMFNextTime->setString(m_pModel->getConfig()->getRobPeriod().c_str());
	}
	//m_pNodeTime->setVisible(guildboss::GuildBossStateBoss == eState || guildboss::GuildBossStateRob == eState);

	uint32_t uNow = GameManager::sharedGameManager()->getServerTime();
	//m_pNodeToday->setVisible(uNow > m_pModel->getConfig()->getEndTime());
	//m_pNodeYesterday->setVisible(!m_pNodeToday->isVisible());
}

void GuildBossView::update(float dt)
{
	guildboss::GuildBossState eState = m_pModel->getStateOfLivingPoint();
	if (eState == guildboss::GuildBossStateClose)
	{
		TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
	}
	if (m_pModel->getStateOfLivingPoint() != m_eLastState)
	{
		m_pModel->resetStageData();
		updateView();
	}
	//m_pBMFRunningTime->setString(TimeUtil::getSeconds2String(m_pModel->getCountingDownOfRob()));
	updatePoints();
}

void GuildBossView::updateDeadLine(float dt)
{
	m_fWaitTime -= dt;
	if (m_fWaitTime < 0)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossView::updateDeadLine), this);
		updateView();
	}
	else if (m_pModel->getConfig()->getApplyDeadline() - GameManager::sharedGameManager()->getServerTime() <= 0)
	{
		m_pBoss->setIsDeclaredToday(true);
		updateView();
	}
	else
	{
		m_pBMFTime->setString(TimeUtil::getSeconds2String(m_fWaitTime, "%H:%M:%S"));
	}
}

void GuildBossView::updatePoints()
{
	int idx = m_pModel->getIndexOfLivingPoint();
	if (idx < 0)
	{
		return;
	}
	for (int i = 0; i < MAX_GUILD_BOSS_POINT; i++)
	{
		m_pBtnPoint[i]->setEnabled(i >= idx);
		if (i == idx)
		{
			m_pBtnPoint[i]->selected();
		}
		else if (i > idx)
		{
			m_pBtnPoint[i]->unselected();
		}
	}
	//m_pNodeTime->setPosition(ccp(m_pBtnPoint[idx]->getPosition().x, m_pBtnPoint[idx]->getPosition().y - m_pBtnPoint[idx]->getContentSize().height / 2 - 10));
}

void GuildBossView::updateStart(float dt)
{
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossView::updateStart), this);
	HTTP_CLIENT->guildBossGet();
}

void GuildBossView::callBackSettle(CCObject *pSender)
{
	if (!m_pModel->getHasSettleData())
	{
		Tips_Error(D_LOCAL_STRING("GuildBossNoData"));
		return;
	}
	if (!m_pModel->isSettleSynced())
	{
		HTTP_CLIENT->guildBossDailySettle(this, callfuncJson_selector(GuildBossView::callBackSettle));
		return;
	}
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavSettle);
	}
}

void GuildBossView::callBackPrev(CCObject *pSender)
{
	guildboss::GuildBossState eState = m_pModel->getStateOfLivingPoint();
	if (eState != guildboss::GuildBossStateRob)
	{
		if (!m_pModel->getHasSettleData())
		{
			Tips_Error(D_LOCAL_STRING("GuildBossNoData"));
			return;
		}
		if (!m_pModel->isSettleSynced())
		{
			HTTP_CLIENT->guildBossDailySettle(this, callfuncJson_selector(GuildBossView::callBackRobSettle));
			return;
		}
	}
	if (m_pDelegate)
	{
		switch(eState)
		{
		case guildboss::GuildBossStateRob:
			m_pDelegate->GuildBossNavigateTo(GuildBossNavDragonSettle);
			break;
		default:
			m_pDelegate->GuildBossNavigateTo(GuildBossNavFightSettle);
			break;
		}
	}
}

void GuildBossView::callBackCurr(CCObject *pSender)
{
	if (m_pDelegate)
	{
		guildboss::GuildBossState eState = m_pModel->getStateOfLivingPoint();
		switch(eState)
		{
		case guildboss::GuildBossStateBoss:
			m_pDelegate->GuildBossNavigateTo(GuildBossNavDragon);
			break;
		case guildboss::GuildBossStateRob:
			m_pDelegate->GuildBossNavigateTo(GuildBossNavFight);
			break;
		default:
			break;
		}
	}
}

void GuildBossView::callBackSettle(const Json::Value &,const Json::Value &responseData)
{
	if (responseData["code"].asString().compare(GUILDBOSS_NO_DATA)==0)
	{
		Tips_Error(D_LOCAL_STRING("GuildBossNoData"));
		return;
	}
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	m_pModel->setHasSettleData(true);
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavSettle);
	}
}

void GuildBossView::callBackRobSettle(const Json::Value &,const Json::Value &responseData)
{
	if (responseData["code"].asString().compare(GUILDBOSS_NO_DATA)==0)
	{
		Tips_Error(D_LOCAL_STRING("GuildBossNoData"));
		return;
	}
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	m_pModel->setHasSettleData(true);
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavFightSettle);
	}
}

void GuildBossView::callBackWeek(CCObject *pSender)
{
	CCMenuItem *pBtn = static_cast<CCMenuItem *>(pSender);
	int tag = pBtn->getTag();
	//m_pNodeWeek[tag]->setVisible(true);
	//m_pNodeWeek[tag]->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(2.0), CCCallFuncN::create(this, callfuncN_selector(GuildBossView::callBackHide))));
}

void GuildBossView::callBackHide(CCNode *pNode)
{
	pNode->setVisible(false);
}