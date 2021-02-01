//
//  GuildBossRank.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossRank__
#define __QSMY__GuildBossRank__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../guild/DLoadingTabelView.h"

#include "GuildBossRankCell.h"

namespace guildboss
{
	enum GuildBossRankTableType
	{
		GuildBossRankTableTypeGuild = 0,
		GuildBossRankTableTypeHatred = 1,
		GuildBossRankTableTypeCount = 2,
	};
}

class GuildBossViewDelegate;

class GuildBossRank 
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCTableViewDelegate
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCBSelectorResolver
	,public DLoadingTableViewDelegate
{

public:
	GuildBossRank();
	~GuildBossRank();

	static GuildBossRank* create();
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

	void callBackClose(CCObject *pSender);
	void callBackSelect(CCObject *pSender);

	void setType(guildboss::GuildBossRankTableType type);
protected:
	CC_SYNTHESIZE(GuildBossViewDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(int, m_iSelected, Selected);
private:
	CCNode *m_pContainer;
	CCNode	*m_pPersonRank;
	CCNode	*m_pGuildRank;

	CCNode *m_pCell;//[guildboss::GuildBossRankTableTypeCount];
	CCData *m_pData;//[guildboss::GuildBossRankTableTypeCount];

	DTableView *m_pTableView[guildboss::GuildBossRankTableTypeCount];
	DButton *m_pCloseButton;
	DButton *m_pBtnSwitch[guildboss::GuildBossRankTableTypeCount];
	CCNode *m_pNodeTitle[guildboss::GuildBossRankTableTypeCount];

	guildboss::GuildBossRankTableType m_eType;

	void refresh();

	GuildBossModel *m_pModel;
	CCArray *m_pList;

	bool m_bIsPreparing;
	bool m_bLastWeek;

#pragma mark DLoadingTableViewDelegate
public:
	virtual void startLoading();
};

#endif