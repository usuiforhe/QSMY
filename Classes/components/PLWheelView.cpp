//
//  PLWheelView.cpp
//  QSMY
//
//  Created by wanghejun on 13-5-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PLWheelView.h"
#include "../utils/OpacityUtil.h"
#include "../managers/SoundManager.h"
#define SOUND_ACTION_SELECT_MAP "chose1"
#define WHEEL_BOUNCE_MAX	75

USING_NS_CC;
USING_NS_CC_EXT;


PLWheelView::~PLWheelView()
{
	this->unschedule(schedule_selector(PLWheelView::moveToCenter));
}

PLWheelView* PLWheelView::create(CCTableViewDataSource* dataSource, CCSize size)
{
    return PLWheelView::create(dataSource, size, NULL);
}

PLWheelView* PLWheelView::create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container)
{
    PLWheelView *table = new PLWheelView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
	table->_updateCellPositions();
    table->_updateContentSize();
    
    return table;
}

bool PLWheelView::initWithViewSize(CCSize size, CCNode* container/* = NULL*/)
{
    if (!CCTableView::initWithViewSize(size,container)) return false;
    CCTableView::setVerticalFillOrder(kCCTableViewFillTopDown);
//	CCScrollView::setBounceable(false);
    return true;
}
/*
void PLWheelView::setContentSize(const CCSize & size)
{
    CCSize sSize = CCSizeMake(size.width, size.height+getViewSize().height*0.8);
    CCTableView::setContentSize(sSize);
}


void PLWheelView::reloadData()
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

void PLWheelView::updateContentOffset()
{
    CCPoint curContentOffset = getContentOffset();
    
    const CCPoint minOffset = this->minContainerOffset();
    const CCPoint maxOffset = this->maxContainerOffset();
    
    curContentOffset.x = MAX(minOffset.x, MIN(maxOffset.x, curContentOffset.x));
    curContentOffset.y = MAX(minOffset.y, MIN(maxOffset.y, curContentOffset.y));
    
    setContentOffset(curContentOffset);
}


void PLWheelView::updateCellPositions()
{
	this->_updateCellPositions();
	this->_updateContentSize();
	reloadData();
}



void PLWheelView::updateCellAtIndex(unsigned int idx)
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
*/



void PLWheelView::scrollViewDidScroll(CCScrollView* view)
{
	do 
	{
		CCPoint pt = this->getContentOffset();
//		CCLOG("%f", pt.y);
		if (pt.y > maxContainerOffset().y + WHEEL_BOUNCE_MAX)
		{
			pt.y = maxContainerOffset().y + WHEEL_BOUNCE_MAX;
		}
		else if (pt.y < minContainerOffset().y - WHEEL_BOUNCE_MAX)
		{
			pt.y = minContainerOffset().y - WHEEL_BOUNCE_MAX;
		}
		else
		{
			if (pt.y >= maxContainerOffset().y)
			{
				if (m_pWheelDelegate)
				{
					m_pWheelDelegate->PLWheelViewBouncePercent(this, (pt.y - maxContainerOffset().y) / WHEEL_BOUNCE_MAX);
				}
			}
			else if (pt.y <= minContainerOffset().y)
			{
				if (m_pWheelDelegate)
				{
					m_pWheelDelegate->PLWheelViewBouncePercent(this, (pt.y - minContainerOffset().y) / WHEEL_BOUNCE_MAX);
				}
			}
			else
			{
				if (m_pWheelDelegate)
				{
					m_pWheelDelegate->PLWheelViewBouncePercent(this, 0.0f);
				}
			}
			break;
		}
		m_pContainer->setPosition(pt);
	} while (0);
	
	
	DSelectorView::scrollViewDidScroll(view);
}

