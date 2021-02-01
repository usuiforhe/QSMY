//
//  FirstRecharge.cpp
//  账户余额不足
//
//  Created by cll on 13-6-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "FirstRecharge.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "../Game.h"
#include "../warrior/WarriorInfo.h"
#include "../../../utils/TimeUtil.h"
#include "../equip/EquipInfoView.h"
#include "../inventory/ItemInfoView.h"
USING_NS_CC;


/************************ FirstRecharge class begin*******************/


FirstRecharge::FirstRecharge()
	:m_pHeroContainer(NULL)
	,m_pGradeContainer(NULL)
	,m_pPopupLayer(NULL)
	,pWarriorBase(NULL)
	,m_pGrade1(NULL)
	,m_pGrade2(NULL)
	,m_pGrade3(NULL)
	,m_pGrade4(NULL)
	,m_pTime(NULL)
	,m_pCloseButton(NULL)
	,m_pArr(NULL)
	,m_pItemInfoView(NULL)
{
	for (uint8_t i =0; i<PRESENT_COUNT; ++i)
	{
		m_pItemBox[i] = NULL;
	}
}

FirstRecharge::~FirstRecharge()
{
	CC_SAFE_RELEASE(m_pHeroContainer);
	CC_SAFE_RELEASE(m_pGradeContainer);
	CC_SAFE_RELEASE(m_pGrade1);
	CC_SAFE_RELEASE(m_pGrade2);
	CC_SAFE_RELEASE(m_pGrade3);
	CC_SAFE_RELEASE(m_pGrade4);
	CC_SAFE_RELEASE(m_pTime);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pArr);
	CC_SAFE_RELEASE(m_pItemInfoView);

	for (uint8_t i =0; i<PRESENT_COUNT; ++i)
	{
		CC_SAFE_RELEASE(m_pItemBox[i]);
	}
}

void FirstRecharge::readItemsInfo(CCString *str)
{
	//这个 方法已经 不用了

	/*char* source = NULL;
	char* token = NULL;
	CCArray* tempArray = CCArray::create();

	source = const_cast<char*>(str->getCString());
	token = strtok(source , ";");
	while(token != NULL)
	{
		tempArray->addObject(CCString::create(token));
		token = strtok(NULL , ";");
	}
	
	for(int i = 0 ; i<tempArray->count() ; i++)
	{
		CCString * tempString = dynamic_cast<CCString*>(tempArray->objectAtIndex(i));
		char* subSource = const_cast<char*>(tempString->getCString());
		char* subToken = NULL;
		bool isID = true;
		subToken = strtok(subSource , ",");
		while(subToken != NULL)
		{
			if(isID){
				m_pItemsID[i] = CCString::create(subToken);
			}
			else{
				m_pItemsCount[i] = CCString::create(subToken);
			}
			isID=false;
			subToken = strtok(NULL , ",");
		}
	}
	tempArray->removeAllObjects();
	tempArray->release();*/
}

bool FirstRecharge::init()
{
	if(!DPopup::init(true))
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_shouchong/pl_shouchong");

	m_pCloseButton->getButton()->setTarget(this, menu_selector(FirstRecharge::closeCallBack));

	m_pArr = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pArr);
	for(uint8_t i = 0 ; i < PRESENT_COUNT ; i++ )
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
				m_pArr->addObject(pInfo);
				int iGrade = static_cast<int>(baseInfo->getItemGrade());
				warrior::WarriorGrade wGrade = static_cast<warrior::WarriorGrade>(iGrade);
				m_pItemBox[i]->getGradeBg()->setWarriorGrade(wGrade);
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
				m_pArr->addObject(pInfo);
				int iGrade = static_cast<int>(baseInfo->getEquipGrade());
				warrior::WarriorGrade wGrade = static_cast<warrior::WarriorGrade>(iGrade);
				m_pItemBox[i]->getGradeBg()->setWarriorGrade(wGrade);
			}
			
		}
		
		//item::ItemBase* baseInfo = DM_GET_ITEM_MODEL->getItemBaseByID(m_pItemsID[i]);
		//if(baseInfo)
		//{
		//	m_pItemBox[i]->getName()->setString(baseInfo->getItemName().c_str());
		//	m_pItemBox[i]->getCount()->setString(CCString::createWithFormat("%d" , m_pItemsCount[i])->getCString());
		//	m_pItemBox[i]->getContainer()->removeAllChildren();
		//	m_pItemBox[i]->getContainer()->addChild(ResourceManager::sharedResourceManager()->getItemIcon30P(m_pItemsID[i]));
		//	//处理碎片
		//	if(baseInfo->getItemType() == item::kItemTypeFragment)
		//	{
		//		m_pItemBox[i]->getContainer()->removeAllChildren();
		//		m_pItemBox[i]->getContainer()->addChild(ResourceManager::sharedResourceManager()->getEquipIcon30P(baseInfo->getResID()));
		//		m_pItemBox[i]->getPieceIcon()->setVisible(true);
		//	}
		//}
		/*else{

		}*/
	}

	//顯示首充英雄
	uint8_t heroID = D_CONFIG_UINT_FOR_KEY("FIRSTPUNCH_HERO");
	m_pHeroContainer->removeAllChildren();
	/*if(heroID){
		m_pHeroContainer->addChild(ResourceManager::sharedResourceManager()->getUnitAnimate2XAsync(heroID));
		pWarriorBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(heroID);
		warrior::WarriorGrade grade = pWarriorBase->getColorGrade();
		m_pGrade1->setVisible(grade == warrior::kWarGradeGreen);
		m_pGrade2->setVisible(grade == warrior::kWarGradeBlue);
		m_pGrade3->setVisible(grade == warrior::kWarGradePurple);
		m_pGrade4->setVisible(grade == warrior::kWarGradeRed);
	}*/
	//get current time from the server(second by 1970)
	unsigned int serverTime = GameManager::sharedGameManager()->getServerTime();
	unsigned int runTime = GameManager::sharedGameManager()->getTimer();
	m_pTime->setString(TimeUtil::getSeconds2String(604800));

	m_pPopupLayer = CCNode::create();
	PL_MOVE_WITH_RESOLUTION_DIFF(m_pPopupLayer , -1);
	addChild(m_pPopupLayer);

	return true;

}

