//
//  TeamMake.h
//  进行组队
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#define UNSELECT -1

#include "TeamMake.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include <algorithm>

TeamMake::TeamMake()
	: m_pBMFGold(NULL)
	, m_pNodeRush(NULL)
	, m_pNodeConfirm(NULL)
	, m_pTTFName(NULL)
	, m_pBMFNum(NULL)
	, m_pBtnRule(NULL)
	, m_pNodePoint(NULL)
	, m_pNodeFree(NULL)
	, m_pNodeVip(NULL)
	, m_pBMFVip(NULL)
	, m_pNodeMaxVip(NULL)
	, m_pBtnGo(NULL)
	, m_pNodeFreeFont(NULL)
	, m_pNodePayed(NULL)
	, m_pNodeLock(NULL)
	, m_pDelegate(NULL)
	, m_pModel(NULL)
	, m_nEmptyIdx(0)
{
	D_RESET_C_ARRAY(m_pWarrior, FORMATION_MAX);
}

TeamMake::~TeamMake()
{
	CC_SAFE_RELEASE(m_pBMFGold);
	CC_SAFE_RELEASE(m_pNodeRush);
	CC_SAFE_RELEASE(m_pNodeConfirm);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pBMFNum);
	CC_SAFE_RELEASE(m_pBtnRule);
	CC_SAFE_RELEASE(m_pNodePoint);
	CC_SAFE_RELEASE(m_pNodeFree);
	CC_SAFE_RELEASE(m_pNodeVip);
	CC_SAFE_RELEASE(m_pBMFVip);
	CC_SAFE_RELEASE(m_pNodeMaxVip);
	CC_SAFE_RELEASE(m_pBtnGo);
	CC_SAFE_RELEASE(m_pNodeFreeFont);
	CC_SAFE_RELEASE(m_pNodePayed);
	CC_SAFE_RELEASE(m_pNodeLock);
	D_SAFE_RELEASE_C_ARRAY(m_pWarrior, FORMATION_MAX);
}

