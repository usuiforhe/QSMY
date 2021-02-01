//
//  FriendsTipsView.h
//  弹窗
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "FriendsTipsView.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "Friends.h"

using namespace sp;

FriendsTipsView::FriendsTipsView()
	: m_pDBtnEnter(NULL)
	, m_pDBtnCancel(NULL)
	, m_pTTFFullTips(NULL)
	, m_pTTFRemoveTips(NULL)
	, m_pBMFCount(NULL)
	, m_pNodeFullError(NULL)
	, m_pNodeRemoveConfirm(NULL)
	, m_pDelegate(NULL)
	, m_eNav(kFriendsNavFullError)
	, m_pModel(NULL)
{
}

FriendsTipsView::~FriendsTipsView()
{
	CC_SAFE_RELEASE(m_pDBtnCancel);
	CC_SAFE_RELEASE(m_pDBtnEnter);
	CC_SAFE_RELEASE(m_pTTFFullTips);
	CC_SAFE_RELEASE(m_pTTFRemoveTips);
	CC_SAFE_RELEASE(m_pBMFCount);
	CC_SAFE_RELEASE(m_pNodeFullError);
	CC_SAFE_RELEASE(m_pNodeRemoveConfirm);
}

FriendsTipsView* FriendsTipsView::create()
{
	FriendsTipsView *pRet = new FriendsTipsView();
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

bool FriendsTipsView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("ui/ccb/friends/friendsTipsBox");

	m_pDBtnEnter->getButton()->setTarget(this, menu_selector(FriendsTipsView::callBackEnter));
	m_pDBtnCancel->getButton()->setTarget(this, menu_selector(FriendsTipsView::callBackCancel));

	m_pTTFRemoveTips->setString(D_LOCAL_STRING("FriendsDeleteConfirm").c_str());
	m_pTTFFullTips->setString(D_LOCAL_STRING("FriendsFullError").c_str());

	m_pModel = DM_GET_FRIENDS_MODEL;
	if (m_pModel->getRemoveFriends())
	{
		m_pBMFCount->setString(D_CSTRING_FROM_UINT(m_pModel->getRemoveFriends()->count()));
	}

	return true;
}

void FriendsTipsView::onEnter()
{
	DLayer::onEnter();
}

void FriendsTipsView::onExit()
{
	DLayer::onExit();
}

bool FriendsTipsView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnCancel", DButton *, m_pDBtnCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnEnter", DButton *, m_pDBtnEnter);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFFullTips", CCLabelTTF *, m_pTTFFullTips);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFRemoveTips", CCLabelTTF *, m_pTTFRemoveTips);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCount", CCLabelBMFont *, m_pBMFCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFullError", CCNode *, m_pNodeFullError);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRemoveConfirm", CCNode *, m_pNodeRemoveConfirm);
	return false;
}

void FriendsTipsView::callBackEnter(CCObject *pSender)
{
	if (m_eNav == kFriendsNavRemoveConfirm)
	{
		m_pModel->removeFriendsServer();
	}
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(kFriendsNavNow);
	}
}

void FriendsTipsView::callBackCancel(CCObject *pSender)
{
	if (m_eNav == kFriendsNavRemoveConfirm)
	{
		m_pModel->setRemoveFriends(NULL);
	}
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(kFriendsNavNoPop);
	}
}

void FriendsTipsView::setNav(FriendsNav eNav)
{
	m_eNav = eNav;
	m_pNodeFullError->setVisible(eNav == kFriendsNavFullError);
	m_pNodeRemoveConfirm->setVisible(!m_pNodeFullError->isVisible());
}