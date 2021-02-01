//
//  target_rule.h
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__target_rule__
#define __battle__target_rule__

#include "battle_prefix.h"
#include "singleton.h"

namespace battle {
    class target_rule{
    public:
        target_rule();
        ~target_rule();
        
        std::list<uint8_t>* get_target_list(uint8_t position);
        
        std::list<uint8_t>* get_protectee_list(uint8_t position);
        
        std::list<uint8_t>* get_protector_list(uint8_t position);
        
    private:
        boost::unordered_map<uint8_t, std::list<uint8_t>* > target_list_map_;
        boost::unordered_map<uint8_t, std::list<uint8_t>* > protectee_list_map_;
        boost::unordered_map<uint8_t, std::list<uint8_t>* > protector_list_map_;
    };
}

#define S_TARGET_RULE latte::singleton<target_rule>::instance()

#endif /* defined(__battle__target_rule__) */