void PLWheelView::_setIndexForCell(unsigned int index, CCTableViewCell *cell)
{
    cell->setAnchorPoint(ccp(2.5,2.5));
    cell->setIdx(index);
    
    const CCSize cellSize = m_pDataSource->tableCellSizeForIndex(this, index);
    
	float contentOffsetY = this->getContentOffset().y;

    float offsetY = this->_offsetFromIndex(index).y + contentOffsetY;
    
	float viewHeight = getViewSize().height;

    offsetY += -viewHeight + cellSize.height;
    
	//CCLOG("%d, %f",index,offset.y);

	float scale = 1.0;
	//scale = scale*pow((double)(1 - (fabsf(offset.y))/getViewSize().height),4.0);
	float r = (1.0 - (fabsf(offsetY))/viewHeight);
	scale = r * r * r * r; 

	CCPoint newPt = ccpAdd(this->_offsetFromIndex(index),ccp(0,-viewHeight*0.5 + 0.1));

	float ry = newPt.y + contentOffsetY - viewHeight * 0.5;

	float halfHeight = cellSize.height / 2;
	if (ry > -halfHeight - cellSize.height + 5 && ry <= -halfHeight)
	{
		if (m_uCenterIdx != index)
		{
			CCTableViewCell *pFormal = this->cellAtIndex(m_uCenterIdx);
			if (pFormal)
			{
				pFormal->setScale(0.6);
				pFormal->setPosition(ccpAdd(pFormal->getPosition(), ccp(60, 20)));
			}
			m_uCenterIdx = index;
			if (m_pWheelDelegate)
			{
				m_pWheelDelegate->PLWheelViewCenterIdxChanged(this, m_uCenterIdx);
			}
			
		}
	}
	if (m_uCenterIdx == index)
	{
		m_fCenterRy = ry;
	}
	
	unsigned int lastIdx = m_pDataSource->numberOfCellsInTableView(this) - 1;

// 	if (m_uCenterIdx == index)
// 	{
// 		newPt.y += (cellSize.height + 8) * (1 - scale);
// 	}
// 	
// 
// 	if (index < m_uCenterIdx)
// 	{
// 		if (m_uCenterIdx == lastIdx)
// 		{
// 			float realScale = scale;
// 			if (realScale < 0.6)
// 			{
// 				realScale = 0.6;
// 			}
// 			float plus = 10;
// 			if (index == m_uCenterIdx - 1)
// 			{
// 				plus = 0;
// 			}
// 			
// 			newPt.y += cellSize.height * realScale + plus;
// 		}
// 		else
// 		{
// 			float plus = 0;
// 			if (index == m_uCenterIdx - 1)
// 			{
// 				plus = 8;
// 			}
// 			
// 			newPt.y += cellSize.height * (1 - scale * scale) + plus;
// 		}
// 	}
// 
// 	if (index == m_uCenterIdx && m_uCenterIdx == lastIdx)
// 	{
// 		newPt.y += (cellSize.height - 40) * (1 - scale);
// 	}
// 	
// 	if (index == m_uCenterIdx && m_uCenterIdx == 0)
// 	{
// 		newPt.y -= (cellSize.height) * (1 - scale);
// 	}

	if (index < m_uCenterIdx)
	{
		if (floor(ry) == -32)
		{
			newPt.y += 28;
		}
		else if (ry < 1)
		{
			newPt.y += (ry + cellSize.height - 32) * 0.8 + 28;
		}
		else
		{
			newPt.y += cellSize.height * 5 / 6;
		}
	}
	if (index == m_uCenterIdx)
	{
//		if (ry + cellSize.height > 0)
		if (ry + cellSize.height < 0 && index == 0);
//		else if (ry + cellSize.height > 0 && index == lastIdx);
		else
		{
			newPt.y += (ry + cellSize.height) / 10;
			//CCLOG("%f", ry);
		}
	}

	if (index != m_uCenterIdx)
	{
		if (scale < 0.9)
		{
			scale = 0.6;
		}
		
	}
	else if (ry + cellSize.height < 0 && index == 0)
	{
		scale = 1.0;
	}
	else if (ry + cellSize.height > 0 && index == lastIdx)
	{
		scale = 1.0;
	}
	else
	{
		if (scale > 0.6 && scale < 0.9)
		{
			scale = 0.9;
		}
		
	}

	newPt.x += 300 * (1 - scale) + fabs(offsetY) * 2.5;

	cell->setPosition(newPt);//让切换当前条的位置向下偏移0.1

	scale = MAX(0.6, scale);
	cell->setScale(scale);

	int zOrder = index == m_uCenterIdx ? 1 : 0;

    if(cell->getParent())
    {
        cell->getParent()->reorderChild(cell, zOrder);
    }else
    {
        cell->setZOrder(zOrder);
	}
}


