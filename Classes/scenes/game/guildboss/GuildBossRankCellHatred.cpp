//
//  GuildBossRankCellHatred.h
//  公会主页
//
//  Created by Eci on 14-03-22.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossRankCellHatred.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"

using namespace sp;

GuildBossRankCellHatred::GuildBossRankCellHatred()
	: m_pBMFRank(NULL)
	, m_pBMFRobStar(NULL)
	, m_pTTFName(NULL)
	, m_pNodeLastWeek(NULL)
	, m_pNodeThisWeek(NULL)
	, m_pData(NULL)
	, m_pRankNode(NULL)
	, m_pHateNode(NULL)
{
}

GuildBossRankCellHatred::~GuildBossRankCellHatred()
{
	CC_SAFE_RELEASE(m_pBMFRank);
	CC_SAFE_RELEASE(m_pBMFRobStar);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pNodeLastWeek);
	CC_SAFE_RELEASE(m_pNodeThisWeek);
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pRankNode);
	CC_SAFE_RELEASE(m_pHateNode);
}

bool GuildBossRankCellHatred::init(cocos2d::extension::CCData *pCCBData)
{
	if(!GuildBossRankCell::init(pCCBData))
	{
		return false;
	}

	removeAllChildren();
	CCB_READER_DATA_AND_ADDCHILD(pCCBData);
	m_pModel = DM_GET_GUILDBOSS_MODEL;

	m_pHateNode->setVisible(true);
	m_pRankNode->setVisible(false);
	return true;
}

bool GuildBossRankCellHatred::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRank", CCLabelBMFont *, m_pBMFRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRobStar", CCLabelBMFont *, m_pBMFRobStar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeLastWeek", CCNode *, m_pNodeLastWeek);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeThisWeek", CCNode *, m_pNodeThisWeek);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRankNode", CCNode *, m_pRankNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHateNode", CCNode *, m_pHateNode);
	return false;
}

void GuildBossRankCellHatred::setData(CCObject *pData, uint32_t iRank, bool bChangeVisible, bool bLastWeek)
{
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
	guildboss::GuildHatred *pHatred = dynamic_cast<guildboss::GuildHatred *>(pData);
	if (!pHatred)
	{
		CCAssert(NULL != pHatred, "Data error of GuildBossRankCellContribution");
		return;
	}
	m_pBMFRobStar->setString(stringForUIntWith10K(pHatred->getDamageForRob()));
	m_pTTFName->setString(pHatred->getGuildName().c_str());
	m_pBMFRank->setString(D_CSTRING_FROM_UINT(iRank));
	//m_pNodeLastWeek->setVisible(m_pModel->isShowingLastWeek());
	m_pNodeThisWeek->setVisible(!m_pModel->isShowingLastWeek());
}