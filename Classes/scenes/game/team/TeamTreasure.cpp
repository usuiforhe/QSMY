//
//  TeamTreasure.h
//  组队可能掉落查看
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#define UNSELECT -1

#include "TeamTreasure.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"

TeamTreasure::TeamTreasure()
	: m_pDBtnConfirm(NULL)
	, m_pDelegate(NULL)
	, m_pModel(NULL)
{
}

TeamTreasure::~TeamTreasure()
{
	CC_SAFE_RELEASE(m_pDBtnConfirm);
}

TeamTreasure* TeamTreasure::create()
{
	TeamTreasure *pRet = new TeamTreasure();
	if (pRet && pRet->initTreasure())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool TeamTreasure::initTreasure()
{
	m_pModel = DM_GET_FRIENDS_MODEL;

	CCArray *pAwardList = CCArray::create();
	CCArray *pDropList = m_pModel->getSelectedChapter()->getDropList();
	for (int i = 0; i < pDropList->count(); i++)
	{
		friends::DropInfo *pInfo = static_cast<friends::DropInfo *>(pDropList->objectAtIndex(i));
		drop::Drop *pDrop = NULL;

		uint32_t dropID = pInfo->getDropID();

		switch (pInfo->getDropType())
		{
		case drop::kDropTypeItem:
		case drop::kDropTypePkg:
			pDrop = drop::Drop::create(item::ItemInfo::create(dropID, 1));
			break;
		case drop::kDropTypeEquip:
			pDrop = drop::Drop::create(equip::EquipCurInfo::create(dropID));
			break;
		case drop::kDropTypeSoul:
			pDrop = drop::Drop::create(warrior::SoulInfo::create(dropID, 1));
			break;
		case drop::kDropTypeWarrior:
			pDrop = drop::Drop::create(warrior::WarriorCurInfo::create(dropID));
			break;
		case drop::kDropTypeAvatar:
			pDrop = drop::Drop::create(avatar::AvatarInfo::create(dropID));
			break;
		default:
			break;
		}
		if (pDrop)
		{
			pAwardList->addObject(pDrop);
		}
	}

	return BaseAwardListView::init(pAwardList);
}



void TeamTreasure::addChildrenBeforeList()
{
	m_pBg->setOpacity(0);
	CCB_READER_AND_ADDCHILD("ui/ccb/team/teamTreasure");

	m_pDBtnConfirm->getButton()->setTarget(this, menu_selector(TeamTreasure::callBackConfirm));

}


void TeamTreasure::onEnter()
{
	BaseAwardListView::onEnter();
}

void TeamTreasure::updateView()
{
}

void TeamTreasure::onExit()
{
	BaseAwardListView::onExit();
}

bool TeamTreasure::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnConfirm", DButton *, m_pDBtnConfirm);
	return BaseAwardListView::onAssignCCBMemberVariable(pTarget,pMemberVariableName,pNode);
}

void TeamTreasure::callBackConfirm(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamNavigateTo(kTeamNavNoPop);
	}
}