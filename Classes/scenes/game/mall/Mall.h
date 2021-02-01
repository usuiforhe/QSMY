//
//  Mall.h
//  市集/商城模块
//
//  Created by cll on 13-6-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MALL__
#define __QSMY__MALL__

#include "AppInclude.h"
#include "components/components.h"
#include "model/ItemModel.h"
#include "../inventory/InventoryDelegate.h"
#include "../inventory/ItemInfoView.h"
#include "../Game.h"
#include "../lottery/Lottery.h"

class MallDelegate
:public InventoryDelegate
{   
public:
	virtual void showAvatar(mall::ProductInfo  *pInfo){};
};

class Mall
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public MallDelegate
,public BackKeyInterface
,public SoulExchangeDelegate
,public DTabViewDelegate
{
    
public:
    
	Mall();
	~Mall();
	bool init(bool isLottery);
	//CREATE_FUNC(Mall);

	static Mall* create(bool isLottery = false);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
	virtual void onBackKey();
	virtual void addHead(DLayer* pChild);
	virtual void SelectTabAtIndex(DTabView *tabView, uint32_t idx);

	void showLotteryDing(float dt);
	virtual void onEnter();
	virtual void onExit();
	
private:	
	virtual void showAvatar(mall::ProductInfo  *pInfo);
	virtual void showInfoView(InventoryType type,item::ItemInfo* pItem,cocos2d::CCObject* param = NULL);
    virtual void closeInfoView(InventoryType type);
    
private:
    void rechargeCallBack(cocos2d::CCObject*);
    
private:
	
	DTabView            *m_pDTabView;
    cocos2d::CCNode     *m_pContainer;
    DButton             *m_pRecharge;
	cocos2d::CCMenuItem *m_pSellButton;
	cocos2d::CCMenuItem *m_pSellButtonFirst;
    cocos2d::CCNode		*m_pBottom;
	cocos2d::CCNode		*m_pTableNode;

	cocos2d::CCLabelBMFont		*m_pFreeCardLabel;
	cocos2d::CCNode                 *m_pFreeCardBg;

private:
	cocos2d::CCNode     *m_pSoulExchangeLayer;
    cocos2d::CCNode     *m_pPopupLayer;
    ItemInfoView        *m_pItemInfo;

	bool m_isLottery;
};



#endif /* defined(__QSMY__MALL__) */
