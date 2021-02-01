//
//  GuildBossRankMember.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossRankMember.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBoss.h"
#include "GuildBossRankCell.h"

using namespace sp;
using namespace guildboss;

GuildBossRankMember::GuildBossRankMember()
	: m_pContainer(NULL)
	, m_pPersonRank(NULL)
	, m_pGuildRank(NULL)
	, m_pCloseButton(NULL)
	, m_pDelegate(NULL)
	, m_iSelected(-1)
	, m_eType((guildboss::GuildBossRankMemberTableType)0)
	, m_pModel(NULL)
	, m_pList(NULL)
	, m_bIsPreparing(false)
	, m_bLastWeek(false)
	, m_pCell(NULL)
	, m_pData(NULL)
{
	D_RESET_C_ARRAY(m_pBtnSwitch, guildboss::GuildBossRankMemberTableTypeCount);
	D_RESET_C_ARRAY(m_pNodeTitle, guildboss::GuildBossRankMemberTableTypeCount);
	//D_RESET_C_ARRAY(m_pCell, guildboss::GuildBossRankMemberTableTypeCount);
	//D_RESET_C_ARRAY(m_pData, guildboss::GuildBossRankMemberTableTypeCount);
	D_RESET_C_ARRAY(m_pTableView, guildboss::GuildBossRankMemberTableTypeCount);
}

GuildBossRankMember::~GuildBossRankMember()
{
	D_SAFE_RELEASE_C_ARRAY(m_pBtnSwitch, guildboss::GuildBossRankMemberTableTypeCount);
	D_SAFE_RELEASE_C_ARRAY(m_pNodeTitle, guildboss::GuildBossRankMemberTableTypeCount);
	//D_SAFE_RELEASE_C_ARRAY(m_pCell, guildboss::GuildBossRankMemberTableTypeCount);
	//D_SAFE_RELEASE_C_ARRAY(m_pData, guildboss::GuildBossRankMemberTableTypeCount);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pGuildRank);
	CC_SAFE_RELEASE(m_pPersonRank);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pData);
}

GuildBossRankMember* GuildBossRankMember::create()
{
	GuildBossRankMember *pRet = new GuildBossRankMember();
	if (pRet && pRet->init())
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

bool GuildBossRankMember::init()
{
	if(!DPopup::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILDBOSS_MODEL;

	//CCB_READER_AND_ADDCHILD("ui/ccb/guildBoss/guildBossHelpTimes");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_bangpaipaihangbang");

	CCB_READER("PL_ui/ccb/pl_banghui/pl_gongxianpaihang_cell", m_pCell);
	CC_SAFE_RETAIN(m_pCell);

	m_pData = readCCData("PL_ui/ccb/pl_banghui/pl_gongxianpaihang_cell");
	CC_SAFE_RETAIN(m_pData);

	m_pGuildRank->setVisible(false);
	m_pPersonRank->setVisible(true);
	m_pCloseButton->getButton()->setTarget(this , menu_selector(GuildBossRankMember::callBackClose));

	m_pTableView[0] = DTableView::create(this, m_pContainer->getContentSize());
	m_pTableView[0]->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pContainer->addChild(m_pTableView[0]);
	m_pTableView[1] = DTableView::create(this, m_pContainer->getContentSize());
	m_pTableView[1]->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pContainer->addChild(m_pTableView[1]);

	for (int i = 0; i < guildboss::GuildBossRankMemberTableTypeCount; i++)
	{
		m_pBtnSwitch[i]->getButton()->setTarget(this, menu_selector(GuildBossRankMember::callBackSelect));
		m_pBtnSwitch[i]->getButton()->setTag(i);
	}

	refresh();

	return true;
}

void GuildBossRankMember::onEnter()
{
	BINDING_EXEC(m_pModel, this, GuildBossRankMember::updateView, D_EVENT_GUILDBOSS_GET_RANK);
	DPopup::onEnter();
}

void GuildBossRankMember::onExit()
{
	DPopup::onExit();
	UNBINDING_ALL(m_pModel, this);
}

bool GuildBossRankMember::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuildRank", CCNode *, m_pGuildRank);	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPersonRank", CCNode *, m_pPersonRank);	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);	D_CCB_ASSIGN_ARRAY("m_pBtnSwitch", DButton *, m_pBtnSwitch, guildboss::GuildBossRankMemberTableTypeCount);
	D_CCB_ASSIGN_ARRAY("m_pNodeTitle", CCNode *, m_pNodeTitle, guildboss::GuildBossRankMemberTableTypeCount);
	return false;
}

