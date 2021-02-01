//
//  Weekly.cpp
//  活动
//
//  Created by cll on 13-7-10.
//
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Weekly.h"
#include "LuaQSMY.h"
#include "LuaProxy/LuaLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;

/************************ Weekly class begin*****************************/

Weekly::Weekly()
{
    
}

Weekly::~Weekly()
{
}

void Weekly::runLua()
{
    // register lua engine
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    pEngine->getLuaStack()->clean();
    
    pEngine->getLuaStack()->pushCCObject(this, "Weekly");
    lua_setglobal(pEngine->getLuaStack()->getLuaState(), "this");
    //pEngine->executeScriptFile("weekly.weekly");
	pEngine->executeString(readLuaScript("scripts/weekly/weekly.lua").c_str());
}


void Weekly::onBackKey()
{
	if (this->onBackKeyForInfoView() == false)
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_WEEKLY_ONBACK);
	}
}

/************************ Weekly class end*****************************/

