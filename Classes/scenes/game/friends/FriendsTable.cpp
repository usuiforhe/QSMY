//
//  FriendsTable.h
//  好友列表
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "FriendsTable.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "Friends.h"
#include "../common/OtherFormationView.h"

#define UNSELECTED	-1

FriendsTable::FriendsTable()
	: m_eType(FriendsViewCellTypeNow)
	, m_pTableView(NULL)
	, m_pCell(NULL)
	, m_pDelegate(NULL)
	, m_pModel(NULL)
	, m_pList(NULL)
	, m_uSelected(UNSELECTED)
{
}

FriendsTable::~FriendsTable()
{
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pList);
}

FriendsTable* FriendsTable::create(CCNode *pContainer)
{
	FriendsTable *pRet = new FriendsTable();
	if (pRet && pRet->init(pContainer))
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

bool FriendsTable::init(CCNode *pContainer)
{
	if(!DLayer::init())
	{
		return false;
	}

	if (!pContainer)
	{
		return false;
	}

	CCB_READER("ui/ccb/friends/friendsListSelectCell", m_pCell);
	CC_SAFE_RETAIN(m_pCell);

	m_pList = CCArray::create();
	CC_SAFE_RETAIN(m_pList);

	m_pTableView = DTableView::create(this, pContainer->getContentSize());
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	pContainer->addChild(m_pTableView);

	m_pModel = DM_GET_FRIENDS_MODEL;
	return true;
}

void FriendsTable::onEnter()
{
	BINDING_ONLY(m_pModel, this, FriendsTable::updateView, D_EVENT_FRIENDS_GET);
	BINDING_ONLY(m_pModel, this, FriendsTable::updateView, D_EVENT_FRIENDS_REQUESTS);
	BINDING_ONLY(m_pModel, this, FriendsTable::updateView, D_EVENT_FRIENDS_RECOMMEND);
	DLayer::onEnter();
}

void FriendsTable::updateView()
{
	switch (m_eType)
	{
	case FriendsViewCellTypeNow:
		m_pModel->getFriends(m_pList);
		break;
	case FriendsViewCellTypeApply:
		m_pModel->getRequests(m_pList);
		break;
	case FriendsViewCellTypeAdd:
		m_pModel->getRecommend(m_pList);
		break;
	default:;
	}
	m_pTableView->reloadData();
	m_pTableView->setContentOffset(m_pTableView->minContainerOffset());
	if (m_uSelected != UNSELECTED)
	{
		CCPoint offset = ccp(0, (int)(m_uSelected - this->numberOfCellsInTableView(m_pTableView) + 1) * tableCellSizeForIndex(m_pTableView, 0).height);
		offset.y = MIN(m_pTableView->maxContainerOffset().y, offset.y);
		offset.y = MAX(m_pTableView->minContainerOffset().y, offset.y);

		m_pTableView->setContentOffset(offset);
	}
}
	

void FriendsTable::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	DLayer::onExit();
}

CCTableViewCell* FriendsTable::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	FriendsViewCell *pCell = (FriendsViewCell *)table->dequeueCell();
	if (!pCell)
	{
		pCell = FriendsViewCell::create();
		pCell->setDelegate(this);
	}
	pCell->setType(m_eType);
	pCell->setData((friends::FriendInfo *)m_pList->objectAtIndex(idx));
	return pCell;
}

uint32_t FriendsTable::numberOfCellsInTableView(CCTableView *table)
{
	if (!m_pList)
	{
		return 0;
	}
	return m_pList->count();
}


CCSize FriendsTable::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
}

void FriendsTable::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}

void FriendsTable::setType(FriendsViewCellType eType)
{
	if (eType == m_eType)
	{
		return;
	}
	m_eType = eType;
	m_uSelected = UNSELECTED;
	updateView();
	//m_pTableView->reloadData();
}


void FriendsTable::FriendsViewCellGift(FriendsViewCell *pCell)
{
	HTTP_CLIENT->friendSendGift(pCell->getData()->getFriendID());
}

void FriendsTable::FriendsViewCellReceive(FriendsViewCell *pCell)
{
	HTTP_CLIENT->friendReceiveGift(pCell->getData()->getFriendID());
}

void FriendsTable::FriendsViewCellFormation(FriendsViewCell *pCell)
{
	OtherFormationView *m_pOtherFormationView = OtherFormationView::create();
	//m_pOtherFormationView->setPopupDelegate(this);
	setVisible(false);
	m_pOtherFormationView->setFormationInfo(DM_GET_OTHERFORMATION_MODEL->getForamtionInfoByUid(pCell->getData()->getFriendID()));
	Game::sharedGame()->openPopup(m_pOtherFormationView);
}

void FriendsTable::FriendsViewCellAdd(FriendsViewCell *pCell)
{
	if (m_pModel->isFriendsFull())
	{
		DM_GET_ERRORTIP_MODEL->showErrorTips(FRIENDS_SELF_FULL);
		return;
	}
	m_uSelected = pCell->getIdx();
	HTTP_CLIENT->addFriend(pCell->getData()->getFriendID());
}

void FriendsTable::FriendsViewCellAgree(FriendsViewCell *pCell)
{
	if (m_pModel->isFriendsFull())
	{
		DM_GET_ERRORTIP_MODEL->showErrorTips(FRIENDS_SELF_FULL);
		return;
	}
	HTTP_CLIENT->acceptFriend(pCell->getData()->getFriendID());
}

void FriendsTable::FriendsViewCellRefuse(FriendsViewCell *pCell)
{
	HTTP_CLIENT->refuseFriend(pCell->getData()->getFriendID());
}