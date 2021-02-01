//
//  scene_builder.cpp
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "scene_builder.h"
#include "force_builder.h"
#include "force/force.h"
#include "unit/unit.h"
#include "unit/assistant.h"
#include "skill/skill.h"
#include "config.h"
#include "util.h"
#include "util/script_loader.h"
#include "util/script_util.h"
#include "game/scene.h"
#include "game/report.h"
#include "game/end_checker/normal_end_checker.h"

using namespace battle;

/**
 * 通过protobuf构建场景
 */
scene * scene_builder::build(battle::scene_meta_data & data){
    battle::scene * scene = new battle::scene(static_cast<uint32_t>(data.sid()), static_cast<scene_type>(data.type()));
    try{
        scene->map_id_ = data.map_id();
        
        if(data.has_left_force()){
            scene->left_force_ = S_FORCE_BUILDER.build(scene, LEFT_FORCE_ID, data.left_force());
        }
        
        if(data.has_right_force()){
            scene->right_force_ = S_FORCE_BUILDER.build(scene, RIGHT_FORCE_ID, data.right_force());
        }
        
        switch (scene->type_) {
            case SCENE_TYPE_PVE:
                build_pve(scene);
                break;
            case SCENE_TYPE_CUP:
                build_cup(scene);
                break;
            case SCENE_TYPE_CHALLENGE:
                build_challenge(scene);
                break;
            case SCENE_TYPE_TOWER:
                build_tower(scene);
                break;
            case SCENE_TYPE_HUNT:
                build_hunt(scene);
                break;
            case SCENE_TYPE_GUILD_ROB:
                build_guild_boss(scene);
                break;
            default:
                scene->report_ = new report(scene);
                scene->end_checker_ = &S_NORMAL_END_CHECKER;
                break;
        }
        
        if(scene->left_force_ == NULL || scene->right_force_ == NULL){
            std::stringstream & ss = latte::fast_stringstream::ss();
            ss << "not enough forces. scene id:" << scene->sid_;
            BOOST_THROW_EXCEPTION(std::runtime_error(ss.str()));
        }
        bind_class(scene);
        
        return scene;
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "scene_builder::build throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "scene_builder::build throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());        
    }
    delete scene;
    return NULL;
}

scene * scene_builder::build(const Json::Value &json){
    battle::scene * scene = new battle::scene(json["scene_id"].asUInt(), static_cast<scene_type>(json["scene_type"].asUInt()));
    try{        
        scene->map_id_ = 101;
        
        if(!json["left_force"].isNull()){
            scene->left_force_ = S_FORCE_BUILDER.build(scene, LEFT_FORCE_ID, json["left_force"]);
        }
        
        if(!json["right_force"].isNull()){
            scene->right_force_ = S_FORCE_BUILDER.build(scene, RIGHT_FORCE_ID, json["right_force"]);
        }
        
        switch (scene->type_) {
            case SCENE_TYPE_PVE:
                build_pve(scene);
                break;
            case SCENE_TYPE_CUP:
                build_cup(scene);
                break;
            case SCENE_TYPE_CHALLENGE:
                build_challenge(scene);
                break;
            case SCENE_TYPE_TOWER:
                build_tower(scene);
                break;
            case SCENE_TYPE_HUNT:
                build_hunt(scene);
                break;
            case SCENE_TYPE_GUILD_ROB:
                build_guild_boss(scene);
                break;
            default:
                scene->report_ = new report(scene);
                scene->end_checker_ = &S_NORMAL_END_CHECKER;
                break;
        }
        
        if(scene->left_force_ == NULL || scene->right_force_ == NULL){
            std::stringstream & ss = latte::fast_stringstream::ss();
            ss << "not enough forces. scene id:" << scene->sid_;
            BOOST_THROW_EXCEPTION(std::runtime_error(ss.str()));
        }
        
        return scene;
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "scene_builder::build throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "scene_builder::build throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());
    }
    delete scene;
    return NULL;
}

