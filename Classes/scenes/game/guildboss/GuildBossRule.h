//
//  GuildBossRule.h
//  公会主页
//
//  Created by Eci on 14-02-21.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossRule__
#define __QSMY__GuildBossRule__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

#define RULE_NODE_MAX 2

class GuildBossViewDelegate;

class GuildBossRule 
	:public DPopup
 	,public cocos2d::extension::CCBMemberVariableAssigner
 	,public cocos2d::extension::CCBSelectorResolver
{

public:
	GuildBossRule();
	~GuildBossRule();

	static GuildBossRule* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

protected:
	CC_SYNTHESIZE(GuildBossViewDelegate *, m_pDelegate, Delegate);
private:
	cocos2d::extension::CCScrollView    *m_pPage[RULE_NODE_MAX];
	void closeCallBack(cocos2d::CCObject * obj);

	cocos2d::CCNode *m_pNode[RULE_NODE_MAX];
	DButton *m_pBtnSwitch[RULE_NODE_MAX];
	DButton *m_pCloseButton;

	void callBackSwitch(cocos2d::CCObject *pSender);
};

#endif