bool FirstRecharge::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHeroContainer", CCNode *, this->m_pHeroContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeContainer", CCNode *, this->m_pGradeContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade1", CCSprite *, this->m_pGrade1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade2", CCSprite *, this->m_pGrade2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade3", CCSprite *, this->m_pGrade3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade4", CCSprite *, this->m_pGrade4);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTime", CCLabelBMFont *, this->m_pTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);

	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pItemBox",ItemBox*,this->m_pItemBox,PRESENT_COUNT);

	return false;
}

SEL_MenuHandler FirstRecharge::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onHeroClick", FirstRecharge::onHeroClick);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "rechargeCallBack", FirstRecharge::rechargeCallBack);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", FirstRecharge::closeCallBack);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onItemTouch", FirstRecharge::onItemTouch);
	return NULL;
}

void FirstRecharge::closeCallBack(cocos2d::CCObject *obj)
{
	closePopup();
}

void FirstRecharge::onHeroClick(cocos2d::CCObject *obj)
{
	m_pPopupLayer->removeAllChildren();
	WarriorInfoView *m_pWarriorInfoView = WarriorInfoView::create(WarriorInfoView::kTypeCloseOnly);
	m_pWarriorInfoView->setWarrior(pWarriorBase);
	m_pPopupLayer->addChild(m_pWarriorInfoView);
}

void FirstRecharge::rechargeCallBack(cocos2d::CCObject * obj)
{
	//NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHOW_RECHARGE);

	//close and go to recharge page
	closePopup();
	Game::sharedGame()->showRecharge(true);
}

void FirstRecharge::onItemTouch(cocos2d::CCObject *obj)
{
	if(obj)
	{
		CCMenuItem *btn = dynamic_cast<CCMenuItem*>(obj);
		uint8_t tag = btn->getTag();
		
		user::FirstPunchInfo* pInfo = (user::FirstPunchInfo*)(m_pArr->objectAtIndex(tag-1));
		if(pInfo->getType() == drop::kDropTypeItem)
		{
			item::ItemBase* baseInfo = DM_GET_ITEM_MODEL->getItemBaseByID(pInfo->getID());
			if(baseInfo)
			{
				FirstRecharge::showInfoView(kItemInfo,item::ItemInfo::create(baseInfo));
			}
		}
		else if(pInfo->getType() == drop::kDropTypeEquip)
		{
			equip::EquipBaseInfo* baseInfo = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(pInfo->getID());
			if(baseInfo)
			{
				showInfoView(kEquipInfo,equip::EquipCurInfo::create(baseInfo));
			}
			
		}
		
	}
}

void FirstRecharge::showInfoView(InventoryType type,item::ItemInfo* pItem,CCObject* param)
{
	m_pPopupLayer->removeAllChildren();

	if(m_pItemInfoView == NULL)
	{
		m_pItemInfoView = ItemInfoView::create();
		m_pItemInfoView->setDelegate(this);
		m_pItemInfoView->retain();
	}

	m_pItemInfoView->setItemInfo(pItem);
	m_pPopupLayer->addChild(m_pItemInfoView);
}

void FirstRecharge::showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param)
{
	m_pPopupLayer->removeAllChildren();
	//if(m_pEquipInfoView == NULL)
	//{
	//	m_pEquipInfoView = EquipInfoView::create(EquipInfoView::kTypeViewOnly);
	//	m_pEquipInfoView->setDelegate(this);
	//	m_pEquipInfoView->retain();
	//}

	EquipInfoView* m_pEquipInfoView = EquipInfoView::create(EquipInfoView::kTypeViewOnly);
	m_pEquipInfoView->setDelegate(this);

	m_pEquipInfoView->setEquip(pEquip);
	m_pPopupLayer->addChild(m_pEquipInfoView);
}

void FirstRecharge::closeInfoView(InventoryType type)
{
	m_pPopupLayer->removeAllChildren();
}

void FirstRecharge::closeInfoView(EquipViewType type)
{
	m_pPopupLayer->removeAllChildren();
}
