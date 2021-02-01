//
//  GuildBossHealth.h
//  公会被救援
//
//  Created by Eci on 14-03-03.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossHealth__
#define __QSMY__GuildBossHealth__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class GuildBossViewDelegate;

class GuildBossHealth 
	:public DLayer
 	,public cocos2d::extension::CCBMemberVariableAssigner
 	,public cocos2d::extension::CCBSelectorResolver
{

public:
	GuildBossHealth();
	~GuildBossHealth();

	static GuildBossHealth* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void setNotice(guildboss::GuildBossNotice *m_pNotice);
protected:
	CC_SYNTHESIZE(GuildBossViewDelegate *, m_pDelegate, Delegate);
private:
	void callBackClose(cocos2d::CCObject * obj);

	cocos2d::CCLabelTTF *m_pTTFName;
};

#endif