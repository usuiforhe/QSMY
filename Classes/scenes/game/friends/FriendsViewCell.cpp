//
//  FriendsViewCell.h
//  好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "FriendsViewCell.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "Friends.h"

using namespace sp;

FriendsViewCell::FriendsViewCell()
	: m_pNodeFriend(NULL)
	, m_pNodeAdd(NULL)
	, m_pTTFName(NULL)
	, m_pNodeAvatorContainer(NULL)
	, m_pBMFLevel(NULL)
	, m_pBtnFriend(NULL)
	, m_pBtnFormation(NULL)
	, m_pNodeReceive(NULL)
	, m_pNodeFormation(NULL)
	, m_pNodeGift(NULL)
	, m_pNodeReturn(NULL)
	, m_pBtnAdd(NULL)
	, m_pBtnAgree(NULL)
	, m_pAnimatePower(NULL)
	, m_pAnimateCoins(NULL)
	, m_pNodeApply(NULL)
	, m_pNodeInvite(NULL)
	, m_pNodeInvited(NULL)
	, m_eStatus(friends::kGiftStatePresent)
	, m_pDelegate(NULL)
	, m_pRoot(NULL)
	, m_pModel(NULL)
	, m_pData(NULL)
	, m_pBMFPower(NULL)
	, m_pBMFCoins(NULL)
{
}

FriendsViewCell::~FriendsViewCell()
{
	CC_SAFE_RELEASE(m_pNodeFriend);
	CC_SAFE_RELEASE(m_pNodeAdd);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pNodeAvatorContainer);
	CC_SAFE_RELEASE(m_pBMFLevel);
	CC_SAFE_RELEASE(m_pBtnFriend);
	CC_SAFE_RELEASE(m_pBtnFormation);
	CC_SAFE_RELEASE(m_pNodeReceive);
	CC_SAFE_RELEASE(m_pNodeFormation);
	CC_SAFE_RELEASE(m_pNodeGift);
	CC_SAFE_RELEASE(m_pNodeReturn);
	CC_SAFE_RELEASE(m_pBtnAdd);
	CC_SAFE_RELEASE(m_pBtnAgree);
	CC_SAFE_RELEASE(m_pAnimatePower);
	CC_SAFE_RELEASE(m_pAnimateCoins);
	CC_SAFE_RELEASE(m_pNodeApply);
	CC_SAFE_RELEASE(m_pNodeInvite);
	CC_SAFE_RELEASE(m_pNodeInvited);
	CC_SAFE_RELEASE(m_pData);
}

FriendsViewCell* FriendsViewCell::create()
{
	FriendsViewCell *pRet = new FriendsViewCell();
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

bool FriendsViewCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	CCB_READER("ui/ccb/friends/friendsListSelectCell",m_pRoot);
	if(m_pRoot)
	{
		addChild(m_pRoot);
	}
	setStatus(friends::kGiftStatePresent);

	m_pBtnFriend->setTarget(this, menu_selector(FriendsViewCell::callBackFriend));
	m_pBtnFormation->setTarget(this, menu_selector(FriendsViewCell::callBackFriend));
	m_pBtnAdd->setTarget(this, menu_selector(FriendsViewCell::callBackAdd));
	m_pBtnAgree->setTarget(this, menu_selector(FriendsViewCell::callBackAgree));

	m_pRoot->setContentSize(CCSizeMake(0, 0));

	m_pModel = DM_GET_FRIENDS_MODEL;


	SPContainer* pContainer = SPContainer::create(sp::ZORDER);
	m_pBMFPower = CCLabelBMFont::create("", "fonts/number.fnt");
	m_pBMFPower->setAnchorPoint(ccp(0,0.5));
	pContainer->addChild(m_pBMFPower);
	m_pAnimatePower->replaceNodeInBone(pContainer, "num");

	pContainer = SPContainer::create(sp::ZORDER);
	m_pBMFCoins = CCLabelBMFont::create("", "fonts/number.fnt");
	m_pBMFCoins->setAnchorPoint(ccp(0,0.5));
	pContainer->addChild(m_pBMFCoins);
	m_pAnimateCoins->replaceNodeInBone(pContainer, "num");

	return true;
}

void FriendsViewCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void FriendsViewCell::onExit()
{
	UNBINDING_ALL(m_pData, this);
	CCTableViewCell::onExit();
}

void FriendsViewCell::updateView()
{
	if (!m_pData)
	{
		return;
	}
	m_pTTFName->setString(m_pData->getName().c_str());
	m_pNodeAvatorContainer->removeAllChildren();
	m_pNodeAvatorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(m_pData->getAvatarID()));
	m_pBMFLevel->setString(D_CSTRING_FROM_UINT(m_pData->getLevel()));
	m_pNodeInvite->setVisible(!m_pData->getHasRequested());
	m_pNodeInvited->setVisible(m_pData->getHasRequested());
	m_pBtnAdd->setEnabled(!m_pData->getHasRequested());
}

