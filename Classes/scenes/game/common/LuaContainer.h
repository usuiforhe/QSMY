//
//  LuaContainer.h
//  QSMY
//
//  Created by wanghejun on 13-11-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__LuaContainer__
#define __QSMY__LuaContainer__

#include "AppInclude.h"
#include "components/components.h"
#include "../warrior/WarriorDelegate.h"
#include "../equip/EquipDelegate.h"
#include "../warrior/WarriorInfo.h"
#include "../equip/EquipInfoView.h"
#include "../skill/SkillInfo.h"
#include "../inventory/InventoryDelegate.h"
#include "../inventory/ItemInfoView.h"
#include "../Game.h"

class LuaContainer
:public DLayer
,public WarriorDelegate
,public EquipDelegate
,public InventoryDelegate
,public BackKeyInterface
{
    
public:
	LuaContainer();
	virtual ~LuaContainer();
	virtual bool init();
    
    virtual void onEnter();
    
    virtual void runLua() = 0;
    
	void showSkill(warrior::WarriorCurInfo * pWarrior , cocos2d::CCObject* param);
    virtual void closeInfoView(WarriorViewType type);
    virtual void closeInfoView(EquipViewType type);
	virtual void showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,cocos2d::CCObject* param = NULL);
    virtual void showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param = NULL);
    virtual void showInfoView(InventoryType type,item::ItemInfo* pItem,cocos2d::CCObject* param = NULL);
    virtual void closeInfoView(InventoryType type);
    virtual void showInfoView(cocos2d::CCNode*,bool closeOtherInfo = true);
    virtual void closeInfoView();
    virtual void cacheMember(cocos2d::CCObject* pMember);
private:
    cocos2d::CCNode     *m_pInfoContainer;
    cocos2d::CCNode     *m_pContainer;
    ItemInfoView        *m_pItemInfoView;
    WarriorInfoView     *m_pWarrior;
    SkillInfoView       *m_pSkillInfo;
    cocos2d::CCArray    *m_pCacheList;
    
    bool                isFirstEnter;
    
public:
	virtual void onBackKey();
protected:
	bool onBackKeyForInfoView();
};

#endif /* defined(__QSMY__LuaContainer__) */
