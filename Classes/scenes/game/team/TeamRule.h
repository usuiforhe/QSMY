//
//  TeamRule.h
//  组队规则
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__TeamRule__
#define __QSMY__TeamRule__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class TeamViewDelegate;

class TeamRule 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(TeamViewDelegate *, m_pDelegate, Delegate);
public:
	TeamRule();
	~TeamRule();

	static TeamRule* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };
private:

	void callBackClose(CCObject *pObj);
};

#endif