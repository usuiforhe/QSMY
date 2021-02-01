//
//  DStepTableView.cpp
//  QSMY
//
//  Created by wanghejun on 13-10-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DStepTableView.h"

USING_NS_CC;
USING_NS_CC_EXT;

DStepTableView* DStepTableView::create(CCTableViewDataSource* dataSource, CCSize size)
{
    return DStepTableView::create(dataSource, size, NULL);
}

DStepTableView* DStepTableView::create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container)
{
    DStepTableView *table = new DStepTableView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
	table->_updateCellPositions();
    table->_updateContentSize();
    
    return table;
}

void DStepTableView::fixPosition(cocos2d::CCPoint &endPoint)
{
    
    m_tScrollDistance = CCPointZero;
    this->unscheduleAllSelectors();
    m_pContainer->stopAllActions();
    
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems)
    {
        return;
    }
    
//    unsigned int startIdx = 0, endIdx = 0, maxIdx = 0;
//    CCPoint offset = ccpMult(this->getContentOffset(), -1);
//    maxIdx = MAX(uCountOfItems-1, 0);
//    
//    if (m_eVordering == kCCTableViewFillTopDown)
//    {
//        offset.y = offset.y + m_tViewSize.height/this->getContainer()->getScaleY();
//    }
//    startIdx = this->_indexFromOffset(offset);
//	if (startIdx == CC_INVALID_INDEX)
//	{
//		startIdx = uCountOfItems - 1;
//	}
//    
//    endIdx = startIdx+1;
//    
    float distance =0.f;
    float offsetSize = 0.f;
    switch (this->getDirection())
    {
        case kCCScrollViewDirectionHorizontal:
            distance = endPoint.x - m_touchPoint.x;
            offsetSize = m_pDataSource->tableCellSizeForIndex(this,m_pCurrentPage).width*0.2;
            break;
        default:
            distance = m_touchPoint.y - endPoint.y;
            offsetSize = m_pDataSource->tableCellSizeForIndex(this,m_pCurrentPage).height*0.2;
            break;
    }
    
    
    if(D_FLOAT_EQUALS(distance, 0.f)) return;
    
    int step = 0;
	if(fabsf(distance) > offsetSize)
	{
        step = distance>0?-1:1;
    }
    
    
    step+=m_pCurrentPage;
    if(step>=(int)uCountOfItems) step = uCountOfItems-1;
    if(step<0) step = 0;
    
    CCPoint adjustPos = _offsetFromIndex(step);
    m_pCurrentPage=step;

    this->setContentOffset(ccpMult(adjustPos, -1), true);
}

bool DStepTableView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	m_touchPoint = pTouch->getLocation();
	return DTableView::ccTouchBegan(pTouch,pEvent);
}


void DStepTableView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!this->isVisible()) {
        return;
    }
    DTableView::ccTouchEnded(pTouch, pEvent);
    CCPoint endPoint = pTouch->getLocation();
    fixPosition(endPoint);
}

void DStepTableView::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!this->isVisible()) {
        return;
    }
    DTableView::ccTouchCancelled(pTouch, pEvent);
    CCPoint endPoint = pTouch->getLocation();
    fixPosition(endPoint);
}