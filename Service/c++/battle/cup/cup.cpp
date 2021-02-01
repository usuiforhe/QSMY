//
//  cup.cpp
//  battle
//
//  Created by 汪燕军 on 13-4-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "cup.h"
#include "cup_match.h"
#include "config.h"
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include <boost/filesystem.hpp>
#include "builder/bot_builder.h"
#include "util/script_util.h"
#include <time.h>

using namespace battle;

cup::~cup(){
    if(lua_state_ != NULL){
        lua_close(lua_state_);
    }
}

void cup::init(){
    std::srand(static_cast<uint32_t>(date_));
    lua_state_ = lua_open();
    luabind::open(lua_state_);
    luaL_openlibs(lua_state_);
    luabind::set_pcall_callback(&battle::lua_call_error);
    load_bot();
    
    sql::Connection * raw_conn = sql::mysql::get_mysql_driver_instance()->connect(S_CONFIG.get_cup_mysql_server(), S_CONFIG.get_cup_mysql_user(), S_CONFIG.get_cup_mysql_pwd());
    std::auto_ptr<sql::Connection > connection(raw_conn);
    connection->setSchema(S_CONFIG.get_cup_mysql_db());
    
    //清除比赛历史
    sql::Statement * raw = connection->createStatement();
    std::auto_ptr<sql::Statement> stmt(raw);
    stmt->execute("TRUNCATE TABLE T_CUP_MATCH;");
    //清除竞猜历史
    stmt->execute("TRUNCATE TABLE T_USER_CUP_BET;");
    
    //加载所有报名玩家的信息
    sql::PreparedStatement * raw_prepared = connection->prepareStatement("SELECT user_id, win_count, best_grade, formation, disp, disp_detail FROM T_USER_CUP WHERE signup_date >= ? LIMIT 0, ?;");
    std::auto_ptr<sql::PreparedStatement> prepared_stmt(raw_prepared);
    prepared_stmt->setInt(1, static_cast<uint32_t>(date_));
    prepared_stmt->setInt(2, static_cast<uint32_t>(S_CONFIG.get_max_players()));
    std::auto_ptr<sql::ResultSet> result(prepared_stmt->executeQuery());
    while(result->next()){
        uint32_t user_id = result->getUInt(1);
        uint32_t win_count = result->getUInt(2);
        uint16_t best_grade = static_cast<uint16_t>(result->getUInt(3));
        
        std::auto_ptr<std::istream> stream(result->getBlob(4));        
        std::string disp = result->getString(5);
        std::string disp_detail = result->getString(6);
        
        cup_player * p_player = new cup_player(user_id, stream, disp, disp_detail, win_count, best_grade);
        boost::shared_ptr<cup_player> player(p_player);
        
        player_list_.push_back(player);
    };
    
    connection->close();
    
    std::random_shuffle(player_list_.begin(), player_list_.end());
    std::random_shuffle(name_list_.begin(), name_list_.end());
}

void cup::run(){
    ++cur_round_;
    uint16_t min_forces = pow(2, S_CONFIG.get_min_rounds());
    
    uint16_t final_num = min_forces;
    if(player_list_.size() > min_forces){
        uint8_t rounds = static_cast<uint8_t>(ceil(log2(player_list_.size())));
        final_num = pow(2, rounds);
    }
    
    //16强之前的比赛
    while (final_num > 8) {
        std::vector<boost::shared_ptr<cup_player> > player_list = player_list_;
        player_list_.clear();
        
        uint16_t next_final_num = final_num / 2;
        uint16_t match_id = 0;
        
        while(player_list.size() + player_list_.size() > next_final_num){
            boost::shared_ptr<cup_player>  left = player_list.front();
            player_list.erase(player_list.begin());
            boost::shared_ptr<cup_player>  right = player_list.front();
            player_list.erase(player_list.begin());
            
            match(final_num, ++match_id, left, right);
        }
        
        while(player_list.size() > 0){
            boost::shared_ptr<cup_player>  force = player_list.front();
            player_list.erase(player_list.begin());
            match(final_num, ++match_id, force);
        }
        
        final_num = next_final_num;
    }
    
    uint16_t need_bot_num = final_num - player_list_.size();
    player_list_.reserve(final_num);
    std::vector<boost::shared_ptr<cup_player> >::iterator iter = player_list_.begin();
    for(uint16_t i = 0; i < need_bot_num; ++i){
        if(iter != player_list_.end()) {
            ++iter;
        }
        boost::shared_ptr<cup_player> bot = get_bot();
        player_list_.insert(iter, bot);
    }
    
    //16强之后的比赛
    while (final_num >= 2) {
        std::vector<boost::shared_ptr<cup_player> > player_list = player_list_;
        player_list_.clear();
        uint16_t match_id = 0;
        
        while(player_list.size() > 0){
            boost::shared_ptr<cup_player>  left = player_list.front();
            player_list.erase(player_list.begin());
            boost::shared_ptr<cup_player>  right = player_list.front();
            player_list.erase(player_list.begin());
            
            match(final_num, ++match_id, left, right);
        }
        final_num /= 2;
    }
    char timebuf[255];
    struct tm * t = localtime(&date_);
    strftime(timebuf, 255, "run cup for:%Y-%m-%d %H:%M:%S", t);
    std::string output(timebuf);
    s_log.out_string(output);
}

void cup::match(uint16_t final_num, uint16_t match_id, boost::shared_ptr<cup_player>  user1, boost::shared_ptr<cup_player> user2){
    bool user2_null = !user2;
    
    if(user2_null){
        user2 = get_bot();
    }
    boost::shared_ptr<cup_player> left, right;
    if(std::rand() % 2 == 1){
        left = user1;
        right = user2;
    }
    else{
        left = user2;
        right = user1;
    }
    cup_match match(final_num, match_id, left, right);
    match.match();
    
    if(final_num <= 16 || !user2_null || match.get_winner() == user1){
        if(match.get_winner()){
            player_list_.push_back(match.get_winner());
        }
    }
}

void cup::load_bot(){
    name_list_.reserve(8500);
    S_BOT_BUILDER.load_bot_name(lua_state_, name_list_);
    bot_list_.reserve(10);
    
    boost::filesystem::directory_iterator end;
    for(boost::filesystem::directory_iterator pos(S_CONFIG.get_script_path() + "bot");pos != end; ++pos){
        const boost::filesystem::path & p(pos->path());
        
#if DEBUG
        s_log.out_debug("file:" + pos->path().string());
#endif
        if(!p.empty() && p.has_extension() && p.extension().string() == S_CONFIG.get_lua_extension()){
            uint32_t bot_id = boost::lexical_cast<uint32_t>(p.filename().replace_extension().string());
            boost::shared_ptr<force_meta_data> force(new force_meta_data(S_BOT_BUILDER.build(bot_id, lua_state_)));
            bot_list_.push_back(force);
        }
    }
}

boost::shared_ptr<cup_player> cup::get_bot(){
    boost::shared_ptr<bot_name> b_name = name_list_.front();
    if(name_list_.size() > 1){
        name_list_.erase(name_list_.begin());
    }
    
    boost::shared_ptr<force_meta_data> force = bot_list_.at(b_name->id % bot_list_.size());
    force->set_name(b_name->name);
    cup_player * player = new cup_player(*force);
    
    return boost::shared_ptr<cup_player>(player);
}
