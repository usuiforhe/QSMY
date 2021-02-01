//
//  BusyLayer.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "BusyLayer.h"
#include "managers/ResourceManager.h"

BusyLayer::BusyLayer()
:busyCount(0)
,m_pLoading(NULL)
{
    
}

BusyLayer::~BusyLayer()
{
    CC_SAFE_RELEASE(m_pLoading);
}

bool BusyLayer::init()
{
    if(!DLayerModal::init()) return false;
    removeAllChildren();
    CC_SAFE_RELEASE(m_pLoading);
    m_pLoading = ResourceManager::sharedResourceManager()->getUIAnimate("loading");
    CC_SAFE_RETAIN(m_pLoading);
    m_pLoading->setPosition(D_DESIGN_POINT_CENTER);
    
    updateBusyLayer();
    return true;
}

void BusyLayer::showBusy()
{
    busyCount++;
    updateBusyLayer();
}

void BusyLayer::hideBusy()
{
    CC_ASSERT(busyCount>0);
    busyCount--;
    updateBusyLayer();
}

void BusyLayer::updateBusyLayer()
{
    BaseLayer::setTouchEnabled(busyCount>0);
    setVisible(busyCount>0);
    if(busyCount>0)
    {
        if(m_pLoading->getParent()==NULL)
        {
            addChild(m_pLoading);
        }
    }else
    {
        m_pLoading->removeFromParent();
    }
}