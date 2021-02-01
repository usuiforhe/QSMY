//
//  GuildCreateView.h
//  公会创建
//
//  Created by Eci on 13-10-23.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildCreateView__
#define __QSMY__GuildCreateView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class GuildCreateViewDelegate
{
public:
	virtual void createGuildWithName(const char * pGuildName) = 0;
	virtual void closeGuildCreateView() = 0;
};


class GuildCreateView 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
    ,public cocos2d::extension::CCEditBoxDelegate
{
public:
	GuildCreateView();
	~GuildCreateView();

	static GuildCreateView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    

#pragma mark CCEditBoxDelegate

    virtual void editBoxReturn(CCEditBox* editBox);

private:
	DButton *m_pButtonClose;
	DButton *m_pButtonCreate;
	DButton *m_pButtonCreate2;
	CCNode *m_pContainer;
	CCEditBox *m_pEditBox;

#pragma mark callback function

	void callBackCreate(CCObject *sender);
	void callBackClose(CCObject *sender);
protected:
	CC_SYNTHESIZE(GuildCreateViewDelegate *, m_pDelegate, Delegate);
};

#endif