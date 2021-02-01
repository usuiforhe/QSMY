//
//  Wonder.cpp
//  奇遇
//
//  Created by cll on 13-4-23.
//
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Wonder.h"
#include "LuaQSMY.h"
#include "LuaProxy/LuaLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;


/************************ wonder class begin*****************************/

Wonder::Wonder()
{

}


Wonder::~Wonder()
{
    
}


void Wonder::runLua()
{    
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    
    pEngine->getLuaStack()->clean();
    
    pEngine->getLuaStack()->pushCCObject(this, "Wonder");
	lua_setglobal(pEngine->getLuaStack()->getLuaState(), "this");
    pEngine->executeString(readLuaScript("scripts/wonder/wonder.lua").c_str());
    
}


/************************ wonder class end*****************************/
