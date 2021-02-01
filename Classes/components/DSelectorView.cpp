//
//  DSelectorView.cpp
//  QSMY
//
//  Created by wanghejun on 13-5-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DSelectorView.h"
#include "../utils/OpacityUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;

DSelectorView* DSelectorView::create(CCTableViewDataSource* dataSource, CCSize size)
{
    return DSelectorView::create(dataSource, size, NULL);
}

DSelectorView* DSelectorView::create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container)
{
    DSelectorView *table = new DSelectorView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
	table->_updateCellPositions();
    table->_updateContentSize();
    
    return table;
}

bool DSelectorView::initWithViewSize(CCSize size, CCNode* container/* = NULL*/)
{
    if (!CCTableView::initWithViewSize(size,container)) return false;
    CCTableView::setVerticalFillOrder(kCCTableViewFillTopDown);
    return true;
}

void DSelectorView::setContentSize(const CCSize & size)
{
    CCSize sSize = CCSizeMake(size.width, size.height+getViewSize().height*0.8);
    CCTableView::setContentSize(sSize);
}


void DSelectorView::reloadData()
{
    //m_eOldDirection = kCCScrollViewDirectionNone;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(m_pCellsUsed, pObj)
    {
        CCTableViewCell* cell = (CCTableViewCell*)pObj;
        
        if(m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellWillRecycle(this, cell);
        }
        
        m_pCellsFreed->addObject(cell);
        cell->reset();
        if (cell->getParent() == this->getContainer())
        {
            this->getContainer()->removeChild(cell, true);
        }
    }
    
    m_pIndices->clear();
    m_pCellsUsed->release();
    m_pCellsUsed = new CCArrayForObjectSorting();
    
    this->_updateCellPositions();
    this->_updateContentSize();
    if (m_pDataSource->numberOfCellsInTableView(this) > 0)
    {
        this->scrollViewDidScroll(this);
        
    }
    
}

void DSelectorView::updateContentOffset()
{
    CCPoint curContentOffset = getContentOffset();
    
    const CCPoint minOffset = this->minContainerOffset();
    const CCPoint maxOffset = this->maxContainerOffset();
    
    curContentOffset.x = MAX(minOffset.x, MIN(maxOffset.x, curContentOffset.x));
    curContentOffset.y = MAX(minOffset.y, MIN(maxOffset.y, curContentOffset.y));
    
    setContentOffset(curContentOffset);
}


void DSelectorView::updateCellPositions()
{
	this->_updateCellPositions();
	this->_updateContentSize();
	reloadData();
	/*
     if (m_pDataSource->numberOfCellsInTableView(this) > 0)
     {
     this->scrollViewDidScroll(this);
     
     //updateContentOffset();
     }
     */
}





void DSelectorView::scrollViewDidScroll(CCScrollView* view)
{
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems)
    {
        return;
    }
    
    if(m_pTableViewDelegate != NULL) {
        m_pTableViewDelegate->scrollViewDidScroll(this);
    }
    
    unsigned int startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    CCPoint offset = ccpMult(this->getContentOffset(), -1);
    maxIdx = MAX(uCountOfItems-1, 0);
    
    
    offset.y = offset.y + m_tViewSize.height/this->getContainer()->getScaleY();
    
    m_iStartIdx = this->_indexFromOffset(offset);
    
	if (m_iStartIdx == CC_INVALID_INDEX)
	{
		m_iStartIdx = uCountOfItems - 1;
	}
    
    startIdx = m_iStartIdx>=3?m_iStartIdx-3:0;
        
    offset.y -= m_tViewSize.height/this->getContainer()->getScaleY();
    
    offset.x += m_tViewSize.width/this->getContainer()->getScaleX();
    
    endIdx   = this->_indexFromOffset(offset);
    if (endIdx == CC_INVALID_INDEX)
	{
		endIdx = uCountOfItems - 1;
	}
    
