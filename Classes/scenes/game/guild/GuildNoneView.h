//
//  GuildNoneView.h
//  无工会
//
//  Created by Eci on 13-10-23.

//  Copyright (c) 2013年 thedream. All rights reserved.
//



#ifndef __QSMY__GuildNoneView__
#define __QSMY__GuildNoneView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class GuildNoneViewDelegate
{
public:
	virtual void willJoinGuild() = 0;
	virtual void willCreateGuild() = 0;
	virtual void willClose() = 0;
};

class GuildNoneView 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	GuildNoneView();
	~GuildNoneView();

	static GuildNoneView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

protected:
	CC_SYNTHESIZE(GuildNoneViewDelegate *, m_pDelegate, Delegate);
private:
	DButton *m_pButtonJoin;
	DButton *m_pButtonCreate;
	DButton *m_pCloseButton;
#pragma mark callback function
	void callBackJoin(CCObject *sender);
	void callBackCreate(CCObject *sender);
	void callBackClose(CCObject *sender);
};

#endif