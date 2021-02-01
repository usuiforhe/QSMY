//
//  command_center.h
//  cache
//
//  Created by 汪燕军 on 13-9-17.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef cache_command_center_h
#define cache_command_center_h


#include "cache_prefix.h"
#include "config.h"
#include "player_manager.h"
namespace cache {
    class command_center{
    public:
        std::string get_opq_count(){
            return boost::lexical_cast<std::string>(SINGLETON(player_manager).op_queue().count());
        }
    };
}




#endif
