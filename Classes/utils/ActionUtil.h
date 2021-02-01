//
//  ActionUtil.h
//  QSMY
//
//  Created by wanghejun on 13-8-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ActionUtil__
#define __QSMY__ActionUtil__

#include "cocos2d.h"

typedef void (cocos2d::CCObject::*SEL_TWEEN_NUM)(float);

#define tween_num_selector(_SELECTOR) (SEL_TWEEN_NUM)(&_SELECTOR)

class  CCNumberBy : public cocos2d::CCActionInterval
{
public:
    /** initializes the action */
    bool initWithDuration(float duration,float from,float delta,SEL_TWEEN_NUM fun);
    
    virtual void update(float time);
    
public:
    /** creates the action */
    static CCNumberBy* create(float duration,float from,float delta,SEL_TWEEN_NUM fun);
    
protected:
    float m_delta;
    float m_start;
    float m_previous;
    SEL_TWEEN_NUM m_fun;
};


class  CCNumberTo : public CCNumberBy
{
public:
    /** initializes the action */
    bool initWithDuration(float duration,float from,float to,SEL_TWEEN_NUM fun);
public:
    /** creates the action */
    static CCNumberTo* create(float duration,float from,float to,SEL_TWEEN_NUM fun);
    
};


#endif /* defined(__QSMY__ActionUtil__) */
