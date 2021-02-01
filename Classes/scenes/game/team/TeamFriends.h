//
//  TeamFriends.h
//  组队好友选择
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__TeamFriends__
#define __QSMY__TeamFriends__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "Team.h"
#include "TeamFriendsCell.h"

class TeamFriends
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public TeamFriendsCellDelegate
{
	CC_SYNTHESIZE(TeamViewDelegate *, m_pDelegate, Delegate);
public:
	TeamFriends();
	~TeamFriends();

	static TeamFriends* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

#pragma mark CCTableViewDataSource
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);

#pragma mark CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};

#pragma mark TeamFriendsCellDelegate

	virtual void TeamFriendsCellSelected(TeamFriendsCell *pCell);
	virtual void TeamFriendsCellView(TeamFriendsCell *pCell);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

private:
	CCNode *m_pNodeContainer;
	CCMenuItem *m_pBtnRule;
	DButton *m_pDBtnCancel;
	CCNode *m_pCell;
	DTableView *m_pTableView;

	FriendsModel *m_pModel;
	CCArray *m_pList;

	CCArray *m_pFateWarriors;
	CCArray *m_pFormationWarriors;

	void callBackCancel(CCObject *pObj);
	void callBackRule(CCObject *pObj);
};

#endif