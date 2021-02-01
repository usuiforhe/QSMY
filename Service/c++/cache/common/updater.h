//
//  updater.h
//  cache
//
//  Created by 汪燕军 on 13-11-26.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__updater__
#define __cache__updater__

#include "cache_prefix.h"
#include "base/base_player_object.h"
#include "protocol/common.pb.h"
#include "base/async_update_object.h"

namespace cache
{
    class update_request
    {
    public:
        update_request(const std::string & key, const std::string sql)
        :key_(key), sql_(sql)
        {
        }
        
        virtual ~update_request(){}
                
        GETTER(const std::string &, key);
        
        void sql(const std::string & sql)
        {
            sql_ = sql;
        }
        
        GETTER(const std::string &, sql);
        
        GETTER(time_t, time);
        
    private:
        std::string key_;
        std::string sql_;
        time_t time_;
    };
    
    typedef boost::shared_ptr<update_request> update_request_ptr;
    
    class updater:public base_player_object, public async_update_object, public boost::enable_shared_from_this<updater>
    {
    public:
        explicit updater(uint32_t user_id)
        : base_player_object(user_id)
        , async_update_object()
        , mutex_()
        , sql_map_()
        , pending_update_(false){
            sql_map_.reserve(1000);
        }
        
        virtual ~updater(){
            
        }
        
        void load(){}
        
        void request_update(pro_async_update_ptr update);
        
    private:
        boost::mutex mutex_;
        typedef boost::unordered_map<std::string, update_request_ptr> sql_map_t;
        sql_map_t sql_map_;
        bool pending_update_;
        
        bool real_update();
    };
}

#endif /* defined(__cache__updater__) */
