//
//  SoulHunterSuccess.cpp
//  狩魂战斗胜利并抢到魂魄界面
//
//  Created by liuxiaogang  on 13-10-26
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoulHunterSuccess.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  AWARD_CELL_SIZE    CCSizeMake(600, 220)
#define  CELL_ITEM_NUM		3

SoulHunterSuccess::SoulHunterSuccess()
:m_pAwardContainer(NULL)
,m_pBackButton(NULL)
,m_pListView(NULL)
,m_pAwardList(NULL)
,m_pCCData(NULL)
{
}


SoulHunterSuccess::~SoulHunterSuccess()
{
	CC_SAFE_RELEASE(m_pAwardContainer);
	CC_SAFE_RELEASE(m_pBackButton);
	CC_SAFE_RELEASE(m_pListView);
	CC_SAFE_RELEASE(m_pAwardList);
	CC_SAFE_RELEASE(m_pCCData);
}


SoulHunterSuccess*  SoulHunterSuccess::create()
{
	SoulHunterSuccess* pRet = new SoulHunterSuccess();
	if(pRet&& pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}


bool SoulHunterSuccess::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !DLayer::init());
		CCB_READER_AND_ADDCHILD("ui/ccb/soulHunter/soulHunterSuccess");
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


void SoulHunterSuccess::onEnter()
{
	BINDING_EXEC(DM_GET_SOUL_HUNTER_MODEL, this, SoulHunterSuccess::updateView, D_EVENT_SOULHUNTER_SNATCH_FINISH);
	m_pBackButton->getButton()->setTarget(this, menu_selector(SoulHunterSuccess::closeCallBack));
	DLayer::onEnter();
}


void SoulHunterSuccess::updateView()
{
    cocos2d::CCArray *pAwardList = DM_GET_SOUL_HUNTER_MODEL->getHunterDrops();
	CC_SAFE_RETAIN(pAwardList);
    CC_SAFE_RELEASE(m_pAwardList);
	m_pAwardList = pAwardList;
    
	m_pListView->reloadData();
}


void SoulHunterSuccess::onExit()
{
	UNBINDING_ALL(DM_GET_SOUL_HUNTER_MODEL, this);
	DLayer::onExit();
}


bool SoulHunterSuccess::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pAwardContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackButton", DButton *, this->m_pBackButton);
	return false;
}


CCSize SoulHunterSuccess::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return AWARD_CELL_SIZE;
}


CCTableViewCell* SoulHunterSuccess::tableCellAtIndex(CCTableView *table, uint32_t idx)
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


uint32_t SoulHunterSuccess::numberOfCellsInTableView(CCTableView *table)
{
	if(!m_pAwardList) return 0;

	uint32_t cellNum =  (m_pAwardList->count() + CELL_ITEM_NUM - 1)/CELL_ITEM_NUM;
	return cellNum;
}


void SoulHunterSuccess::closeCallBack(cocos2d::CCObject* sender)
{
	removeFromParent();
}