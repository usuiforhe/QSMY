//
//  buff_behavior_result.cpp
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "buff_behavior_result.h"
using namespace battle;

Json::Value buff_behavior_result::to_json(){
    Json::Value json;
    //类型
    if(is_add_){
        json["type"] = BEHAVIOR_RESULT_ADD_BUFF;
    }
    else{
        json["type"] = BEHAVIOR_RESULT_REMOVE_BUFF;
    }
    json["actor"] = actor_->get_id();
    json["buff_id"] = buff_id_;
    return json;
}

void buff_behavior_result::append_to_bin(std::vector<char> &bin){
    //类型
    if(is_add_){
        bin.push_back(static_cast<char>(BEHAVIOR_RESULT_ADD_BUFF));
    }
    else{
        bin.push_back(static_cast<char>(BEHAVIOR_RESULT_REMOVE_BUFF));
    }
    //受影响者编号
    bin.push_back(static_cast<char>(actor_->get_id()));
    //buff编号
    bin.push_back(static_cast<char>(buff_id_ & 0x000000ff));
    bin.push_back(static_cast<char>((buff_id_ & 0x0000ff00) >> 8));
    bin.push_back(static_cast<char>((buff_id_ & 0x00ff0000) >> 16));
    bin.push_back(static_cast<char>((buff_id_ & 0xff000000) >> 24));
}
