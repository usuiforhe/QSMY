//
//  updater.cpp
//  cache
//
//  Created by 汪燕军 on 13-11-26.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "updater.h"
#include "player.h"
#include "player_manager.h"

using namespace cache;

void updater::request_update(pro_async_update_ptr update)
{
    {
        boost::mutex::scoped_lock lock(mutex_);
        BOOST_AUTO(pos, sql_map_.find(update->key()));
        if(pos == sql_map_.end())
        {
            update_request_ptr request(new update_request(update->key(), update->sql()));
            sql_map_.insert(std::make_pair(request->key(), request));
        }
        else
        {
            pos->second->sql(update->sql());
        }
        if(!pending_update_)
        {
            pending_update_ = true;
            OP_QUEUE_KEY(std::string op_key, std::stringstream & ss, "updater::real_update", user_id_, "");
            SINGLETON(player_manager).op_queue().enque(op_key , boost::bind(&updater::real_update, shared_from_this()));
        }
    }
}

bool updater::real_update()
{   
    if(!can_update())
    {
        return false;
    }
    
    boost::unordered_map<std::string, update_request_ptr> requests;
    {
        boost::mutex::scoped_lock lock(mutex_);
        requests = sql_map_;
        sql_map_.clear();
        pending_update_ = false;
    }
    if(requests.size() == 0)
    {
        return true;
    }
    
    update_request_ptr request;
    try
    {
        SCOPED_CONNECT(scoped, connection);
        connection->setAutoCommit(false);
        CREATE_STMT(connection, stmt);
        BOOST_FOREACH(sql_map_t::value_type val, requests)
        {
            request = val.second;
            stmt->executeUpdate(request->sql());
            
#if DEBUG
            std::stringstream &ss = latte::fast_stringstream::ss();
            ss << val.second->sql();
            s_log.out_string(ss.str());
#endif
        }
        stmt->close();
        connection->commit();
        last_update_ts_ = TIME_NOW.get_time_t();
        return true;
    }
    catch(const std::exception & e)
    {
        if(request)
        {
            std::stringstream &ss = latte::fast_stringstream::ss();
            ss << "updater::real_update(" << request->sql() << " throws exceptions. what:" + boost::diagnostic_information(e);
            s_log.out_error(ss.str());
        }
    }
    catch(const boost::exception & e)
    {
        if(request)
        {
            std::stringstream &ss = latte::fast_stringstream::ss();
            ss << "updater::real_update(" << request->sql() << " throws exceptions. what:" + boost::diagnostic_information(e);
            s_log.out_error(ss.str());
        }
    }
    
    boost::mutex::scoped_lock lock(mutex_);
    {
        BOOST_FOREACH(sql_map_t::value_type val, requests)
        {
            update_request_ptr request(val.second);
            BOOST_AUTO(pos, sql_map_.find(request->key()));
            if(pos == sql_map_.end())
            {
                sql_map_.insert(std::make_pair(request->key(), request));
                pending_update_ = true;
            }
        }
    }
    return false;
}
