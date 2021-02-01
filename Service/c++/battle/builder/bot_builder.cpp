//
//  bot_builder.cpp
//  battle
//
//  Created by 汪燕军 on 13-4-15.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "bot_builder.h"
using namespace battle;

force_meta_data bot_builder::build(uint32_t bot_id, lua_State * L){
    lua_object obj = S_SCRIPT_LOADER.load_bot(bot_id, L);
    force_meta_data force;
    
    force.set_name("");
    force.set_leader_sid(luabind::object_cast<uint32_t>(obj["leader_sid"]));
    
    force.set_level(luabind::object_cast<uint16_t>(obj["level"]));
    force.set_is_mob(true);
    force.set_user_id(MOB_USER_ID);
    
    for(luabind::iterator pos(obj["formation"]), end; pos != end; ++pos){
        lua_object position_unit = *pos;
        unit_meta_data * unit = force.add_units();
        unit->set_sid(luabind::object_cast<uint32_t>(position_unit["sid"]));
        
        unit->set_level(luabind::object_cast<uint32_t>(position_unit["level"]));
        unit->set_hp(luabind::object_cast<uint32_t>(position_unit["hp"]));
        unit->set_max_hp(luabind::object_cast<uint32_t>(position_unit["hp"]));
        unit->set_attack(luabind::object_cast<uint32_t>(position_unit["attack"]));
        unit->set_defense(luabind::object_cast<uint32_t>(position_unit["defense"]));
        unit->set_speed(luabind::object_cast<uint32_t>(position_unit["speed"]));
        unit->set_dodge_rate(luabind::object_cast<uint32_t>(position_unit["dodge_rate"]));
        unit->set_critical_rate(luabind::object_cast<uint32_t>(position_unit["critical_rate"]));
        unit->set_block_rate(luabind::object_cast<uint32_t>(position_unit["block_rate"]));
        unit->set_position(luabind::object_cast<uint32_t>(position_unit["position"]));
        unit->set_size(1);
        unit->set_up(0);
        unit->set_skill_fight(luabind::object_cast<float>(position_unit["skill_fight"]));
        unit->set_is_boss(false);
        
        for(luabind::iterator s_pos(position_unit["skills"]), s_end; s_pos != s_end; ++s_pos){
            skill_meta_data * skill = unit->add_skills();
            skill->set_sid(luabind::object_cast<uint32_t>((*s_pos)["sid"]));
            skill->set_level(luabind::object_cast<uint32_t>((*s_pos)["level"]));
        }
    }
    
    return force;
}

void bot_builder::load_bot_name(lua_State * L, std::vector<boost::shared_ptr<bot_name> > & list){
    lua_object obj = S_SCRIPT_LOADER.load_bot_name(L);
    for(luabind::iterator pos(obj), end; pos != end; ++pos){
        boost::shared_ptr<bot_name> name = boost::make_shared<bot_name>(luabind::object_cast<uint32_t>((*pos)["id"]), luabind::object_cast<std::string>((*pos)["name"]));
        
        list.push_back(name);
    }
}
