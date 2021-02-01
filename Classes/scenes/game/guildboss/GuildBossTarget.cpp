//
//  GuildBossTarget.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossTarget.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBoss.h"
#include "GuildBossTargetCell.h"

using namespace sp;

GuildBossTarget::GuildBossTarget()
	: m_pContainer(NULL)
	, m_pDBtnSubmit(NULL)
	, m_pCloseButton(NULL)
	, m_pDelegate(NULL)
	, m_pCell(NULL)
	, m_pTableView(NULL)
	, m_iSelected(-1)
	, m_pList(NULL)
	, m_pModel(NULL)
	, m_bLastWeek(false)
{
}

GuildBossTarget::~GuildBossTarget()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pDBtnSubmit);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pCell);
}

GuildBossTarget* GuildBossTarget::create()
{
	GuildBossTarget *pRet = new GuildBossTarget();
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

bool GuildBossTarget::init()
{
	if(!DPopup::init())
	{
		return false;
	}


	m_pModel = DM_GET_GUILDBOSS_MODEL;
	
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_xuanzhan");
	
	CCB_READER("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_xz_cell", m_pCell);
	CC_SAFE_RETAIN(m_pCell);

	m_pTableView = DLoadingTableView::create(this, m_pContainer->getContentSize());
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pTableView->setLoadingDelegate(this);
	m_pTableView->setPageSize(GUILDBOSS_GUILD_RANK_PAGE_SIZE);
	m_pContainer->addChild(m_pTableView);

	//m_pDBtnSubmit->getButton()->setTarget(this, menu_selector(GuildBossTarget::callBackSubmit));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(GuildBossTarget::callBackClose));

	return true;
}

void GuildBossTarget::onEnter()
{
	BINDING_ONLY(m_pModel, this, GuildBossTarget::updateView, D_EVENT_GUILDBOSS_GET_TARGETS);
	m_pModel->syncTargets();
	DPopup::onEnter();
}

void GuildBossTarget::onExit()
{
	DPopup::onExit();
	UNBINDING_ALL(m_pModel, this);
}

bool GuildBossTarget::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnSubmit", DButton *, m_pDBtnSubmit);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);
	return false;
}

cocos2d::SEL_MenuHandler GuildBossTarget::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", GuildBossTarget::callBackClose);
	return NULL;
}

#pragma mark CCTableViewDataSource
CCTableViewCell* GuildBossTarget::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildBossTargetCell *cell = (GuildBossTargetCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildBossTargetCell::create();
		cell->setDelegate(this);
	}
	cell->setSelected(m_iSelected == idx);
	guildboss::GuildBossBasic *pData = dynamic_cast<guildboss::GuildBossBasic *>(m_pList->objectAtIndex(idx));
	cell->setData(pData);
	cell->setRank(idx + 1);
	cell->setLastWeek(m_bLastWeek);
	return cell;
}

uint32_t GuildBossTarget::numberOfCellsInTableView(CCTableView *table)
{
	if (!m_pList)
	{
		return 0;
	}
	
	return m_pList->count();
}

CCSize GuildBossTarget::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
}

#pragma mark CCTableViewDelegate
void GuildBossTarget::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

#pragma mark GuildBossTargetCellDelegate
void GuildBossTarget::GuildBossTargetSelect(GuildBossTargetCell *pNode)
{
	int oldIndex = m_iSelected;
	m_iSelected = pNode->getIdx();
	if (oldIndex != m_iSelected)
	{
		m_pTableView->updateCellAtIndex(oldIndex);
	}
}

void GuildBossTarget::callBackClose(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavHome);
	}
	closePopup();
}

void GuildBossTarget::callBackSubmit(CCObject *pSender)
{
	if (m_pDelegate)
	{
		if (m_iSelected >= 0)
		{
			guildboss::GuildBossBasic *pTarget = dynamic_cast<guildboss::GuildBossBasic *>(m_pList->objectAtIndex(m_iSelected));
			m_pDelegate->GuildBossTargetSubmit(this, pTarget);
		}
		else
		{
			m_pDelegate->GuildBossNavigateTo(GuildBossNavHome);
		}
	}
	
}


void GuildBossTarget::updateView()
{
	m_pList = m_pModel->getTargets();
	m_bLastWeek = m_pModel->isShowingLastWeek();
	m_pTableView->reloadData();
	m_pTableView->endLoading();
}


void GuildBossTarget::startLoading()
{
	m_pModel->syncTargets(true);
}