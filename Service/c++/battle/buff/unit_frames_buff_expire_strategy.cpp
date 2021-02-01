//
//  unit_frames_buff_expire_strategy.cpp
//  battle
//
//  Created by wangyanjun on 13-3-19.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "unit_frames_buff_expire_strategy.h"
#include "unit/unit.h"

using namespace battle;

void unit_frames_buff_expire_strategy::on_add(){
    add_frames_ = owner_->get_self_frames();
}

bool unit_frames_buff_expire_strategy::is_expired(){
    return owner_->get_self_frames() > add_frames_ + last_frames_;
}

void unit_frames_buff_expire_strategy::refresh(){
    add_frames_ = owner_->get_self_frames();    
}
