//
//  equip_pkg.h
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__equip_pkg__
#define __cache__equip_pkg__

#include "cache_prefix.h"
#include "base/base_player_object.h"
#include "protocol/equip.pb.h"

namespace cache {
    class equip_pkg : public base_player_object, public boost::enable_shared_from_this<equip_pkg>{
    public:
        explicit equip_pkg(uint32_t user_id):base_player_object(user_id){
            
        }
        virtual ~equip_pkg(){}
        
        virtual void load();
                
        equip_ptr get(uint64_t id);
        
        boost::shared_ptr<std::vector<equip_ptr> > get_by_pos(uint32_t pos);
        
        error_code add(equip_ptr equip);
        
        error_code add(std::vector<equip_ptr> & list);
        
        error_code update(equip_ptr equip);
        
        error_code update(std::vector<equip_ptr> & list);
        
        error_code remove(uint64_t id);
        
        error_code remove(std::vector<uint64_t> & list);
        
        void copy_to_equips_res(pro_equips_res_ptr res);
        
    private:
        typedef boost::unordered_map<uint64_t, equip_ptr> id_map_t;
        id_map_t id_map_;
        typedef boost::unordered_map<uint32_t, boost::shared_ptr<std::vector<equip_ptr> > > pos_map_t;
        pos_map_t pos_map_;
        
        boost::shared_mutex mutex_;
        
        void set(equip_ptr to_set);
        
        void set_pos(equip_ptr equip);
        void unset_pos(equip_ptr equip);
        
        void async_update(equip_ptr equip);
        
        void async_remove(equip_ptr equip);
        
        bool db_insert(equip_ptr equip);
        bool db_insert(sql::Connection * conn, equip_ptr equip);
        bool db_insert(std::vector<equip_ptr> & list);
        bool db_update(equip_ptr equip);
        bool db_remove(uint64_t id);
    };
}

#endif /* defined(__cache__equip_pkg__) */