TeamMake* TeamMake::create()
{
	TeamMake *pRet = new TeamMake();
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

bool TeamMake::init()
{
	if(!DLayer::init())
	{
		return false;
	}


	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("TeamFormationItem", TeamFormationItemLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("ChangeFormationItem", ChangeFormationItemLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/team/teamMade", pNodeLoaderLibrary);

	m_pNodeRush->setVisible(false);

	m_pModel = DM_GET_FRIENDS_MODEL;

	m_nEmptyIdx = m_pModel->getFriendIndex();

	m_pWarrior[m_nEmptyIdx]->setDelegate(this);
	m_pWarrior[m_nEmptyIdx]->setIsEmpty(true);
	m_pWarrior[m_nEmptyIdx]->updateView(NULL, 1, true);

	m_pBtnGo->setTarget(this, menu_selector(TeamMake::callBackConfirm));

	m_pBMFNum->setString("0/0");

	FormationModel *pFormation = DM_GET_FORMATION_MODEL;
	for (uint8_t i = 0; i < FORMATION_POS_MAX - 1; ++i)
	{
		uint8_t idx = i;
		if (m_nEmptyIdx <= idx)
		{
			idx += 1;
		}
		formation::PositionInfo *pPosInfo = pFormation->getUnlockedPositionInfoByIndex(idx);

		if (NULL == pPosInfo)
		{
			m_pWarrior[idx]->updateViewUnlock(pFormation->getPositionInfoByIndex(idx)->getPosId());
		}
		else
		{
 			warrior::WarriorCurInfo * pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(pPosInfo->iWarriorId);
			if (NULL == pWarrior)
			{
				m_pWarrior[idx]->updateViewUnlock(pFormation->getPositionInfoByIndex(idx)->getPosId());
			}
			else
			{
				m_pWarrior[idx]->updateView(friends::FriendWarrior::create(pWarrior), pWarrior->getWarriorLv());
			}
		}
	}

	m_pBtnRule->setTarget(this, menu_selector(TeamMake::callBackRule));

	return true;
}

void TeamMake::onEnter()
{
	BINDING_EXEC(m_pModel, this, TeamMake::updateView, D_EVENT_FRIENDS_DUNGEON);
	m_pModel->syncDungeon();
	DLayer::onEnter();
}

void TeamMake::updateView()
{
	if (!m_pModel->getSelectedChapter())
	{
		if (m_pDelegate)
		{
			m_pDelegate->TeamNavigateTo(kTeamNavHome);
		}
		return;
	}
	m_pTTFName->setString(m_pModel->getSelectedChapter()->getName().c_str());
	m_pBMFNum->setString(CCString::createWithFormat("0/%u", m_pModel->getSelectedChapter()->getTurns())->getCString());


	if (m_pModel->getSelectedChapter()->getIsDone())
	{
		m_pBMFGold->setString(D_CONFIG_CSTRING_FOR_KEY(TEAM_RESET_COST));
	}

	m_pNodeRush->setVisible(m_pModel->getSelectedChapter()->getCanRush());
	m_pNodeConfirm->setVisible(!m_pNodeRush->isVisible());

	friends::FriendWarrior *pWarrior = m_pModel->getDungeon()->getFriendWarrior();
	if (pWarrior)
	{
		int uLv = DM_GET_USER_MODEL->getUserInfo()->getUserLv();
		m_pWarrior[m_nEmptyIdx]->updateView(pWarrior, uLv * D_CONFIG_UINT_FOR_KEY(TEAM_LEVEL_MULTIPLIER), true);
	}
	else
	{
		m_pWarrior[m_nEmptyIdx]->updateView(NULL, 1, true);
	}

	if (m_pModel->getSelectedChapter()->getIsDone() && m_pModel->getResetRest() == 0)
	{

		m_pNodeVip->setVisible(false);
		int nVipLv = DM_GET_USER_MODEL->getUserInfo()->getVipLevel();
		if (nVipLv == 0)
		{
			nVipLv = 1;
		}

		user::VipInfo *pCurInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(nVipLv);
		int nMaxLv = DM_GET_USER_MODEL->getMaxVipLevel();
		for (int i = nVipLv + 1; i <= nMaxLv; i++)
		{
			user::VipInfo *pInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(i);
			if (pInfo->getTeamResetMax() > pCurInfo->getTeamResetMax())
			{
				m_pBMFVip->setString(D_CSTRING_FROM_INT(i));
				m_pNodeVip->setVisible(true);
				//NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV,CCString::create(D_LOCAL_STRING("PVP_NEED_VIP%d",i)));
				break;;
			}

		}
		m_pNodeMaxVip->setVisible(!m_pNodeVip->isVisible());
	}
	else
	{
		m_pNodeVip->setVisible(false);
		m_pNodeMaxVip->setVisible(false);
	}

	m_pNodeLock->setVisible(m_pModel->getSelectedChapter()->getIsDone() && m_pModel->getResetRest() == 0);
	m_pNodePayed->setVisible(m_pModel->getSelectedChapter()->getIsDone() && m_pModel->getResetRest() > 0);
	m_pNodePoint->setVisible(m_pNodePayed->isVisible());
	m_pNodeFree->setVisible(!m_pModel->getSelectedChapter()->getIsDone());
	m_pNodeFreeFont->setVisible(m_pNodeFree->isVisible());
}

void TeamMake::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	DLayer::onExit();
}

bool TeamMake::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFGold", CCLabelBMFont *, m_pBMFGold);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRush", CCNode *, m_pNodeRush);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeConfirm", CCNode *, m_pNodeConfirm);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCRichLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFNum", CCLabelBMFont *, m_pBMFNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRule", CCMenuItem *, m_pBtnRule);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodePoint", CCNode *, m_pNodePoint);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFree", CCNode *, m_pNodeFree);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeVip", CCNode *, m_pNodeVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFVip", CCLabelBMFont *, m_pBMFVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeMax", CCNode *, m_pNodeMaxVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnGo", CCMenuItem *, m_pBtnGo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFreeFont", CCNode *, m_pNodeFreeFont);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodePayed", CCNode *, m_pNodePayed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeLock", CCNode *, m_pNodeLock);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pWarrior", TeamFormationItem *, m_pWarrior, FORMATION_MAX);
	return false;
}
SEL_MenuHandler TeamMake::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackClose", TeamMake::callBackClose);
	return NULL;
}

void TeamMake::TeamFormationItemSelected(TeamFormationItem *pItem)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamNavigateTo(kTeamNavFriends);
	}
}

void TeamMake::callBackConfirm(CCObject *pSender)
{
	if (!m_pModel->getDungeon()->getFriendWarrior())
	{
		DM_GET_ERRORTIP_MODEL->showErrorTips(TEAM_WARRIOR_NOT_EXIST);
		return;
	}
	if (m_pModel->getSelectedChapter()->getIsDone() && m_pModel->getResetRest() == 0)
	{
		DM_GET_ERRORTIP_MODEL->showErrorTips(TEAM_RESET_MAX);
		return;
	}
	m_pModel->friendGoAndResetIfNeeded();
}

void TeamMake::callBackRule(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamNavigateTo(kTeamNavRule);
	}
}

void TeamMake::callBackClose(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamNavigateTo(kTeamNavHome);
	}
}