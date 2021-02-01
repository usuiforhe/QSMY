//
//  Burn.h
//  QSMY
//
//  Created by 汪燕军 on 13-8-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__BURN__
#define __QSMY__BURN__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../warrior/WarriorListView.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../common/common.h"
#include "BurnSelectView.h"

//#include "TreasureSelectView.h"

struct BurnWarriorFilter
	: public WarriorFilterDelegate {
		CREATE_FUNC(BurnWarriorFilter);
		bool init()
		{
			return true;
		}
		virtual ~BurnWarriorFilter(){}
		virtual bool operator()(warrior::WarriorCurInfo * pWarrior);
};

class Burn:
public DLayer,
public cocos2d::extension::CCBMemberVariableAssigner
,public BackKeyInterface
,public WarriorSelectDelegate
//,public TreasureSelectDelegate
{
public:
    Burn();
    
    virtual ~Burn();
    
    CREATE_FUNC(Burn);
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    void showWarriorSelectList();
	void showTreasureSelectList();

	virtual void setSelectedWarrior(warrior::WarriorCurInfo * pWarrior);
	virtual void setSelectedSoul(warrior::SoulInfo * pWarrior);
	virtual void setSelectedTreasure(equip::EquipCurInfo * pInfo);
	virtual void setSelectedWarriorMore(cocos2d::CCArray *pWarrior,uint8_t heroType);
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    //英雄炼化/宝物炼化之间切换
	void toggleViewByType(BurnModel::BurnType burnType = BurnModel::kBurnTypeHero);
	//设置按钮是否可以点击（屏蔽按钮触摸）
	void setMenuTouchEnable(bool val);
private:

	BurnModel::BurnType m_BurnType;

	//宝物炼化 英雄炼化切换
	void onToggleClick(cocos2d::CCObject * sender);
	//更新盒子
	void updateView();    
    //按键回调 炼化
    void onBurnClick(cocos2d::CCObject * sender);
    //按键回调 自动炼化
    void onAutoClick(cocos2d::CCObject * sender);
	//按键回调 说明
	void onBookClick(cocos2d::CCObject * sender);
	//ICON点击回调 英雄头像
	void onIconClick(cocos2d::CCObject * sender);
	//炼化完成后 重置界面
	void resetAfterBurn();
	//响应自动炼化
	void onAutoList();
	//动画回调
	void animateEnd(sp::Armature * target, const char* event,const char* data);
    //炼化确认
	void confirmBurn(MessageType type, DMessageBox* target);
	void onOpenRebirth(cocos2d::CCObject * sender);
private:
    
    cocos2d::CCNode * m_pAnimation;
    
    
    cocos2d::CCArray * m_pDrops;
    warrior::SoulInfo * m_pTargetSoul;
    uint32_t m_index;
    uint32_t m_remainActionNum;
	//WarriorSelectPopup	*m_pWarriorPopup;
	BurnSelectView      *m_pWarriorPopup;
	//TreasureSelectView	*m_pTreasurePopup;

	cocos2d::CCNode						*m_pCenterNode;
	cocos2d::CCNode						*m_pAvatorContainer;
	cocos2d::CCNode						*m_pHideNode;
	cocos2d::CCMenuItemImage	*m_pBook;
	cocos2d::CCNode						*m_pTxtHeroDetail;
	cocos2d::CCNode						*m_pTxtTreasureDetail;
	cocos2d::CCNode						*m_pHeroAwardList;
	cocos2d::CCNode						*m_pTreasureAwardList;
	cocos2d::CCNode						*m_pAwardNode;

	DButton * m_pBtnBurn;
	DButton * m_pBtnAutoBurn;
	DButton * m_pConfirmButton;

	ItemBox				*m_pIcons[TOTAL_BURN_NUM];
	ItemBox							*m_pBox[HERO_BURN_PREVIEW_COUNT];
	unsigned int					m_pItemIdList[HERO_BURN_PREVIEW_COUNT];
	ItemBox							*m_pBoxTreasureCoin;
	ItemBox							*m_pBoxTreasureItem;
	cocos2d::CCNode			*m_pTxtHero;
	cocos2d::CCNode			*m_pTxtTreasure;
	cocos2d::CCNode			*m_pTitleHero;
	cocos2d::CCNode			*m_pTitleTreasure;
	cocos2d::CCNode         *m_pRebirthTxt;
	cocos2d::CCMenuItemImage	*m_pMenuHero;
	cocos2d::CCMenuItemImage	*m_pMenuTreasure;
	cocos2d::CCMenuItemImage    *m_pRebirthBtn;
	uint32_t							m_uCurrentTag;

	BurnModel *m_pModel;

	sp::Armature            *m_pEffect;

public:
	virtual void onBackKey();
};

#endif /* defined(__QSMY__Burn__) */
