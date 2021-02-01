//
//  async_update_object.h
//  cache
//
//  Created by 汪燕军 on 13-12-16.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef cache_async_update_object_h
#define cache_async_update_object_h

#include "cache_prefix.h"
namespace cache {
    class async_update_object{
    public:
        explicit async_update_object():last_update_ts_(0){
            
        }
        explicit async_update_object(time_t time):last_update_ts_(time){
            
        }
        
        virtual ~async_update_object(){
            
        }
        
        inline GETTER(time_t, last_update_ts);
        inline SETTER(time_t, last_update_ts);
        
        virtual bool can_update();
    
    protected:
        time_t last_update_ts_;
    };
}

#endif
