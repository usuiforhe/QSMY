//
//  KingTombRankCell.cpp
//  皇陵探秘历史记录
//
//  Created by mzp on 14-8-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "KingTombRankCell.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"

KingTombRankCell::KingTombRankCell()
	:m_pLabelRank(NULL)
	,m_pLabelCount(NULL)
	,m_pLabelLevel(NULL)
	,m_pLabelName(NULL)
	,m_pLabelFight(NULL)
	,m_pGuildNode(NULL)
	,m_pUserNode(NULL)
{
	D_RESET_C_ARRAY(m_pRankIcon, RANK_ICON_COUNT);
}

KingTombRankCell::~KingTombRankCell()
{
	CC_SAFE_RELEASE(m_pLabelRank);
	CC_SAFE_RELEASE(m_pLabelCount);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pLabelFight);
	CC_SAFE_RELEASE(m_pGuildNode);
	CC_SAFE_RELEASE(m_pUserNode);
	D_SAFE_RELEASE_C_ARRAY(m_pRankIcon, RANK_ICON_COUNT);
}

KingTombRankCell* KingTombRankCell::create(cocos2d::extension::CCData* pData)
{
	KingTombRankCell* pRet = new KingTombRankCell();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
    return NULL;
}

bool KingTombRankCell::init(cocos2d::extension::CCData* pData)
{
	CCB_READER_DATA_AND_ADDCHILD(pData);

	return true;
}

bool KingTombRankCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_rank_cell_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_rank_cell_rank", CCLabelBMFont *, this->m_pLabelRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_count", CCLabelBMFont *, this->m_pLabelCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_fight", CCLabelBMFont *, this->m_pLabelFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuildNode", CCNode *, this->m_pGuildNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUserNode", CCNode *, this->m_pUserNode);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("rank",CCNode*,this->m_pRankIcon,RANK_ICON_COUNT);
	return false;
}

void KingTombRankCell::setData(kingTomb::kingTombRankInfo* pInfo , unsigned int rank)
{
	if(pInfo)
	{
		m_pLabelName->setString(pInfo->getName().c_str());
		m_pLabelRank->setString(D_CSTRING_FROM_UINT(rank + 1));
		m_pLabelCount->setString(D_CSTRING_FROM_UINT(pInfo->getCount()));
		m_pLabelLevel->setString(D_CSTRING_FROM_UINT(pInfo->getLevel()));
		m_pLabelFight->setString(D_CSTRING_FROM_UINT(pInfo->getFight()));
		for(int i = 0 ; i < RANK_ICON_COUNT ; i++)
			m_pRankIcon[i]->setVisible(rank == i);
	}
	
}

void KingTombRankCell::setGuildRankData(kingTomb::kingTombRankInfo* pInfo , unsigned int rank)
{
	setData(pInfo,rank);
	m_pGuildNode->setVisible(true);
	m_pUserNode->setVisible(false);
}

void KingTombRankCell::setUserRankData(kingTomb::kingTombRankInfo* pInfo , unsigned int rank)
{
	setData(pInfo,rank);
	m_pGuildNode->setVisible(false);
	m_pUserNode->setVisible(true);
}
