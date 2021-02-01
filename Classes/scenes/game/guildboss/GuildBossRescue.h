//
//  GuildBossRescue.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossRescue__
#define __QSMY__GuildBossRescue__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

#include "GuildBossRescueCell.h"

class GuildBossViewDelegate;

class GuildBossRescue 
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCTableViewDelegate
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCBSelectorResolver
	,public GuildBossRescueCellDelegate
{

public:
	GuildBossRescue();
	~GuildBossRescue();

	static GuildBossRescue* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

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

#pragma mark GuildBossRescueCellDelegate
	virtual void GuildBossRescueUser(GuildBossRescueCell *pNode, uint32_t userID);

	void callBackClose(CCObject *pSender);
	void callBackAll(CCObject *pSender);
protected:
	CC_SYNTHESIZE(GuildBossViewDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(int, m_iSelected, Selected);
private:
	CCNode *m_pContainer;
	DButton *m_pDBtnBack;
	DButton *m_pDBtnAll;
	DButton *m_pCloseButton;
	CCLabelBMFont *m_pBMFGoldAll;

	CCNode *m_pCell;

	DTableView *m_pTableView;

	GuildBossModel *m_pModel;
	CCArray *m_pList;
};

#endif