void scene_builder::build_pve(battle::scene *scene){
    scene->report_ = new report(scene);
    scene->end_checker_ = &S_NORMAL_END_CHECKER;
    if(scene->right_force_ == NULL){
        lua_object obj = S_SCRIPT_LOADER.load_scene(scene->sid_, scene->get_lua_state());
        
		if (!obj.is_valid() || luabind::type(obj) == LUA_TNIL){
            std::stringstream & ss = latte::fast_stringstream::ss();
            ss << "invalid scene lua script. scene id:" << scene->sid_;
            BOOST_THROW_EXCEPTION(std::runtime_error(ss.str()));
        }
        scene->right_force_ = S_FORCE_BUILDER.build(scene, RIGHT_FORCE_ID, obj);
    }
    add_unit_property(scene, LEFT_FORCE_ID);
}

void scene_builder::build_cup(battle::scene *scene){
    scene->report_ = new report(scene);
    scene->end_checker_ = &S_NORMAL_END_CHECKER;
}

void scene_builder::build_challenge(battle::scene *scene){
    scene->report_ = new report(scene);
    scene->end_checker_ = &S_NORMAL_END_CHECKER;
}
void scene_builder::build_tower(battle::scene *scene){
    scene->report_ = new report(scene);
    scene->end_checker_ = &S_NORMAL_END_CHECKER;
}

void scene_builder::build_hunt(battle::scene *scene){
    scene->report_ = new report(scene);
    scene->end_checker_ = &S_NORMAL_END_CHECKER;
}

void scene_builder::build_guild_boss(battle::scene *scene){
    scene->report_ = new report_hp(scene);
    scene->end_checker_ = &S_NORMAL_END_CHECKER;
}

void scene_builder::add_unit_property(battle::scene *scene, uint8_t forceId){
	battle::force *force_ = NULL;
	if(forceId == RIGHT_FORCE_ID){
		force_ = scene->get_right_force();
	}else{
		force_ = scene->get_left_force();
	}

	if(force_ == NULL){
		return;
	}

	force_->add_unit_property();
}

