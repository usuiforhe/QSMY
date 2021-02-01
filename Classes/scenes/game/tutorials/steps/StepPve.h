//
//  StepPve.h
//  探索
//
//  Created by wanghejun on 13-8-22.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__StepPve__
#define __QSMY__StepPve__

#include "../Tutorial.h"
class StepPve
:public StepEnForce
{
public:
    virtual void nextSubStep();     //下一小步
    virtual void continueStep();
};


#endif /* defined(__QSMY__StepPve__) */
