//
//  config.cpp
//  qsmy-log
//
//  Created by 汪燕军 on 13-5-21.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include <fstream>
#include "config.h"

using namespace qsmy_log;

config::config(){
    Json::Value root;
    Json::Reader reader;
    
    std::ifstream file("config.json");
    BOOST_ASSERT(reader.parse(file, root));
    
    port = static_cast<uint16_t>(root["port"].asUInt());
    handle_thread_count = static_cast<uint8_t>(root["handle_thread_count"].asUInt());
    mysql_server = root["mysql_server"].asString();
    mysql_db = root["mysql_db"].asString();
    mysql_user = root["mysql_user"].asString();
    mysql_pwd = root["mysql_password"].asString();
    std::vector<uint16_t> vec;
    latte::split(vec, root["archive_time"].asString(), ":");
    archive_hour = static_cast<uint8_t>(vec[0]);
    archive_minute = static_cast<uint8_t>(vec[1]);
    recent_seconds = 86400 * static_cast<uint8_t>(root["recent_days"].asUInt());
}

std::auto_ptr<sql::Connection> config::connect_db(bool is_persist)
{    
    sql::ConnectOptionsMap connection_properties;
    connection_properties["hostName"] = sql::SQLString(mysql_server);
    connection_properties["userName"] = sql::SQLString(mysql_user);
    connection_properties["password"] = sql::SQLString(mysql_pwd);
    connection_properties["schema"] = sql::SQLString(mysql_db);
    bool reconnect = is_persist;
    connection_properties["OPT_RECONNECT"] = &reconnect;
    
    std::auto_ptr<sql::Connection> connection(sql::mysql::get_mysql_driver_instance()->connect(connection_properties));
    
    return connection;
}
