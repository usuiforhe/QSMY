//
//  DPageDot.cpp
//  QSMY
//
//  Created by OnePiece on 13-6-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DPageDot.h"
#define kPageDotOffset 30
#define kminPageDotOffset 12
#define kPageSelectSpriteTag 9999

USING_NS_CC;

DPageDot::DPageDot()
:m_pDotContainer(NULL)
,m_nPages(0)
,m_nCurrentPage(0)
,m_nMarginX(kPageDotOffset)
,m_nPageSize(0)
{
    
}
DPageDot::~DPageDot()
{
    
}

DPageDot * DPageDot::create(const char *selectString , const char *unselectString)
{
    DPageDot *pDPageDot = new DPageDot();
    if(pDPageDot && pDPageDot->init(selectString, unselectString))
    {
        pDPageDot->autorelease();
    }
    else
    {
        CC_SAFE_RELEASE_NULL(pDPageDot);
    }
    
    return pDPageDot;
}

bool DPageDot::init()
{
    return init("PL_ui/common/img_point_h.png","PL_ui/common/img_point_n.png");
}

bool DPageDot::init(const char *selectString , const char *unselectString)
{
    
    m_sSelectString = selectString;
    m_sUnelectString = unselectString;
    
    m_pDotContainer = CCNode::create();
    addChild(m_pDotContainer);
    m_pDotContainer->setAnchorPoint(D_ANCHOR_POINT_CENTER);
    
    return true;
}


void DPageDot::setPages(const int &pages)
{
    if(m_nPages == pages) return;
    m_nPages = pages;    
    m_nCurrentPage = MIN(m_nPages, m_nCurrentPage);
    updateView(true);
}

void DPageDot::setCurrentPage(const int &currentPage)
{
    if(m_nCurrentPage == currentPage) return;
    m_nCurrentPage = currentPage;
    
    updateView(false);
}

int DPageDot::getFixedMargin() const
{
    if(m_nPageSize==0) return m_nMarginX;
    int margin(m_nMarginX);
    while (m_nPages*margin>m_nPageSize)
    {
        --margin;
    }
    return MAX(margin, kminPageDotOffset);
}

void DPageDot::updateView(const bool &clear)
{
    
    if(clear)
    {
        int margin = getFixedMargin();
        m_pDotContainer->removeAllChildren();
        for(int i = 0 ; i < m_nPages ; i++)
        {
            CCSprite *pUnselectSprite = CCSprite::create(m_sUnelectString.c_str());
            pUnselectSprite->setPosition(ccp(i*margin,0));
            m_pDotContainer->addChild(pUnselectSprite,0,i);
        }
        CCSprite *pSelectSprite = CCSprite::create(m_sSelectString.c_str());
        m_pDotContainer->addChild(pSelectSprite,1,kPageSelectSpriteTag);
        m_pDotContainer->setContentSize(CCSizeMake(margin*m_nPages, pSelectSprite->getContentSize().height));
        
        //上面的计算方法 因为点的注册点在中间 导致坐标向左偏半个点 下面向右移动1/4个点
        //FIXME: 这里点大小素材不一样会不对！
        m_pDotContainer->setPosition(ccp(kPageDotOffset*0.25f,0));
    }
    
    if(m_nPages==0) return;
    if(m_nPages==m_nCurrentPage) return;
    CCNode *pSelectSprite = m_pDotContainer->getChildByTag(kPageSelectSpriteTag);
    CCNode *pCurrPageSprite = m_pDotContainer->getChildByTag(m_nCurrentPage);
    if(pCurrPageSprite==NULL) return;
    pSelectSprite->setPosition(pCurrPageSprite->getPosition());

}

void DPageDot::setPageSize(int pageSize)
{
    if(m_nPageSize == pageSize) return;
    m_nPageSize = pageSize;
    updateView(false);
}

void DPageDot::setOffsetX(const int &marginX)
{
    if(m_nMarginX == marginX) return;    
    m_nMarginX = marginX;
    updateView(false);
}