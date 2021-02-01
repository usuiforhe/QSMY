//
//  GuildRankView.h
//  公会排名
//
//  Created by Eci on 13-10-24.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildRankView__
#define __QSMY__GuildRankView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "GuildDetailsView.h"
#include "GuildRankTable.h"

class GuildRankViewDelegate
{
public:
	virtual void willCloseRankView() = 0;
	virtual void openGuildDetail(CCNode *popUp) = 0;
};


class GuildRankView
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public GuildDetailsViewDelegate
	,public GuildRankTableDelegate
//	,public cocos2d::extension::CCTableViewDataSource
//	,public cocos2d::extension::CCTableViewDelegate
{
public:
	GuildRankView();
	~GuildRankView();

	static GuildRankView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

protected:
	CC_SYNTHESIZE(GuildRankViewDelegate *, m_pDelegate, Delegate);
private:
	GuildModel *m_pModel;
	CCNode *m_pContainer;
	CCNode *m_pTableNode;
	cocos2d::CCMenuItemImage *m_pButtonCancel;
	cocos2d::CCLabelBMFont	*m_pGuildRank;

	int32_t m_iSelectIndex;
	DTabView    *m_pDTabView;
	GuildDetailsView *m_pDetailsView;

#pragma mark callback function
	void callBackCancel(CCObject *sender);


	void updateView();


	void willShowGuildDetails(uint32_t guildID, uint32_t rank);
#pragma mark GuildDetailsViewDelegate
	virtual void closeGuildDetailsView();
};

#endif