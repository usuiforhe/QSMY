//
//  assistant.cpp
//  battle
//
//  Created by 汪燕军 on 14-7-2.
//  Copyright (c) 2014年 汪燕军. All rights reserved.
//

#include "assistant.h"

using namespace battle;

assistant::assistant(scene * scene, force * force, user * user, uint8_t id, uint32_t sid)
:unit(scene, force, user, id, sid, unit_type_assist)
{
    
}

assistant::~assistant(){
    
}

void assistant::init(){
    init_hp = hp;
    fight_factor_ = 0;
    is_show = false;
    
    state_.set_can_attack(false);
    state_.set_can_be_attacked(false);
    state_.set_can_block(false);
    state_.set_can_dodge(false);
}

bool assistant::action(bool inc_self_frames_)
{
    return true;
}


