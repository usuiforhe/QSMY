//
//  script_util.h
//  battle
//
//  Created by wangyanjun on 13-3-19.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef battle_script_util_h
#define battle_script_util_h

#include "battle_prefix.h"
#include "util.h"
#include "log/log.h"
#include "random/random_helper.h"

namespace battle {    
    static void script_output_debug(std::string text)
    {
        s_log.out_debug("lua:" + text);
    }
    
    static void script_out_string(std::string text)
    {
    	s_log.out_string("lua:" + text);
    }

    static double script_get_random(int min, int max)
    {
        if (min == max)
        {
            return min;
        }
        
        if (min > max)
        {
            std::swap(min, max);
        }
        
        return get_random(min, max);
    }
    
    static bool script_check_prob(int prob)
    {
        if(prob == 0)
        {
            return false;
        }
        
        if(prob == 100)
        {
            return true;
        }

        return get_random(1, 100) <= prob;
    }

    static int lua_call_error(lua_State *L)
    {
        lua_Debug d;
        lua_getstack(L, 1, &d);
        lua_getinfo(L, "Sln", &d);
        std::string err = lua_tostring(L, -1);
        lua_pop(L, 1);
        std::stringstream msg;
        msg << d.short_src << ":" << d.currentline;
        
        if (d.name != 0)
        {
            msg << "(" << d.namewhat << " " << d.name << ")";
        }
        msg << " " << err;
        lua_pushstring(L, msg.str().c_str());
        
        s_log.out_error("lua script throws error:" + msg.str());
        return 1;
    }
}

#endif
