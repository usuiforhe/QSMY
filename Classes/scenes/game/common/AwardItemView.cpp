//
//  AwardItemView.cpp
//  Award--Item
//
//  Created by Icy on 13-7-18
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "AwardItemView.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "model/DropModel.h"
#include "utils/SceneUtil.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace sp;
#define ICON_COIN_ID		9901			//coin

#define ICON_POINT_ID		9902			//point
AwardItemView::AwardItemView()
	:m_pContainer(NULL)
	,m_pDesc(NULL)
	,m_pName(NULL)
	,m_pItemNum(NULL)
	,m_pButton(NULL)
	,m_pCloseButton(NULL)
	,m_pProperty(NULL)
	,m_pAttack(NULL)
	,m_pHp(NULL)
	,m_pDefence(NULL)
	,m_pSpeed(NULL)
	,m_pEffectNode(NULL)
	,m_pGradeBg(NULL)
	,m_pHunpo(NULL)
{
	for (uint8_t i =0; i<equip::kEquipPropTotal; ++i)
	{
		m_pPropertyType[i] = NULL;
	}
}

AwardItemView::~AwardItemView()
{
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pItemNum);
	CC_SAFE_RELEASE(m_pDesc);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pButton);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pHp);
	CC_SAFE_RELEASE(m_pAttack);
	CC_SAFE_RELEASE(m_pDefence);
	CC_SAFE_RELEASE(m_pSpeed);
	CC_SAFE_RELEASE(m_pEffectNode);
	CC_SAFE_RELEASE(m_pGradeBg);
	CC_SAFE_RELEASE(m_pHunpo);
	for (uint8_t i =0; i<equip::kEquipPropTotal; ++i)
	{
		CC_SAFE_RELEASE(m_pPropertyType[i]);
	}
}


bool AwardItemView::init(drop::Drop *obj)

{
	if(!DPopup::init(true))
	{
		return false;
	}
	if(!obj) return false;

	drop::Drop * pDropInfo = obj;
	CC_SAFE_RETAIN(pDropInfo);

	//m_pEffectNode->removeAllChildren();
	//m_pEffectNode->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("rewardmulti"));


	switch(pDropInfo->getType())
	{
		case drop::kDropTypeItem:
		case drop::kDropTypePkg:
			return setItemInfo(pDropInfo->getItem());
		case drop::kDropTypeEquip:
			return setEquipInfo(pDropInfo->getEquip());
		case drop::kDropTypeWarrior:
			return setWarriorInfo(pDropInfo->getWarrior());
		case drop::kDropTypeSoul:
			return setSoulInfo(pDropInfo->getSoul());
		case drop::kDropTypePoint:
			return setPointInfo(pDropInfo->getPoint());
		case drop::kDropTypeCoin:
			return setCoinInfo(pDropInfo->getCoin());
		case drop::kDropTypeVip:
			return setVipInfo(pDropInfo->getVip());
        case drop::kDropTypePower:
            return setPowerInfo(pDropInfo->getPower());
        case drop::kDropTypeAvatar:
            return setAvatarInfo(pDropInfo->getAvatarInfo());
		default:
			return false;
	}
	return true;
}

bool AwardItemView::setAvatarInfo(avatar::AvatarInfo *obj)
{
    //CCB_READER_AND_ADDCHILD("ui/ccb/common/reward/rewardDress");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_danchou");
	m_pContainer->removeAllChildren();
    
    
	//m_pEffectNode->removeAllChildren();
    
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForDetail(obj->getBaseInfo()->getResId()));
	m_pName->setString(obj->getBaseInfo()->getName().c_str());
	m_pDesc->setString(obj->getBaseInfo()->getDesc().c_str());
	m_pButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
	m_pCloseButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
	return true;
}

bool AwardItemView::setItemInfo(item::ItemInfo *obj)
{
	//CCB_READER_AND_ADDCHILD("ui/ccb/common/reward/rewardItem");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_danchou");
    //m_pEffectNode->removeAllChildren();
	//Armature *pEffect = getAnimation(obj);
	//if(pEffect) m_pEffectNode->addChild(pEffect);
    
	m_pContainer->removeAllChildren();
	
	if(obj->getBaseInfo()->getItemType() == item::kItemTypeFragment){
		m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon(obj->getBaseInfo()->getResID()));
	}
	else{
		m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon(obj->getItemId()));
	}

	m_pName->setString(obj->getBaseInfo()->getItemName().c_str());

	m_pDesc->setString(obj->getBaseInfo()->getItemDesc().c_str());
	m_pItemNum->setString(CCString::createWithFormat("x%d",obj->getNum())->getCString());
	m_pButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
	m_pCloseButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));

	int grade = (int)obj->getBaseInfo()->getItemGrade();
	m_pGradeBg->setWarriorGrade(warrior::WarriorGrade(grade));

	return true;
}

