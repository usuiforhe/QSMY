//
//  GuildJoinView.h
//  加入公会
//
//  Created by Eci on 13-10-23.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildJoinView__
#define __QSMY__GuildJoinView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "GuildJoinCell.h"
#include "GuildSearchView.h"
#include "GuildDetailsView.h"
#include "DLoadingTabelView.h"

class GuildJoinViewDelegate
{
public:
	virtual void closeGuildSearchView() = 0;
	virtual void openGuildDetail(CCNode *popUp) = 0;
};

class GuildJoinView
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public GuildJoinCellDelegate
	,public GuildSearchViewDelegate
	,public GuildDetailsViewDelegate
	,public DLoadingTableViewDelegate
	,public cocos2d::extension::CCEditBoxDelegate
{
public:
	GuildJoinView();
	~GuildJoinView();

	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox){};

	static GuildJoinView* create();
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
	CC_SYNTHESIZE(GuildJoinViewDelegate *, m_pDelegate, Delegate);
private:
	GuildModel *m_pModel;

	DButton *m_pButtonSearch;
	DButton *m_pButtonCancel;
	CCNode *m_pContainer;
	CCNode *m_pBoxContainer;
	cocos2d::CCMenuItemImage *m_pMenuList;
	cocos2d::extension::CCEditBox	*m_pEditBox;

	DLoadingTableView *m_pListView;

	GuildSearchView *m_pSearchView;

	int32_t m_iSelectIndex;

	//初始化搜索框
	void initEditBox();
	//重新获取排行榜列表
	void getGuildList(cocos2d::CCObject*);

#pragma callback function
	void callBackSearch(CCObject *sender);
	void callBackCancel(CCObject *sender);
	void updateView();
	void onTouch(cocos2d::CCObject*);

public:

#pragma mark GuildJoinCellDelegate
	virtual void willApplyForTheGuild(uint32_t guildID, int action);
	virtual void willShowGuildDetails(uint32_t guildID, uint32_t rank);
	void willApplyForTheGuild(const Json::Value &,const Json::Value &responseData);

#pragma mark GuildSearchViewDelegate
	virtual void searchGuildWithName(const char * pGuildName);
	virtual void closeGuildSearchView();
	void searchGuildWithName(const Json::Value &,const Json::Value &responseData);

#pragma mark GuildDetailsViewDelegate
	virtual void closeGuildDetailsView();

#pragma mark DLoadingTableViewDelegate
	virtual void startLoading();
	void startLoading(const Json::Value &,const Json::Value &responseData);

private:
	GuildDetailsView *m_pDetailsView;

	uint32_t m_iLastCount;
    
    bool m_bScrollToTop;

};

#endif