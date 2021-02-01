//
//  Strategy.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Strategy.h"
#include "../Game.h"


USING_NS_CC;
USING_NS_CC_EXT;

StrategyView::StrategyView()
:m_pWebView(NULL)
,m_pContainer(NULL)
{
    
}

StrategyView::~StrategyView()
{
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pWebView);
}

bool StrategyView::init()
{
    if(!DLayer::init()) return false;
    m_pWebView = CCWebView::create();
    CC_SAFE_RETAIN(m_pWebView);
    return true;
}

void StrategyView::onExit()
{
    DLayer::onExit();
    m_pWebView->removeWebView();
}

void StrategyView::onEnter()
{
    DLayer::onEnter();
    m_pWebView->showWebView(DM_GET_PLATFORM_MODEL->getConfig().getSyzzbURL().c_str(),m_pContainer);
}


#pragma mark -

//对应ccb tag
enum StrategyNav
{
    kStrategyView       = 1,
    kStrategyExchange   = 2,
    kBack               = 3,
};

Strategy::Strategy()
:m_pContainer(NULL)
,m_pStrategyExchange(NULL)
,m_pStrategyView(NULL)
,m_pSelected(NULL)
,m_pMenu(NULL)
,m_pWebContainer(NULL)
{
    
}

Strategy::~Strategy()
{
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pStrategyView);
    CC_SAFE_RELEASE(m_pStrategyExchange);
    CC_SAFE_RELEASE(m_pMenu);
    CC_SAFE_RELEASE(m_pWebContainer);
}

bool Strategy::init()
{
    if(!DLayer::init()) return false;
    CCB_READER_AND_ADDCHILD("ui/ccb/strategy/strategy");
    CCMenuItem* pItem = static_cast<CCMenuItem*>(m_pMenu->getChildByTag(kStrategyView));
    navCallBack(pItem);
    return true;
}

void Strategy::onEnter()
{
	DLayer::onEnter();

	NotificationCenter::sharedNotificationCenter()->postNotification(D_STRATEGY_SHOW);
}

void Strategy::onExit()
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_STRATEGY_HIDE);

	DLayer::onExit();
}

bool Strategy::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWebContainer", CCNode *, this->m_pWebContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenu", CCMenu *, this->m_pMenu);
    
    return false;
}

SEL_MenuHandler Strategy::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "navCallBack", Strategy::navCallBack);
    return NULL;
}

void Strategy::showStrategyView()
{
    m_pContainer->removeAllChildren();
    if(m_pStrategyView==NULL)
    {
        m_pStrategyView = StrategyView::create();
        m_pStrategyView->setContainer(m_pWebContainer);
        CC_SAFE_RETAIN(m_pStrategyView);
    }
    m_pContainer->addChild(m_pStrategyView);

}

void Strategy::showStrategyExchange()
{
    m_pContainer->removeAllChildren();
    if(m_pStrategyExchange==NULL)
    {
        m_pStrategyExchange = StrategyExchange::create();
        CC_SAFE_RETAIN(m_pStrategyExchange);
    }
    m_pContainer->addChild(m_pStrategyExchange);
}

void Strategy::navCallBack(cocos2d::CCObject* obj)
{
    if(m_pSelected) m_pSelected->setEnabled(true);
    CCMenuItem* pItem = static_cast<CCMenuItem*>(obj);
    m_pSelected = pItem;
    pItem->setEnabled(false);
    int tag = pItem->getTag();
    
    switch (tag)
    {
        case kStrategyView:         //ccb显示网页
            showStrategyView();
            break;
        case kStrategyExchange:         //ccb 兑换
            showStrategyExchange();
            break;
        case kBack:         //ccb 返回
            Game::sharedGame()->NavigateTo(navigation::kNavHome);
            break;
        default:
            break;
    }
}
