//
//  buff_builder.cpp
//  battle
//
//  Created by wangyanjun on 13-3-19.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "buff_builder.h"
#include "util/script_loader.h"
#include "game/scene.h"
#include "unit/unit.h"
#include "buff/unit_frames_buff_expire_strategy.h"

using namespace battle;

unit_buff * buff_builder::build(unit * owner, uint32_t sid, uint32_t source_unit_id, uint32_t source_skill_id, const lua_object & meta_data){
    lua_object script = S_SCRIPT_LOADER.load_buff(sid, owner->get_scene()->get_lua_state());
    
    if(!script.is_valid() || luabind::type(script) == LUA_TNIL){
        return NULL;
    }
    
    bool is_visible = luabind::object_cast<bool>(meta_data["visible"]);
    buff_expire_t expire_type = static_cast<buff_expire_t>(luabind::object_cast<uint32_t>(meta_data["expire_type"]));
    uint8_t interval = luabind::object_cast<uint8_t>(meta_data["interval"]);
    
    unit_buff * buff = new unit_buff(sid, is_visible, script, owner, source_unit_id, source_skill_id, meta_data["arg"], interval);
    
    switch(expire_type){
        case BUFF_EXPIRE_TYPE_UNIT_FRAMES:
        default:
            uint32_t last_frames = luabind::object_cast<uint32_t>(meta_data["last"]);
            buff->expire_strategy_ = new unit_frames_buff_expire_strategy(buff, owner, last_frames);
            break;
    }
    
    return buff;
}
