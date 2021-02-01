//
//  TimerUtil.h
//  计时器
//
//  Created by wanghejun on 13-7-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TimerUtil__
#define __QSMY__TimerUtil__

#include "AppInclude.h"


class TimerUtil:public cocos2d::CCScheduler
{
    
public:
    static TimerUtil* sharedTimerUtil(void);
    static void purgeTimerUtil(void);
    virtual void update(float dt);
    void scheduleOnce(cocos2d::SEL_SCHEDULE selector,cocos2d::CCObject* target, float delay);
    void schedule(cocos2d::SEL_SCHEDULE selector,cocos2d::CCObject* target, float interval);
private:
    void calculateDeltaTime(void);
    TimerUtil();
    virtual ~TimerUtil();
private:
    float   m_fDeltaTime;
    double  m_dLastUpdate;
    bool    inited;
};

#endif /* defined(__QSMY__TimerUtil__) */
