//
//  FriendsTipsView.h
//  弹窗
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__FriendsTipsView__
#define __QSMY__FriendsTipsView__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "Friends.h"

class FriendsViewDelegate;

class FriendsTipsView 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE(FriendsViewDelegate *, m_pDelegate, Delegate);
public:
	FriendsTipsView();
	~FriendsTipsView();

	static FriendsTipsView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void setNav(FriendsNav eNav);
private:
	DButton *m_pDBtnCancel;
	DButton *m_pDBtnEnter;
	CCLabelTTF *m_pTTFFullTips;
	CCLabelTTF *m_pTTFRemoveTips;
	CCLabelBMFont *m_pBMFCount;
	CCNode *m_pNodeFullError;
	CCNode *m_pNodeRemoveConfirm;

	FriendsNav m_eNav;
	FriendsModel *m_pModel;

	void callBackEnter(CCObject *pSender);
	void callBackCancel(CCObject *pSender);
};

#endif