//
//  scheduler.cpp
//  battle
//
//  Created by wangyanjun on 13-3-18.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "scheduler.h"
#include "unit/unit.h"
#include "scene.h"
#include "force/force.h"
#include "random/random_helper.h"
#include "log/log.h"
#include "util.h"

using namespace battle;

scheduler::~scheduler(){
    BOOST_FOREACH(unit_task * task, task_list_){
        delete task;
    }
    
    BOOST_FOREACH(tasks_map_t::value_type pair, tasks_map_){
        delete pair.second;
    }
}

void scheduler::init(){
    force * left_force = scene_->get_left_force();
    force * right_force = scene_->get_right_force();
    
    //每回合的路程为素对最快的单位的速度
    if(left_force->get_max_speed() >= right_force->get_max_speed()){
        frame_distance_ = left_force->get_max_speed();
    }
    else{
        frame_distance_ = right_force->get_max_speed();
    }
    
    if(left_force->get_total_speed() > right_force->get_total_speed()){
        prior_force_id_ = LEFT_FORCE_ID;
    }
    else if( left_force->get_total_speed() == right_force->get_total_speed()){
        if(get_random(1, 100) <= 50){
            prior_force_id_ = LEFT_FORCE_ID;
        }
        else{
            prior_force_id_ = RIGHT_FORCE_ID;
        }
    }
    else{
        prior_force_id_ = RIGHT_FORCE_ID;
    }
}

void scheduler::execute(){
    scene_->check_end();
    while(!scene_->is_end()){
        unit_task_list * task_list = init_next_frame();
        if(task_list == NULL)
        {
            continue;
        }
        
#if DEBUG
        
        std::stringstream & ss = latte::fast_stringstream::ss();
        ss << "---------------------------------------------------------" << std::endl;
        BOOST_FOREACH(unit_task * task, *task_list){
            ss << "id:" << static_cast<uint32_t>(task->get_owner()->get_id());
            ss << ",position:" << static_cast<uint32_t>(task->get_owner()->get_position()->get_id()) << std::endl;
        }
        
        ss << "start execute frame:" << frames_ << std::endl;
        ss << "---------------------------------------------------------";
        s_log.out_debug(ss.str());
        
#endif
        while(task_list->size() > 0 && !scene_->is_end()){
            unit_task * task = task_list->front();
            task_list->pop_front();
            
            if(task->get_bind_frame() != frames_)
            {
                continue;
            }
            try{
                task->execute();
                scene_->check_end();
            }
            catch (boost::exception &e)
            {
                std::stringstream &ss = latte::fast_stringstream::ss();
                ss << "scheduler::execute task->execute() throws exceptions. what:" + boost::diagnostic_information(e);
                s_log.out_error(ss.str());
            }
            catch (std::exception &e)
            {
                std::stringstream &ss = latte::fast_stringstream::ss();
                ss << "scheduler::execute task->execute() throws exceptions. what:" + boost::diagnostic_information(e);
                s_log.out_error(ss.str());
            }
        }
    }
}

void scheduler::add_task(unit_task * task){
    bind_task(task);
    task_list_.push_back(task);
}

void scheduler::bind_task(unit_task * task){
    BOOST_AUTO(pos, tasks_map_.find(task->get_bind_frame()));
    if(pos != tasks_map_.end()){
        pos->second->push_back(task);
    }
    else{
        unit_task_list* list = new unit_task_list;
        list->push_back(task);
        tasks_map_.insert(std::make_pair(task->get_bind_frame(), list));
    }
}

//-------------------------------private methods start-----------------------------------

inline unit_task_list* scheduler::init_next_frame(){    
    ++frames_;
    
    BOOST_AUTO(pos, tasks_map_.find(frames_));
    if(pos == tasks_map_.end()){
        return NULL;
    }
    unit_task_list* task_list = pos->second;
    task_list->sort(S_UNIT_task_SORTER);
    for(BOOST_AUTO(pos, task_list->begin()); pos != task_list->end(); ++pos){
        if(pos == task_list->begin()) {
            continue;
        }
        unit_task * previous = *(--pos);
        unit_task * current = *(++pos);
        
#if DEBUG
        std::stringstream & ss = latte::fast_stringstream::ss();
        ss << "previous id:" << static_cast<uint32_t>(previous->get_owner()->get_id()) << ", position:" << static_cast<uint32_t>(previous->get_owner()->get_position()->get_id()) << ", priority:" << previous->get_priority() << "\n";
        
        ss << "current id:" << static_cast<uint32_t>(current->get_owner()->get_id()) << ", position:" << static_cast<uint32_t>(current->get_owner()->get_position()->get_id()) << ", priority:" << current->get_priority() << std::endl;
        s_log.out_debug(ss.str());
#endif
        
        if(current->get_priority() != previous->get_priority() || current->get_owner()->get_position()->get_id() != previous->get_owner()->get_position()->get_id())
        {
            continue;
        }
        
        if(current->get_owner()->get_force()->get_id() == get_prior_force_id()){
            *(--pos) = current;
            *(++pos) = previous;
        }
        
        ++pos;
        if(pos == task_list->end()) break;
    }
    
    return task_list;
}


//-------------------------------unit task sorter start--------------------------------

bool unit_task_sorter::operator()(unit_task * t1, unit_task * t2){
    unit * u1 = t1->get_owner();
    unit * u2 = t2->get_owner();
    
    if(t1->get_priority() < t2->get_priority()) {
        return true;
    }
    
    if(t1->get_priority() > t2->get_priority()) {
        return false;
    }
    
    if(u1->get_position()->get_id() <= u2->get_position()->get_id()) {
        return true;
    }
    else {
        return false;
    }
}

