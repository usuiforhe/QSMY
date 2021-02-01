//
//  Weekly.h
//  周活动模块
//
//  Created by Eci on 14-5-14.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__Weekly__
#define __QSMY__Weekly__

#include "../common/LuaContainer.h"

class Weekly
:public LuaContainer
{

public:
	Weekly();
	virtual ~Weekly();
	CREATE_FUNC(Weekly);
    virtual void runLua();

	virtual void onBackKey();
};

#endif /* defined(__QSMY__Weekly__) */
