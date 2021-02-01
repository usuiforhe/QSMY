//
//  FriendsSelectView.h
//  选择好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__FriendsSelectView__
#define __QSMY__FriendsSelectView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "Friends.h"
#include "FriendsSelectViewCell.h"

class FriendsSelectView
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public FriendsSelectViewCellDelegate
{
	CC_SYNTHESIZE(FriendsViewDelegate *, m_pDelegate, Delegate);
public:
	FriendsSelectView();
	~FriendsSelectView();

	static FriendsSelectView* create();
	virtual bool init();

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

#pragma mark FriendsSelectViewCellDelegate

	virtual void FriendsSelectViewCellSelected(FriendsSelectViewCell *pCell);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void setType(FriendsNav eType);

private:
	CCNode *m_pNodeRemove;
	CCNode *m_pNodeMessage;

	DButton *m_pDBtnBack;

	CCLabelBMFont *m_pBMFCount;

	DButton *m_pDBtnRemove;
	DButton *m_pDBtnMessage;

	CCNode *m_pNodeContainer;

	DTableView *m_pTableView;


	CCNode *m_pCell;

	void callBackBack(CCObject *pSender);
	void callBackRemove(CCObject *pSender);
	void callBackMessage(CCObject *pSender);

	std::vector<uint32_t> m_vRemove;
	uint32_t m_uSelected;
	
	FriendsNav m_eNav;

	FriendsModel *m_pModel;
	CCArray *m_pList;
};

#endif