//
//  GuildApplyTable.h
//  公会申请
//
//  Created by Eci on 13-10-25.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildApplyTable.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#define UNSELECT -1

GuildApplyTable::GuildApplyTable()
	:m_pModel(NULL)
	,m_pListView(NULL)
	,m_iSelectIndex(UNSELECT)
	,m_iType(0)
{
}

GuildApplyTable::~GuildApplyTable()
{
	CC_SAFE_RELEASE(m_pListView);
}

GuildApplyTable* GuildApplyTable::create(const cocos2d::CCSize tableSize)
{
	GuildApplyTable *pRet = new GuildApplyTable();
	if (pRet && pRet->init(tableSize))
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

bool GuildApplyTable::init(const cocos2d::CCSize tableSize)
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILD_MODEL;

	m_pListView = DTableView::create(this, tableSize);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->retain();
	this->addChild(m_pListView);

	return true;
}

void GuildApplyTable::onEnter()
{
	DLayer::onEnter();

	BINDING_ONLY(m_pModel, this, GuildApplyTable::updateView, D_EVENT_GUILD_APPLY_LIST);
//#ifdef QSMY_DEBUG
//	HTTP_CLIENT->guildApplyList();
//#endif
//	if (m_pModel->getGuildApplication() == NULL)
//	{
//		HTTP_CLIENT->guildApplyList();
//	}
//	else
//	{
		this->updateView();
//	}
}

void GuildApplyTable::updateView()
{
	m_pListView->reloadData();
}

void GuildApplyTable::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
}

CCTableViewCell* GuildApplyTable::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildApplyCell *cell = (GuildApplyCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildApplyCell::create();
		cell->setDelegate(this);
		//cell->getSelectButton()->setTarget(this, menu_selector(GuildApplyTable::onTouch));
	}
	CCArray * pList = m_pModel->getGuildApplication();
	ValueObjcet *pData = (ValueObjcet *)pList->objectAtIndex(idx);
	cell->setData(pData);
	//cell->getSelectButton()->setUserData(cell);
	//cell->setExpand(idx == m_iSelectIndex);
	return cell;
}

uint32_t GuildApplyTable::numberOfCellsInTableView(CCTableView *table)
{
	CCArray * pList = m_pModel->getGuildApplication();
	if (NULL == pList)
	{
		return 0;
	}
	else
	{
		return pList->count();
	}
}


CCSize GuildApplyTable::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	if (idx == m_iSelectIndex)
	{
		return APPLY_CELL_SIZE_EXPAND;
	}
	else
	{
		return APPLY_CELL_SIZE;
	}
}

void GuildApplyTable::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}

void GuildApplyTable::setType(uint32_t iType)
{
	if(iType == m_iType)
	{
		return;
	}  
	m_iType = iType;   
	m_pListView->reloadData();
}


void GuildApplyTable::onTouch(cocos2d::CCObject* obj)
{
	cocos2d::CCMenuItem* pItem = static_cast<cocos2d::CCMenuItem*>(obj);

	GuildApplyCell* pCell = static_cast<GuildApplyCell*>(pItem->getUserData());
	if(!pCell) return;
	guild::GuildPosition iPosition = m_pModel->getSelfInfo()->getPosition();
	if (guild::GuildPositionAssistant != iPosition && guild::GuildPositionPresident != iPosition)
	{
		return;
	}
	//CCLOG("equip cell touched at index: %i,%i", idx,pCell->getIdx());
	if(m_iSelectIndex == pCell->getIdx())
	{
		m_iSelectIndex = - 1;
		m_pListView->updateCellPositions(pCell);
		m_pListView->reloadData();
	}
	else
	{
		m_iSelectIndex = pCell->getIdx();
		m_pListView->updateCellPositions(pCell);
		m_pListView->reloadData();
	}
}


void GuildApplyTable::willDealApply(uint32_t applyID, int actoin)
{
	if (actoin == 1)
	{
		if (m_pModel->getGuildUser()->count() >= D_CONFIG_UINT_FOR_KEY(GUILD_MEMBER_MAX))
		{
			Tips_Alert(D_LOCAL_STRING("GuildDealApplyError").c_str());
			return;
		}
	}
	HTTP_CLIENT->guildDealApply(applyID, actoin, this, callfuncJson_selector(GuildApplyTable::willDealApply));
}

void GuildApplyTable::willDealApply(const Json::Value &,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) 
	{
		if(responseData["code"].asString().compare(GUILD_APPLICATION_CANCELED)==0)
		{
			Tips_Alert(D_LOCAL_STRING("GuildApplicationCanceled").c_str());
			HTTP_CLIENT->guildApplyList();
			return;
		}
	}
	if(!isRunning()) return;
	m_pListView->reloadData();
}