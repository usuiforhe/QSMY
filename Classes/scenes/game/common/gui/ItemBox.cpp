//
//  ItemBox.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ItemBox.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "model/WarriorModel.h"
#include "model/EquipModel.h"
#include "model/ItemModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

ItemBox::ItemBox()
:m_pGrade(NULL)
,m_pContainer(NULL)
,m_pTouchEvent(NULL)
,m_pName(NULL)
,m_pCount(NULL)
,m_pNameBg(NULL)
,m_pPieceIcon(NULL)
,m_pGradeBg(NULL)
,m_pHunpo(NULL)
,m_pAmountNode(NULL)
,m_pAddBurn(NULL)
,m_pEmpty(NULL)
,m_bCount(0)
,m_bHeroType(0)
{
    
}

ItemBox::~ItemBox()
{
    CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pTouchEvent);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pCount);
    CC_SAFE_RELEASE(m_pNameBg);
	CC_SAFE_RELEASE(m_pPieceIcon);
	CC_SAFE_RELEASE(m_pGradeBg);
	CC_SAFE_RELEASE(m_pHunpo);
	CC_SAFE_RELEASE(m_pAmountNode);
	CC_SAFE_RELEASE(m_pAddBurn);
	CC_SAFE_RELEASE(m_pEmpty);
}


bool ItemBox::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade",  CCNodeRGBA*, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer",  CCNode*, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchEvent",  CCMenuItem*, this->m_pTouchEvent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName",  CCLabelTTF*, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCount",  CCLabelTTF*, this->m_pCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameBg",  CCNode*, this->m_pNameBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPieceIcon",  CCNode*, this->m_pPieceIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBg" ,  WarriorHeadGradeBg*,this->m_pGradeBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHunpo" ,  CCNode*,this->m_pHunpo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmountNode", CCNode*, this->m_pAmountNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddBurn", CCNode *, this->m_pAddBurn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEmpty", CCNode *, this->m_pEmpty);
    return false;
}

void ItemBox::updateTombItem(drop::Drop *pDropItem , int count)
{
	updateDropItem(pDropItem);
	if(count != -1) m_bCount = count;
	m_pCount->setString(D_CSTRING_WITH10K_FROM_UINT(m_bCount));

	//必然显示
	setVisible(true);
	bool isEmpty = (pDropItem == NULL);
	if(m_pAmountNode)m_pAmountNode->setVisible(!isEmpty);
	if(m_pName)m_pName->setVisible(!isEmpty);
	if(m_pEmpty)m_pEmpty->setVisible(isEmpty);
}

