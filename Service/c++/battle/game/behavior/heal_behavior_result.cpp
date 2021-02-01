//
//  heal_behavior_result.cpp
//  battle
//
//  Created by 汪燕军 on 13-5-2.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "heal_behavior_result.h"
#include "unit/unit.h"

using namespace battle;

Json::Value heal_behavior_result::to_json(){
    Json::Value json;
    json["type"] = BEHAVIOR_RESULT_HEAL;
    json["actor"] = actor_->get_id();
    json["heal"] = heal_;
    json["is_critical"] = is_critical_;
    return json;
}

void heal_behavior_result::append_to_bin(std::vector<char> &bin){
    //类型
    bin.push_back(static_cast<char>(BEHAVIOR_RESULT_HEAL));
    //受到治疗者编号
    bin.push_back(static_cast<char>(actor_->get_id()));
    //治疗量
    bin.push_back(static_cast<char>(heal_ & 0x000000ff));
    bin.push_back(static_cast<char>((heal_ & 0x0000ff00) >> 8));
    bin.push_back(static_cast<char>((heal_ & 0x00ff0000) >> 16));
    bin.push_back(static_cast<char>((heal_ & 0xff000000) >> 24));
    
    //是否暴击
    bin.push_back(is_critical_ ? 0x01 : 0x00);
}