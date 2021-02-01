//
//  item_pkg.h
//  cache
//
//  Created by 汪燕军 on 13-5-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__item_pkg__
#define __cache__item_pkg__

#include "cache_prefix.h"
#include "base/base_player_object.h"
#include "protocol/item.pb.h"

namespace cache {
    class item_pkg : public base_player_object, public boost::enable_shared_from_this<item_pkg>{
    public:
        explicit item_pkg(uint32_t user_id):base_player_object(user_id){
            
        }
        virtual ~item_pkg(){}
        
        virtual void load();
        
        item_ptr get(uint32_t item_id);
        
        error_code add(item_ptr item);
        
        error_code add(std::vector<item_ptr> & list);
        
        error_code remove(item_ptr item);
        
        error_code remove(std::vector<item_ptr> & list);
        
        void copy_to_items_res(pro_items_res_ptr res);
        
    private:
        typedef boost::unordered_map<uint32_t, item_ptr> map_t;
        map_t map_;
        
        boost::shared_mutex mutex_;
        
        void async_update(item_ptr item);
        void async_remove(uint64_t id);
        
        bool db_insert(item_ptr item);
        bool db_insert(sql::Connection * conn, item_ptr item);
        bool db_insert(std::vector<item_ptr> & list);
        bool db_update(item_ptr item);
        bool db_remove(uint64_t id);
    };
}


#endif /* defined(__cache__item_pkg__) */
