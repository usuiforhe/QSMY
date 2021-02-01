//
//  damage_behavior_result.h
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__damage_behavior_result__
#define __battle__damage_behavior_result__

#include "battle_prefix.h"
#include "base/base_behavior_result.h"
#include "behavior_enums.h"

namespace battle {
    class damage_behavior_result:public base_behavior_result{
    public:
        damage_behavior_result(base_unit * actor, uint32_t damage, bool is_critical, bool is_dodge, bool is_block, uint32_t hp):base_behavior_result(actor), damage_(damage), is_critical_(is_critical), is_dodge_(is_dodge), is_block_(is_block), hp_(hp){
            
        }
        
        ~damage_behavior_result(){
            
        }
        
        virtual Json::Value to_json();
        
        virtual void append_to_bin(std::vector<char> & bin);
        
    private:
        uint32_t damage_;
        bool is_critical_;
        bool is_dodge_;
        bool is_block_;
        uint32_t hp_;
        
#define CRITICAL_FLAG 0x01
#define DODGE_FLAG 0x02
#define BLOCK_FLAG 0x04
    };
}

#endif /* defined(__battle__damage_behavior_result__) */
