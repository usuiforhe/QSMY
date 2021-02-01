//
//  GuildManageView.h
//  公会管理
//
//  Created by Eci on 13-10-24.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__GuildManageView__
#define __QSMY__GuildManageView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "GuildManageTable.h"
#include "GuildDemiseView.h"

class GuildManageViewDelegate
{
public:
	virtual void willCloseManageView() = 0;
	virtual void openUpperPopup(CCNode *pPopup) = 0;
};


class GuildManageView
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public GuildManageTableDelegate
	,public GuildDemiseViewDelegate
{
public:
	GuildManageView();
	~GuildManageView();

	static GuildManageView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

protected:
	CC_SYNTHESIZE(GuildManageViewDelegate *, m_pDelegate, Delegate);
private:
	GuildModel *m_pModel;
	CCNode *m_pContainer;
    CCNode *m_pNodeRequestNum;

	cocos2d::CCMenuItem *m_pButtonCancel;
	int32_t m_iSelectIndex;

	DTabView    *m_pTabView;
	GuildDemiseView *m_pDemiseView;
    

#pragma mark callback function
	void callBackCancel(CCObject *sender);
	void updateView();

public:
#pragma mark GuildManageViewDelegate
	virtual void willShowDemiseView();
	virtual void willShowFormation(DLayer*);
#pragma mark GuildDemiseViewDelegate
	virtual void closeGuildDemiseView();
	virtual void demiseToUser(uint32_t userID);
	void demiseToUser(const Json::Value &,const Json::Value &responseData);
};

#endif