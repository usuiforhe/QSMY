//
//  buff_effect_behavior.cpp
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "buff_effect_behavior.h"

using namespace battle;

Json::Value buff_effect_behavior::to_json(){    
    Json::Value json;
    json["type"] = BEHAVIOR_BUFF_EFFECT;
    json["actor"] = actor_->get_id();
    json["buff_id"] = buff_id_;
    
    append_results(json);
    
    return json;
}

void buff_effect_behavior::append_to_bin(std::vector<char> &bin){
    //类型
    bin.push_back(static_cast<char>(BEHAVIOR_BUFF_EFFECT));
    //buff生效者编号
    bin.push_back(static_cast<char>(actor_->get_id()));
    //buff编号
    bin.push_back(static_cast<char>(buff_id_ & 0x000000ff));
    bin.push_back(static_cast<char>((buff_id_ & 0x0000ff00) >> 8));
    bin.push_back(static_cast<char>((buff_id_ & 0x00ff0000) >> 16));
    bin.push_back(static_cast<char>((buff_id_ & 0xff000000) >> 24));
    
    append_results(bin);
}
