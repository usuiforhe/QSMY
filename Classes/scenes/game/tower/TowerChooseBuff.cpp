//
//  TowerChooseBuff.cpp
//  千机楼-选择buff
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TowerChooseBuff.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "model/TowerModel.h"

#include "../../../managers/TipsManager.h"

#include "../../../network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

TowerChooseBuff::TowerChooseBuff():
	m_pDelegate(NULL),
	m_MissionName(NULL), 
	m_HavePower(NULL),m_pUseValue(0),m_attackAdd(NULL),m_defenseAdd(NULL),m_hpAdd(NULL)
{
	for (int i=0; i<3; ++i)
	{
		m_BuffValue[i]		= NULL;
		m_UseValue[i]		= NULL;
		m_BuffType[i]		= NULL;
		m_btnBuffChoose[i]	= NULL;
	}
}

TowerChooseBuff::~TowerChooseBuff()
{
	CC_SAFE_RELEASE(m_MissionName);

	for (int i=0; i<3; ++i)
	{
		CC_SAFE_RELEASE(m_BuffValue[i]);
		CC_SAFE_RELEASE(m_UseValue[i]);
		CC_SAFE_RELEASE(m_BuffType[i]);
		CC_SAFE_RELEASE(m_btnBuffChoose[i]);
	}

	CC_SAFE_RELEASE(m_HavePower);
	CC_SAFE_RELEASE(m_defenseAdd);
	CC_SAFE_RELEASE(m_attackAdd);
	CC_SAFE_RELEASE(m_hpAdd);
}

bool TowerChooseBuff::init()
{
	if (!DLayer::init())
	{
		return false;
	}

    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_huodong/PL_tianshoufuge/pl_tsfg_chooseBuff");

	m_pTowerModel = DM_GET_TOWER_MODEL;

	return true;
}

bool TowerChooseBuff::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_MissionName", CCLabelBMFont*, m_MissionName);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_BuffType0",		CCNode *,			m_BuffType[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_UseValue0",		CCLabelBMFont *,	m_UseValue[0]);
 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_BuffValue0",		CCLabelBMFont *,	m_BuffValue[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnBuffChoose0",	CCMenuItemImage*,	m_btnBuffChoose[0]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_BuffType1",		CCNode *,			m_BuffType[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_UseValue1",		CCLabelBMFont *,	m_UseValue[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_BuffValue1",		CCLabelBMFont *,	m_BuffValue[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnBuffChoose1",	CCMenuItemImage*,	m_btnBuffChoose[1]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_BuffType2",		CCNode *,			m_BuffType[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_UseValue2",		CCLabelBMFont *,	m_UseValue[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_BuffValue2",		CCLabelBMFont *,	m_BuffValue[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnBuffChoose2",	CCMenuItemImage*,	m_btnBuffChoose[2]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_HavePower",		CCLabelBMFont*,		m_HavePower);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_attackAdd",		CCLabelBMFont*,		m_attackAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_defenseAdd",		CCLabelBMFont*,		m_defenseAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_hpAdd",		CCLabelBMFont*,		m_hpAdd);

	return false;
}

SEL_MenuHandler TowerChooseBuff::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onChoose", TowerChooseBuff::onChoose);

	return NULL;
}

void TowerChooseBuff::onChoose(CCObject *sender)
{
	CCNode *pNode = (CCNode*)sender;
	if (pNode)
	{
		tower::TowerBuff* pBuff = (tower::TowerBuff*)(pNode->getUserData());
		if (pBuff)
		{
			if (m_pTowerModel->GetTowerInfo()->scores < pBuff->cost)
			{
				std::string strTips = D_LOCAL_STRING("TOWERPOWERNOTENOUGH").c_str();
				Tips_Warning(strTips);
				return;
			}
			m_pUseValue = pBuff->cost;
			HTTP_CLIENT->towerSetBuff(pBuff->id, this, callfuncJson_selector(TowerChooseBuff::ChooseResult));
		}
	}
}

void TowerChooseBuff::ChooseResult(const Json::Value &requestData, const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
 
	if (m_pDelegate)
	{
		m_pDelegate->NavigateTo(tower::kNavDifficulty);

		m_pTowerModel->AddTowerScores(-m_pUseValue);
		m_pTowerModel->ClearTowerBuff();
		m_pTowerModel->postNotification(D_EVENT_TOWER_INFO_UPDATE);
		m_pUseValue = 0;
	}
}

void TowerChooseBuff::NotificationUpdate()
{
	UpdateInfo();
}

const std::string strBaseDir = "PL_ui/tianshoufuge/";
const std::string strChooseBuffType[] = 
{
	"",
	"pl_tsfg_shuxing_wenzi_gong.png",
	"pl_tsfg_shuxing_wenzi_fang.png",
	"pl_tsfg_shuxing_wenzi_xue.png",
	"su.png",
	"bao.png",
	"zhaojia.png",
	"shanbi.png"
};

void TowerChooseBuff::UpdateInfo()
{
	tower::TowerInfo *pInfo = m_pTowerModel->GetTowerInfo();
	if (!pInfo)
	{
		return;
	}

	if (!pInfo->arrBuff || pInfo->arrBuff->count()==0)
	{
		return;
	}

	int nPos = 0;
	CCObject *pObj;
	tower::TowerBuff* pBuffData = NULL;
	CCARRAY_FOREACH(pInfo->arrBuff, pObj)
	{
		pBuffData = (tower::TowerBuff*)pObj;
		if (pBuffData)
		{
			if (nPos >= 3)
			{
				break;
			}

			if (pBuffData->bid>7 || pBuffData->bid<1)
			{
				pBuffData->bid = 1;
			}

			std::string strPicDir = strBaseDir+strChooseBuffType[pBuffData->bid];
			m_BuffType[nPos]->removeAllChildren();
			m_BuffType[nPos]->addChild(CCSprite::create(strPicDir.c_str()));
			m_UseValue[nPos]->setString(CCString::createWithFormat("%d",pBuffData->cost)->getCString());		
			m_BuffValue[nPos]->setString(CCString::createWithFormat("+%d%%",pBuffData->value)->getCString());	
			m_btnBuffChoose[nPos]->setUserData(pBuffData);

			nPos++;
		}
	}

	m_MissionName->setString(CCString::createWithFormat("%d",pInfo->mid)->getCString());
	m_HavePower->setString(CCString::createWithFormat("%d", pInfo->scores)->getCString());

	tower::TowerAdd *pAddInfo = m_pTowerModel->GetTowerAdd();
	m_attackAdd->setString(CCString::createWithFormat("%d%%", (int)(pAddInfo->fAtt * 100))->getCString());
	m_defenseAdd->setString(CCString::createWithFormat("%d%%", (int)(pAddInfo->fDef * 100))->getCString());
	m_hpAdd->setString(CCString::createWithFormat("%d%%", (int)(pAddInfo->fHp * 100))->getCString());
}

void TowerChooseBuff::onEnter()
{
	DLayer::onEnter();
	BINDING_EXEC(m_pTowerModel, this, TowerChooseBuff::NotificationUpdate, D_EVENT_TOWER_INFO_UPDATE);
}

void TowerChooseBuff::onExit()
{
	UNBINDING_ALL(m_pTowerModel, this);
	DLayer::onExit();
}
