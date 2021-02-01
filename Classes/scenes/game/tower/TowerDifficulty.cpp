//
//  TowerDifficulty.cpp
//  千机楼-困难选择
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TowerDifficulty.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "model/TowerModel.h"

#include "../../../managers/TipsManager.h"
#include "../../../managers/ResourceManager.h"

#include "../../../network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

TowerDifficulty::TowerDifficulty()
	:m_MissionName(NULL), 
	m_btnHardFight(NULL),
	m_HardReward(NULL),
	m_HardItem(NULL),
	m_HardInteral(NULL),
	m_btnNormalFight(NULL), 
	m_NormalReward(NULL),
	m_NormalItem(NULL),
	m_NormalInteral(NULL),
	m_btnEasyFight(NULL),
	m_EasyReward(NULL),
	m_EasyItem(NULL),
	m_EasyInteral(NULL),
	m_HavePower(NULL),
	m_pass(NULL),
	//m_unPass(NULL),
	m_attackAdd(NULL),
	m_defenseAdd(NULL),
	m_hpAdd(NULL),
	m_HardItemBg(NULL),
	m_NormalItemBg(NULL),
	m_EasyItemBg(NULL)
{
	for (int i=0; i<POINT_COUNT; ++i)
	{
		m_point[i] = NULL;
		m_tips_[i] = NULL;
	}
}

TowerDifficulty::~TowerDifficulty()
{
	CC_SAFE_RELEASE(m_MissionName);
	CC_SAFE_RELEASE(m_btnHardFight);
	CC_SAFE_RELEASE(m_HardReward);
	//CC_SAFE_RELEASE(m_hardBackground);
	CC_SAFE_RELEASE(m_HardItem);
	CC_SAFE_RELEASE(m_btnNormalFight);
	//CC_SAFE_RELEASE(m_NormalBackground);
	CC_SAFE_RELEASE(m_NormalItem);
	CC_SAFE_RELEASE(m_btnEasyFight);
	CC_SAFE_RELEASE(m_EasyReward);
	//CC_SAFE_RELEASE(m_EasyBackground);
	CC_SAFE_RELEASE(m_EasyItem);
	for (int i=0; i<POINT_COUNT; ++i)
	{
		CC_SAFE_RELEASE(m_point[i]);
		CC_SAFE_RELEASE(m_tips_[i]);
	}
	CC_SAFE_RELEASE(m_HavePower);
	CC_SAFE_RELEASE(m_pass);
	//CC_SAFE_RELEASE(m_unPass);
	CC_SAFE_RELEASE(m_defenseAdd);
	CC_SAFE_RELEASE(m_attackAdd);
	CC_SAFE_RELEASE(m_hpAdd);
	CC_SAFE_RELEASE(m_HardInteral);
	CC_SAFE_RELEASE(m_NormalInteral);
	CC_SAFE_RELEASE(m_EasyInteral);
	CC_SAFE_RELEASE(m_HardItemBg);
	CC_SAFE_RELEASE(m_NormalItemBg);
	CC_SAFE_RELEASE(m_EasyItemBg);
}

bool TowerDifficulty::init()
{
	if (!DLayer::init())
	{
		return false;
	}

    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_huodong/PL_tianshoufuge/pl_tsfg_difficulty");
	
	m_pTowerModel = DM_GET_TOWER_MODEL;

	return true;
}

