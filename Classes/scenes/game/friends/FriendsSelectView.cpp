//
//  FriendsSelectView.h
//  选择好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#define UNSELECT -1

#include "FriendsSelectView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "FriendsSelectViewCell.h"
#include <algorithm>

FriendsSelectView::FriendsSelectView()
	: m_pNodeRemove(NULL)
	, m_pNodeMessage(NULL)
	, m_pDBtnBack(NULL)
	, m_pBMFCount(NULL)
	, m_pDBtnRemove(NULL)
	, m_pDBtnMessage(NULL)
	, m_pNodeContainer(NULL)
	, m_pCell(NULL)
	, m_pTableView(NULL)
	, m_pDelegate(NULL)
	, m_vRemove()
	, m_uSelected(UNSELECT)
	, m_eNav(kFriendsNavMessage)
	, m_pModel(NULL)
	, m_pList(NULL)
{
}

FriendsSelectView::~FriendsSelectView()
{
	CC_SAFE_RELEASE(m_pNodeRemove);
	CC_SAFE_RELEASE(m_pNodeMessage);
	CC_SAFE_RELEASE(m_pDBtnBack);
	CC_SAFE_RELEASE(m_pBMFCount);
	CC_SAFE_RELEASE(m_pDBtnRemove);
	CC_SAFE_RELEASE(m_pDBtnMessage);
	CC_SAFE_RELEASE(m_pNodeContainer);
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pList);
}

FriendsSelectView* FriendsSelectView::create()
{
	FriendsSelectView *pRet = new FriendsSelectView();
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

bool FriendsSelectView::init()
{
	if(!DLayer::init())
	{
		return false;
	}


	CCB_READER_AND_ADDCHILD("ui/ccb/friends/friendsDeleteMail");

	CCB_READER("ui/ccb/friends/friendsDeleteMailCell", m_pCell);
	CC_SAFE_RETAIN(m_pCell);

	m_pTableView = DTableView::create(this, m_pNodeContainer->getContentSize());
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pNodeContainer->addChild(m_pTableView);

	m_pDBtnBack->getButton()->setTarget(this, menu_selector(FriendsSelectView::callBackBack));
	m_pDBtnRemove->getButton()->setTarget(this, menu_selector(FriendsSelectView::callBackRemove));
	m_pDBtnMessage->getButton()->setTarget(this, menu_selector(FriendsSelectView::callBackMessage));

	m_pModel = DM_GET_FRIENDS_MODEL;
	m_pList = CCArray::create();
	CC_SAFE_RETAIN(m_pList);

	return true;
}

void FriendsSelectView::onEnter()
{
	updateView();
	DLayer::onEnter();
}

void FriendsSelectView::updateView()
{
	m_pModel->getFriends(m_pList);
	m_pTableView->reloadData();
	int nVipLv = DM_GET_USER_MODEL->getUserInfo()->getVipLevel();
	if (nVipLv == 0)
	{
		nVipLv = 1;
	}

	user::VipInfo *pCurInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(nVipLv);
	uint32_t friendMax = pCurInfo->getFriendsMax();
	uint32_t friendNum = m_pModel->getFriendsCount();
	m_pBMFCount->setString(CCString::createWithFormat("%u/%u", friendNum, friendMax)->getCString());
}

void FriendsSelectView::onExit()
{
	DLayer::onExit();
}

bool FriendsSelectView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDelete", CCNode *, m_pNodeRemove);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeMail", CCNode *, m_pNodeMessage);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnBack", DButton *, m_pDBtnBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCount", CCLabelBMFont *, m_pBMFCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnDelete", DButton *, m_pDBtnRemove);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnSend", DButton *, m_pDBtnMessage);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeContainer", CCNode *, m_pNodeContainer);
	return false;
}

CCTableViewCell* FriendsSelectView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	FriendsSelectViewCell *pCell = (FriendsSelectViewCell *)table->dequeueCell();
	if (!pCell)
	{
		pCell = FriendsSelectViewCell::create();
		pCell->setDelegate(this);
	}
		
	if (m_eNav == kFriendsNavMessage)
	{
		pCell->setSelected(m_uSelected == idx);
	}
	else
	{
		pCell->setSelected(std::find(m_vRemove.begin(), m_vRemove.end(), idx) != m_vRemove.end());
	}
	pCell->setData((friends::FriendInfo *)m_pList->objectAtIndex(idx));
	return pCell;
}

uint32_t FriendsSelectView::numberOfCellsInTableView(CCTableView *table)
{
	if (!m_pList)
	{
		return 0;
	}
	return m_pList->count();
}


CCSize FriendsSelectView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
}

void FriendsSelectView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}

void FriendsSelectView::setType(FriendsNav eType)
{
	m_pNodeMessage->setVisible(eType == kFriendsNavMessage);
	m_pNodeRemove->setVisible(!m_pNodeMessage->isVisible());
	m_eNav = eType;
}

void FriendsSelectView::callBackBack(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(kFriendsNavHome);
	}
}

void FriendsSelectView::callBackRemove(CCObject *pSender)
{
	if (m_vRemove.size() == 0)
	{
		return;
	}
	CCArray *pRemove = CCArray::create();
	for (std::vector<uint32_t>::iterator iter = m_vRemove.begin(); iter != m_vRemove.end(); iter++)
	{
		pRemove->addObject(m_pList->objectAtIndex(*iter));
	}
	m_pModel->setRemoveFriends(pRemove);
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(kFriendsNavRemoveConfirm);
	}
}

void FriendsSelectView::callBackMessage(CCObject *pSender)
{
	if (m_uSelected == UNSELECT)
	{
		return;
	}
	m_pModel->setMsgReceiver((friends::FriendInfo *)m_pList->objectAtIndex(m_uSelected));
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(kFriendsNavSend);
	}
}

void FriendsSelectView::FriendsSelectViewCellSelected(FriendsSelectViewCell *pCell)
{
	m_uSelected = pCell->getIdx();
	std::vector<uint32_t>::iterator iter = find(m_vRemove.begin(), m_vRemove.end(), m_uSelected);
	if (iter != m_vRemove.end())
	{
		m_vRemove.erase(iter);
	}
	else
	{
		m_vRemove.push_back(m_uSelected);
	}
	if (m_eNav == kFriendsNavMessage)
	{
		m_pTableView->reloadData();
	}
}