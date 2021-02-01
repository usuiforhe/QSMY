//
//  BaseScenes.h
//  场景
//
//  Created by wanghejun on 13-12-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__BaseScenes__
#define __QSMY__BaseScenes__

#include "components/components.h"

class BaseScenes :public DRoot
{
    
public:
    BaseScenes();
    virtual ~BaseScenes();
    virtual bool init();
    virtual void keyBackClicked();
};

#endif /* defined(__QSMY__BaseScenes__) */
