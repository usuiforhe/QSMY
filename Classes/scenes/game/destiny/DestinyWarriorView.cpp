//
//  DestinyWarriorView.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DestinyWarriorView.h"
#include "managers/ResourceManager.h"
#include "network/ErrorCode.h"

USING_NS_CC;
USING_NS_CC_EXT;

DestinyWarriorView::DestinyWarriorView()
:m_pDestinyInfo(NULL)
,m_pGiveup(NULL)
,m_pSubmit(NULL)
,m_pWarriorIcon(NULL)
,m_pWarriorHead(NULL)
,m_pSoulHead(NULL)
,m_pDestinyModel(NULL)
,m_pWarriorModel(NULL)
,m_pFormationModel(NULL)
,m_pSlot(NULL)
,m_pCompleteCount(NULL)
,m_pNeedCount(NULL)
,m_bIsFull(false)
{
    
}
DestinyWarriorView::~DestinyWarriorView()
{
    CC_SAFE_RELEASE(m_pGiveup);
    CC_SAFE_RELEASE(m_pSubmit);
    CC_SAFE_RELEASE(m_pWarriorIcon);
    CC_SAFE_RELEASE(m_pWarriorHead);
    CC_SAFE_RELEASE(m_pSoulHead);
    CC_SAFE_RELEASE(m_pCompleteCount);
    CC_SAFE_RELEASE(m_pNeedCount);
    SAFE_RELEASE_UNBINDING_ALL(m_pDestinyInfo, this);
}

bool DestinyWarriorView::init()
{
    if(!DLayer::init()) return false;
    
    m_pDestinyModel = DM_GET_DESTINY_MODEL;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    m_pFormationModel = DM_GET_FORMATION_MODEL;
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("DestinyWarriorHead", DestinyWarriorHeadLoader::loader());
    pNodeLoaderLibrary->registerCCNodeLoader("DestinyWarriorIcon", DestinyWarriorIconLoader::loader());
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/destiny/destinyWarriorView", pNodeLoaderLibrary);
    
    m_pGiveup->setTarget(this, menu_selector(DestinyWarriorView::closeCallBack));
    m_pSubmit->setTarget(this, menu_selector(DestinyWarriorView::submitCallBack));
    m_pSubmit->setEnabled(false);
    return true;
}

void DestinyWarriorView::submitCallBack(cocos2d::CCObject* sender)
{
    
    Json::Value warriors = Json::arrayValue;
    Json::Value souls = Json::arrayValue;
    
    if(m_pWarriorHead->getData()->getAdded()>0)
    {
        Json::Value warrior;
        warriors.append(m_pWarriorHead->getData()->getID());
    }
    
    if(m_pSoulHead->getData()->getAdded()>0)
    {
        Json::Value soul;
        soul["wid"] = m_pSoulHead->getData()->getID();
        soul["num"] = m_pSoulHead->getData()->getAdded();
        souls.append(soul);
    }
    
    if(getDelegate()) getDelegate()->improveDestinys(m_pDestinyInfo->getID(), warriors, souls,this,callfuncJson_selector(DestinyWarriorView::improveCallBack));
}

void DestinyWarriorView::improveCallBack(const Json::Value &requestData, const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
    
    closeCallBack(NULL);
}

void DestinyWarriorView::setDestinyInfo(destiny::DestinyInfo* pDestinyInfo,uint8_t slotIndex)
{
    CC_SAFE_RETAIN(pDestinyInfo);
    SAFE_RELEASE_UNBINDING_ALL(m_pDestinyInfo, this);
    m_pDestinyInfo = pDestinyInfo;
    m_pSlot = m_pDestinyInfo->getDestinySlotByIndex(slotIndex);
}

bool DestinyWarriorView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGiveup", CCMenuItemImage *, this->m_pGiveup);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmit", CCMenuItemImage *, this->m_pSubmit);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorIcon", DestinyWarriorIcon *, this->m_pWarriorIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorHead", DestinyWarriorHead *, this->m_pWarriorHead);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoulHead", DestinyWarriorHead *, this->m_pSoulHead);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCompleteCount", CCLabelBMFont *, this->m_pCompleteCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNeedCount", CCLabelBMFont *, this->m_pNeedCount);
    return false;
}

cocos2d::SEL_MenuHandler DestinyWarriorView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", DestinyWarriorView::closeCallBack);
    return NULL;
}

void DestinyWarriorView::closeCallBack(cocos2d::CCObject* sender)
{
    m_bIsFull = false;
    if(getDelegate()) getDelegate()->showDestinyInfoView(m_pDestinyInfo);
}


