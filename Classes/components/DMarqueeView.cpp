//
//  DMarqueeView.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DMarqueeView.h"
#include "./gui/DClippingNode.h"

USING_NS_CC;

DMarqueeView::DMarqueeView():
m_fSpeed(40),
m_iStack(10),
m_pStringStack(new CCArray),
m_currntPtr(-1),
m_pLabel1(NULL),
m_pLabel2(NULL),
m_bLabel1Move(true),
m_bFirst(true),
m_iPadding(D_WIN_SIZE_WIDTH_HALF), //小于屏幕一半的间距会穿帮
m_pDataSource(NULL)
{
    
}

DMarqueeView::~DMarqueeView()
{
    CC_SAFE_RELEASE(m_pStringStack);
    CC_SAFE_RELEASE(m_pLabel1);
    CC_SAFE_RELEASE(m_pLabel2);
}

DMarqueeView* DMarqueeView::create(int width)
{
    DMarqueeView* pMarquee = new DMarqueeView();
	if( pMarquee && pMarquee->initWithWidth(width))
	{
		pMarquee->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pMarquee);
	}
    return pMarquee;
}

DMarqueeView* DMarqueeView::create(int width,
                            cocos2d::ccColor3B fontColor,
                            const char* fontName,
                            float fontSize)
{
    DMarqueeView* pMarquee = new DMarqueeView();
	if( pMarquee && pMarquee->initWithWidthAndFont(width,fontColor,fontName,fontSize))
	{
		pMarquee->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pMarquee);
	}
    return pMarquee;
}


bool DMarqueeView::init()
{
    return initWithWidth(D_WIN_SIZE_WIDTH);
}


bool DMarqueeView::initWithWidth(int width)
{
    return initWithWidthAndFont(width, ccc3(0xff, 0xff, 0xff), D_FONT_DEFAULT, NOTICE_FONT_SIZE);
}

bool DMarqueeView::initWithWidthAndFont(int width,
                                  cocos2d::ccColor3B fontColor,
                                  const char* fontName,
                                  float fontSize)
{    
    if(!DLayer::init())
    {
        return false;
    }
    
    setAnchorPoint(CCPointZero);
    
    m_pLabel1 = CCRichLabelTTF::create(" ", fontName, D_FONT_SIZE(fontSize));
    m_pLabel1->setColor(fontColor);
    m_pLabel1->setAnchorPoint(ccp(0.0f,0.5f));
    m_pLabel1->retain();
    
    m_pLabel2 = CCRichLabelTTF::create(" ", fontName, D_FONT_SIZE(fontSize));
    m_pLabel2->setColor(fontColor);
    m_pLabel2->setAnchorPoint(ccp(0.0f,0.5f));
    m_pLabel2->retain();
    
    
    this->setContentSize(CCSizeMake(width,this->getContentSize().height));
    DClippingNode * pNode = DClippingNode::create();
    pNode->setContentSize(this->getContentSize());
    pNode->addChild(m_pLabel1);
    pNode->addChild(m_pLabel2);
    this->addChild(pNode);
    m_pLabel1->setPosition(ccp(width,m_pLabel1->getContentSize().height*0.5f));
    m_pLabel2->setPosition(ccp(width,m_pLabel2->getContentSize().height*0.5f));
    
    
    
    return true;
}


void DMarqueeView::onEnter()
{
    DLayer::onEnter();
    scheduleUpdate();
}
void DMarqueeView::onExit()
{
    DLayer::onExit();
    unscheduleUpdate();
}


void DMarqueeView::addString(const char* str)
{
    m_pStringStack->addObject(CCString::create(str));
    setStack(m_iStack);
}

void DMarqueeView::addString(cocos2d::CCString *str)
{
    m_pStringStack->addObject(str);
    setStack(m_iStack);
}

void DMarqueeView::setStack(uint32_t num)
{
    while(m_pStringStack->count()>num)
    {
        m_pStringStack->removeObjectAtIndex(0);
    }
}

void DMarqueeView::setDataSource(DMarqueeViewDataSouce *pDataSource)
{
    if(pDataSource == m_pDataSource)
    {
        return;
    }  
    m_pDataSource = pDataSource;    
}

void DMarqueeView::update(float dt)
{
    m_pLabel1->setVisible(true);
    m_pLabel2->setVisible(true);
    DLayer::update(dt);
    if(!isVisible()) return;
    if(m_bFirst)
    {
        m_bFirst=false;
        m_pLabel1->setString(getText()->getCString());
        m_pLabel2->setString(getText()->getCString());
    }
    
    if(m_pStringStack->count()==0) return;
    
    if(m_bLabel1Move)
    {
        moveLabel(m_pLabel1,m_pLabel2,dt);
    }else
    {
        moveLabel(m_pLabel2,m_pLabel1,dt);
    }
}

void DMarqueeView::moveLabel(cocos2d::CCRichLabelTTF *move,cocos2d::CCRichLabelTTF *forward,float dt)
{
    move->setPositionX(move->getPosition().x-m_fSpeed*dt);
    forward->setPositionX(move->getPosition().x+m_iPadding+move->getContentSize().width);
    if(move->getPosition().x+move->getContentSize().width<0)
    {
        move->setString(getText()->getCString());
        move->setVisible(false);
        m_bLabel1Move = !m_bLabel1Move;
    }
}

CCString * DMarqueeView::getText()
{
    if(m_pDataSource != NULL)
    {
        CCString * pText = m_pDataSource->getMarqueeText();
        if(pText != NULL)
        {
//            m_pStringStack->removeAllObjects();
            addString(pText);
            return pText;
        }
    }
    
    if(m_pStringStack->count()==0)
    {
        return CCString::create("");
    }
    if(++m_currntPtr>=m_pStringStack->count())
    {
        m_currntPtr=0;
    }
    
    return (CCString *)m_pStringStack->objectAtIndex(m_currntPtr);
}
