//
//  DPopup.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DPopup.h"

USING_NS_CC;

DPopup::DPopup()
:m_pCloseButton(NULL)
,m_PopuppDelegate(NULL)
,m_pContainer(NULL)
,m_pBg(NULL)
,m_bInited(false)
,m_bAnimate(false)
,m_bBackKeyEnabled(true)
{
    
}

DPopup::~DPopup()
{
    CC_SAFE_RELEASE(m_pCloseButton);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pBg);
}

bool DPopup::init(bool animate)
{
    m_pBg = CCLayerColor::create(ccc4(0,0,0,205));
	m_pBg->setContentSize(cocos2d::CCSizeMake(cocos2d::CCDirector::sharedDirector()->getWinSize().width , DESIGN_SIZE.height));
    CC_SAFE_RETAIN(m_pBg);
    addChild(m_pBg);
    
    m_pContainer = CCNode::create();
    m_pContainer->setContentSize(DESIGN_SIZE);
    m_pContainer->setAnchorPoint(D_ANCHOR_POINT_CENTER);
    m_pContainer->setPosition(D_DESIGN_POINT_CENTER);
    CC_SAFE_RETAIN(m_pContainer);
    addChild(m_pContainer);
    m_bAnimate = animate;
	m_bInited = true;
	PL_MOVE_NODE_BELOW_CENTER_Y(this);
    return true;
}

bool DPopup::init()
{
    return init(false);
}

void DPopup::addChild(cocos2d::CCNode *child)
{
    addChild(child,0,0);
}

void DPopup::addChild(CCNode *child, int zOrder, int tag)
{
    if(!m_bInited)
    {
        CCLayer::addChild(child,zOrder,tag);
        return;
    }
    m_pContainer->addChild(child,zOrder,tag);
}

void DPopup::removeAllChildrenWithCleanup(bool cleanup)
{
    this->m_pContainer->removeAllChildrenWithCleanup(cleanup);
}

void DPopup::setOpacity(GLubyte opacity)
{
    m_pBg->setOpacity(opacity);
}

void DPopup::createCloseMenu()
{
    m_pCloseButton = CCMenuItemFont::create("close", this, menu_selector(DPopup::doClosePopup));
    m_pCloseButton->setPosition(ccpSub(D_DESIGN_POINT_RIGHTTOP,
                                       ccp(m_pCloseButton->getContentSize().width,m_pCloseButton->getContentSize().height)));
    m_pCloseButton->retain();
    CCMenu * pMenu = CCMenu::create(m_pCloseButton,NULL);
    pMenu->setTouchEnabled(false);
    pMenu->setPosition(CCPointZero);
    
    CCLayer::addChild(pMenu,1000);
}

void DPopup::removeFromParentAndCleanup(bool cleanup)
{
    DPopupLayer* pParent = dynamic_cast<DPopupLayer*>(m_pParent);
    CCLayer::removeFromParentAndCleanup(cleanup);
    if (pParent != NULL)
    {
        pParent->update(0.f);
    }
}

//取消所有CCLayer的默认触摸事件
static void setChildrenTouchDisable( cocos2d::CCNode* pParent )
{
    cocos2d::CCArray* pChildren = pParent->getChildren();
    if( !pChildren )
    {
        return;
    }
    int iNumChildren = pChildren->count();
    for( int i = 0; i < iNumChildren; ++i )
    {
        cocos2d::CCLayer* pLayer = NULL;
        cocos2d::CCNode* pChild = ( cocos2d::CCNode* )pChildren->objectAtIndex( i );
        if (( pLayer = dynamic_cast< cocos2d::CCLayer* >( pChild )) )
        {
            pLayer->setTouchEnabled( false );
        }
        
        setChildrenTouchDisable( pChild );
    }
}

void DPopup::onEnter()
{
    setChildrenTouchDisable(this);
    CCLayer::onEnter();
    if(m_PopuppDelegate!=NULL) m_PopuppDelegate->onPopupOpen(this);
    if(m_bAnimate) showAnimate();
}

void DPopup::showAnimate()
{
    m_pContainer->setScale(0.0f);
    m_pContainer->runAction(CCEaseBackOut::create(CCScaleTo::create(0.3f, 1.f)));
}

void DPopup::closePopup()
{
    if(m_PopuppDelegate!=NULL) m_PopuppDelegate->onPopupClose(this);
    removeFromParent();
}

bool DPopup::isOpening()
{
    return m_pParent != NULL;
}

void DPopup::doClosePopup(CCObject* obj)
{
    closePopup();
}

void DPopup::onBackKey()
{
	if(m_bBackKeyEnabled)
	{
		if (m_PopuppDelegate)
		{
			m_PopuppDelegate->onPopupBack(this);
		}
		else
		{
			removeFromParent();
		}
	}	
}

#pragma mark DPopupLayer

DPopupLayer::DPopupLayer()
:deque(NULL)
{
    
}

DPopupLayer::~DPopupLayer()
{
    CC_SAFE_RELEASE(deque);
}


bool DPopupLayer::init()
{
    if(!DLayer::init()) return false;
    deque = CCArray::create();
    CC_SAFE_RETAIN(deque);
    return true;
}

void DPopupLayer::addChild(CCNode *child, int zOrder, int tag)
{
    CCAssert(0, "plz use addNode");
}

void DPopupLayer::openPopup(DPopup* pNode)
{
    if(getChildrenCount()==0 && isRunning() && isVisible())
    {
        CCNode::addChild(pNode, 0, -1);
    }else
    {
        if(deque->count()==0)
        {
            if(isVisible()) scheduleUpdate();
        }
        
        DPopup * pPopup = NULL;
        uint32_t i = 0;
        for (; i<deque->count(); ++i)
        {
            pPopup = static_cast<DPopup*>(deque->objectAtIndex(i));
            if (pNode->getPriority() < pPopup->getPriority())
            {
                break;
            }
        }
        deque->insertObject(pNode,i);
    }
}

void DPopupLayer::onEnter()
{
    DLayer::onEnter();
    update(0.f);
}

void DPopupLayer::update(float dt)
{
    if(getChildrenCount()==0)
    {
        if(deque->count()>0)
        {
            DPopup* pNode = static_cast<DPopup*>(deque->objectAtIndex(0));
            CCNode::addChild(pNode, 0, -1);
            deque->removeObjectAtIndex(0);
        }
        if(deque->count()==0)
        {
            unscheduleUpdate();
        }
    }
}

void DPopupLayer::setVisible(bool val)
{
    DLayer::setVisible(val);
    if(val)
    {
        if(deque->count()>0)
        {
            scheduleUpdate();
            update(0);
        }
    }else
    {
        unscheduleUpdate();
    }
}

DPopup *DPopupLayer::getPopingUp()
{
	if (getChildrenCount() && isRunning() && isVisible())
	{
		return static_cast<DPopup *>(getChildren()->lastObject());
	}
	return NULL;
}
