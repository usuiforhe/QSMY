//
//  FriendsView.h
//  好友主页
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "FriendsView.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "Friends.h"
#include "FriendsTable.h"


#define  FRIEND_TAB_VIEW		CCRectMake(0, 200, 640, 534)

#define BTN_TAG_REMOVE	0
#define BTN_TAG_MESSAGE	1

#define BTN_TAG_APPLY	0
#define BTN_TAG_RECOMMEND	1

using namespace sp;


FriendsView::FriendsView()
	: m_pTabView(NULL)
	, m_pNodeContainerFriends(NULL)
	, m_pNodeContainerAdd(NULL)
	, m_pNodeFriends(NULL)
	, m_pNodeAdd(NULL)
	, m_pBtnRemove(NULL)
	, m_pBtnMessage(NULL)
	, m_pBtnRecommend(NULL)
	, m_pBtnApply(NULL)
	, m_pBMFID(NULL)
	, m_pBMFCount(NULL)
	, m_pNodeNoApply(NULL)
	, m_pDBtnBottom(NULL)
	, m_pNodeSearch(NULL)
	, m_pNodeIgnore(NULL)
	, m_pNodeRequestNum(NULL)
	, m_pBMFRequestNum(NULL)
	, m_pDelegate(NULL)
	, m_pNowTable(NULL)
	, m_pAddTable(NULL)
	, m_pModel(NULL)
	, m_nTag(BTN_TAG_RECOMMEND)
{
}

FriendsView::~FriendsView()
{
	CC_SAFE_RELEASE(m_pTabView);
	CC_SAFE_RELEASE(m_pNodeContainerFriends);
	CC_SAFE_RELEASE(m_pNodeContainerAdd);
	CC_SAFE_RELEASE(m_pNodeFriends);
	CC_SAFE_RELEASE(m_pNodeAdd);
	CC_SAFE_RELEASE(m_pBtnRemove);
	CC_SAFE_RELEASE(m_pBtnMessage);
	CC_SAFE_RELEASE(m_pBtnRecommend);
	CC_SAFE_RELEASE(m_pBtnApply);
	CC_SAFE_RELEASE(m_pBMFID);
	CC_SAFE_RELEASE(m_pBMFCount);
	CC_SAFE_RELEASE(m_pNodeNoApply);
	CC_SAFE_RELEASE(m_pDBtnBottom);
	CC_SAFE_RELEASE(m_pNodeSearch);
	CC_SAFE_RELEASE(m_pNodeIgnore);
	CC_SAFE_RELEASE(m_pNodeRequestNum);
	CC_SAFE_RELEASE(m_pBMFRequestNum);
}

FriendsView* FriendsView::create()
{
	FriendsView *pRet = new FriendsView();
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

bool FriendsView::init()
{
	if(!DLayer::init())
	{
		return false;
	}


	cocos2d::CCNode *pRoot = NULL;
	CCB_READER("ui/ccb/friends/friends",pRoot);
	if(pRoot) addChild(pRoot);


	m_pModel = DM_GET_FRIENDS_MODEL;

	m_pTabView = DTabView::create(FRIEND_TAB_VIEW,270);
	m_pTabView->retain();
	m_pTabView->setDelegate(this);

	m_pNowTable = FriendsTable::create(m_pNodeContainerFriends);
	m_pNowTable->setType(FriendsViewCellTypeNow);
	//m_pTabView->addTabWithFontImg(friendTable, "ui/common/font_sheet_friendNow.png");
	m_pTabView->addTab(m_pNowTable, "", "ui/common/sheet_bg_wider.png", "ui/common/sheet_bg_wider_h.png", NULL, "ui/common/font_sheet_friendNow.png");

	m_pAddTable = FriendsTable::create(m_pNodeContainerAdd);
	m_pAddTable->setType(FriendsViewCellTypeAdd);
	m_pTabView->addTab(m_pAddTable, "", "ui/common/sheet_bg_wider.png", "ui/common/sheet_bg_wider_h.png", NULL, "ui/common/font_sheet_friendAdd.png");



	pRoot->addChild(m_pTabView);

	m_pBtnRemove->setTag(BTN_TAG_REMOVE);
	m_pBtnRemove->setTarget(this, menu_selector(FriendsView::callBackPopup));
	m_pBtnMessage->setTag(BTN_TAG_MESSAGE);
	m_pBtnMessage->setTarget(this, menu_selector(FriendsView::callBackPopup));

	m_pBtnApply->setTag(BTN_TAG_APPLY);
	m_pBtnApply->setTarget(this, menu_selector(FriendsView::callBackSwitch));
	m_pBtnRecommend->setTag(BTN_TAG_RECOMMEND);
	m_pBtnRecommend->setTarget(this, menu_selector(FriendsView::callBackSwitch));

	m_pDBtnBottom->getButton()->setTarget(this, menu_selector(FriendsView::callBackBottom));

	m_pNodeNoApply->setVisible(false);

	m_pNodeRequestNum->setZOrder(1);

	return true;
}

void FriendsView::onEnter()
{
	BINDING_EXEC(m_pModel, this, FriendsView::updateRequest, D_EVENT_FRIENDS_NEW_REQUEST);
	BINDING_ONLY(m_pModel, this, FriendsView::updateView, D_EVENT_FRIENDS_GET);
	//BINDING_EXEC(DM_GET_USER_MODEL, this, FriendsView::updateID, D_EVENT_USERINFO_UPDATE);
	updateID();
	m_pModel->syncFriends();
	DLayer::onEnter();
}

void FriendsView::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	DLayer::onExit();
}

