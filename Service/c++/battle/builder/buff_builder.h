//
//  buff_builder.h
//  battle
//
//  Created by wangyanjun on 13-3-19.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__buff_builder__
#define __battle__buff_builder__

#include "battle_prefix.h"
#include "buff/unit_buff.h"
#include "singleton.h"
namespace battle {
    typedef enum buff_expire_t{
        BUFF_EXPIRE_TYPE_UNIT_FRAMES = 1
    } buff_expire_t;
    
    class scene;
    class unit;
    class buff_builder{
    public:
        buff_builder(){
            
        }
        
        ~buff_builder(){
            
        }
        
        unit_buff * build(unit * owner, uint32_t sid, uint32_t source_unit_id, uint32_t source_skill_id, const lua_object & meta_data);
    };
}

#define S_BUFF_BUILDER latte::singleton<buff_builder>::instance()

#endif /* defined(__battle__buff_builder__) */
