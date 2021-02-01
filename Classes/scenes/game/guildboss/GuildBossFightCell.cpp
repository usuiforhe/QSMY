//
//  GuildBossFightCell.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossFightCell.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBoss.h"

using namespace sp;

GuildBossFightCell::GuildBossFightCell()
	: m_pBtnFight(NULL)
	, m_pTTFName(NULL)
	, m_pTTFGuild(NULL)
	, m_pBMFFight(NULL)
	, m_pBMFStar(NULL)
	, m_pBMFGold(NULL)
	, m_pBMFCountDown(NULL)
	, m_pNodeAttack(NULL)
	, m_pNodeGold(NULL)
	, m_pNodeHealthy(NULL)
	, m_pNodeWounded(NULL)
	, m_pDelegate(NULL)
	, m_pData(NULL)
	, m_pModel(NULL)
{
	D_RESET_C_ARRAY(m_pBtnBlood, GUILDBOSS_MAX_BLOOD);
}

GuildBossFightCell::~GuildBossFightCell()
{
	CC_SAFE_RELEASE(m_pBtnFight);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pTTFGuild);
	CC_SAFE_RELEASE(m_pBMFFight);
	CC_SAFE_RELEASE(m_pBMFStar);
	CC_SAFE_RELEASE(m_pBMFGold);
	CC_SAFE_RELEASE(m_pBMFCountDown);
	CC_SAFE_RELEASE(m_pNodeAttack);
	CC_SAFE_RELEASE(m_pNodeGold);
	CC_SAFE_RELEASE(m_pNodeHealthy);
	CC_SAFE_RELEASE(m_pNodeWounded);
	D_SAFE_RELEASE_C_ARRAY(m_pBtnBlood, GUILDBOSS_MAX_BLOOD);
}

GuildBossFightCell* GuildBossFightCell::create()
{
	GuildBossFightCell *pRet = new GuildBossFightCell();
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

bool GuildBossFightCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_fz_cell");

	m_pBtnFight->setTarget(this, menu_selector(GuildBossFightCell::callBackRob));
	m_pModel = DM_GET_GUILDBOSS_MODEL;

	m_pBMFGold->setString(D_CSTRING_FROM_INT(m_pModel->getConfig()->getFightCost()));//getAttackCost()));
	m_pNodeWounded->setVisible(false);
	return true;
}

void GuildBossFightCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildBossFightCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildBossFightCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnFight", CCMenuItem *, m_pBtnFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFGuild", CCLabelTTF *, m_pTTFGuild);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFStar", CCLabelBMFont *, m_pBMFStar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFFight", CCLabelBMFont *, m_pBMFFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFGold", CCLabelBMFont *, m_pBMFGold);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCountDown", CCLabelBMFont *, m_pBMFCountDown);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAttack", CCNode *, m_pNodeAttack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeGold", CCNode *, m_pNodeGold);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeHealthy", CCNode *, m_pNodeHealthy);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeWounded", CCNode *, m_pNodeWounded);
	D_CCB_ASSIGN_ARRAY("m_pBtnBlood", CCMenuItem *, m_pBtnBlood, GUILDBOSS_MAX_BLOOD);
	return false;
}

void GuildBossFightCell::callBackRob(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossFightUser(this, m_pData);
		m_pModel->setRobTarget(m_pData);
	}
}

void GuildBossFightCell::setData(guildboss::GuildBossUserBasic *pData)
{
	if (!pData)
	{
		return;
	}
	m_pData = pData;
	m_pTTFName->setString(pData->getUserName().c_str());
	m_pTTFGuild->setString(pData->getGuildName().c_str());
	m_pBMFFight->setString(stringForUIntWith10K(pData->getUserFight()));
	//m_pBMFStar->setString(stringForUIntWith10K(pData->getDamageForRob()));
	m_pBMFStar->setString(stringForUIntWith10K(pData->getDamageForRobSuccess()));
	m_pNodeGold->setVisible(!m_pModel->getBossSelf()->getAvailTimes());
	m_pNodeHealthy->setVisible(pData->getRobBlood() > 0);
	m_pNodeWounded->setVisible(pData->getRobBlood() == 0);
	for (int i = 0; i < GUILDBOSS_MAX_BLOOD; i++)
	{
		m_pBtnBlood[i]->setEnabled(i < pData->getRobBlood());
	}
}

void GuildBossFightCell::setCountDown(float fCountDown)
{
	m_pNodeAttack->setVisible(fCountDown <= 0);
	m_pBMFCountDown->setVisible(fCountDown > 0);
	if (fCountDown > 0.f)
	{
		m_pBMFCountDown->setString(D_CSTRING_FROM_INT((int)fCountDown));
	}
}