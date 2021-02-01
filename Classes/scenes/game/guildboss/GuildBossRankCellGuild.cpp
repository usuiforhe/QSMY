//
//  GuildBossRankCellGuild.h
//  公会主页
//
//  Created by Eci on 14-03-22.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossRankCellGuild.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"

using namespace sp;

GuildBossRankCellGuild::GuildBossRankCellGuild()
	: m_pBMFFight(NULL)
	, m_pBMFRank(NULL)
	, m_pBMFStar(NULL)
	, m_pTTFName(NULL)
	, m_pNodeLastWeek(NULL)
	, m_pNodeThisWeek(NULL)
	, m_pNodeSelf(NULL)
	, m_pNodeOther(NULL)
	, m_pCCBChange(NULL)
	, m_pData(NULL)
	, m_pRankNode(NULL)
	, m_pHateNode(NULL)
{
	D_RESET_C_ARRAY(m_pNodeChange, guildboss::RankChangeTypeMax);
}

GuildBossRankCellGuild::~GuildBossRankCellGuild()
{
	CC_SAFE_RELEASE(m_pBMFFight);
	CC_SAFE_RELEASE(m_pBMFRank);
	CC_SAFE_RELEASE(m_pBMFStar);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pNodeLastWeek);
	CC_SAFE_RELEASE(m_pNodeThisWeek);
	CC_SAFE_RELEASE(m_pNodeSelf);
	CC_SAFE_RELEASE(m_pNodeOther);
	CC_SAFE_RELEASE(m_pCCBChange);
	D_SAFE_RELEASE_C_ARRAY(m_pNodeChange, guildboss::RankChangeTypeMax);
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pRankNode);
	CC_SAFE_RELEASE(m_pHateNode);
}

bool GuildBossRankCellGuild::init(cocos2d::extension::CCData *pCCBData)
{
	if(!GuildBossRankCell::init(pCCBData))
	{
		return false;
	}

	removeAllChildren();
	CCB_READER_DATA_AND_ADDCHILD(pCCBData);
	m_pModel = DM_GET_GUILDBOSS_MODEL;

	m_pHateNode->setVisible(false);
	m_pRankNode->setVisible(true);
	return true;
}

bool GuildBossRankCellGuild::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFFight", CCLabelBMFont *, m_pBMFFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRank", CCLabelBMFont *, m_pBMFRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFStar", CCLabelBMFont *, m_pBMFStar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeLastWeek", CCNode *, m_pNodeLastWeek);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeThisWeek", CCNode *, m_pNodeThisWeek);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeSelf", CCNode *, m_pNodeSelf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeOther", CCNode *, m_pNodeOther);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCCBChange", CCNode *, m_pCCBChange);
	D_CCB_ASSIGN_ARRAY("m_pNode", CCNode *, m_pNodeChange, guildboss::RankChangeTypeMax);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRankNode", CCNode *, m_pRankNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHateNode", CCNode *, m_pHateNode);
	return false;
}

void GuildBossRankCellGuild::setData(CCObject *pData, uint32_t iRank, bool bChangeVisible, bool bLastWeek)
{
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
	guildboss::GuildBossBasic *pGuild = dynamic_cast<guildboss::GuildBossBasic *>(pData);
	if (!pGuild)
	{
		CCAssert(NULL != pGuild, "Data error of GuildBossRankCellContribution");
		return;
	}
	m_pBMFFight->setString(stringForUIntWith10K(pGuild->getGuildFight()));
	m_pBMFStar->setString(stringForUIntWith10K(pGuild->getDamage()));
	m_pTTFName->setString(pGuild->getGuildName().c_str());
	//m_pNodeSelf->setVisible(m_pModel->getBoss()->getGuildID() == pGuild->getGuildID());
	//m_pNodeOther->setVisible(m_pModel->getBoss()->getGuildID() != pGuild->getGuildID());
	m_pBMFRank->setString(D_CSTRING_FROM_UINT(iRank));
	guildboss::RankChangeType eChange = guildboss::RankChangeTypeNone;
	if (!pGuild->getLastRank() || pGuild->getLastRank() > iRank)
	{
		eChange = guildboss::RankChangeTypeUp;
	}
	else if (pGuild->getLastRank() < iRank)
	{
		eChange = guildboss::RankChangeTypeDown;
	}
	for (int i = 0; i < guildboss::RankChangeTypeMax; i++)
	{
		//m_pNodeChange[i]->setVisible(i == eChange && pGuild->getLastRank());
	}
	//m_pCCBChange->setVisible(bChangeVisible);
	//m_pNodeLastWeek->setVisible(bLastWeek);
	m_pNodeThisWeek->setVisible(!bLastWeek);
}