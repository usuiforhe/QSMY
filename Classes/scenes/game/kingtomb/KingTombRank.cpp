//
//  KingTombRank.cpp
//  皇陵探秘历史记录
//
//  Created by mzp on 14-8-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "KingTombRank.h"
#include "KingTombRankCell.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"

#define  KINGTOMB_RANK_CELL_SIZE			CCSizeMake(575, 100)

KingTombRank::KingTombRank()
	: m_pKingTombModel(NULL)
	, m_pCellData(NULL)
	, m_pUserRankList(NULL)
	, m_pGuildRankList(NULL)
	, m_pBtnReward(NULL)
	, m_pBtnLog(NULL)
	, m_pContainer(NULL)
	, m_pBtnCancel(NULL)
	, m_pCloseButton(NULL)
	, m_pTvUser(NULL)
	, m_pTvGuild(NULL)
{

}

KingTombRank::~KingTombRank()
{
	CC_SAFE_RELEASE(m_pCellData);

	CC_SAFE_RELEASE(m_pBtnReward);
	CC_SAFE_RELEASE(m_pBtnLog);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pBtnCancel);
	CC_SAFE_RELEASE(m_pCloseButton);
}

bool KingTombRank::init()
{
	if (!DPopup::init())
	{
		return false;
	}

	m_pKingTombModel = DM_GET_KINGTOMB_MODEL;
	
	m_pUserRankList = m_pKingTombModel->getUserRankList();
	m_pGuildRankList = m_pKingTombModel->getGuildRankList();

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_tanxian_baokuph");
	m_pCellData = readCCData("PL_ui/ccb/pl_banghui/pl_banghui_tanxian_baokuph_cell");
	CC_SAFE_RETAIN(m_pCellData);

	m_pBtnReward->setTarget(this, menu_selector(KingTombRank::rewardCallBack));
	m_pBtnLog->setTarget(this, menu_selector(KingTombRank::logCallBack));
	m_pBtnCancel->getButton()->setTarget(this, menu_selector(KingTombRank::cancelCallBack));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(KingTombRank::cancelCallBack));

	m_pTvUser = DTableView::create(this, m_pContainer->getContentSize());
	m_pTvUser->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pContainer->addChild(m_pTvUser);

	m_pTvGuild = DTableView::create(this, m_pContainer->getContentSize());
	m_pTvGuild->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pContainer->addChild(m_pTvGuild);

	// 默认选择助战奖励
	rewardCallBack(m_pBtnReward);

	PL_MOVE_NODE_ABOVE_CENTER_Y(this);
	return true;
}

void KingTombRank::onEnter()
{
	DPopup::onEnter();
	BINDING_EXEC(m_pKingTombModel, this, KingTombRank::updateView, D_EVENT_KINGTOMB_GET_RANK);
	//更新两个表
	m_pKingTombModel->syncRankList(true);
	m_pKingTombModel->syncRankList(false);
}

void KingTombRank::onExit()
{
	UNBINDING_ALL(m_pKingTombModel, this);
	DPopup::onExit();
}

bool KingTombRank::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnReward", CCMenuItem *, m_pBtnReward);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnLog", CCMenuItem *, m_pBtnLog);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNewReward", CCSprite *, m_pNodeNewReward);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNewLog", CCSprite *, m_pNodeNewLog);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnCancel", DButton *, m_pBtnCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);

	return false;
}

void KingTombRank::rewardCallBack(cocos2d::CCObject* sender)
{
	m_pBtnReward->selected();
	m_pBtnLog->unselected();

	m_pTvUser->setVisible(false);
	m_pTvGuild->setVisible(true);
}

void KingTombRank::logCallBack(cocos2d::CCObject* sender)
{
	m_pBtnReward->unselected();
	m_pBtnLog->selected();

	m_pTvUser->setVisible(true);
	m_pTvGuild->setVisible(false);
}

void KingTombRank::cancelCallBack(cocos2d::CCObject* sender)
{
	closePopup();
    //this->removeFromParent();
}

CCTableViewCell* KingTombRank::tableCellAtIndex(CCTableView* table, uint32_t idx)
{
	if (table == m_pTvUser)
	{
		KingTombRankCell* cell = (KingTombRankCell*)table->dequeueCell();
		if (!cell)
		{
			cell = KingTombRankCell::create(m_pCellData);
		}

		//if (idx < m_pAssistBonusList->count())
		//{
			cell->setUserRankData((kingTomb::kingTombRankInfo*)m_pUserRankList->objectAtIndex(idx),idx);
		//}
		//else
		//{
		//	cell->setAssistBonusInfo(NULL);
		//}

		return cell;
	}
	else if (table == m_pTvGuild)
	{
		KingTombRankCell* cell = (KingTombRankCell*)table->dequeueCell();
		if (!cell)
		{
			cell = KingTombRankCell::create(m_pCellData);
		}

		//if (idx < m_pGuildRankList->count())
		//{
			cell->setGuildRankData((kingTomb::kingTombRankInfo*)m_pGuildRankList->objectAtIndex(idx),idx);
		//}
		//else
		//{
		//	cell->setLogInfo(NULL);
		//}

		return cell;
	}

	return NULL;
}

unsigned int KingTombRank::numberOfCellsInTableView(CCTableView* table)
{
	if (table == m_pTvUser)
	{
		return m_pUserRankList->count();
	}
	else if (table == m_pTvGuild)
	{
		return m_pGuildRankList->count();
	}
	
	return 0;
}

CCSize KingTombRank::tableCellSizeForIndex(CCTableView* table, unsigned int idx)
{
	return KINGTOMB_RANK_CELL_SIZE;
}

void KingTombRank::updateView()
{
	m_pTvGuild->reloadData();
	m_pTvUser->reloadData();
}
