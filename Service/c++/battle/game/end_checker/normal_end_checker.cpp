//
//  normal_end_checker.cpp
//  battle
//
//  Created by 汪燕军 on 13-5-3.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "normal_end_checker.h"
#include "game/scene.h"
using namespace battle;

void normal_end_checker::check(battle::scene *scene){
    force * left = scene->get_left_force();
    force * right = scene->get_right_force();
    if(left->is_dead()){
        right->win();
        return;
    }
    
    if(right->is_dead()){
        left->win();
        return;
    }
    
    if(scene->get_scheduler().get_action_times() >= MAX_ACTION_TIMES){
        double left_percent = left->get_fight() / left->get_max_fight();
        double right_percent = right->get_fight() / right->get_max_fight();
        if(left_percent >= right_percent){
            left->win();
        }
        else{
            right->win();
        }
    }
}
