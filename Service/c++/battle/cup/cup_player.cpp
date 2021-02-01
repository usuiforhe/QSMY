//
//  cup_player.cpp
//  battle
//
//  Created by 汪燕军 on 13-4-10.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "cup_player.h"
#include "cup_match.h"
using namespace battle;

cup_player::cup_player(uint32_t user_id, std::auto_ptr<std::istream> formation_stream, const std::string & disp, std::string & disp_detail, uint32_t win_count, uint16_t best_grade)
    :user_id_(user_id),
    force_(),
    disp_(disp),
    disp_detail_(disp_detail),
    win_count_(win_count),
    best_grade_(best_grade)
{
    force_.ParseFromIstream(formation_stream.get());
}

cup_player::cup_player(force_meta_data & force)
:user_id_(force.user_id()),
force_(force)
{
    Json::Value root;
    root["name"] = force.name();
    root["lv"] = force.level();
    root["lid"] = force.leader_sid();
    
    disp_ = Json::FastWriter().write(root);
    
    Json::Value units(Json::arrayValue);
    
    BOOST_FOREACH(const battle::unit_meta_data & unit, force.units()){
        Json::Value u_root;
        u_root["sid"] = unit.sid();
        u_root["lv"] = unit.level();
        u_root["pid"] = unit.position();
        units.append(u_root);
    }
    
    disp_detail_ = Json::FastWriter().write(units);
}

cup_player::~cup_player(){
    
}

void cup_player::match_over(cup_match * match){
    if(match->get_winner().get() == this){
        ++win_count_;
    }
    if(best_grade_ == 0 || match->get_final_num() < best_grade_){
        best_grade_ = match->get_final_num() / 2;
    }
}
