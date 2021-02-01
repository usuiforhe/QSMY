//
//  TeamFormation.h
//  组队阵容&buff
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#define UNSELECT -1

#include "TeamFormation.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include <algorithm>

TeamFormation::TeamFormation()
	: m_pTTFTips(NULL)
	, m_pDBtnBattle(NULL)
	, m_pTTFName(NULL)
	, m_pBMFNum(NULL)
	, m_pDelegate(NULL)
	, m_pModel(NULL)
	, m_nItemSelected(false)
	, m_mItemNumUsed()
{
	D_RESET_C_ARRAY(m_pWarrior, FORMATION_MAX);
	D_RESET_C_ARRAY(m_pItem, ITEM_MAX);
}

TeamFormation::~TeamFormation()
{
	CC_SAFE_RELEASE(m_pTTFTips);
	CC_SAFE_RELEASE(m_pDBtnBattle);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pBMFNum);
	D_SAFE_RELEASE_C_ARRAY(m_pWarrior, FORMATION_MAX);
	D_SAFE_RELEASE_C_ARRAY(m_pItem, ITEM_MAX);
}

TeamFormation* TeamFormation::create()
{
	TeamFormation *pRet = new TeamFormation();
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

bool TeamFormation::init()
{
	if(!DLayer::init())
	{
		return false;
	}


	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("TeamFormationItem", TeamFormationItemLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("ChangeFormationItem", ChangeFormationItemLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/team/teamFormation", pNodeLoaderLibrary);

	m_pDBtnBattle->getButton()->setTarget(this, menu_selector(TeamFormation::callBackBattle));

	m_pBMFNum->setString("0/0");

	m_pModel = DM_GET_FRIENDS_MODEL;


	for (int i = 0; i < FORMATION_MAX; i++)
	{
		m_pWarrior[i]->setDelegate(this);
		m_pWarrior[i]->setTag(i);
	}

	for (int i = 0; i < ITEM_MAX; i++)
	{
		m_mItemNumUsed[TEAM_ITEMS[i]] = 0;
	}

	m_pTTFTips->setString(D_LOCAL_STRING("TeamItemUnselected").c_str());

	return true;
}

void TeamFormation::onEnter()
{
	BINDING_EXEC(m_pModel, this, TeamFormation::updateView, D_EVENT_FRIENDS_DUNGEON);
	Game::sharedGame()->setBackKeyEnabled(false);
	DLayer::onEnter();
}

void TeamFormation::updateView()
{
	if (m_pModel->getSelectedChapter() != NULL)
	{
		m_pTTFName->setString(m_pModel->getSelectedChapter()->getName().c_str());
		m_pBMFNum->setString(CCString::createWithFormat("%u/%u", m_pModel->getDungeon()->getOngoingProgress(), m_pModel->getSelectedChapter()->getTurns())->getCString());
	}
	CCDictionary *pWarriors = m_pModel->getDungeon()->getWarriors();
	for (int i = 0; i < FORMATION_MAX; i++)
	{
		formation::PositionInfo *pPos = DM_GET_FORMATION_MODEL->getPositionInfoByIndex(i);
		if (!pPos->isOpened())
		{
			m_pWarrior[i]->updateViewUnlock(pPos->getPosId());
			continue;
		}
		friends::DungeonWarrior *pWarrior = static_cast<friends::DungeonWarrior *>(pWarriors->objectForKey(pPos->getPosId()));
		bool bFriend = m_pModel->getSelectedChapter() != NULL && m_pModel->getFriendIndex() == i;
		m_pWarrior[i]->updateViewWithHealth(pWarrior, bFriend);
	}
	updateItem();
}

void TeamFormation::updateItem()
{
	ItemModel *pItemModel = DM_GET_ITEM_MODEL;

	for (int i = 0; i < ITEM_MAX; i++)
	{
		uint32_t itemID = TEAM_ITEMS[i];
		m_pItem[i]->updateView(drop::Drop::create(item::ItemInfo::create(itemID, pItemModel->getItemNum(itemID) - m_mItemNumUsed[itemID])));
		m_pItem[i]->getIconMenu()->setTag(i);
		m_pItem[i]->getIconMenu()->setTarget(this, menu_selector(TeamFormation::callBackItem));
		m_pItem[i]->setRotateAnimationPlay(itemID == m_nItemSelected);
		m_pItem[i]->setScaleAnimatePlay(itemID == m_nItemSelected);
	}
}

void TeamFormation::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	Game::sharedGame()->setBackKeyEnabled(true);
	DLayer::onExit();
}

bool TeamFormation::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFTips", CCLabelTTF *, m_pTTFTips);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnBattle", DButton *, m_pDBtnBattle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCRichLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFNum", CCLabelBMFont *, m_pBMFNum);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pWarrior", TeamFormationItem *, m_pWarrior, FORMATION_MAX);
	D_CCB_ASSIGN_ARRAY("m_pItem", AwardItemSmall *, m_pItem, ITEM_MAX);
	return false;
}


