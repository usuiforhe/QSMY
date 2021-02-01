//
//  buff_behavior_result.h
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__buff_behavior_result__
#define __battle__buff_behavior_result__

#include "battle_prefix.h"
#include "base/base_behavior_result.h"
#include "behavior_enums.h"

namespace battle {
    class buff_behavior_result:public base_behavior_result{
    public:
        buff_behavior_result(base_unit * actor, uint32_t buff_id, bool is_add):base_behavior_result(actor), buff_id_(buff_id), is_add_(is_add){
            
        }
        
        ~buff_behavior_result(){
            
        }
        
        virtual Json::Value to_json();
        
        virtual void append_to_bin(std::vector<char> & bin);
        
    private:
        uint32_t buff_id_;
        bool is_add_;
    };
}

#endif /* defined(__battle__buff_behavior_result__) */
