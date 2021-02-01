//
//  ActivitySoulExchange.h
//  活动模块
//
//  Created by Eci on 14-3-25.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__ActivitySoulExchange__
#define __QSMY__ActivitySoulExchange__

#define K_BMF_SOUL_MAX	4
#define K_NODE_CONTAINER_MAX	2
#define K_NODE_SOUL_MAX	2

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../common/gui/WarriorHeadSmall.h"

class ActivitySoulExchange
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	ActivitySoulExchange();
	virtual ~ActivitySoulExchange();
	CREATE_FUNC(ActivitySoulExchange);
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();
	void updateVisible();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void onHeroClick(CCObject* target);
private:
	cocos2d::CCLabelBMFont *m_pMyPoint;
	cocos2d::CCLabelBMFont *m_pBMFPoint;
	cocos2d::CCLabelBMFont *m_pBMFSoul[K_BMF_SOUL_MAX];
	cocos2d::CCNode *m_pNode[K_NODE_SOUL_MAX];
	WarriorHeadSmall *m_pNode1Container[K_NODE_CONTAINER_MAX];
	WarriorHeadSmall *m_pNode0Container;
	cocos2d::CCMenuItem *m_pBtnEnter;
	UIAnimate *m_pAnimateSoulPoint;

	ActivityModel *m_pModel;

	void callBackEnter(CCObject *pSender);
};

#endif /* defined(__QSMY__ActivitySoulExchange__) */