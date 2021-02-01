//
//  DressInfo.cpp
//  时装展示
//  Created by mzp on 14-5-15.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "DressInfo.h"
#include "managers/GameManager.h"
#include "../../../model/AvatarModel.h"


DressInfo::DressInfo()
	: m_nId(0)
	, m_pDelegate(NULL)
	, m_pContainer(NULL)
	, m_pName(NULL)
	, m_pName1(NULL)
	, m_pDesc(NULL)
	, m_pButton(NULL)
	, m_pPropertyGroup(NULL)
	, m_pPropertyGroup1(NULL)
	, m_pProperty(NULL)
	, m_pEquipPropertyType(NULL)
	, m_pProperty1(NULL)
	, m_pEquipPropertyType1(NULL)
{

}

DressInfo::~DressInfo()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pName1);
	CC_SAFE_RELEASE(m_pDesc);
	CC_SAFE_RELEASE(m_pButton);
	CC_SAFE_RELEASE(m_pPropertyGroup);
	CC_SAFE_RELEASE(m_pPropertyGroup1);
	CC_SAFE_RELEASE(m_pProperty);
	CC_SAFE_RELEASE(m_pEquipPropertyType);
	CC_SAFE_RELEASE(m_pProperty1);
	CC_SAFE_RELEASE(m_pEquipPropertyType1);
}

bool DressInfo::init()
{
	if (!DLayer::init())
	{
		return false;
	}

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("EquipTypeMark", EquipTypeMarkLoader::loader());

	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/dress/dressInfo", pNodeLoaderLibrary);
	
	m_pButton->getButton()->setTarget(this, menu_selector(DressInfo::closeCallBack));

	return true;
}

bool DressInfo::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName1", CCLabelTTF *, this->m_pName1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF *, this->m_pDesc);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, this->m_pButton);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyGroup", CCNode *, this->m_pPropertyGroup);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyGroup1", CCNode *, this->m_pPropertyGroup1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty", CCLabelBMFont *, this->m_pProperty);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipPropertyType", EquipTypeMark *, this->m_pEquipPropertyType);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty1", CCLabelBMFont *, this->m_pProperty1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipPropertyType1", EquipTypeMark *, this->m_pEquipPropertyType1);

	return false;
}

cocos2d::SEL_MenuHandler DressInfo::onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", DressInfo::closeCallBack);
	return NULL;
}

void DressInfo::setDressId(unsigned int id)
{
	m_nId = id;
	updateView();
}

