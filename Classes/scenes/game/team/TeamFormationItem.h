//
//  TeamFormationItem.h
//  组队阵容
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__TeamFormationItem__
#define __QSMY__TeamFormationItem__

#define RIVIVAL_ITEM 9093
#define RECOVER_ITEM 9094
#define ENHANCE_ITEM 9095

const int TEAM_ITEMS[] = {RIVIVAL_ITEM, RECOVER_ITEM, ENHANCE_ITEM};

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../formation/ChangeFormationView.h"

class TeamFormationItem;

class TeamFormationItemDelegate
{
public:
	virtual void TeamFormationItemSelected(TeamFormationItem *pItem) = 0;
};

class TeamFormationItem
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(TeamFormationItemDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE(bool, m_bEmpty, IsEmpty);
	CC_SYNTHESIZE_READONLY(bool, m_bAnimating, Animating);
	CC_SYNTHESIZE_READONLY(friends::DungeonWarrior *, m_pDungeonWarrior, DungeonWarrior);
public:
	TeamFormationItem();
	~TeamFormationItem();

	CREATE_FUNC(TeamFormationItem);
	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };

	void updateView(friends::FriendWarrior *pWarrior, int level, bool bFriend = false);

	void updateViewUnlock(int pos);

	void updateViewWithHealth(friends::DungeonWarrior *pWarrior, bool bFriend = false);

	void selectedIfAvailable(item::ItemInfo *pItem);
private:
	DProgressBar *m_pPBHealth;
	WarriorHeadMid *m_pWarrior;
	UIAnimate *m_pAnimateSelected;
	WarriorTypeMark *m_pWarriorTypeMark;
	CCNode *m_pNodeDefense;
	CCNode *m_pNodeAttack;
	CCNode *m_pNodeDead;

	CCNode *m_pNodeFriend;
	UIAnimate *m_pAnimateAdd;

	warrior::WarriorCurInfo *m_pInfo;

	void callBackTouch(CCObject *pSenders);
};

class TeamFormationItemLoader : public cocos2d::extension::CCNodeLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(TeamFormationItemLoader, loader);

protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(TeamFormationItem);
};

#endif