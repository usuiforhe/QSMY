//
//  player.h
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__player__
#define __cache__player__

#include "cache_prefix.h"

namespace cache {
    class player{
    public:
        player():initialized_(false)
        {
            
        }
        ~player(){
            
        }
        
        inline GETTER(equip_pkg_ptr, equip_pkg)
        inline GETTER(item_pkg_ptr, item_pkg)
        inline GETTER(soul_pkg_ptr, soul_pkg)
        inline GETTER(destiny_pkg_ptr, destiny_pkg)
        inline GETTER(warrior_pkg_ptr, warrior_pkg)
        inline GETTER(dress_pkg_ptr, dress_pkg)
        inline GETTER(updater_ptr, updater)
        
        inline GETTER(uint32_t, user_id)
        
        inline GETTER(bool, initialized)
        
        inline GETTER(boost::timed_mutex &, mutex);
        
        void initialize();
        
        static player_ptr create_player(uint32_t user_id, bool & got);
        
    private:
        uint32_t user_id_;
        equip_pkg_ptr equip_pkg_;
        item_pkg_ptr item_pkg_;
        soul_pkg_ptr soul_pkg_;
        destiny_pkg_ptr destiny_pkg_;
        warrior_pkg_ptr warrior_pkg_;
        dress_pkg_ptr dress_pkg_;
        updater_ptr updater_;
        
        bool initialized_;
        
        boost::timed_mutex mutex_;
        
        boost::mutex self_mutex_;
        
        explicit player(uint32_t user_id)
        :user_id_(user_id)
        ,initialized_(false){
            
        }
    };    
}

#endif /* defined(__cache__player__) */
