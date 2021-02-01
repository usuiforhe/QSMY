//
//  DPageViewLoop.cpp
//  QSMY
//
//  Created by wanghejun on 13-5-17.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DPageViewLoop.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define BOUNCE_DURATION      0.15f
#define MOVE_TAG 0


DPageViewLoop::DPageViewLoop()
{
}

DPageViewLoop::~DPageViewLoop()
{
    
}

DPageViewLoop* DPageViewLoop::create(DPageViewDataSource* dataSource,CCSize size, CCNode* container/* = NULL*/)
{
    DPageViewLoop* pRet = new DPageViewLoop();
    if (pRet && pRet->init(dataSource,size, container))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void DPageViewLoop::preparePages()
{
    uint32_t from;
    uint32_t to;
    
    switch (scrollDirection)
    {
        case kScrollDirectionRight:
            from = m_iCurrentPage;
            to = m_iCurrentPage+1;
            break;
        case kScrollDirectionLeft:
        default:
            from = m_iCurrentPage-1;
            to = m_iCurrentPage;
            break;
    }
    
    //m_pContainer->removeAllChildren();
    for (uint32_t i=from; i<=to; ++i)
    {
        cocos2d::CCNode *pNode= m_pDataSource->cellAtIndex(this, i%m_pDataSource->numberOfCellsInTableView(this));
        cocos2d::CCPoint adjustPos = ccp(m_pDataSource->cellSizeForTable(this) * i, 0);
        pNode->setPosition(ccp(adjustPos.x,0));
        if(pNode->getParent()==NULL)this->m_pContainer->addChild(pNode);
    }
}



void DPageViewLoop::fixCurrentPage()
{
    if(m_pDataSource->numberOfCellsInTableView(this)==0) return;
    m_iCurrentPage=m_pDataSource->numberOfCellsInTableView(this)+m_iCurrentPage%m_pDataSource->numberOfCellsInTableView(this);
    cocos2d::CCNode *pNode= m_pDataSource->cellAtIndex(this, m_iCurrentPage%m_pDataSource->numberOfCellsInTableView(this));
    cocos2d::CCPoint adjustPos = ccp(- m_pDataSource->cellSizeForTable(this) * m_iCurrentPage, 0);
    m_pContainer->stopAllActions();
    this->unscheduleAllSelectors();
    this->setContentOffset(adjustPos, false);
    pNode->setPosition(ccp(-adjustPos.x,0));
    
    //m_pContainer->removeAllChildren();
    //this->m_pContainer->addChild(pNode);
    
    for (int i =0; i< m_pContainer->getChildrenCount(); ++i) {
        if(m_pContainer->getChildren()->objectAtIndex(i)!=pNode)
        {
            ((cocos2d::CCNode*)m_pContainer->getChildren()->objectAtIndex(i))->removeFromParentAndCleanup(false);
            --i;
        }
    }
    
    if(pNode->getParent()==NULL) this->m_pContainer->addChild(pNode);
    
    fixAlpha();
}

void DPageViewLoop::setCurrentPage(int32_t val,bool animated)
{
    if(m_pDataSource->numberOfCellsInTableView(this)==0) return;
    if(val!=m_iCurrentPage)
    {
        m_iCurrentPage = val;
        cocos2d::CCPoint adjustPos = ccp(- m_pDataSource->cellSizeForTable(this) * m_iCurrentPage, 0);
        this->unscheduleAllSelectors();
        this->setContentOffset(adjustPos, animated);
        if(animated)
        {
            this->runAction(CCSequence::create(CCDelayTime::create(BOUNCE_DURATION),CCCallFunc::create(this, callfunc_selector(DPageViewLoop::fixCurrentPage)),NULL));
            this->schedule(schedule_selector(DPageViewLoop::fixAlpha));
        }else
        {
            fixCurrentPage();
        }
    }
    
}

