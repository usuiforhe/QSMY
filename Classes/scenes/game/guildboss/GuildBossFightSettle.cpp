//
//  GuildBossFightSettle.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossFightSettle.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBoss.h"
#include "../common/gui/WarriorGradeBg.h"

using namespace sp;

GuildBossFightSettle::GuildBossFightSettle()
	: m_pConfirm(NULL)
	, m_pCloseButton(NULL)
	, m_pNodeHead(NULL)
	, m_pTTFName(NULL)
	, m_pBMFLevel(NULL)
	, m_pBMFDamageAdd(NULL)
	, m_pBMFDamageAll(NULL)
	, m_pBMFDamageRob(NULL)
	, m_pNodeHealthy(NULL)
	, m_pNodeHurtBoss(NULL)
	, m_pNodeHurtRob(NULL)
	, m_pNodeBoss(NULL)
	, m_pNodeRob(NULL)
	, m_pNodeWin(NULL)
	, m_pNodeLose(NULL)
	, m_pNodePercent50(NULL)
	, m_pNodePercent80(NULL)
	, m_pModel(NULL)
	, m_pDelegate(NULL)
	, m_fWaitTime(.0f)
	, m_eState(guildboss::GuildBossStateClose)
	, m_bWounded(false)
	, m_pWarriorGradeBg(NULL)
{
	D_RESET_C_ARRAY(m_pGrade, PROGRESS_BAR_COUNT);
}

GuildBossFightSettle::~GuildBossFightSettle()
{
	CC_SAFE_RELEASE(m_pConfirm);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pNodeHead);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pBMFLevel);
	CC_SAFE_RELEASE(m_pBMFDamageAdd);
	CC_SAFE_RELEASE(m_pBMFDamageAll);
	CC_SAFE_RELEASE(m_pBMFDamageRob);
	CC_SAFE_RELEASE(m_pNodeHealthy);
	CC_SAFE_RELEASE(m_pNodeHurtBoss);
	CC_SAFE_RELEASE(m_pNodeHurtRob);
	CC_SAFE_RELEASE(m_pNodeBoss);
	CC_SAFE_RELEASE(m_pNodeRob);
	CC_SAFE_RELEASE(m_pNodeWin);
	CC_SAFE_RELEASE(m_pNodeLose);
	CC_SAFE_RELEASE(m_pNodePercent50);
	CC_SAFE_RELEASE(m_pNodePercent80);
	CC_SAFE_RELEASE(m_pWarriorGradeBg);
	D_SAFE_RELEASE_C_ARRAY(m_pGrade, PROGRESS_BAR_COUNT);
}

GuildBossFightSettle* GuildBossFightSettle::create()
{
	GuildBossFightSettle *pRet = new GuildBossFightSettle();
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

bool GuildBossFightSettle::init()
{
	if(!DPopup::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILDBOSS_MODEL;

	//CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_js_che");
	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorGradeBg", WarriorGradeBgLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_js_che", pNodeLoaderLibrary);


 	m_pConfirm->getButton()->setTarget(this, menu_selector(GuildBossFightSettle::closeCallBack));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(GuildBossFightSettle::closeCallBack));
	uint32_t leaderID = DM_GET_FORMATION_MODEL->getFormationLeader();
	uint32_t avatarID = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(leaderID)->getAvatarId();

	m_pNodeHead->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForDetail(avatarID));
	m_pTTFName->setString(DM_GET_USER_MODEL->getUserInfo()->getUserName().c_str());
	m_pBMFLevel->setString(D_CSTRING_FROM_INT(DM_GET_USER_MODEL->getUserInfo()->getUserLv()));

	//处理背景
	warrior::WarriorBaseInfo	*pInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(avatarID);
	m_pWarriorGradeBg->setWarriorGrade(pInfo->getColorGrade());
	int grade = (int)pInfo->getColorGrade();
	for(int i = 0 ; i < PROGRESS_BAR_COUNT ; i++)
	{		
		m_pGrade[i]->setVisible( grade == i+1 );
	}
	
	m_pNodeBoss->setVisible(false);
	m_pNodeRob->setVisible(false);

	m_pNodeHealthy->setVisible(false);
	m_pNodeHurtBoss->setVisible(false);
	m_pNodeHurtRob->setVisible(false);

	//m_pNodeWin->setVisible(false);
	//m_pNodeLose->setVisible(false);

	return true;
}

void GuildBossFightSettle::onEnter()
{
	BINDING_ONLY(m_pModel, this, GuildBossFightSettle::updateView, D_EVENT_GUILDBOSS_ALL_SETTLE);
	BINDING_EXEC(m_pModel, this, GuildBossFightSettle::updateView, D_EVENT_GUILDBOSS_REFRESH);
	m_pModel->syncSettle();
	DPopup::onEnter();
}

