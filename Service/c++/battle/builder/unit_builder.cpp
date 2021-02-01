//
//  unit_builder.cpp
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "unit_builder.h"
#include "game/scene.h"
#include "force/force.h"
#include "unit/unit.h"
#include "unit/assistant.h"
#include "game/position.h"
#include "skill_builder.h"
#include "util/script_loader.h"
#include "skill_builder.h"
#include "game/position.h"
using namespace battle;

unit * unit_builder::build(battle::user * user, const battle::unit_meta_data &data){
//    if(data.hp() <= 0) return NULL;
    
    battle::scene * scene = user->get_scene();
    battle::force * force = user->get_force();


    unit_type type = unit_type_normal;
    if(data.has_type())
    {
        type = static_cast<battle::unit_type>(data.type());
    }
    battle::unit * unit(NULL);
    switch (type) {
        case unit_type_assist:
            unit = new battle::assistant(scene, force, user, scene->next_unit_id(), data.sid());
            break;
        case unit_type_normal:
        default:
            unit = new battle::unit(scene, force, user, scene->next_unit_id(), data.sid(), unit_type_normal);
            break;
    }

    unit->level = data.level();
    unit->max_hp = data.max_hp();
    unit->hp = data.hp();
    unit->base_attack_point = data.attack();
    unit->attack_point = data.attack();
    unit->base_defense = data.defense();
    unit->defense = data.defense();

    unit->base_speed = data.speed();
    unit->speed = data.speed();
    if(unit->base_speed == 0){
        unit->base_speed = 1;
        unit->speed = 1;
    }
    
    unit->base_dodge_rate = data.dodge_rate();
    if(unit->base_dodge_rate >= 100){
        unit->base_dodge_rate = 99;
    }
    unit->dodge_rate = unit->base_dodge_rate;
    
    unit->base_critical_rate = data.critical_rate();   
    unit->critical_rate = unit->base_critical_rate;
    
    unit->base_block_rate = data.block_rate();
    if(unit->base_block_rate >= 100){
        unit->base_block_rate = 99;
    }
    unit->block_rate = unit->base_block_rate;
    
    unit->critical_multiplier = CRITICAL_MULTIPLIER;
    unit->block_multiplier = BLOCK_MULTIPLIER;
    unit->up = static_cast<uint8_t>(data.up());
    
    unit->skill_fight = data.skill_fight();
    unit->is_boss = data.is_boss();
    if(data.has_dress_id())
    {
        unit->dress_id = data.dress_id();
    }
    if(data.has_is_show())
    {
        unit->is_show = data.is_show();
    }
    
    set_position(scene, force, user, unit, data.position(), static_cast<uint8_t>(data.size()));
    
    BOOST_FOREACH(const ::skill_meta_data & skill_data, data.skills()){
        boost::shared_ptr<battle::skill> skill = S_SKILL_BUILDER.build(unit, skill_data);
        if(skill)
        {
            unit->skills_.push_back(skill);
        }
    }


    unit->base_hit_rate = data.hit_rate();
    if (unit->base_hit_rate >= 100) {
        unit->base_hit_rate = 99;
    }
    unit->hit_rate = unit->base_hit_rate;
    
    unit->event_hurt = data.event_hurt();
    unit->event_injury = data.event_injury();

    unit->first_turn = data.first_turn();

    unit->base_heal_rate = data.heal_rate();
    if(unit->base_heal_rate >= 100){
        unit->base_heal_rate = 99;
    }
    unit->heal_rate = unit->base_heal_rate;
    

    unit->base_break_rate = data.break_rate();
    if(unit->base_break_rate >= 100){
        unit->base_break_rate = 99;
    }
    unit->break_rate = unit->base_break_rate;

    unit->base_tough_rate = data.tough_rate();
    if(unit->base_tough_rate >= 100){
        unit->base_tough_rate = 99;
    }
    unit->tough_rate = unit->base_tough_rate;

    unit->base_criticaldam_rate = data.criticaldam_rate();
    if(unit->base_criticaldam_rate >= 100){
        unit->base_criticaldam_rate = 99;
    }
    unit->criticaldam_rate = unit->base_criticaldam_rate;

    unit->locationg = data.locationg();
    unit->skill_spec = data.skill_spec();
    unit->attack_spec = data.attack_spec();
    unit->immu_debuff = data.immu_debuff();
    unit->double_atk = data.double_atk();
    unit->atk_heal = data.atk_heal();
    unit->highhp_dam = data.highhp_dam();
    unit->highhp_def = data.highhp_def();
    unit->fieldbuff = data.fieldbuff();
    unit->heal_debuff = data.heal_debuff();
    unit->skill_resist = data.skill_resist();
    unit->init_buff = data.init_buff();
    return unit;
}

