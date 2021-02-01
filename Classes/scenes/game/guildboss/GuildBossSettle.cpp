//
//  GuildBossSettle.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossSettle.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBoss.h"

using namespace sp;

GuildBossSettle::GuildBossSettle()
	: m_pBMFDamageAllSelf(NULL)
	, m_pBMFDamageRobSelf(NULL)
	, m_pBMFDamageGotSelf(NULL)
	, m_pBMFDamageAllGuild(NULL)
	, m_pBMFDamageRobGuild(NULL)
	, m_pBMFDamageGotGuild(NULL)
	, m_pBMFDamageAllTarget(NULL)
	, m_pBMFDamageRobTarget(NULL)
	, m_pBMFDamageGotTarget(NULL)
	, m_pTTFName(NULL)
	, m_pBMFMailTime(NULL)
	, m_pDBtnBack(NULL)
	, m_pNodeToday(NULL)
	, m_pNodeYesterday(NULL)
	, m_pDelegate(NULL)
	, m_pModel(NULL)
	, m_fWaitTime(0.f)
{
}

GuildBossSettle::~GuildBossSettle()
{
	CC_SAFE_RELEASE(m_pBMFDamageAllSelf);
	CC_SAFE_RELEASE(m_pBMFDamageRobSelf);
	CC_SAFE_RELEASE(m_pBMFDamageGotSelf);
	CC_SAFE_RELEASE(m_pBMFDamageAllGuild);
	CC_SAFE_RELEASE(m_pBMFDamageRobGuild);
	CC_SAFE_RELEASE(m_pBMFDamageGotGuild);
	CC_SAFE_RELEASE(m_pBMFDamageAllTarget);
	CC_SAFE_RELEASE(m_pBMFDamageRobTarget);
	CC_SAFE_RELEASE(m_pBMFDamageGotTarget);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pBMFMailTime);
	CC_SAFE_RELEASE(m_pDBtnBack);
	CC_SAFE_RELEASE(m_pNodeToday);
	CC_SAFE_RELEASE(m_pNodeYesterday);
}

GuildBossSettle* GuildBossSettle::create()
{
	GuildBossSettle *pRet = new GuildBossSettle();
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

bool GuildBossSettle::init()
{
	if(!DPopup::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILDBOSS_MODEL;

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("GuildBossMenu", GuildBossMenuLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_banghui/pl_jianghufenzheng_jiesuan", pNodeLoaderLibrary);

	m_pDBtnBack->getButton()->setTarget(this, menu_selector(GuildBossSettle::callBackClose));

	//m_pNodeToday->setVisible(false);

	return true;
}

void GuildBossSettle::onEnter()
{
	BINDING_EXEC(m_pModel, this, GuildBossSettle::updateView, D_EVENT_GUILDBOSS_ALL_SETTLE);
	m_pModel->syncSettle();
	DPopup::onEnter();
}

void GuildBossSettle::onExit()
{
	DPopup::onExit();
	UNBINDING_ALL(m_pModel, this);
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
}

bool GuildBossSettle::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageAllSelf", CCLabelBMFont *, m_pBMFDamageAllSelf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageRobSelf", CCLabelBMFont *, m_pBMFDamageRobSelf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageGotSelf", CCLabelBMFont *, m_pBMFDamageGotSelf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageAllGuild", CCLabelBMFont *, m_pBMFDamageAllGuild);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageRobGuild", CCLabelBMFont *, m_pBMFDamageRobGuild);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageGotGuild", CCLabelBMFont *, m_pBMFDamageGotGuild);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageAllTarget", CCLabelBMFont *, m_pBMFDamageAllTarget);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageRobTarget", CCLabelBMFont *, m_pBMFDamageRobTarget);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageGotTarget", CCLabelBMFont *, m_pBMFDamageGotTarget);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFMailTime", CCLabelBMFont *, m_pBMFMailTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnBack", DButton *, m_pDBtnBack);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeToday", CCNode *, m_pNodeToday);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeYesterDay", CCNode *, m_pNodeYesterday);
	return false;
}

void GuildBossSettle::callBackClose(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavHome);
	}
	closePopup();
}

void GuildBossSettle::updateView()
{
	m_pBMFDamageAllSelf->setString(stringForUIntWith10K(m_pModel->getAllResult()->getDamageAllSelf()));
	m_pBMFDamageRobSelf->setString(stringForIntWith10K(m_pModel->getAllResult()->getDamageRobSelf()));
	m_pBMFDamageGotSelf->setString(stringForUIntWith10K(m_pModel->getAllResult()->getDamageGotSelf()));
	m_pBMFDamageAllGuild->setString(stringForUIntWith10K(m_pModel->getAllResult()->getDamageAllGuild()));
	m_pBMFDamageRobGuild->setString(stringForIntWith10K(m_pModel->getAllResult()->getDamageRobGuild()));
	m_pBMFDamageGotGuild->setString(stringForUIntWith10K(m_pModel->getAllResult()->getDamageGotGuild()));
	m_pBMFDamageAllTarget->setString(stringForUIntWith10K(m_pModel->getAllResult()->getDamageAllTarget()));
	m_pBMFDamageRobTarget->setString(stringForIntWith10K(m_pModel->getAllResult()->getDamageRobTarget()));
	m_pBMFDamageGotTarget->setString(stringForUIntWith10K(m_pModel->getAllResult()->getDamageGotTarget()));
	//m_pTTFName->setString(m_pModel->getAllResult()->getTargetName().c_str());
	//m_pBMFMailTime->setString(m_pModel->getConfig()->getMailTime().c_str());
	uint32_t uNow = GameManager::sharedGameManager()->getServerTime();
	//m_pNodeToday->setVisible(uNow > m_pModel->getConfig()->getEndTime());
	//m_pNodeYesterday->setVisible(!m_pNodeToday->isVisible());
	uint32_t uCount = m_pModel->getCountingDownOfRob();
	if (uCount > 0)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossSettle::updateRefresh), this);
		TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(GuildBossSettle::updateRefresh), this, uCount + 1);
	}
}

void GuildBossSettle::updateRefresh(float dt)
{
	m_pModel->syncSettle();
}