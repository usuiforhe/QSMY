//
//  DTableView.cpp
//  QSMY
//
//  Created by wanghejun on 13-4-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DTableView.h"

USING_NS_CC;
USING_NS_CC_EXT;

DTableView* DTableView::create(CCTableViewDataSource* dataSource, CCSize size)
{
    return DTableView::create(dataSource, size, NULL);
}

DTableView* DTableView::create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container)
{
    DTableView *table = new DTableView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
	table->_updateCellPositions();
    table->_updateContentSize();
    
    return table;
}

void DTableView::reloadData()
{
    //m_eOldDirection = kCCScrollViewDirectionNone;
    if(getContentSize().equals(CCSizeZero))
    {
        m_eOldDirection = kCCScrollViewDirectionNone;
    }
    
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
    this->updateContentOffset();
    if (m_pDataSource->numberOfCellsInTableView(this) > 0)
    {
        this->scrollViewDidScroll(this);

    }

	if (m_eFlyInStyle == kCCTableViewFlyInReload || m_eFlyInStyle == kCCTableViewFlyInBoth)
	{
		runFlyInAction();
	}
}

void DTableView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCScrollView::ccTouchMoved(pTouch, pEvent);
    
    if (m_pTouchedCell /*&& isTouchMoved()*/) {//去掉isTouchMoved的判断在我们的组件更合理
        if(m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
        }
        
        m_pTouchedCell = NULL;
    }
}


void DTableView::updateContentOffset()
{
    unscheduleAllSelectors();
    CCPoint curContentOffset = getContentOffset();
    
    const CCPoint minOffset = this->minContainerOffset();
    const CCPoint maxOffset = this->maxContainerOffset();
    
    if(minOffset.x<maxOffset.x)
    {
        curContentOffset.x = MAX(minOffset.x, MIN(maxOffset.x, curContentOffset.x));
    }else
    {
        curContentOffset.x = 0.f;
    }
    if(minOffset.y<maxOffset.y)
    {
        curContentOffset.y = MAX(minOffset.y, MIN(maxOffset.y, curContentOffset.y));
    }else
    {
        curContentOffset.y = minOffset.y;
    }
    
    
    setContentOffset(curContentOffset);
}

void DTableView::updateCellPositions(CCTableViewCell* cell)
{
    const CCSize oSize = getContentSize();
    if(getDirection()==kCCScrollViewDirectionVertical)
    {
        const CCSize cellSize = m_pDataSource->tableCellSizeForIndex(this, cell->getIdx());
        
        this->_updateCellPositions();
        this->_updateContentSize();
        
        setContentOffset(ccpSub(getContentOffset(),ccp(0,getContentSize().height-oSize.height)));
        
        if(cell->getPosition().y+getContentOffset().y<0.0f)
        {
            unscheduleAllSelectors();
            this->setContentOffset(ccp(cell->getPosition().x,-cell->getPosition().y));
        }
        else if(cell->getPosition().y+getContentOffset().y+cellSize.height > getViewSize().height)
        {
            unscheduleAllSelectors();
            this->setContentOffset(ccp(cell->getPosition().x,-cell->getPosition().y+getViewSize().height-cellSize.height));
        }
        updateContentOffset();
    }else
    {
        //TODO:  目前只支持竖向滚动
    }
}

void DTableView::releaseCellsFreed()
{
    CC_SAFE_RELEASE(m_pCellsFreed);
    m_pCellsFreed = new CCArrayForObjectSorting();
}