//
//  LowBalance.h
//  账户余额不足弹出框
//
//  Created by cll on 13-6-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__LOWBALANCE__
#define __QSMY__LOWBALANCE__

#define BOX_COUNT 4

#include "AppInclude.h"
#include "model/MallModel.h"
#include "Account.h"
#include "../common/common.h"

class LowBalance
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    
    CC_SYNTHESIZE(AccountDelegate*, m_pDelegate, Delegate);
    
public:
	LowBalance();
	virtual ~LowBalance();
    
	CREATE_FUNC(LowBalance);
    
    virtual bool init();
    
    void setTips(const char* tips);

	//默认是元宝页，这里设置为VIP页
	void setVipNotEnough();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
protected:
    virtual void loadUI();
	void rechargeCallBack(cocos2d::CCObject * obj);
	void closeCallBack(cocos2d::CCObject *obj);
	void vipCallBack(cocos2d::CCObject *obj);
protected:
    cocos2d::CCLabelTTF *m_pTips;    //描述文字
    DButton             *m_pRecharge;//充值按钮
    DButton             *m_pClose;

	//首充界面
	cocos2d::CCNode*				m_pFirstRechargeNode;
	//四个box
	ItemBox*								m_pItemBox[BOX_COUNT];

	//VIP界面
	cocos2d::CCNode*				m_pVipNode;
	cocos2d::CCLabelBMFont*	m_pVipNow;
	cocos2d::CCLabelBMFont*	m_pVipNext;
	cocos2d::CCLabelBMFont*	m_pPoint;
	cocos2d::CCLabelBMFont*	m_pPercent;
	cocos2d::CCMenuItemImage*m_pVipMenuItem;		
	DProgressBar*						m_pProgressBar;
	cocos2d::CCNode*				m_pTVip;
	cocos2d::CCNode*				m_pTPoint;

    
};


class LowBalanceFirst
:public LowBalance
{
public:
    CREATE_FUNC(LowBalanceFirst);
    //D_INIT_DEFAULT(LowBalance);
protected:
    //virtual void loadUI();
	virtual bool init();
};



#endif /* defined(__QSMY__LOWBALANCE__) */
