//
//  AwardItemSmall.cpp
//  
//
//  Created by cll on 13-7-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "AwardItemSmall.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "model/WarriorModel.h"
#include "model/EquipModel.h"
#include "model/ItemModel.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace drop;

AwardItemSmall::AwardItemSmall()
:m_pItemName(NULL)
,m_pItemRichName(NULL)
,m_pItemNum(NULL)
,m_pItemNumTTF(NULL)
,m_pGrade(NULL)
,m_pGradeNode(NULL)
,m_pGradeBg(NULL)
,m_pIconMenu(NULL)
,m_pIconSprite(NULL)
,m_pSoulMark(NULL)
,m_pPieceIcon(NULL)
,m_pDressMark(NULL)
,m_pAnimateRotate(NULL)
,m_pAnimateScale(NULL)
,m_pIcon(NULL)
,m_pGreenNode(NULL)
,m_pBlueNode(NULL)
,m_pPurpleNode(NULL)
,m_pOrangeNode(NULL)
,m_pGreenCircle(NULL)
,m_pBlueCircle(NULL)
,m_pPurpleCircle(NULL)
,m_pOrangeCircle(NULL)
{
    
}


AwardItemSmall::~AwardItemSmall()
{
	CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pGradeNode);
    CC_SAFE_RELEASE(m_pGradeBg);
	CC_SAFE_RELEASE(m_pItemName);
	CC_SAFE_RELEASE(m_pItemRichName);
    CC_SAFE_RELEASE(m_pItemNum);
    CC_SAFE_RELEASE(m_pItemNumTTF);
    CC_SAFE_RELEASE(m_pIconSprite);
	CC_SAFE_RELEASE(m_pIconMenu);
	CC_SAFE_RELEASE(m_pSoulMark);
	CC_SAFE_RELEASE(m_pPieceIcon);
    CC_SAFE_RELEASE(m_pDressMark);
	CC_SAFE_RELEASE(m_pAnimateRotate);
	CC_SAFE_RELEASE(m_pAnimateScale);
	CC_SAFE_RELEASE(m_pIcon);
	CC_SAFE_RELEASE(m_pGreenNode);
	CC_SAFE_RELEASE(m_pBlueNode);
	CC_SAFE_RELEASE(m_pPurpleNode);
	CC_SAFE_RELEASE(m_pOrangeNode);
	CC_SAFE_RELEASE(m_pGreenCircle);
	CC_SAFE_RELEASE(m_pBlueCircle);
	CC_SAFE_RELEASE(m_pPurpleCircle);
	CC_SAFE_RELEASE(m_pOrangeCircle);
}


bool AwardItemSmall::init()
{
	if(!CCNode::init())
	{
		return false;
	}
	m_pIcon = CCSprite::create();
	CC_SAFE_RETAIN(m_pIcon);
	return true;

}

bool AwardItemSmall::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", cocos2d::CCNodeRGBA *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeNode", WarriorHeadGradeBg*, this->m_pGradeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBg", WarriorGradeBg*, this->m_pGradeBg);
    
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconSprite", CCNode *, this->m_pIconSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconMenu", CCMenuItem *, this->m_pIconMenu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemRichName", CCRichLabelTTF *, this->m_pItemRichName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemName", CCLabelTTF *, this->m_pItemName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemNum", CCLabelBMFont *, this->m_pItemNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemNumTTF", CCLabelTTF*, this->m_pItemNumTTF);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoulMark", CCNode *, this->m_pSoulMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPieceIcon", CCNode *, this->m_pPieceIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDressMark",CCNode *, this->m_pDressMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateRotate", UIAnimate *, this->m_pAnimateRotate);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateScale", UIAnimate *, this->m_pAnimateScale);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGreenNode", CCNode *, this->m_pGreenNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBlueNode", CCNode *, this->m_pBlueNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPurpleNode", CCNode *, this->m_pPurpleNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOrangeNode", CCNode *, this->m_pOrangeNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGreenCircle", CCNode *, this->m_pGreenCircle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBlueCircle", CCNode *, this->m_pBlueCircle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPurpleCircle", CCNode *, this->m_pPurpleCircle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOrangeCircle", CCNode *, this->m_pOrangeCircle);

    return false;
}

SEL_MenuHandler AwardItemSmall::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onIconMenu", AwardItemSmall::onIconMenu);
	return NULL;
}

