//
//  PrizeCell.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-9-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PrizeCell.h"
#include "model/DropModel.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "Prize.h"

USING_NS_CC;
USING_NS_CC_EXT;

PrizeCell * PrizeCell::create(cocos2d::extension::CCData *pData)
{
    PrizeCell * pRet = new PrizeCell;
    if(pRet != NULL && pRet->init(pData))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        pRet = NULL;
    }
    
    return pRet;
}

PrizeCell::PrizeCell()
: CCTableViewCell()
, m_pContainer(NULL)
, m_pBgGrade(NULL)
, m_pSoulMark(NULL)
, m_pDressMark(NULL)
, m_pDesc(NULL)
, m_pName(NULL)
, m_pAmountContainer(NULL)
, m_pAmount(NULL)
, m_pInProcess(NULL)
, m_pDrawContainer(NULL)
, m_pPrizeItem(NULL)
, m_pDelegate(NULL)
{
    
}

PrizeCell::~PrizeCell()
{
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pBgGrade);
    CC_SAFE_RELEASE(m_pSoulMark);
    CC_SAFE_RELEASE(m_pDressMark);
    CC_SAFE_RELEASE(m_pDesc);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pAmountContainer);
    CC_SAFE_RELEASE(m_pAmount);
    CC_SAFE_RELEASE(m_pInProcess);
    CC_SAFE_RELEASE(m_pDrawContainer);
    CC_SAFE_RELEASE(m_pPrizeItem);
}

bool PrizeCell::init(cocos2d::extension::CCData *pData)
{
    if(!CCTableViewCell::init())
    {
        return false;
    }
    
    CCB_READER_DATA_AND_ADDCHILD(pData);
    
    return true;
}

void PrizeCell::setPrizeItem(prize::PrizeItem *pPrizeItem)
{
    CC_SAFE_RELEASE(m_pPrizeItem);
    m_pPrizeItem = pPrizeItem;
    CC_SAFE_RETAIN(m_pPrizeItem);
    
    updateView();
}

bool PrizeCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode * , m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgGrade", CCSprite * , m_pBgGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoulMark", CCSprite * , m_pSoulMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDressMark", CCSprite * , m_pDressMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF * , m_pDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF * , m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmountContainer", CCNode * , m_pAmountContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmount", CCLabelBMFont * , m_pAmount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pInProcess", CCSprite * , m_pInProcess);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDrawContainer", CCNode * , m_pDrawContainer);
    
    return false;
}

cocos2d::SEL_MenuHandler PrizeCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "viewCallBack", PrizeCell::viewCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "drawCallBack", PrizeCell::drawCallBack);
    
    return NULL;
}

