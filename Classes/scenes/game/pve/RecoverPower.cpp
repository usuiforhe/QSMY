//
//  RecoverPower.cpp
//  
//
//  Created by wanghejun on 13-9-4.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "RecoverPower.h"
#include "Pve.h"
#include "managers/TipsManager.h"

USING_NS_CC;

static uint32_t MIN = 1;

RecoverPower::RecoverPower()
:m_Delegate(NULL)
,m_pTodayCurrent(NULL)
//,m_pVip(NULL)
,m_pTodayTotal0(NULL)
//,m_pTodayTotal1(NULL)
,m_pCount(NULL)
,m_pCost(NULL)
,m_pPower(NULL)
,m_pCloseButton(NULL)
,m_pCannelButton(NULL)
,m_pSubmitButton(NULL)
,m_pSubButton(NULL)
,m_pAddButton(NULL)
,m_pUserModel(NULL)
,m_pMallModel(NULL)
,m_nCount(MIN)
,m_pProductInfo(NULL)
,m_pUserInfo(NULL)
,m_pSubMinButton(NULL)
,m_pAddMaxButton(NULL)
//,m_pVipNode(NULL)
{
    
}

RecoverPower::~RecoverPower()
{
    CC_SAFE_RELEASE(m_pTodayCurrent);
    //CC_SAFE_RELEASE(m_pVip);
    CC_SAFE_RELEASE(m_pTodayTotal0);
    //CC_SAFE_RELEASE(m_pTodayTotal1);
    
    CC_SAFE_RELEASE(m_pCount);
    
    CC_SAFE_RELEASE(m_pCost);
    CC_SAFE_RELEASE(m_pPower);
    
    CC_SAFE_RELEASE(m_pCloseButton);
    CC_SAFE_RELEASE(m_pCannelButton);
    CC_SAFE_RELEASE(m_pSubmitButton);
    CC_SAFE_RELEASE(m_pSubButton);
    CC_SAFE_RELEASE(m_pAddButton);
    
    CC_SAFE_RELEASE(m_pSubMinButton);
    CC_SAFE_RELEASE(m_pAddMaxButton);
    
    //CC_SAFE_RELEASE(m_pVipNode);
    
}

bool RecoverPower::init()
{
    if(!CCLayer::init()) return false;
    //CCB_READER_AND_ADDCHILD("ui/ccb/pve/recoverPower");
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_tilibuzu/PL_tilibuzu_tankuang");
    m_pUserModel = DM_GET_USER_MODEL;
    m_pMallModel = DM_GET_MALL_MODEL;
    m_pProductInfo = m_pMallModel->getProductById(PRODUCT_POW_ID);
    m_pUserInfo = m_pUserModel->getUserInfo();
    m_pCannelButton->getButton()->setTarget(this, menu_selector(RecoverPower::closeCallBack));
    m_pCloseButton->getButton()->setTarget(this, menu_selector(RecoverPower::closeCallBack));
    m_pAddButton->getButton()->setTarget(this, menu_selector(RecoverPower::addCallBack));
    m_pAddMaxButton->getButton()->setTarget(this, menu_selector(RecoverPower::addMaxCallBack));
    m_pSubButton->getButton()->setTarget(this, menu_selector(RecoverPower::subCallBack));
    m_pSubmitButton->getButton()->setTarget(this, menu_selector(RecoverPower::submitCallBack));
    
    updateView();
    return true;
}

void RecoverPower::onEnter()
{
    m_nCount = MIN;
    CCLayer::onEnter();
    BINDING_EXEC(m_pUserInfo, this, RecoverPower::updateView, D_EVENT_USERINFO_UPDATE);
}

void RecoverPower::onExit()
{
    CCLayer::onExit();
    UNBINDING_ALL(m_pUserInfo, this);
}

bool RecoverPower::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTodayCurrent", CCLabelTTF *, this->m_pTodayCurrent);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVip", CCLabelBMFont *, this->m_pVip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTodayTotal0", CCLabelTTF *, this->m_pTodayTotal0);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTodayTotal1", CCLabelBMFont *, this->m_pTodayTotal1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCount", CCLabelBMFont *, this->m_pCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCost", CCLabelTTF *, this->m_pCost);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPower", CCLabelBMFont *, this->m_pPower);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton*, m_pCloseButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCannelButton", DButton *, this->m_pCannelButton);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmitButton", DButton *, this->m_pSubmitButton);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubButton", DButton *, this->m_pSubButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddButton", DButton *, this->m_pAddButton);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubMinButton", CCMenuItem *, this->m_pSubMinButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddMaxButton", DButton *, this->m_pAddMaxButton);
    
    
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipNode", CCNode *, this->m_pVipNode);
    
    
    return false;
}

