//
//  DPageView.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DPageView.h"
#include "../utils/OpacityUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define BOUNCE_DURATION      0.15f
#define MOVE_TAG 0


DPageView::DPageView()
:m_iCurrentPage(0)
,m_bAutoMove(false)
,m_bFade(true)
,m_fMoveInterval(3.0f)
,scrollDirection(kScrollDirectionRight)
{
}

DPageView::~DPageView()
{
    
}

DPageView* DPageView::create(DPageViewDataSource* dataSource,CCSize size, CCNode* container/* = NULL*/)
{
    DPageView* pRet = new DPageView();
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

bool DPageView::init(DPageViewDataSource* dataSource,CCSize size, CCNode *container/* = NULL*/)
{
    if (CCScrollView::initWithViewSize(size,container))
    {
        m_pDataSource = dataSource;
        m_eDirection = kCCScrollViewDirectionHorizontal;
        return true;
    }
    return false;
}


void DPageView::fixAlpha(float dt)
{
    fixAlpha();
}

void DPageView::fixAlpha()
{
    if(!m_bFade) return;
    CCObject* pObj = NULL;
    CCNode* pNode = NULL;
    float size = m_pDataSource->cellSizeForTable(this);
    CCARRAY_FOREACH(m_pContainer->getChildren(), pObj)
    {
        pNode = dynamic_cast<CCNode*>(pObj);
        float percent = 1-fabs(pNode->getPosition().x+ m_pContainer->getPosition().x)/size;
        percent = MAX(0.0f, percent);
        if(pNode) ::setOpacity(pNode,(GLubyte)(255*percent));
    }
}



void DPageView::nextPage()
{
    preparePages();
    switch (scrollDirection) {
        case kScrollDirectionLeft:
            setCurrentPage(m_iCurrentPage-1,true);
            break;
        case kScrollDirectionRight:
        default:
            setCurrentPage(m_iCurrentPage+1,true);
            break;
    }
}


void DPageView::reloadData()
{
    getContainer()->removeAllChildren();
    getContainer()->setContentSize(CCSizeMake(m_pDataSource->cellSizeForTable(this)*m_pDataSource->numberOfCellsInTableView(this)*2,getViewSize().height));
    fixCurrentPage();
    stopActionByTag(MOVE_TAG);
    autoMove();
}

void DPageView::setFade(bool val)
{
    if(m_bFade==val) return;
    if(!val)
    {
        CCObject* pObj = NULL;
        CCNode* pNode = NULL;
        CCARRAY_FOREACH(m_pContainer->getChildren(), pObj)
        {
            pNode = dynamic_cast<CCNode*>(pObj);
            if(pNode) ::setOpacity(pNode,(GLubyte)255);
        }
    }
    m_bFade = val;
    fixAlpha();
}

void DPageView::setAutoMove(bool val)
{
    if(m_bAutoMove==val) return;
    if(!val) stopActionByTag(MOVE_TAG);
    m_bAutoMove = val;
    autoMove();
}

void DPageView::autoMove()
{
    if(m_bAutoMove && m_pDataSource->numberOfCellsInTableView(this)>1)
    {
        CCAction* m_pAutoMove = CCRepeatForever::create(CCSequence::create(CCDelayTime::create(m_fMoveInterval),CCCallFunc::create(this, callfunc_selector(DPageView::nextPage)),NULL));
        m_pAutoMove->setTag(MOVE_TAG);
        runAction(m_pAutoMove);
    }
}


bool DPageView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_pDataSource->numberOfCellsInTableView(this)<=1)
    {
        CCScrollView::ccTouchBegan(pTouch,pEvent);
        return false;
    }
    bool ret = CCScrollView::ccTouchBegan(pTouch,pEvent);
	if (ret)
	{
		oldScrollDirection = scrollDirection;
		scrollDirection = kScrollDirectionNone;

		stopActionByTag(MOVE_TAG);
		m_touchPoint = pTouch->getLocation();
	}
	return ret;
}

void DPageView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCScrollView::ccTouchMoved(pTouch, pEvent);
    
    cocos2d::CCPoint adjustPos = ccp(- m_pDataSource->cellSizeForTable(this) * m_iCurrentPage, 0);
    float distance = m_pContainer->getPosition().x - adjustPos.x;
    
    ScrollDirection temp = scrollDirection;
    if(distance>0)
    {
        scrollDirection = kScrollDirectionLeft;
    }else
    {
        scrollDirection = kScrollDirectionRight;
    }
    
    oldScrollDirection = scrollDirection;
    
    if(temp != scrollDirection)
    {
        preparePages();
    }
    fixAlpha();
}

