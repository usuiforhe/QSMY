//
//  GuildBossFight.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossFight__
#define __QSMY__GuildBossFight__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

#include "GuildBossFightCell.h"

class GuildBossViewDelegate;

class GuildBossFight 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCTableViewDelegate
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCBSelectorResolver
	,public GuildBossFightCellDelegate
	,public UIAnimateDelegate
{

public:
	GuildBossFight();
	~GuildBossFight();

	static GuildBossFight* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();
	void updateTable();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}


#pragma mark CCTableViewDataSource
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);

#pragma mark CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};

#pragma mark GuildBossFightCellDelegate
	virtual void GuildBossFightUser(GuildBossFightCell *pNode, guildboss::GuildBossUserBasic *pTarget);
#pragma mark UIAnimateDelegate
	virtual void animationComplete(UIAnimate *pAnimate);


	void callBackClose(CCObject *pSender);
	void callBackRank(CCObject *pSender);
	void callBackContribution(CCObject *pSender);
protected:
	CC_SYNTHESIZE(GuildBossViewDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(int, m_iSelected, Selected);
	CC_SYNTHESIZE(int, m_iRobDamage, RobDamage);
private:
	CCNode *m_pContainer;
	CCMenuItem *m_pBtnBack;
	DButton *m_pDBtnCheck;
	CCLabelBMFont *m_pBMFTime;
	CCLabelTTF *m_pTTFName;
	CCLabelBMFont *m_pBMFFight;
	CCLabelBMFont *m_pBMFCount;
	UIAnimate *m_pAnimateStar;
	DButton *m_pBtnRefresh;
	CCLabelBMFont *m_pBMFRefreshCD;
	CCNode *m_pNodeFreeCount;
	CCNode *m_pNodePayedCount;
	CCNode *m_pTxtEmpty;

	DButton *m_pBtnGuildInfo;
	DButton *m_pBtnPersonInfo;

	CCNode *m_pCell;

	DTableView *m_pTableView;

	GuildBossModel *m_pModel;
	CCArray *m_pList;

	bool m_bOnlyDeclared;
	void callBackCheck(CCObject *pSender);

	float m_fColdDelay;
	bool m_bReloadCountDown;
	void updateCountDown(float dt);
	float m_fWaitTime;
	void update(float dt);
	CCLabelBMFont *m_pBMFStar;
	void showRobDamage();

	float m_fRefreshCD;
	void callBackRefresh(CCObject *pSender);
	void callBackRefresh(const Json::Value &,const Json::Value &responseData);
	void updateRefresh(float dt);
};

#endif