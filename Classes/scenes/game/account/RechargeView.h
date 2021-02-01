//
//  RechargeView.h
//  充值列表
//
//  Created by cll on 13-6-24.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__RECHARGEVIEW__
#define __QSMY__RECHARGEVIEW__

#define VIP_BOX_MAX	5
#define RECHARGE_VIEW_FIRST_RECHARGE_BOX	4

#include "AppInclude.h"
#include "../common/common.h"
#include "model/UserModel.h"
#include "model/PlatformModel.h"
#include "RechargeItem.h"

class AccountDelegate;
class RechargeViewCell;

class RechargeView
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    
    CC_SYNTHESIZE(AccountDelegate*, m_pDelegate, Delegate);
public:
    
	RechargeView();
	virtual ~RechargeView();
    
	CREATE_FUNC(RechargeView);
    
    virtual bool init();
    
	virtual void onEnter();
	virtual void onExit();
    
    
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
	//显示首充奖励
	void showFirstRecharge();
	//充值后更新
	void updateAfterRecharge(CCObject *obj);
protected:
    virtual void loadUI();
	void updateRechargeView();
    void updateVipInfo();
    
	void closeCallBack(cocos2d::CCObject * obj);
    void viewVipCallBack(cocos2d::CCObject* obj);
private:
    DTableView  *m_pListView;
    UserModel   *m_pUserModel;
protected:
    cocos2d::CCNode                 *m_pContainer;

	cocos2d::CCLabelBMFont          *m_pVipLevelCurrent;
	DButton                         *m_pCloseButton;
	DButton                         *m_pViewVipButton;
	DProgressBar					*m_pPBVip;
	cocos2d::CCLabelTTF				*m_pVipTips;
	cocos2d::CCLabelBMFont			*m_pBMFNextVip;
	cocos2d::CCNode					*m_pNodeNextVip;
	cocos2d::CCNode					*m_pNodeMaxVip;
    cocos2d::extension::CCData      *m_pCellData;
	cocos2d::CCNode					*m_pCell;
    cocos2d::CCArray                *m_pRechargeInfos;
	cocos2d::CCLabelTTF      *m_pExp;

	cocos2d::CCNode					 *m_pExpInfoNode;
	cocos2d::CCLabelTTF			 *m_pExpCoin;
	cocos2d::CCLabelTTF			 *m_pExpLevel;

	cocos2d::CCNode					*m_pTitleNode;
	cocos2d::CCNode					*m_pNodeFirst;

	AwardItemSmall					*m_pBox[VIP_BOX_MAX];
	ItemBox								*m_pItemBox[RECHARGE_VIEW_FIRST_RECHARGE_BOX];
	
};



class RechargeFirstView
:public RechargeView
{
public:
    CREATE_FUNC(RechargeFirstView);
    D_INIT_DEFAULT(RechargeView);
protected:
    virtual void loadUI();
};

class RechargeViewCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE(AccountDelegate*, m_pDelegate, Delegate);
    
public:
    
	RechargeViewCell();
    virtual ~RechargeViewCell();
    static RechargeViewCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);
    
    void setRechargeInfo(platform::RechargeInfo* pInfo);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
private:
//    void menuCallback(CCObject * obj);
	void updateCellView();
private:
    cocos2d::CCLabelBMFont *m_pBMFPoint;
	cocos2d::CCLabelBMFont *m_pBMFAddition;
	cocos2d::CCLabelBMFont *m_pBMFPrice;
	DButton	*m_pDBtnCharge;

	cocos2d::CCNode *m_pMonthCard;
	cocos2d::CCNode *m_pNoMonthCard;
	cocos2d::CCLabelBMFont *m_pMonthCardPrice;
	cocos2d::CCNode *m_pIconContainer;

	cocos2d::CCLabelBMFont *m_pMCFirst;
	cocos2d::CCLabelBMFont *m_pMCDay;
	cocos2d::CCLabelBMFont *m_pMCTimes;

	void callBackCharge(CCObject *pObj);

	platform::RechargeInfo *m_pRechargeInfo;
};



#endif /* defined(__QSMY__RECHARGEVIEW__) */
