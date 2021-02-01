//
//  GuildDemiseView.h
//  公会让位
//
//  Created by Eci on 13-10-26.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildDemiseView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "GuildDemiseCell.h"
#include <algorithm>

#define UNSELECT -1


static int sortUsersByLevel(const CCObject* p1, const CCObject* p2)
{
	return ((guild::GuildUserBasics*)p1)->getLevel() > ((guild::GuildUserBasics*)p2)->getLevel();
};


GuildDemiseView::GuildDemiseView()
	:m_pModel(NULL)
	,m_pButtonCancel(NULL)
	,m_pCloseButton(NULL)
	,m_pButtonEnter(NULL)
	,m_pContainer(NULL)
	,m_pListView(NULL)
	,m_pDelegate(NULL)
	,m_iSelectIndex(UNSELECT)
	,m_pUserList(NULL)
{
}

GuildDemiseView::~GuildDemiseView()
{
	CC_SAFE_RELEASE(m_pButtonCancel);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pButtonEnter);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pListView);
	CC_SAFE_RELEASE(m_pUserList);
}

GuildDemiseView* GuildDemiseView::create()
{
	GuildDemiseView *pRet = new GuildDemiseView();
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

bool GuildDemiseView::init()
{
	if(!DPopup::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILD_MODEL;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_guanli_chuanwei");
	m_pListView = DTableView::create(this, m_pContainer->getContentSize());
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_pListView->retain();
	m_pContainer->addChild(m_pListView);

	m_pButtonEnter->getButton()->setTarget(this, menu_selector(GuildDemiseView::callBackEnter));
	m_pButtonCancel->getButton()->setTarget(this, menu_selector(GuildDemiseView::callBackCancel));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(GuildDemiseView::callBackCancel));
	return true;
}

void GuildDemiseView::onEnter()
{
	DPopup::onEnter();

	BINDING_EXEC(m_pModel, this, GuildDemiseView::updateView, D_EVENT_GUILD_GET);
	if (m_pModel->getGuildUser()->count() == 0)
	{
		HTTP_CLIENT->guildGet();
	}
}

void GuildDemiseView::updateView()
{

	CCArray * userList = m_pModel->getGuildUser();
	if (NULL == m_pUserList)
	{
		m_pUserList = CCArray::create();
		m_pUserList->retain();
	}
	else
	{
		m_pUserList->removeAllObjects();
	}
	m_pUserList->addObjectsFromArray(userList);
	uint32_t removeIndex = 0;
	for (uint32_t i = 0; i < m_pUserList->count(); i++)
	{
		guild::GuildUserBasics *pUser = (guild::GuildUserBasics *)m_pUserList->objectAtIndex(i);
		if (pUser->getUserId() == m_pModel->getSelfInfo()->getUserId())
		{
			removeIndex = i;
			break;
		}
	}
	m_pUserList->removeObjectAtIndex(removeIndex);
	std::sort(m_pUserList->data->arr, m_pUserList->data->arr + m_pUserList->data->num, sortUsersByLevel);
	m_pListView->reloadData();
}

void GuildDemiseView::onExit()
{
	DPopup::onExit();
	UNBINDING_ALL(m_pModel, this);
}

bool GuildDemiseView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_cancel", DButton *, this->m_pButtonCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_enter", DButton *, this->m_pButtonEnter);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);

	return false;
}

void GuildDemiseView::callBackEnter(CCObject *sender)
{
	if (UNSELECT == m_iSelectIndex)
	{
		return;
	}
	DMessageBox::showMessageBox(D_LOCAL_STRING("GuildDemiseConfirm").c_str(), this, messageBox_callfunc_selector(GuildDemiseView::confirmToDemise));
}

void GuildDemiseView::confirmToDemise(MessageType type,DMessageBox* targe)
{
	if(type == kMessageTypeCancel) return;
	if (UNSELECT == m_iSelectIndex)
	{
		return;
	}
	if (m_pDelegate)
	{
		guild::GuildUserBasics *pUser = (guild::GuildUserBasics *)m_pUserList->objectAtIndex(m_iSelectIndex);
		m_pDelegate->demiseToUser(pUser->getUserId());
	}
}

void GuildDemiseView::callBackCancel(CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->closeGuildDemiseView();
	}
	closePopup();
}


CCTableViewCell* GuildDemiseView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildDemiseCell *cell = (GuildDemiseCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildDemiseCell::create();
	}
	cell->setSelected(idx == m_iSelectIndex);
	guild::GuildUserBasics *pData = (guild::GuildUserBasics *)m_pUserList->objectAtIndex(idx);
	cell->setData(pData);
	return cell;
}

uint32_t GuildDemiseView::numberOfCellsInTableView(CCTableView *table)
{
	if (NULL == m_pUserList)
	{
		return 0;
	}
	else
	{
		return m_pUserList->count();
	}
	
}


CCSize GuildDemiseView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
		return DEMISE_CELL_SIZE;
}

void GuildDemiseView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	m_iSelectIndex = m_iSelectIndex == cell->getIdx() ?UNSELECT :cell->getIdx();
	DTableView * DTable = dynamic_cast<DTableView *>(table);
	DTable->reloadData();
}