bool AwardItemView::setWarriorInfo(warrior::WarriorCurInfo *obj)
{

	//CCB_READER_AND_ADDCHILD("ui/ccb/common/reward/rewardWarrior");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_danchou");

	m_pContainer->removeAllChildren();

	//m_pEffectNode->removeAllChildren();
	//m_pEffectNode->addChild(getAnimation(obj));

	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon(obj->getWid() , 0.5f));
	m_pContainer->setPositionY(m_pContainer->getPositionY() - 70.0f);
	m_pName->setString(obj->getBaseInfo()->getWarriorName().c_str());
	m_pDesc->setString(obj->getBaseInfo()->getWarriorDesc().c_str());
	//m_pAttack->setString(CCString::createWithFormat("%d",obj->getBaseInfo()->getBasicAttack())->getCString());
	//m_pDefence->setString(CCString::createWithFormat("%d",obj->getBaseInfo()->getBasicDefence())->getCString());
	//m_pHp->setString(CCString::createWithFormat("%d",obj->getBaseInfo()->getBasicHp())->getCString());
	//m_pSpeed->setString(CCString::createWithFormat("%d",obj->getBaseInfo()->getBasicSpeed())->getCString());

	m_pButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
	m_pCloseButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));

	int grade = (int)obj->getBaseInfo()->getColorGrade();
	m_pGradeBg->setWarriorGrade(warrior::WarriorGrade(grade));

	return true;
}



bool AwardItemView::setEquipInfo(equip::EquipCurInfo *obj)
{
	//CCB_READER_AND_ADDCHILD("ui/ccb/common/reward/rewardEquip");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_danchou");

	m_pContainer->removeAllChildren();

	//m_pEffectNode->removeAllChildren();
    //Armature *pEffect = getAnimation(obj);
	//if(pEffect) m_pEffectNode->addChild(pEffect);

	CCSprite* equipIcon = ResourceManager::sharedResourceManager()->getEquipIcon(obj->getEquipId());
	if (equipIcon)
	{
		equipIcon->setScale(0.6f);
		m_pContainer->removeAllChildren();
		m_pContainer->addChild(equipIcon);
	}
	
	m_pName->setString(obj->getBaseInfo()->getEquipName().c_str());
	m_pDesc->setString(obj->getBaseInfo()->getEquipDesc().c_str());
	m_pButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
	m_pCloseButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));

	if (m_pGradeBg)
	{
		m_pGradeBg->setWarriorGrade((warrior::WarriorGrade)(obj->getBaseInfo()->getEquipGrade()));
	}

	equip::EquipPropType type = equip::kEquipPropAttack;
	uint32_t prop_value = 0;
	std::string pecent;

	switch(obj->getBaseInfo()->getEquipType())
	{
		case equip::kEquipTypeWeapon:	//weapon +attack
			type = equip::kEquipPropAttack;
			prop_value = obj->getBaseInfo()->getAttack();
			break;
		case equip::kEquipTypeArmor:	//defence +defence
			type = equip::kEquipPropDefence;
			prop_value = obj->getBaseInfo()->getDefence();
			break;
		case equip::kEquipTypeOrnament:	//ornament + hp
			type = equip::kEquipPropHp;
			prop_value = obj->getBaseInfo()->getHp();
			break;
		case equip::kEquipTypeTreasure:		//treasure + speed/dodge/critical/block
		{
			if(obj->getBaseInfo()->getSpeed()>0)
			{
				type = equip::kEquipPropSpeed;
				prop_value = obj->getBaseInfo()->getSpeed();
			}
			else if(obj->getBaseInfo()->getDodgeRate()>0)
			{
				type = equip::kEquipPropDodge;
				prop_value = obj->getBaseInfo()->getDodgeRate();
				pecent = "%";
			}
			else if(obj->getBaseInfo()->getCriticalRate()>0)
			{
				type = equip::kEquipPropCritical;
				prop_value = obj->getBaseInfo()->getCriticalRate();
				pecent = "%";
			}
			else if(obj->getBaseInfo()->getBlockRate()>0)
			{
				type = equip::kEquipPropBlock;
				prop_value = obj->getBaseInfo()->getBlockRate();
				pecent = "%";
			}else
			{
				CCLOG("error prop");
			}
			break;
		}
		default:
			CCLOG("error type");
			break;
	}

	//for (uint8_t i =0; i<equip::kEquipPropTotal; ++i)
	//{
	//	if(m_pPropertyType[i])
	//	{
	//		m_pPropertyType[i]->setVisible(i==type);
	//	}
	//}

	//m_pProperty->setString(CCString::createWithFormat("+%d%s",prop_value,pecent.c_str())->getCString());

	return true;
}

bool AwardItemView::setSoulInfo(warrior::SoulInfo *obj)
{
	//CCB_READER_AND_ADDCHILD("ui/ccb/common/reward/rewardSoul");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_danchou");

	m_pContainer->removeAllChildren();


	//m_pEffectNode->removeAllChildren();
	//m_pEffectNode->addChild(getAnimation(obj));
	m_pHunpo->setVisible(true);
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon(obj->getWid() , 0.5f));
	m_pContainer->setPositionY(m_pContainer->getPositionY() - 70.0f);
	m_pName->setString(obj->getBaseInfo()->getWarriorName().c_str());
	m_pDesc->setString(obj->getBaseInfo()->getWarriorDesc().c_str());
	m_pButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
	m_pCloseButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
	m_pItemNum->setString(CCString::createWithFormat("x%d",obj->getNum())->getCString());

	int grade = (int)obj->getBaseInfo()->getColorGrade();
	m_pGradeBg->setWarriorGrade(warrior::WarriorGrade(grade));

	return true;
}

