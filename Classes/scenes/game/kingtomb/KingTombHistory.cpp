//
//  KingTombHistory.cpp
//  皇陵探秘历史记录
//
//  Created by mzp on 14-8-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "KingTombHistory.h"
#include "KingTombHistoryCell.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"


#define  KINGTOMB_REWARD_CELL_SIZE		CCSizeMake(575, 91)
#define  KINGTOMB_LOG_CELL_SIZE			CCSizeMake(575, 100)


KingTombHistory::KingTombHistory()
	: m_pKingTombModel(NULL)
	, m_pLogData(NULL)
	, m_pRewardData(NULL)
	, m_pAssistBonusList(NULL)
	, m_pLogList(NULL)
	, m_pBtnReward(NULL)
	, m_pBtnLog(NULL)
	, m_pNodeNewReward(NULL)
	, m_pNodeNewLog(NULL)
	, m_pContainer(NULL)
	, m_pBtnNodeReward(NULL)
	, m_pBtnNodeLog(NULL)
	, m_pBtnCancel(NULL)
	, m_pBtnReceive(NULL)
	, m_pBtnCancel1(NULL)
	, m_pCloseButton(NULL)
	, m_pTvReward(NULL)
	, m_pTvLog(NULL)
{

}

KingTombHistory::~KingTombHistory()
{
	CC_SAFE_RELEASE(m_pLogData);
	CC_SAFE_RELEASE(m_pRewardData);
	CC_SAFE_RELEASE(m_pAssistBonusList);
	CC_SAFE_RELEASE(m_pLogList);

	CC_SAFE_RELEASE(m_pBtnReward);
	CC_SAFE_RELEASE(m_pBtnLog);
	CC_SAFE_RELEASE(m_pNodeNewReward);
	CC_SAFE_RELEASE(m_pNodeNewLog);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pBtnNodeReward);
	CC_SAFE_RELEASE(m_pBtnNodeLog);
	CC_SAFE_RELEASE(m_pBtnCancel);
	CC_SAFE_RELEASE(m_pBtnReceive);
	CC_SAFE_RELEASE(m_pBtnCancel1);
	CC_SAFE_RELEASE(m_pCloseButton);
}

bool KingTombHistory::init()
{
	if (!DPopup::init())
	{
		return false;
	}

	m_pKingTombModel = DM_GET_KINGTOMB_MODEL;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_tanxian_jilu");
	m_pLogData = readCCData("PL_ui/ccb/pl_banghui/pl_banghui_tanxian_jilu_wz");
	m_pRewardData = readCCData("PL_ui/ccb/pl_banghui/pl_banghui_tanxian_jilu_jl");
	CC_SAFE_RETAIN(m_pLogData);
	CC_SAFE_RETAIN(m_pRewardData);

	m_pBtnReward->setTarget(this, menu_selector(KingTombHistory::rewardCallBack));
	m_pBtnLog->setTarget(this, menu_selector(KingTombHistory::logCallBack));
	m_pBtnCancel->getButton()->setTarget(this, menu_selector(KingTombHistory::cancelCallBack));
	m_pBtnReceive->getButton()->setTarget(this, menu_selector(KingTombHistory::receiveCallBack));
	m_pBtnCancel1->getButton()->setTarget(this, menu_selector(KingTombHistory::cancelCallBack));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(KingTombHistory::cancelCallBack));

	m_pAssistBonusList = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pAssistBonusList);
	m_pLogList = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pLogList);

	m_pTvReward = DTableView::create(this, m_pContainer->getContentSize());
	m_pTvReward->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pContainer->addChild(m_pTvReward);

	m_pTvLog = DTableView::create(this, m_pContainer->getContentSize());
	m_pTvLog->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pContainer->addChild(m_pTvLog);

	// 默认选择助战奖励
	rewardCallBack(m_pBtnReward);

	PL_MOVE_NODE_ABOVE_CENTER_Y(this);
	return true;
}

void KingTombHistory::onEnter()
{
	DPopup::onEnter();

	BINDING_EXEC(m_pKingTombModel, this, KingTombHistory::updateReward, D_EVENT_KINGTOMB_GET_ASSIST_BONUS_LIST);
	BINDING_EXEC(m_pKingTombModel, this, KingTombHistory::updateLog, D_EVENT_KINGTOMB_GET_LOG);

	// 获取记录
	m_pKingTombModel->syncAssistBonusList();
	m_pKingTombModel->syncLog();
}

void KingTombHistory::onExit()
{
	UNBINDING_ALL(m_pKingTombModel, this);
	DPopup::onExit();
}

