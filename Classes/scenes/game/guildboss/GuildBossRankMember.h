//
//  GuildBossRankMember.h
//  公会主页
//
//  Created by Eci on 14-03-28.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossRankMember__
#define __QSMY__GuildBossRankMember__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../guild/DLoadingTabelView.h"

#include "GuildBossRankCell.h"


namespace guildboss
{
	enum GuildBossRankMemberTableType
	{
		GuildBossRankMemberTableTypeContribution = 0,
		GuildBossRankMemberTableTypeRescue = 1,
		GuildBossRankMemberTableTypeCount = 2,
	};
}

class GuildBossViewDelegate;

class GuildBossRankMember 
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCTableViewDelegate
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCBSelectorResolver
	,public DLoadingTableViewDelegate
{

public:
	GuildBossRankMember();
	~GuildBossRankMember();

	static GuildBossRankMember* create();
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

	void setType(guildboss::GuildBossRankMemberTableType eType);
protected:
	CC_SYNTHESIZE(GuildBossViewDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(int, m_iSelected, Selected);
private:
	CCNode *m_pContainer;
	CCNode	*m_pPersonRank;
	CCNode	*m_pGuildRank;

	CCNode *m_pCell;//[guildboss::GuildBossRankMemberTableTypeCount];
	CCData *m_pData;//[guildboss::GuildBossRankMemberTableTypeCount];

	DTableView *m_pTableView[guildboss::GuildBossRankMemberTableTypeCount];
	DButton *m_pCloseButton;
	DButton *m_pBtnSwitch[guildboss::GuildBossRankMemberTableTypeCount];
	CCNode *m_pNodeTitle[guildboss::GuildBossRankMemberTableTypeCount];

	guildboss::GuildBossRankMemberTableType m_eType;

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