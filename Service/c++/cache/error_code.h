//
//  error_code.h
//  cache
//
//  Created by 汪燕军 on 13-5-8.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef cache_error_code_h
#define cache_error_code_h

namespace cache {
    typedef enum error_code{
        //成功
        OPT_SUCCESS = 0,
//-------------道具相关---------------
        //道具不足
        NOT_ENOUGH_ITEM = 1401,
//-------------装备相关---------------
        //装备已存在
        ALREADY_HAS_EQUIP = 2401,
        //装备不存在
        NO_SUCH_EQUIP = 2402,
//-------------魂魄相关---------------
        //魂魄不足
        NOT_ENOUGH_SOUL = 1306,
//-------------天命相关---------------
        //天命不存在
        DESTINY_NOT_EXIST = 2304,
//-------------弟子相关---------------
        //弟子已存在
        WARRIOR_EXIST = 1305,
        //弟子不存在
        WARRIOR_NOT_EXIST = 1307,
//-------------时装相关---------------
        //时装已存在
        DRESS_EXIST = 3901,
        //时装不存在
        DRESS_NOT_EXIST = 3902,
//-------------系统相关---------------
        //参数错误
        INVALID_ARGUMENT = 9002,
        //系统错误
        SYS_ERROR = 9999
    } error_code;
}

#endif
