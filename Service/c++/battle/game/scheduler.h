//
//  scheduler.h
//  battle
//
//  Created by wangyanjun on 13-3-18.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__scheduler__
#define __battle__scheduler__

#include "battle_prefix.h"
#include "unit/unit_task.h"

namespace battle {
    class unit;
    class scene;
    class scheduler{
    public:
        explicit scheduler(scene * scene):scene_(scene), prior_force_id_(0), frame_distance_(0), frames_(0), action_times_(0){
            
        }
        
        ~scheduler();
        
        void init();
        
        void execute();
        
        void add_task(unit_task * task);
        
        void bind_task(unit_task * task);
        
        inline GETTER(scene*, scene);
        
        inline GETTER(uint32_t, frame_distance);
        
        inline GETTER(uint32_t, frames);
        
        inline uint32_t get_action_times(){
            return action_times_;
        }
        
        inline void increase_action_times(){
            ++action_times_;
        }
        
        inline uint8_t get_prior_force_id(){
            //每使用一次优先势力，则做一次变换，以交替优先
            if(prior_force_id_ == LEFT_FORCE_ID){
                prior_force_id_ = RIGHT_FORCE_ID;
                return LEFT_FORCE_ID;
            }
            else{
                prior_force_id_ = LEFT_FORCE_ID;
                return RIGHT_FORCE_ID;
            }
        }
        
    private:
        scene * scene_;
        uint8_t prior_force_id_;
        uint32_t frame_distance_;
        uint32_t frames_;
        uint32_t action_times_;
        
        typedef boost::unordered_map<uint32_t, unit_task_list*> tasks_map_t;
        tasks_map_t tasks_map_;
        std::list<unit_task*> task_list_;
                
        inline unit_task_list * init_next_frame();
        
    };
    
    class unit_task_sorter{
    public:
        unit_task_sorter(){}
        
        virtual ~unit_task_sorter(){}
        
        virtual bool operator()(unit_task * u1, unit_task * u2);
    };
}

#define S_UNIT_task_SORTER latte::singleton<unit_task_sorter>::instance()

#endif /* defined(__battle__scheduler__) */
