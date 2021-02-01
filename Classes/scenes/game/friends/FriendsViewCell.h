//
//  FriendsViewCell.h
//  好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__FriendsViewCell__
#define __QSMY__FriendsViewCell__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

enum FriendsViewCellType
{
	FriendsViewCellTypeNow = 0,
	FriendsViewCellTypeAdd = 1,
	FriendsViewCellTypeApply = 2,
};

class FriendsViewCell;

class FriendsViewCellDelegate
{
public:
	virtual void FriendsViewCellGift(FriendsViewCell *pCell) = 0;
	virtual void FriendsViewCellReceive(FriendsViewCell *pCell) = 0;
	virtual void FriendsViewCellFormation(FriendsViewCell *pCell) = 0;
	virtual void FriendsViewCellAdd(FriendsViewCell *pCell) = 0;
	virtual void FriendsViewCellRefuse(FriendsViewCell *pCell) = 0;
	virtual void FriendsViewCellAgree(FriendsViewCell *pCell) = 0;
};

class FriendsViewCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(FriendsViewCellDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(friends::FriendBasic *, m_pData, Data);
public:
	FriendsViewCell();
	~FriendsViewCell();

	static FriendsViewCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();
	void updateStatus(cocos2d::CCObject *pObj = NULL);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };

	void setType(FriendsViewCellType eType);

	void setStatus(friends::GiftState eStatus);

	void setData(friends::FriendBasic *pData);
private:
	CCNode *m_pNodeFriend;
	CCNode *m_pNodeAdd;

	CCLabelTTF *m_pTTFName;
	CCNode *m_pNodeAvatorContainer;
	CCLabelBMFont *m_pBMFLevel;
	CCMenuItem *m_pBtnFriend;
	CCMenuItem *m_pBtnFormation;
	CCNode *m_pNodeReceive;
	CCNode *m_pNodeFormation;
	CCNode *m_pNodeGift;
	CCNode *m_pNodeReturn;
	CCMenuItem *m_pBtnAdd;
	CCMenuItem *m_pBtnAgree;

	UIAnimate *m_pAnimatePower;
	UIAnimate *m_pAnimateCoins;

	CCNode *m_pNodeApply;
	
	CCNode *m_pNodeInvite;
	CCNode *m_pNodeInvited;

	void callBackFriend(CCObject *pSender);
	void callBackAdd(CCObject *pSender);
	void callBackAgree(CCObject *pSender);

	friends::GiftState m_eStatus;

	CCNode *m_pRoot;
	FriendsModel *m_pModel;

	CCLabelBMFont *m_pBMFPower;
	CCLabelBMFont *m_pBMFCoins;
};

#endif