cocos2d::SEL_MenuHandler GuildBossRankMember::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", GuildBossRankMember::callBackClose);
	return NULL;
}

#pragma mark CCTableViewDataSource
CCTableViewCell* GuildBossRankMember::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildBossRankCell *cell = (GuildBossRankCell *)table->dequeueCell();
	if (!cell)
	{
		guildboss::GuildBossRankType eRankType = guildboss::GuildBossRankTypeContribution;
		switch(m_eType)
		{
		case GuildBossRankMemberTableTypeContribution:
			eRankType = guildboss::GuildBossRankTypeContribution;
			break;
		case GuildBossRankMemberTableTypeRescue:
			eRankType = guildboss::GuildBossRankTypeRescue;
			break;
		default:
			break;
		}
		cell = GuildBossRankCell::create(eRankType, m_pData);
	}
	cell->setData(m_pList->objectAtIndex(idx), idx + 1, !m_bIsPreparing, m_bLastWeek);
	return cell;
}

uint32_t GuildBossRankMember::numberOfCellsInTableView(CCTableView *table)
{
	if (!m_pList)
	{
		return 0;
	}
	return m_pList->count();
}

CCSize GuildBossRankMember::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
}

#pragma mark CCTableViewDelegate
void GuildBossRankMember::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

void GuildBossRankMember::callBackClose(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavNoPop);
	}
	closePopup();
}

void GuildBossRankMember::setType(guildboss::GuildBossRankMemberTableType eType)
{
	m_eType = eType;
	switch (m_eType)
	{
	case guildboss::GuildBossRankMemberTableTypeContribution:
		m_pModel->syncMemberRank();
		break;
	case guildboss::GuildBossRankMemberTableTypeRescue:
		m_pModel->syncMemberRank();
		break;
	default:;
	};
	refresh();
}

void GuildBossRankMember::callBackSelect(CCObject *pSender)
{
	CCMenuItem *pButton = dynamic_cast<CCMenuItem *>(pSender);
	if (m_eType == pButton->getTag())
	{
		pButton->selected();
		return;
	}
	
	setType((guildboss::GuildBossRankMemberTableType)pButton->getTag());
}

void GuildBossRankMember::refresh()
{
	for (int i = 0; i < guildboss::GuildBossRankMemberTableTypeCount; i++)
	{
		if (m_eType == i)
		{
			m_pBtnSwitch[i]->getButton()->selected();
		}
		else
		{
			m_pBtnSwitch[i]->getButton()->unselected();
		}
		//m_pNodeTitle[i]->setVisible(m_eType == i);
		m_pTableView[i]->setVisible(m_eType == i);
	}
}

void GuildBossRankMember::updateView()
{
	uint32_t uServerTime = GameManager::sharedGameManager()->getServerTime();
	m_bIsPreparing = uServerTime >= m_pModel->getConfig()->getApplyDeadline() && uServerTime <= m_pModel->getConfig()->getStartTime();
	switch (m_eType)
	{
	case guildboss::GuildBossRankMemberTableTypeContribution:
		m_pList = m_pModel->getMemberRank();
		break;
	case guildboss::GuildBossRankMemberTableTypeRescue:
		m_pList = m_pModel->getRescueRank();
		break;
	default:;
	};
	m_bLastWeek = m_pModel->isShowingLastWeek();
	m_pTableView[(int)m_eType]->reloadData();
}


void GuildBossRankMember::startLoading()
{
	m_pModel->syncGuildRank(true);
}