//
//  LowBalance.cpp
//  账户余额不足
//
//  Created by cll on 13-6-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "LowBalance.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "../Game.h"

USING_NS_CC;


/************************ LowBalance class begin*******************/


LowBalance::LowBalance()
:m_pRecharge(NULL)
,m_pTips(NULL)
,m_pClose(NULL)
,m_pVipNode(NULL)
,m_pVipNow(NULL)
,m_pVipNext(NULL)
,m_pPoint(NULL)
,m_pPercent(NULL)
,m_pVipMenuItem(NULL)
,m_pProgressBar(NULL)
,m_pTVip(NULL)
,m_pTPoint(NULL)
,m_pFirstRechargeNode(NULL)
{
	for (uint8_t i =0; i<BOX_COUNT; ++i)
	{
		m_pItemBox[i] = NULL;
	}
}

LowBalance::~LowBalance()
{
    CC_SAFE_RELEASE(m_pTips);
    CC_SAFE_RELEASE(m_pRecharge);
	CC_SAFE_RELEASE(m_pClose);

	CC_SAFE_RELEASE(m_pVipNode);
	CC_SAFE_RELEASE(m_pVipNow);
	CC_SAFE_RELEASE(m_pVipNext);
	CC_SAFE_RELEASE(m_pPoint);
	CC_SAFE_RELEASE(m_pPercent);
	CC_SAFE_RELEASE(m_pVipMenuItem);
	CC_SAFE_RELEASE(m_pProgressBar);
	CC_SAFE_RELEASE(m_pTVip);
	CC_SAFE_RELEASE(m_pTPoint);
	CC_SAFE_RELEASE(m_pFirstRechargeNode);

	for (uint8_t i =0; i<BOX_COUNT; ++i)
	{
		CC_SAFE_RELEASE(m_pItemBox[i]);
	}
}


bool LowBalance::init()
{
	if(!DPopup::init(true))
	{
		return false;
	}
    
    loadUI();
    m_pRecharge->getButton()->setTarget(this, menu_selector(LowBalance::rechargeCallBack));
    m_pClose->getButton()->setTarget(this, menu_selector(LowBalance::closeCallBack));
	m_pVipMenuItem->setTarget(this, menu_selector(LowBalance::vipCallBack));

	//默认非首充界面
	m_pFirstRechargeNode->setVisible(false);
	m_pVipNode->setVisible(true);
	//默认元宝不足
	m_pTVip->setVisible(false);
	m_pTPoint->setVisible(true);

	//VIP进度条
	user::UserInfo   *m_pUserInfo = DM_GET_USER_MODEL->getUserInfo();
	cocos2d::CCArray *m_pVipInfos = DM_GET_USER_MODEL->getVipInfos();
	//当前VIP等级
	uint32_t userVipLevel = m_pUserInfo->getVipLevel();
	//下一级
	uint32_t nextVipLevel = userVipLevel + 1;

	if(userVipLevel < DM_GET_USER_MODEL->getMaxVipLevel()){
		//未满级的情况
		//再充多少元宝
		uint32_t pointNeed = DM_GET_USER_MODEL->getToVipLevelCost(nextVipLevel);

		user::VipInfo* nextVipInfo = static_cast<user::VipInfo*>(m_pVipInfos->objectAtIndex(nextVipLevel-1));
		user::VipInfo* nowVipInfo = NULL;
		if(m_pUserInfo->getVipLevel() != 0)
		{
			nowVipInfo = static_cast<user::VipInfo*>(m_pVipInfos->objectAtIndex(nextVipLevel-2));
		}

		int pointDetla = (userVipLevel==0) ? nextVipInfo->getPoint() : nextVipInfo->getPoint() - nowVipInfo->getPoint();//兩級相差多少元寶
		int pointHadDetla = (userVipLevel==0) ? m_pUserInfo->getAmount() : m_pUserInfo->getAmount() - nowVipInfo->getPoint();//在這個級別上沖了多少元寶
		if(pointHadDetla < 0) pointHadDetla = 0;

		//显示
		m_pVipNow->setString(CCString::createWithFormat("%d" , userVipLevel)->getCString());	
		m_pVipNext->setString(CCString::createWithFormat("%d" , nextVipLevel)->getCString());	
		m_pPoint->setString(CCString::createWithFormat("%d" , pointNeed)->getCString());	
		m_pPercent->setString(CCString::createWithFormat("%d/%d" , pointHadDetla , pointDetla)->getCString());	
		m_pProgressBar->setPercent((float)pointHadDetla/pointDetla,0.5);
	}
	else{
		//满级了
	}

	return true;  
}

