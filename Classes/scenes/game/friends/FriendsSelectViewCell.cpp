//
//  FriendsSelectViewCell.h
//  选择好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "FriendsSelectViewCell.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
using namespace sp;

FriendsSelectViewCell::FriendsSelectViewCell()
	: m_pTTFName(NULL)
	, m_pNodeAvatorContainer(NULL)
	, m_pBMFLevel(NULL)
	, m_pBtnCheck(NULL)
	, m_pDelegate(NULL)
	, m_bSelected(false)
{
}

FriendsSelectViewCell::~FriendsSelectViewCell()
{
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pNodeAvatorContainer);
	CC_SAFE_RELEASE(m_pBMFLevel);
	CC_SAFE_RELEASE(m_pBtnCheck);
}

FriendsSelectViewCell* FriendsSelectViewCell::create()
{
	FriendsSelectViewCell *pRet = new FriendsSelectViewCell();
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

bool FriendsSelectViewCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("ui/ccb/friends/friendsDeleteMailCell");

	m_pBtnCheck->setTarget(this, menu_selector(FriendsSelectViewCell::callBackCheck));
	return true;
}

void FriendsSelectViewCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void FriendsSelectViewCell::onExit()
{
	CCTableViewCell::onExit();
}

bool FriendsSelectViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAvatorContainer", CCNode *, m_pNodeAvatorContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFLevel", CCLabelBMFont *, m_pBMFLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnCheck", CCMenuItem *, m_pBtnCheck);
	return false;
}

SEL_MenuHandler FriendsSelectViewCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackCheck", FriendsSelectViewCell::callBackCheck);
	return NULL;
}

void FriendsSelectViewCell::callBackCheck(CCObject *pSender)
{
	setSelected(!m_bSelected);
	if (m_pDelegate)
	{
		m_pDelegate->FriendsSelectViewCellSelected(this);
	}
}

void FriendsSelectViewCell::setSelected(bool bSelected)
{
	if (bSelected)
	{
		m_pBtnCheck->selected();
	}
	else
	{
		m_pBtnCheck->unselected();
	}
	m_bSelected = bSelected;
}

void FriendsSelectViewCell::setData(friends::FriendInfo *pData)
{
	if (!pData)
	{
		return;
	}
	m_pTTFName->setString(pData->getName().c_str());
	m_pNodeAvatorContainer->removeAllChildren();
	m_pNodeAvatorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(pData->getAvatarID()));
	m_pBMFLevel->setString(D_CSTRING_FROM_UINT(pData->getLevel()));
}