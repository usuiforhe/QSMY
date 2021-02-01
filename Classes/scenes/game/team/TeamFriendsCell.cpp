//
//  TeamFriendsCell.h
//  组队好友选择
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "TeamFriendsCell.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
using namespace sp;

TeamFriendsCell::TeamFriendsCell()
	: m_pRgbaGrade(NULL) 
	, m_pTTFName(NULL)
	, m_pNodeAvatorContainer(NULL)
	, m_pBtnSelect(NULL)
	, m_pWarriorTypeMark(NULL)
	, m_pNodeRanout(NULL)
	, m_pNodeNew(NULL)
	, m_pNodeAvailable(NULL)
	, m_pNodeDup(NULL)
	, m_pNodeNotDup(NULL)
	, m_pNodeFate(NULL)
	, m_pBMFFight(NULL)
	, m_pDelegate(NULL)
	, m_pData(NULL)
	, m_pModel(NULL)
{
}

TeamFriendsCell::~TeamFriendsCell()
{
	CC_SAFE_RELEASE(m_pRgbaGrade);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pNodeAvatorContainer);
	CC_SAFE_RELEASE(m_pBtnSelect);
	CC_SAFE_RELEASE(m_pWarriorTypeMark);
	CC_SAFE_RELEASE(m_pNodeRanout);
	CC_SAFE_RELEASE(m_pNodeNew);
	CC_SAFE_RELEASE(m_pNodeAvailable);
	CC_SAFE_RELEASE(m_pNodeDup);
	CC_SAFE_RELEASE(m_pNodeNotDup);
	CC_SAFE_RELEASE(m_pNodeFate);
	CC_SAFE_RELEASE(m_pBMFFight);
	CC_SAFE_RELEASE(m_pData);
}

TeamFriendsCell* TeamFriendsCell::create()
{
	TeamFriendsCell *pRet = new TeamFriendsCell();
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

bool TeamFriendsCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorTypeMark", WarriorTypeMarkLoader::loader());
	
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/team/teamFriendsListCell", pNodeLoaderLibrary);

	m_pBtnSelect->setTarget(this, menu_selector(TeamFriendsCell::callBackSelect));

	m_pNodeRanout->setVisible(false);
	m_pNodeAvailable->setVisible(false);
	m_pNodeDup->setVisible(false);
	m_pNodeFate->setVisible(true);
	m_pNodeNew->setVisible(false);

	m_pModel = DM_GET_FRIENDS_MODEL;
	return true;
}

void TeamFriendsCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void TeamFriendsCell::onExit()
{
	CCTableViewCell::onExit();
}

bool TeamFriendsCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteGrade", CCNodeRGBA *, m_pRgbaGrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAvatorContainer", CCNode *, m_pNodeAvatorContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnSelect", CCMenuItem *, m_pBtnSelect);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, m_pWarriorTypeMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRanout", CCNode *, m_pNodeRanout);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNew", CCNode *, m_pNodeNew);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAvailable", CCNode *, m_pNodeAvailable);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDup", CCNode *, m_pNodeDup);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNotDup", CCNode *, m_pNodeNotDup);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFate", CCNode *, m_pNodeFate);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFFight", CCLabelBMFont *, m_pBMFFight);
	return false;
}

SEL_MenuHandler TeamFriendsCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackView", TeamFriendsCell::callBackView);
	return NULL;
}

void TeamFriendsCell::callBackSelect(CCObject *pObj)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamFriendsCellSelected(this);
	}
}

void TeamFriendsCell::callBackView(CCObject *pObj)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamFriendsCellView(this);
	}
}

void TeamFriendsCell::setData(friends::FriendInfo *pData)
{
	if (!pData)
	{
		return;
	}
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
	m_pTTFName->setString(pData->getName().c_str());
	m_pNodeAvatorContainer->removeAllChildren();
	m_pNodeAvatorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(pData->getWarriorAtIndex(m_pModel->getFriendIndex())->getAvatarID()));
	warrior::WarriorBaseInfo *pBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pData->getWarriorAtIndex(m_pModel->getFriendIndex())->getWID());
	if (pBase)
	{
		m_pRgbaGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByColorGrade(pBase->getColorGrade()));
	}
	m_pBMFFight->setString(D_CSTRING_FROM_UINT(pData->getWarriorAtIndex(m_pModel->getFriendIndex())->getFight()));
	m_pNodeFate->setVisible(pData->getFate());
	m_pWarriorTypeMark->setWarriorInfo(DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pData->getWarriorAtIndex(m_pModel->getFriendIndex())->getWID()));

	m_pNodeRanout->setVisible(pData->getRanout()); //ranout > new > dup > available
	m_pNodeNew->setVisible(!pData->getRanout() && pData->getNew());
	m_pNodeDup->setVisible(!pData->getRanout() && !pData->getNew() && pData->getDup());
	m_pBtnSelect->setEnabled(!pData->getRanout() && !pData->getDup() && !pData->getNew());
	m_pNodeAvailable->setVisible(!pData->getRanout() && !pData->getDup() && !pData->getNew());
	m_pNodeNotDup->setVisible(true);
}