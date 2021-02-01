//
//  RechargeItem.cpp
//  QSMY
//
//  Created by jiangke on 13-12-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "RechargeItem.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

RechargeItem::RechargeItem()
:m_pRechageButton(NULL)
,m_pDelegate(NULL)
,m_pNum(NULL)
,m_pInfoTTF(NULL)
,m_pAdditionalTf(NULL)
,m_pYuanbaoTF(NULL)
,m_pEveryDayYuanbaoTf(NULL)
,m_pInfo(NULL)
,m_pAdditionalIco(NULL)
,m_pMonthCard(NULL)
,m_PYuanbao(NULL)
,m_pRechargeInfo(NULL)
,m_pContainer(NULL)
{
    
}


RechargeItem::~RechargeItem()
{
    CC_SAFE_RELEASE(m_pRechageButton);
    CC_SAFE_RELEASE(m_PYuanbao);
    CC_SAFE_RELEASE(m_pMonthCard);
    CC_SAFE_RELEASE(m_pNum);
    CC_SAFE_RELEASE(m_pInfoTTF);
    CC_SAFE_RELEASE(m_pAdditionalTf);
    CC_SAFE_RELEASE(m_pYuanbaoTF);
    CC_SAFE_RELEASE(m_pEveryDayYuanbaoTf);
    CC_SAFE_RELEASE(m_pInfo);
    CC_SAFE_RELEASE(m_pAdditionalIco);
    CC_SAFE_RELEASE(m_pRechargeInfo);
    CC_SAFE_RELEASE(m_pContainer);
}




bool RechargeItem::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRechageButton", DButton *, this->m_pRechageButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_PYuanbao", CCNode *, this->m_PYuanbao);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMonthCard", CCNode *, this->m_pMonthCard);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNum", CCLabelBMFont *, this->m_pNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pInfoTTF", CCLabelTTF *, this->m_pInfoTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAdditionalTf", CCLabelBMFont *, this->m_pAdditionalTf);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pYuanbaoTF", CCLabelBMFont *, this->m_pYuanbaoTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEveryDayYuanbaoTf", CCLabelBMFont *, this->m_pEveryDayYuanbaoTf);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pInfo", CCNode *, this->m_pInfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAdditionalIco", CCNode *, this->m_pAdditionalIco);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    
    return false;
}




void RechargeItem::menuCallback(CCObject * obj)
{
	if(m_pDelegate) m_pDelegate->recharge(m_pRechargeInfo);
}



void RechargeItem::setInfoData(platform::RechargeInfo* info)
{
    if(m_pRechargeInfo == info && m_pRechargeInfo!=NULL)return;
    CC_SAFE_RELEASE(m_pRechargeInfo);
    CC_SAFE_RETAIN(info);
    m_pRechargeInfo = info;
    updateView();
}

void RechargeItem::reset()
{
    setVisible(false);
    
}

void RechargeItem::updateView(){
    if(m_pRechargeInfo==NULL)
    {
        reset();
        return;
    }
    setVisible(true);
    m_pContainer->removeAllChildren();
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getRechargeIcon(m_pRechargeInfo->getResId()));
    m_pRechageButton->getButton()->setTarget(this, menu_selector(RechargeItem::menuCallback));
 
    if(m_pRechargeInfo->isMonthCard())
    {
        m_pNum->setString(m_pRechargeInfo->getPrice().c_str());
    }else
    {
        m_pNum->setString(CCString::createWithFormat("%d",m_pRechargeInfo->getPoint())->getCString());
    }
    
    m_pInfoTTF->setString(D_LOCAL_STRING("%sYuan",m_pRechargeInfo->getPrice().c_str()).c_str());
    m_pYuanbaoTF->setString(CCString::createWithFormat("%d",m_pRechargeInfo->getPoint())->getCString());
    m_pAdditionalTf->setString(CCString::createWithFormat("%d%%",(int)m_pRechargeInfo->getExtRate())->getCString());
    m_pEveryDayYuanbaoTf->setString(CCString::createWithFormat("%d",(int)m_pRechargeInfo->getExtRate())->getCString());
    //m_pInfo->setString("");
    
    m_pInfo->setVisible(m_pRechargeInfo->isMonthCard());
    m_pYuanbaoTF->setVisible(m_pInfo->isVisible());
    m_pEveryDayYuanbaoTf->setVisible(m_pInfo->isVisible());
    m_pMonthCard->setVisible(m_pInfo->isVisible());
    
    m_pAdditionalIco->setVisible(!m_pInfo->isVisible()&& m_pRechargeInfo->getExtRate()>0);
    m_pAdditionalTf->setVisible(m_pAdditionalIco->isVisible());
    m_PYuanbao->setVisible(!m_pInfo->isVisible());
}