void DressInfo::updateView()
{
	avatar::AvatarBaseInfo* pAvatarBaseInfo = DM_GET_AVATAR_MODEL->getAvatarBaseInfoById(m_nId);
	if (!pAvatarBaseInfo)
	{
		return;
	}

	m_pContainer->removeAllChildren();
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForLottery(pAvatarBaseInfo->getResId()));
	warrior::WarriorBaseInfo* pWarriorBaseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pAvatarBaseInfo->getWarriorId());
	if (pWarriorBaseInfo)
	{
		m_pName->setString(pWarriorBaseInfo->getWarriorName().c_str());
	}

	m_pName1->setString(pAvatarBaseInfo->getName().c_str());
	m_pDesc->setString(pAvatarBaseInfo->getDesc().c_str());

	m_pPropertyGroup->setVisible(false);
	m_pPropertyGroup1->setVisible(false);



	/*unsigned int group = 0;
	if (pAvatarBaseInfo->getAttack() > 0.0f)
	{
	m_pPropertyGroup->setVisible(true);
	m_pEquipPropertyType->setEquipType(equip::kEquipPropAttack);
	m_pProperty->setString(CCString::createWithFormat("+%d%%", (int)(pAvatarBaseInfo->getAttack() * 100))->getCString());
	++group;
	}

	if (pAvatarBaseInfo->getDefence() > 0.0f)
	{
	if (group == 0)
	{
	m_pPropertyGroup->setVisible(true);
	m_pEquipPropertyType->setEquipType(equip::kEquipPropDefence);
	m_pProperty->setString(CCString::createWithFormat("+%d%%", (int)(pAvatarBaseInfo->getDefence() * 100))->getCString());
	}
	else if (group == 1)
	{
	m_pPropertyGroup1->setVisible(true);
	m_pEquipPropertyType1->setEquipType(equip::kEquipPropDefence);
	m_pProperty1->setString(CCString::createWithFormat("+%d%%", (int)(pAvatarBaseInfo->getDefence() * 100))->getCString());
	}

	++group;
	}

	if (pAvatarBaseInfo->getHp() > 0.0f)
	{
	if (group == 0)
	{
	m_pPropertyGroup->setVisible(true);
	m_pEquipPropertyType->setEquipType(equip::kEquipPropHp);
	m_pProperty->setString(CCString::createWithFormat("+%d%%", (int)(pAvatarBaseInfo->getHp() * 100))->getCString());
	}
	else if (group == 1)
	{
	m_pPropertyGroup1->setVisible(true);
	m_pEquipPropertyType1->setEquipType(equip::kEquipPropHp);
	m_pProperty1->setString(CCString::createWithFormat("+%d%%", (int)(pAvatarBaseInfo->getHp() * 100))->getCString());
	}

	++group;
	}

	if (pAvatarBaseInfo->getSpeed() > 0)
	{
	if (group == 0)
	{
	m_pPropertyGroup->setVisible(true);
	m_pEquipPropertyType->setEquipType(equip::kEquipPropSpeed);
	m_pProperty->setString(CCString::createWithFormat("+%d", pAvatarBaseInfo->getSpeed())->getCString());
	}
	else if (group == 1)
	{
	m_pPropertyGroup1->setVisible(true);
	m_pEquipPropertyType1->setEquipType(equip::kEquipPropSpeed);
	m_pProperty1->setString(CCString::createWithFormat("+%d", pAvatarBaseInfo->getSpeed())->getCString());
	}

	++group;
	}

	if (pAvatarBaseInfo->getDodgeRate() > 0)
	{
	if (group == 0)
	{
	m_pPropertyGroup->setVisible(true);
	m_pEquipPropertyType->setEquipType(equip::kEquipPropDodge);
	m_pProperty->setString(CCString::createWithFormat("+%d%%", pAvatarBaseInfo->getDodgeRate())->getCString());
	}
	else if (group == 1)
	{
	m_pPropertyGroup1->setVisible(true);
	m_pEquipPropertyType1->setEquipType(equip::kEquipPropDodge);
	m_pProperty1->setString(CCString::createWithFormat("+%d%%", pAvatarBaseInfo->getDodgeRate())->getCString());
	}

	++group;
	}

	if (pAvatarBaseInfo->getCriticalRate() > 0)
	{
	if (group == 0)
	{
	m_pPropertyGroup->setVisible(true);
	m_pEquipPropertyType->setEquipType(equip::kEquipPropCritical);
	m_pProperty->setString(CCString::createWithFormat("+%d%%", pAvatarBaseInfo->getCriticalRate())->getCString());
	}
	else if (group == 1)
	{
	m_pPropertyGroup1->setVisible(true);
	m_pEquipPropertyType1->setEquipType(equip::kEquipPropCritical);
	m_pProperty1->setString(CCString::createWithFormat("+%d%%", pAvatarBaseInfo->getCriticalRate())->getCString());
	}

	++group;
	}

	if (pAvatarBaseInfo->getBlockRate() > 0)
	{
	if (group == 0)
	{
	m_pPropertyGroup->setVisible(true);
	m_pEquipPropertyType->setEquipType(equip::kEquipPropBlock);
	m_pProperty->setString(CCString::createWithFormat("+%d%%", pAvatarBaseInfo->getBlockRate())->getCString());
	}
	else if (group == 1)
	{
	m_pPropertyGroup1->setVisible(true);
	m_pEquipPropertyType1->setEquipType(equip::kEquipPropBlock);
	m_pProperty1->setString(CCString::createWithFormat("+%d%%", pAvatarBaseInfo->getBlockRate())->getCString());
	}

	++group;
	}*/
}

void DressInfo::closeCallBack(cocos2d::CCObject* obj)
{
	//if (m_pDelegate)
	//{
	//	m_pDelegate->closeDressInfoView();
	//}
}
