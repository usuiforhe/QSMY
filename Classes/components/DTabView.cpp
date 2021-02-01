//
//  DTabView.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DTabView.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define DEF_TAB_SIZE cocos2d::CCSizeMake(173,65)//Tab按钮尺寸

#define MENU 123

#define TAB_FONT_Y_PLUS -6

#define TAB_NORMAL		"PL_ui/all/all_btn_fenye_lv.png"
#define TAB_SELECTED	"PL_ui/all/all_btn_fenye_lan.png"
#define TAB_DISABLE		"PL_ui/all/all_btn_fenye_lv.png"

DTabView::DTabView()
:m_pTabView(NULL)
,m_pTabMenuScrollView(NULL)
,m_pTabNameArray(NULL)
,m_pTabViewArray(NULL)
,m_tabSize(DEF_TAB_SIZE)
,m_titleFontName(D_FONT_DEFAULT)
,m_titleFontSize(D_FONT_SIZE_14)
,m_titleFontColor(ccWHITE)//ccc3(0xfb,0xcf,0x4b)),
,m_padding(0.0)
,m_menuBoderMargin(0.0)
,m_selectedTabIdx(0)
,m_tabNum(0)
,m_alignment(false)
,m_contentSize(CCSizeZero)
,m_bIsLeftAlignment(true)
,m_pDelegate(NULL)
{
    
    
}

DTabView::~DTabView()
{
    // pre-load tab images
    CCTextureCache::sharedTextureCache()->removeTextureForKey(TAB_NORMAL);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(TAB_SELECTED);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(TAB_DISABLE);
    CC_SAFE_RELEASE(m_pTabNameArray);
	CC_SAFE_RELEASE(m_pTabViewArray);
	//CC_SAFE_RELEASE(m_pTabMenuScrollView);
}

/*rect--内容显示区域*/
DTabView * DTabView::create(CCRect rect)
{
    DTabView* pTabView = new DTabView();
	if( pTabView && pTabView->initWithRect(rect) )
	{
		pTabView->autorelease();
	}
	else
	{
		CC_SAFE_RELEASE_NULL(pTabView);
	}
    return pTabView;
}

/*menuBoderMargin--tab左边距*/
DTabView * DTabView::create(CCRect rect, float menuBoderMargin)
{
    DTabView* pTabView = new DTabView();
	if( pTabView && pTabView->initWithRectAndMargin(rect, menuBoderMargin) )
	{
		pTabView->autorelease();
	}
	else
	{
		CC_SAFE_RELEASE_NULL(pTabView);
	}
    return pTabView;
}

/*alignment为false表示tab显示在顶部，反之亦然 */
DTabView * DTabView::create(CCRect rect, float menuBoderMargin, bool alignment)
{
	DTabView* pTabView = new DTabView();
	if( pTabView && pTabView->initWithRectAndMarginAndAlign(rect, menuBoderMargin, alignment) )
	{
		pTabView->autorelease();
	}
	else
	{
		CC_SAFE_RELEASE_NULL(pTabView);
	}
	return pTabView;
}


DTabView* DTabView::create(cocos2d::CCRect rect,
                           cocos2d::ccColor3B titleFontColor,
                           const char* titleFontName,
                           float titleFontSize,
                           float padding,
                           float menuBoderMargin,
                           int selectedTab,
                           bool alignment,
                           bool clippingToBounds
                           )
{
    DTabView* pTabView = new DTabView();
	if( pTabView && pTabView->initWithAll(rect,titleFontColor,titleFontName,titleFontSize,
                                          padding,menuBoderMargin,selectedTab,alignment) )
	{
		pTabView->autorelease();
	}
	else
	{
		CC_SAFE_RELEASE_NULL(pTabView);
	}
    return pTabView;
}


bool DTabView::init()
{
    return initWithRect(CCRectMake(0, 0, D_WIN_SIZE_WIDTH, D_WIN_SIZE_HEIGHT));
}


bool DTabView::initWithRect(CCRect rect)
{
    
	return initWithRectAndMargin(rect, m_menuBoderMargin);
}


bool DTabView::initWithRectAndMargin(cocos2d::CCRect rect, float menuBoderMargin)
{
	return initWithRectAndMarginAndAlign(rect, menuBoderMargin, m_alignment);
}

bool DTabView::initWithRectAndMarginAndAlign(cocos2d::CCRect rect,float menuBoderMargin, bool alignment)
{
	return initWithAll(rect, m_titleFontColor, m_titleFontName.c_str(), m_titleFontSize, m_padding, menuBoderMargin, m_selectedTabIdx, alignment);
}

