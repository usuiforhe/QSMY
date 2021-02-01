//
//  GuildSearchView.h
//  公会搜索
//
//  Created by Eci on 13-10-25.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildSearchView__
#define __QSMY__GuildSearchView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"


class GuildSearchViewDelegate
{
public:
	virtual void searchGuildWithName(const char * pGuildName) = 0;
	virtual void closeGuildSearchView() = 0;
};


class GuildSearchView 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCEditBoxDelegate
{
public:
	GuildSearchView();
	~GuildSearchView();

	static GuildSearchView* create();
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
protected:
	CC_SYNTHESIZE(GuildSearchViewDelegate *, m_pDelegate, Delegate);
};

#endif