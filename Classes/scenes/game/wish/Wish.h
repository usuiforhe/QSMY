//
//  Wish.h
//  QSMY
//
//  Created by 汪燕军 on 13-8-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Wish__
#define __QSMY__Wish__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/ItemModel.h"
#include "../../../model/WishModel.h"
#include "../warrior/WarriorListView.h"
#include "DragonBone.h"
#include "../Game.h"

#include "../common/gui/WarriorGradeBg.h"

struct WishWarriorFilter
: public WarriorFilterDelegate {
    CREATE_FUNC(WishWarriorFilter);
    
    bool init()
    {
        return true;
    }
    
    virtual ~WishWarriorFilter(){}
    
    virtual bool operator()(warrior::WarriorCurInfo * pWarrior);
};

/**
 *	@brief	许愿代理
 */
struct WishDelegate
{
    /**
     *	@brief	显示弟子选择列表
     */
    virtual void showWarriorSelectList(WarriorSelectDelegate * pDelegate) = 0;
};

class WishCell;

class Wish:
public DLayer,
public WishDelegate,
public cocos2d::extension::CCBMemberVariableAssigner
,public BackKeyInterface
{
public:
    Wish():
	m_pPopup(NULL),
    m_pLaternNum(NULL),
    m_pLightCost(NULL),
    m_pWishButton(NULL),
    m_pWishTenButton(NULL),
    m_pAnimation(NULL),
    m_pActionDelegate(NULL),
    m_pItem(NULL),
    m_pWishItemCellMap(NULL),
    m_pDrops(NULL),
    m_pTargetSoul(NULL),
	m_pWarriorGradeBg(NULL),
	m_pCenterNode(NULL),
	m_pAvatorContainer(NULL),
    m_index(6),
    m_remainActionNum(0)
    {
        for(uint8_t i(0); i < TOTAL_WISH_NUM; ++i)
        {
            m_pWishCells[i] = NULL;
        }
    }
    
    virtual ~Wish();
    
    CREATE_FUNC(Wish);
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual void showWarriorSelectList(WarriorSelectDelegate * pDelegate);
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
private:
    void updateView();
    
    void showWishRes();
    
    void showMultiRes();
    
    void showSingleRes();
    
    void showAnimation(CCObject * pObj);
    
    void animateEnd(sp::Armature * target, const char* event,const char* data);
    
    void showDrop();

	void showOneDrop();
    
    void nextLight();
    
    void onWishClick(cocos2d::CCObject * sender);
    
    void onWishTenClick(cocos2d::CCObject * sender);
    
    warrior::WarriorGrade getGrade(uint32_t index);
    
private:
    cocos2d::CCLabelBMFont * m_pLaternNum;
    cocos2d::CCLabelBMFont * m_pLightCost;
    DButton * m_pWishButton;
    DButton * m_pWishTenButton;
    cocos2d::CCNode * m_pAnimation;
    WishCell * m_pWishCells[TOTAL_WISH_NUM];
    
    cocos2d::CCSprite * m_pActionDelegate;
    
    item::ItemInfo * m_pItem;   //天灯道具
    
    cocos2d::CCDictionary * m_pWishItemCellMap;                    //可设置的许愿格子列表
    
    cocos2d::CCArray * m_pDrops;
    warrior::SoulInfo * m_pTargetSoul;
    uint32_t m_index;
    uint32_t m_remainActionNum;
	WarriorSelectPopup * m_pPopup;

	WarriorGradeBg				*m_pWarriorGradeBg;
	cocos2d::CCNode				*m_pCenterNode;
	cocos2d::CCNode				*m_pAvatorContainer;

public:
	virtual void onBackKey();
};

#endif /* defined(__QSMY__Wish__) */
