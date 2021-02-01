//
//  skill_builder.cpp
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "skill_builder.h"
#include "unit/unit.h"
#include "util/script_loader.h"
#include "game/scene.h"
#include "skill/skill.h"

using namespace battle;

boost::shared_ptr<battle::skill> skill_builder::build(battle::unit * unit, const battle::skill_meta_data &data){
    return build(unit, data.sid(), data.level());
}

boost::shared_ptr<battle::skill> skill_builder::build(battle::unit * unit, const Json::Value & json){
    return build(unit, json["sid"].asUInt(), json["level"].asUInt());
}

boost::shared_ptr<battle::skill> skill_builder::build(battle::unit * unit, const lua_object & obj){
    return build(unit, luabind::object_cast<uint32_t>(obj["sid"]), luabind::object_cast<uint32_t>(obj["level"]));
}

boost::shared_ptr<battle::skill> skill_builder::build(unit * unit, uint32_t sid, uint32_t level){
    lua_object obj = S_SCRIPT_LOADER.load_skill(sid, unit->get_scene()->get_lua_state());
    
    if(!obj.is_valid() || luabind::type(obj) == LUA_TNIL){
        return boost::shared_ptr<battle::skill>();
    }
    

    uint8_t skill_type = luabind::object_cast<uint8_t>(obj["skill_type"]);
    uint8_t unit_up = luabind::object_cast<uint8_t>(obj["unit_up"]);

    uint32_t cd = luabind::object_cast<uint32_t>(obj["cd"]);
    uint8_t trigger_rate = luabind::object_cast<uint8_t>(obj["trigger_rate"]);
    bool visible = luabind::object_cast<bool>(obj["visible"]);
    lua_object bind_events = obj["bind_event"];
    
    boost::shared_ptr<battle::skill> skill = boost::make_shared<battle::skill>(sid, cd, level, obj, unit, trigger_rate, visible);
    
    skill->skill_type_ = skill_type;
    skill->unit_up_ = unit_up;

    lua_object priority_obj = obj["priority"];
    if(priority_obj.is_valid() && luabind::type(priority_obj) != LUA_TNIL){
        skill->priority_ = luabind::object_cast<uint8_t>(priority_obj);
    }
    lua_object prd_n_obj = obj["prd_n"];
    if(prd_n_obj.is_valid() && luabind::type(prd_n_obj) != LUA_TNIL){
        skill->prd_n_ = luabind::object_cast<uint8_t>(prd_n_obj);
    }
    
    uint32_t event_id(0);
    for(luabind::iterator pos(bind_events), end; pos != end; ++pos){
        event_id = luabind::object_cast<uint32_t>(*pos);
        unit->bind_skill(event_id, skill);
    }
    
    return skill;
}
