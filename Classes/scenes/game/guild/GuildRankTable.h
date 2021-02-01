//
//  GuildRankTable.h
//  公会排名
//
//  Created by Eci on 13-10-24.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildRankTable__
#define __QSMY__GuildRankTable__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "DLoadingTabelView.h"
#include "GuildDetailsView.h"

class GuildRankTableDelegate
{
public:
	virtual void willShowGuildDetails(uint32_t guildID, uint32_t rank) = 0;
};

class GuildRankTable
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public DLoadingTableViewDelegate
{
public:
	GuildRankTable();
	~GuildRankTable();

	static GuildRankTable* create(const cocos2d::CCSize tableSize , int val);
	virtual bool init(const cocos2d::CCSize tableSize , int val);

	virtual void onEnter();
	virtual void onExit();

#pragma mark CCTableViewDataSource
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);

#pragma mark CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};

#pragma mark setter
	void setType(uint32_t iType);
protected:
	uint32_t m_iType;
	CC_SYNTHESIZE(GuildRankTableDelegate *, m_pDelegate, Delegate);
private:
	GuildModel *m_pModel;

	DLoadingTableView *m_pListView;


	int32_t m_iSelectIndex;
	uint32_t m_iLastCount;

	void updateView();


#pragma mark DLoadingTableViewDelegate
public:
	virtual void startLoading();
	void startLoading(const Json::Value &,const Json::Value &responseData);

};

#endif