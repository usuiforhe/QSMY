//
//  buff_effect_behavior.h
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__buff_effect_behavior__
#define __battle__buff_effect_behavior__

#include "battle_prefix.h"
#include "base/base_behavior.h"
#include "behavior_enums.h"

namespace battle {
    class buff_effect_behavior:public base_behavior{
    public:
        buff_effect_behavior(base_unit * actor, uint32_t buff_id):base_behavior(actor), buff_id_(buff_id){
            
        }
        
        ~buff_effect_behavior(){
            
        }
        
        virtual Json::Value to_json();
        
        virtual void append_to_bin(std::vector<char> & bin);
        
    private:
        uint32_t buff_id_;
    };
}

#endif /* defined(__battle__buff_effect_behavior__) */
