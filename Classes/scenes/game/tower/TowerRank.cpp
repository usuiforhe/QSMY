//
//  TowerRank.cpp
//  千机楼-排行榜
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TowerRank.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "model/TowerModel.h"

#include "network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

TowerRank::TowerRank()
:m_btnBack(NULL)
//,m_UserRank(NULL)
,m_pView(NULL)
,m_tabViewLayer(NULL)
,m_pCCData(NULL)
,m_pArrCells(NULL)
,m_pDelegate(NULL)
,m_pRankBtn(NULL)
,m_pNowTitle(NULL)
,m_pNowTxt(NULL)
,m_pOldTitle(NULL)
,m_pOldTxt(NULL)
,isNowRank(false)
{}

TowerRank::~TowerRank()
{
	CC_SAFE_RELEASE(m_btnBack);
	//CC_SAFE_RELEASE(m_UserRank);
	CC_SAFE_RELEASE(m_tabViewLayer);
	CC_SAFE_RELEASE(m_pCCData);
	CC_SAFE_RELEASE(m_pRankBtn);
	CC_SAFE_RELEASE(m_pNowTxt);
	CC_SAFE_RELEASE(m_pNowTitle);
	CC_SAFE_RELEASE(m_pOldTitle);
	CC_SAFE_RELEASE(m_pOldTxt);
}

#define TOWER_RANK_SCROLL_SIZE	CCSizeMake(620.f, 580)
#define TOWER_RANK_CELL_SIZE	CCSizeMake(618.f, 150.f)

bool TowerRank::init()
{
	if (!DLayer::init())
	{
		return false;
	}

    m_pTowerModel = DM_GET_TOWER_MODEL;
    
    m_pCCData = readCCData("PL_ui/ccb/pl_huodong/PL_tianshoufuge/pl_tsfg_rank_cell");
	CC_SAFE_RETAIN(m_pCCData);
    
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_huodong/PL_tianshoufuge/pl_tsfg_rank");

	m_pView = DTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_tabViewLayer->getContentSize(), -50));
	m_pView->setDirection(kCCScrollViewDirectionVertical);
	m_pView->setDelegate(this);
	m_pView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_tabViewLayer->addChild(m_pView);

	m_pRankBtn->setTarget(this, menu_selector(TowerRank::onRankShow));
	return true;
}

bool TowerRank::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnBack",		CCMenuItemImage *,	m_btnBack);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_UserRank",		CCLabelBMFont *,	m_UserRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tabViewLayer",	CCNode *,			m_tabViewLayer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRankBtn" , CCMenuItemImage*, m_pRankBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNowTxt" , CCNode*, m_pNowTxt);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNowTitle" , CCNode*, m_pNowTitle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOldTxt" , CCNode*, m_pOldTxt);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOldTitle" , CCNode*, m_pOldTitle);
	return false;
}

SEL_MenuHandler TowerRank::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchBack", TowerRank::onTouchBack);

	return NULL;
}

void TowerRank::onTouchBack(CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->ClosePopup();
	}
}

void TowerRank::onRankShow(CCObject *sender)
{
	isNowRank =!isNowRank;
	if(isNowRank)
	{
		m_pTowerModel->updateTowerRank(false);
	}
	else
	{
		m_pTowerModel->updateTowerOldRank();
	}

	updateView();
}

void TowerRank::setRankState()
{
	m_pOldTitle->setVisible(!isNowRank);
	m_pOldTxt->setVisible(isNowRank);
	m_pNowTitle->setVisible(isNowRank);
	m_pNowTxt->setVisible(!isNowRank);
}

void TowerRank::updateView()
{
	setRankState();
	if(isNowRank)
	{
		m_pArrCells = m_pTowerModel->getRanks();
	}
	else
	{
		 m_pArrCells = m_pTowerModel->getOldRanks();
	}
   
	//m_UserRank->setString(CCString::createWithFormat("%d", m_pTowerModel->getMyRank())->getCString());
	m_pView->reloadData();
	m_pView->setContentOffset(m_pView->minContainerOffset());
}

void TowerRank::onEnter()
{
    BINDING_EXEC(m_pTowerModel, this, TowerRank::updateView, D_EVENT_TOWER_OLD_RANK_UPDATE);
	BINDING_EXEC(m_pTowerModel, this, TowerRank::updateView, D_EVENT_TOWER_RANK_UPDATE);
	isNowRank = true;
	m_pTowerModel->updateTowerRank(true);
	updateView();
    DLayer::onEnter();
}

