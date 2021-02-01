//
//  TeamTreasure.h
//  组队可能掉落查看
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__TeamTreasure__
#define __QSMY__TeamTreasure__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "Team.h"
#include "../common/AwardListView.h"

class TeamTreasure
	: public BaseAwardListView
{
	CC_SYNTHESIZE(TeamViewDelegate *, m_pDelegate, Delegate);
public:
	TeamTreasure();
	~TeamTreasure();

	static TeamTreasure* create();
	virtual bool initTreasure();

	virtual void onEnter();
	virtual void onExit();

	void updateView();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
protected:

	virtual void addChildrenBeforeList();
private:
	DButton *m_pDBtnConfirm;

	FriendsModel *m_pModel;

	void callBackConfirm(CCObject *pSender);
};

#endif