bool FriendsView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeContainerFriends", CCNode *, m_pNodeContainerFriends);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeContainerAdd", CCNode *, m_pNodeContainerAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFriends", CCNode *, m_pNodeFriends);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAdd", CCNode *, m_pNodeAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRemove", CCMenuItem *, m_pBtnRemove);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnMessage", CCMenuItem *, m_pBtnMessage);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnApply", CCMenuItem *, m_pBtnApply);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRecommend", CCMenuItem *, m_pBtnRecommend);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFID", CCLabelBMFont *, m_pBMFID);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCount", CCLabelBMFont *, m_pBMFCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNoApply", CCNode *, m_pNodeNoApply);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnBottom", DButton *, m_pDBtnBottom);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeSearch", CCNode *, m_pNodeSearch);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeIgnore", CCNode *, m_pNodeIgnore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRequestNum", CCNode *, m_pNodeRequestNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRequestNum", CCLabelBMFont *, m_pBMFRequestNum);
	return false;
}

void FriendsView::updateView()
{
	int nVipLv = DM_GET_USER_MODEL->getUserInfo()->getVipLevel();
	if (nVipLv == 0)
	{
		nVipLv = 1;
	}

	user::VipInfo *pCurInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(nVipLv);
	uint32_t friendMax = pCurInfo->getFriendsMax();
	uint32_t friendNum = m_pModel->getFriendsCount();
	m_pBMFCount->setString(CCString::createWithFormat("%u/%u", friendNum, friendMax)->getCString());
	m_pNowTable->updateView();
	
	if (!friendNum)
	{
		m_pTabView->selectedTab(1);
	}
	
}

void FriendsView::updateRequest()
{
	uint32_t count = m_pModel->getRequestsCount();
	m_pBMFRequestNum->setString(D_CSTRING_FROM_UINT(count));
	m_pNodeRequestNum->setVisible(count > 0);
	if (m_eNav == kFriendsNavAdd && m_nTag == BTN_TAG_APPLY)
	{
		m_pModel->syncRequests();
	}
	
}

void FriendsView::SelectTabAtIndex(DTabView *tabView, uint32_t idx)
{
	m_pNodeFriends->setVisible(idx == 0);
	m_pNodeAdd->setVisible(!m_pNodeFriends->isVisible());

	
	if (idx == 0)
	{
		m_eNav = kFriendsNavNow;
	}
	else
	{
		m_eNav = kFriendsNavAdd;
		uint32_t count = m_pModel->getRequestsCount();
		if (count > 0)
		{
			callBackSwitch(m_pBtnApply);
		}
		else
		{
			callBackSwitch(m_pBtnRecommend);
		}
	}
}

void FriendsView::callBackPopup(CCObject *pSender)
{
	CCMenuItem *pBtn = static_cast<CCMenuItem *>(pSender);
	FriendsNav eNav = kFriendsNavHome;

	switch (pBtn->getTag())
	{
	case BTN_TAG_REMOVE:
		eNav = kFriendsNavRemove;
		break;
	case BTN_TAG_MESSAGE:
		eNav = kFriendsNavMessage;
		break;
	default:
		eNav = kFriendsNavHome;
	}
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(eNav);
	}
}

void FriendsView::callBackSwitch(CCObject *pSender)
{
	CCMenuItem *pBtn = static_cast<CCMenuItem *>(pSender);
	m_pBtnApply->unselected();
	m_pBtnRecommend->unselected();
	m_nTag = pBtn->getTag();
	switch (pBtn->getTag())
	{
	case BTN_TAG_APPLY:
		m_pBtnApply->selected();
		m_pAddTable->setType(FriendsViewCellTypeApply);
		break;
	case BTN_TAG_RECOMMEND:
		m_pBtnRecommend->selected();
		m_pAddTable->setType(FriendsViewCellTypeAdd);
		break;
	default:
		;
	}

	m_pNodeIgnore->setVisible(pBtn->getTag() == BTN_TAG_APPLY);
	m_pNodeSearch->setVisible(!m_pNodeIgnore->isVisible());
	uint32_t count = m_pModel->getRequestsCount();
	m_pNodeNoApply->setVisible(count == 0 && m_pNodeIgnore->isVisible());
	if (m_nTag == BTN_TAG_APPLY)
	{
		m_pModel->syncRequests();
	}
	else
	{
		m_pModel->syncRecommend();
	}
}

void FriendsView::setType(FriendsNav eNav)
{
	if (eNav == kFriendsNavAdd)
	{
		m_pTabView->selectedTab(1);
	}
	else
	{
		m_pTabView->selectedTab(0);
	}
}

void FriendsView::setDelegate(FriendsViewDelegate *pDelegate)
{
	m_pDelegate = pDelegate;
	m_pNowTable->setDelegate(pDelegate);
	m_pAddTable->setDelegate(pDelegate);
}

void FriendsView::updateID()
{
	m_pBMFID->setString(D_CSTRING_FROM_UINT(DM_GET_USER_MODEL->getUserInfo()->getUserId()));
}

void FriendsView::callBackBottom(CCObject *pSender)
{
	if (m_eNav == kFriendsNavNow)
	{
		m_pModel->refreshFriendsIfNeeded();
	}
	else if (m_nTag == BTN_TAG_APPLY)
	{
		if (m_pAddTable->numberOfCellsInTableView(NULL) > 0)
		{
			HTTP_CLIENT->refuseAllFriend();
		}
	}
	else
	{
		if (m_pDelegate)
		{
			m_pDelegate->FriendsNavigateTo(kFriendsNavSearch);
		}
	}
}