void AwardItemSmall::updateView(drop::Drop *pDropItem)
{
	
	if(m_pSoulMark) m_pSoulMark->setVisible(false);	
	if(m_pDressMark) m_pDressMark->setVisible(false);
	if(m_pPieceIcon) m_pPieceIcon->setVisible(false);
	
    m_pIconSprite->removeAllChildren();
    
	this->setVisible(pDropItem!=NULL);

	if (!pDropItem) return;

	//更新ICON
	
        
	CCSprite *pIcon = NULL;
    cocos2d::ccColor3B color;
    std::string name("");
    uint32_t num(0);
    bool showNum(false);
    bool showGrade(false);
	int itemGrade = 0;
    
    switch (pDropItem->getType()) {
        case drop::kDropTypeItem:
        case drop::kDropTypePkg:
        {
            showGrade = pDropItem->getItem()->getBaseInfo()->isNeedShowGradeColor();
            showNum = true;
            color = DM_GET_ITEM_MODEL->getItemColorByGrade(pDropItem->getItem()->getBaseInfo()->getItemGrade());
			itemGrade = (int)pDropItem->getItem()->getBaseInfo()->getItemGrade();
            name = pDropItem->getItem()->getBaseInfo()->getItemName();
            num = pDropItem->getItem()->getNum();

			if(pDropItem->getItem()->getBaseInfo()->getItemType() == item::kItemTypeFragment){
				//碎片
				if(m_pPieceIcon) m_pPieceIcon->setVisible(true);
				pIcon = ResourceManager::sharedResourceManager()->getEquipIcon50PForList(pDropItem->getItem()->getBaseInfo()->getResID());
			}
			else{
				pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(pDropItem->getItem()->getItemId());
			}
            
			//if(pIcon != NULL)m_pIconSprite->setScale(0.7f);
            break;
        }
        case drop::kDropTypeEquip:
        {
            showGrade = pDropItem->getEquip()->getBaseInfo()->isNeedShowGradeColor();
            showNum = true;
            color = DM_GET_EQUIP_MODEL->getEquipColorByGrade(pDropItem->getEquip()->getBaseInfo()->getEquipGrade());
			itemGrade = (int)pDropItem->getEquip()->getBaseInfo()->getEquipGrade();
            name = pDropItem->getEquip()->getBaseInfo()->getEquipName();
			num = 1;
			
            pIcon = ResourceManager::sharedResourceManager()->getEquipIcon40P(pDropItem->getEquip()->getEquipId());
			//if(pIcon != NULL)m_pIconSprite->setScale(0.7f);
            break;
        }
        case drop::kDropTypeSoul:
        {
            showGrade = true;
            showNum = true;
            color = DM_GET_WARRIOR_MODEL->getWarriorColorByGrade(pDropItem->getSoul()->getBaseInfo()->getRealGrade());
			itemGrade = (int)pDropItem->getSoul()->getBaseInfo()->getColorGrade();
            name = pDropItem->getSoul()->getBaseInfo()->getWarriorName();
            num = pDropItem->getSoul()->getNum();

			if (m_pSoulMark)
			{
				//转魂圆形头像
				m_pSoulMark->setVisible(true);
				pIcon =  ResourceManager::sharedResourceManager()->getWarriorIcon40PForDestiny(pDropItem->getSoul()->getWid());
				if (m_pGreenCircle) m_pGreenCircle->setVisible( itemGrade == 1 );
				if (m_pBlueCircle) m_pBlueCircle->setVisible( itemGrade == 2 );
				if (m_pPurpleCircle) m_pPurpleCircle->setVisible( itemGrade == 3 );
				if (m_pOrangeCircle) m_pOrangeCircle->setVisible( itemGrade == 4 );				
			}
			else
			{
				//方头像
				//pIcon =  ResourceManager::sharedResourceManager()->getWarriorIconForChangeFormation(pDropItem->getSoul()->getWid());
				pIcon =  ResourceManager::sharedResourceManager()->getWarriorHeadIcon(pDropItem->getSoul()->getWid());
			}
			
            break;
        }
        case drop::kDropTypeAvatar:
        {
            name = pDropItem->getAvatarInfo()->getBaseInfo()->getName();
			showNum = true;
            pIcon =  ResourceManager::sharedResourceManager()->getWarriorIconForAward(pDropItem->getAvatarInfo()->getBaseInfo()->getResId());
            //if(pIcon != NULL)m_pIconSprite->setScale(0.7f);
			m_pDressMark->setVisible(true);
            num = 1;
            break;
        }
        case drop::kDropTypeWarrior:
        {
            showGrade = true;
            showNum = true;
            color = DM_GET_WARRIOR_MODEL->getWarriorColorByGrade(pDropItem->getWarrior()->getBaseInfo()->getRealGrade());
            itemGrade = (int)pDropItem->getWarrior()->getBaseInfo()->getColorGrade();
			name = pDropItem->getWarrior()->getBaseInfo()->getWarriorName();
			num = 1;
			
            pIcon =  ResourceManager::sharedResourceManager()->getWarriorIcon40PForDestiny(pDropItem->getWarrior()->getWid());
            break;
        }
        case drop::kDropTypeCoin:
        {
            showGrade = false;
            itemGrade = 0;
            showNum = true;
            num = pDropItem->getCoin();
            item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(COIN_ID);
            name = pItemBase->getItemName();
            pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(COIN_ID);
			//if(pIcon != NULL)m_pIconSprite->setScale(0.7f);

            break;
        }
        case drop::kDropTypePoint:
        {
            showGrade = false;
            itemGrade = 0;
            showNum = true;
            num = pDropItem->getPoint();
            item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(POINT_ID);
            name = pItemBase->getItemName();
            pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(POINT_ID);
			//if(pIcon != NULL)m_pIconSprite->setScale(0.7f);

            break;
        }
        case drop::kDropTypeVip:
        {
            showGrade = false;
            showNum = false;
            
            user::VipInfo * pVipInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(pDropItem->getVip());
            item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(pVipInfo->getVipItemId());
            
            name = pItemBase->getItemName();
            pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(pItemBase->getItemId());
            //if(pIcon != NULL)m_pIconSprite->setScale(0.7f);

            break;
        }
        case drop::kDropTypePower:
        {
            showGrade = false;
            showNum = true;
            num = pDropItem->getPower();
            item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(POWER_ID);
            name = pItemBase->getItemName();
            pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(POWER_ID);
			//if(pIcon != NULL)m_pIconSprite->setScale(0.7f);

            break;
        }
        default:
        {
            return;
            break;
        }
    }

    if (showNum)
    {
        if (m_pItemNum)
        {
            m_pItemNum->setVisible(showNum);
            m_pItemNum->setString(CCString::createWithFormat("x%d",num)->getCString());
        }
        else if(m_pItemNumTTF)
        {
            m_pItemNumTTF->setVisible(showNum);
            m_pItemNumTTF->setString(CCString::createWithFormat("x%d",num)->getCString());
        }
    }
		
    if (m_pItemName)
    {
        m_pItemName->setString(name.c_str());
    }
	if (m_pItemRichName)
	{
		m_pItemRichName->setString(name.c_str());
	}
    
    if (m_pGradeBg && itemGrade >=0 && itemGrade <=4 ) {
        m_pGradeBg->setWarriorGrade((warrior::WarriorGrade)itemGrade);
    }
	
    if(m_pGradeNode && itemGrade >=1 && itemGrade <=4 )
    {
        m_pGradeNode->setWarriorGrade((warrior::WarriorGrade)itemGrade);
    }    
	else if(showGrade)
	{
		//CCLog(CCString::createWithFormat("Warrior %d Grade is %d ------------------------------------" , (int)pDropItem->getSoul()->getWid() , itemGrade)->getCString());
		if (m_pGreenNode) m_pGreenNode->setVisible( itemGrade == 1 );
		if (m_pBlueNode) m_pBlueNode->setVisible( itemGrade == 2 );
		if (m_pPurpleNode) m_pPurpleNode->setVisible( itemGrade == 3 );
		if (m_pOrangeNode) m_pOrangeNode->setVisible( itemGrade == 4 );
	}

	if (m_pAnimateRotate)
	{
		if (showGrade)
		{
			m_pAnimateRotate->setColor(color);
		}
		m_pAnimateRotate->playStill();
	}
	
	if(pIcon != NULL)
    {
        m_pIconSprite->addChild(pIcon);
		m_pIconSprite->setScale(0.7f);
		if(pDropItem->getType() == drop::kDropTypeSoul){
			m_pIconSprite->setScale(1.0f);
		}
    }

	if (m_pAnimateScale && pIcon)
	{
		pIcon->removeFromParent();
		m_pIcon->removeAllChildren();
		m_pIcon->addChild(pIcon);
		if (m_pIcon->getParent() == NULL)
		{
			m_pAnimateScale->replaceNodeInBone(m_pIcon, "itemIcon");
		}
		m_pAnimateScale->playStill();
	}
	
}

void AwardItemSmall::setRotateAnimationPlay(bool bPlay)
{
	if (!m_pAnimateRotate)
	{
		return;
	}
	if (bPlay)
	{
		m_pAnimateRotate->playAtIndex(0);
	}
	else
	{
		m_pAnimateRotate->playStill();
	}
}

void AwardItemSmall::setScaleAnimatePlay(bool bPlay)
{
	if (!m_pAnimateScale)
	{
		return;
	}
	if (bPlay)
	{
		m_pAnimateScale->playAtIndex(0);
	}
	else
	{
		m_pAnimateScale->playStill();
	}
}