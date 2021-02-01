//
//  FriendsSelectViewCell.h
//  选择好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__FriendsSelectViewCell__
#define __QSMY__FriendsSelectViewCell__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class FriendsSelectViewCell;

class FriendsSelectViewCellDelegate
{
public:
	virtual void FriendsSelectViewCellSelected(FriendsSelectViewCell *pCell) = 0;
};

class FriendsSelectViewCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(FriendsSelectViewCellDelegate *, m_pDelegate, Delegate);
public:
	FriendsSelectViewCell();
	~FriendsSelectViewCell();

	static FriendsSelectViewCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };

	void setSelected(bool bSelected);
	void setData(friends::FriendInfo *pData);
private:
	CCLabelTTF *m_pTTFName;
	CCNode *m_pNodeAvatorContainer;
	CCLabelBMFont *m_pBMFLevel;
	CCMenuItem *m_pBtnCheck;

	void callBackCheck(CCObject *pSender);
	bool m_bSelected;
};

#endif