bool DTabView::initWithAll(cocos2d::CCRect rect,
                           cocos2d::ccColor3B titleFontColor,
                           const char* titleFontName,
                           float titleFontSize,
                           float padding,
                           float menuBoderMargin,
                           int selectedTab,
                           bool alignment
                           )
{
    
    if(!DLayer::init())
    {
        return false;
    }
	m_alignment = alignment;
    
    m_titleFontColor = titleFontColor;
    m_titleFontName = titleFontName;
    m_titleFontSize = D_FONT_SIZE(titleFontSize);
    m_padding = padding;
    m_menuBoderMargin = menuBoderMargin;
    m_selectedTabIdx = selectedTab;
    
    m_contentSize = rect.size;
    
    this->setContentSize(CCSizeMake(rect.size.width,rect.size.height+m_tabSize.height));
    this->setAnchorPoint(CCPointZero);
    this->setPosition(rect.origin);
    
	// create sub view container
	m_pTabView = DLayer::create();
	m_pTabView->setContentSize(CCSizeMake(rect.size.width,rect.size.height));
	m_pTabView->setAnchorPoint(CCPointZero);
	m_pTabView->setPosition(CCPointZero);
	this->addChild(m_pTabView);
    
	m_pTabNameArray = CCArray::create();
    m_pTabViewArray = CCArray::create();
    
	m_pTabMenuScrollView = DTableView::create(this,CCSizeMake(rect.size.width-m_menuBoderMargin, m_tabSize.height));
	m_pTabMenuScrollView->setDirection(kCCScrollViewDirectionHorizontal);
	m_pTabMenuScrollView->setDelegate(this);
	m_pTabMenuScrollView->setAnchorPoint(CCPointZero);
	m_pTabMenuScrollView->setPosition(ccp(m_menuBoderMargin,rect.size.height+1));
	m_pTabMenuScrollView->setBounceable(false);	//默认不能滚动
    
	if(m_alignment == true )
	{
		m_pTabView->setPosition(ccp(menuBoderMargin,m_tabSize.height));	//往上移m_tabSize.height
		m_pTabMenuScrollView->setPosition(ccp(m_menuBoderMargin,0));
	}
    
	//m_pTabMenuScrollView->retain();
	this->addChild(m_pTabMenuScrollView);
	
	m_pTabNameArray->retain();
	m_pTabViewArray->retain();
    
    // pre-load tab images
    CCTextureCache::sharedTextureCache()->addImage(TAB_NORMAL);
    CCTextureCache::sharedTextureCache()->addImage(TAB_SELECTED);
	CCTextureCache::sharedTextureCache()->addImage(TAB_DISABLE);
    
    
    
    return true;
}

void DTabView::updateLeftAlignment()
{
    if(m_bIsLeftAlignment)
    {
        m_pTabMenuScrollView->setPositionX(m_menuBoderMargin);
    }else
    {
        m_pTabMenuScrollView->setPositionX(m_contentSize.width - m_pTabMenuScrollView->getViewSize().width-m_menuBoderMargin);
    }

}

void DTabView::setTopMenuisLeftAlignment(bool value)
{
    if(m_bIsLeftAlignment==value) return;
    m_bIsLeftAlignment = value;
    if(isRunning()) updateLeftAlignment();
}

void DTabView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell)
{;
    CCMenuItemImage *tab_item = (CCMenuItemImage*)cell->getChildByTag(MENU)->getChildByTag(MENU);
    if(!tab_item->isEnabled()||cell->getIdx() == m_selectedTabIdx) return;
    selectedTab(cell->getIdx());
	CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX);
}


CCSize DTabView::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	return CCSizeMake(m_tabSize.width + m_padding , m_tabSize.height);
}

CCTableViewCell* DTabView::tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx)
{
    
    if(m_pTabNameArray->count()==0) return NULL;
	CCTableViewCell *cell = (CCTableViewCell *)m_pTabNameArray->objectAtIndex(idx);
    
    CCMenuItemImage *tab_item = (CCMenuItemImage*)cell->getChildByTag(MENU)->getChildByTag(MENU);
    
	if(tab_item)
	{
		if(!tab_item->isEnabled())
			return cell;
        
		if(m_selectedTabIdx == idx)
		{
			//tab_item->setEnabled(false);
			tab_item->selected();
		}else
		{
			//tab_item->setEnabled(true);
			tab_item->unselected();
		}
	}
    
	return cell;
    
}

uint32_t DTabView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return m_tabNum;
}

void DTabView::addTabForTest(CCNode *tabView)
{
    addTab(tabView,"a",TAB_NORMAL,TAB_SELECTED,TAB_DISABLE,NULL);
}

