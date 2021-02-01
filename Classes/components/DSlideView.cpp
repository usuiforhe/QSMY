//
//  DSlideView.cpp
//  QSMY
//
//  Created by cll on 13-4-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DSlideView.h"
#include "gui/DMenuItemSound.h"

USING_NS_CC;
USING_NS_CC_EXT;


#define DEF_POS_X			0
#define DEF_POS_Y			774
#define DEF_TAB_HEIGHT		166	//Tab按钮高
#define DEF_TAB_WIDTH		129	//Tab按钮宽

#define TAB_MARGIN_BOTTOM   5 //Tab下补丁

#define DEF_TABVIEW_RECT	cocos2d::CCRectMake(DEF_POS_X, DEF_POS_Y+TAB_MARGIN_BOTTOM, DEF_TAB_WIDTH, DEF_TAB_HEIGHT)			//Tab
#define DEF_CONTENTVIEW_RECT	cocos2d::CCRectMake(0,0,D_DESIGN_SIZE_WIDTH,DEF_POS_Y)			//内容

#define SFX_SLIDE CCString::create("tip_button")

//#define DEF_CONTENT_SIZE    CCSizeMake(D_DESIGN_SIZE_WIDTH, D_DESIGN_SIZE_HEIGHT)

//#define DEF_TABSIZE			CCSizeMake(DEF_TAB_WIDTH, DEF_TAB_HEIGHT)

#define TAB_TAG			123
//#define CONTENT_TAG		456

DSlideView::DSlideView():
m_pTabView(NULL),
m_pContentView(NULL),
m_pTabViewArray(NULL),
m_pContentViewArray(NULL),
m_selectedSlideIdx(0),
m_slideCount(0),
m_contentRect(DEF_CONTENTVIEW_RECT),
m_tabRect(DEF_TABVIEW_RECT),
m_touchPoint(CCPointZero),
m_bSlide(false),
m_bTouchMovedEnabled(true)
{
    
    
}

DSlideView::~DSlideView()
{
	CC_SAFE_RELEASE(m_pContentViewArray);
	CC_SAFE_RELEASE(m_pContentView);
	CC_SAFE_RELEASE(m_pTabViewArray);
	CC_SAFE_RELEASE(m_pTabView);
}

DSlideView * DSlideView::create(CCRect contentRect)
{
    DSlideView* pSlideView = new DSlideView();
	if( pSlideView && pSlideView->initWithRect(contentRect) )
	{
		pSlideView->autorelease();
	}
	else
	{
		CC_SAFE_RELEASE_NULL(pSlideView);
	}
    return pSlideView;
}

DSlideView * DSlideView::create(CCRect tabRect, CCRect contentRect)
{
	DSlideView* pSlideView = new DSlideView();
	if( pSlideView && pSlideView->initWithRects(tabRect, contentRect) )
	{
		pSlideView->autorelease();
	}
	else
	{
		CC_SAFE_RELEASE_NULL(pSlideView);
	}
	return pSlideView;
}


DSlideView * DSlideView::create(CCRect tabRect, CCRect contentRect, unsigned short selectedSlide)
{
    DSlideView* pSlideView = new DSlideView();
	if( pSlideView && pSlideView->initWithRectsAndIdx(tabRect, contentRect, selectedSlide) )
	{
		pSlideView->autorelease();
	}
	else
	{
		CC_SAFE_RELEASE_NULL(pSlideView);
	}
    return pSlideView;
}


bool DSlideView::init()
{
	return initWithRect(DEF_CONTENTVIEW_RECT);
}

bool DSlideView::initWithRect(CCRect contentRect)
{
    
	return initWithRects(DEF_TABVIEW_RECT, contentRect);
}


bool DSlideView::initWithRects(CCRect tabRect, CCRect contentRect)
{
    
	return initWithRectsAndIdx(tabRect, contentRect, m_selectedSlideIdx);
}


