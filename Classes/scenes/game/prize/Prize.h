//
//  Prize.h
//  QSMY
//
//  Created by 汪燕军 on 13-9-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Prize__
#define __QSMY__Prize__

#include "AppInclude.h"
#include "components/components.h"
#include "model/PrizeModel.h"

#include "../warrior/WarriorDelegate.h"
#include "../equip/EquipDelegate.h"
#include "../warrior/WarriorInfo.h"
#include "../equip/EquipInfoView.h"
#include "../skill/SkillInfo.h"
#include "../inventory/InventoryDelegate.h"
#include "../inventory/ItemInfoView.h"
#include "../Game.h"


class PrizeDelegate:public WarriorDelegate,public EquipDelegate,public InventoryDelegate
{
public:
    virtual void closeInfoView(WarriorViewType type) = 0;
    virtual void closeInfoView(EquipViewType type) = 0;
	virtual void showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,cocos2d::CCObject* param = NULL) = 0;
    virtual void showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param = NULL) = 0;
    virtual void showInfoView(InventoryType type,item::ItemInfo* pItem,cocos2d::CCObject* param = NULL) = 0;
    virtual void closeInfoView(InventoryType type) = 0;

};

class Prize
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public cocos2d::extension::CCBMemberVariableAssigner
,public PrizeDelegate
,public BackKeyInterface
{
public:
    Prize()
    : DLayer()
    , m_pContainer(NULL)
    , m_pCloseButton(NULL)
    , m_pData(NULL)
    , m_pDTableView(NULL)
    , m_pPrizeList(NULL)
    , m_pInfoContainer(NULL)
    , m_pWarrior(NULL)
    , m_pSkillInfo(NULL)
    , m_pItemInfoView(NULL)
    {
    
    }
    
    ~Prize()
    {
        CC_SAFE_RELEASE(m_pContainer);
        CC_SAFE_RELEASE(m_pCloseButton);
        CC_SAFE_RELEASE(m_pPrizeList);
        CC_SAFE_RELEASE(m_pData);
        CC_SAFE_RELEASE(m_pInfoContainer);
        CC_SAFE_RELEASE(m_pWarrior);
        CC_SAFE_RELEASE(m_pSkillInfo);
        CC_SAFE_RELEASE(m_pItemInfoView);
    }
    
    CREATE_FUNC(Prize);
    
    virtual bool init();
    
	virtual void onEnter();
	virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
    
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell){
        CCLOG("tableCellTouched %d", cell->getIdx());
    };
    
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    virtual void closeInfoView(WarriorViewType type);
    virtual void closeInfoView(EquipViewType type);
	virtual void showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,cocos2d::CCObject* param = NULL);
    virtual void showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param = NULL);
    virtual void showInfoView(InventoryType type,item::ItemInfo* pItem,cocos2d::CCObject* param = NULL);
    virtual void closeInfoView(InventoryType type);
    
private:
    void updateView();
    
    void closeCallBack(cocos2d::CCObject * sender);
    
private:
    cocos2d::CCNode * m_pContainer;
    DButton * m_pCloseButton;
    cocos2d::extension::CCData * m_pData;
    
    DTableView * m_pDTableView;
    cocos2d::CCArray * m_pPrizeList;
    
    
    cocos2d::CCNode     *m_pInfoContainer;
    WarriorInfoView     *m_pWarrior;
    SkillInfoView       *m_pSkillInfo;
    ItemInfoView        *m_pItemInfoView;
public:
	virtual void onBackKey();
};

#endif /* defined(__QSMY__Prize__) */
