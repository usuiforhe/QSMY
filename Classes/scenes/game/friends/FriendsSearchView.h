//
//  FriendsSearchView.h
//  好友搜索
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__FriendsSearchView__
#define __QSMY__FriendsSearchView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class FriendsViewDelegate;

class FriendsSearchView 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCEditBoxDelegate
{
	CC_SYNTHESIZE(FriendsViewDelegate *, m_pDelegate, Delegate);
public:
	FriendsSearchView();
	~FriendsSearchView();

	static FriendsSearchView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

#pragma mark CCEditBoxDelegate
	virtual void editBoxReturn(CCEditBox* editBox);

private:
	CCMenuItem *m_pButtonClose;
	DButton *m_pButtonSearch;
	DButton *m_pButtonCancel;
	CCNode *m_pContainer;
	CCEditBox *m_pEditBox;

#pragma mark callback function
	void callBackSearch(CCObject *sender);
	void callBackClose(CCObject *sender);


	void callBackSearch(const Json::Value &request, const Json::Value &response);
};

#endif