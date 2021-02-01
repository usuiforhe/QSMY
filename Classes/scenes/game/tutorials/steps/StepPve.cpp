//
//  StepPve.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-22.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "StepPve.h"
#include "../../../../managers/DataManager.h"
#include "../../../../managers/ResourceManager.h"

USING_NS_CC;


void StepPve::nextSubStep()
{
    if(!m_pStepInfo->isTrigger()) return;
    simTouch();
    removeAllChildren();
    if(m_nCurrentSubStep==10)
    {
        setVisible(false);
        NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(StepPve::continueStep),D_EVENT_SHOW_PVE_SETTLEMENT);
        return;
    }
    ++m_nCurrentSubStep;
    if(m_nCurrentSubStep>m_pStepInfo->getSteps())
    {
        nextStep(); //下一步
        return;
    }
    loadUI();
    updateContentInfo();
}

void StepPve::continueStep()
{
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    setVisible(true);
    ++m_nCurrentSubStep;
    if(m_nCurrentSubStep>m_pStepInfo->getSteps())
    {
        nextStep(); //下一步
        return;
    }
    loadUI();
    updateContentInfo();
}