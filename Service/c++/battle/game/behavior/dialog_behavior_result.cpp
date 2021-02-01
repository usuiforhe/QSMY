//
//  dialog_behavior_result.cpp
//  battle
//
//  Created by 汪燕军 on 14-2-8.
//  Copyright (c) 2014年 汪燕军. All rights reserved.
//

#include "dialog_behavior_result.h"

using namespace battle;

Json::Value dialog_behavior_result::to_json(){
    Json::Value json;
    //类型
    json["type"] = BEHAVIOR_RESULT_DIALOG;
    json["actor"] = actor_->get_id();
    json["dialog_id"] = dialog_id_;
    
    return json;
}

void dialog_behavior_result::append_to_bin(std::vector<char> &bin){
    //类型
    bin.push_back(static_cast<char>(BEHAVIOR_RESULT_DIALOG));
    //施法者编号
    bin.push_back(static_cast<char>(actor_->get_id()));
    //对话编号
    bin.push_back(static_cast<char>(dialog_id_ & 0x000000ff));
    bin.push_back(static_cast<char>((dialog_id_ & 0x0000ff00) >> 8));
    bin.push_back(static_cast<char>((dialog_id_ & 0x00ff0000) >> 16));
    bin.push_back(static_cast<char>((dialog_id_ & 0xff000000) >> 24));
}