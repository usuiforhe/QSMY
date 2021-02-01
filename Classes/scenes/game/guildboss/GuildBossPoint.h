//
//  GuildBossPoint.h
//  苍龙星宿
//
//  Created by Eci on 14-01-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__GuildBossPoint__
#define __QSMY__GuildBossPoint__

#include "AppInclude.h"
#include "components/DProgressBar.h"
#include "managers/DataManager.h"

class GuildBossViewDelegate;

class GuildBossPoint
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    
public:
    CREATE_FUNC(GuildBossPoint);
    GuildBossPoint();
    virtual ~GuildBossPoint();
    D_INIT_DEFAULT(CCNode);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };

	void callBackPoint(CCObject *pSender);

	void setCleared(bool cleared);
	void setCountingDown(int countingDown);
	void setDayCountingDown(int dayCountingDown);
	bool getIsOpen();
protected:
	CC_SYNTHESIZE(GuildBossViewDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE(GuildBossPoint *, m_pNextPoint, NextPoint);
private:
	cocos2d::CCMenuItem *m_pBtnPoint;
	cocos2d::CCLabelBMFont *m_pBMFTime;
	cocos2d::CCNode *m_pNodeTime;
};


class GuildBossPointLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildBossPointLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildBossPoint);
};

#endif /* defined(__QSMY__GuildBossPoint__) */
