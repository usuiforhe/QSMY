//
//  user.cpp
//  battle
//
//  Created by wangyanjun on 13-3-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "user.h"
#include "unit/unit.h"
#include "force.h"
#include "game/scene.h"
using namespace battle;

user::~user(){
    BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
        delete val.second;
    }
}

void user::add_unit(battle::unit *unit){
    if(unit_map_.count(unit->get_id()) == 0){
        if(unit->get_type() != unit_type_assist)
        {
			total_speed_ += unit->base_speed;
			if(unit->base_speed > max_speed_){
				max_speed_ = unit->base_speed;
			}
        }
        
        unit_map_.insert(std::make_pair(unit->get_id(), unit));
    }
    
//    force_->add_unit(unit);
}

scene * user::get_scene(){
    return force_->get_scene();
}

double user::get_fight(){
    double fight(0);
    BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
        fight += val.second->get_fight();
    }
    
    return fight;
}

double user::get_max_fight(){
    double fight(0);
    BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
        fight += val.second->get_max_fight();
    }
    
    return fight;
}