void DTabView::addTabWithImage(CCNode *tabView,const char *normalImage, const char *selectedImage/* = NULL*/, const char *disableImage/* = NULL*/)
{
    addTab(tabView,"",normalImage,selectedImage,disableImage,NULL);
}

void DTabView::addTabWithName(CCNode * tabView,const char* tabName)
{
	addTab(tabView,tabName,TAB_NORMAL,TAB_SELECTED,TAB_DISABLE,NULL);
}

void DTabView::addTabWithFontImg(CCNode * tabView,const char *fontImgName)
{
	addTab(tabView,"",TAB_NORMAL,TAB_SELECTED,TAB_DISABLE,fontImgName);
}

void DTabView::addTab(CCNode *tabView,const char* _tabName,const char *normalImage, const char *selectedImage/* = NULL*/, const char *disableImage/* = NULL*/,const char *tabFontImage/* = NULL*/)
{
	
    CCTableViewCell* cell = new CCTableViewCell();
    cell->autorelease();
    
    CCMenu *tab_Menu = CCMenu::create();
    tab_Menu->setTouchEnabled(false);
    tab_Menu->setEnabled(false);
    tab_Menu->setTag(MENU);
    tab_Menu->setAnchorPoint(CCPointZero);
    tab_Menu->setPosition(CCPointZero);
    
    CCMenuItemImage *tab_item = CCMenuItemImage::create(normalImage, selectedImage, disableImage);
    tab_Menu->addChild(tab_item);
    tab_item->setAnchorPoint(CCPointZero);
    tab_item->setPosition(CCPointZero);
    tab_item->setTag(MENU);
    
	m_tabSize = CCSizeMake(D_NODE_WIDTH(tab_item)-3,D_NODE_HEIGHT(tab_item)+6);	//获取实际tab图片size -3 是为equip标签排版
    
    CCLabelTTF *tabName= CCLabelTTF::create(_tabName,m_titleFontName.c_str(),m_titleFontSize);
    tabName->setPosition(ccp(m_tabSize.width/2,m_tabSize.height/2));
    tabName->setColor(m_titleFontColor);
    
	CCMenuItemImage *tab_font_item = CCMenuItemImage::create(tabFontImage, tabFontImage, tabFontImage);
	tab_Menu->addChild(tab_font_item);
	tab_font_item->setPosition(ccp(m_tabSize.width/2,m_tabSize.height * 0.35/*/2 + TAB_FONT_Y_PLUS*/));
    
    cell->addChild(tab_Menu);
    cell->addChild(tabName);
    
    m_pTabNameArray->addObject(cell);
	m_pTabViewArray->addObject(tabView);
	
}


void DTabView::reloadTabData()
{
    m_tabNum = m_pTabViewArray->count();
    if (m_tabNum<=0) return;
	m_pTabMenuScrollView->reloadData();
    
    
    if(!m_pTabMenuScrollView->isBounceable())
    {
        if((m_tabSize.width+m_padding)*m_tabNum<m_contentSize.width)
            m_pTabMenuScrollView->setViewSize(CCSizeMake((m_tabSize.width+m_padding)*m_tabNum, m_tabSize.height));
        else m_pTabMenuScrollView->setViewSize(CCSizeMake(m_contentSize.width, m_tabSize.height));
    }
    
    selectedTab(m_selectedTabIdx);
    
	m_pTabMenuScrollView->setContentOffset(ccp(m_pTabMenuScrollView->maxContainerOffset().x,m_pTabMenuScrollView->minContainerOffset().y));//FIXME 按位置滚动
}

void DTabView::selectedTab(int index)
{
    m_pTabView->removeAllChildren();
    CCLayer * pView = (CCLayer*) m_pTabViewArray->objectAtIndex(index);
    m_pTabView->addChild(pView);
    m_selectedTabIdx = index;
    m_pTabMenuScrollView->reloadData();
	if (m_pDelegate)
	{
		m_pDelegate->SelectTabAtIndex(this, m_selectedTabIdx);
	}
	
}

void DTabView::setTabEnable(bool value, int idx)
{
    
	if (m_pTabNameArray->count() == 0)
		return;
    
	CCTableViewCell *cell = (CCTableViewCell *)m_pTabNameArray->objectAtIndex(idx);
    
	CCMenuItemImage *tab_item = (CCMenuItemImage*)cell->getChildByTag(MENU)->getChildByTag(MENU);
    
	tab_item->setEnabled(value);
    
}


void DTabView::onEnter()
{
    reloadTabData();
    updateLeftAlignment();
    DLayer::onEnter();
}
