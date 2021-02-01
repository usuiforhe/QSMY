//
//  unit_buff_partitial.cpp
//  battle
//
//  Created by wangyanjun on 13-3-19.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "unit.h"
#include "event/args/unit_event_args.h"
#include "builder/buff_builder.h"
#include "game/scene.h"
#include "game/behavior/buff_behavior_result.h"
#include "game/behavior/buff_effect_behavior.h"

using namespace battle;

void unit::add_buff(uint32_t buff_id, uint8_t source_unit_id, uint32_t source_skill_id, const lua_object & meta_data){
    
    if(!is_alive()){
        return;
    }
    
    if(this->immu_debuff == buff_id){
    	return;
    }

    BOOST_AUTO(pos, buff_list_map_.find(buff_id));
    if(pos == buff_list_map_.end()){
        unit_buff * buff = S_BUFF_BUILDER.build(this, buff_id, source_unit_id, source_skill_id, meta_data);
        if(buff == NULL){
            return;
        }
        
        std::list<unit_buff*> * list = new std::list<unit_buff*>;
        list->push_back(buff);
        buff_list_map_.insert(std::make_pair(buff->get_id(), list));
        buff->enter();
    }
    else{
        bool is_new(true);
        BOOST_FOREACH(unit_buff * ub, *(pos->second)){
            if(ub->get_id() == buff_id && ub->get_source_skill_id() == source_skill_id && ub->get_source_unit()->get_id() == source_unit_id && !ub->is_expired()){
                ub->add_layer();
                is_new = false;
                break;
            }
        }
        
        if(is_new){
            unit_buff * buff = S_BUFF_BUILDER.build(this, buff_id, source_unit_id, source_skill_id, meta_data);
            if(buff == NULL){
                return;
            }
            pos->second->push_back(buff);
            buff->enter();
        }
    }
    base_behavior_result * result = new buff_behavior_result(this, buff_id, true);
    scene_->get_report()->add_behavior_result(result);
}

void unit::add_buff_noresult(uint32_t buff_id, uint8_t source_unit_id, uint32_t source_skill_id, const lua_object & meta_data){

    if(!is_alive()){
        return;
    }

    if(this->immu_debuff == buff_id){
    	return;
    }

    BOOST_AUTO(pos, buff_list_map_.find(buff_id));
    if(pos == buff_list_map_.end()){
        unit_buff * buff = S_BUFF_BUILDER.build(this, buff_id, source_unit_id, source_skill_id, meta_data);
        if(buff == NULL){
            return;
        }

        std::list<unit_buff*> * list = new std::list<unit_buff*>;
        list->push_back(buff);
        buff_list_map_.insert(std::make_pair(buff->get_id(), list));
        buff->enter();
    }
    else{
        bool is_new(true);
        BOOST_FOREACH(unit_buff * ub, *(pos->second)){
            if(ub->get_id() == buff_id && ub->get_source_skill_id() == source_skill_id && ub->get_source_unit()->get_id() == source_unit_id && !ub->is_expired()){
                ub->add_layer();
                is_new = false;
                break;
            }
        }

        if(is_new){
            unit_buff * buff = S_BUFF_BUILDER.build(this, buff_id, source_unit_id, source_skill_id, meta_data);
            if(buff == NULL){
                return;
            }
            pos->second->push_back(buff);
            buff->enter();
        }
    }
}

void unit::remove_buff(uint32_t buff_id, uint8_t source_unit_id, uint32_t source_skill_id){
    if(!is_alive()){
        return;
    }
    
    BOOST_AUTO(pos, buff_list_map_.find(buff_id));
    
    if(pos == buff_list_map_.end()){
        return;
    }
    
    std::list<unit_buff*> list;
    BOOST_FOREACH(unit_buff * buff, *(pos->second)){
        if(buff->is_match(source_unit_id, source_skill_id)){
            list.push_back(buff);
        }
    }
    
    BOOST_FOREACH(unit_buff * buff, list){
        pos->second->remove(buff);
        buff->exit();
        delete buff;
    }
    base_behavior_result * result = new buff_behavior_result(this, buff_id, false);
    scene_->get_report()->add_behavior_result(result);
    
    if(pos->second->size() == 0){
        delete pos->second;
        buff_list_map_.erase(pos);
    }
}

void unit::remove_buff_layer(uint32_t buff_id, uint8_t layer, uint8_t source_unit_id, uint32_t source_skill_id){
    if(!is_alive()){
        return;
    }
    
    BOOST_AUTO(pos, buff_list_map_.find(buff_id));
    
    if(pos == buff_list_map_.end()){
        return;
    }
    std::list<unit_buff*> list;
    BOOST_FOREACH(unit_buff * buff, *(pos->second)){
        if(buff->is_match(source_unit_id, source_skill_id)){
            list.push_back(buff);
        }
    }
    
    BOOST_FOREACH(unit_buff * buff, list){
        buff->remove_layer(layer);
        if(buff->is_expired()){
            pos->second->remove(buff);
            buff->exit();
            delete buff;
        }
    }
}

uint16_t unit::get_buff_layer(uint32_t buff_id, uint8_t source_unit_id, uint32_t source_skill_id){
    BOOST_AUTO(pos, buff_list_map_.find(buff_id));
    if(pos == buff_list_map_.end()){
        return 0;
    }
    uint16_t layer(0);
    BOOST_FOREACH(unit_buff * buff, *(pos->second)){
        if(buff->is_match(source_unit_id, source_skill_id)){
            layer+= buff->get_layer();
        }
    }
    
    return layer;
}

bool unit::is_buff_exist(uint32_t buff_id){
    BOOST_AUTO(pos, buff_list_map_.find(buff_id));
    if (pos == buff_list_map_.end()) {
        return false;
    }else{
        return true;
    }
}

void unit::update_buffs(){
    if(!is_alive()){
        return;
    }
    
    BOOST_FOREACH(buff_list_map_t::value_type value, buff_list_map_){
        std::list<unit_buff*> expired_list;
        BOOST_FOREACH(unit_buff * buff , *(value.second)){
            if(buff->is_expired()){
                expired_list.push_back(buff);
            }
            else if(buff->is_trigger()){
                base_behavior * behavior = new buff_effect_behavior(this, buff->get_id());
                scene_->get_report()->push_behavior(behavior);
                buff->effect();
                scene_->get_report()->pop_behavior();
            }
        }
        BOOST_FOREACH(unit_buff * buff, expired_list){
            value.second->remove(buff);
            buff->exit();
            delete buff;
        }
    }    
}
