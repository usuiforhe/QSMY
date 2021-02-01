//
//  Friends.h
//  好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#include "Friends.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "FriendsView.h"
#include "../user/UserInfo.h"
#include "FriendsSelectView.h"
#include "FriendsSearchView.h"
#include "FriendsMessageView.h"
#include "FriendsTipsView.h"
#include "FriendsFoundView.h"

Friends::Friends()
	: m_pFocusLayer(NULL)
	, m_pPopupLayer(NULL)
	, m_pFriendsView(NULL)
	, m_eShowingNav(kFriendsNavHome)
	, m_ePopingNav(kFriendsNavNoPop)
	, m_pModel(NULL)
{
}

Friends::~Friends()
{
}

Friends* Friends::create()
{
	Friends *pRet = new Friends();
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

bool Friends::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView(),1);

	m_pFocusLayer = CCNode::create();
	addChild(m_pFocusLayer, 1);
	m_pPopupLayer = CCNode::create();
	addChild(m_pPopupLayer, 1);

	//m_pModel = DM_GET_Friends_MODEL;

	m_pFriendsView = FriendsView::create();
	m_pFriendsView->setDelegate(this);
	addChild(m_pFriendsView);

	m_pModel = DM_GET_FRIENDS_MODEL;

	return true;
}

void Friends::onEnter()
{
	updateRequest(0);
	DLayer::onEnter();
}

void Friends::onExit()
{
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
	DLayer::onExit();
	FriendsNavigateTo(kFriendsNavHome);
}

void Friends::onBackKey()
{
	if (m_ePopingNav != kFriendsNavNoPop)
	{
		FriendsNavigateTo(kFriendsNavNoPop);
		return;
	}
	if (m_eShowingNav != kFriendsNavHome)
	{
		FriendsNavigateTo(kFriendsNavHome);
		return;
	}
	BackKeyInterface::onBackKey();
}

void Friends::FriendsNavigateTo(FriendsNav nav)
{
	if (m_ePopingNav == nav || (m_ePopingNav == kFriendsNavNoPop && m_eShowingNav == nav))
	{
		return;
	}
	
	switch(nav)
	{
	case kFriendsNavHome:
	case kFriendsNavRemove:
	case kFriendsNavMessage:
	case kFriendsNavNow:
	case kFriendsNavAdd:
		m_pFocusLayer->removeAllChildren();
		m_pPopupLayer->removeAllChildren();
		m_ePopingNav = kFriendsNavNoPop;
		m_eShowingNav = nav;
		break;
	case kFriendsNavSend:
	case kFriendsNavRemoveConfirm:
	case kFriendsNavFullError:
	case kFriendsNavSearch:
	case kFriendsNavFound:
	case kFriendsNavNoPop:
		m_ePopingNav = nav;
		m_pPopupLayer->removeAllChildren();
		break;
	}
	switch(nav)
	{
	case kFriendsNavRemove:
	case kFriendsNavMessage:
		{
			FriendsSelectView *pView = FriendsSelectView::create();
			pView->setType(nav);
			pView->setDelegate(this);
			m_pFocusLayer->addChild(pView);
			break;
		}
	case kFriendsNavSearch:
		{
			FriendsSearchView *pView = FriendsSearchView::create();
			pView->setDelegate(this);
			m_pPopupLayer->addChild(pView);
			break;
		}
	case kFriendsNavSend:
		{
			FriendsMessageView *pView = FriendsMessageView::create();
			pView->setDelegate(this);
			pView->setName(m_pModel->getMsgReceiver()->getName());
			pView->setUserID(m_pModel->getMsgReceiver()->getFriendID());
			pView->setType(mail::kMailTypeFromFriend);
			m_pPopupLayer->addChild(pView);
			break;
		}
	case kFriendsNavRemoveConfirm:
	case kFriendsNavFullError:
		{
			FriendsTipsView *pView = FriendsTipsView::create();
			pView->setDelegate(this);
			pView->setNav(nav);
			m_pPopupLayer->addChild(pView);
			break;
		}
	case kFriendsNavFound:
		{
			FriendsFoundView *pView = FriendsFoundView::create();
			pView->setDelegate(this);
			m_pPopupLayer->addChild(pView);
			break;
		}
	case kFriendsNavNow:
	case kFriendsNavAdd:
		{
			m_pFriendsView->setType(nav);
			break;
		}
	case kFriendsNavHome:
	case kFriendsNavNoPop:
	default:
		break;
	}
}

void Friends::updateRequest(float dt)
{
	HTTP_CLIENT->friendGetNewRequest(this, callfuncJson_selector(Friends::callBackRequest));
}

void Friends::callBackRequest(const Json::Value &request, const Json::Value &response)
{
	if(response["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
	TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(Friends::updateRequest), this, D_CONFIG_UINT_FOR_KEY(FRIENDS_REQUEST_REFRESH_INTERVAL));
}