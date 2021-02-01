//
//  TeamMake.h
//  进行组队
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__TeamMake__
#define __QSMY__TeamMake__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "Team.h"
#include "../formation/ChangeFormationView.h"
#include "TeamFormationItem.h"

class TeamMake
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public TeamFormationItemDelegate
{
	CC_SYNTHESIZE(TeamViewDelegate *, m_pDelegate, Delegate);
public:
	TeamMake();
	~TeamMake();

	static TeamMake* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };


	virtual void TeamFormationItemSelected(TeamFormationItem *pItem);
private:
	CCLabelBMFont *m_pBMFGold;
	CCNode *m_pNodeRush;
	CCNode *m_pNodeConfirm;

	TeamFormationItem *m_pWarrior[FORMATION_MAX];
	CCRichLabelTTF *m_pTTFName;
	CCLabelBMFont *m_pBMFNum;

	CCMenuItem *m_pBtnRule;

	CCNode *m_pNodePoint;

	CCNode *m_pNodeFree;
	CCNode *m_pNodeVip;
	CCLabelBMFont *m_pBMFVip;
	CCNode *m_pNodeMaxVip;
	CCMenuItem *m_pBtnGo;
	CCNode *m_pNodeFreeFont;
	CCNode *m_pNodePayed;
	CCNode *m_pNodeLock;

	FriendsModel *m_pModel;

	void callBackConfirm(CCObject *pSender);
	void callBackRule(CCObject *pSender);
	void callBackClose(CCObject *pSender);

	uint32_t m_nEmptyIdx;
};

#endif