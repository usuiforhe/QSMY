//
//  Account.h
//  账户系统(充值 vip特权)
//
//  Created by cll on 13-6-24.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ACCOUNT__
#define __QSMY__ACCOUNT__

#include "AppInclude.h"
#include "components/components.h"
#include "model/UserModel.h"
#include "model/PlatformModel.h"
#include "../inventory/InventoryDelegate.h"
#include "../inventory/ItemInfoView.h"

class RechargeView;
class VipView;
class RechargeFirstView;

//台币与元宝兑换率
#define EXCHANGE_POINT 1.5f

enum AccountType
{
    kAccountTypeNone,
    kAccountTypeRecharge,
    kAccountTypeVipView
};

class AccountDelegate:public InventoryDelegate
{
public:
    virtual void showInfo(AccountType) = 0;
    virtual void closeInfo()  = 0;
    virtual void recharge(platform::RechargeInfo*) = 0;
};


class Account
:public DLayer
,public AccountDelegate
{
    
public:
	Account();
	~Account();
	virtual bool init();
	CREATE_FUNC(Account);
	virtual void showInfo(AccountType);
private:
	virtual void showInfoView(InventoryType type,item::ItemInfo* pItem,cocos2d::CCObject* param = NULL);
    virtual void closeInfoView(InventoryType type);
private:
    
    virtual void recharge(platform::RechargeInfo*);
    virtual void closeInfo();
    virtual void updateView();
private:
	RechargeView         *m_pRechargeView;
    RechargeFirstView    *m_pRechargeFirstView;
    VipView                     *m_pVipView;
    UserModel                   *m_pUserModel;
    AccountType                 m_kAccountType;
    cocos2d::CCNode             *m_pContainer;
    cocos2d::CCNode             *m_pPopupLayer;
    ItemInfoView                *m_pItemInfo;
};



#endif /* defined(__QSMY__ACCOUNT__) */