bool DSlideView::initWithRectsAndIdx(CCRect tabRect, CCRect contentRect, unsigned short selectedSlideIdx)
{
    
    if(!DLayer::init())
    {
        return false;
    }
    
    m_selectedSlideIdx = selectedSlideIdx;
	m_contentRect = contentRect;
	m_tabRect = tabRect;
    
	m_pTabViewArray = CCArray::create();
    m_pContentViewArray = CCArray::create();
	m_pTabViewArray->retain();
	m_pContentViewArray->retain();


	m_pContentView = DTableView::create(this,m_contentRect.size);
	m_pContentView->setDirection(kCCScrollViewDirectionHorizontal);
	m_pContentView->setPosition(contentRect.origin);
	m_pContentView->retain();
	m_pContentView->setMutiTable(true);
	//this->addChild(m_pContentView, 0, 1);
    
    
	m_pTabView = DTableView::create(this,CCSizeMake(m_contentRect.size.width- 2*m_tabRect.origin.x, m_tabRect.size.height));
	m_pTabView->setDirection(kCCScrollViewDirectionHorizontal);
	m_pTabView->setDelegate(this);
	m_pTabView->setPosition(tabRect.origin);
	m_pTabView->retain();
	//this->addChild(m_pTabView, 10, 1);
    
	/************************************************************************/
	/* fixed by billy
		如果标签区域的rectsize是0
		则先content区域在上方*/
	/************************************************************************/
	if(m_tabRect.equals(CCRectZero))
	{
		this->addChild(m_pContentView, 10, 1);
		this->addChild(m_pTabView, 0, 1);
	}
	else
	{
		this->addChild(m_pContentView, 0, 1);
		this->addChild(m_pTabView, 10, 1);
	}

    return true;
}


void DSlideView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell)
{
    if(table == m_pTabView)
    {
        int idx = cell->getIdx();
		if (idx == m_selectedSlideIdx)	return;
        selectedTab(idx);
        CCLOG("slide cell touched at index: %i", idx );
		CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, SFX_SLIDE);
    }
}


CCSize DSlideView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    if(table == m_pContentView) return m_contentRect.size;
    return m_tabRect.size;
}

CCTableViewCell* DSlideView::tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx)
{
    //if(m_selectedSlideIdx != idx) return NULL;
    if(m_pContentViewArray->count()==0) return NULL;
    CCTableViewCell *cell;
    if(table == m_pContentView)
    {
        cell = (CCTableViewCell *)m_pContentViewArray->objectAtIndex(idx);
    }else
    {
        cell = (CCTableViewCell *)m_pTabViewArray->objectAtIndex(idx);
        CCMenuItemImage *tab_item = (CCMenuItemImage*)cell->getChildByTag(TAB_TAG)->getChildByTag(TAB_TAG);
        
        if(tab_item)
        {
            if(!tab_item->isEnabled())
                return cell;
            
            if(m_selectedSlideIdx== idx)
            {
                //tab_item->setEnabled(false);
                tab_item->selected();
            }else
            {
                //tab_item->setEnabled(true);
                tab_item->unselected();
            }
        }
    }
    
	return cell;
    
}

uint32_t DSlideView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return m_slideCount;
}


void DSlideView::removeSlideByIdx(unsigned short cellIdx)
{
    int slideIdx = -1;
    
    DSlideViewCell* cell = NULL;
    for (uint32_t i = 0; i<m_pTabViewArray->count(); ++i) {
        cell = (DSlideViewCell*)m_pTabViewArray->objectAtIndex(i);
        if(cell && cell->getId() == cellIdx)
        {
            slideIdx = i;
            break;
        }
    }
    
    float adjustTabOffset = 0.f;

	/* 0 -  m_slideCount -1 */
	if(slideIdx<0 || slideIdx >= m_slideCount) return;

	m_pTabViewArray->removeObjectAtIndex(slideIdx);

	m_pContentViewArray->removeObjectAtIndex(slideIdx);

	
	if (slideIdx == m_selectedSlideIdx && m_selectedSlideIdx == m_slideCount -1)
	{
		if(m_slideCount==1)
			m_selectedSlideIdx = 0;
		else
		{
			m_selectedSlideIdx--;

			adjustTabOffset = m_pTabView->getContentOffset().x;

			if(m_tabRect.size.width*m_slideCount>m_contentRect.size.width)
				adjustTabOffset += m_tabRect.size.width;
		}
	}
	else if(slideIdx < m_selectedSlideIdx)
	{
		m_selectedSlideIdx--;
		adjustTabOffset = m_pTabView->getContentOffset().x;
	}
	else
		adjustTabOffset = m_pTabView->getContentOffset().x;
	
	reloadSlideData(adjustTabOffset,false);
	
}

unsigned int DSlideView::getSlideCount()
{
    return m_slideCount;
}

