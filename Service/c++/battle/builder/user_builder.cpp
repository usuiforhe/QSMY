//
//  user_builder.cpp
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "user_builder.h"
#include "unit_builder.h"
#include "game/scene.h"
#include "force/force.h"
using namespace battle;

battle::user * user_builder::build(battle::force *force, const battle::force_meta_data & data){
    battle::user * user = new battle::user(force, data.user_id(), data.level(), data.is_mob());
    
    BOOST_FOREACH(const battle::unit_meta_data & unit_data, data.units()){
        S_UNIT_BUILDER.build(user, unit_data);
    }
    
    return user;
}

battle::user * user_builder::build(battle::force *force, const lua_object &obj){
    uint16_t level = luabind::object_cast<uint16_t>(obj["level"]);
    battle::user * user = new battle::user(force, MOB_USER_ID, level, true);
    for(luabind::iterator pos(obj["mobs"]), end; pos != end; ++pos){
        S_UNIT_BUILDER.build(user, *pos);
    }
    
    return user;
}

battle::user * user_builder::build(battle::force *force, const Json::Value & json){
    uint32_t user_id = json["user_id"].asUInt();
    uint16_t level = static_cast<uint16_t>(json["level"].asUInt());
    bool is_mob = json["is_mob"].asBool();
    battle::user * user = new battle::user(force, user_id, level, is_mob);
    
    for(BOOST_AUTO(pos, json["units"].begin()); pos != json["units"].end(); ++pos){
        S_UNIT_BUILDER.build(user, *pos);
    }
    
    return user;
}
