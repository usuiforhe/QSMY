//
//  SoulHunterFail.cpp
//  狩魂战斗胜利却未抢到魂魄界面
//
//  Created by liuxiaogang  on 13-10-26
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoulHunterFail.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  AWARD_CELL_SIZE    CCSizeMake(600, 220)
#define  CELL_ITEM_NUM		3
#define SNATCH_LOWEST_POWER 0

SoulHunterFail::SoulHunterFail()
:m_pAwardContainer(NULL)
,m_pBackButton(NULL)
,m_pAgainButton(NULL)
,m_pListView(NULL)
,m_pAwardList(NULL)
,m_pCCData(NULL)
{
}


SoulHunterFail::~SoulHunterFail()
{
	CC_SAFE_RELEASE(m_pAwardContainer);
	CC_SAFE_RELEASE(m_pBackButton);
	CC_SAFE_RELEASE(m_pAgainButton);
	CC_SAFE_RELEASE(m_pListView);
	CC_SAFE_RELEASE(m_pAwardList);
	CC_SAFE_RELEASE(m_pCCData);
}


SoulHunterFail*  SoulHunterFail::create()
{
	SoulHunterFail* pRet = new SoulHunterFail();
	if(pRet&& pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}


bool SoulHunterFail::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !DLayer::init());
		CCB_READER_AND_ADDCHILD("ui/ccb/soulHunter/soulHunterFail");
		bRet = true;
	}while(0);
    
	m_pCCData = readCCData("ui/ccb/common/awardCell");
	CC_SAFE_RETAIN(m_pCCData);
    
	m_pListView = DTableView::create(this, m_pAwardContainer->getContentSize());
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setAnchorPoint(CCPointZero);
	CC_SAFE_RETAIN(m_pListView);

	m_pAwardContainer->addChild(m_pListView);

	return bRet;
}


void SoulHunterFail::onEnter()
{
	BINDING_EXEC(DM_GET_SOUL_HUNTER_MODEL, this, SoulHunterFail::updateView, D_EVENT_SOULHUNTER_SNATCH_FINISH);
	BINDING_ONLY(DM_GET_SOUL_HUNTER_MODEL, this, SoulHunterFail::removeFromParent, D_EVENT_SOULHUNTER_NOT_EXIST);
	m_pBackButton->getButton()->setTarget(this, menu_selector(SoulHunterFail::closeCallBack));
	m_pAgainButton->getButton()->setTarget(this, menu_selector(SoulHunterFail::againCallBack));
	DLayer::onEnter();
}


void SoulHunterFail::updateView()
{
    cocos2d::CCArray *pAwardList = DM_GET_SOUL_HUNTER_MODEL->getHunterDrops();
	CC_SAFE_RETAIN(pAwardList);
	CC_SAFE_RELEASE(m_pAwardList);
	m_pAwardList = pAwardList;
	m_pListView->reloadData();
}


void SoulHunterFail::onExit()
{
	UNBINDING_ALL(DM_GET_SOUL_HUNTER_MODEL, this);
	DLayer::onExit();
}


bool SoulHunterFail::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pAwardContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackButton", DButton *, this->m_pBackButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAgainButton", DButton *, this->m_pAgainButton);
	return false;
}



CCSize SoulHunterFail::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return AWARD_CELL_SIZE;
}


CCTableViewCell* SoulHunterFail::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	if (!m_pAwardList) return NULL;

	AwardListViewCell *cell = (AwardListViewCell*)table->dequeueCell();
	CCObject *pObject0 = NULL;
	CCObject *pObject1 = NULL;
	CCObject *pObject2 = NULL;
	CCArray * pAwardList = NULL;
	uint32_t count = m_pAwardList->count();

	if (count>CELL_ITEM_NUM*idx)
	{
		pObject0 = m_pAwardList->objectAtIndex(CELL_ITEM_NUM*idx);
	}

	if (count>CELL_ITEM_NUM*idx+1)
	{
		pObject1 = m_pAwardList->objectAtIndex(CELL_ITEM_NUM*idx+1);
	}

	if (count>CELL_ITEM_NUM*idx+2)
	{
		pObject2 = m_pAwardList->objectAtIndex(CELL_ITEM_NUM*idx+2);
	}

	if (!cell)
	{
		cell = AwardListViewCell::create(m_pCCData);
	}

	pAwardList= CCArray::create(pObject0,pObject1,pObject2,NULL);
	cell->setAwardItems(pAwardList);

	return cell;

}

uint32_t SoulHunterFail::numberOfCellsInTableView(CCTableView *table)
{
	if(!m_pAwardList) return 0;

	uint32_t cellNum =  (m_pAwardList->count() + CELL_ITEM_NUM - 1)/CELL_ITEM_NUM;
	return cellNum;
}


void SoulHunterFail::closeCallBack(cocos2d::CCObject* sender)
{
	removeFromParent();
}


void SoulHunterFail::againCallBack(cocos2d::CCObject* sender)
{
	soulhunter::SoulHunterInfo * info = DM_GET_SOUL_HUNTER_MODEL->getSoulHunterInfo();
	if(info->getPower() <= SNATCH_LOWEST_POWER)
	{
		TIPSMANAGER->warning(D_LOCAL_STRING("SoulHunterNotEnoughPower"));
	}
	else
	{
		DM_GET_SOUL_HUNTER_MODEL->doLastBattleRequest();
	}
	removeFromParent();
}