unit * unit_builder::build(battle::user *user, const lua_object &obj){
    battle::scene * scene = user->get_scene();
    battle::force * force = user->get_force();
    
    battle::unit * unit = new battle::unit(scene, force, user, scene->next_unit_id(), luabind::object_cast<uint32_t>(obj["sid"]), unit_type_normal);
        
    unit->max_hp = luabind::object_cast<int32_t>(obj["hp"]);
    unit->hp = unit->max_hp;
    unit->level = luabind::object_cast<int32_t>(obj["level"]);
    unit->base_attack_point = luabind::object_cast<int32_t>(obj["attack"]);
    unit->attack_point = unit->base_attack_point;
    unit->base_defense = luabind::object_cast<int32_t>(obj["defense"]);
    unit->defense = unit->base_defense;

    unit->base_speed = luabind::object_cast<uint32_t>(obj["speed"]);
    unit->speed = unit->base_speed;
    if(unit->base_speed == 0){
        unit->base_speed = 1;
        unit->speed = 1;
    }
    
    unit->base_dodge_rate = luabind::object_cast<uint32_t>(obj["dodge_rate"]);
    if(unit->base_dodge_rate >= 100){
        unit->base_dodge_rate = 99;
    }
    unit->dodge_rate = unit->base_dodge_rate;
    
    unit->base_critical_rate = luabind::object_cast<uint32_t>(obj["critical_rate"]);
    unit->critical_rate = unit->base_critical_rate;
    
    unit->base_block_rate = luabind::object_cast<uint32_t>(obj["block_rate"]);
    if(unit->base_block_rate >= 100){
        unit->base_block_rate = 99;
    }
    unit->block_rate = unit->base_block_rate;

    unit->critical_multiplier = CRITICAL_MULTIPLIER;
    unit->block_multiplier = BLOCK_MULTIPLIER;
    unit->up = luabind::object_cast<uint8_t>(obj["up"]);
    unit->skill_fight = luabind::object_cast<float>(obj["skill_fight"]);
    
    for(luabind::iterator iter(obj["skills"]), end; iter != end; ++iter){
        boost::shared_ptr<battle::skill> skill = S_SKILL_BUILDER.build(unit, *iter);
        if(skill){
            unit->skills_.push_back(skill);
        }
    }
    
    uint8_t pos_id = static_cast<uint8_t>(luabind::object_cast<uint8_t>(obj["position"]));
    unit->size = static_cast<uint8_t>(luabind::object_cast<uint8_t>(obj["size"]));
    set_position(scene, force, user, unit, pos_id, unit->size);

    
    unit->base_hit_rate = luabind::object_cast<uint16_t>(obj["hit_rate"]);
    if (unit->base_hit_rate >= 100) {
        unit->base_hit_rate = 99;
    }
    unit->hit_rate = unit->base_hit_rate;
    
    unit->event_hurt = luabind::object_cast<uint16_t>(obj["event_hurt"]);
    unit->event_injury = luabind::object_cast<uint16_t>(obj["event_injury"]);

    unit->first_turn = luabind::object_cast<bool>(obj["first_turn"]);

    unit->base_heal_rate = luabind::object_cast<uint16_t>(obj["heal_rate"]);
    if(unit->base_heal_rate >= 100){
        unit->base_heal_rate = 99;
    }
    unit->heal_rate = unit->base_heal_rate;


    unit->base_break_rate = luabind::object_cast<uint16_t>(obj["break_rate"]);
    if(unit->base_break_rate >= 100){
        unit->base_break_rate = 99;
    }
    unit->break_rate = unit->base_break_rate;

    unit->base_tough_rate = luabind::object_cast<uint16_t>(obj["tough_rate"]);
    if(unit->base_tough_rate >= 100){
        unit->base_tough_rate = 99;
    }
    unit->tough_rate = unit->base_tough_rate;

    unit->base_criticaldam_rate = luabind::object_cast<uint16_t>(obj["criticaldam_rate"]);
    if(unit->base_criticaldam_rate >= 100){
        unit->base_criticaldam_rate = 99;
    }
    unit->criticaldam_rate = unit->base_criticaldam_rate;

    unit->locationg = luabind::object_cast<uint8_t>(obj["locationg"]);
    unit->skill_spec = luabind::object_cast<bool>(obj["skill_spec"]);
    unit->attack_spec = luabind::object_cast<std::string>(obj["attack_spec"]);
    unit->immu_debuff = luabind::object_cast<uint16_t>(obj["immu_debuff"]);
    unit->double_atk = luabind::object_cast<uint8_t>(obj["double_atk"]);
    unit->atk_heal = luabind::object_cast<uint8_t>(obj["atk_heal"]);
    unit->highhp_dam = luabind::object_cast<uint8_t>(obj["highhp_dam"]);
    unit->highhp_def = luabind::object_cast<uint8_t>(obj["highhp_def"]);
    unit->fieldbuff = luabind::object_cast<std::string>(obj["fieldbuff"]);
    unit->heal_debuff = luabind::object_cast<bool>(obj["heal_debuff"]);
    unit->skill_resist = luabind::object_cast<uint16_t>(obj["skill_resist"]);
    unit->init_buff = luabind::object_cast<std::string>(obj["init_buff"]);
    return unit;
}

