//
//  GuildRankTable.h
//  公会排名
//
//  Created by Eci on 13-10-24.

//  Copyright (c) 2013年 thedream. All rights reserved.
//



#include "GuildRankTable.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "GuildRankCell.h"


#define GUILD_RANK_PAGE_SIZE 20
#define UNSELECT -1

GuildRankTable::GuildRankTable()
	:m_pModel(NULL)
	,m_pListView(NULL)
	,m_iSelectIndex(UNSELECT)
	,m_iType(0)
	,m_iLastCount(0)
	,m_pDelegate(NULL)
{
}

GuildRankTable::~GuildRankTable()
{
	CC_SAFE_RELEASE(m_pListView);
}

GuildRankTable* GuildRankTable::create(const cocos2d::CCSize tableSize , int val)
{
	GuildRankTable *pRet = new GuildRankTable();
	if (pRet && pRet->init(tableSize , val))
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

bool GuildRankTable::init(const cocos2d::CCSize tableSize , int val)
{
	if(!DLayer::init())
	{
		return false;
	}
	m_iType = val;
	m_pModel = DM_GET_GUILD_MODEL;

	m_pListView = DLoadingTableView::create(this, tableSize);
	m_pListView->setPageSize(GUILD_RANK_PAGE_SIZE);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setLoadingDelegate(this);
	m_pListView->retain();
	this->addChild(m_pListView);

	//暂时屏蔽掉loading功能
	m_pListView->setLoadingEnabled(false);

	return true;
}

void GuildRankTable::onEnter()
{
	DLayer::onEnter();

	if (m_iType == 0)
	{
		BINDING_ONLY(m_pModel, this, GuildRankTable::updateView, D_EVENT_GUILD_GET_LIST);
		if (m_pModel->getGuildList() == NULL || m_pModel->getGuildList()->count() == 0)
		{
			HTTP_CLIENT->guildGetList(0, this, callfuncJson_selector(GuildRankTable::startLoading));
		}
		else
		{
			this->updateView();
		}
	}
	else
	{
		BINDING_ONLY(m_pModel, this, GuildRankTable::updateView, D_EVENT_GUILD_GET);
		if (m_pModel->getGuildUser()->count() == 0)
		{
			HTTP_CLIENT->guildGet();
		}
		else
		{
			this->updateView();
		}
	}
}

void GuildRankTable::updateView()
{
	m_pListView->reloadData();
// 	if (m_pModel->getGuildList()->count() - m_iLastCount < GUILD_RANK_PAGE_SIZE)
// 	{
// 		m_pListView->setReachTheEnd(true);
// 	}
// 	else
// 	{
// 		m_pListView->setReachTheEnd(false);
// 	}
}

void GuildRankTable::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
}

CCTableViewCell* GuildRankTable::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildRankCell *cell = (GuildRankCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildRankCell::create(m_iType);
	}
	CCArray * pList;
	if (m_iType == 0)
	{
		pList = m_pModel->getGuildList();
	}
	else
	{
		pList = m_pModel->getGuildUser();
	}
	ValueObjcet *pData = (ValueObjcet *)pList->objectAtIndex(idx);
	cell->setData(pData);
	cell->setRank(idx + 1);
	return cell;
}

uint32_t GuildRankTable::numberOfCellsInTableView(CCTableView *table)
{
	CCArray * pList;
	if (m_iType == 0)
	{
		pList = m_pModel->getGuildList();
	}
	else
	{
		pList = m_pModel->getGuildUser();
	}
	if (NULL == pList)
	{
		return 0;
	}
	else
	{
		return pList->count();
	}
}


CCSize GuildRankTable::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return RANK_CELL_SIZE;
}

void GuildRankTable::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	if (m_iType != 0)
	{
		return;
	}
	if (m_pDelegate)
	{
		GuildRankCell *pCell = (GuildRankCell *)cell;
		m_pDelegate->willShowGuildDetails(((guild::GuildBasics *)pCell->getData())->getGuildID(), pCell->getRank());
	}
}

void GuildRankTable::setType(uint32_t iType)
{
	if(iType == m_iType)
	{
		return;
	}  
	m_iType = iType;   
	m_pListView->reloadData();
	if (m_iType)
	{
		m_pListView->setLoadingEnabled(false);
	}
}

void GuildRankTable::startLoading()
{
	if (m_iType != 0)
	{
		return;
	}
	m_iLastCount = m_pModel->getGuildList()->count();
	HTTP_CLIENT->guildGetList(m_pModel->getGuildList()->count(), this, callfuncJson_selector(GuildRankTable::startLoading));
}


void GuildRankTable::startLoading(const Json::Value &,const Json::Value &responseData)
{
	m_pListView->endLoading();
// 	if (0 != m_iLastCount)
// 	{
// 		m_pListView->endLoading();
// 	}
// 	if (m_pModel->getGuildList()->count() - m_iLastCount < GUILD_RANK_PAGE_SIZE)
// 	{
// 		m_pListView->setReachTheEnd(true);
// 	}
}