unsigned int DSlideView::addSlideWithTabView(CCNode *contentView,CCMenu *tabView)
{
	CCAssert(tabView!=NULL && contentView!=NULL, "slide can't be null");
    
    static uint32_t cell_idx = 0;
    
    DSlideViewCell* cell;
    cell = new DSlideViewCell(cell_idx);
    cell->autorelease();
    cell->addChild(tabView);
    m_pTabViewArray->addObject(cell);
    
    tabView->setEnabled(false);
    tabView->setAnchorPoint(CCPointZero);
    tabView->setPosition(CCPointZero);
    
    cell = new DSlideViewCell(cell_idx);
    cell->autorelease();
    cell->addChild(contentView);
    m_pContentViewArray->addObject(cell);
    
    if(isRunning())
    {
        reloadSlideData();
    }
    
    return cell_idx++;
}


unsigned int DSlideView::addSlideWithTab(CCNode *contentView,const char *normalImage, const char *selectedImage/* = NULL*/, const char *disableImage/* = NULL*/)
{
	
    CCMenu *tab_Menu = CCMenu::create();
    tab_Menu->setTouchEnabled(false);
    tab_Menu->setTag(TAB_TAG);
    tab_Menu->setAnchorPoint(CCPointZero);
    tab_Menu->setPosition(CCPointZero);
    
    CCMenuItemImage *tab_item = CCMenuItemImage::create(normalImage, selectedImage, disableImage);
    tab_Menu->addChild(tab_item);
    tab_item->setAnchorPoint(CCPointZero);
    tab_item->setPosition(CCPointZero);
    tab_item->setTag(TAB_TAG);
    
    return addSlideWithTabView(contentView,tab_Menu);
	
}


void DSlideView::reloadSlideData(float adjustTabOffset,bool animate)
{
    m_slideCount = MAX(m_pTabViewArray->count(), m_pContentViewArray->count());
    
	float adjustX = adjustTabOffset;
	
	m_pContentView->reloadData();
	m_pTabView->reloadData();
	
	if (m_slideCount <= 0) return;

    //selectedTab(m_selectedSlideIdx);
    
    
    if(!m_pTabView->isBounceable())
    {
        if(m_tabRect.size.width*m_slideCount<m_contentRect.size.width)
        {
            m_pTabView->setViewSize(CCSizeMake(m_tabRect.size.width*m_slideCount, m_tabRect.size.height));
            
        } else m_pTabView->setViewSize(CCSizeMake(m_contentRect.size.width, m_tabRect.size.height));
    }
    
	//m_pContentView->setContentOffset(ccp(m_pContentView->maxContainerOffset().x,m_pContentView->minContainerOffset().y));
	//m_pTabView->setContentOffset(ccp(m_pTabView->maxContainerOffset().x,m_pTabView->minContainerOffset().y));
    m_pContentView->unscheduleAllSelectors();
    m_pContentView->stopAllActions();
	m_pContentView->setContentOffset(ccp(-m_contentRect.size.width * m_selectedSlideIdx,m_pContentView->minContainerOffset().y),animate);
	
	if(D_FLOAT_EQUALS(adjustTabOffset, 0.f))
		adjustX = -m_tabRect.size.width * m_selectedSlideIdx;

    m_pTabView->unscheduleAllSelectors();
    m_pTabView->stopAllActions();
	m_pTabView->setContentOffset(ccp(MIN(0,MAX(adjustX,m_pTabView->minContainerOffset().x)),m_pTabView->minContainerOffset().y));

}

void DSlideView::selectedTabById(unsigned short Id,bool animate)
{
    CCObject* obj;
    DSlideViewCell *pCell;
    uint16_t index = 0;
    CCARRAY_FOREACH(m_pContentViewArray, obj)
    {
        pCell = static_cast<DSlideViewCell*>(obj);
        if(pCell->getId()==Id)
        {
            selectedTab(index,animate);
            break;
        }
        index++;
    }
}