bool PLWheelView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	bool ret = DSelectorView::ccTouchBegan(pTouch, pEvent);
	if (ret)
	{
		this->unscheduleAllSelectors();
		if (m_pWheelDelegate)
		{
			m_pWheelDelegate->PLWheelViewTouched(this);
		}
		
	}
	return ret;
}

void PLWheelView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	DSelectorView::ccTouchMoved(pTouch, pEvent);

	this->unscheduleAllSelectors();
// 	if (m_pTouches->containsObject(pTouch))
// 	{
// 		if (m_pTouches->count() == 1 && m_bDragging)
// 		{
// 			float newX     = m_pContainer->getPosition().x - m_tScrollDistance.x * 0.5;
// 			float newY     = m_pContainer->getPosition().y - m_tScrollDistance.y * 0.5;
// 
// 			this->setContentOffset(ccp(newX, newY));
// 		}
// 	}
}

void PLWheelView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
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
    

	if (m_pWheelDelegate)
	{
		//m_nSoundID = SoundManager::sharedSoundManager()->playSFX(CCString::createWithFormat("skill/%s", SOUND_ACTION_SELECT_MAP));
		m_pWheelDelegate->PLWheelViewStartWheeling(this, m_uCenterIdx);
	}

	this->unscheduleAllSelectors();
	schedule(schedule_selector(PLWheelView::moveToCenter), 0.01);
}

void PLWheelView::moveToCenter(float dt)
{

	const CCSize cellSize = m_pDataSource->tableCellSizeForIndex(this, m_uCenterIdx);
//	CCLOG("%u,%f", m_uCenterIdx, m_fCenterRy);

	if (m_fCenterRy >= -cellSize.height - 0.5 && m_fCenterRy <= -cellSize.height + 0.5)
	{
		this->setContentOffset(ccpAdd(this->getContentOffset(), ccp(0, - m_fCenterRy - cellSize.height)));
		unschedule(schedule_selector(PLWheelView::moveToCenter));
		if (m_pWheelDelegate)
		{
			/*if (m_nSoundID) {
				SoundManager::sharedSoundManager()->stopSFX(m_nSoundID);
				m_nSoundID = 0;
			}*/
			m_pWheelDelegate->PLWheelViewStopWheeling(this, m_uCenterIdx);
		}
	}
	else// if (m_fCenterRy < -cellSize.height - 1)
	{
		this->setContentOffset(ccpAdd(this->getContentOffset(), ccp(0, (-cellSize.height - m_fCenterRy) / 10)));
	}
// 	else
// 	{
// 		this->setContentOffset(ccpAdd(this->getContentOffset(), ccp(0, -1)));
// 	}

// 	if (m_fCenterRy < 0 && m_fCenterRy >= - cellSize.height / 2 + 1)
// 	{
// 		this->setContentOffset(ccpSub(this->getContentOffset(), ccp(0, 1)));
// 	}
// 	else if (m_fCenterRy >= - cellSize.height / 2 * 3 && m_fCenterRy < - cellSize.height / 2)
// 	{
// 		this->setContentOffset(ccpAdd(this->getContentOffset(), ccp(0, 1)));
// 	}
// 	else
// 	{
// 		unschedule(schedule_selector(PLWheelView::moveToCenter));
// 	}
}

void PLWheelView::_updateCellPositions() {
	CCTableView::_updateCellPositions();
	int cellsCount = m_pDataSource->numberOfCellsInTableView(this);
	CCSize cellSize = m_pDataSource->tableCellSizeForIndex(this, cellsCount - 1);
	m_vCellsPositions[cellsCount] += cellSize.height + 10;
}