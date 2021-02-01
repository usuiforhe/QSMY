//
//  player_manager.h
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__player_manager__
#define __cache__player_manager__

#include "cache_prefix.h"
#include "cache/lru_cache.h"
#include "operation_queue/operation_queue.h"
#include "player.h"
#include "config.h"
#include "cache_service.h"

namespace cache {
    class player_manager{
    public:
        player_manager():cache_(SINGLETON(config).cache_capacity, SINGLETON(cache_service).thread_pool()){
            
        }
        
        ~player_manager(){
            
        }
        
        void init();
        
        inline void dispose(){
            op_queue_.join();
        }
        
        inline player_ptr get_player(uint32_t user_id){
            bool got(false),error(false);
            player_ptr player = cache_.get(user_id, boost::bind(&player::create_player, _1, _2), got, error);
            
            player->initialize();
            
            return player;
        }
        
        inline GETTER(latte::operation_queue &, op_queue);
        
    private:
        latte::lru_cache<uint32_t, player_ptr> cache_;
        latte::operation_queue op_queue_;
    };
    
#define OP_QUEUE_KEY(keydef, ssdef, ns, user, id) ssdef = latte::fast_stringstream::ss();\
    ss << ns << ":" << user << ":" << id;\
    keydef = ss.str();
}

#endif /* defined(__cache__player_manager__) */
