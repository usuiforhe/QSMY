//
//  Destiny.h
//  天命
//
//  Created by wanghejun on 13-7-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Destiny__
#define __QSMY__Destiny__

#include "AppInclude.h"
#include "json/json.h"
#include "components/components.h"
#include "managers/DataManager.h"
#include  "network/HTTP_API.h"
#include "../inventory/InventoryDelegate.h"
#include "../inventory/ItemInfoView.h"
#include "../equip/EquipDelegate.h"
#include "../equip/EquipInfoView.h"
#include "../Game.h"

#pragma mark - -------------------------------------------------
#pragma mark - -----------------DestinyDelegate-----------------
#pragma mark - -------------------------------------------------
class DestinyDelegate
:public InventoryDelegate
,public EquipDelegate
{
public:
    virtual void showInfoView(InventoryType type,item::ItemInfo* pItem,cocos2d::CCObject* param = NULL) = 0;
    virtual void closeInfoView(InventoryType type) = 0;
    virtual void closeInfoView(EquipViewType type) = 0;
    virtual void showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param = NULL) = 0;

    virtual void showDestinyInfoView(destiny::DestinyInfo*) = 0;
    virtual void showDestinyWarriorView(destiny::DestinyInfo*, uint8_t slotIndex) = 0;
    virtual void reward(uint32_t id) = 0;
    virtual void improveDestinys(uint32_t did, Json::Value &warriors , Json::Value &souls,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL) =0;
};

class DestinyView;
class DestinyInfoView;
class DestinyWarriorView;
#pragma mark - -------------------------------------------------
#pragma mark - ---------------------Destiny---------------------
#pragma mark - -------------------------------------------------
class Destiny
:public DLayer
,public DestinyDelegate
,public BackKeyInterface
{
#pragma mark - - init etc.
public:
    Destiny();
    ~Destiny();
    virtual bool init();
    CREATE_FUNC(Destiny);
    
    virtual void onEnter();
    virtual void onExit();
    
#pragma mark - - DestinyDelegate    
private:
    virtual void showInfoView(InventoryType type,item::ItemInfo* pItem,cocos2d::CCObject* param = NULL);
    virtual void closeInfoView(InventoryType type);
    virtual void closeInfoView(EquipViewType type);
    virtual void showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param = NULL);
    
    //virtual void showDestinyView(const uint32_t &did);
    virtual void showDestinyInfoView(destiny::DestinyInfo*);
    virtual void showDestinyWarriorView(destiny::DestinyInfo*, uint8_t slotIndex);
    virtual void reward(uint32_t id);
    virtual void improveDestinys(uint32_t did, Json::Value &warriors , Json::Value &souls,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL);
    //void updateView();
    void updateCanCollectStatus();
    
    void updateSoulList();
#pragma mark - - variable
private:
    DestinyModel            *m_pDestinyModel;
    WarriorModel            *m_pWarriorModel;
    FormationModel          *m_pFormationModel;
private:
    ItemInfoView            *m_pItemInfo;           
    CCNode                  *m_pInfoContainer;
    CCNode                  *m_pContainer;
    DestinyInfoView         *m_pDestinyInfoViewNoAward;         //没奖励
    DestinyInfoView         *m_pDestinyInfoViewAward;           //有奖励
    DestinyInfoView         *m_pDestinyInfoViewAny;             // 随意插
    DestinyWarriorView      *m_pDestinyWarriorView;
    DTabView                *m_pDTabView;

public:
	virtual void onBackKey();
};

#endif /* defined(__QSMY__Destiny__) */
