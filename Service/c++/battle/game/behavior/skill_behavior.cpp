//
//  skill_behavior.cpp
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "skill_behavior.h"

using namespace battle;

Json::Value skill_behavior::to_json(){
    Json::Value json;
    //类型
    json["type"] = BEHAVIOR_SKILL;
    json["actor"] = actor_->get_id();
    json["skill_id"] = skill_id_;
    json["position"] = position_;
    json["target"] = target_;
    Json::Value coactors(Json::arrayValue);
    //协同施法者列表
    BOOST_FOREACH(uint8_t unit_id, coactors_){
        coactors.append(unit_id);
    }
    
    json["coactors"] = coactors;
    append_results(json);
    
    return json;
}

void skill_behavior::append_to_bin(std::vector<char> &bin){
    //类型
    bin.push_back(static_cast<char>(BEHAVIOR_SKILL));
    //施法者编号
    bin.push_back(static_cast<char>(actor_->get_id()));
    //技能编号
    bin.push_back(static_cast<char>(skill_id_ & 0x000000ff));
    bin.push_back(static_cast<char>((skill_id_ & 0x0000ff00) >> 8));
    bin.push_back(static_cast<char>((skill_id_ & 0x00ff0000) >> 16));
    bin.push_back(static_cast<char>((skill_id_ & 0xff000000) >> 24));
    //释放位置 前两位表示目标类型 1:己方 2:敌方 4:全部
    bin.push_back(static_cast<char>((position_ & 0x0000003f) | ((0x000000ff &target_) << 6)));
    //协同施法者数量
    bin.push_back(static_cast<char>(coactors_.size() & 0x000000ff));
    //协同施法者列表
    BOOST_FOREACH(uint8_t unit_id, coactors_){
        bin.push_back(static_cast<char>(unit_id));
    }
    
    append_results(bin);
}

