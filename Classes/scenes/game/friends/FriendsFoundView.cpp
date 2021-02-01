//
//  FriendsFoundView.h
//  找到好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "FriendsFoundView.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "Friends.h"

using namespace sp;

FriendsFoundView::FriendsFoundView()
	: m_pTTFName(NULL)
	, m_pTTFTips(NULL)
	, m_pBMFLevel(NULL)
	, m_pContainer(NULL)
	, m_pRgbaGrade(NULL)
	, m_pDelegate(NULL)
	, m_pModel(NULL)
{
}

FriendsFoundView::~FriendsFoundView()
{
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pTTFTips);
	CC_SAFE_RELEASE(m_pBMFLevel);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pRgbaGrade);
}

FriendsFoundView* FriendsFoundView::create()
{
	FriendsFoundView *pRet = new FriendsFoundView();
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

bool FriendsFoundView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("ui/ccb/friends/friendFound");

	m_pTTFName->setString("");
	m_pTTFTips->setString("");

	m_pModel = DM_GET_FRIENDS_MODEL;

	return true;
}

void FriendsFoundView::onEnter()
{
	updateView();
	DLayer::onEnter();
}

void FriendsFoundView::onExit()
{
	DLayer::onExit();
}

void FriendsFoundView::updateView()
{
	if (NULL == m_pModel->getFound())
	{
		return;
	}
	m_pTTFName->setString(m_pModel->getFound()->getName().c_str());
	m_pTTFTips->setString(D_LOCAL_STRING("FriendCofirm%s", m_pModel->getFound()->getName().c_str()).c_str());
	m_pBMFLevel->setString(D_CSTRING_FROM_UINT(m_pModel->getFound()->getLevel()));
	m_pContainer->removeAllChildren();
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon60PForFriendFound(m_pModel->getFound()->getAvatarID()));
	warrior::WarriorBaseInfo *pBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pModel->getFound()->getLeaderID());
	m_pRgbaGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByColorGrade(pBase->getColorGrade()));
}

bool FriendsFoundView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFTips", CCLabelTTF *, m_pTTFTips);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFLevel", CCLabelBMFont *, m_pBMFLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRgbaGrade", CCNodeRGBA *, m_pRgbaGrade);
	return false;
}


SEL_MenuHandler FriendsFoundView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackClose", FriendsFoundView::callBackClose);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackAdd", FriendsFoundView::callBackAdd);
	return NULL;
}

void FriendsFoundView::callBackAdd(CCObject *pSender)
{
	if (m_pModel->isFriendsFull())
	{
		DM_GET_ERRORTIP_MODEL->showErrorTips(FRIENDS_SELF_FULL);
		return;
	}
	HTTP_CLIENT->addFriend(m_pModel->getFound()->getFriendID(), this, callfuncJson_selector(FriendsFoundView::callBackAdd));
}

void FriendsFoundView::callBackClose(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(kFriendsNavNoPop);
	}
}


void FriendsFoundView::callBackAdd(const Json::Value &request, const Json::Value &response)
{
	if(response["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(kFriendsNavNoPop);
	}
}