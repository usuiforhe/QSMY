//
//  TowerFailed.cpp
//  千机楼-失败
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TowerFailed.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "model/TowerModel.h"

#include "../../../network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

TowerFailed::TowerFailed():m_btnBack(NULL), m_CurCount(NULL)
{

}
TowerFailed::~TowerFailed()
{
	CC_SAFE_RELEASE(m_btnBack);
	CC_SAFE_RELEASE(m_CurCount);

}

bool TowerFailed::init()
{
	if (!DLayer::init())
	{
		return false;
	}

    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_huodong/PL_tianshoufuge/pl_tsfg_over");

	m_pTowerModel = DM_GET_TOWER_MODEL;

	return true;
}

bool TowerFailed::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnBack",	CCMenuItemImage *,	m_btnBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_CurCount",	CCLabelBMFont *,	m_CurCount);
	return false;
}

SEL_MenuHandler TowerFailed::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchBack", TowerFailed::onTouchBack);

	return NULL;
}

void TowerFailed::onTouchBack(CCObject *sender)
{
	if (m_pDelegate)
	{
		//m_pDelegate->NavigateTo(tower::KNavHome);
		m_pDelegate->ClosePopup();
	}
}

void TowerFailed::UpdateInfo()
{
	tower::TowerInfo *pInfo = m_pTowerModel->GetTowerInfo();
	if (!pInfo)
	{
		return;
	}
	m_CurCount->setString(CCString::createWithFormat("%d",pInfo->mnum)->getCString());

	
	
}

void TowerFailed::NotificationUpdate()
{
	UpdateInfo();
}

void TowerFailed::onEnter()
{
	DLayer::onEnter();

	m_pTowerModel->SetInBattle(false);
	BINDING_EXEC(m_pTowerModel, this, TowerFailed::NotificationUpdate, D_EVENT_TOWER_INFO_UPDATE);
}

void TowerFailed::onExit()
{
	UNBINDING_ALL(m_pTowerModel, this);

	DLayer::onExit();
}