void scene_builder::bind_class(scene * scene)
{    
	luabind::module(scene->get_lua_state())
	[
     luabind::class_<battle::scene>("scene")
     .property("sid", &scene::get_sid)
     .property("map_id", &scene::get_map_id)
     .property("type", &scene::get_type)
     .def("get_frames", &scene::get_frames)
     .def("get_unit", &scene::get_unit)
     ];
    
	luabind::module(scene->get_lua_state())
	[
     luabind::class_<force>("force")
     .property("id", &force::get_id)
     .property("opponent", &force::get_opponent)
     .property("is_mob", &force::is_mob)
     .def("get_unit_list_table", &force::get_unit_list_table)
     .def("get_position_unit", &force::get_position_unit)
     .def("get_unit_by_sid", &force::get_unit_by_sid)
     .def("get_hp_min", &force::get_hp_min)
     .def("get_hp_max", &force::get_hp_max)
     .def("get_unit_list_table_all", &force::get_unit_list_table_all)
     .def("get_unit_by_sid_all", &force::get_unit_by_sid_all)
     ];    
	luabind::module(scene->get_lua_state())
	[
     luabind::class_<unit_state>("unit_state")
     .property("can_action", &unit_state::get_can_action, &unit_state::set_can_action)
     .property("can_attack", &unit_state::get_can_attack, &unit_state::set_can_attack)
     .property("can_cast_skill", &unit_state::get_can_cast_skill, &unit_state::set_can_cast_skill)
     .property("can_block", &unit_state::get_can_block, &unit_state::set_can_block)
     .property("can_dodge", &unit_state::get_can_dodge, &unit_state::set_can_dodge)
     .property("can_critical", &unit_state::get_can_critical, &unit_state::set_can_critical)
     .property("can_be_attacked", &unit_state::get_can_be_attacked, &unit_state::set_can_be_attacked)
     .property("break_action", &unit_state::get_break_action, &unit_state::set_break_action)
     ];
	luabind::module(scene->get_lua_state())
	[
     luabind::class_<unit>("unit")
     .def_readonly("level", &unit::level)
     .def_readonly("hp", &unit::hp)
     .def_readonly("max_hp", &unit::max_hp)
     .def_readonly("base_attack", &unit::base_attack_point)
     .def_readwrite("attack", &unit::attack_point)
     .def_readonly("base_defense", &unit::base_defense)
     .def_readwrite("defense", &unit::defense)
     .def_readonly("base_speed", &unit::base_speed)
     .def_readwrite("speed", &unit::speed)
     .def_readonly("base_dodge_rate", &unit::base_dodge_rate)
     .def_readwrite("dodge_rate", &unit::dodge_rate)
     .def_readonly("base_critical_rate", &unit::base_critical_rate)
     .def_readwrite("critical_rate", &unit::critical_rate)
     .def_readonly("base_block_rate", &unit::base_block_rate)
     .def_readwrite("block_rate", &unit::block_rate)
     .def_readwrite("critical_multiplier", &unit::critical_multiplier)
     .def_readwrite("block_multiplier", &unit::block_multiplier)
     .def_readonly("up", &unit::up)
     .def_readonly("is_show", &unit::is_show)
     .def_readonly("related_position", &unit::related_pos_id)

     .def_readonly("base_hit_rate", &unit::base_hit_rate)
     .def_readwrite("hit_rate", &unit::hit_rate)
     .def_readonly("base_heal_rate", &unit::base_heal_rate)
     .def_readwrite("heal_rate", &unit::heal_rate)
     .def_readonly("base_tough_rate", &unit::base_tough_rate)
     .def_readwrite("tough_rate", &unit::tough_rate)
     .def_readonly("base_break_rate", &unit::base_break_rate)
     .def_readwrite("break_rate", &unit::break_rate)
     .def_readonly("base_criticaldam_rate", &unit::base_criticaldam_rate)
     .def_readwrite("criticaldam_rate", &unit::criticaldam_rate)
     .def_readonly("event_hurt", &unit::event_hurt)
     .def_readonly("event_injury", &unit::event_injury)
     .def_readonly("first_turn", &unit::first_turn)
     .def_readonly("locationg", &unit::locationg)
     .def_readonly("skill_spec", &unit::skill_spec)
     .def_readonly("attack_spec", &unit::attack_spec)
     .def_readonly("immu_debuff", &unit::immu_debuff)
     .def_readonly("double_atk", &unit::double_atk)
     .def_readonly("atk_heal", &unit::atk_heal)
     .def_readonly("highhp_dam", &unit::highhp_dam)
     .def_readonly("highhp_def", &unit::highhp_def)
     .def_readonly("fieldbuff", &unit::fieldbuff)
     .def_readonly("heal_debuff", &unit::heal_debuff)
     .def_readwrite("is_first_cast", &unit::is_first_cast)
     .def_readonly("skill_resist", &unit::skill_resist)

     .property("id", &unit::get_id)
     .property("sid", &unit::get_sid)
     .property("position", &unit::get_position_id)
     .property("scene", &unit::get_scene)
     .property("force", &unit::get_force)
     .property("target", &unit::get_target)
     .property("is_alive", &unit::is_alive)
     .property("state", &unit::get_state_ptr)
     .def("get_skill", &unit::get_skill_ptr)
     .def("taking_damage", &unit::skill_taking_damage)
     .def("heal", &unit::heal)
     .def("add_buff", &unit::add_buff)
     .def("remove_buff", &unit::remove_buff)
     .def("remove_buff_layer", &unit::remove_buff_layer)
     .def("get_buff_layer", &unit::get_buff_layer)
     .def("add_skill", &unit::add_skill)
     .def("remove_skill", &unit::remove_skill)
     .def("show", &unit::show)
     .def("hide", &unit::hide)
     .def("dialog", &unit::dialog)
     .def("is_buff_exist", &unit::is_buff_exist),
     luabind::class_<assistant, unit>("assistant")
     ];
	
	luabind::module(scene->get_lua_state())
	[
     luabind::class_<skill>("skill")
     .property("sid", &skill::get_sid)
     .property("cd", &skill::get_cd, &skill::set_cd)
     .property("trigger_rate", &skill::get_trigger_rate, &skill::set_trigger_rate)
     .property("is_enabled", &skill::is_enabled)
     .property("type", &skill::get_type)
     .def("refresh", &skill::refresh)
     ];
    
	luabind::module(scene->get_lua_state())
	[
     luabind::def("script_output_debug", &battle::script_output_debug)
     ];
    
	luabind::module(scene->get_lua_state())
	[
     luabind::def("script_get_random", &battle::script_get_random)
     ];
    
    luabind::module(scene->get_lua_state())
    [
     luabind::def("script_check_prob", &battle::script_check_prob)
    ];

    luabind::module(scene->get_lua_state())
    [
     luabind::def("script_out_string", &battle::script_out_string)
    ];
}