unit * unit_builder::build(battle::user *user, const Json::Value & json){
    battle::scene * scene = user->get_scene();
    battle::force * force = user->get_force();    
    battle::unit * unit = new battle::unit(scene, force, user, scene->next_unit_id(), json["sid"].asUInt(), unit_type_normal);
    
    unit->max_hp = json["hp"].asInt();
    unit->hp = unit->max_hp;
    unit->level = static_cast<uint16_t>(json["level"].asUInt());
    unit->base_attack_point = json["attack"].asUInt();
    unit->attack_point = unit->base_attack_point;
    unit->base_defense = json["defense"].asUInt();
    unit->defense = unit->base_defense;

    unit->base_speed = json["speed"].asUInt();
    unit->speed = unit->base_speed;
    if(unit->base_speed == 0){
        unit->base_speed = 1;
        unit->speed = 1;
    }
    
    unit->base_dodge_rate = static_cast<uint16_t>(json["dodge_rate"].asUInt());
    if(unit->base_dodge_rate >= 100){
        unit->base_dodge_rate = 99;
    }
    unit->dodge_rate = unit->base_dodge_rate;
    
    unit->base_critical_rate = static_cast<uint16_t>(json["critical_rate"].asUInt());
    unit->critical_rate = unit->base_critical_rate;
    
    unit->base_block_rate = static_cast<uint16_t>(json["block_rate"].asUInt());
    if(unit->base_block_rate >= 100){
        unit->base_block_rate = 99;
    }
    unit->block_rate = unit->base_block_rate;
    
    unit->critical_multiplier = CRITICAL_MULTIPLIER;
    unit->block_multiplier = BLOCK_MULTIPLIER;
    unit->up = static_cast<uint8_t>(json["up"].asUInt());
    
    if(json.isMember("skill_fight")){
        unit->skill_fight = static_cast<float>(json["skill_fight"].asDouble());
    }
    
    for(BOOST_AUTO(pos, json["skills"].begin()); pos != json["skills"].end(); ++pos){
        boost::shared_ptr<battle::skill> skill = S_SKILL_BUILDER.build(unit, *pos);
        if(skill){
            unit->skills_.push_back(skill);
        }
    }
    
    uint8_t pos_id = static_cast<uint8_t>(json["position"].asUInt());
    unit->size = static_cast<uint8_t>(json["size"].asUInt());
    set_position(scene, force, user, unit, pos_id, unit->size);

    
    
    unit->base_hit_rate = static_cast<uint16_t>(json["hit_rate"].asUInt());
    if (unit->base_hit_rate >= 100) {
        unit->base_hit_rate = 99;
    }
    unit->hit_rate = unit->base_hit_rate;

    unit->event_hurt = static_cast<uint16_t>(json["event_hurt"].asUInt());
    unit->event_injury = static_cast<uint16_t>(json["event_injury"].asUInt());

    unit->first_turn = static_cast<bool>(json["first_turn"].asBool());

    unit->base_heal_rate = static_cast<uint16_t>(json["heal_rate"].asUInt());
    if(unit->base_heal_rate >= 100){
        unit->base_heal_rate = 99;
    }
    unit->heal_rate = unit->base_heal_rate;


    unit->base_break_rate = static_cast<uint16_t>(json["break_rate"].asUInt());
    if(unit->base_break_rate >= 100){
        unit->base_break_rate = 99;
    }
    unit->break_rate = unit->base_break_rate;

    unit->base_tough_rate = static_cast<uint16_t>(json["tough_rate"].asUInt());
    if(unit->base_tough_rate >= 100){
        unit->base_tough_rate = 99;
    }
    unit->tough_rate = unit->base_tough_rate;

    unit->base_criticaldam_rate = static_cast<uint16_t>(json["criticaldam_rate"].asUInt());
    if(unit->base_criticaldam_rate >= 100){
        unit->base_criticaldam_rate = 99;
    }
    unit->criticaldam_rate = unit->base_criticaldam_rate;

    unit->locationg = static_cast<uint8_t>(json["locationg"].asUInt());
    unit->skill_spec = static_cast<bool>(json["skill_spec"].asBool());
    unit->attack_spec = static_cast<std::string>(json["attack_spec"].asString());
    unit->immu_debuff = static_cast<uint16_t>(json["immu_debuff"].asUInt());
    unit->double_atk = static_cast<uint8_t>(json["double_atk"].asUInt());
    unit->atk_heal = static_cast<uint8_t>(json["atk_heal"].asUInt());
    unit->highhp_dam = static_cast<uint8_t>(json["highhp_dam"].asUInt());
    unit->highhp_def = static_cast<uint8_t>(json["highhp_def"].asUInt());
    unit->fieldbuff = static_cast<std::string>(json["fieldbuff"].asString());
    unit->heal_debuff = static_cast<bool>(json["heal_debuff"].asBool());
    unit->skill_resist = static_cast<uint16_t>(json["skill_resist"].asUInt());
    unit->init_buff = static_cast<std::string>(json["init_buff"].asString());
    return unit;
}

void unit_builder::set_position(scene * scene, force * force, user * user, unit * unit, uint8_t position_id, uint8_t size){
    if(unit->get_type() != unit_type_normal){
        unit->related_pos_id = position_id;
        position_id = 0;
        size = 1;
    }

    battle::position * position = new battle::position(position_id);
    position->set_unit(unit);
    unit->position_ = position;
    unit->size = size;

    force->add_position(position);
    if(unit->size > 1){
        for(uint8_t i(0); i < unit->size; ++i){
            if(i == 1) continue;
            battle::position * position = new battle::position(position_id + 1 - i);
            position->set_unit(unit);
            force->add_position(position);
            unit->extra_positions_.push_back(position);
        }
    }
    user->add_unit(unit);
    force->add_unit(unit);
    scene->add_unit(unit);
}

