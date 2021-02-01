//
//  config.cpp
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "config.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include "meta/scene_meta_data.pb.h"
#include "log/log.h"

using namespace battle;

config::config(){
    Json::Value root;
    Json::Reader reader;
    
    std::ifstream file("config.json");
    BOOST_ASSERT(reader.parse(file, root));
    
    io_thread_count_ = static_cast<uint16_t>(root["io_thread_count"].asUInt());
    port_ = static_cast<uint16_t>(root["port"].asUInt());
    http_port_ = static_cast<uint16_t>(root["http_port"].asUInt());
    script_path_ = root["script_path"].asString();
    lua_extension_ = root["lua_extension"].asString();
    is_debug_ = root["is_debug"].asBool();
    battle_thread_count_ = static_cast<uint8_t>(root["battle_thread_count"].asUInt());
    enable_cup_ = false;
    if(root.isMember("enable_unit_hide"))
    {
        enable_unit_hide_ = root["enable_unit_hide"].asBool();
    }
    else
    {
        enable_unit_hide_ = true;
    }
    if(root.isMember("cup")){
        Json::Value cup = root["cup"];
        enable_cup_ = cup["enable"].asBool();
        if(enable_cup_){
            min_rounds_ = static_cast<uint8_t>(cup["min_rounds"].asUInt());
            max_players_ = static_cast<uint16_t>(cup["max_players"].asUInt());
            std::string cup_run_time = cup["cup_run_time"].asString();
            std::vector<uint16_t> cup_run;
            latte::split(cup_run, cup_run_time, ":");
            cup_run_hour_ = static_cast<uint8_t>(cup_run[0]);
            cup_run_minute_ = static_cast<uint8_t>(cup_run[1]);
            cup_scene_id_ = cup["scene_id"].asUInt();
            cup_mysql_server_ = cup["mysql_server"].asString();
            cup_mysql_db_ = cup["mysql_db"].asString();
            cup_mysql_user_ = cup["mysql_user"].asString();
            cup_mysql_pwd_ = cup["mysql_password"].asString();
        }
    }
}

config::~config(){
}

