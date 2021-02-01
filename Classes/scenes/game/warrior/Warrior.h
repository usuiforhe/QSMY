//
//  Warrior.h
//  弟子模块
//  Created by cll on 13-3-28.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__WARRIOR__
#define __QSMY__WARRIOR__
#include "PictureCollect.h"
#include "AppInclude.h"
#include "components/components.h"
#include "model/WarriorModel.h"
#include "WarriorListView.h"
#include "WarriorDelegate.h"
#include "../Game.h"

class WarriorInfoView;
class SoulInfoView;
class PromotionView;
class TransForceView;
class WarriorTransForceSettleView;
class WarriorDevelopView;
class WarriorDevelopSettleView;
class RecruitSettleView;
class SkillInfoView;
class SkillUpgradeView;
class WarriorUpgradeView;
class WarriorSortView;
class SoulListView;
//class PictureCollect;
class PictureCollectCell;

class Warrior:
public DLayer,
public cocos2d::extension::CCBMemberVariableAssigner,
public WarriorDelegate
,public BackKeyInterface
,DTabViewDelegate
,WarriorSortDelegate
{

public:
	Warrior();
	~Warrior();
	virtual bool init();
	CREATE_FUNC(Warrior);
    virtual void onEnter();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual void SelectTabAtIndex(DTabView *tabView, uint32_t idx);
    virtual void setSortList(warrior::WarriorSortType type);
private:
	virtual void closeInfoView(WarriorViewType type);
	virtual void showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,cocos2d::CCObject* param = NULL);
	//virtual void showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,PictureCollectCell* param_t,cocos2d::CCObject* param = NULL);
	virtual void showDevSettleView(warrior::WarriorCurInfo * pWarrior);
    
    
private:
    void showViewList();
	void showPopView();
	void onSortClick(cocos2d::CCObject* psender);

private:
	cocos2d::CCNode         * m_pContainer;
    cocos2d::CCNode         * m_pPageContainer;
    cocos2d::CCNode         * m_pTabContainer;
	DTabView                *m_pDTabView;

	cocos2d::CCMenuItemImage *m_pSortButton;
    
	TransForceView              *m_pTransForceView;
	WarriorTransForceSettleView *m_pTransSettleView;
	WarriorDevelopView          *m_pDevelopView;	
	WarriorDevelopSettleView    *m_pDevSettleView;
	RecruitSettleView           *m_pRecruitSettleView;
	WarriorInfoView             *m_pWarriorInfoView;
	WarriorInfoView             *m_pSoulInfoView;
	PromotionView               *m_pPromotionView;
    WarriorUpgradeView          *m_pUpgradeView;
    
    SkillInfoView               *m_pSkillInfo;
	SkillUpgradeView            *m_pSkillUpgrade;

	WarriorViewType				m_eShowingType;

	WarriorListView            *m_pWarriorListView;
	SoulListView               *m_pSoulList;
	PictureCollect             *m_pPictureCollect_Diff;
	WarriorSortView            *m_pSortView;
public:
	virtual void onBackKey();
};



#endif /* defined(__QSMY__WARRIOR__) */
