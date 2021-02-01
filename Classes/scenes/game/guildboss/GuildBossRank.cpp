//
//  GuildBossRank.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossRank.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBoss.h"
#include "GuildBossRankCell.h"

using namespace sp;
using namespace guildboss;

GuildBossRank::GuildBossRank()
	: m_pContainer(NULL)
	, m_pPersonRank(NULL)
	, m_pGuildRank(NULL)
	, m_pCloseButton(NULL)
	, m_pDelegate(NULL)
	, m_iSelected(-1)
	, m_eType(GuildBossRankTableType(0))
	, m_pModel(NULL)
	, m_pList(NULL)
	, m_bIsPreparing(false)
	, m_bLastWeek(false)
	, m_pCell(NULL)
	, m_pData(NULL)
{
	D_RESET_C_ARRAY(m_pBtnSwitch, GuildBossRankTableTypeCount);
	D_RESET_C_ARRAY(m_pNodeTitle, GuildBossRankTableTypeCount);
	//D_RESET_C_ARRAY(m_pCell, GuildBossRankTableTypeCount);
	//D_RESET_C_ARRAY(m_pData, GuildBossRankTableTypeCount);
	D_RESET_C_ARRAY(m_pTableView, GuildBossRankTableTypeCount);
}

GuildBossRank::~GuildBossRank()
{
	D_SAFE_RELEASE_C_ARRAY(m_pBtnSwitch, GuildBossRankTableTypeCount);
	D_SAFE_RELEASE_C_ARRAY(m_pNodeTitle, GuildBossRankTableTypeCount);
	//D_SAFE_RELEASE_C_ARRAY(m_pCell, GuildBossRankTableTypeCount);
	//D_SAFE_RELEASE_C_ARRAY(m_pData, GuildBossRankTableTypeCount);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pGuildRank);
	CC_SAFE_RELEASE(m_pPersonRank);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pData);
}

GuildBossRank* GuildBossRank::create()
{
	GuildBossRank *pRet = new GuildBossRank();
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

bool GuildBossRank::init()
{
	if(!DPopup::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILDBOSS_MODEL;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_bangpaipaihangbang");
	
	CCB_READER("PL_ui/ccb/pl_banghui/pl_bangpaipaihangbang_cell", m_pCell);
	CC_SAFE_RETAIN(m_pCell);

	m_pData = readCCData("PL_ui/ccb/pl_banghui/pl_bangpaipaihangbang_cell");
	CC_SAFE_RETAIN(m_pData);

	m_pGuildRank->setVisible(true);
	m_pPersonRank->setVisible(false);
	m_pCloseButton->getButton()->setTarget(this , menu_selector(GuildBossRank::callBackClose));

	DLoadingTableView *pLoadingView = DLoadingTableView::create(this, m_pContainer->getContentSize());
	pLoadingView->setVerticalFillOrder(kCCTableViewFillTopDown);
	pLoadingView->setLoadingDelegate(this);
	pLoadingView->setPageSize(GUILDBOSS_GUILD_RANK_PAGE_SIZE);
	pLoadingView->setLoadingEnabled(true);
	m_pContainer->addChild(pLoadingView);
	m_pTableView[0] = pLoadingView;
	m_pTableView[1] = DTableView::create(this, m_pContainer->getContentSize());
	m_pTableView[1]->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pContainer->addChild(m_pTableView[1]);


	for (int i = 0; i < GuildBossRankTableTypeCount; i++)
	{
		m_pBtnSwitch[i]->getButton()->setTarget(this, menu_selector(GuildBossRank::callBackSelect));
		m_pBtnSwitch[i]->getButton()->setTag(i);
	}

	refresh();

	return true;
}

void GuildBossRank::onEnter()
{
	BINDING_EXEC(m_pModel, this, GuildBossRank::updateView, D_EVENT_GUILDBOSS_GET_RANK);
	DPopup::onEnter();
}

void GuildBossRank::onExit()
{
	DPopup::onExit();
	UNBINDING_ALL(m_pModel, this);
}

bool GuildBossRank::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuildRank", CCNode *, m_pGuildRank);	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPersonRank", CCNode *, m_pPersonRank);	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);	D_CCB_ASSIGN_ARRAY("m_pBtnSwitch", DButton *, m_pBtnSwitch, GuildBossRankTableTypeCount);
	D_CCB_ASSIGN_ARRAY("m_pNodeTitle", CCNode *, m_pNodeTitle, GuildBossRankTableTypeCount);
	return false;
}

cocos2d::SEL_MenuHandler GuildBossRank::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", GuildBossRank::callBackClose);
	return NULL;
}

#pragma mark CCTableViewDataSource
CCTableViewCell* GuildBossRank::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildBossRankCell *cell = (GuildBossRankCell *)table->dequeueCell();
	if (!cell)
	{
		guildboss::GuildBossRankType eRankType = guildboss::GuildBossRankTypeGuild;
		switch(m_eType)
		{
		case GuildBossRankTableTypeGuild:
			eRankType = guildboss::GuildBossRankTypeGuild;
			break;
		case GuildBossRankTableTypeHatred:
			eRankType = guildboss::GuildBossRankTypeHatred;
			break;
		default:
			break;
		}
		cell = GuildBossRankCell::create(eRankType, m_pData);
	}
	cell->setData(m_pList->objectAtIndex(idx), idx + 1, !m_bIsPreparing, m_bLastWeek);
	return cell;
}

uint32_t GuildBossRank::numberOfCellsInTableView(CCTableView *table)
{
	if (!m_pList)
	{
		return 0;
	}
	return m_pList->count();
}

CCSize GuildBossRank::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
}

#pragma mark CCTableViewDelegate
void GuildBossRank::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

void GuildBossRank::callBackClose(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavNoPop);
	}
	closePopup();
}

void GuildBossRank::setType(GuildBossRankTableType eType)
{
	m_eType = eType;
	switch (m_eType)
	{
	case 0:
		m_pModel->syncGuildRank();
		break;
	case 1:
		m_pModel->syncHatred();
		break;
	default:;
	};
	refresh();
}

void GuildBossRank::callBackSelect(CCObject *pSender)
{
	CCMenuItem *pButton = dynamic_cast<CCMenuItem *>(pSender);
	if (m_eType == pButton->getTag())
	{
		pButton->selected();
		return;
	}
	setType((GuildBossRankTableType)pButton->getTag());
}

void GuildBossRank::refresh()
{
	for (int i = 0; i < GuildBossRankTableTypeCount; i++)
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

void GuildBossRank::updateView()
{
	uint32_t uServerTime = GameManager::sharedGameManager()->getServerTime();
	m_bIsPreparing = uServerTime >= m_pModel->getConfig()->getApplyDeadline() && uServerTime <= m_pModel->getConfig()->getStartTime();
	switch (m_eType)
	{
	case 1:
		m_pList = m_pModel->getHatredGuilds();
		break;
	case 0:
		m_pList = m_pModel->getGuildRank();
		break;
	default:;
	};
	m_bLastWeek = m_pModel->isShowingLastWeek();
	m_pTableView[(int)m_eType]->reloadData();

	if (GuildBossRankTableTypeGuild == m_eType)
	{
		DLoadingTableView *pLoadingView = dynamic_cast<DLoadingTableView *>(m_pTableView[0]);
		pLoadingView->endLoading();
	}
}


void GuildBossRank::startLoading()
{
	m_pModel->syncGuildRank(true);
}