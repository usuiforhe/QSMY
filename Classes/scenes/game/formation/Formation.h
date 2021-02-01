//
//  Formation.h
//  阵容
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Formation__
#define __QSMY__Formation__

#include "components/components.h"
#include "model/FormationModel.h"
#include "model/WarriorModel.h"
#include "model/EquipModel.h"
#include "../equip/EquipSelectView.h"
#include "../warrior/WarriorDelegate.h"
#include "../Game.h"
#include "DressChange.h"

class ChangeFormationView;
class FormationView;
class SkillInfoView;
class SkillUpgradeView;
class EquipSelectView;
class EquipInfoView;
class EquipEnhance;
class EquipRefine;
class WarriorUpgradeView;
class WarriorInfoView;
class PromotionView;
class TransForceView;
class WarriorTransForceSettleView;
class WarriorDevelopView;
class WarriorDevelopSettleView;
class RecruitSettleView;
class FormationSelectView;

enum LastPage{
	kChangeFormationView,
	kFormationView,
    kPartnerView
};

#pragma mark - FormationDelegate
class FormationDelegate
:public WarriorDelegate
,public EquipSelecteDelegate
{
	CC_SYNTHESIZE(LastPage, m_pLastPage, LastPage);
public:
    virtual void showFormationView() =0;
    virtual void showChangeFormationView()=0;
	virtual void showFormationSelectView(formation::PositionInfo* pPos)=0;
    virtual void showEquipSelectView(cocos2d::CCArray* pEquiList,formation::PositionInfo* pPos= NULL,uint32_t currEquipProperty=0)=0;
	virtual void showEquipInfoView(uint32_t eid,formation::PositionInfo*)=0;
	virtual void showEquipEnhanceView(uint32_t eid)=0;
	virtual void showEquipRefineView(uint32_t eid)=0;
	virtual	void joinFormation(uint32_t wid,uint32_t posId){};
};


#pragma mark - Formation
class Formation
:public DLayer
,public FormationDelegate
,public BackKeyInterface
{
#pragma mark - - init etc.
public:
    Formation();
    ~Formation();
    virtual bool init();
    CREATE_FUNC(Formation);
    
    void onEnter();
    
#pragma mark - - variable
private:
	DressChange					*m_pDressChange;
    ChangeFormationView         *m_pChangeFormation;
    FormationView               *m_pFormation;
	SkillInfoView               *m_pSkillInfo;
	SkillUpgradeView            *m_pSkillUpgrade;
	EquipSelectView             *m_pEquipList;
	FormationSelectView		*m_pFormationSelectView;
	EquipRefine                 *m_pEquipRefine;
    WarriorInfoView             *m_pWarriorInfoView;
    FormationModel              *m_pFormationModel;
    WarriorModel                *m_pWarriorModel;
    formation::PositionInfo     *m_pSelectPos;
    
    
    cocos2d::CCNode     *m_pPopupLayer;
    cocos2d::CCNode     *m_Container;
    
    bool                m_bFirstEnter;
    
#pragma mark - - FormationDelegate    
private:
    virtual void closeInfoView(WarriorViewType type);
	virtual void showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,cocos2d::CCObject* param = NULL);
private:
    virtual void showDevSettleView(warrior::WarriorCurInfo * pWarrior);
private:
    WarriorUpgradeView              *m_pUpgradeView;
	WarriorTransForceSettleView *m_pTransSettleView;
	WarriorDevelopView          *m_pDevelopView;
	WarriorDevelopSettleView    *m_pDevSettleView;
	RecruitSettleView           *m_pRecruitSettleView;
	PromotionView               *m_pPromotionView;
#pragma mark - - FormationDelegate
private:
    virtual void showFormationView();
    virtual void showChangeFormationView();
	virtual void showEquipSelectView(cocos2d::CCArray* pEquiList,formation::PositionInfo* pPos= NULL,uint32_t currEquipProperty=0);
	virtual void showEquipInfoView(uint32_t eid,formation::PositionInfo*);
	virtual void showEquipEnhanceView(uint32_t eid);
	virtual void showEquipRefineView(uint32_t eid);
	virtual void showFormationSelectView(formation::PositionInfo* pPos);
	virtual	void joinFormation(uint32_t wid,uint32_t posId);
#pragma mark EquipSelecteDelegate
private:
    virtual void selectedEquip(equip::EquipCurInfo*);

	WarriorViewType           m_eShowingType;
public:
	virtual void onBackKey();
};

#endif /* defined(__QSMY__Formation__) */
