//
//  WishCell.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-8-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "WishCell.h"
#include "../../../managers/GameManager.h"
#include "../../../managers/ResourceManager.h"
#include "Wish.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark ------------------WishCell--------------------
bool WishCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", cocos2d::CCSprite *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLight", cocos2d::CCSprite *, this->m_pLight);
    
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeGreen", cocos2d::CCNode *, this->m_pNodeGreen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeBlue", cocos2d::CCNode *, this->m_pNodeBlue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodePurple", cocos2d::CCNode *, this->m_pNodePurple);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeOrange", cocos2d::CCNode *, this->m_pNodeOrange);

    return false;
}

void WishCell::setColorGrade(warrior::WarriorGrade grade)
{
    m_eColorGrade = grade;
    //m_pGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByColorGrade(grade));
}

#pragma mark ------------------WishCellVip--------------------

bool WishCellVip::isUnlocked()
{
    return m_pWishItem->isUnlocked();
}

bool WishCellVip::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    if(WishCell::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode))
    {
        return true;
    }
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHead", cocos2d::CCNode *, this->m_pHead);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLock", cocos2d::CCSprite *, this->m_pLock);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectWarrior", cocos2d::CCSprite *, this->m_pSelectWarrior);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipContainer", cocos2d::CCNode *, this->m_pVipContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipLevel", cocos2d::CCLabelBMFont *, this->m_pVipLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", cocos2d::CCMenuItem *, this->m_pButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", cocos2d::CCNode *, this->m_pEffectNode);
    
    return false;
}


void WishCellVip::setSelectedWarrior(warrior::WarriorCurInfo * pWarrior)
{
    if(pWarrior != NULL && pWarrior->getWid() != m_pWishItem->getWarriorId())
    {
        m_pWishItem->setWarriorId(pWarrior->getWid());
		DM_GET_WISH_MODEL->setWarriorChanged();
        updateView();
    }
}

void WishCellVip::updateView()
{
    m_pHead->removeAllChildren();
    
    if(m_pWishItem->isUnlocked())
    {
        m_pLock->setVisible(false);
        m_pVipContainer->setVisible(false);
        m_pButton->setVisible(true);
        m_pButton->setTarget(this, menu_selector(WishCellVip::onSelectWarriorClick));
        m_pSelectWarrior->setVisible(true);
        
        m_pEffectNode->removeAllChildren();
        if(m_pWishItem->getWarriorId() > 0)
        {
            warrior::WarriorCurInfo * pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(m_pWishItem->getWarriorId());
            if(pWarrior != NULL)
            {
                m_pHead->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon40PForDestiny(pWarrior->getAvatarId()));
                ccColor3B color = DM_GET_WARRIOR_MODEL->getWarriorColorByWid(pWarrior->getWid());
                //m_pGrade->setColor(color);
                m_pSelectWarrior->setVisible(false);
            }
            else
            {
                m_pWishItem->setWarriorId(0);
                m_pSelectWarrior->setVisible(true);
                m_pEffectNode->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("wishprompt"));
            }
        }
        else
        {
            m_pEffectNode->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("wishprompt"));
        }
    }
    else
    {
        m_pLock->setVisible(true);
        m_pVipContainer->setVisible(true);
        m_pSelectWarrior->setVisible(false);
        m_pButton->setVisible(true);
        m_pButton->setTarget(this, menu_selector(WishCellVip::unlockWarrior));
        
        m_pVipLevel->setString(CCString::createWithFormat("%d", m_pWishItem->getVipLevel())->getCString());
    }
}

void WishCellVip::unlockWarrior(cocos2d::CCObject * sender)
{
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV);
}

void WishCellVip::onSelectWarriorClick(cocos2d::CCObject * sender)
{
    if(m_pDelegate)
    {
        m_pDelegate->showWarriorSelectList(this);
    }
}