void DPageView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    CCScrollView::ccTouchEnded(pTouch,pEvent);
	CCPoint endPoint = pTouch->getLocation();
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > m_pDataSource->cellSizeForTable(this)*0.2)
	{
		adjustScrollView(distance);
	}else
    {
        adjustScrollView(0);
    }
    scrollDirection = oldScrollDirection;
    autoMove();
}

void DPageView::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCScrollView::ccTouchCancelled(pTouch,pEvent);
    CCPoint endPoint = pTouch->getLocation();
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > m_pDataSource->cellSizeForTable(this)*0.2)
	{
		adjustScrollView(distance);
	}else
    {
        adjustScrollView(0);
    }
    scrollDirection = oldScrollDirection;
    autoMove();
}

void DPageView::adjustScrollView(float offset)
{
    cocos2d::CCPoint adjustPos = ccp(- m_pDataSource->cellSizeForTable(this) * m_iCurrentPage, 0);
    if(D_FLOAT_EQUALS(offset, 0.f))
    {
        this->unscheduleAllSelectors();
        this->setContentOffset(adjustPos, true);
        this->runAction(CCSequence::create(CCDelayTime::create(BOUNCE_DURATION),CCCallFunc::create(this, callfunc_selector(DPageView::fixCurrentPage)),NULL));
        this->schedule(schedule_selector(DPageView::fixAlpha));
    }else
        if (offset<0)
        {
            setCurrentPage(m_iCurrentPage+1,true);
        }else if(offset>0)
        {
            setCurrentPage(m_iCurrentPage-1,true);
        }
}






void DPageView::preparePages()
{
    int32_t from;
    int32_t to;
    
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
    
    for (int32_t i=from; i<=to; ++i)
    {
        if(i < 0 || i>= m_pDataSource->numberOfCellsInTableView(this)) continue;
        cocos2d::CCNode *pNode= m_pDataSource->cellAtIndex(this, i);
        cocos2d::CCPoint adjustPos = ccp(m_pDataSource->cellSizeForTable(this) * i, 0);
        pNode->setPosition(ccp(adjustPos.x,0));
        if(pNode->getParent()==NULL)this->m_pContainer->addChild(pNode);
    }
}



void DPageView::fixCurrentPage()
{
    if(m_pDataSource->numberOfCellsInTableView(this)==0) return;
    cocos2d::CCNode *pNode= m_pDataSource->cellAtIndex(this, m_iCurrentPage);
    cocos2d::CCPoint adjustPos = ccp(- m_pDataSource->cellSizeForTable(this) * m_iCurrentPage, 0);
    m_pContainer->stopAllActions();
    this->unscheduleAllSelectors();
    this->setContentOffset(adjustPos, false);
    pNode->setPosition(ccp(-adjustPos.x,0));
    
    //m_pContainer->removeAllChildren();
    //this->m_pContainer->addChild(pNode);
    
    for (uint32_t i =0; i< m_pContainer->getChildrenCount(); ++i) {
        if(m_pContainer->getChildren()->objectAtIndex(i)!=pNode)
        {
            ((cocos2d::CCNode*)m_pContainer->getChildren()->objectAtIndex(i))->removeFromParentAndCleanup(false);
            --i;
        }
    }
    
    if(pNode->getParent()==NULL) this->m_pContainer->addChild(pNode);
    
    fixAlpha();
}

void DPageView::setCurrentPage(int32_t val,bool animated)
{
    if(m_pDataSource->numberOfCellsInTableView(this)==0) return;
    if(val!=m_iCurrentPage)
    {
        m_iCurrentPage = val;
        m_iCurrentPage =MIN(MAX(0,m_iCurrentPage),m_pDataSource->numberOfCellsInTableView(this)-1);
        cocos2d::CCPoint adjustPos = ccp(- m_pDataSource->cellSizeForTable(this) * m_iCurrentPage, 0);
        this->unscheduleAllSelectors();
        this->setContentOffset(adjustPos, animated);
        if(animated)
        {
            this->runAction(CCSequence::create(CCDelayTime::create(BOUNCE_DURATION),CCCallFunc::create(this, callfunc_selector(DPageView::fixCurrentPage)),NULL));
            this->schedule(schedule_selector(DPageView::fixAlpha));
        }else
        {
            fixCurrentPage();
        }
    }
    
}