bool TowerDifficulty::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_MissionName",		CCLabelBMFont *,	m_MissionName);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_HardReward",		CCLabelBMFont *,	m_HardReward);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_HardBackground",	CCScale9Sprite *,	m_hardBackground);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_HardItem",			CCNode *,			m_HardItem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnHardFight",		CCMenuItemImage*,	m_btnHardFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_HardInteral",		CCLabelBMFont *,	m_HardInteral);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_NormalReward",		CCLabelBMFont *,	m_NormalReward);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_NormalBackground",	CCScale9Sprite *,	m_NormalBackground);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_NormalItem",		CCNode *,			m_NormalItem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnNormalFight",	CCMenuItemImage*,	m_btnNormalFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_NormalInteral",	CCLabelBMFont *,	m_NormalInteral);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_EasyReward",		CCLabelBMFont *,	m_EasyReward);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_EasyBackground",	CCScale9Sprite *,   m_EasyBackground);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_EasyItem",			CCNode *,			m_EasyItem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnEasyFight",		CCMenuItemImage*,	m_btnEasyFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_EasyInteral",		CCLabelBMFont *,	m_EasyInteral);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_point0",			CCMenuItemImage *,			m_point[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_point1",			CCMenuItemImage *,			m_point[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_point2",			CCMenuItemImage *,			m_point[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_point3",			CCMenuItemImage *,			m_point[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_point4",			CCMenuItemImage *,			m_point[4]);
    
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tips_0",			CCNode *,			m_tips_[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tips_1",			CCNode *,			m_tips_[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tips_2",			CCNode *,			m_tips_[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tips_3",			CCNode *,			m_tips_[3]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tips_4",			CCNode *,			m_tips_[4]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_HavePower",		CCLabelBMFont*,		m_HavePower);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_attackAdd",		CCLabelBMFont*,		m_attackAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_defenseAdd",		CCLabelBMFont*,		m_defenseAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_hpAdd",		CCLabelBMFont*,		m_hpAdd);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pass",			CCLabelTTF *,			m_pass);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_unPass",			CCSprite *,			m_unPass);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_HardItemBg",			WarriorGradeBg *,	m_HardItemBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_NormalItemBg",			WarriorGradeBg *,	m_NormalItemBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_EasyItemBg",			WarriorGradeBg *,	m_EasyItemBg);

	return false;
}

SEL_MenuHandler TowerDifficulty::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSelectFight", TowerDifficulty::onSelectFight);

	return NULL;
}

void TowerDifficulty::onSelectFight(CCObject *sender)
{
	CCNode *pNode = (CCNode*)sender;
	if (pNode)
	{
		int nTag = pNode->getTag();
		HTTP_CLIENT->towerFight(nTag);
	}
}

void TowerDifficulty::NotificationUpdate()
{
	UpdateInfo();
} 

