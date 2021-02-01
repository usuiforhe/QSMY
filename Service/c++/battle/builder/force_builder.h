//
//  force_builder.h
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__force_builder__
#define __battle__force_builder__

#include "battle_prefix.h"
#include "meta/scene_meta_data.pb.h"
#include "singleton.h"
#include "force/force.h"

namespace battle {
    class scene;
    class force_builder{
    public:
        force_builder(){}
        ~force_builder(){}
        
        force * build(scene * scene, uint8_t id, const battle::force_meta_data & data);
        
        force * build(scene * scene, uint8_t id, const lua_object & obj);
        
        force * build(scene * scene, uint8_t id, const Json::Value & json);
        
    private:
        inline void add_user(force * force, user * user);
    };
}

#define S_FORCE_BUILDER latte::singleton<force_builder>::instance()

#endif /* defined(__battle__force_builder__) */
