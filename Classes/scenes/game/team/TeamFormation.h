//
//  TeamFormation.h
//  组队阵容&buff
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__TeamFormation__
#define __QSMY__TeamFormation__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "Team.h"
#include "../formation/ChangeFormationView.h"
#include "TeamFormationItem.h"
#include "../common/gui/AwardItemSmall.h"

class TeamFormation
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public TeamFormationItemDelegate
{
	CC_SYNTHESIZE(TeamViewDelegate *, m_pDelegate, Delegate);
public:
	TeamFormation();
	~TeamFormation();

	static TeamFormation* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();
	void updateItem();
	void updateSelect();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };

	virtual void TeamFormationItemSelected(TeamFormationItem *pItem);
private:
	TeamFormationItem *m_pWarrior[FORMATION_MAX];
	AwardItemSmall *m_pItem[ITEM_MAX];
	CCLabelTTF *m_pTTFTips;
	DButton *m_pDBtnBattle;
	CCRichLabelTTF *m_pTTFName;
	CCLabelBMFont *m_pBMFNum;

	FriendsModel *m_pModel;

	void callBackBattle(CCObject *pSender);
	void callBackItem(CCObject *pSender);

	void callBackDeselect(CCObject *pSender);

	uint32_t m_nItemSelected;
	std::map<uint32_t, uint32_t> m_mItemNumUsed;
};

#endif