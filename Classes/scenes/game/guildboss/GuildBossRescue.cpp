//
//  GuildBossRescue.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossRescue.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBoss.h"
#include "GuildBossRescueCell.h"

using namespace sp;

GuildBossRescue::GuildBossRescue()
	: m_pContainer(NULL)
	, m_pDBtnBack(NULL)
	, m_pDBtnAll(NULL)
	, m_pCloseButton(NULL)
	, m_pBMFGoldAll(NULL)
	, m_pDelegate(NULL)
	, m_pCell(NULL)
	, m_pTableView(NULL)
	, m_iSelected(-1)
	, m_pModel(NULL)
	, m_pList(NULL)
{
}

GuildBossRescue::~GuildBossRescue()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pDBtnBack);
	CC_SAFE_RELEASE(m_pDBtnAll);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pBMFGoldAll);
	CC_SAFE_RELEASE(m_pCell);
}

GuildBossRescue* GuildBossRescue::create()
{
	GuildBossRescue *pRet = new GuildBossRescue();
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

bool GuildBossRescue::init()
{
	if(!DPopup::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILDBOSS_MODEL;

	m_pList = m_pModel->getWoundedUsers();
	
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_jiuzhu");

	CCB_READER("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_jz_cell", m_pCell);
	CC_SAFE_RETAIN(m_pCell);

	m_pTableView = DTableView::create(this, m_pContainer->getContentSize());
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pContainer->addChild(m_pTableView);

	m_pDBtnBack->getButton()->setTarget(this, menu_selector(GuildBossRescue::callBackClose));
	m_pDBtnAll->getButton()->setTarget(this, menu_selector(GuildBossRescue::callBackAll));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(GuildBossRescue::callBackClose));
	return true;
}

void GuildBossRescue::onEnter()
{
	m_pModel->syncWounded();
	BINDING_EXEC(m_pModel, this, GuildBossRescue::updateView, D_EVENT_GUILDBOSS_WOUNDED_LIST);
	DPopup::onEnter();
}

void GuildBossRescue::onExit()
{
	DPopup::onExit();
	UNBINDING_ALL(m_pModel, this);
}

bool GuildBossRescue::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnBack", DButton *, m_pDBtnBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnAll", DButton *, m_pDBtnAll);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFGoldAll", CCLabelBMFont *, m_pBMFGoldAll);
	return false;
}

cocos2d::SEL_MenuHandler GuildBossRescue::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", GuildBossRescue::callBackClose);
	return NULL;
}

#pragma mark CCTableViewDataSource
CCTableViewCell* GuildBossRescue::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildBossRescueCell *cell = (GuildBossRescueCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildBossRescueCell::create();
		cell->setDelegate(this);
	}
	cell->setData((guildboss::GuildWoundedUser *)m_pList->objectAtIndex(idx));
	return cell;

}

uint32_t GuildBossRescue::numberOfCellsInTableView(CCTableView *table)
{
	if (!m_pList)
	{
		return 0;
	}
	return m_pList->count();
}

CCSize GuildBossRescue::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
}

#pragma mark CCTableViewDelegate
void GuildBossRescue::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

#pragma mark GuildBossRescueCellDelegate
void GuildBossRescue::GuildBossRescueUser(GuildBossRescueCell *pNode, uint32_t userID)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossRescueUser(pNode, userID);
	}

}

void GuildBossRescue::callBackClose(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavNoPop);
	}
	closePopup();
}


void GuildBossRescue::callBackAll(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossRescueAll(this, m_pModel->getConfig()->getRescueCost() * m_pList->count());
	}
}

void GuildBossRescue::updateView()
{
	m_pList = m_pModel->getWoundedUsers();
	m_pTableView->reloadData();

	uint32_t gold = 0;
	if (m_pList)
	{
		gold = m_pModel->getConfig()->getRescueCost() * m_pList->count();
	}
	m_pBMFGoldAll->setString(D_CSTRING_FROM_UINT(gold));
}