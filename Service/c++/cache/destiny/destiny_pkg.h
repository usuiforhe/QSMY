//
//  destiny_pkg.h
//  cache
//
//  Created by 汪燕军 on 13-5-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__destiny_pkg__
#define __cache__destiny_pkg__

#include "cache_prefix.h"
#include "base/base_player_object.h"
#include "destiny.h"

namespace cache {
    class destiny_pkg:public base_player_object, public boost::enable_shared_from_this<destiny_pkg>{
    public:
        explicit destiny_pkg(uint32_t user_id):base_player_object(user_id){
            
        }
        
        virtual ~destiny_pkg(){}
        
        virtual void load();
        
        destiny_ptr get(uint32_t destiny_id);
        
        error_code save(destiny_ptr destiny);
        
        void copy_to_destinies_res(pro_destinies_res_ptr res, uint8_t type = 0);
        
    private:
        typedef boost::unordered_map<uint32_t, destiny_ptr> id_map_t;
        id_map_t id_map_;
        typedef boost::unordered_map<uint8_t, boost::shared_ptr<std::vector<destiny_ptr> > > type_map_t;
        type_map_t type_map_;
        
        boost::shared_mutex mutex_;
        void add(destiny_ptr destiny);
        
        bool db_insert(destiny_ptr destiny);
        bool db_update(destiny_ptr destiny);
    };
}

#endif /* defined(__cache__destiny_pkg__) */
