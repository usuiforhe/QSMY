//
//  MallView.h
//  商品列表
//  Created by cll on 13-6-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MALLVIEW__
#define __QSMY__MALLVIEW__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/DataManager.h"

class MallDelegate;

class MallView
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
    
    CC_SYNTHESIZE(MallDelegate*, m_pDelegate, Delegate);
public:
	MallView();
	~MallView();
    
    static MallView* create(mall::ProductType pType,const cocos2d::CCSize& size);

    virtual bool init(mall::ProductType pType,const cocos2d::CCSize& size);

	virtual void onEnter();
	virtual void onExit();
    
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
private:
	void updateMallView();    
	void buyMenuCallback(CCObject * obj);
    void buyCallback(const Json::Value & requestData,const Json::Value &responseData);
    
private:
    cocos2d::extension::CCData  *m_pData;
    
private:
	mall::ProductType           m_pType;		//商品类型
    DTableView                  *m_pListView;
	MallModel::ProductInfoVec   m_products;
	user::UserInfo              *m_pUserInfo;
	bool m_bNoFly;

private:
    MallModel                   *m_pMallModel;
};




class MallViewCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{

    CC_SYNTHESIZE(MallDelegate*, m_pDelegate, Delegate);
    
    CC_SYNTHESIZE(cocos2d::CCNode*, m_pContainer, Container);
    CC_SYNTHESIZE(cocos2d::CCLabelTTF*, m_pName, Name);
    CC_SYNTHESIZE(cocos2d::CCLabelBMFont*, m_pAmount, Amount);
    CC_SYNTHESIZE(cocos2d::CCLabelTTF*, m_pDesc, Desc);
    CC_SYNTHESIZE(cocos2d::CCMenuItem*, m_pBuyMenuItem, BuyMenuItem);
    
    CC_SYNTHESIZE(cocos2d::CCNode*, m_pNoSale, NoSale);
    CC_SYNTHESIZE(cocos2d::CCLabelBMFont*, m_pPriceSingle, PriceSingle);
    
    CC_SYNTHESIZE(cocos2d::CCNode*, m_pOnSale, OnSale);
    
    CC_SYNTHESIZE(cocos2d::CCLabelBMFont*, m_pPrice, Price);
    CC_SYNTHESIZE(cocos2d::CCLabelBMFont*, m_pPriceNow, PriceNow);
    CC_SYNTHESIZE(cocos2d::CCLabelBMFont*, m_pNum, Num);
    CC_SYNTHESIZE(cocos2d::CCNode*, m_pIfTime, IfTime);

	CC_SYNTHESIZE(cocos2d::CCNode*, m_pPriceBeforeNode, PriceBeforeNode);
    CC_SYNTHESIZE(DButton*, m_pBuy, BuyButton);

	CC_SYNTHESIZE(cocos2d::CCNode*, m_pGradeBlue, GradeBlue);
	CC_SYNTHESIZE(cocos2d::CCNode*, m_pGradePurple, GradePurple);
	CC_SYNTHESIZE(cocos2d::CCNode*, m_pGradeOrange, GradeOrange);

public:
    
	MallViewCell();
    virtual ~MallViewCell();
    
	static MallViewCell* create(cocos2d::extension::CCData* pData);
    
	virtual bool init(cocos2d::extension::CCData* pData);
	
    void setProductInfo(mall::ProductInfo *pProductInfo);

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
private:
    
	void updateProductInfo();
    void viewCallBack(cocos2d::CCObject* obj);
private:
    mall::ProductInfo   *m_pProductInfo;
    UIAnimate* m_pLimiteBuy;
};



#endif /* defined(__QSMY__MALLVIEW__) */
