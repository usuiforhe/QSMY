//
//  unit_builder.h
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__unit_builder__
#define __battle__unit_builder__

#include "battle_prefix.h"
#include "meta/scene_meta_data.pb.h"
#include "singleton.h"
#include "force/force.h"

namespace battle {
    class scene;
    class base_force;
    class unit;
    class unit_builder{
    public:
        unit_builder(){}
        
        ~unit_builder(){}
        
        unit * build(user * user, const battle::unit_meta_data & data);
        
        unit * build(user * user, const lua_object & obj);
        
        unit * build(user * user, const Json::Value & json);
        
    private:
        inline void set_position(scene * scene, force * force, user * user, unit * unit, uint8_t position_id, uint8_t size);
    };
}

#define S_UNIT_BUILDER latte::singleton<unit_builder>::instance()

#endif /* defined(__battle__unit_builder__) */
