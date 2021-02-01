//
//  Inventory.h
//  背包模块
//
//  Created by cll on 13-4-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#ifndef __QSMY__Inventory__
#define __QSMY__Inventory__

#include "AppInclude.h"
#include "model/ItemModel.h"
#include "components/components.h"
#include "InventoryDelegate.h"
#include "../Game.h"

class SelectItemView;
class ItemInfoView;
class SelectItemViewCell;
class EquipInfoView;


/*背包*/
class Inventory
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public InventoryDelegate
,public BackKeyInterface
{

public:
	Inventory();
	~Inventory();
	virtual bool init();
	CREATE_FUNC(Inventory);
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual void showInfoView(InventoryType type,item::ItemInfo* pItem,cocos2d::CCObject* param = NULL);
    virtual void closeInfoView(InventoryType type);
	virtual void useItem(item::ItemInfo *pItem);
	//开箱子动画结束回调
	void onBoxEffectEnd(CCObject* obj);

	void showChangeNameView();
private:
	cocos2d::CCNode         *m_pContainer;
	ItemInfoView            *m_pItemInfo;
    cocos2d::CCNode         *m_pPopupContainer;
	cocos2d::CCMenuItemImage*m_pSellButton;
    cocos2d::CCNode			*m_pTableNode;

	cocos2d::CCNode				*m_pAvailableBg;
	cocos2d::CCLabelBMFont	*m_pAvailableLabel;
    
    DTabView                *m_pDTabView;
public:
	virtual void onBackKey();
	virtual void onEnter(); 
	virtual void onExit();
	void updateAvailable();
};


class SelectItemViewCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{

public:

	SelectItemViewCell();
	~SelectItemViewCell();

	static SelectItemViewCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);

	void setItemInfo(item::ItemInfo *pItemInfo);

	//保留父節點的指針
	void setTableView(SelectItemView *pSelectView)
	{
		m_pSelectView = pSelectView;
	}
   
	void setDelegate(InventoryDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
    
	//显示优化
    void adjust(bool last = false);	

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;};
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

private:
	void updateCellView();
	void setApplyEnabled(bool value);
	void viewCallBack(cocos2d::CCObject *sender);
    void useCallBack(cocos2d::CCObject *sender);
private:
	cocos2d::CCNode             *m_pInfoContainer;		
    cocos2d::CCNode             *m_pAvatorContainer;		/*icon*/
	cocos2d::CCLabelTTF         *m_pName;		/*名*/
	cocos2d::CCLabelTTF         *m_pDesc;		/*描述*/
	cocos2d::CCLabelBMFont      *m_pAmount;		/*数量*/
    cocos2d::CCMenuItem         *m_pApply;
    cocos2d::CCSprite           *m_pGradeBg;

	cocos2d::CCNode				*m_pGradeGreen;
	cocos2d::CCNode				*m_pGradeBlue;
	cocos2d::CCNode				*m_pGradePurple;
	cocos2d::CCNode				*m_pGradeOrange;
	cocos2d::CCNode				*m_pDaojuNode;
	cocos2d::CCNode				*m_pSuipianNode;
	DButton						*m_pButton;

	cocos2d::CCSprite           *m_pSpriteTen;
	cocos2d::CCSprite           *m_pOpen;
	cocos2d::CCSprite           *m_pSpriteUse;
	cocos2d::CCSprite           *m_pSpriteBuild;
	cocos2d::CCSprite           *m_pSpriteNotEnough;

private:
	item::ItemInfo      *m_pItemInfo;
	cocos2d::CCNode     *m_pUseFont;
    cocos2d::CCNode     *m_pUseCompoundFont;
    cocos2d::CCNode     *m_pUseTenFont;
	InventoryDelegate   *m_pDelegate;

	SelectItemView			*m_pSelectView;
};



class SelectItemView
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
public:
	SelectItemView();
	~SelectItemView();

	virtual bool init(uint32_t sheet,const cocos2d::CCSize& size);
	static SelectItemView* create(uint32_t sheet,const cocos2d::CCSize& size);

	virtual void onEnter(); 

	virtual void onExit();

	void setDelegate(InventoryDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

private:

	void updateItemList();
	
private:
	DTableView *m_pListView;
	InventoryDelegate *m_pDelegate;
	ItemModel * m_pItemModel;
    cocos2d::extension::CCData* m_pCCData;
    cocos2d::CCArray   m_ItemDataList;
    uint32_t m_uSheet;

public:
	bool m_bNoFly;
};



#endif /* defined(__QSMY__InventoryView__) */
