//
//  TowerReward.cpp
//  千机楼-奖励
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TowerReward.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "model/TowerModel.h"

#include "../../../managers/ResourceManager.h"

#include "../../../network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

TowerReward::TowerReward():m_WinAni(NULL), m_item1Icon(NULL),m_item2Icon(NULL),m_itemName1(NULL),m_itemName2(NULL),m_GetMoney(NULL),m_btnBack(NULL){}
TowerReward::~TowerReward()
{
	CC_SAFE_RELEASE(m_WinAni);
	CC_SAFE_RELEASE(m_item1Icon);
	CC_SAFE_RELEASE(m_item2Icon);
	CC_SAFE_RELEASE(m_itemName1);
	CC_SAFE_RELEASE(m_itemName2);
	CC_SAFE_RELEASE(m_GetMoney);
	CC_SAFE_RELEASE(m_btnBack);
}

bool TowerReward::init()
{
	if (!DLayer::init())
	{
		return false;
	}

    CCB_READER_AND_ADDCHILD("ui/ccb/tower/TowerReward");

	m_pTowerModel = DM_GET_TOWER_MODEL;

	return true;
}

bool TowerReward::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_WinAni",	CCNode *,			m_WinAni);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_item1Icon",CCNode *,			m_item1Icon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_item2Icon",CCNode *,			m_item2Icon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_itemName1",CCLabelTTF *,		m_itemName1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_itemName2",CCLabelTTF *,		m_itemName2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_GetMoney",	CCLabelBMFont *,	m_GetMoney);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnBack",	CCMenuItemImage *,	m_btnBack);

	return false;
}

SEL_MenuHandler TowerReward::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchBack", TowerReward::onTouchBack);

	return NULL;
}

void TowerReward::onTouchBack(CCObject *sender)
{    
    m_pTowerModel->SetInBattle(false);
    
	if (m_pDelegate)
	{
		m_pDelegate->ClosePopup();
	}
}

void TowerReward::NotificationUpdate()
{
	UpdateInfo();
}

void TowerReward::UpdateInfo()
{
	m_item1Icon->removeAllChildren();
	m_item2Icon->removeAllChildren();
	m_itemName1->setString("");
	m_itemName2->setString("");
	m_GetMoney->setString("0");

	//test null begin
	m_WinAni->getTag();
	//end
}

void TowerReward::onEnter()
{
	DLayer::onEnter();
	BINDING_EXEC(m_pTowerModel, this, TowerReward::NotificationUpdate, D_EVENT_TOWER_INFO_UPDATE);
}

void TowerReward::onExit()
{
	UNBINDING_ALL(m_pTowerModel, this);
	DLayer::onExit();
}
