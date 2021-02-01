//
//  DestinyView.h
//  QSMY
//
//  Created by wanghejun on 13-7-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DestinyView__
#define __QSMY__DestinyView__

#include "AppInclude.h"
#include "Destiny.h"
#include "components/DPageView.h"
#include "components/DPageDot.h"
#include "model/DestinyModel.h"

class DestinyViewItem;
#pragma mark - -------------------------------------------------
#pragma mark - -------------------DestinyView-------------------
#pragma mark - -------------------------------------------------
class DestinyView
:public DLayer
,public DPageViewDataSource
,public cocos2d::extension::CCScrollViewDelegate
{
    
#pragma mark - - init etc.
    
    CC_SYNTHESIZE(DestinyDelegate*, m_pDelegate, Delegate);
public:
    DestinyView();
    ~DestinyView();
    bool init(destiny::DestinyRequireType type);
    static DestinyView* create(destiny::DestinyRequireType type);
    
    void onEnter();
    void onExit();
    
    virtual float cellSizeForTable(DPageView *view);
    
    virtual cocos2d::CCNode* cellAtIndex(DPageView *view, unsigned int idx);
    
    virtual uint32_t numberOfCellsInTableView(DPageView *view);
    
    
    void showItemPage(const uint32_t did);
    void setCurrentPage(const uint32_t page);
    
#pragma mark - - variable
private:
    DPageView *m_pPageView;
    DPageDot  *m_pPageDot;
    cocos2d::CCArray * m_pPages;
    DestinyModel *m_pDestinyModel;
    
    destiny::DestinyRequireType m_kRequireType;
    cocos2d::extension::CCData* m_pCCData;
    
    
#pragma mark - - CCScrollViewDelegate
private:
	void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
	void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};
    
#pragma mark - - methods private
private:
    void showDestiny();
    
};


#pragma mark - -------------------------------------------------
#pragma mark - -----------------DestinyViewItem-----------------
#pragma mark - -------------------------------------------------
class DestinyViewItem
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    
    CC_SYNTHESIZE_READONLY(destiny::DestinyInfo*, m_pDestinyInfo, DestinyInfo);
    CC_SYNTHESIZE(DestinyDelegate*, m_pDelegate, Delegate);
    
#pragma mark - - init etc.
public:
    DestinyViewItem();
    ~DestinyViewItem();
    bool init(cocos2d::extension::CCData*);
    static DestinyViewItem* create(cocos2d::extension::CCData*);
    
    void setDestinyInfo(destiny::DestinyInfo* pDestinyInfo);
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;};
    
    void onEnter();
    void onExit();
    void update(float dt);
#pragma mark - - variable
private:
    
    DestinyModel*m_pModel;
    
    cocos2d::CCNode         *m_pHighLight;      //高亮
    cocos2d::CCNode         *m_pCanBeCollected; //可收集
    cocos2d::CCSprite       *m_pRewarded;       //可领奖
    cocos2d::CCSprite       *m_pGrade;          //品质
    cocos2d::CCMenuItem     *m_pRewardMenuItem; //点击领取
    cocos2d::CCNode         *m_pRewardEffect;   //领取效果
    cocos2d::CCLabelBMFont  *m_pProperty;       //属性加成
    cocos2d::CCLabelBMFont  *m_pLevel;          //级别
    cocos2d::CCLabelBMFont  *m_pInterval;       //领奖计时
    cocos2d::CCNode         *m_pContainer;      //图像容器
    
private:
    void startInterval();
#pragma mark - - methods private
private:
    void updateView();
    void touchItem(CCObject *obj);
    void reward(CCObject*obj);
    
};

#endif /* defined(__QSMY__DestinyView__) */
