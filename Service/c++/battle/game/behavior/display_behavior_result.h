//
//  display_behavior_result.h
//  battle
//
//  Created by 汪燕军 on 14-2-8.
//  Copyright (c) 2014年 汪燕军. All rights reserved.
//

#ifndef __battle__display_behavior_result__
#define __battle__display_behavior_result__

#include "battle_prefix.h"
#include "base/base_behavior_result.h"
#include "behavior_enums.h"

namespace battle {
    class display_behavior_result:public base_behavior_result{
    public:
        display_behavior_result(base_unit * actor, bool is_show):base_behavior_result(actor), is_show_(is_show){
            
        }
        
        ~display_behavior_result(){
            
        }
        
        virtual Json::Value to_json();
        
        virtual void append_to_bin(std::vector<char> & bin);
        
    private:
        bool is_show_;
    };
}

#endif /* defined(__battle__display_behavior_result__) */
