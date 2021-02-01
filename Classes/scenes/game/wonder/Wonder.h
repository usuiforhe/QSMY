//
//  Wonder.h
//  奇遇
//
//  Created by cll on 13-4-23.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Wonder__
#define __QSMY__Wonder__

#include "../common/LuaContainer.h"

class Wonder
:public LuaContainer
{
public:
	Wonder();
	virtual ~Wonder();
	CREATE_FUNC(Wonder);
    virtual void runLua();
};

#endif /* defined(__QSMY__Wonder__) */
