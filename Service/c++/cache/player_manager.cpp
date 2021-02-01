//
//  player_manager.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-8.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "player_manager.h"

using namespace cache;

void player_manager::init(){
    SCOPED_CONNECT(scoped, conn);
    
    PREPARE_SQL(conn, stmt, "\
                select `user_id` from `T_USER` ORDER BY `last_login_ts` DESC LIMIT 0, ?;");
    stmt->setUInt(1, SINGLETON(config).cache_capacity * 0.5);
    sql::ResultSet * raw_rs = stmt->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    while(rs->next()){
        SINGLETON(cache_service).thread_pool()->queue_work_item(boost::bind(&player_manager::get_player, this, rs->getUInt(1)));
    }
    rs->close();
    stmt->close();    
    
    op_queue_.initialize(SINGLETON(config).db_op_thread_count, SINGLETON(config).db_op_thread_count);
    op_queue_.set_thread_exit_callback(boost::bind<void>(&cache_service::before_thread_exit, &SINGLETON(cache_service)));
}
