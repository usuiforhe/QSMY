//
//  user_builder.h
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__user_builder__
#define __battle__user_builder__

#include "battle_prefix.h"
#include "force/user.h"
#include "meta/scene_meta_data.pb.h"

namespace battle {
    class user_builder{
    public:
        user_builder(){}
        ~user_builder(){}
        
        user * build(force * force, const battle::force_meta_data & data);
        
        user * build(force * force, const lua_object & obj);
        
        user * build(force * force, const Json::Value & json);
    };
}
#define S_USER_BUILDER latte::singleton<user_builder>::instance()

#endif /* defined(__battle__user_builder__) */