void GuildBossFightSettle::onExit()
{
	DPopup::onExit();
	UNBINDING_ALL(m_pModel, this);
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
}

bool GuildBossFightSettle::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirm", DButton *, m_pConfirm);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeHead", CCNode *, m_pNodeHead);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFLevel", CCLabelBMFont *, m_pBMFLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageAdd", CCLabelBMFont *, m_pBMFDamageAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageAll", CCLabelBMFont *, m_pBMFDamageAll);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFDamageRob", CCLabelBMFont *, m_pBMFDamageRob);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeHealthy", CCNode *, m_pNodeHealthy);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeHurtBoss", CCNode *, m_pNodeHurtBoss);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeHurtRob", CCNode *, m_pNodeHurtRob);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeBoss", CCNode *, m_pNodeBoss);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRob", CCNode *, m_pNodeRob);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeWin", CCNode *, m_pNodeWin);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeLose", CCNode *, m_pNodeLose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodePercent80", CCNode *, m_pNodePercent80);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodePercent50", CCNode *, m_pNodePercent50);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg *, m_pWarriorGradeBg);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pProgressBar", DProgressBar *, m_pGrade, PROGRESS_BAR_COUNT);
	return false;
}

void GuildBossFightSettle::closeCallBack(CCObject*)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavHome);
	}
	closePopup();
}

void GuildBossFightSettle::callBackSubmit(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavHome);
	}
}

void GuildBossFightSettle::updateView()
{
	//m_pNodeWin->setVisible(m_pModel->getAllResult()->getDamageRobSelf() >= 0 && m_eState == guildboss::GuildBossStateRob);
	//m_pNodeLose->setVisible(m_pModel->getAllResult()->getDamageRobSelf() < 0 && m_eState == guildboss::GuildBossStateRob);
	m_pBMFDamageAdd->setString(stringForUIntWith10K(abs(m_pModel->getAllResult()->getDamageRobSelf())));
	bool bLiving = m_pModel->getStateOfLivingPoint() == guildboss::GuildBossStateBoss
		|| m_pModel->getStateOfLivingPoint() == guildboss::GuildBossStateRob;
	if(bLiving)
	{
		TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
		TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
	}
	uint32_t uCount = m_pModel->getCountingDownOfRob();
	if (uCount > 0)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossFightSettle::updateRefresh), this);
		TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(GuildBossFightSettle::updateRefresh), this, uCount + 1);
	}
	updateStatus();
}

void GuildBossFightSettle::update(float dt)
{
	m_fWaitTime = m_pModel->getCountingDownOfRob();
	guildboss::GuildBossState eState = m_pModel->getStateOfLivingPoint();
	if (eState == m_eState)
	{
		guildboss::GuildBossState eNewState = guildboss::GuildBossStateClose;
		switch (eState)
		{
		case guildboss::GuildBossStateBoss:
			eNewState = guildboss::GuildBossStateRob;
			break;
		case guildboss::GuildBossStateRob:
			eNewState = guildboss::GuildBossStateBoss;
			break;
		default:
			eNewState = guildboss::GuildBossStateClose;
		}
		setState(eNewState);
	}
}

void GuildBossFightSettle::updateStatus()
{
	bool wounded = !m_pModel->getAllResult()->getYesterdayBlood();
	m_pNodeHealthy->setVisible(!wounded);
	m_pNodeHurtBoss->setVisible(wounded && m_eState == guildboss::GuildBossStateBoss);
	m_pNodeHurtRob->setVisible(wounded && m_eState == guildboss::GuildBossStateRob);
	m_pNodePercent50->setVisible(m_pNodeHealthy->isVisible());
	m_pNodePercent80->setVisible(wounded);
	m_pBMFDamageRob->setString(stringForUIntWith10K(m_pModel->getBossSelf()->getDamageForRobBase()));
	uint32_t damageAll = m_pModel->getBossSelf()->getDamageForRobBase() * 100 / m_pModel->getConfig()->getStarRateNormal();
	if (wounded)
	{
		damageAll = m_pModel->getBossSelf()->getDamageForRobBase() * 100 / m_pModel->getConfig()->getStarRateWounded();
	}
	m_pBMFDamageAll->setString(stringForUIntWith10K(damageAll));
}

void GuildBossFightSettle::setState(guildboss::GuildBossState eState)
{
	CCLOG("GuildBossFightSettle::setState%d", eState);
	if (m_eState == eState)
	{
		return;
	}
	m_bWounded = !m_pModel->getBossSelf()->getRobBlood();
	m_eState = eState;
	m_pNodeBoss->setVisible(guildboss::GuildBossStateBoss == eState);
	m_pNodeRob->setVisible(guildboss::GuildBossStateRob == eState);
}

void GuildBossFightSettle::updateRefresh(float dt)
{
	m_pModel->syncSettle();
}