SEL_MenuHandler RecoverPower::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", RecoverPower::closeCallBack);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "addCallBack", RecoverPower::addCallBack);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "subCallBack", RecoverPower::subCallBack);
//    
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "addMaxCallBack", RecoverPower::addMaxCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "subMinCallBack", RecoverPower::subMinCallBack);
    
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "submitCallBack", RecoverPower::submitCallBack);
    
    
    return NULL;
}

void RecoverPower::updateView()
{
    
    user::VipInfo* pVipInfo = m_pUserModel->getVipInfoByVipLevel(m_pUserInfo->getVipLevel());
    //user::VipInfo* nextLvVipInfo = m_pUserModel->getVipInfoByVipLevel(m_pUserInfo->getVipLevel()+1);
    
    
    m_pTodayCurrent->setString(CCString::createWithFormat("%d",m_pUserInfo->getPowTimes())->getCString());
    m_pTodayTotal0->setString(CCString::createWithFormat("%d",pVipInfo->getAddPowerCountMax())->getCString());
    
//    if(nextLvVipInfo)
//    {
//        m_pVip->setString(CCString::createWithFormat("%d",nextLvVipInfo->getVipLv())->getCString());
//        m_pTodayTotal1->setString(CCString::createWithFormat("%d",nextLvVipInfo->getAddPowerCountMax())->getCString());
//    }else
//    {
//        
//    }
    
    updateInfo();
    
    //m_pVipNode->setVisible(m_pUserInfo->getVipLevel()<m_pUserModel->getMaxVipLevel());
}

void RecoverPower::updateInfo()
{
    m_pCount->setString(CCString::createWithFormat("%d",m_nCount)->getCString());
    m_pCost->setString(CCString::createWithFormat("%d",m_pProductInfo->getPrice()*m_nCount)->getCString());
    m_pPower->setString(CCString::createWithFormat("%d",BUY_POWER_ONCE*m_nCount)->getCString());
    m_pSubButton->getButton()->setEnabled(m_nCount>MIN);
    m_pAddButton->getButton()->setEnabled(m_nCount<m_pUserInfo->getRemainBuyPowTimes());
    
    m_pSubMinButton->setEnabled(m_pSubButton->getButton()->isEnabled());
    m_pAddMaxButton->getButton()->setEnabled(m_pAddButton->getButton()->isEnabled());
    
    m_pSubmitButton->getButton()->setEnabled(!(m_nCount > m_pUserInfo->getRemainBuyPowTimes() && m_pUserInfo->getVipLevel()>=m_pUserModel->getMaxVipLevel()));
}

void RecoverPower::closeCallBack(cocos2d::CCObject*)
{
    if(m_Delegate) m_Delegate->closePopup();
    else removeFromParent();
}

void RecoverPower::addMaxCallBack(cocos2d::CCObject*)
{
    m_nCount = m_pUserInfo->getRemainBuyPowTimes();
    updateInfo();
}

void RecoverPower::subMinCallBack(cocos2d::CCObject*)
{
    m_nCount = MIN;
    updateInfo();
}

void RecoverPower::addCallBack(cocos2d::CCObject*)
{
    m_nCount++;
    m_nCount = MIN(m_nCount, m_pUserInfo->getRemainBuyPowTimes());
    updateInfo();
}

void RecoverPower::subCallBack(cocos2d::CCObject*)
{
    m_nCount--;
    m_nCount = MAX(m_nCount, MIN);
    updateInfo();
}

void RecoverPower::submitCallBack(cocos2d::CCObject*)
{
    if(m_nCount > m_pUserInfo->getRemainBuyPowTimes())
    {
        if(m_pUserInfo->getVipLevel()<m_pUserModel->getMaxVipLevel())
        {
            NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV);
        }
        return;
    }
    if(m_pProductInfo->getPrice() * m_nCount>m_pUserInfo->getPoints())
    {
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
        return;
    }
    HTTP_CLIENT->buyPow(m_nCount, this, callfuncJson_selector(RecoverPower::buyCallBack),true);
}

void RecoverPower::buyCallBack(const Json::Value &requestData, const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
    closeCallBack(NULL);
}


