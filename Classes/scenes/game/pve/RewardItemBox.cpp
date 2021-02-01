//
//  RewardItemBox.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "RewardItemBox.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"
#include "../common/gui/WarriorHeadGradeBg.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace pve;

RewardItemBox::RewardItemBox()
:m_pGrade(NULL)
,m_pContainer(NULL)
,m_pTouchEvent(NULL)
,m_pName(NULL)
,m_pCount(NULL)
,m_pVipLevel(NULL)
,m_pVip(NULL)
,m_pCanGet(NULL)
,m_pGotten(NULL)
,m_pChapterInfo(NULL)
,m_nVipLevel(0)
{
}

RewardItemBox::~RewardItemBox()
{
    CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pTouchEvent);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pCount);
    CC_SAFE_RELEASE(m_pVipLevel);
    CC_SAFE_RELEASE(m_pVip);
    CC_SAFE_RELEASE(m_pCanGet);
    CC_SAFE_RELEASE(m_pGotten);
    CC_SAFE_RELEASE(m_pChapterInfo);
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

bool RewardItemBox::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade",  WarriorHeadGradeBg*, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer",  CCNode*, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchEvent",  CCMenuItem*, this->m_pTouchEvent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName",  CCLabelTTF*, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCount",  CCLabelTTF*, this->m_pCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipLevel",  CCLabelBMFont*, this->m_pVipLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVip",  CCNode*, this->m_pVip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCanGet",  CCNode*, this->m_pCanGet);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGotten",  CCNode*, this->m_pGotten);
    return false;
}

void RewardItemBox::updateStatus()
{
    if(m_pChapterInfo==NULL)
    {
        setVisible(false);
        return;
    }
    
    PrestigeReward* pInfo = m_pChapterInfo->getAwardByVipLv(m_nVipLevel);
    if(pInfo==NULL)
    {
        setVisible(false);
        return;
    }
    
    uint8_t userViplv = DM_GET_USER_MODEL->getUserInfo()->getVipLevel();
    
    if(m_pCount)
    {
        m_pCount->setString(CCString::createWithFormat("X %d",pInfo->prestige_reward_quantity)->getCString());
    }
    
    if(m_pVipLevel)
    {
        m_pVipLevel->setString(CCString::createWithFormat("%d",pInfo->prestige_reward_need_viplv)->getCString());
    }
    
    if(m_pVip)
    {
        m_pVip->setVisible(pInfo->prestige_reward_need_viplv>userViplv);
    }
    
    if(m_pCanGet)
    {
        m_pCanGet->setVisible(m_pChapterInfo->isPresAwardCanGet(m_nVipLevel));
        if(m_pCanGet->isVisible())
        {
            //m_pCanGet->removeAllChildren();
            if(m_pCanGet->getChildrenCount()==0) m_pCanGet->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("collect_get"));
        }
    }
    
    if(m_pGotten)
    {
        m_pGotten->setVisible(m_pChapterInfo->isPresAwardIsGotten(m_nVipLevel));
    }

}

void RewardItemBox::setInfo(pve::ChapterInfo* pChapterInfo,uint8_t vipLevel)
{
    CC_SAFE_RETAIN(pChapterInfo);
    CC_SAFE_RELEASE(m_pChapterInfo);
    m_pChapterInfo = pChapterInfo;
    m_nVipLevel = vipLevel;
    if(m_pChapterInfo==NULL)
    {
        setVisible(false);
        return;
    }
    m_pTouchEvent->setTag(m_pChapterInfo->getCid());
    PrestigeReward* pInfo = pChapterInfo->getAwardByVipLv(vipLevel);
    if(pInfo==NULL)
    {
        setVisible(false);
        return;
    }
    
    EquipModel* pEquipModel     = DM_GET_EQUIP_MODEL;
    ItemModel*  pItemModel      = DM_GET_ITEM_MODEL;
    WarriorModel* pWarriorModel = DM_GET_WARRIOR_MODEL;
    
    setVisible(true);
    switch (pInfo->prestige_reward_type)
    {
        case pve::kPveRewardTypeEquip:
        {
            equip::EquipBaseInfo* pItemInfo = pEquipModel->getEquipBaseInfoByID(pInfo->prestige_reward_id);
            if(m_pGrade)
            {
                m_pGrade->setVisible(true);
                //m_pGrade->setColor(pEquipModel->getEquipColorByGrade(pItemInfo->getEquipGrade()));
                m_pGrade->setWarriorGrade(warrior::WarriorGrade(pItemInfo->getEquipGrade()));
            }
            if(m_pContainer)
            {
                m_pContainer->removeAllChildren();
                m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon40P(pInfo->prestige_reward_id));
            }
            
            if(m_pName)
            {
                m_pName->setString(pItemInfo->getEquipName().c_str());
            }
       
            
            break;
        }
        case pve::kPveRewardTypeCoin:
		{
			uint32_t coinId = D_CONFIG_UINT_FOR_KEY(ITEM_COIN);

			if(m_pGrade) m_pGrade->setVisible(false);
			if(m_pContainer)
			{
				m_pContainer->removeAllChildren();
				m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon40P(coinId));
			}
			if(m_pName)
			{
				//m_pName->setString(pItemInfo->getItemName().c_str());
				m_pName->setString(D_LOCAL_STRING("$tongbi").c_str());
			}
			break;
		}
		case pve::kPveRewardTypePoint:
		{
			uint32_t pointId = D_CONFIG_UINT_FOR_KEY(ITEM_POINT);

			if(m_pGrade) m_pGrade->setVisible(false);
			if(m_pContainer)
			{
				m_pContainer->removeAllChildren();
				m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon40P(pointId));
			}
			if(m_pName)
			{
				//m_pName->setString(pItemInfo->getItemName().c_str());
				m_pName->setString(D_LOCAL_STRING("$yuanbao").c_str());
			}
			break;
		}
		case pve::kPveRewardTypeItem:
        {
            item::ItemBase* pItemInfo = pItemModel->getItemBaseByID(pInfo->prestige_reward_id);
            if(m_pGrade)
            {
                m_pGrade->setVisible(true);
                //m_pGrade->setColor(pEquipModel->getEquipColorByGrade(pItemInfo->getEquipGrade()));
                m_pGrade->setWarriorGrade(warrior::WarriorGrade(pItemInfo->getItemGrade()));
            }
            if(m_pContainer)
            {
                m_pContainer->removeAllChildren();
                m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon40P(pInfo->prestige_reward_id));
            }
            if(m_pName)
            {
                m_pName->setString(pItemInfo->getItemName().c_str());
            }
         
            break;
        }
        case pve::kPveRewardTypeWarrior:
        {
            warrior::WarriorBaseInfo* pItemInfo = pWarriorModel->getWarriorBaseByWID(pInfo->prestige_reward_id);
            if(m_pGrade)
            {
                m_pGrade->setVisible(true);
                //m_pGrade->setColor(pWarriorModel->getWarriorColorByWid(pInfo->prestige_reward_id));
                m_pGrade->setWarriorGrade(pItemInfo->getColorGrade());
            }
            if(m_pContainer)
            {
                m_pContainer->removeAllChildren();
                m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon40P(pInfo->prestige_reward_id));
            }
            if(m_pName)
            {
                m_pName->setString(pItemInfo->getWarriorName().c_str());
            }
            
            break;
        }
        case pve::kPveRewardTypeNone:
            
        default:
            setVisible(false);
            return;
    }
	m_pContainer->setScale(0.7f);
    updateStatus();
    NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(RewardItemBox::updateStatus), D_EVENT_USER_VIP_LEVELUP);
}

