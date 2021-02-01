//
//  LuaLoader.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "LuaLoader.h"
#include <string>
#include <algorithm>

using namespace cocos2d;

extern "C"
{
    int lua_loader(lua_State *L)
    {
        std::string filename(luaL_checkstring(L, 1));
        size_t pos = filename.rfind(".lua");
        if (pos != std::string::npos)
        {
            filename = filename.substr(0, pos);
        }
        
        pos = filename.find_first_of(".");
        while (pos != std::string::npos)
        {
            filename.replace(pos, 1, "/");
            pos = filename.find_first_of(".");
        }
        filename.append(".lua");
        
        std::string codeBuffer = readLuaScript(filename.c_str());
               
        if (!codeBuffer.empty())
        {
            if (luaL_loadbuffer(L, codeBuffer.c_str(), codeBuffer.length(), filename.c_str()) != 0)
            {
                luaL_error(L, "error loading module %s from file %s :\n\t%s",
                           lua_tostring(L, 1), filename.c_str(), lua_tostring(L, -1));
            }
        }
        else
        {
            CCLOG("can not get file data of %s", filename.c_str());
        }
        
        return 1;
    }
}