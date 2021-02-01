//
//  user.h
//  battle
//
//  Created by wangyanjun on 13-3-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__user__
#define __battle__user__

#include "battle_prefix.h"

namespace battle {
    class force;
    class unit;
    class scene;
    class user{
        friend class user_builder;
    public:
        user(force * force, uint32_t user_id, uint16_t level, bool is_mob = false):
            force_(force),
            user_id_(user_id),
            level_(level),
            is_mob_(is_mob),
            total_speed_(0),
            max_speed_(0){
            
        }
        
        ~user();
        
        void add_unit(unit * unit);
        
        GETTER(force * , force);
        GETTER(uint32_t, user_id);
        GETTER(const std::string &, name);
        GETTER(uint16_t, level);
        GETTER(bool, is_mob);
        GETTER(uint32_t, total_speed);
        GETTER(uint32_t, max_speed);
        
        scene * get_scene();
        
        double get_fight();
        
        double get_max_fight();
        
    private:        
        force * force_;
        
        uint32_t user_id_;
        std::string name_;
        uint16_t level_;
        bool is_mob_;
        
        uint32_t total_speed_;
        uint32_t max_speed_;
        
        typedef std::map<uint8_t, unit*> unit_map_t;
        unit_map_t unit_map_;
    };
}

#endif /* defined(__battle__user__) */
