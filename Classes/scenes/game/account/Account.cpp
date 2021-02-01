//
//  Accout.cpp
//  账户系统
//
//  Created by cll on 13-6-24.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Account.h"
#include "managers/GameManager.h"
#include "managers/PlatformManager.h"
#include "RechargeView.h"
#include "VipView.h"
#include "../Game.h"

USING_NS_CC;


Account::Account()
:m_pRechargeView(NULL)
,m_pVipView(NULL)
,m_pUserModel(NULL)
,m_pRechargeFirstView(NULL)
,m_kAccountType(kAccountTypeNone)
,m_pPopupLayer(NULL)
,m_pItemInfo(NULL)
,m_pContainer(NULL)
{
	
}

Account::~Account()
{
	CC_SAFE_RELEASE(m_pRechargeView);
    CC_SAFE_RELEASE(m_pVipView);
    CC_SAFE_RELEASE(m_pRechargeFirstView);
    UNBINDING_ALL(m_pUserModel, this);
    CC_SAFE_RELEASE(m_pUserModel);
    CC_SAFE_RELEASE(m_pPopupLayer);
    CC_SAFE_RELEASE(m_pItemInfo);
    CC_SAFE_RELEASE(m_pContainer);
}

bool Account::init()
{
	if(!DLayer::init())
	{
		return false;
	}
    m_pUserModel = DM_GET_USER_MODEL;
    CC_SAFE_RETAIN(m_pUserModel);
    if(!m_pUserModel->getUserInfo()->isPayed())
    {
        BINDING_EXEC(m_pUserModel, this, Account::updateView, D_EVENT_USERINFO_UPDATE);
    }
    
    m_pContainer = CCNode::create();
    addChild(m_pContainer);

	//popup自适应
	PL_MOVE_NODE_BELOW_CENTER_Y(m_pContainer);

    CC_SAFE_RETAIN(m_pContainer);
    m_pPopupLayer = CCNode::create();
    addChild(m_pPopupLayer);
    CC_SAFE_RETAIN(m_pPopupLayer);
    showInfo(kAccountTypeRecharge);
	return true;
}

void Account::updateView()
{
    if(m_kAccountType==kAccountTypeRecharge)
    {
        showInfo(m_kAccountType);
    }
}

void Account::showInfo(AccountType type)
{
    m_kAccountType = type;
    m_pContainer->removeAllChildrenWithCleanup(false);
    
    switch (type) {
        case kAccountTypeRecharge:
            if(m_pUserModel->getUserInfo()->isPayed())
            {
                if(m_pRechargeView==NULL)
                {
                    m_pRechargeView = RechargeView::create();
                    CC_SAFE_RETAIN(m_pRechargeView);
                    m_pRechargeView->setDelegate(this);
                }
                m_pContainer->addChild(m_pRechargeView);
            }else
            {
                if(m_pRechargeFirstView==NULL)
                {
                    m_pRechargeFirstView = RechargeFirstView::create();
                    CC_SAFE_RETAIN(m_pRechargeFirstView);
                    m_pRechargeFirstView->setDelegate(this);
                }
                m_pContainer->addChild(m_pRechargeFirstView);
            }
            break;
        case kAccountTypeVipView:
            if(m_pVipView==NULL)
            {
                m_pVipView = VipView::create();
                CC_SAFE_RETAIN(m_pVipView);
                m_pVipView->setDelegate(this);
            }
            m_pContainer->addChild(m_pVipView);
            break;
        default:
            break;
    }
}

void Account::closeInfo()
{
//    closePopup();
	Game::sharedGame()->NavigateTo(navigation::kNavHome);
}

void Account::recharge(platform::RechargeInfo* pInfo)
{
    DM_GET_PLATFORM_MODEL->setRechargeInfo(pInfo);
    DM_GET_PLATFORM_MODEL->getChargeOrder();
	//PlatformManager::sharedPlatformManager()->getChargeOrder();
    //HTTP_CLIENT->getChargeOrder();
}

void Account::closeInfoView(InventoryType type)
{
    m_pPopupLayer->removeAllChildren();
}

void Account::showInfoView(InventoryType type,item::ItemInfo* pItemInfo,cocos2d::CCObject* param)
{
    m_pPopupLayer->removeAllChildren();
	if(m_pItemInfo == NULL)
	{
		m_pItemInfo = ItemInfoView::create();
        m_pItemInfo->setDelegate(this);
		m_pItemInfo->retain();
	}
    
	m_pItemInfo->setItemInfo(pItemInfo);
	m_pPopupLayer->addChild(m_pItemInfo);
}

