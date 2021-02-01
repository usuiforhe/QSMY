//
//  soul_pkg.h
//  cache
//
//  Created by 汪燕军 on 13-5-10.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__soul_pkg__
#define __cache__soul_pkg__


#include "cache_prefix.h"
#include "base/base_player_object.h"
#include "soul.h"

namespace cache {
    class soul_pkg : public base_player_object, public boost::enable_shared_from_this<soul_pkg>{
    public:
        explicit soul_pkg(uint32_t user_id):base_player_object(user_id){
            
        }
        virtual ~soul_pkg(){}
        
        virtual void load();
        
        soul_ptr get(uint32_t soul_id);
        
        error_code add(soul_ptr soul);
        
        error_code add(std::vector<soul_ptr> & list);
        
        error_code remove(soul_ptr soul);
        
        error_code remove(std::vector<soul_ptr> & list);
        
        void copy_to_souls_res(pro_souls_res_ptr res, std::vector<uint32_t> & souls);
        
    private:
        typedef boost::unordered_map<uint32_t, soul_ptr> map_t;
        map_t map_;
        
        boost::shared_mutex mutex_;
        
        void async_update(soul_ptr soul);
        
        bool db_insert(soul_ptr soul);
        bool db_insert(sql::Connection * conn, soul_ptr soul);
        bool db_insert(std::vector<soul_ptr> & list);
        bool db_update(soul_ptr soul);
    };
}

#endif /* defined(__cache__soul_pkg__) */
