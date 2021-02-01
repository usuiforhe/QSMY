//
//  SystemMaintain.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SystemMaintain.h"
#include "managers/GameManager.h"
#include "managers/PlatformManager.h"

USING_NS_CC;

// singleton stuff
static SystemMaintain *s_SharedSystemMaintain = NULL;

SystemMaintain* SystemMaintain::sharedSystemMaintain(void)
{
    if (!s_SharedSystemMaintain)
    {
        s_SharedSystemMaintain = new SystemMaintain();
        s_SharedSystemMaintain->init();
    }
    return s_SharedSystemMaintain;
}


void SystemMaintain::purgeSystemMaintain(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedSystemMaintain);
}

SystemMaintain::SystemMaintain()
:m_pWebView(NULL)
,m_pCloseButton(NULL)
,m_pContainer(NULL)
{
    
}

SystemMaintain::~SystemMaintain()
{
    m_pWebView->removeWebView();
    CC_SAFE_RELEASE(m_pWebView);
}

bool SystemMaintain::init()
{
    if(!CCNode::init()) return false;
    
    CCB_READER_AND_ADDCHILD("ui/ccb/system/SystemMaintain");
    
    m_pWebView = CCWebView::create();
    CC_SAFE_RETAIN(m_pWebView);
    
    m_pCloseButton->getButton()->setTarget(this, menu_selector(SystemMaintain::close));
    
    return true;
}

void SystemMaintain::onEnter()
{
    CCLayer::onEnter();//要addChild才能取得舞台坐标
    m_pWebView->showWebView(DM_GET_PLATFORM_MODEL->getMaintainURL().c_str(),m_pContainer);
}


bool SystemMaintain::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    
    return false;
}

void SystemMaintain::close(cocos2d::CCObject*)
{
    purgeSystemMaintain();
    removeFromParent();
}

