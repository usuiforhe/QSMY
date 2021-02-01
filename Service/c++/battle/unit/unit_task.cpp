//
//  unit_task.cpp
//  battle
//
//  Created by wangyanjun on 13-3-18.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "unit_task.h"
#include "unit.h"
#include "game/scene.h"
#include "game/scheduler.h"

using namespace battle;

unit_action_task::unit_action_task(unit * owner, uint32_t bind_frame, double priority, bool is_main):
    unit_task(owner, bind_frame, priority),
    is_main_(is_main)
{
    if(bind_frame == 0){
        bind_next_frame();
    }
}

void unit_action_task::execute(){
    if(owner_->is_alive()){
        if(owner_->action(is_main_)){
            scene_->get_scheduler().increase_action_times();
        }
    }
    
    if(is_main_ && owner_->is_alive() && !owner_->get_scene()->is_end()){
        bind_next_frame(true);
    }
}

void unit_action_task::bind_next_frame(bool bind_to_scheduler){
    scheduler & scheduler = owner_->get_scene()->get_scheduler();
    uint32_t distance = scheduler.get_frame_distance();
    
    uint32_t next_total_distance = (owner_->get_self_frames() + 1) * distance;
    
    bind_frame_ = (next_total_distance + owner_->speed - 1) / owner_->speed;
    
    priority_ = static_cast<double>(next_total_distance - (bind_frame_ - 1) * owner_->speed) / owner_->speed;
    if(bind_to_scheduler)
    {
        scheduler.bind_task(this);
    }
}
