//
//  GuildBossView.h
//  公会主页
//
//  Created by Eci on 14-01-26.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossView__
#define __QSMY__GuildBossView__

#define MAX_GUILD_BOSS_POINT 7

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class GuildBossViewDelegate;
class GuildBossPoint;

class GuildBossView 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{

public:
	GuildBossView();
	~GuildBossView();

	static GuildBossView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void setDelegate(GuildBossViewDelegate *pDelegate);

	void callBackTarget(CCObject *pSender);
	void callBackRule(CCObject *pSender);

	void callBackRank(CCObject *pSender);
	void callBackContribution(CCObject *pSender);

	void callBackSettle(CCObject *pSender);
	void callBackSettle(const Json::Value &,const Json::Value &responseData);
	void callBackPrev(CCObject *pSender);
	void callBackRobSettle(const Json::Value &,const Json::Value &responseData);
	void callBackCurr(CCObject *pSender);

	void callBackWeek(CCObject *pSender);
	
	void callBackHide(CCNode *pNode);
protected:
	GuildBossViewDelegate *m_pDelegate;
private:
	CCMenuItem *m_pBtnPoint[MAX_GUILD_BOSS_POINT];
	CCMenuItemImage *m_pBtnDeclare;
	CCNode *m_pDeclare;
	DButton *m_pBtnRule;
	CCLabelTTF *m_pTTFName;
	CCLabelBMFont *m_pBMFTime;

	CCNode *m_pNodeDay;

	DButton *m_pBtnSettle;
	DButton *m_pBtnPrev;
	CCMenuItem *m_pBtnCurr;
	CCNode	*m_pOpen;
	CCNode	*m_pClose;
	CCNode *m_pNodeDragon;
	CCNode *m_pNodeFight;
	CCNode *m_pNodeDragonBg;
	CCNode *m_pNodeFightBg;
	CCNode *m_pNodeWaiting;
	CCNode *m_pNodeRunning;

	CCLabelBMFont *m_pBMFRunningTime;
	CCLabelBMFont *m_pBMFCurrTime;
	CCLabelBMFont *m_pBMFNextTime;
	CCNode *m_pNodeTime;

	CCMenuItem *m_pBtnWeek[MAX_GUILD_BOSS_POINT];
	CCNode *m_pNodeWeek[MAX_GUILD_BOSS_POINT];

	CCNode *m_pNodeToday;
	CCNode *m_pNodeYesterday;

	DButton *m_pBtnGuildInfo;
	DButton *m_pBtnPersonInfo;

	GuildBossModel *m_pModel;
	guildboss::GuildBoss *m_pBoss;

	float m_fWaitTime;

	guildboss::GuildBossState m_eLastState;
	void updateDeadLine(float dt);
	void update(float dt);
	void updateStart(float dt);
	void updatePoints();
};

#endif