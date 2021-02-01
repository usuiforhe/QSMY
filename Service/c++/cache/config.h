//
//  config.h
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__config__
#define __cache__config__

#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/prepared_statement.h"
#include "defines.h"

namespace latte
{
    class db_info;
}

namespace cache{
    struct config{
        config();
        
        ~config(){
            
        }
        
        size_t cache_capacity;
        uint16_t port;
        uint16_t http_port;
        uint8_t io_thread_count;
        uint8_t handle_thread_count;
        uint8_t db_op_thread_count;
        std::string mysql_server;
        std::string mysql_db;
        std::string mysql_user;
        std::string mysql_pwd;
        uint16_t mysql_conn_pool_max;
        uint32_t write_delay;
        
        boost::shared_ptr<latte::db_info> db_info;
    };

#define PREPARE_SQL(conn, stmt, str_sql) \
    sql::PreparedStatement * raw_##stmt = conn->prepareStatement(str_sql);    \
    std::auto_ptr<sql::PreparedStatement> stmt(raw_##stmt);
    
#define CREATE_STMT(conn, stmt) \
sql::Statement * raw_##stmt = conn->createStatement();    \
std::auto_ptr<sql::Statement> stmt(raw_##stmt);
    
}

#endif /* defined(__cache__config__) */
