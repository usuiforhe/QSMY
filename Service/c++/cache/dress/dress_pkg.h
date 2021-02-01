//
//  dress_pkg.h
//  cache
//
//  Created by 汪燕军 on 14-3-4.
//  Copyright (c) 2014年 汪燕军. All rights reserved.
//

#ifndef __cache__dress_pkg__
#define __cache__dress_pkg__


#include "cache_prefix.h"
#include "base/base_player_object.h"
#include "dress.h"

namespace cache {
    class dress_pkg:public base_player_object, public boost::enable_shared_from_this<dress_pkg>{
    public:
        explicit dress_pkg(uint32_t user_id):base_player_object(user_id){
            
        }
        
        virtual ~dress_pkg(){}
        
        virtual void load();
        
        dress_ptr get(uint32_t sid);
        
        boost::shared_ptr<std::vector<dress_ptr> > get_by_warrior(uint32_t warrior_id);
        
        error_code add(dress_ptr dress);
        
        error_code update(dress_ptr dress);
        
        error_code remove(uint32_t sid);
        
        void copy_to_dresses_res(pro_dresses_res_ptr res);
        
    private:
        typedef boost::unordered_map<uint32_t, dress_ptr> map_t;
        typedef boost::unordered_map<uint32_t, boost::shared_ptr<std::vector<dress_ptr> > > warrior_map_t;
        map_t map_;
        
        warrior_map_t warrior_map_;
        
        boost::shared_mutex mutex_;
        
        void async_update(dress_ptr dress);
        void async_remove(uint64_t id);
        
        bool db_insert(dress_ptr dress);
        bool db_update(dress_ptr dress);
        bool db_remove(uint64_t id);
    };
}


#endif /* defined(__cache__dress_pkg__) */