bool FriendsViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFriend", CCNode *, m_pNodeFriend);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAdd", CCNode *, m_pNodeAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAvatorContainer", CCNode *, m_pNodeAvatorContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFLevel", CCLabelBMFont *, m_pBMFLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnFriend", CCMenuItem *, m_pBtnFriend);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnFormation", CCMenuItem *, m_pBtnFormation);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeReceive", CCNode *, m_pNodeReceive);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFormation", CCNode *, m_pNodeFormation);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeGift", CCNode *, m_pNodeGift);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeReturn", CCNode *, m_pNodeReturn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAdd", CCMenuItem *, m_pBtnAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAgree", CCMenuItem *, m_pBtnAgree);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimatePower", UIAnimate *, m_pAnimatePower);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateCoins", UIAnimate *, m_pAnimateCoins);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeApply", CCNode *, m_pNodeApply);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeInvite", CCNode *, m_pNodeInvite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeInvited", CCNode *, m_pNodeInvited);

	return false;
}


SEL_MenuHandler FriendsViewCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	return NULL;
}

void FriendsViewCell::setType(FriendsViewCellType eType)
{
	m_pNodeFriend->setVisible(eType == FriendsViewCellTypeNow);
	m_pNodeAdd->setVisible(eType == FriendsViewCellTypeAdd);
	m_pNodeApply->setVisible(eType == FriendsViewCellTypeApply);
}

void FriendsViewCell::setStatus(friends::GiftState eStatus)
{
	m_eStatus = eStatus;
	m_pNodeReceive->setVisible(eStatus == friends::kGiftStateReceive);
	m_pNodeFormation->setVisible(eStatus == friends::kGiftStateFormation);
	m_pNodeGift->setVisible(eStatus == friends::kGiftStatePresent);
	m_pNodeReturn->setVisible(eStatus == friends::kGiftStateReturn);
	m_pBtnFormation->setVisible(eStatus == friends::kGiftStateFormation || eStatus == friends::kGiftStateReceive);
	m_pBtnFriend->setVisible(!m_pBtnFormation->isVisible());
}

void FriendsViewCell::callBackFriend(CCObject *pSender)
{
	if (!m_pDelegate)
	{
		return;
	}
	switch(m_eStatus)
	{
	case friends::kGiftStatePresent:
	case friends::kGiftStateReturn:
		m_pDelegate->FriendsViewCellGift(this);
		break;
	case friends::kGiftStateReceive:
		m_pDelegate->FriendsViewCellReceive(this);
		break;
	case friends::kGiftStateFormation:
		m_pDelegate->FriendsViewCellFormation(this);
		break;
	default:
		;
	}
}

void FriendsViewCell::callBackAdd(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->FriendsViewCellAdd(this);
	}
}

void FriendsViewCell::callBackAgree(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->FriendsViewCellAgree(this);
	}
}

void FriendsViewCell::setData(friends::FriendBasic *pData)
{
	if (!pData)
	{
		return;
	}

	UNBINDING_ALL(m_pData, this);
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
	friends::FriendInfo *pInfo = dynamic_cast<friends::FriendInfo *>(pData);
	if (NULL != pInfo)
	{
		BINDING_EXEC_O(pInfo, this, FriendsViewCell::updateStatus, D_EVENT_FRIENDS_STATUS_REFRESH);
	}
	BINDING_EXEC(m_pData, this, FriendsViewCell::updateView, D_EVENT_FRIENDS_BASIC_REFRESH);
}

void FriendsViewCell::updateStatus(CCObject *pObj)
{
	friends::FriendInfo *pInfo = dynamic_cast<friends::FriendInfo *>(m_pData);
	if (NULL != pInfo)
	{
		setStatus(m_pModel->getGiftState(pInfo));
	}
	drop::Drop *pDrop = dynamic_cast<drop::Drop *>(pObj);
	if (!pDrop)
	{
		return;
	}
	m_pAnimatePower->setVisible(pDrop->getType() == drop::kDropTypePower);
	m_pAnimateCoins->setVisible(pDrop->getType() == drop::kDropTypeCoin);
	if (pDrop->getType() == drop::kDropTypePower)
	{
		m_pBMFPower->setString(D_CSTRING_FROM_UINT(pDrop->getPower()));
		m_pAnimatePower->playAtIndex(0);
	}
	else if (pDrop->getType() == drop::kDropTypeCoin)
	{
		m_pBMFCoins->setString(D_CSTRING_FROM_UINT(pDrop->getCoin()));
		m_pAnimateCoins->playAtIndex(0);
	}
}