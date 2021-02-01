//
//  skill_behavior.h
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__skill_behavior__
#define __battle__skill_behavior__

#include "battle_prefix.h"
#include "base/base_behavior.h"
#include "behavior_enums.h"

namespace battle {
    class skill_behavior:public base_behavior{
    public:
        skill_behavior(base_unit * actor, uint32_t skill_id, uint8_t position, uint8_t target):
        base_behavior( actor),
        skill_id_(skill_id),
        position_(position),
        target_(target){
            
        }
        
        ~skill_behavior(){
            
        }
        
        void add_coactor(uint8_t unit_id){
            coactors_.push_back(unit_id);
        }
        
        virtual Json::Value to_json();
        
        virtual void append_to_bin(std::vector<char> & bin);
        
    private:
        uint32_t skill_id_;
        uint8_t position_;
        uint8_t target_;
        std::vector<uint8_t> coactors_;
    };
}

#endif /* defined(__battle__skill_behavior__) */
