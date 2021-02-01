//
//  Activity.h
//  活动模块
//
//  Created by cll on 13-7-10.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Activity__
#define __QSMY__Activity__

#include "../common/LuaContainer.h"

class Activity
:public LuaContainer
{

public:
	Activity();
	virtual ~Activity();
	CREATE_FUNC(Activity);
    virtual void runLua();
};

#endif /* defined(__QSMY__Activity__) */