void TowerDifficulty::UpdateInfo()
{	
	m_EasyReward->setString(CCString::createWithFormat("%d",	1)->getCString());
	m_NormalReward->setString(CCString::createWithFormat("%d",	3)->getCString());
	m_HardReward->setString(CCString::createWithFormat("%d",	5)->getCString());
	m_EasyInteral->setString(CCString::createWithFormat("%d",	1)->getCString());
	m_NormalInteral->setString(CCString::createWithFormat("%d",	3)->getCString());
	m_HardInteral->setString(CCString::createWithFormat("%d",	5)->getCString());

	tower::TowerInfo *pInfo = m_pTowerModel->GetTowerInfo();
	if (!pInfo)
	{
		return;
	}
	m_EasyItem->removeAllChildren();
	m_NormalItem->removeAllChildren();
	m_HardItem->removeAllChildren();
	if (pInfo->item_lv != 1)
	{
		//m_EasyBackground->setVisible(false);		
		m_EasyItem->setVisible(false);
		m_EasyItemBg->setVisible(false);
	}
	else
	{
		//m_EasyBackground->setVisible(true);
		m_EasyItem->setVisible(true);
		m_EasyItemBg->setVisible(true);
		if (pInfo->pItemInfo)
		{
			m_EasyItem->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(pInfo->pItemInfo->getItemId()));
			warrior::WarriorGrade grade = static_cast<warrior::WarriorGrade>(pInfo->pItemInfo->getBaseInfo()->getItemGrade());
			m_EasyItemBg->setWarriorGrade(grade);
		}
	}

	if (pInfo->item_lv != 2)
	{
		//m_NormalBackground->setVisible(false);
		
		m_NormalItem->setVisible(false);
		m_NormalItemBg->setVisible(false);
	}
	else
	{
		//m_NormalBackground->setVisible(true);
		m_NormalItem->setVisible(true);
		m_NormalItemBg->setVisible(true);
		if (pInfo->pItemInfo)
		{
			m_NormalItem->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(pInfo->pItemInfo->getItemId()));
			warrior::WarriorGrade grade = static_cast<warrior::WarriorGrade>(pInfo->pItemInfo->getBaseInfo()->getItemGrade());
			m_NormalItemBg->setWarriorGrade(grade);
		}
	}

	if (pInfo->item_lv != 3)
	{
		//m_hardBackground->setVisible(false);
		m_HardItem->setVisible(false);
		m_HardItemBg->setVisible(false);
	}
	else
	{
		//m_hardBackground->setVisible(true);
		m_HardItem->setVisible(true);
		m_HardItemBg->setVisible(true);
		if (pInfo->pItemInfo)
		{			
			m_HardItem->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(pInfo->pItemInfo->getItemId()));
			warrior::WarriorGrade grade = static_cast<warrior::WarriorGrade>(pInfo->pItemInfo->getBaseInfo()->getItemGrade());
			m_HardItemBg->setWarriorGrade(grade);
		}
	}

	int nPos = pInfo->mid%POINT_COUNT;
	for (int i=0; i<POINT_COUNT; ++i)
	{
		m_point[i]->setEnabled(false);
		if (i == nPos)
		{
			//m_point[i]->setVisible(true);
			m_point[i]->selected();
		}
		else
		{
			//m_point[i]->setVisible(false);
			m_point[i]->unselected();
		}
	}

	//(INT（L/5）*5+1)%3
	if(pInfo->mid % POINT_COUNT > 0)
	{
		for (int i=0; i<POINT_COUNT; ++i)
		{
			int nTmp = ((int)(pInfo->mid/POINT_COUNT)*POINT_COUNT+1+i)%3;
			m_tips_[i]->setVisible(nTmp == 0);
		}
	}
	else
	{
		for (int i=0; i<POINT_COUNT; ++i)
		{
			int nTmp = ((int)(pInfo->mid/POINT_COUNT - 1)*POINT_COUNT+1+i)%3;
			m_tips_[i]->setVisible(nTmp == 0);
		}
	}
	

	m_MissionName->setString(CCString::createWithFormat("%d",pInfo->mid)->getCString());
	m_HavePower->setString(CCString::createWithFormat("%d", pInfo->scores)->getCString());

	if(pInfo->lastHard >= pInfo->mid)
	{
		//m_pass->setVisible(true);
		m_pass->setString(D_LOCAL_STRING("Towerinfo2").c_str());
		//m_unPass->setVisible(false);

	}
	else
	{
		m_pass->setString(D_LOCAL_STRING("Towerinfo1").c_str());
		//m_pass->setVisible(false);
		//m_unPass->setVisible(true);
	}
	
	tower::TowerAdd *pAddInfo = m_pTowerModel->GetTowerAdd();
	m_attackAdd->setString(CCString::createWithFormat("%d%%", (int)(pAddInfo->fAtt * 100))->getCString());
	m_defenseAdd->setString(CCString::createWithFormat("%d%%", (int)(pAddInfo->fDef * 100))->getCString());
	m_hpAdd->setString(CCString::createWithFormat("%d%%", (int)(pAddInfo->fHp * 100))->getCString());
	//end
}

void TowerDifficulty::onEnter()
{
	DLayer::onEnter();
	BINDING_EXEC(m_pTowerModel, this, TowerDifficulty::NotificationUpdate, D_EVENT_TOWER_INFO_UPDATE);
}

void TowerDifficulty::onExit()
{
	UNBINDING_ALL(m_pTowerModel, this);
	DLayer::onExit();
}
