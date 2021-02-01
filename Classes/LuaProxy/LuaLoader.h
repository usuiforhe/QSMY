//
//  LuaLoader.h
//  lua 加载器
//
//  Created by wanghejun on 13-9-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__LuaLoader__
#define __QSMY__LuaLoader__

#include "cocos2d.h"
#include "AppConsts.h"

#ifdef CRYPT
#include "utils/aes.h"
#endif

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
    
    extern int lua_loader(lua_State *L);
}

static std::string readLuaScript(const char* pszFileName)
{
    std::string fileName(pszFileName);
    fileName+= SUFFIX;
    unsigned long codeBufferSize = 0;
    unsigned char* codeBuffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fileName.c_str(), "rb", &codeBufferSize);
    std::string string((char*)codeBuffer,codeBufferSize);
#ifdef CRYPT
    string = DecodeAES(LUA_PASSWORD, string);
#endif
    CC_SAFE_DELETE_ARRAY(codeBuffer);
    return string;
}

#endif /* defined(__QSMY__LuaLoader__) */
