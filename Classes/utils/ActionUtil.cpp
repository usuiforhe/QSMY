//
//  ActionUtil.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ActionUtil.h"


CCNumberBy* CCNumberBy::create(float duration,float from,float delta,SEL_TWEEN_NUM fun)
{
    CCNumberBy *pRet = new CCNumberBy();
    pRet->initWithDuration(duration, from,delta,fun);
    pRet->autorelease();
    
    return pRet;
}

bool CCNumberBy::initWithDuration(float duration,float from,float delta,SEL_TWEEN_NUM fun)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_start = from;
        m_delta = delta;
        m_fun = fun;
        return true;
    }
    return false;
}


void CCNumberBy::update(float t)
{
    if (m_pTarget)
    {
        if (m_fun)
        {
            (m_pTarget->*m_fun)(m_start+m_delta*t);
        }
    }
}



CCNumberTo* CCNumberTo::create(float duration,float from,float to,SEL_TWEEN_NUM fun)
{
    CCNumberTo *pRet = new CCNumberTo();
    pRet->initWithDuration(duration, from,to,fun);
    pRet->autorelease();
    return pRet;
}

bool CCNumberTo::initWithDuration(float duration,float from,float to,SEL_TWEEN_NUM fun)
{
    if (CCNumberBy::initWithDuration(duration,from,to-from,fun))
    {
        return true;
    }
    return false;
}