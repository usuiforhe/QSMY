//
//  Activity.cpp
//  活动
//
//  Created by cll on 13-7-10.
//
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Activity.h"
#include "LuaQSMY.h"
#include "LuaProxy/LuaLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;

/************************ Activity class begin*****************************/

Activity::Activity()
{
    
}

Activity::~Activity()
{
}

void Activity::runLua()
{
    // register lua engine
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    pEngine->getLuaStack()->clean();
    
    pEngine->getLuaStack()->pushCCObject(this, "Activity");
    lua_setglobal(pEngine->getLuaStack()->getLuaState(), "this");
    pEngine->executeString(readLuaScript("scripts/activity/activity.lua").c_str());
}


/************************ Activity class end*****************************/