void DSlideView::selectedTab(unsigned short index,bool animate)
{
    index = MIN(MAX(0, index),m_slideCount);
    if(m_selectedSlideIdx == index) return;

	//不恢复滑动，仅当可以滑动时才继续
	//setTouchMovedEnabled(true);
	if(!m_bTouchMovedEnabled)return;

	//之前选中的去掉高亮
	CCTableViewCell *cell = (CCTableViewCell *)m_pTabViewArray->objectAtIndex(m_selectedSlideIdx);
	CCMenuItemImage *tab_item = (CCMenuItemImage*)cell->getChildByTag(TAB_TAG)->getChildByTag(TAB_TAG);
	tab_item->unselected();
	
	m_selectedSlideIdx = index;
    //m_pTabView->reloadData(); 
	tableCellAtIndex(m_pTabView, m_selectedSlideIdx);

    m_pContentView->unscheduleAllSelectors();
    m_pContentView->stopAllActions();
    cocos2d::CCPoint adjustPos = ccp(- m_contentRect.size.width * m_selectedSlideIdx, 0);
    m_pContentView->setContentOffset(adjustPos,animate);
    
    ////////tab/////
    
    float tabOffset =  -m_tabRect.size.width * m_selectedSlideIdx;
    float tabCurrOffset = m_pTabView->getContentOffset().x;
    
	//修改tab滚动条件
    if(tabCurrOffset-tabOffset>m_contentRect.size.width-m_tabRect.size.width/2- 2*m_tabRect.origin.x)
    {
        while (tabCurrOffset-tabOffset>m_contentRect.size.width-m_tabRect.size.width/2- 2*m_tabRect.origin.x) {
            tabCurrOffset-=m_tabRect.size.width;
        }
		int adjustX = MAX(tabCurrOffset,m_pTabView->minContainerOffset().x);
		adjustPos= ccp(adjustX, 0);
        m_pTabView->unscheduleAllSelectors();
        m_pTabView->stopAllActions();
        m_pTabView->setContentOffset(adjustPos,animate);
    }
	else if( tabOffset-tabCurrOffset>0 )
	{
		adjustPos= ccp(tabOffset, 0);
		m_pTabView->unscheduleAllSelectors();
        m_pTabView->stopAllActions();
		m_pTabView->setContentOffset(adjustPos,animate);
	}
}


void DSlideView::adjustSlide(float offset)
{
	//不让滑动的话，就不处理
	if(!m_bTouchMovedEnabled)return;

	cocos2d::CCPoint adjustPos = ccp(- m_contentRect.size.width * m_selectedSlideIdx, 0);
    if(D_FLOAT_EQUALS(offset, 0.f))
    {
        m_pContentView->unscheduleAllSelectors();
        m_pContentView->stopAllActions();
        m_pContentView->setContentOffset(adjustPos, true);
    }
    else
    {
		if (offset<0)
        {
            if(m_selectedSlideIdx==m_slideCount-1)
            {
                m_pContentView->unscheduleAllSelectors();
                m_pTabView->stopAllActions();
                m_pContentView->setContentOffset(adjustPos, true);
            }
            else selectedTab(m_selectedSlideIdx+1,true);
        }else if(offset>0)
        {
            if(m_selectedSlideIdx==0)
            {
                m_pContentView->unscheduleAllSelectors();
                m_pTabView->stopAllActions();
                m_pContentView->setContentOffset(adjustPos, true);
            }
            else selectedTab(m_selectedSlideIdx-1,true);
        }
	}
}



void DSlideView::onEnter()
{
    reloadSlideData();
    DLayer::onEnter();
}

void DSlideView::setTouchMovedEnabled(bool val)
{
	m_bTouchMovedEnabled = val;
	m_pContentView->setScrollEnabled(val);
}

bool DSlideView::dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches)
{
    m_touchPoint = pTouch->getLocation();
    CCRect tabRect = CCRectMake( m_tabRect.origin.x, m_tabRect.origin.y, m_contentRect.size.width, m_tabRect.size.height );
    tabRect = CCRectApplyAffineTransform( tabRect, this->nodeToWorldTransform() );
    
    CCRect contentRect = CCRectMake( m_contentRect.origin.x, m_contentRect.origin.y, m_contentRect.size.width, m_contentRect.size.height );
    contentRect = CCRectApplyAffineTransform( contentRect, this->nodeToWorldTransform() );
    m_bSlide = contentRect.containsPoint(m_touchPoint) && !tabRect.containsPoint(m_touchPoint);
    return DLayer::dTouchBegan(pTouch, pEvent);
}

void DSlideView::dTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    DLayer::dTouchEnded(pTouch, pEvent);
    if(!m_bSlide) return;
    m_bSlide = false;
    CCPoint endPoint = pTouch->getLocationInView();
    float distance = endPoint.x - m_touchPoint.x;
    
    if(fabs(distance) > 0.2 * m_contentRect.size.width)
    {
        adjustSlide(distance);
    }
    else
    {
        adjustSlide(0);
    }
}

void DSlideView::dTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    DLayer::dTouchCancelled(pTouch, pEvent);
    if(!m_bSlide) return;
    m_bSlide = false;
    CCPoint endPoint = pTouch->getLocationInView();
    float distance = endPoint.x - m_touchPoint.x;
    
    if(fabs(distance) > 0.2 * m_contentRect.size.width)
    {
        adjustSlide(distance);
    }
    else
    {
        adjustSlide(0);
    }
}


