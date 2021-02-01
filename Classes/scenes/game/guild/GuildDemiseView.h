//
//  GuildDemiseView.h
//  公会让位
//
//  Created by Eci on 13-10-26.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildDemiseView__
#define __QSMY__GuildDemiseView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "GuildJoinCell.h"
#include "GuildSearchView.h"

class GuildDemiseViewDelegate
{
public:
	virtual void closeGuildDemiseView() = 0;
	virtual void demiseToUser(uint32_t userID) = 0;
};

class GuildDemiseView
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
{
public:
	GuildDemiseView();
	~GuildDemiseView();

	static GuildDemiseView* create();
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
protected:
	CC_SYNTHESIZE(GuildDemiseViewDelegate *, m_pDelegate, Delegate);
private:
	GuildModel *m_pModel;

	DButton *m_pButtonEnter;
	DButton *m_pButtonCancel;
	DButton *m_pCloseButton;
	CCNode *m_pContainer;
	cocos2d::CCArray *m_pUserList;

	DTableView *m_pListView;

	int32_t m_iSelectIndex;

	void updateView();

#pragma mark callback function

	void callBackEnter(CCObject *sender);
	void callBackCancel(CCObject *sender);

	void confirmToDemise(MessageType type,DMessageBox* targe);

};

#endif