void PrizeCell::updateView()
{
    m_pSoulMark->setVisible(false);
    m_pDressMark->setVisible(false);
    m_pAmountContainer->setVisible(true);
    m_pContainer->removeAllChildren();
    m_pBgGrade->setVisible(false);
    switch (m_pPrizeItem->getDropType()) {
        case drop::kDropTypeCoin:
        {
            m_pAmountContainer->setVisible(true);
            
            m_pName->setString(D_LOCAL_STRING("Coin").c_str());
            m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(COIN_ID));
            break;
        }
        case drop::kDropTypePoint:
        {
            m_pAmountContainer->setVisible(true);
            
            m_pName->setString(D_LOCAL_STRING("Point").c_str());
            m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(POINT_ID));
            break;
        }
        case drop::kDropTypeVip:
        {
            user::VipInfo * pVipInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(m_pPrizeItem->getDropNum());
            item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(pVipInfo->getVipItemId());
            
            m_pName->setString(pItemBase->getItemName().c_str());
            m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(pItemBase->getItemId()));
            break;
        }
        case drop::kDropTypeEquip:
        {
           
            
            equip::EquipBaseInfo * pEquipBase = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(m_pPrizeItem->getDropId());
            
            if(pEquipBase->isNeedShowGradeColor())
            {
                m_pBgGrade->setVisible(true);
                ccColor3B color = DM_GET_EQUIP_MODEL->getEquipColorByGrade(pEquipBase->getEquipGrade());
                m_pBgGrade->setColor(color);

            }
            
            m_pName->setString(pEquipBase->getEquipName().c_str());
            m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon50PForList(pEquipBase->getEquipId()));
            
            break;
        }
        case drop::kDropTypeItem:
        case drop::kDropTypePkg:
        {
            m_pAmountContainer->setVisible(true);
            
            item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(m_pPrizeItem->getDropId());
            if(pItemBase->isNeedShowGradeColor())
            {
                m_pBgGrade->setVisible(true);
                ccColor3B color = DM_GET_ITEM_MODEL->getItemColorByGrade(pItemBase->getItemGrade());
                m_pBgGrade->setColor(color);
            }
            
            m_pName->setString(pItemBase->getItemName().c_str());
            m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(pItemBase->getItemId()));
            
            break;
        }
        case drop::kDropTypeSoul:
        {
            m_pSoulMark->setVisible(true);
            m_pAmountContainer->setVisible(true);
            m_pBgGrade->setVisible(true);
            
            warrior::WarriorBaseInfo * pWarriorBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pPrizeItem->getDropId());
            m_pName->setString(pWarriorBase->getWarriorName().c_str());
            ccColor3B color = DM_GET_WARRIOR_MODEL->getWarriorColorByWid(pWarriorBase->getWarriorId());
            m_pBgGrade->setColor(color);
            m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForActivityList(pWarriorBase->getWarriorId()));
            break;
        }
        case drop::kDropTypeAvatar:
        {
            m_pDressMark->setVisible(true);
            
            avatar::AvatarBaseInfo * pAvatarBaseInfo = DM_GET_AVATAR_MODEL->getAvatarBaseInfoById(m_pPrizeItem->getDropId());
            m_pName->setString(pAvatarBaseInfo->getName().c_str());
            m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForActivityList(pAvatarBaseInfo->getResId()));
            break;
        }
        case drop::kDropTypeWarrior:
        {
            m_pBgGrade->setVisible(true);
            
            warrior::WarriorBaseInfo * pWarriorBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pPrizeItem->getDropId());
            m_pName->setString(pWarriorBase->getWarriorName().c_str());
            ccColor3B color = DM_GET_WARRIOR_MODEL->getWarriorColorByWid(pWarriorBase->getWarriorId());
            m_pBgGrade->setColor(color);
            m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForActivityList(pWarriorBase->getWarriorId()));
            
            break;
        }
        default:
            return;
            break;
    }
    
    if(m_pPrizeItem->isQualified())
    {
        m_pDrawContainer->setVisible(true);
        m_pInProcess->setVisible(false);
    }
    else
    {
        m_pDrawContainer->setVisible(false);
        m_pInProcess->setVisible(true);
    }
    
    m_pAmount->setString(CCString::createWithFormat("%d",m_pPrizeItem->getDropNum())->getCString());
    m_pDesc->setString(m_pPrizeItem->getDesc().c_str());
}

void PrizeCell::viewCallBack(cocos2d::CCObject *sender)
{
    if(m_pDelegate==NULL) return;
    switch (m_pPrizeItem->getDropType()) {
        case drop::kDropTypeEquip:
        {
            m_pDelegate->showInfoView(kEquipInfo, equip::EquipCurInfo::create(m_pPrizeItem->getDropId()));
            break;
        }
        case drop::kDropTypeSoul:
        case drop::kDropTypeWarrior:
        {
            m_pDelegate->showInfoView(kWarriorInfo, warrior::WarriorCurInfo::create(m_pPrizeItem->getDropId()));
            break;
        }
        case drop::kDropTypeItem:
        case drop::kDropTypePkg:
        {
            m_pDelegate->showInfoView(kItemInfo, item::ItemInfo::create(m_pPrizeItem->getDropId(), m_pPrizeItem->getDropNum()));
            break;
        }
        case drop::kDropTypeCoin:
        case drop::kDropTypePoint:
        case drop::kDropTypeVip:
        default:
            break;
    }

}

void PrizeCell::drawCallBack(cocos2d::CCObject *sender)
{
    CCLOG("draw call back");
    
    switch (m_pPrizeItem->getCheckerType())
    {
        case prize::kPrizeItemCheckerLoginDays:
        {
            HTTP_CLIENT->activityTLoginAward(m_pPrizeItem->getThreshold());
            break;
        }
        case prize::kPrizeItemCheckerLevel:
        {
            HTTP_CLIENT->activityLvUpAward(m_pPrizeItem->getThreshold());
            break;
        }
        case prize::kPrizeItemCheckerNone:
        case prize::kPrizeItemCheckerTotalCharge:
        default:
            break;
            
    }    
}
