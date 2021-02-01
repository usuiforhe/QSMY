//
//  config.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "cache_prefix.h"
#include "config.h"
#include <fstream>
using namespace cache;

config::config(){
    Json::Value root;
    Json::Reader reader;
    
    std::ifstream file("config.json");
    BOOST_ASSERT(reader.parse(file, root));
    
    port = static_cast<uint16_t>(root["port"].asUInt());
    http_port = static_cast<uint16_t>(root["http_port"].asUInt());
    cache_capacity = static_cast<size_t>(root["cache_capacity"].asUInt());
    io_thread_count = static_cast<uint16_t>(root["io_thread_count"].asUInt());
    handle_thread_count = static_cast<uint8_t>(root["handle_thread_count"].asUInt());
    db_op_thread_count = static_cast<uint8_t>(root["db_op_thread_count"].asUInt());
    mysql_server = root["mysql_server"].asString();
    mysql_db = root["mysql_db"].asString();
    mysql_user = root["mysql_user"].asString();
    mysql_pwd = root["mysql_password"].asString();
    mysql_conn_pool_max = static_cast<uint16_t>(root["mysql_conn_pool_max"].asUInt());
    write_delay = root["write_delay"].asUInt();
    
    db_info = boost::shared_ptr<latte::db_info>(new latte::db_info(mysql_server, mysql_db, mysql_user, mysql_pwd));
}