void ItemBox::updateBurnItem(drop::Drop *pDropItem)
{
	updateDropItem(pDropItem);
	m_bBurnData = static_cast<bool>(pDropItem !=NULL);
	if(!m_bBurnData) m_bHeroType = 0;
	//隐藏
	if(m_pAmountNode) m_pAmountNode->setVisible(false);
	if(m_pName) m_pName->setVisible(false);
	//处理NULL
	m_pAddBurn->setVisible(pDropItem == NULL);
	this->setVisible(true);
	if(pDropItem == NULL){
		if(m_pGradeBg) m_pGradeBg->setWarriorGrade(warrior::kWarGradeGreen);
	}
	else
	{
		//魂魄显示消耗数量
		if(pDropItem->getType() == drop::kDropTypeSoul)
		{
			if(m_pAmountNode) m_pAmountNode->setVisible(true);
			warrior::WarriorGrowInfo * pGrowInfo = DM_GET_WARRIOR_MODEL->getWarriorGrowInfoByGrade(pDropItem->getSoul()->getBaseInfo()->getRealGrade());
			m_pCount->setString(CCString::createWithFormat("x%d",pGrowInfo->getRctSoul())->getCString());
		}
	}
}
void ItemBox::updateDropItem(drop::Drop *pDropItem)
{
	if(m_pPieceIcon) m_pPieceIcon->setVisible(false);
	if(m_pHunpo) m_pHunpo->setVisible(false);

	this->setVisible(pDropItem!= NULL);
	if(m_pContainer) m_pContainer->removeAllChildren();
	if(!pDropItem) return;

	CCSprite *pIcon = NULL;
	//cocos2d::ccColor3B color;
	std::string name("");
	uint32_t num(0);
	bool showNum(false);
	//bool showGrade(false);
	int itemGrade = 0;

	m_bCount = 0;//置位

	switch (pDropItem->getType()) {
	case drop::kDropTypeItem:
	case drop::kDropTypePkg:
		{
			//showGrade = pDropItem->getItem()->getBaseInfo()->isNeedShowGradeColor();
			showNum = true;
			//color = DM_GET_ITEM_MODEL->getItemColorByGrade(pDropItem->getItem()->getBaseInfo()->getItemGrade());
			itemGrade = (int)pDropItem->getItem()->getBaseInfo()->getItemGrade();
			name = pDropItem->getItem()->getBaseInfo()->getItemName();
			num = pDropItem->getItem()->getNum();

			if(pDropItem->getItem()->getBaseInfo()->getItemType() == item::kItemTypeFragment){
				//碎片
				if(m_pPieceIcon) m_pPieceIcon->setVisible(true);
				pIcon = ResourceManager::sharedResourceManager()->getEquipIcon30P(pDropItem->getItem()->getBaseInfo()->getResID());
			}
			else{
				pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(pDropItem->getItem()->getItemId());
			}
			break;
		}
	case drop::kDropTypeEquip:
		{
			//showGrade = pDropItem->getEquip()->getBaseInfo()->isNeedShowGradeColor();
			showNum = true;
			//color = DM_GET_EQUIP_MODEL->getEquipColorByGrade(pDropItem->getEquip()->getBaseInfo()->getEquipGrade());
			itemGrade = (int)pDropItem->getEquip()->getBaseInfo()->getEquipGrade();
			name = pDropItem->getEquip()->getBaseInfo()->getEquipName();
			num = 1;

			pIcon = ResourceManager::sharedResourceManager()->getEquipIcon40P(pDropItem->getEquip()->getEquipId());
			//if(pIcon != NULL)m_pIconSprite->setScale(0.7f);
			break;
		}
	case drop::kDropTypeSoul:
		{
			//showGrade = true;
			showNum = true;
			//color = DM_GET_WARRIOR_MODEL->getWarriorColorByGrade(pDropItem->getSoul()->getBaseInfo()->getRealGrade());
			itemGrade = (int)pDropItem->getSoul()->getBaseInfo()->getColorGrade();
			name = pDropItem->getSoul()->getBaseInfo()->getWarriorName();
			num = pDropItem->getSoul()->getNum();
			if(m_pHunpo) m_pHunpo->setVisible(true);
			pIcon =  ResourceManager::sharedResourceManager()->getWarriorHeadIcon(pDropItem->getSoul()->getWid());
			break;
		}
	case drop::kDropTypeAvatar:
		{
			name = pDropItem->getAvatarInfo()->getBaseInfo()->getName();
			showNum = true;
			pIcon =  ResourceManager::sharedResourceManager()->getWarriorIconForAward(pDropItem->getAvatarInfo()->getBaseInfo()->getResId());
			num = 1;
			break;
		}
	case drop::kDropTypeWarrior:
		{
			//showGrade = true;
			showNum = true;
			//color = DM_GET_WARRIOR_MODEL->getWarriorColorByGrade(pDropItem->getWarrior()->getBaseInfo()->getRealGrade());
			itemGrade = (int)pDropItem->getWarrior()->getBaseInfo()->getColorGrade();
			name = pDropItem->getWarrior()->getBaseInfo()->getWarriorName();
			num = 1;

			pIcon =  ResourceManager::sharedResourceManager()->getWarriorIcon40PForDestiny(pDropItem->getWarrior()->getWid());
			break;
		}
	case drop::kDropTypeCoin:
		{
			//showGrade = false;
			showNum = true;
			num = pDropItem->getCoin();
			item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(COIN_ID);
			name = pItemBase->getItemName();
			pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(COIN_ID);

			break;
		}
	case drop::kDropTypePoint:
		{
			//showGrade = false;
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
			//showGrade = false;
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
			//showGrade = false;
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

	m_bCount = num;//赋值
	if (m_pCount)
	{
		m_pCount->setVisible(showNum);
		if(showNum)
		{
			m_pCount->setString(CCString::createWithFormat("x%d",num)->getCString());
		}
	}

	if (m_pName)
	{
		m_pName->setString(name.c_str());
	}
	/*if (m_pItemRichName)
	{
		m_pItemRichName->setString(name.c_str());
	}*/

	if (m_pGradeBg && itemGrade >=1 && itemGrade <=4 ) {
		m_pGradeBg->setWarriorGrade((warrior::WarriorGrade)itemGrade);
	}

	if(pIcon != NULL)
	{
		if(m_pContainer) m_pContainer->addChild(pIcon);
		m_pContainer->setScale(0.7f);
		if(pDropItem->getType() == drop::kDropTypeSoul || pDropItem->getType() == drop::kDropTypeWarrior){
			m_pContainer->setScale(1.0f);
		}
	}
}

void ItemBox::updateSigninItem(signin::SigninAward *pData , unsigned int idx)
{
	//處理多類型
	drop::DropType type = (idx == 0) ? (drop::DropType)pData->getItem0Type() : (drop::DropType)pData->getItem1Type();
	unsigned num = (idx == 0) ? (drop::DropType)pData->getItem0Num() : (drop::DropType)pData->getItem1Num();
	unsigned id = (idx == 0) ? (drop::DropType)pData->getItem0ID() : (drop::DropType)pData->getItem1ID();

	//處理贈送物品1
	switch (type)
	{
	case drop::kDropTypeItem:
		{
		updateDropItem(drop::Drop::create(item::ItemInfo::create(id, num)));
		break;
		}
	case drop::kDropTypeSoul:
		{
		updateDropItem(drop::Drop::create(warrior::SoulInfo::create(id, num)));
		break;
		}
	case drop::kDropTypeWarrior:
		{
			updateDropItem(drop::Drop::create(warrior::WarriorCurInfo::create(id)));
			break;
		}
	case drop::kDropTypePoint:
		{
		updateDropItem(drop::Drop::createPoint(num));
		break;
		}
	case drop::kDropTypeCoin:
		{
		updateDropItem(drop::Drop::createCoin(num));
		break;
		}
	case drop::kDropTypeEquip:
		{
		updateDropItem(drop::Drop::create(equip::EquipCurInfo::create(id)));
		break;
		}
	}
}


