//
//  dialog_behavior_result.h
//  battle
//
//  Created by 汪燕军 on 14-2-8.
//  Copyright (c) 2014年 汪燕军. All rights reserved.
//

#ifndef __battle__dialog_behavior_result__
#define __battle__dialog_behavior_result__


#include "battle_prefix.h"
#include "base/base_behavior.h"
#include "behavior_enums.h"

namespace battle {
    class dialog_behavior_result:public base_behavior_result{
    public:
        dialog_behavior_result(base_unit * actor, uint32_t dialog_id):
        base_behavior_result( actor),
        dialog_id_(dialog_id){
            
        }
        
        ~dialog_behavior_result(){
            
        }
        
        virtual Json::Value to_json();
        
        virtual void append_to_bin(std::vector<char> & bin);
        
    private:
        uint32_t dialog_id_;
    };
}

#endif /* defined(__battle__dialog_behavior_result__) */