void TowerRank::onExit()
{
    UNBINDING_ALL(m_pTowerModel, this);
	DLayer::onExit();
}

void TowerRank::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}

CCSize TowerRank::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return TOWER_RANK_CELL_SIZE;
}

CCTableViewCell* TowerRank::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	TowerRankCell *cell = (TowerRankCell*)table->dequeueCell();
	if (!cell)
	{
		cell = TowerRankCell::create(this, m_pCCData);
	}
    cell->setDelegate(m_pDelegate);
    cell->updateCell((tower::TowerRankInfo*)m_pArrCells->objectAtIndex(idx));
	return cell;
}

uint32_t TowerRank::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return m_pArrCells?m_pArrCells->count():0;
}



TowerRankCell::TowerRankCell()
:m_lookUserInfo(NULL)
,m_rankNum(NULL)
//,m_userLev(NULL)
,m_todayTotal(NULL)
,m_userName(NULL)
,m_allTotal(NULL)
,m_pRank(NULL)
,m_userimg(NULL)
,m_pDelegate(NULL)
{}

TowerRankCell::~TowerRankCell()
{
	CC_SAFE_RELEASE(m_lookUserInfo);
	CC_SAFE_RELEASE(m_rankNum);
	//CC_SAFE_RELEASE(m_userLev);
	CC_SAFE_RELEASE(m_todayTotal);
	CC_SAFE_RELEASE(m_userName);
	CC_SAFE_RELEASE(m_allTotal);
	CC_SAFE_RELEASE(m_userimg);	
}

bool TowerRankCell::init(TowerRank *pRank, CCData* pCCData)
{	
	if (!pRank)
	{
		return false;
	}

	if (!pCCData)
	{
		return false;
	}

	m_pRank = pRank;
	CCB_READER_DATA_AND_ADDCHILD(pCCData);
    
    m_lookUserInfo->getButton()->setTarget(this, menu_selector(TowerRankCell::onLookUser));

	return true;
}

TowerRankCell* TowerRankCell::create(TowerRank *pRank, CCData* pCCData)
{
	TowerRankCell *pRet = new TowerRankCell();
	if (pRet && pRet->init(pRank, pCCData))
	{
		pRet->autorelease();
		return pRet;
	}

	delete pRet;
	return NULL;
}

bool TowerRankCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_lookUserInfo",	DButton *,	m_lookUserInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_rankNum",		CCLabelBMFont *,	m_rankNum);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_userLev",		CCLabelBMFont *,	m_userLev);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_todayTotal",	CCLabelBMFont *,	m_todayTotal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_allTotal",		CCLabelBMFont *,	m_allTotal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_userName",		CCLabelTTF *,		m_userName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer",		CCNode *,			m_userimg);
	
	return false;
}

//SEL_MenuHandler TowerRankCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
//{
//	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onLookUser", TowerRankCell::onLookUser);
//
//	return NULL;
//}

void TowerRankCell::updateCell(const tower::TowerRankInfo *towerRankInfo)
{
	m_lookUserInfo->getButton()->setUserData((void*)towerRankInfo);
	m_rankNum->setString(CCString::createWithFormat("%d", towerRankInfo->m_Pos)->getCString());
	//m_userLev->setString(CCString::createWithFormat("%d", towerRankInfo->m_Lev)->getCString());
	m_allTotal->setString(CCString::createWithFormat("%d", towerRankInfo->m_totalNum)->getCString());
	m_todayTotal->setString(CCString::createWithFormat("%d", towerRankInfo->m_todayNum)->getCString());
	m_userName->setString(CCString::createWithFormat("%d %s", towerRankInfo->m_Lev, towerRankInfo->m_strName.c_str())->getCString());
	
    m_userimg->removeAllChildren();
    CCSprite * pLeader = ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(towerRankInfo->getAvatarId());
	m_userimg->addChild(pLeader);
}

void TowerRankCell::onLookUser(CCObject* pObj)
{
	const tower::TowerRankInfo *pInfo = (const tower::TowerRankInfo*)(m_lookUserInfo->getButton()->getUserData());
	if (pInfo && m_pDelegate)
	{
		m_pDelegate->showOtherFormation(DM_GET_OTHERFORMATION_MODEL->getForamtionInfoByUid(pInfo->m_id));
	}
}

