//
//  TowerHome.cpp
//  千机楼-首页
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TowerHome.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "model/TowerModel.h"

#include "managers/TipsManager.h"

#include "network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

TowerHome::TowerHome():m_StartFight(NULL), m_todayNum(NULL), m_WarriorTodayValue(NULL),m_todayValue(NULL),m_WarriorTomValue(NULL){}
TowerHome::~TowerHome()
{
	CC_SAFE_RELEASE(m_StartFight);
	CC_SAFE_RELEASE(m_todayNum);
	CC_SAFE_RELEASE(m_WarriorTodayValue);
	CC_SAFE_RELEASE(m_todayValue);
	CC_SAFE_RELEASE(m_WarriorTomValue);
}

bool TowerHome::init()
{
	if (!DLayer::init())
	{
		return false;
	}

    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_huodong/PL_tianshoufuge/pl_tsfg_home");

	m_pTowerModel = DM_GET_TOWER_MODEL;

	return true;
}

bool TowerHome::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_StartFight",		CCMenuItemImage *,	m_StartFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_todayNum",			CCLabelBMFont *,	m_todayNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_WarriorTodayValue",CCLabelBMFont *,	m_WarriorTodayValue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_TodayValue",		CCLabelBMFont *,	m_todayValue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_WarriorTomValue",	CCLabelBMFont *,	m_WarriorTomValue);

	return false;
}

SEL_MenuHandler TowerHome::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSelectFight", TowerHome::onSelectFight);

	return NULL;
}

void TowerHome::StartResult(const Json::Value &requestData, const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;

	if (m_pDelegate)
	{
		m_pDelegate->NavigateTo(tower::kNavDifficulty);
	}
}

void TowerHome::onSelectFight(CCObject *sender)
{
	HTTP_CLIENT->towerStart(this, callfuncJson_selector(TowerHome::StartResult));
}

void TowerHome::UpdateInfo()
{
	if (!m_pTowerModel)
	{
		return;
	}

	tower::TowerInfo *pInfo = m_pTowerModel->GetTowerInfo();
	if (!pInfo)
	{
		return;
	}

	m_StartFight->setEnabled(pInfo->hasRound());

	m_todayValue->setString(CCString::createWithFormat("%d", pInfo->mnum)->getCString());
	m_todayNum->setString(CCString::createWithFormat("%d", pInfo->todayRound())->getCString());

	int nTmp = (int)((int)(pInfo->oldnum/5))*3;
	if (nTmp > 30)
	{
		nTmp = 30;
	}
	m_WarriorTodayValue->setString(CCString::createWithFormat("+%d%%", nTmp)->getCString());

	int nTmp1 = (int)((int)(pInfo->mnum/5))*3;
	if (nTmp1 > 30)
	{
		nTmp1 = 30;
	}
	m_WarriorTomValue->setString(CCString::createWithFormat("+%d%%", nTmp1)->getCString());
}

void TowerHome::NotificationUpdate()
{
	UpdateInfo();
}

void TowerHome::onEnter()
{
	DLayer::onEnter();
 	BINDING_EXEC(m_pTowerModel, this, TowerHome::NotificationUpdate, D_EVENT_TOWER_INFO_UPDATE);
}

void TowerHome::onExit()
{
 	UNBINDING_ALL(m_pTowerModel, this);
	DLayer::onExit();
}
