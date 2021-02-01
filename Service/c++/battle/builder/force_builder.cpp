//
//  force_builder.cpp
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "force_builder.h"
#include "user_builder.h"
#include "force/force.h"
#include "game/scene.h"
#include "unit/unit.h"
using namespace battle;

battle::force * force_builder::build(battle::scene *scene, uint8_t id, const ::force_meta_data &data){
    battle::force * force = new battle::force(id, scene);
    
    if(data.has_name()){
        force->name_ = data.name();
    }
    battle::user * user = S_USER_BUILDER.build(force, data);
    add_user(force, user);
    
    return force;
}

force * force_builder::build(battle::scene *scene, uint8_t id, const lua_object & obj){
    battle::force * force = new battle::force(id, scene);
    battle::user * user = S_USER_BUILDER.build(force, obj);
    add_user(force, user);
    
    return force;
}

force * force_builder::build(battle::scene *scene, uint8_t id, const Json::Value & json){
    battle::force * force = new battle::force(id, scene);
    
    for(BOOST_AUTO(pos, json["users"].begin()); pos != json["users"].end(); ++pos){
        battle::user * user = S_USER_BUILDER.build(force, *pos);
        add_user(force, user);
    }
    
    return force;
}

void force_builder::add_user(battle::force *force, battle::user *user){  
    force->total_speed_ += user->get_total_speed();
    
    if(user->get_max_speed() > force->max_speed_){
        force->max_speed_ = user->get_max_speed();
    }
    
    if(force->get_name() == ""){
        force->name_ = user->get_name();
    }
    force->users_.push_back(user);
    if(user->get_is_mob()){
        force->is_mob_ = true;
    }
}