void DestinyWarriorView::onEnter()
{
    DLayer::onEnter();
    
    CCAssert(m_pDestinyInfo, "");
    
    BINDING_ONLY(m_pWarriorModel, this,     DestinyWarriorView::updateCanCollectStatus, D_EVENT_WARRIORLIST_UPDATE);
    BINDING_ONLY(m_pWarriorModel, this,     DestinyWarriorView::updateCanCollectStatus, D_EVENT_SOULLIST_UPDATE);
    BINDING_ONLY(m_pFormationModel, this,   DestinyWarriorView::updateCanCollectStatus, D_EVENT_FORMATION_UPDATE);
    BINDING_EXEC(m_pDestinyInfo, this,      DestinyWarriorView::updateDestinyInfo,      D_EVENT_DESTINY_UPDATED);
    updateSlotInfo();   //定值
    updateCanCollectStatus();
    
    BINDING_ONLY(m_pWarriorHead->getData(), this, DestinyWarriorView::updateView, D_EVENT_DESTINYWARRIOR_ADD);
    BINDING_ONLY(m_pSoulHead->getData(), this, DestinyWarriorView::updateView, D_EVENT_DESTINYWARRIOR_ADD);
    updateView();
}

void DestinyWarriorView::updateView()
{
    warrior::WarriorGrowInfo* pInfo = m_pWarriorModel->getWarriorGrowInfoByGrade(m_pWarriorHead->getData()->getID());
    uint32_t n = m_pSoulHead->getData()->getAdded();
    n+= m_pWarriorHead->getData()->getAdded()*pInfo->getRctSoul();
    int sub = n+m_pSlot->getCurrentNum() - m_pSlot->getUpgradeNum();
    if(sub>0)
    {
        n-=sub;
        m_pSoulHead->getData()->fixAdd(m_pSoulHead->getData()->getAdded()-sub);
    }
    
    m_pCompleteCount->setString(CCString::createWithFormat("%d",n+m_pSlot->getCurrentNum())->getCString());
    m_pWarriorIcon->getHighLight()->setVisible(n+m_pSlot->getCurrentNum()>=m_pSlot->getUpgradeNum());
    m_pWarriorIcon->getFullEffect()->setVisible(m_pWarriorIcon->getHighLight()->isVisible());
    updateHighLightStatus();
    m_pSubmit->setEnabled(n>0);
}


void DestinyWarriorView::onExit()
{
    DLayer::onExit();
    UNBINDING_ALL(m_pWarriorModel, this);
    UNBINDING_ALL(m_pFormationModel, this);
    UNBINDING_ALL(m_pDestinyInfo, this);
    UNBINDING_ALL(m_pWarriorHead->getData(),this);
    UNBINDING_ALL(m_pSoulHead->getData(),this);
}

void DestinyWarriorView::updateSlotInfo()
{
    
    warrior::WarriorBaseInfo *pWInfo= m_pWarriorModel->getWarriorBaseByWID(m_pSlot->getWarriorID());
    
    m_pWarriorIcon->getGrade()->setColor(m_pWarriorModel->getWarriorColorByWid(m_pSlot->getWarriorID()));
    m_pWarriorIcon->getHeadContainer()->removeAllChildren();
    m_pWarriorIcon->getHeadContainer()->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon40PForDestiny(m_pSlot->getWarriorID()));
    m_pWarriorIcon->getMenuItem()->setEnabled(false);
    m_pWarriorIcon->getName()->setString(pWInfo->getWarriorName().c_str());
    
    
    DestinyWarriorData* pData  = DestinyWarriorData::create(m_pWarriorModel->getWarriorBaseByWID(m_pSlot->getWarriorID()));
    m_pWarriorHead->setData(pData);
    
    
    pData  = DestinyWarriorData::create(m_pWarriorModel->getWarriorBaseByWID(m_pSlot->getWarriorID()));
    pData->setIsWarrior(false);
    m_pSoulHead->setData(pData);
}

void DestinyWarriorView::updateCanCollectStatus()
{
    m_pWarriorHead->getData()->setCount(m_pWarriorModel->getWarriorCurInfoByWID(m_pSlot->getWarriorID())==NULL?0:1);
    warrior::SoulInfo* pSouInfo = m_pWarriorModel->getSoulInfoByWID(m_pSlot->getWarriorID());
    m_pSoulHead->getData()->setCount(pSouInfo==NULL?0:pSouInfo->getNum());
    m_pSoulHead->getData()->fixAdd(0);
}

void DestinyWarriorView::updateDestinyInfo()
{
    m_pWarriorIcon->getHighLight()->setVisible(m_pSlot->isSlotUnlock());
    m_pWarriorIcon->getFullEffect()->setVisible(m_pWarriorIcon->getHighLight()->isVisible());
    updateHighLightStatus();
    m_pCompleteCount->setString(CCString::createWithFormat("%d",m_pSlot->getCurrentNum())->getCString());
    m_pNeedCount->setString(CCString::createWithFormat("%d",m_pSlot->getUpgradeNum())->getCString());
}

void DestinyWarriorView::updateHighLightStatus()
{
    if(m_bIsFull) return;
    m_bIsFull = m_pWarriorIcon->getFullEffect()->isVisible();
    m_pWarriorIcon->getFullEffect()->removeAllChildren();
    if(m_bIsFull)
    {
        CCNode* pEffect = ResourceManager::sharedResourceManager()->getUIAnimate("destinyfull");
        pEffect->setScale(0.7f);
        m_pWarriorIcon->getFullEffect()->addChild(pEffect);
    }
}