SEL_MenuHandler TeamFormation::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackDeselect", TeamFormation::callBackDeselect);
	return NULL; 
}

void TeamFormation::TeamFormationItemSelected(TeamFormationItem *pItem)
{
	if (m_nItemSelected > 0 && pItem->getAnimating() && pItem->getDungeonWarrior() != NULL)
	{
		friends::DungeonWarrior *pWarrior = pItem->getDungeonWarrior();
		switch(m_nItemSelected)
		{
		case RIVIVAL_ITEM:
			pWarrior->setRevival(true);
			break;
		case ENHANCE_ITEM:
			pWarrior->setEnhance(true);
			break;
		case RECOVER_ITEM:
			pWarrior->setRecover(true);
			break;
		default:
			;
		}
		bool bFriend = m_pModel->getSelectedChapter() != NULL && m_pModel->getFriendIndex() == pItem->getTag();
		pItem->updateViewWithHealth(pWarrior, bFriend);
		m_mItemNumUsed[m_nItemSelected] += 1;
		ItemModel *pItemModel = DM_GET_ITEM_MODEL;
		if (pItemModel->getItemNum(m_nItemSelected) <= m_mItemNumUsed[m_nItemSelected])
		{
			updateSelect();
		}
		else
		{
			pItem->selectedIfAvailable(item::ItemInfo::create(m_nItemSelected, pItemModel->getItemNum(m_nItemSelected) - m_mItemNumUsed[m_nItemSelected]));
		}
		updateItem();
	}
}

void TeamFormation::callBackBattle(CCObject *pSender)
{
	m_pModel->setBuffItemNum(m_mItemNumUsed);
	m_pModel->friendGo();
}

void TeamFormation::callBackItem(CCObject *pSender)
{
	CCMenuItem *pBtn = static_cast<CCMenuItem *>(pSender);
	int tag = pBtn->getTag();
	m_nItemSelected = TEAM_ITEMS[tag];
	ItemModel *pItemModel = DM_GET_ITEM_MODEL;
	m_pTTFTips->setString(D_LOCAL_STRING(CCString::createWithFormat("TeamItem%u", m_nItemSelected)->getCString()).c_str());
	if (pItemModel->getItemNum(m_nItemSelected) - m_mItemNumUsed[m_nItemSelected] == 0)
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_ITEMS,CCInteger::create(m_nItemSelected));
		return;
	}
	for (int i = 0; i < ITEM_MAX; i++)
	{
		m_pItem[i]->setRotateAnimationPlay(i == tag);
		m_pItem[i]->setScaleAnimatePlay(i == tag);
	}
	updateSelect();
}

void TeamFormation::callBackDeselect(CCObject *pSender)
{
	if (m_nItemSelected == 0)
	{
		return;
	}
	m_nItemSelected = 0;
	m_pTTFTips->setString(D_LOCAL_STRING("TeamItemUnselected").c_str());
	for (int i = 0; i < ITEM_MAX; i++)
	{
		m_pItem[i]->setRotateAnimationPlay(false);
		m_pItem[i]->setScaleAnimatePlay(false);
	}
	//ItemModel *pItemModel = DM_GET_ITEM_MODEL;
	for (int i = 0; i < FORMATION_MAX; i++)
	{
		m_pWarrior[i]->selectedIfAvailable(NULL);
	}
}

void TeamFormation::updateSelect()
{
	ItemModel *pItemModel = DM_GET_ITEM_MODEL;
	for (int i = 0; i < FORMATION_MAX; i++)
	{
		m_pWarrior[i]->selectedIfAvailable(item::ItemInfo::create(m_nItemSelected, pItemModel->getItemNum(m_nItemSelected) - m_mItemNumUsed[m_nItemSelected]));
	}
}