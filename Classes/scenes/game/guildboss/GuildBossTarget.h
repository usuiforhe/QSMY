//
//  GuildBossTarget.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossTarget__
#define __QSMY__GuildBossTarget__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../guild/DLoadingTabelView.h"

#include "GuildBossTargetCell.h"

class GuildBossViewDelegate;

class GuildBossTarget 
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCTableViewDelegate
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCBSelectorResolver
	,public GuildBossTargetCellDelegate
	,public DLoadingTableViewDelegate
{

public:
	GuildBossTarget();
	~GuildBossTarget();

	static GuildBossTarget* create();
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

#pragma mark GuildBossTargetCellDelegate
	virtual void GuildBossTargetSelect(GuildBossTargetCell *pNode);
	virtual void callBackSubmit(CCObject *pSender);

	void callBackClose(CCObject *pSender);
protected:
	CC_SYNTHESIZE(GuildBossViewDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(int, m_iSelected, Selected);
private:
	CCNode *m_pContainer;
	DButton *m_pDBtnSubmit;
	DButton *m_pCloseButton;
	CCNode *m_pCell;

	DLoadingTableView *m_pTableView;

	cocos2d::CCArray *m_pList;
	GuildBossModel *m_pModel;

	bool m_bLastWeek;


#pragma mark DLoadingTableViewDelegate
public:
	virtual void startLoading();
};

#endif