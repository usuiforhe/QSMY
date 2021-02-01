//
//  GuildApplyTable.h
//  公会申请
//
//  Created by Eci on 13-10-25.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildApplyTable__
#define __QSMY__GuildApplyTable__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "GuildApplyCell.h"

class GuildApplyTable
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public GuildApplyCellDelegate
{
public:
	GuildApplyTable();
	~GuildApplyTable();

	static GuildApplyTable* create(const cocos2d::CCSize tableSize);
	virtual bool init(const cocos2d::CCSize tableSize);

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

private:
	int32_t m_iSelectIndex;
	uint32_t m_iType;
	GuildModel *m_pModel;
	DTableView *m_pListView;

	void updateView();
	void onTouch(cocos2d::CCObject*);
public:

#pragma mark GuildApplyCellDelegate

	virtual void willDealApply(uint32_t applyID, int actoin);
	void willDealApply(const Json::Value &,const Json::Value &responseData);
};

#endif