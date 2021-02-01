//
//  AwardShareItemView.cpp
//  Award--Item
//
//  Created by mzp on 14-4-29
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "AwardShareItemView.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "model/DropModel.h"
#include "utils/SceneUtil.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace sp;
#define ICON_COIN_ID		9901			//coin

#define ICON_POINT_ID		9902			//point
AwardShareItemView::AwardShareItemView()
	:m_pContainer(NULL)
	,m_pDesc(NULL)
	,m_pName(NULL)
	,m_pItemNum(NULL)
	,m_pButton(NULL)
	,m_pEffectNode(NULL)
{
	
}

AwardShareItemView::~AwardShareItemView()
{
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pItemNum);
	CC_SAFE_RELEASE(m_pDesc);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pButton);
	CC_SAFE_RELEASE(m_pEffectNode);
}

AwardShareItemView* AwardShareItemView::create(drop::Drop* obj)
{
	AwardShareItemView* pRet = new AwardShareItemView();
	if (pRet && pRet->init(obj))
	{
		pRet->autorelease();
		return pRet;
	}
	
	CC_SAFE_DELETE(pRet);
	pRet = NULL;
	return NULL;
}

bool AwardShareItemView::init(drop::Drop* obj)
{
	if(!DPopup::init(true))
	{
		return false;
	}
	if(!obj) return false;

	drop::Drop * pDropInfo = obj;
	CC_SAFE_RETAIN(pDropInfo);

	return setPointInfo(pDropInfo->getPoint());
}

bool AwardShareItemView::setPointInfo(uint32_t num)
{
	CCB_READER_AND_ADDCHILD("ui/ccb/common/reward/rewardShare");

	m_pContainer->removeAllChildren();

	item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(POINT_ID);
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon(POINT_ID));

	m_pName->setString(pItemBase->getItemName().c_str());
	m_pDesc->setString(pItemBase->getItemDesc().c_str());

	m_pItemNum->setString(CCString::createWithFormat("x%d",num)->getCString());
	m_pButton->getButton()->setTarget(this,menu_selector(AwardShareItemView::closeCallBack));
	return true;
}

bool AwardShareItemView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemNum", CCLabelBMFont *, this->m_pItemNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF *, this->m_pDesc);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, this->m_pButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode",CCNode *, this->m_pEffectNode);

	return false;
}

void AwardShareItemView::closeCallBack(cocos2d::CCObject* obj)
{
	removeFromParent();
}

Armature * AwardShareItemView::getAnimation(warrior::WarriorCurInfo *obj)
{
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("grade");
	pArmature->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(obj->getWid()));
	return pArmature;

}

Armature * AwardShareItemView::getAnimation(equip::EquipCurInfo *obj)
{
    if(!obj->getBaseInfo()->isNeedShowGradeColor()) return NULL;
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("grade");
	pArmature->setColor(DM_GET_EQUIP_MODEL->getEquipColorByGrade(obj->getBaseInfo()->getEquipGrade()));
	return pArmature;

}

Armature * AwardShareItemView::getAnimation(item::ItemInfo *obj)
{
    if(!obj->getBaseInfo()->isNeedShowGradeColor()) return NULL;
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("grade");
	pArmature->setColor(DM_GET_ITEM_MODEL->getItemColorByGrade(obj->getBaseInfo()->getItemGrade()));
	return pArmature;
    
}

Armature * AwardShareItemView::getAnimation(warrior::SoulInfo *obj)
{
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("grade");
	pArmature->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(obj->getWid()));
	return pArmature;
}



