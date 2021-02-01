//
//  config.h
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__config__
#define __battle__config__

#include "battle_prefix.h"
#include "singleton.h"
namespace battle {
    class scene_meta_data;
    class force_meta_data;
    class config{
    public:
        config();
        ~config();
        
        inline GETTER(uint16_t, io_thread_count);
        inline GETTER(uint16_t, port);
        inline GETTER(uint16_t, http_port);
        inline GETTER(std::string,script_path);
        inline GETTER(std::string,lua_extension);
        inline GETTER(bool, is_debug);
        inline GETTER(uint8_t, battle_thread_count);
        inline GETTER(bool, enable_cup);
        inline GETTER(bool, enable_unit_hide);
        inline GETTER(uint8_t, min_rounds);
        inline GETTER(uint16_t, max_players);
        inline GETTER(uint8_t, cup_run_hour);
        inline GETTER(uint8_t, cup_run_minute);
        inline GETTER(uint32_t, cup_scene_id);
        inline GETTER(const std::string &, cup_mysql_server);
        inline GETTER(const std::string &, cup_mysql_db);
        inline GETTER(const std::string &, cup_mysql_user);
        inline GETTER(const std::string &, cup_mysql_pwd);
        
    private:
        uint16_t io_thread_count_;
        uint16_t port_;
        uint16_t http_port_;
        std::string script_path_;
        std::string lua_extension_;
        bool is_debug_;
        uint8_t battle_thread_count_;
        
        bool enable_cup_;
        bool enable_unit_hide_;
        
        uint8_t min_rounds_;
        uint16_t max_players_;
        uint8_t cup_run_hour_;
        uint8_t cup_run_minute_;
        uint32_t cup_scene_id_;
        std::string cup_mysql_server_;
        std::string cup_mysql_db_;
        std::string cup_mysql_user_;
        std::string cup_mysql_pwd_;
    };
}

#define S_CONFIG latte::singleton<config>::instance()

#endif /* defined(__battle__config__) */
