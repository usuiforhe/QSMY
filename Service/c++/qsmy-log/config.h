//
//  config.h
//  qsmy-log
//
//  Created by 汪燕军 on 13-5-21.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __qsmy_log__config__
#define __qsmy_log__config__

#include "log_prefix.h"
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/prepared_statement.h"

namespace qsmy_log{
    struct config{
        config();
        
        ~config(){
            
        }
        
        uint16_t port;
        uint8_t handle_thread_count;
        std::string mysql_server;
        std::string mysql_db;
        std::string mysql_user;
        std::string mysql_pwd;
        uint8_t archive_hour;
        uint8_t archive_minute;
        uint32_t recent_seconds;
        
        std::auto_ptr<sql::Connection> connect_db(bool is_persist);
    };
    
#define CONNECT_DB(conn) sql::Connection * raw_##conn = sql::mysql::get_mysql_driver_instance()->connect(SINGLETON(config).mysql_server, SINGLETON(config).mysql_user, SINGLETON(config).mysql_pwd);\
std::auto_ptr<sql::Connection> conn(raw_##conn);\
conn->setSchema(SINGLETON(config).mysql_db);\

#define PREPARE_SQL(conn, stmt, str_sql) \
sql::PreparedStatement * raw_##stmt = conn->prepareStatement(str_sql);    \
std::auto_ptr<sql::PreparedStatement> stmt(raw_##stmt);

}

#endif /* defined(__qsmy_log__config__) */
