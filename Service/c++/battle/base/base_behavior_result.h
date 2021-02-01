//
//  base_behavior_result.h
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef battle_base_behavior_result_h
#define battle_base_behavior_result_h

#include "battle_prefix.h"
#include "base_unit.h"

namespace battle {
    class base_behavior_result{
    public:
        base_behavior_result(base_unit * actor):actor_(actor){
            
        }
        
        virtual ~base_behavior_result(){
            
        }
        
        inline GETTER(base_unit*, actor);
        
        virtual Json::Value to_json() = 0;
        
        virtual void append_to_bin(std::vector<char> & bin) = 0;
    protected:
        base_unit * actor_;
    };
}


#endif
