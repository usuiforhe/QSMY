//
//  FriendsTable.h
//  好友列表
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__FriendsTable__
#define __QSMY__FriendsTable__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "FriendsViewCell.h"

class FriendsViewDelegate;

class FriendsTable
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public FriendsViewCellDelegate
{
	CC_SYNTHESIZE(FriendsViewDelegate *, m_pDelegate, Delegate);
public:
	FriendsTable();
	~FriendsTable();

	static FriendsTable* create(CCNode *pContainer);
	virtual bool init(CCNode *pContainer);

	virtual void onEnter();
	virtual void onExit();

	void updateView();

#pragma mark CCTableViewDataSource
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);

#pragma mark CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};

	void setType(FriendsViewCellType eType);

#pragma mark FriendsViewCellDelegate
	virtual void FriendsViewCellGift(FriendsViewCell *pCell);
	virtual void FriendsViewCellReceive(FriendsViewCell *pCell);
	virtual void FriendsViewCellFormation(FriendsViewCell *pCell);
	virtual void FriendsViewCellAdd(FriendsViewCell *pCell);
	virtual void FriendsViewCellAgree(FriendsViewCell *pCell);
	virtual void FriendsViewCellRefuse(FriendsViewCell *pCell);
private:
	FriendsViewCellType m_eType;

	DTableView *m_pTableView;
	CCNode *m_pCell;

	FriendsModel *m_pModel;
	CCArray *m_pList;

	uint32_t m_uSelected;
};

#endif