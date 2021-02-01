//
//  SoulHunterView.cpp
//  狩魂的主界面
//
//  Created by liuxiaogang on 13-10-22.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoulHunterView.h"
#include <algorithm>
#define SOUHLHUNTER_EVERY_PAGE_COUNT 6
USING_NS_CC;
USING_NS_CC_EXT;

static int sortWarriorByGradeAndID(CCObject *p0, CCObject *p1)
{
	warrior::WarriorCurInfo *pInfo0 = (warrior::WarriorCurInfo *)p0;
	warrior::WarriorCurInfo *pInfo1 = (warrior::WarriorCurInfo *)p1;
	if (pInfo0->getBaseInfo()->getColorGrade() == pInfo1->getBaseInfo()->getColorGrade())
	{
		return pInfo0->getWid() < pInfo1->getWid();
	}
	return pInfo0->getBaseInfo()->getColorGrade() > pInfo1->getBaseInfo()->getColorGrade();
}

#pragma mark - SoulHunterView
#pragma mark - - init etc.
SoulHunterView::SoulHunterView()
:m_pPageData(NULL)
,m_pTableView(NULL)
,m_pPage(NULL)
,m_pWarriorList(NULL)
,m_pSettleContainer(NULL)
,m_pDotContainer(NULL)
,m_pPageDot(NULL)
,m_pCurrentPage(0)
{
}


SoulHunterView::~SoulHunterView()
{
	CC_SAFE_RELEASE(m_pPageData);
	CC_SAFE_RELEASE(m_pTableView);
	CC_SAFE_RELEASE(m_pPage);
	CC_SAFE_RELEASE(m_pSettleContainer);
	CC_SAFE_RELEASE(m_pDotContainer);
	CC_SAFE_RELEASE(m_pWarriorList);
}


bool SoulHunterView::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !DLayer::init());
		CCB_READER_AND_ADDCHILD("ui/ccb/soulHunter/soulHunterView");
		bRet = true;
	}while(0);

	m_pPageData = readCCData("ui/ccb/soulHunter/soulHunterPage");
	CC_SAFE_RETAIN(m_pPageData);

	m_pPage  = CCArray::create();
	CC_SAFE_RETAIN(m_pPage);

	m_pWarriorList = CCArray::create();
	CC_SAFE_RETAIN(m_pWarriorList);

	m_pTableView = DStepTableView::create(this, m_pSettleContainer->getContentSize());
	m_pTableView->setDirection(kCCScrollViewDirectionHorizontal);
	m_pTableView->setDelegate(this);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pTableView->setPosition(m_pSettleContainer->getPosition());
	m_pTableView->retain();
	this->addChild(m_pTableView);

	 /**
     *   初始化Dot点
     */
    m_pPageDot = DPageDot::create();
    m_pPageDot->setPosition(m_pDotContainer->getPosition());
    m_pPageDot->setPageSize(D_DESIGN_SIZE_WIDTH);
	this->addChild(m_pPageDot);
    
    m_Pos = CCPoint(9999999.f,-9999999.f);

	return bRet;
}


bool SoulHunterView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pSettleContainer); 
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDotContainer", CCNode *, this->m_pDotContainer); 
	return false;
}


CCTableViewCell* SoulHunterView::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	SoulHunterPageView *cell = (SoulHunterPageView*)table->dequeueCell();
	if (!cell)
	{
		cell = SoulHunterPageView::create();
	}
	cell->setDelegate(m_pTransferDelegate);
	cell->setData((CCArray*)m_pPage->objectAtIndex(idx));	
	return cell;
}


unsigned int SoulHunterView::numberOfCellsInTableView(CCTableView *table)
{
	return m_pPage->count();
}


void SoulHunterView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
	m_pPageDot->setCurrentPage(m_pTableView->getCurrentPage());//
}


void SoulHunterView::onExit()
{
	UNBINDING_ALL(DM_GET_WARRIOR_MODEL,this);
	DLayer::onExit();
    m_pWarriorList->removeAllObjects();
    m_pPage->removeAllObjects();
    m_Pos = m_pTableView->getContentOffset();
    m_pTableView->reloadData();
    m_pTableView->releaseCellsFreed();
}


void SoulHunterView::onEnter()
{
	BINDING_ONLY(DM_GET_WARRIOR_MODEL, this, SoulHunterView::soulUpdate, D_EVENT_WARRIORLIST_UPDATE);
	BINDING_ONLY(DM_GET_WARRIOR_MODEL, this, SoulHunterView::soulUpdate, D_EVENT_SOULLIST_UPDATE);
	updateView();
	DLayer::onEnter();
}


void SoulHunterView::soulUpdate()
{
    m_Pos = m_pTableView->getContentOffset();
    updateView();
}

void SoulHunterView::setCurrentPage(uint8_t pageIndex)
{
	if(pageIndex==m_pCurrentPage) return;

	m_pCurrentPage = pageIndex;

	updateView();
}

void SoulHunterView::updateView()
{
	if(!DM_GET_WARRIOR_MODEL->isSynced)
	{
		return;
	}
	CCArray * pWarriors = DM_GET_WARRIOR_MODEL->getWarriorVec();
	m_pWarriorList->removeAllObjects();

	CCObject * obj(NULL);
	CCARRAY_FOREACH(pWarriors, obj)
	{
		warrior::WarriorCurInfo * pWarrior = (warrior::WarriorCurInfo*)obj;
		m_pWarriorList->addObject(pWarrior);
	}
	std::sort(m_pWarriorList->data->arr, m_pWarriorList->data->num + m_pWarriorList->data->arr, sortWarriorByGradeAndID);
	warriorGrouping();
	m_pTableView->reloadData();
    m_pTableView->setContentOffset(m_Pos);
    m_pTableView->updateContentOffset();
}


 CCSize SoulHunterView::cellSizeForTable(CCTableView *table) {
	return m_pSettleContainer->getContentSize();
}


 void SoulHunterView::warriorGrouping()
 {
	 m_pPage->removeAllObjects();
	 uint32_t soulCount = m_pWarriorList->count();
	 uint32_t pageCount = ceilf((float)soulCount/SOUHLHUNTER_EVERY_PAGE_COUNT);
	 m_pPageDot->setPages(pageCount);
	 for (uint32_t i=0;i<soulCount;)
	 {
		 CCArray *tempPage = CCArray::create();
		 for (uint8_t j=0;j<SOUHLHUNTER_EVERY_PAGE_COUNT && i<soulCount;j++)
		 {
			 tempPage->addObject((CCArray *)m_pWarriorList->objectAtIndex(i));
			 i++;
		 }
		 m_pPage->addObject(tempPage);
	 }

 }
