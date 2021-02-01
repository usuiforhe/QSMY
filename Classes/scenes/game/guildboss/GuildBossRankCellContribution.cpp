//
//  GuildBossRankCellContribution.h
//  公会主页
//
//  Created by Eci on 14-03-22.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossRankCellContribution.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"

using namespace sp;

GuildBossRankCellContribution::GuildBossRankCellContribution()
	: m_pBMFStar(NULL)
	, m_pBMFRank(NULL)
	, m_pTTFName(NULL)
	, m_pNodeHead(NULL)
	, m_pNodeSelf(NULL)
	, m_pNodeOther(NULL)
	, m_pContributeNode(NULL)
	, m_pHelpNode(NULL)
	, m_pData(NULL)
{
}

GuildBossRankCellContribution::~GuildBossRankCellContribution()
{
	CC_SAFE_RELEASE(m_pBMFStar);
	CC_SAFE_RELEASE(m_pBMFRank);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pNodeHead);
	CC_SAFE_RELEASE(m_pNodeSelf);
	CC_SAFE_RELEASE(m_pNodeOther);
	CC_SAFE_RELEASE(m_pHelpNode);
	CC_SAFE_RELEASE(m_pContributeNode);
	CC_SAFE_RELEASE(m_pData);
}

bool GuildBossRankCellContribution::init(cocos2d::extension::CCData *pCCBData)
{
	if(!GuildBossRankCell::init(pCCBData))
	{
		return false;
	}

	removeAllChildren();
	CCB_READER_DATA_AND_ADDCHILD(pCCBData);
	m_pModel = DM_GET_GUILDBOSS_MODEL;

	m_pHelpNode->setVisible(false);
	m_pContributeNode->setVisible(true);
	return true;
}

bool GuildBossRankCellContribution::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFStar", CCLabelBMFont *, m_pBMFStar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRank", CCLabelBMFont *, m_pBMFRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeHead", CCNode *, m_pNodeHead);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeSelf", CCNode *, m_pNodeSelf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeOther", CCNode *, m_pNodeOther);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContributeNode", CCNode *, m_pContributeNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHelpNode", CCNode *, m_pHelpNode);
	return false;
}

void GuildBossRankCellContribution::setData(CCObject *pData, uint32_t iRank, bool bChangeVisible, bool bLastWeek)
{
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
	guildboss::GuildBossUserBasic *pUser = dynamic_cast<guildboss::GuildBossUserBasic *>(pData);
	if (!pUser)
	{
		CCAssert(NULL != pUser, "Data error of GuildBossRankCellContribution");
		return;
	}
	m_pBMFStar->setString(stringForUIntWith10K(pUser->getTotalDamage()));
	m_pTTFName->setString(pUser->getUserName().c_str());
	m_pNodeHead->removeAllChildren();
	if (pUser->getLeaderDress())
	{
		m_pNodeHead->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(pUser->getLeaderDress()));
	}
	else
	{
		m_pNodeHead->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(pUser->getLeaderID()));
	}
	//m_pNodeSelf->setVisible(m_pModel->getBossSelf()->getUserID() == pUser->getUserID());
	//m_pNodeOther->setVisible(m_pModel->getBossSelf()->getUserID() != pUser->getUserID());
	m_pBMFRank->setString(D_CSTRING_FROM_UINT(iRank));
}