//
//  GuildBossRescueCell.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossRescueCell.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBoss.h"

using namespace sp;

GuildBossRescueCell::GuildBossRescueCell()
	: m_pBtnRescue(NULL)
	, m_pTTFName(NULL)
	, m_pBMFFight(NULL)
	, m_pBMFGold(NULL)
	, m_pNodeHead(NULL)
	, m_pDelegate(NULL)
	, m_pData(NULL)
	, m_pModel(NULL)
{
}

GuildBossRescueCell::~GuildBossRescueCell()
{
	CC_SAFE_RELEASE(m_pBtnRescue);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pBMFFight);
	CC_SAFE_RELEASE(m_pBMFGold);
	CC_SAFE_RELEASE(m_pNodeHead);
}

GuildBossRescueCell* GuildBossRescueCell::create()
{
	GuildBossRescueCell *pRet = new GuildBossRescueCell();
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

bool GuildBossRescueCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILDBOSS_MODEL;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_jz_cell");

	m_pBtnRescue->setTarget(this, menu_selector(GuildBossRescueCell::callBackRescue));

	m_pBMFGold->setString(D_CSTRING_FROM_INT(m_pModel->getConfig()->getRescueCost()));
	return true;
}

void GuildBossRescueCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildBossRescueCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildBossRescueCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRescue", CCMenuItem *, m_pBtnRescue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFFight", CCLabelBMFont *, m_pBMFFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFGold", CCLabelBMFont *, m_pBMFGold);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeHead", CCNode *, m_pNodeHead);
	return false;
}

void GuildBossRescueCell::callBackRescue(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossRescueUser(this, m_pData->getUserID());
	}
}

void GuildBossRescueCell::setData(guildboss::GuildWoundedUser *pData)
{
	if (!pData)
	{
		return;
	}
	m_pData = pData;
	m_pTTFName->setString(pData->getBasicInfo()->getUserName().c_str());
	m_pBMFFight->setString(stringForUIntWith10K(pData->getBasicInfo()->getUserFight()));
	guildboss::GuildBossUserBasic *pUser = pData->getBasicInfo();
	/*m_pNodeHead->removeAllChildren();
	if (pUser->getLeaderDress())
	{
		m_pNodeHead->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(pUser->getLeaderDress()));
	}
	else
	{
		m_pNodeHead->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(pUser->getLeaderID()));
	}*/
}