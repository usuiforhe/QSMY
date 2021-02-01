//
//  unit_buff.cpp
//  battle
//
//  Created by wangyanjun on 13-3-18.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "unit_buff.h"
#include "skill/skill.h"
#include "unit/unit.h"
#include "util.h"
#include "game/scene.h"

using namespace battle;

unit_buff::unit_buff(uint32_t id, bool is_visible, lua_object script, unit * owner, uint32_t source_unit_id, uint32_t source_skill_id, const lua_object & meta_data, uint8_t interval):
base_buff(id, is_visible, script),
owner_(owner),
source_unit_(owner_->get_scene()->get_unit(source_unit_id)),
source_skill_id_(source_skill_id),
meta_data_(meta_data),
interval_(interval),
last_effect_frame_(0){
    
}

luabind::object unit_buff::build_argument()
{
	luabind::object table = luabind::newtable(owner_->get_scene()->get_lua_state());
	table["arg"]   = meta_data_;
	table["layer"]  = layer_;
	table["times"]  = effect_times_;
    table["source_skill"] = source_skill_id_;
    
	return table;
}

void unit_buff::add_layer(){
    base_buff::add_layer();
    expire_strategy_->refresh();
    try
    {
		if (owner_->is_alive())
		{
			if (script_.is_valid())
			{
				luabind::call_function<void>(script_["on_add_layer"], owner_, source_unit_, build_argument());
			}
		}
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::add_layer() throws exception. what:";
		ss << boost::diagnostic_information(e);
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::add_layer() throws exception. what:";
		ss << e.what();
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
}

void unit_buff::remove_layer(uint8_t layer){
    base_buff::remove_layer(layer);
    try
    {
		if (owner_->is_alive())
		{
			if (script_.is_valid())
			{
				luabind::call_function<void>(script_["on_remove_layer"], owner_, source_unit_, build_argument());
			}
		}
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::remove_layer() throws exception. what:";
		ss << boost::diagnostic_information(e);
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::remove_layer() throws exception. what:";
		ss << e.what();
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
}

void unit_buff::enter(){
    layer_ = 1;
    expire_strategy_->on_add();
    try
    {
		if (owner_->is_alive())
		{
			if (script_.is_valid())
			{
                lua_object on_add = script_["on_add"];
                if(on_add.is_valid()){
                    luabind::call_function<void>(on_add, owner_, source_unit_, build_argument());
                }
			}
		}
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::enter() throws exception. what:";
		ss << boost::diagnostic_information(e);
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::enter() throws exception. what:";
		ss << e.what();
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
}

bool unit_buff::is_trigger(){
    if(owner_->get_self_frames() < last_effect_frame_ + interval_) {
        return false;
    }
    try
    {
		if (owner_->is_alive())
		{
			if (script_.is_valid())
			{
                lua_object is_trigger = script_["is_trigger"];
                if(is_trigger.is_valid()){
                    return luabind::call_function<bool>(is_trigger, owner_, source_unit_, build_argument());
                }
			}
		}
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::is_trigger() throws exception. what:";
		ss << boost::diagnostic_information(e);
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::is_trigger() throws exception. what:";
		ss << e.what();
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
    
    return false;
}

void unit_buff::effect(){
    base_buff::effect();
    last_effect_frame_ = owner_->get_self_frames();
    try
    {
		if (owner_->is_alive())
		{
			if (script_.is_valid())
			{
				luabind::call_function<void>(script_["on_effect"], owner_, source_unit_, build_argument());
			}
		}
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::effect() throws exception. what:";
		ss << boost::diagnostic_information(e);
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::effect() throws exception. what:";
		ss << e.what();
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
}

void unit_buff::exit(){
    try
    {
		if (owner_->is_alive())
		{
			if (script_.is_valid())
			{
				luabind::call_function<void>(script_["on_exit"], owner_, source_unit_, build_argument());
			}
		}
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::exit() throws exception. what:";
		ss << boost::diagnostic_information(e);
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
		ss << "unit_buff::exit() throws exception. what:";
		ss << e.what();
		ss << ". sid:" << id_;
		
        s_log.out_error(ss.str(), false);
    }
}

bool unit_buff::is_match(uint8_t source_unit_id, uint32_t source_skill_id){
    if(source_unit_id != 0 && source_unit_->get_id() != source_unit_id){
        return false;
    }
    if(source_skill_id != 0 && source_skill_id_ != source_skill_id){
        return false;
    }
    
    return true;
}
