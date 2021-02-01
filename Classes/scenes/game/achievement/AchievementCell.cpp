//
//  AchievementCell.cpp
//  QSMY
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "AchievementCell.h"

USING_NS_CC;
USING_NS_CC_EXT;

AchievementCell::AchievementCell()
	: m_pBMFCount(NULL)
	, m_pTTFDes(NULL)
	, m_pTTFName(NULL)
	, m_pIconContainer(NULL)
	, m_pBMFAchPoint(NULL)
	, m_pNodeInvalid(NULL)
	, m_pNodeValid(NULL)
	, m_taskID(0)
	, m_uItemID(0)
	, m_pDelegate(NULL)
{
    
}

AchievementCell::~AchievementCell()
{
	CC_SAFE_RELEASE(m_pBMFCount);
	CC_SAFE_RELEASE(m_pTTFDes);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pIconContainer);
	CC_SAFE_RELEASE(m_pBMFAchPoint);
	CC_SAFE_RELEASE(m_pNodeInvalid);
	CC_SAFE_RELEASE(m_pNodeValid);
}

bool AchievementCell::init()
{
    if(!CCTableViewCell::init()) return false;
    
	cocos2d::CCNode *pNode = NULL;
	CCB_READER("PL_ui/ccb/pl_chengjiu/PL_chengjiu_tiaojian_cell",pNode);
	if(pNode) addChild(pNode);

    return true;
}

bool AchievementCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCount", CCLabelBMFont *, m_pBMFCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFDes", CCLabelTTF *, m_pTTFDes);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pIconContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFAchPoint", CCLabelBMFont *, m_pBMFAchPoint);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeInvalid", CCNode *, m_pNodeInvalid);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeValid", CCNode *, m_pNodeValid);
    return false;
}

SEL_MenuHandler AchievementCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "recieveCallBack", AchievementCell::getAward);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "viewCallBack", AchievementCell::onView);
	return NULL;
}

void AchievementCell::onView(CCObject* obj)
{
	if(m_uItemID != 0 && m_pDelegate)
	{
		m_pDelegate->onView(m_uItemID , drop::kDropTypeItem);
	}
}

void AchievementCell::getAward(CCObject* obj)
{
	DM_GET_ACHIEVEMENT_MODEL->getAward(m_taskID);
}
void AchievementCell::setData(achievement::TaskInfo *pInfo)
{
	achievement::TaskBase *pBaseInfo = pInfo->getBaseInfo();

	m_pBMFCount->setString(D_CSTRING_FROM_UINT(pBaseInfo->getRewardNum()));
	m_pBMFAchPoint->setString(D_CSTRING_FROM_UINT(pBaseInfo->getScore()));

	//点击查看物品详情
	if(pBaseInfo->getRewardType() == drop::kDropTypeItem || pBaseInfo->getRewardType() == drop::kDropTypePkg)
	{
		m_uItemID = pBaseInfo->getRewardID();
	}
	else{
		m_uItemID = 0;
	}


	//記錄taskid
	m_taskID = pInfo->getTaskID();

	//能否領取
	m_pNodeValid->setVisible(pInfo->getTaskState() == achievement::kTaskStateDone);//TODO 以後改成枚舉
	m_pNodeInvalid->setVisible(!m_pNodeValid->isVisible());

	//成就名顯示
	if(pBaseInfo->getType() == achievement::kTaskTypeHeroLevel || pBaseInfo->getType() == achievement::kTaskTypeEquipLevel || pBaseInfo->getType() == achievement::kTaskTypeTower){
		//單個數字 毒鏢
		m_pTTFDes->setString(CCString::createWithFormat(pBaseInfo->getDescription().c_str() , pBaseInfo->getTaskNum())->getCString());
	}
	else{
		//兩個數字
		uint32_t progress = pInfo->getTaskProgress() > pBaseInfo->getTaskNum() ? pBaseInfo->getTaskNum() : pInfo->getTaskProgress();
		m_pTTFDes->setString(CCString::createWithFormat(pBaseInfo->getDescription().c_str() , progress , pBaseInfo->getTaskNum())->getCString());
	}
	

	m_pIconContainer->removeAllChildren();
	switch (pBaseInfo->getRewardType()) {
	case drop::kDropTypeCoin:
		{
			m_pTTFName->setString(D_LOCAL_STRING("Coin").c_str());
			m_pIconContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50P(COIN_ID));
			break;
		}
	case drop::kDropTypePoint:
		{
			m_pTTFName->setString(D_LOCAL_STRING("Point").c_str());
			m_pIconContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50P(POINT_ID));
			break;
		}
	case drop::kDropTypeVip:
		{
			user::VipInfo * pVipInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(pBaseInfo->getRewardNum());
			item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(pVipInfo->getVipItemId());

			m_pTTFName->setString(pItemBase->getItemName().c_str());
			m_pIconContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(pItemBase->getItemId()));
			break;
		}
	case drop::kDropTypeEquip:
		{


			equip::EquipBaseInfo * pEquipBase = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(pBaseInfo->getRewardID());
			m_pTTFName->setString(pEquipBase->getEquipName().c_str());
			m_pIconContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon50PForList(pEquipBase->getEquipId()));

			break;
		}
	case drop::kDropTypeItem:
	case drop::kDropTypePkg:
		{
			item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(pBaseInfo->getRewardID());
			m_pTTFName->setString(pItemBase->getItemName().c_str());
			m_pIconContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(pItemBase->getItemId()));

			break;
		}
	case drop::kDropTypeSoul:
		{
			warrior::WarriorBaseInfo * pWarriorBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pBaseInfo->getRewardID());
			m_pTTFName->setString(pWarriorBase->getWarriorName().c_str());
			m_pIconContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForActivityList(pWarriorBase->getWarriorId()));
			break;
		}
	case drop::kDropTypeAvatar:
		{
			avatar::AvatarBaseInfo * pAvatarBaseInfo = DM_GET_AVATAR_MODEL->getAvatarBaseInfoById(pBaseInfo->getRewardID());
			m_pTTFName->setString(pAvatarBaseInfo->getName().c_str());
			m_pIconContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForActivityList(pAvatarBaseInfo->getResId()));
			break;
		}
	case drop::kDropTypeWarrior:
		{
			warrior::WarriorBaseInfo * pWarriorBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pBaseInfo->getRewardID());
			m_pTTFName->setString(pWarriorBase->getWarriorName().c_str());
			m_pIconContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForActivityList(pWarriorBase->getWarriorId()));

			break;
		}
	default:
		return;
		break;
	}

	//m_pTTFName->setString(D_CSTRING_FROM_UINT((int)pBaseInfo->getType()));
	//m_pBMFCount->setString(D_CSTRING_FROM_UINT(pInfo->getTaskID()));
}