void LowBalance::loadUI()
{
    //CCB_READER_AND_ADDCHILD("ui/ccb/account/lowBalance");
	//CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_shop/PL_shop_3jitankuang");

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_shop/PL_beibao_daojubuzu");
}

bool LowBalance::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTips", CCLabelTTF *, this->m_pTips);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRecharge", DButton *, this->m_pRecharge);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pClose", DButton *, this->m_pClose);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFirstRechargeNode", CCNode *, this->m_pFirstRechargeNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipNode", CCNode *, this->m_pVipNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipNow", CCLabelBMFont *, this->m_pVipNow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipNext", CCLabelBMFont *, this->m_pVipNext);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPoint", CCLabelBMFont *, this->m_pPoint);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPercent", CCLabelBMFont *, this->m_pPercent);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipMenuItem", CCMenuItemImage *, this->m_pVipMenuItem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar", DProgressBar *, this->m_pProgressBar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTVip", CCNode *, this->m_pTVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTPoint", CCNode *, this->m_pTPoint);

	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pItemBox",ItemBox*,this->m_pItemBox,BOX_COUNT);

    return false;
}

SEL_MenuHandler LowBalance::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    //CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", LowBalance::closeCallBack);
    return NULL;
}

void LowBalance::closeCallBack(cocos2d::CCObject *obj)
{
	closePopup();
}

void LowBalance::setTips(const char* tips)
{
    //m_pTips->setString(tips);
}

void LowBalance::setVipNotEnough()
{
	m_pTVip->setVisible(true);
	m_pTPoint->setVisible(false);
}

void LowBalance::rechargeCallBack(cocos2d::CCObject * obj)
{
    //NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHOW_RECHARGE);
	Game::sharedGame()->showRecharge(true);
    closePopup();
}

void LowBalance::vipCallBack(CCObject *sender)
{
	Game::sharedGame()->NavigateTo(navigation::kNavVip);
	closePopup();
}

//void LowBalanceFirst::loadUI()
//{
//    CCB_READER_AND_ADDCHILD("ui/ccb/account/lowBalanceFirst");
//}

bool LowBalanceFirst::init()
{
	LowBalance::init();
	m_pFirstRechargeNode->setVisible(true);
	m_pVipNode->setVisible(false);

	//处理box
	for(uint8_t i = 0 ; i < BOX_COUNT ; i++ )
	{
		user::FirstPunchInfo* pInfo = (user::FirstPunchInfo*)(DM_GET_USER_MODEL->getFirstPunchInfos()->objectAtIndex(i));
		if(pInfo->getType() == drop::kDropTypeItem)
		{
			//物品
			item::ItemBase* baseInfo = DM_GET_ITEM_MODEL->getItemBaseByID(pInfo->getID());
			if(baseInfo)
			{
				m_pItemBox[i]->getName()->setString(baseInfo->getItemName().c_str());
				m_pItemBox[i]->getCount()->setString(CCString::createWithFormat("%d" , pInfo->getNum())->getCString());
				m_pItemBox[i]->getContainer()->removeAllChildren();
				m_pItemBox[i]->getContainer()->addChild(ResourceManager::sharedResourceManager()->getItemIcon30P(pInfo->getID()));
				//处理碎片
				if(baseInfo->getItemType() == item::kItemTypeFragment)
				{
					m_pItemBox[i]->getContainer()->removeAllChildren();
					m_pItemBox[i]->getContainer()->addChild(ResourceManager::sharedResourceManager()->getEquipIcon30P(pInfo->getID()));
					m_pItemBox[i]->getPieceIcon()->setVisible(true);
				}
			}
		}
		else if(pInfo->getType() == drop::kDropTypeEquip){
			//装备
			equip::EquipBaseInfo* baseInfo = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(pInfo->getID());
			if(baseInfo)
			{
				m_pItemBox[i]->getName()->setString(baseInfo->getEquipName().c_str());
				m_pItemBox[i]->getCount()->setString(CCString::createWithFormat("%d" , pInfo->getNum())->getCString());
				m_pItemBox[i]->getContainer()->removeAllChildren();
				m_pItemBox[i]->getContainer()->addChild(ResourceManager::sharedResourceManager()->getEquipIcon30P(pInfo->getID()));
			}
		}
	}

	return true;
}
