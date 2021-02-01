//
//  TeamFormationItem.h
//  组队阵容
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#define UNSELECT -1

#include "TeamFormationItem.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include <algorithm>
#include "Team.h"

TeamFormationItem::TeamFormationItem()
	: m_pPBHealth(NULL)
	, m_pWarrior(NULL)
	, m_pAnimateSelected(NULL)
	, m_pWarriorTypeMark(NULL)
	, m_pNodeDefense(NULL)
	, m_pNodeAttack(NULL)
	, m_pNodeDead(NULL)
	, m_pNodeFriend(NULL)
	, m_pAnimateAdd(NULL)
	, m_pDelegate(NULL)
	, m_bEmpty(false)
	, m_pDungeonWarrior(NULL)
	, m_bAnimating(false)
	, m_pInfo(NULL)
{
}

TeamFormationItem::~TeamFormationItem()
{
	CC_SAFE_RELEASE(m_pPBHealth);
	CC_SAFE_RELEASE(m_pWarrior);
	CC_SAFE_RELEASE(m_pAnimateSelected);
	CC_SAFE_RELEASE(m_pWarriorTypeMark);
	CC_SAFE_RELEASE(m_pNodeAttack);
	CC_SAFE_RELEASE(m_pNodeDefense);
	CC_SAFE_RELEASE(m_pNodeDead);
	CC_SAFE_RELEASE(m_pNodeFriend);
	CC_SAFE_RELEASE(m_pAnimateAdd);
	CC_SAFE_RELEASE(m_pDungeonWarrior);
	CC_SAFE_RELEASE(m_pInfo);
}

bool TeamFormationItem::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	return true;
}

bool TeamFormationItem::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPBHealth", DProgressBar *, m_pPBHealth);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior", WarriorHeadMid *, m_pWarrior);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateSelected", UIAnimate *, m_pAnimateSelected);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, m_pWarriorTypeMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDefense", CCNode *, m_pNodeDefense);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAttack", CCNode *, m_pNodeAttack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDead", CCNode *, m_pNodeDead);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFriend", CCNode *, m_pNodeFriend);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateAdd", UIAnimate *, m_pAnimateAdd);

	return false;
}

SEL_MenuHandler TeamFormationItem::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackTouch", TeamFormationItem::callBackTouch);
	return NULL;
}

void TeamFormationItem::callBackTouch(CCObject *pSenders)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamFormationItemSelected(this);
	}
}

void TeamFormationItem::updateView(friends::FriendWarrior *pWarrior, int level, bool bFriend)
{
	m_pWarrior->updateView(pWarrior, level);
	m_pWarriorTypeMark->setVisible(pWarrior != NULL);
	m_pPBHealth->setVisible(pWarrior != NULL);
	m_pAnimateAdd->setVisible(!pWarrior && bFriend);
	if (!pWarrior && bFriend)
	{
		m_pAnimateAdd->playAtIndex(0);
	}
	m_pNodeFriend->setVisible(bFriend);
	if (pWarrior)
	{
		m_pWarriorTypeMark->setWarriorInfo(DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pWarrior->getWID()));
	}
}

void TeamFormationItem::updateViewUnlock(int pos)
{
	m_pWarrior->updateView(NULL, 0, false, pos);
	m_pWarriorTypeMark->setVisible(false);
	m_pPBHealth->setVisible(false);
	m_pNodeFriend->setVisible(false);
}

void TeamFormationItem::updateViewWithHealth(friends::DungeonWarrior *pWarrior, bool bFriend)
{
	CC_SAFE_RETAIN(pWarrior);
	CC_SAFE_RELEASE(m_pDungeonWarrior);
	m_pDungeonWarrior = pWarrior;

	if (pWarrior)
	{
		updateView(m_pDungeonWarrior, m_pDungeonWarrior->getLevel(), bFriend);
	}
	else
	{
		updateView(NULL, 1, bFriend);
		return;
	}
	uint32_t hp = pWarrior->getHP();
	uint32_t maxHp = pWarrior->getMaxHP();
	if (hp == 0 && pWarrior->getRevival())
	{
		hp = maxHp * D_CONFIG_UINT_FOR_KEY(TEAM_REVIVAL_PERCENT) / 100;
	}
	if (hp > 0 && pWarrior->getRecover())
	{
		hp += maxHp * D_CONFIG_UINT_FOR_KEY(TEAM_RECOVER_PERCENT) / 100;
		if (hp > maxHp)
		{
			hp = maxHp;
		}
	}
	m_pPBHealth->setPercent((float)hp / maxHp);
	m_pNodeDead->setVisible(hp == 0);
	m_pNodeAttack->setVisible(pWarrior->getEnhance() && pWarrior->getPosition() % 2 == 0);
	m_pNodeDefense->setVisible(pWarrior->getEnhance() && pWarrior->getPosition() % 2 == 1);

	m_pAnimateAdd->setVisible(false);
}

void TeamFormationItem::selectedIfAvailable(item::ItemInfo *pItem)
{
	m_bAnimating = false;
	if (!pItem || !pItem->getNum() || !m_pDungeonWarrior)
	{
		m_pAnimateSelected->setVisible(false);
		return;
	}
	bool isDead = m_pDungeonWarrior->getHP() == 0 && !m_pDungeonWarrior->getRevival();
	switch (pItem->getItemId())
	{
	case RIVIVAL_ITEM:
		if (isDead)
		{
			m_bAnimating = true;
		}
		break;
	case ENHANCE_ITEM:
		if (!isDead && !m_pDungeonWarrior->getEnhance())
		{
			m_bAnimating = true;
		}
		break;
	case RECOVER_ITEM:
		if (m_pDungeonWarrior->getHP() < m_pDungeonWarrior->getMaxHP() && !isDead && !m_pDungeonWarrior->getRecover())
		{
			m_bAnimating = true;
		}
		break;
	default:
		m_bAnimating = false;
	}
	m_pAnimateSelected->setVisible(m_bAnimating);
	if (m_bAnimating)
	{
		m_pAnimateSelected->playAtIndex(0);
	}
}