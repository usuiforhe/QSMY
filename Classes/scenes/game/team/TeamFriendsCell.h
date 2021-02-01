//
//  TeamFriendsCell.h
//  组队好友选择
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__TeamFriendsCell__
#define __QSMY__TeamFriendsCell__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../common/gui/WarriorTypeMark.h"

class TeamFriendsCell;

class TeamFriendsCellDelegate
{
public:
	virtual void TeamFriendsCellSelected(TeamFriendsCell *pCell) = 0;
	virtual void TeamFriendsCellView(TeamFriendsCell *pCell) = 0;
};

class TeamFriendsCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(TeamFriendsCellDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(friends::FriendInfo *, m_pData, Data);
public:
	TeamFriendsCell();
	~TeamFriendsCell();

	static TeamFriendsCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };

	void setData(friends::FriendInfo *pData);
private:
	CCNodeRGBA *m_pRgbaGrade;
	CCLabelTTF *m_pTTFName;
	CCNode *m_pNodeAvatorContainer;
	CCMenuItem *m_pBtnSelect;
	WarriorTypeMark *m_pWarriorTypeMark;
	CCNode *m_pNodeRanout;
	CCNode *m_pNodeNew;
	CCNode *m_pNodeAvailable;
	CCNode *m_pNodeDup;
	CCNode *m_pNodeNotDup;
	CCNode *m_pNodeFate;
	CCLabelBMFont *m_pBMFFight;

	FriendsModel *m_pModel;

	void callBackSelect(CCObject *pObj);
	void callBackView(CCObject *pObj);
};

#endif