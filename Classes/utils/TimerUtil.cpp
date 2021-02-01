//
//  TimerUtil.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TimerUtil.h"
#include "../managers/GameManager.h"

USING_NS_CC;

// singleton stuff
static TimerUtil *s_SharedTimerUtil = NULL;

TimerUtil::TimerUtil()
:m_fDeltaTime(0.f)
,m_dLastUpdate(0.0)
,inited(false)
{
    
}

TimerUtil::~TimerUtil()
{
    
}

TimerUtil* TimerUtil::sharedTimerUtil(void)
{
    if (!s_SharedTimerUtil)
    {
        s_SharedTimerUtil = new TimerUtil();
        CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(s_SharedTimerUtil, 0, false);
    }
    
    return s_SharedTimerUtil;
}


void TimerUtil::purgeTimerUtil(void)
{
    if(s_SharedTimerUtil==NULL) return;
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(s_SharedTimerUtil);
    CC_SAFE_RELEASE_NULL(s_SharedTimerUtil);
}


void TimerUtil::update(float dt)
{
    if(!GameManager::sharedGameManager()->isServerTimeSynced()) return;
    if(!inited)
    {
        m_dLastUpdate = GameManager::sharedGameManager()->getServerTimeMilliSecond() - dt;
        inited = true;
    }
    calculateDeltaTime();
    CCScheduler::update(m_fDeltaTime);
}

void TimerUtil::calculateDeltaTime(void)
{
    double now = GameManager::sharedGameManager()->getServerTimeMilliSecond();
    m_fDeltaTime = (now - m_dLastUpdate)/1000.0f;
    m_fDeltaTime = MAX(0, m_fDeltaTime);
    m_dLastUpdate = now;
}

void TimerUtil::scheduleOnce(cocos2d::SEL_SCHEDULE selector,cocos2d::CCObject* target, float delay)
{
    scheduleSelector(selector, target, 0.0f , 0, delay, false);
}

void TimerUtil::schedule(cocos2d::SEL_SCHEDULE selector,cocos2d::CCObject* target, float interval)
{
    scheduleSelector(selector,target, interval, kCCRepeatForever, 0.0f,false);
}
