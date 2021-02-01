//
//  cup_match.cpp
//  battle
//
//  Created by 汪燕军 on 13-4-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "cup_match.h"
#include "builder/scene_builder.h"
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/prepared_statement.h"
#include "config.h"
#include "builder/client_report_builder.h"
#include "cup_player.h"

using namespace battle;

cup_match::cup_match(uint16_t final_num, uint16_t match_id, boost::shared_ptr<cup_player>  left, boost::shared_ptr<cup_player> right)
    :final_num_(final_num),
    match_id_(match_id),
    left_(left),
    right_(right),
    winner_(),
    scene_(NULL)
{
}

cup_match::~cup_match(){
    if(scene_ != NULL)
    {
        delete scene_;
    }
}

void cup_match::match(){
    scene_meta_data scene_meta;
    scene_meta.set_sid(S_CONFIG.get_cup_scene_id());
    scene_meta.set_map_id(0);
    scene_meta.set_type(SCENE_TYPE_CUP);
    scene_meta.set_allocated_left_force(&(left_->get_force()));
    scene_meta.set_allocated_right_force(&(right_->get_force()));
    scene_ = S_SCENE_BUILDER.build(scene_meta);
    if(scene_ != NULL){
        scene_meta.release_left_force();
        scene_meta.release_right_force();
        scene_->execute();
        if(scene_->get_left_force()->get_is_winner()){
            winner_ = left_;
            loser_ = right_;
        }
        else{
            winner_ = right_;
            loser_ = left_;
        }
        winner_->match_over(this);
        loser_->match_over(this);
        save();
        delete scene_;
        scene_ = NULL;
    }
    else{
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "cup_match::match is not valid.";
        s_log.out_error(ss.str());
    }
}

void cup_match::save(){
    report * report = scene_->get_report();
    std::string str = S_CLIENT_REPORT_BUILDER.build(scene_, report);
    std::stringstream & ss = latte::fast_stringstream::ss();
    ss << str;
    try
    {
        sql::Connection * raw_conn = sql::mysql::get_mysql_driver_instance()->connect(S_CONFIG.get_cup_mysql_server(), S_CONFIG.get_cup_mysql_user(), S_CONFIG.get_cup_mysql_pwd());
        std::auto_ptr<sql::Connection > connection(raw_conn);
        
        connection->setSchema(S_CONFIG.get_cup_mysql_db());
        sql::PreparedStatement * raw_stmt = connection->prepareStatement("call P_CUP_MATCH_SAVE(?,?,?,?,?,?,?,?,?,?,?,?,?,?);");
        std::auto_ptr<sql::PreparedStatement> stmt(raw_stmt);
        uint32_t paramIndex = 0;
        
        stmt->setUInt(++paramIndex, final_num_);
        stmt->setUInt(++paramIndex, match_id_);
        bool user1_win = left_ == winner_;
        //是否逆转
        bool is_reversal(report->is_reversal());
        //是否险胜
        bool is_nose_out(report->is_nose_out());
        
        stmt->setBoolean(++paramIndex, user1_win);
        stmt->setUInt(++paramIndex, left_->get_user_id());
        stmt->setString(++paramIndex, left_->get_disp());
        stmt->setString(++paramIndex, left_->get_disp_detail());
        stmt->setUInt(++paramIndex, right_->get_user_id());
        stmt->setString(++paramIndex, right_->get_disp());
        stmt->setString(++paramIndex, right_->get_disp_detail());
        stmt->setBlob(++paramIndex, &ss);
        stmt->setUInt(++paramIndex, winner_->get_win_count());
        stmt->setUInt(++paramIndex, winner_->get_best_grade());
        stmt->setBoolean(++paramIndex, is_reversal);
        stmt->setBoolean(++paramIndex, is_nose_out);
        stmt->execute();
        stmt->close();
        connection->close();
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "cup_match::save throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "cup_match::save throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());
    }
}
