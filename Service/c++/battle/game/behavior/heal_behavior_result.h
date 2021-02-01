//
//  heal_behavior_result.h
//  battle
//
//  Created by 汪燕军 on 13-5-2.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__heal_behavior_result__
#define __battle__heal_behavior_result__

#include "battle_prefix.h"
#include "base/base_behavior_result.h"
#include "behavior_enums.h"

namespace battle{
    class heal_behavior_result: public base_behavior_result{
    public:
        heal_behavior_result(base_unit * actor, uint32_t heal, bool is_critical, uint32_t hp):base_behavior_result(actor), heal_(heal), is_critical_(is_critical), hp_(hp){
            
        }
        
        ~heal_behavior_result(){
            
        }
        
        virtual Json::Value to_json();
        
        virtual void append_to_bin(std::vector<char> & bin);
        
    private:
        uint32_t heal_;
        bool is_critical_;
        uint32_t hp_;
        
#define CRITICAL_FLAG 0x01
#define DODGE_FLAG 0x02
#define BLOCK_FLAG 0x04
    };
}

#endif /* defined(__battle__heal_behavior_result__) */