bool KingTombHistory::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnReward", CCMenuItem *, m_pBtnReward);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnLog", CCMenuItem *, m_pBtnLog);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNewReward", CCSprite *, m_pNodeNewReward);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNewLog", CCSprite *, m_pNodeNewLog);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnNodeReward", CCNode *, m_pBtnNodeReward);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnNodeLog", CCNode *, m_pBtnNodeLog);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnCancel", DButton *, m_pBtnCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnReceive", DButton *, m_pBtnReceive);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnCancel1", DButton *, m_pBtnCancel1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);

	return false;
}

void KingTombHistory::rewardCallBack(cocos2d::CCObject* sender)
{
	m_pBtnReward->selected();
	m_pBtnLog->unselected();

	m_pTvReward->setVisible(true);
	m_pTvLog->setVisible(false);

	m_pBtnNodeReward->setVisible(true);
	m_pBtnNodeLog->setVisible(false);

	m_pKingTombModel->setHaveNewAssistBonus(false);
	//m_pNodeNewReward->setVisible(false);
}

void KingTombHistory::logCallBack(cocos2d::CCObject* sender)
{
	m_pBtnReward->unselected();
	m_pBtnLog->selected();

	m_pTvReward->setVisible(false);
	m_pTvLog->setVisible(true);

	m_pBtnNodeReward->setVisible(false);
	m_pBtnNodeLog->setVisible(true);

	m_pKingTombModel->setHaveNewLog(false);
	//m_pNodeNewLog->setVisible(false);
}

void KingTombHistory::receiveCallBack(cocos2d::CCObject* sender)
{
	HTTP_CLIENT->getAllAssistBonusDragonValley();
}

void KingTombHistory::cancelCallBack(cocos2d::CCObject* sender)
{
	closePopup();
    //this->removeFromParent();
}

CCTableViewCell* KingTombHistory::tableCellAtIndex(CCTableView* table, uint32_t idx)
{
	if (table == m_pTvReward)
	{
		KingTombAssistBonusCell* cell = (KingTombAssistBonusCell*)table->dequeueCell();
		if (!cell)
		{
			cell = KingTombAssistBonusCell::create(m_pRewardData);
		}

		if (idx < m_pAssistBonusList->count())
		{
			cell->setAssistBonusInfo((kingTomb::AssistBonusInfo*)m_pAssistBonusList->objectAtIndex(idx));
		}
		else
		{
			cell->setAssistBonusInfo(NULL);
		}

		return cell;
	}
	else if (table == m_pTvLog)
	{
		KingTombLogCell* cell = (KingTombLogCell*)table->dequeueCell();
		if (!cell)
		{
			cell = KingTombLogCell::create(m_pLogData);
		}

		if (idx < m_pLogList->count())
		{
			cell->setLogInfo((kingTomb::LogInfo*)m_pLogList->objectAtIndex(idx));
		}
		else
		{
			cell->setLogInfo(NULL);
		}

		return cell;
	}

	return NULL;
}

unsigned int KingTombHistory::numberOfCellsInTableView(CCTableView* table)
{
	if (table == m_pTvReward)
	{
		return m_pAssistBonusList->count();
	}
	else if (table == m_pTvLog)
	{
		return m_pLogList->count();
	}
	
	return 0;
}

CCSize KingTombHistory::tableCellSizeForIndex(CCTableView* table, unsigned int idx)
{
	if (table == m_pTvReward)
	{
		return KINGTOMB_REWARD_CELL_SIZE;
	}
	else if (table == m_pTvLog)
	{
		return KINGTOMB_LOG_CELL_SIZE;
	}

	return CCSizeMake(0, 0);
}

void KingTombHistory::updateReward()
{
	// 刷新数据
	m_pKingTombModel->getAssistBonusList(m_pAssistBonusList);

	m_pTvReward->reloadData();

	if (m_pTvReward->isVisible())
	{
		m_pKingTombModel->setHaveNewAssistBonus(false);
	}
	
	//m_pNodeNewReward->setVisible(m_pKingTombModel->getHaveNewAssistBonus());
	m_pBtnReceive->getButton()->setEnabled(m_pAssistBonusList->count() > 0);
}

void KingTombHistory::updateLog()
{
	// 刷新数据
	m_pKingTombModel->getLogList(m_pLogList);

	m_pTvLog->reloadData();

	if (m_pTvLog->isVisible())
	{
		m_pKingTombModel->setHaveNewLog(false);
	}
	
	//m_pNodeNewLog->setVisible(m_pKingTombModel->getHaveNewLog());
}
