//
//  unit_event_handler.cpp
//  battle
//
//  Created by wangyanjun on 13-3-16.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "unit_event_handler.h"
#include "unit/unit.h"
#include "skill/skill.h"
#include "game/scene.h"

using namespace battle;

void skill_event_handler::bind_skill(uint32_t event_id, boost::shared_ptr<battle::skill> skill){
    if(skill_map_.count(event_id) == 0){
        std::list<boost::shared_ptr<battle::skill> > * list = new std::list<boost::shared_ptr<battle::skill> >;
        list->push_back(skill);
        skill_map_[event_id] = list;
        owner_->bind_event_handler(event_id, this);
    }
    else{
        skill_map_[event_id]->push_back(skill);
    }
}

void skill_event_handler::handle_event(battle::unit *sender, battle::base_event_args &args){
    BOOST_AUTO(pos, skill_map_.find(args.id));
    if(pos == skill_map_.end()){
        return;
    }
    
    if(!owner_->get_state().get_can_cast_skill()) {
        return;
    }
    
    if(!owner_->is_alive()){
        return;
    }
    
    if(owner_->get_type() == unit_type_normal && owner_->get_target() == NULL){
        return;
    }
    
    lua_object lua_args = args.to_lua(sender->get_scene()->get_lua_state());

    std::vector<boost::shared_ptr<battle::skill> > passive_skills;
    std::vector<boost::shared_ptr<battle::skill> > enabled_skills;
    uint8_t max_priority(0);
    BOOST_FOREACH(boost::shared_ptr<battle::skill> skill, *(pos->second)){
        if(!owner_->get_skill(skill->get_sid())){
            continue;
        }
        if(skill->is_trigger(lua_args)){
            if(skill->get_type() == SKILL_TYPE_PASSIVE){
                owner_->cast_skill(skill);
            }
            else if(skill->get_priority() == max_priority){
                enabled_skills.push_back(skill);
            }
            else if(skill->get_priority() > max_priority){
                max_priority = skill->get_priority();
                enabled_skills.clear();
                enabled_skills.push_back(skill);
            }
        }
    }
    
    if(enabled_skills.size() == 0) {
        return;
    }
    
    std::random_shuffle(enabled_skills.begin(), enabled_skills.end());
    owner_->cast_skill(enabled_skills.front());
}
