//
//  GuildDetailsView.h
//  公会详情
//
//  Created by Eci on 13-10-28.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildDetailsView__
#define __QSMY__GuildDetailsView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class GuildDetailsViewDelegate
{
public:
	virtual void closeGuildDetailsView() = 0;
};

class GuildDetailsView
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
{
public:
	GuildDetailsView();
	~GuildDetailsView();

	static GuildDetailsView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

#pragma mark CCTableViewDataSource

	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);

#pragma mark CCTableViewDelegate

	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};

#pragma mark setter

	void setRank(uint32_t iRank);

protected:
	CC_SYNTHESIZE(GuildDetailsViewDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE(uint32_t, m_iGuildID, GuildID);
private:
	GuildModel *m_pModel;

	cocos2d::CCMenuItemImage *m_pButtonCancel;
	cocos2d::CCNode *m_pContainer;

	DTableView *m_pListView;

	void updateView();

	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelBMFont *m_pLabelRank;
	cocos2d::CCLabelBMFont *m_pLabelCount;
	cocos2d::CCRichLabelTTF *m_pLabelAnnouncement;
	cocos2d::CCNode		*m_pMarqueeContainer;
	DMarqueeView		*m_pMarqueeAnnouncement;

	guild::GuildInfo *m_pGuildInfo;
	cocos2d::CCArray *m_pGuildUsers;

#pragma mark callback function

	void callBackCancel(CCObject *sender);
};

#endif