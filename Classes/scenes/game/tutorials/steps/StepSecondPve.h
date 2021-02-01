//
//  StepSecondPve.h
//  第二次探索
//
//  Created by heven on 14-3-9.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__StepSecondPve__
#define __QSMY__StepSecondPve__

#include "../Tutorial.h"
class StepSecondPve
:public StepEnForce
{
public:
    StepSecondPve();
    virtual void nextSubStep();     //下一小步
    virtual void continueStep();
private:
    bool m_bNeedFix; //是否需要修正
};


#endif /* defined(__QSMY__StepSecondPve__) */
