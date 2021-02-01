//
//  FriendGift.h
//  商品列表
//  Created by cll on 13-6-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__FRIENDGIFT__
#define __QSMY__FRIENDGIFT__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/DataManager.h"
#include "FriendRecommend.h"

class FriendGift
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	FriendGift();
	~FriendGift();

	static FriendGift* create(const cocos2d::CCSize& size);
	virtual bool init(const cocos2d::CCSize& size);
	virtual void onEnter();
	virtual void onExit();

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	
private:
	void revieveAllWithGift(CCObject*);
	void updateView();

private:
	cocos2d::extension::CCData	*m_pData;
	DTableView							*m_pListView;
	cocos2d::CCArray					m_pDataList;

private:
	cocos2d::CCMenuItemImage	*m_pReceiveAll;
	cocos2d::CCLabelBMFont			*m_pTimes;
	cocos2d::CCNode						*m_pPageContainer;
	
private:
	FriendModel                   *m_pModel;
};

#endif /* defined(__QSMY__FRIENDGIFT__) */
