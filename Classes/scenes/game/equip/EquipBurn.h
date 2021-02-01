//
//  EquipBurn.h
//  QSMY
//
//  Created by 汪燕军 on 13-8-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__EQUIPBURN__
#define __QSMY__EQUIPBURN__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../warrior/WarriorListView.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../common/common.h"
#include "../burn/EquipBurnSelectView.h"

class EquipBurnCloseDelegate
{
public:
	virtual void onBurnClose() = 0;
};

struct BurnEquipFilter
	: public WarriorFilterDelegate {
		CREATE_FUNC(BurnEquipFilter);
		bool init()
		{
			return true;
		}
		virtual ~BurnEquipFilter(){}
		virtual bool operator()(warrior::WarriorCurInfo * pWarrior);
};

class EquipBurn:
public DLayer,
public cocos2d::extension::CCBMemberVariableAssigner
,public BackKeyInterface
,public EquipBurnDelegate
//,public WarriorSelectDelegate
{
public:
    EquipBurn();
    
    virtual ~EquipBurn();
    
    CREATE_FUNC(EquipBurn);
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    void showSelectList();

	virtual void onSelectConfirm();
	virtual void setSelectedEquip(equip::EquipCurInfo * pInfo);
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	//设置按钮是否可以点击（屏蔽按钮触摸）
	void setMenuTouchEnable(bool val);
private:

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
	//ICON刷新
	void onIconRefresh();
	//响应自动炼化
	void onAutoList();
	//动画回调
	void animateEnd(sp::Armature * target, const char* event,const char* data);
    //炼化确认
	void confirmBurn(MessageType type, DMessageBox* target);
	void onOpenShop(cocos2d::CCObject * sender);
	void onClose(cocos2d::CCObject * sender);
	void playShine();
private:
    cocos2d::CCNode * m_pAnimation;
    cocos2d::CCNode * m_pFlyNode;
    
    cocos2d::CCArray * m_pDrops;
    warrior::SoulInfo * m_pTargetSoul;
    uint32_t m_index;
    uint32_t m_remainActionNum;
	//WarriorSelectPopup	*m_pWarriorPopup;
	EquipBurnSelectView      *m_pSelectView;
	//TreasureSelectView	*m_pTreasurePopup;

	cocos2d::CCNode						*m_pCenterNode;
	cocos2d::CCNode						*m_pAvatorContainer;
	cocos2d::CCNode						*m_pHideNode;
	cocos2d::CCMenuItemImage	*m_pBook;
	cocos2d::CCNode						*m_pTreasureAwardList;
	cocos2d::CCNode						*m_pAwardNode;

	DButton * m_pCloseButton;
	DButton * m_pBtnBurn;
	DButton * m_pBtnAutoBurn;
	DButton * m_pConfirmButton;

	ItemBox				*m_pIcons[TOTAL_BURN_NUM];
	UIAnimate			*m_pShine[TOTAL_BURN_NUM];
	cocos2d::CCMenuItemImage    *m_pMenuShop;
	uint32_t							m_uCurrentTag;

	BurnModel *m_pModel;

	sp::Armature            *m_pEffect;
	sp::Armature            *m_pEffectFly;
	cocos2d::CCLabelBMFont	*m_pAmount;
	ItemBox					*m_pBoxCoin;
	ItemBox					*m_pBoxItem;

public:
	virtual void onBackKey();
	CC_SYNTHESIZE(EquipBurnCloseDelegate*, m_pDelegate, EquipBurnCloseDelegate);

};

#endif /* defined(__QSMY__EQUIPBURN__) */
