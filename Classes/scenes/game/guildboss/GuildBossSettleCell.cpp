//
//  GuildBossSettleCell.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossSettleCell.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"

using namespace sp;

GuildBossSettleCell::GuildBossSettleCell()
	: m_pBMFFight(NULL)
	, m_pBMFStar(NULL)
	, m_pBMFRank(NULL)
	, m_pTTFName(NULL)
	, m_pCCBData(NULL)
	, m_pData(NULL)
{
	D_RESET_C_ARRAY(m_pNodeChange, guildboss::RankChangeTypeMax);
}

GuildBossSettleCell::~GuildBossSettleCell()
{
	D_SAFE_RELEASE_C_ARRAY(m_pNodeChange, guildboss::RankChangeTypeMax);
	CC_SAFE_RELEASE(m_pBMFFight);
	CC_SAFE_RELEASE(m_pBMFStar);
	CC_SAFE_RELEASE(m_pBMFRank);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pData);
}

GuildBossSettleCell* GuildBossSettleCell::create()
{
	GuildBossSettleCell *pRet = new GuildBossSettleCell();
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

bool GuildBossSettleCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("ui/ccb/guildBoss/guildBossSettleCell");
	return true;
}

void GuildBossSettleCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildBossSettleCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildBossSettleCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	D_CCB_ASSIGN_ARRAY("m_pNode", CCNode *, m_pNodeChange, guildboss::RankChangeTypeMax);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFFight", CCLabelBMFont *, m_pBMFFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFStar", CCLabelBMFont *, m_pBMFStar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRank", CCLabelBMFont *, m_pBMFRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	return false;
}

void GuildBossSettleCell::setData(guildboss::GuildBossBasic *pData)
{
	if (!pData)
	{
		return;
	}
	guildboss::GuildBossBasic *pGuild = dynamic_cast<guildboss::GuildBossBasic *>(pData);
	if (!pGuild)
	{
		return;
	}
	m_pBMFFight->setString(stringForUIntWith10K(pGuild->getGuildFight()));
	m_pBMFStar->setString(stringForUIntWith10K(pGuild->getDamage()));
	m_pTTFName->setString(pGuild->getGuildName().c_str());
// 	guildboss::RankChangeType eChange = guildboss::RankChangeTypeNone;
// 	if (pGuild->getLastDamage() > pGuild->getDamage())
// 	{
// 		eChange = guildboss::RankChangeTypeDown;
// 	}
// 	else if (pGuild->getLastDamage() < pGuild->getDamage())
// 	{
// 		eChange = guildboss::RankChangeTypeUp;
// 	}
// 	for (int i = 0; i < guildboss::RankChangeTypeMax; i++)
// 	{
// 		m_pNodeChange[i]->setVisible(i == eChange);
// 	}
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
}

void GuildBossSettleCell::setRank(uint32_t iRank)
{
	m_pBMFRank->setString(D_CSTRING_FROM_INT(iRank));
	guildboss::RankChangeType eChange = guildboss::RankChangeTypeNone;
	if (!m_pData->getLastRank() || m_pData->getLastRank() > iRank)
	{
		eChange = guildboss::RankChangeTypeUp;
	}
	else if (m_pData->getLastRank() < iRank)
	{
		eChange = guildboss::RankChangeTypeDown;
	}
	for (int i = 0; i < guildboss::RankChangeTypeMax; i++)
	{
		m_pNodeChange[i]->setVisible(i == eChange);
	}
}