bool AwardItemView::setPointInfo(uint32_t num)
{
	//CCB_READER_AND_ADDCHILD("ui/ccb/common/reward/rewardItem");
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_danchou");

	m_pContainer->removeAllChildren();
    
    item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(POINT_ID);
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon(POINT_ID));
    
	m_pName->setString(pItemBase->getItemName().c_str());
	m_pDesc->setString(pItemBase->getItemDesc().c_str());
    
	m_pItemNum->setString(CCString::createWithFormat("x%d",num)->getCString());
	m_pButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
	m_pCloseButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));

	return true;
}
bool AwardItemView::setCoinInfo(uint32_t num)
{
	//CCB_READER_AND_ADDCHILD("ui/ccb/common/reward/rewardItem");
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_danchou");

	m_pContainer->removeAllChildren();
    
    item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(COIN_ID);
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon(COIN_ID));
    
	m_pName->setString(pItemBase->getItemName().c_str());
	m_pDesc->setString(pItemBase->getItemDesc().c_str());
    
	m_pItemNum->setString(CCString::createWithFormat("x%d",num)->getCString());
	m_pButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
	m_pCloseButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));

	return true;
}

bool AwardItemView::setVipInfo(uint32_t vip)
{
	//CCB_READER_AND_ADDCHILD("ui/ccb/common/reward/rewardVip");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_danchou");

	m_pContainer->removeAllChildren();
    user::VipInfo * pVipInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(vip);
    item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(pVipInfo->getVipItemId());
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon(pItemBase->getItemId()));
	m_pName->setString(pItemBase->getItemName().c_str());
	m_pDesc->setString(pItemBase->getItemDesc().c_str());
	m_pButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
	m_pCloseButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
    
	return true;
}

bool AwardItemView::setPowerInfo(uint32_t num)
{
	//CCB_READER_AND_ADDCHILD("ui/ccb/common/reward/rewardItem");
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_danchou");

	m_pContainer->removeAllChildren();
    
    item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(POWER_ID);
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon(POWER_ID));
    
	m_pName->setString(pItemBase->getItemName().c_str());
	m_pDesc->setString(pItemBase->getItemDesc().c_str());
    
	m_pItemNum->setString(CCString::createWithFormat("x%d",num)->getCString());
	m_pButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
	m_pCloseButton->getButton()->setTarget(this,menu_selector(AwardItemView::closeCallBack));
    
	return true;
}

bool AwardItemView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemNum", CCLabelBMFont *, this->m_pItemNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF *, this->m_pDesc);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, this->m_pButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty", CCLabelBMFont *, this->m_pProperty);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyType0", CCSprite *, this->m_pPropertyType[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyType1", CCSprite *, this->m_pPropertyType[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyType2", CCSprite *, this->m_pPropertyType[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyType3", CCSprite *, this->m_pPropertyType[3]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyType4", CCSprite *, this->m_pPropertyType[4]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyType5", CCSprite *, this->m_pPropertyType[5]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyType6", CCSprite *, this->m_pPropertyType[6]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttack", CCLabelBMFont *, this->m_pAttack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefence", CCLabelBMFont *, this->m_pDefence);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeed", CCLabelBMFont *, this->m_pSpeed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHp", CCLabelBMFont *, this->m_pHp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode",CCNode *, this->m_pEffectNode);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBg", WarriorGradeBg *, this->m_pGradeBg);	

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBg", WarriorGradeBg*, this->m_pGradeBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHunpo", CCNode*, this->m_pHunpo);
	return false;
}

void AwardItemView::closeCallBack(cocos2d::CCObject* obj)
{
	removeFromParent();
}

void AwardItemView::shareCallBack(cocos2d::CCObject* obj)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHARE, CCStringMake(D_LOCAL_STRING("ShareMessage")));

	removeFromParent();
}

Armature * AwardItemView::getAnimation(warrior::WarriorCurInfo *obj)
{
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("grade");
	pArmature->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(obj->getWid()));
	return pArmature;

}

Armature * AwardItemView::getAnimation(equip::EquipCurInfo *obj)
{
    if(!obj->getBaseInfo()->isNeedShowGradeColor()) return NULL;
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("grade");
	pArmature->setColor(DM_GET_EQUIP_MODEL->getEquipColorByGrade(obj->getBaseInfo()->getEquipGrade()));
	return pArmature;

}

Armature * AwardItemView::getAnimation(item::ItemInfo *obj)
{
    if(!obj->getBaseInfo()->isNeedShowGradeColor()) return NULL;
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("grade");
	pArmature->setColor(DM_GET_ITEM_MODEL->getItemColorByGrade(obj->getBaseInfo()->getItemGrade()));
	return pArmature;
    
}

Armature * AwardItemView::getAnimation(warrior::SoulInfo *obj)
{
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("grade");
	pArmature->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(obj->getWid()));
	return pArmature;
}