#if 0 // For Testing.
    CCObject* pObj;
    int i = 0;
    CCARRAY_FOREACH(m_pCellsUsed, pObj)
    {
        CCTableViewCell* pCell = (CCTableViewCell*)pObj;
        CCLOG("cells Used index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    CCLOG("---------------------------------------");
    i = 0;
    CCARRAY_FOREACH(m_pCellsFreed, pObj)
    {
        CCTableViewCell* pCell = (CCTableViewCell*)pObj;
        CCLOG("cells freed index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    CCLOG("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif
    
    if (m_pCellsUsed->count() > 0)
    {
        CCTableViewCell* cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(0);
        
        idx = cell->getIdx();
        while(idx <startIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (m_pCellsUsed->count() > 0)
            {
                cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(0);
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    if (m_pCellsUsed->count() > 0)
    {
        CCTableViewCell *cell = (CCTableViewCell*)m_pCellsUsed->lastObject();
        idx = cell->getIdx();
        
        while(idx <= maxIdx && idx > endIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (m_pCellsUsed->count() > 0)
            {
                cell = (CCTableViewCell*)m_pCellsUsed->lastObject();
                idx = cell->getIdx();
                
            }
            else
            {
                break;
            }
        }
    }
    
    for (unsigned int i=startIdx; i <= endIdx; i++)
    {
        //if ([m_pIndices containsIndex:i])
        //        if (m_pIndices->find(i) != m_pIndices->end())
        //        {
        //            continue;
        //        }
        this->updateCellAtIndex(i);
    }
}

void DSelectorView::updateCellAtIndex(unsigned int idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems || idx > uCountOfItems-1)
    {
        return;
    }
    
    CCTableViewCell* cell = this->cellAtIndex(idx);
    if (cell)
    {
        this->_moveCellOutOfSight(cell);
    }
    cell = m_pDataSource->tableCellAtIndex(this, idx);
    this->_setIndexForCell(idx, cell);
    this->_addCellIfNecessary(cell);
}

void DSelectorView::_setIndexForCell(unsigned int index, CCTableViewCell *cell)
{
    cell->setAnchorPoint(ccp(2.5,2.5));
    cell->setIdx(index);
    
    int zOrder = -fabsf((int)index-m_iStartIdx); //相等zoder最高为零
    
    
    if(cell->getParent())
    {
        cell->getParent()->reorderChild(cell, zOrder);
    }else
    {
        cell->setZOrder(zOrder);
    }
    
    const CCSize cellSize = m_pDataSource->tableCellSizeForIndex(this, index);
    
    CCPoint offset = ccpAdd(this->_offsetFromIndex(index), this->getContentOffset());
    
    offset.y+=-getViewSize().height+cellSize.height;
    
    GLubyte alpha = 255;
    
    //CCLOG("%d, %f",index,offset.y);

    cell->setPosition(ccpAdd(this->_offsetFromIndex(index),ccp(fabsf(offset.y)/5,-getViewSize().height*0.5 + 0.1)));//让切换当前条的位置向下偏移0.1
   
//    if(m_iStartIdx!=index && fabsf(offset.y)>80){
//        alpha = alpha*(1- pow((double)(fabsf(offset.y)-80)/getViewSize().height,1.0f/4));
//    }
    
    alpha = alpha*pow((double)(1 - (fabsf(offset.y))/getViewSize().height),2.0);

    //CCLOG("alpha %d",alpha);
    alpha = MAX(0,MIN(255, alpha));

    setOpacity(cell, alpha);
}


void DSelectorView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    //m_tScrollDistance = CCPointZero;
    
//    float target = getContentOffset().y + m_tScrollDistance.y;
//    
//    const CCSize cellSize = m_pDataSource->tableCellSizeForIndex(this, m_iStartIdx);
//    
//    m_tScrollDistance.y = ceilf(target/cellSize.height)*cellSize.height-getContentOffset().y;
//    
//    CCLOG("%f",m_tScrollDistance.y);
    CCTableView::ccTouchEnded(pTouch, pEvent);
    
}


void DSelectorView::_updateCellPositions()
{
	CCTableView::_updateCellPositions();
}