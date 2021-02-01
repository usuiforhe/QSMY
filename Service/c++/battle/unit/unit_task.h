//
//  unit_task.h
//  battle
//
//  Created by wangyanjun on 13-3-18.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__unit_task__
#define __battle__unit_task__

#include "battle_prefix.h"
#include "unit.h"

namespace battle {
    class unit;
    class scene;
    class scheduler;
    class unit_task{
    public:
        unit_task(unit * owner, uint32_t bind_frame, double priority):
        owner_(owner),
        scene_(owner->get_scene()),
        bind_frame_(bind_frame),
        priority_(priority)
        {
        }
        
        virtual ~unit_task(){}
        
        virtual void execute() = 0;
        
        inline GETTER(unit*, owner);
        inline GETTER(uint32_t, bind_frame);
        
        inline GETTER(double, priority);        
        inline SETTER(double, priority);
    protected:
        unit * owner_;
        scene * scene_;
        uint32_t bind_frame_;
        double priority_;
    };
    
    class unit_action_task:public unit_task{
    public:
        unit_action_task(unit * owner, uint32_t bind_frame = 0, double priority = 10000000, bool is_main = true);
        
        ~unit_action_task(){}
        
        virtual void execute();
        
    private:
        bool is_main_;
        
        void bind_next_frame(bool bind_to_scheduler = false);
    };
    
    typedef std::list<unit_task*> unit_task_list;
}

#endif /* defined(__battle__unit_task__) */
