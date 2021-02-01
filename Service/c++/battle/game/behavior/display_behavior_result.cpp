//
//  display_behavior_result.cpp
//  battle
//
//  Created by 汪燕军 on 14-2-8.
//  Copyright (c) 2014年 汪燕军. All rights reserved.
//

#include "display_behavior_result.h"
using namespace battle;

Json::Value display_behavior_result::to_json(){
    Json::Value json;
    //类型
    if(is_show_){
        json["type"] = BEHAVIOR_RESULT_SHOW;
    }
    else{
        json["type"] = BEHAVIOR_RESULT_HIDE;
    }
    json["actor"] = actor_->get_id();
    return json;
}

void display_behavior_result::append_to_bin(std::vector<char> &bin){
    //类型
    if(is_show_){
        bin.push_back(static_cast<char>(BEHAVIOR_RESULT_SHOW));
    }
    else{
        bin.push_back(static_cast<char>(BEHAVIOR_RESULT_HIDE));
    }
    //受影响者编号
    bin.push_back(static_cast<char>(actor_->get_id()));
}
