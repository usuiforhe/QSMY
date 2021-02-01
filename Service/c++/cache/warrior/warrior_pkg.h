//
//  warrior_pkg.h
//  cache
//
//  Created by 汪燕军 on 13-5-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__warrior_pkg__
#define __cache__warrior_pkg__


#include "cache_prefix.h"
#include "base/base_player_object.h"
#include "warrior.h"

namespace cache {
    class warrior_pkg:public base_player_object, public boost::enable_shared_from_this<warrior_pkg>{
    public:
        explicit warrior_pkg(uint32_t user_id):base_player_object(user_id){
            
        }
        
        virtual ~warrior_pkg(){}
        
        virtual void load();
        
        warrior_ptr get(uint32_t sid);
        
        error_code add(warrior_ptr warrior);
        
        error_code update(warrior_ptr warrior);
        
        error_code update(std::vector<warrior_ptr> & list);
        
        error_code remove(uint32_t sid);
        
        error_code remove(const std::vector<uint32_t> & list);
        
        void copy_to_warriors_res(pro_warriors_res_ptr res);
        
    private:
        typedef boost::unordered_map<uint32_t, warrior_ptr> map_t;
        map_t map_;
        
        boost::shared_mutex mutex_;
        
        void async_update(warrior_ptr warrior);
        void async_remove(uint64_t id);
        
        bool db_insert(warrior_ptr warrior);
        bool db_update(warrior_ptr warrior);
        bool db_remove(uint64_t id);
    };
}

#endif /* defined(__cache__warrior_pkg__) */
