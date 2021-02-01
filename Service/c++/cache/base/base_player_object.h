//
//  base_player_object.h
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef cache_base_player_object_h
#define cache_base_player_object_h

#include "cache_prefix.h"
namespace cache {
    class base_player_object{
    public:
        explicit base_player_object(uint32_t user_id):user_id_(user_id){
            
        }
        
        virtual ~base_player_object(){
            
        }
        
        virtual void load() = 0;
        
    protected:
        uint32_t user_id_;
    };
}

#endif
