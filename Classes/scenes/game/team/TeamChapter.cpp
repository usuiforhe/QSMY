//
//  TeamChapter.h
//  组队章节
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#define UNSELECT -1

#include "TeamChapter.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "TeamChapterCell.h"
#include <algorithm>

TeamChapter::TeamChapter()
	: m_pNodeContainer(NULL)
	, m_pCell(NULL)
	, m_pDelegate(NULL)
	, m_pModel(NULL)
	, m_pList(NULL)
	, m_nSelected(UNSELECT)
{
}

TeamChapter::~TeamChapter()
{
	CC_SAFE_RELEASE(m_pNodeContainer);
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pList);
}

TeamChapter* TeamChapter::create()
{
	TeamChapter *pRet = new TeamChapter();
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

bool TeamChapter::init()
{
	if(!DLayer::init())
	{
		return false;
	}


	CCB_READER_AND_ADDCHILD("ui/ccb/team/teamChapter");

	CCB_READER("ui/ccb/team/teamChapterCell", m_pCell);
	CC_SAFE_RETAIN(m_pCell);

	m_pModel = DM_GET_FRIENDS_MODEL;

	m_pList = CCArray::create();
	CC_SAFE_RETAIN(m_pList);

	m_pModel->getChapters(m_pList);

	m_pTableView = DTableView::create(this, m_pNodeContainer->getContentSize());
	m_pNodeContainer->addChild(m_pTableView);

	return true;
}

void TeamChapter::onEnter()
{
	BINDING_ONLY(m_pModel, this, TeamChapter::updateView, D_EVENT_FRIENDS_DUNGEON);
	m_pModel->syncDungeon();
	DLayer::onEnter();
}

void TeamChapter::updateView()
{
	m_pTableView->reloadData();

	int cid = m_pModel->getDungeon()->getMaxClear();
/*	if (m_pModel->getSelectedChapter() != NULL)
	{
		cid = m_pModel->getSelectedChapter()->getID() - 1;
	}*/
	if (m_nSelected != UNSELECT)
	{
		cid = m_nSelected;
	}
	CCPoint offset = ccp(0, -(cid - 1) * tableCellSizeForIndex(m_pTableView, 0).height);
	offset.y = MIN(m_pTableView->maxContainerOffset().y, offset.y);
	offset.y = MAX(m_pTableView->minContainerOffset().y, offset.y);

	m_pTableView->setContentOffset(offset);
}

void TeamChapter::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	DLayer::onExit();
}

bool TeamChapter::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeContainer", CCNode *, m_pNodeContainer);
	return false;
}

CCTableViewCell* TeamChapter::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	TeamChapterCell *pCell = (TeamChapterCell *)table->dequeueCell();
	if (!pCell)
	{
		pCell = TeamChapterCell::create();
		pCell->setDelegate(this);
	}
	pCell->setData(static_cast<friends::ChapterInfo *>(m_pList->objectAtIndex(idx)));
	pCell->setNew(idx == numberOfCellsInTableView(table) - 1);
	return pCell;
}

uint32_t TeamChapter::numberOfCellsInTableView(CCTableView *table)
{
	if (!m_pList)
	{
		return 0;
	}
	if (m_pModel->getDungeon()->getMaxClear() + 1 > m_pList->count())
	{
		return m_pList->count();
	}
	return m_pModel->getDungeon()->getMaxClear() + 1;
}


CCSize TeamChapter::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
}

void TeamChapter::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}

void TeamChapter::TeamChapterCellSelected(TeamChapterCell *pCell)
{
	if (!pCell->getData()->getIsUnlock())
	{
		DM_GET_ERRORTIP_MODEL->showErrorTips(TEAM_CHAPTER_LOCK);
		return;
	}
	m_nSelected = pCell->getIdx();
	if (m_pDelegate)
	{
		m_pModel->setSelectedChapter(pCell->getData());
		m_pModel->selectFriend(NULL);
		m_pDelegate->TeamNavigateTo(kTeamNavMake);
	}
}

void TeamChapter::TeamChapterCellViewItem(TeamChapterCell *pCell)
{
	if (m_pDelegate)
	{
		m_pModel->setSelectedChapter(pCell->getData());
		m_pDelegate->TeamNavigateTo(kTeamNavTreasure);
	}
}