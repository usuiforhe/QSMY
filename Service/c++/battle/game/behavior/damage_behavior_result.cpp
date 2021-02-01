//
//  damage_behavior_result.cpp
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "damage_behavior_result.h"
#include "unit/unit.h"

using namespace battle;

Json::Value damage_behavior_result::to_json(){
    Json::Value json;
    json["type"] = BEHAVIOR_RESULT_DAMAGE;
    json["actor"] = actor_->get_id();
    json["damage"] = damage_;
    json["is_critical"] = is_critical_;
    json["is_dodge"] = is_dodge_;
    json["is_block"] = is_block_;
    return json;
}

void damage_behavior_result::append_to_bin(std::vector<char> &bin){
    //类型
    bin.push_back(static_cast<char>(BEHAVIOR_RESULT_DAMAGE));
    //受伤者编号
    bin.push_back(static_cast<char>(actor_->get_id()));
    //伤害
    bin.push_back(static_cast<char>(damage_ & 0x000000ff));
    bin.push_back(static_cast<char>((damage_ & 0x0000ff00) >> 8));
    bin.push_back(static_cast<char>((damage_ & 0x00ff0000) >> 16));
    bin.push_back(static_cast<char>((damage_ & 0xff000000) >> 24));
    
    char special(0x00);
    if(is_critical_){
        special |= CRITICAL_FLAG;
    }
    if(is_dodge_){
        special |= DODGE_FLAG;
    }
    if(is_block_){
        special |= BLOCK_FLAG;
    }
    
    //特殊属性标志
    bin.push_back(special);
}
