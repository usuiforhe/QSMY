//
//  report_converter.cpp
//  battle
//
//  Created by 汪燕军 on 13-10-12.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "report_converter.h"
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/prepared_statement.h"
#include "config.h"

using namespace battle;
using namespace battle_report;

void report_converter::convert_cup()
{
    std::vector<boost::shared_ptr<match_info> > list;
    load_cup_matches(list);
    
    BOOST_FOREACH(boost::shared_ptr<match_info> match, list)
    {
        convert_one(match);
        update_cup_match_report(match);
    }
}

void report_converter::load_cup_matches(std::vector<boost::shared_ptr<match_info> > &list)
{
    sql::Connection * raw_conn = sql::mysql::get_mysql_driver_instance()->connect(S_CONFIG.get_cup_mysql_server(), S_CONFIG.get_cup_mysql_user(), S_CONFIG.get_cup_mysql_pwd());
    std::auto_ptr<sql::Connection > connection(raw_conn);
    
    connection->setSchema(S_CONFIG.get_cup_mysql_db());
    sql::PreparedStatement * raw_stmt =
    connection->prepareStatement(
                                 "SELECT final_num, match_id, report FROM T_CUP_MATCH;"
                                 );
    std::auto_ptr<sql::PreparedStatement> stmt(raw_stmt);
    sql::ResultSet * raw_rs = stmt->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    while(rs->next()){
        boost::shared_ptr<match_info> match(new match_info);
        match->final_num = rs->getUInt(1);
        match->match_id = rs->getUInt(2);
        std::auto_ptr<std::istream> stream(rs->getBlob(3));
        
        while(!stream->eof())            
        {
            match->report->push_back(stream->get());
        }
        list.push_back(match);
    }
    rs->close();
    stmt->close();
    connection->close();
}

void report_converter::update_cup_match_report(boost::shared_ptr<match_info> match)
{
    std::stringstream & ss = latte::fast_stringstream::ss();
    
    BOOST_FOREACH(char c, *(match->new_report))
    {
        ss << c;
    }
    
    try
    {
        sql::Connection * raw_conn = sql::mysql::get_mysql_driver_instance()->connect(S_CONFIG.get_cup_mysql_server(), S_CONFIG.get_cup_mysql_user(), S_CONFIG.get_cup_mysql_pwd());
        std::auto_ptr<sql::Connection > connection(raw_conn);
        
        connection->setSchema(S_CONFIG.get_cup_mysql_db());
        sql::PreparedStatement * raw_stmt =
        connection->prepareStatement(
                                     "UPDATE T_CUP_MATCH SET report = ? WHERE final_num = ? AND match_id = ?;"
                                     );
        std::auto_ptr<sql::PreparedStatement> stmt(raw_stmt);
        uint32_t paramIndex = 0;
        
        stmt->setBlob(++paramIndex, &ss);
        stmt->setUInt(++paramIndex, match->final_num);
        stmt->setUInt(++paramIndex, match->match_id);
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

void report_converter::convert_one(boost::shared_ptr<match_info> match)
{
    std::vector<char>::iterator iter = match->report->begin();
    match->new_report->push_back(*iter);
    uint32_t scene_id = static_cast<uint32_t>(*iter++) &0x000000ff;
    match->new_report->push_back(*iter);
    scene_id |= (static_cast<uint32_t>(*iter++) << 8) & 0x0000ff00;
    match->new_report->push_back(*iter);
    scene_id |= (static_cast<uint32_t>(*iter++) << 16) & 0x00ff0000;
    match->new_report->push_back(*iter);
    scene_id |= (static_cast<uint32_t>(*iter++) << 24) & 0xff000000;
    match->new_report->push_back(*iter);
    uint32_t map_id = static_cast<uint32_t>(*iter++) &0x000000ff;
    match->new_report->push_back(*iter);
    map_id |= (static_cast<uint32_t>(*iter++) << 8) & 0x0000ff00;
    match->new_report->push_back(*iter);
    map_id |= (static_cast<uint32_t>(*iter++) << 16) & 0x00ff0000;
    match->new_report->push_back(*iter);
    map_id |= (static_cast<uint32_t>(*iter++) << 24) & 0xff000000;
    match->new_report->push_back(*iter);
    uint8_t grade = static_cast<uint8_t>(*iter++);
    match->new_report->push_back(*iter);
    uint8_t winner = static_cast<uint8_t>(*iter++);
    
    parse_force(match, iter);
    parse_action(match, iter);
}

void report_converter::parse_force(boost::shared_ptr<match_info> match, std::vector<char>::iterator & iter){
    
    match->new_report->push_back(*iter);
    uint8_t force_num = static_cast<uint8_t>(*iter++);
    for(uint8_t i = 0; i < force_num; ++i){
        match->new_report->push_back(*iter);
        uint8_t force_id = static_cast<uint8_t>(*iter++);
        match->new_report->push_back(*iter);
        uint8_t name_size = static_cast<uint8_t>(*iter++);
        
        std::string name = "";
        if(name_size > 0)
        {
            name.append(iter, iter + name_size);
            iter += name_size;
            
            for(std::string::iterator n_pos = name.begin(); n_pos != name.end(); ++n_pos)
            {
                match->new_report->push_back(*n_pos);
            }
        }
        
        match->new_report->push_back(*iter);
        uint32_t speed = static_cast<uint32_t>(*iter++) &0x000000ff;
        match->new_report->push_back(*iter);
        speed |= (static_cast<uint32_t>(*iter++) << 8) & 0x0000ff00;
        match->new_report->push_back(*iter);
        speed |= (static_cast<uint32_t>(*iter++) << 16) & 0x00ff0000;
        match->new_report->push_back(*iter);
        speed |= (static_cast<uint32_t>(*iter++) << 24) & 0xff000000;
        
        match->new_report->push_back(*iter);
        uint32_t user_id = static_cast<uint32_t>(*iter++) &0x000000ff;
        match->new_report->push_back(*iter);
        user_id |= (static_cast<uint32_t>(*iter++) << 8) & 0x0000ff00;
        match->new_report->push_back(*iter);
        user_id |= (static_cast<uint32_t>(*iter++) << 16) & 0x00ff0000;
        match->new_report->push_back(*iter);
        user_id |= (static_cast<uint32_t>(*iter++) << 24) & 0xff000000;
        
        match->new_report->push_back(*iter);
        bool is_mob = *iter++ == 0x01;
        
        match->new_report->push_back(*iter);
        uint16_t level = static_cast<uint16_t>(*iter++) &0x00ff;
        match->new_report->push_back(*iter);
        level |= (static_cast<uint16_t>(*iter++) << 8) & 0xff00;
        
        parse_unit(match, iter);
    }
}

void report_converter::parse_unit(boost::shared_ptr<match_info> match, std::vector<char>::iterator &iter)
{
    match->new_report->push_back(*iter);
    uint8_t unit_num = static_cast<uint8_t>(*iter++);
    for(uint8_t i = 0; i < unit_num; ++i){
        match->new_report->push_back(*iter);
        uint8_t id = static_cast<uint8_t>(*iter++);
        match->new_report->push_back(*iter);
        uint32_t sid = static_cast<uint32_t>(*iter++) &0x000000ff;
        match->new_report->push_back(*iter);
        sid |= (static_cast<uint32_t>(*iter++) << 8) & 0x0000ff00;
        match->new_report->push_back(*iter);
        sid |= (static_cast<uint32_t>(*iter++) << 16) & 0x00ff0000;
        match->new_report->push_back(*iter);
        sid |= (static_cast<uint32_t>(*iter++) << 24) & 0xff000000;
        match->new_report->push_back(*iter);
        uint16_t level = static_cast<uint16_t>(*iter++) &0x00ff;
        match->new_report->push_back(*iter);
        level |= (static_cast<uint16_t>(*iter++) << 8) & 0xff00;
        match->new_report->push_back(*iter);
        uint32_t max_hp = static_cast<uint32_t>(*iter++) &0x000000ff;
        match->new_report->push_back(*iter);
        max_hp |= (static_cast<uint32_t>(*iter++) << 8) & 0x0000ff00;
        match->new_report->push_back(*iter);
        max_hp |= (static_cast<uint32_t>(*iter++) << 16) & 0x00ff0000;
        match->new_report->push_back(*iter);
        max_hp |= (static_cast<uint32_t>(*iter++) << 24) & 0xff000000;
        match->new_report->push_back(*iter);
        uint32_t hp = static_cast<uint32_t>(*iter++) &0x000000ff;
        match->new_report->push_back(*iter);
        hp |= (static_cast<uint32_t>(*iter++) << 8) & 0x0000ff00;
        match->new_report->push_back(*iter);
        hp |= (static_cast<uint32_t>(*iter++) << 16) & 0x00ff0000;
        match->new_report->push_back(*iter);
        hp |= (static_cast<uint32_t>(*iter++) << 24) & 0xff000000;
        match->new_report->push_back(*iter);
        uint8_t position = static_cast<uint8_t>(*iter++);
        match->new_report->push_back(*iter);
        uint8_t size = static_cast<uint8_t>(*iter++);
        
        match->new_report->push_back(*iter);
        uint32_t fight_factor = static_cast<uint32_t>(*iter++) &0x000000ff;
        match->new_report->push_back(*iter);
        fight_factor |= (static_cast<uint32_t>(*iter++) << 8) & 0x0000ff00;
        match->new_report->push_back(*iter);
        fight_factor |= (static_cast<uint32_t>(*iter++) << 16) & 0x00ff0000;
        match->new_report->push_back(*iter);
        fight_factor |= (static_cast<uint32_t>(*iter++) << 24) & 0xff000000;
        
        float out_ff = static_cast<float>(fight_factor) / 10000;
        match->new_report->push_back(*iter);
        bool is_boss = *iter++ == 0x01;
    }
}

void report_converter::parse_action(boost::shared_ptr<match_info> match, std::vector<char>::iterator &iter)
{
    match->new_report->push_back(*iter);
    uint16_t action_num = static_cast<uint16_t>(*iter++) &0x00ff;
    match->new_report->push_back(*iter);
    action_num |= (static_cast<uint16_t>(*iter++) << 8) & 0xff00;
    for(uint16_t i = 0; i < action_num; ++i){
        match->new_report->push_back(*iter);
        uint8_t type = static_cast<uint8_t>(*iter++);
        match->new_report->push_back(*iter);
        uint8_t actor = static_cast<uint8_t>(*iter++);
        switch (type) {
            case 1:
            {
                break;
            }
            case 2:
            {
                match->new_report->push_back(*iter);
                uint32_t skill_id = static_cast<uint32_t>(*iter++) & 0x000000ff;
                match->new_report->push_back(*iter);
                skill_id |= static_cast<uint32_t>(*iter++) << 8 & 0x0000ff00;
                match->new_report->push_back(*iter);
                skill_id |= static_cast<uint32_t>(*iter++) << 16 & 0x00ff0000;
                match->new_report->push_back(*iter);
                skill_id |= static_cast<uint32_t>(*iter++) << 24 & 0xff000000;
                match->new_report->push_back(*iter);
                uint8_t position = static_cast<uint8_t>(*iter++);
                uint8_t skillPosition = position & 0x3f;
                uint8_t target = position >> 6;
                parse_coactors(match, iter);
                break;
            }
            case 3:
            {
                match->new_report->push_back(*iter);
                uint32_t buff_id = static_cast<uint32_t>(*iter++) & 0x000000ff;
                match->new_report->push_back(*iter);
                buff_id |= static_cast<uint32_t>(*iter++) << 8 & 0x0000ff00;
                match->new_report->push_back(*iter);
                buff_id |= static_cast<uint32_t>(*iter++) << 16 & 0x00ff0000;
                match->new_report->push_back(*iter);
                buff_id |= static_cast<uint32_t>(*iter++) << 24 & 0xff000000;
                break;
            }
            default:
            {
                break;
            }
        }
        parse_result(match, iter);
    }
}


void report_converter::parse_result(boost::shared_ptr<match_info> match, std::vector<char>::iterator &iter)
{
    match->new_report->push_back(*iter);
    uint8_t res_num = static_cast<uint8_t>(*iter++);
    for(uint8_t i = 0; i < res_num; ++i){
        match->new_report->push_back(*iter);
        uint8_t type = static_cast<uint8_t>(*iter++);
        match->new_report->push_back(*iter);
        uint8_t actor = static_cast<uint8_t>(*iter++);
        switch (type) {
            case 1:
            {
                match->new_report->push_back(*iter);
                uint32_t damage = static_cast<uint32_t>(*iter++) & 0x000000ff;
                match->new_report->push_back(*iter);
                damage |= static_cast<uint32_t>(*iter++) << 8 & 0x0000ff00;
                match->new_report->push_back(*iter);
                damage |= static_cast<uint32_t>(*iter++) << 16 & 0x00ff0000;
                match->new_report->push_back(*iter);
                damage |= static_cast<uint32_t>(*iter++) << 24 & 0xff000000;
                match->new_report->push_back(*iter);
                char special = *iter++;
                bool is_critical = (0x01 & special) == 0x01;
                bool is_dodge = (0x02 & special) == 0x02;
                bool is_block = (0x04 & special) == 0x04;
                break;
            }
            case 2:
            {
                match->new_report->push_back(*iter);
                uint32_t buff_id = static_cast<uint32_t>(*iter++) & 0x000000ff;
                match->new_report->push_back(*iter);
                buff_id |= static_cast<uint32_t>(*iter++) << 8 & 0x0000ff00;
                match->new_report->push_back(*iter);
                buff_id |= static_cast<uint32_t>(*iter++) << 16 & 0x00ff0000;
                match->new_report->push_back(*iter);
                buff_id |= static_cast<uint32_t>(*iter++) << 24 & 0xff000000;
                break;
            }
            case 3:
            {
                match->new_report->push_back(*iter);
                uint32_t buff_id = static_cast<uint32_t>(*iter++) & 0x000000ff;
                match->new_report->push_back(*iter);
                buff_id |= static_cast<uint32_t>(*iter++) << 8 & 0x0000ff00;
                match->new_report->push_back(*iter);
                buff_id |= static_cast<uint32_t>(*iter++) << 16 & 0x00ff0000;
                match->new_report->push_back(*iter);
                buff_id |= static_cast<uint32_t>(*iter++) << 24 & 0xff000000;
                break;
            }
            case 4:
            {
                match->new_report->push_back(*iter);
                uint32_t heal = static_cast<uint32_t>(*iter++) & 0x000000ff;
                match->new_report->push_back(*iter);
                heal |= static_cast<uint32_t>(*iter++) << 8 & 0x0000ff00;
                match->new_report->push_back(*iter);
                heal |= static_cast<uint32_t>(*iter++) << 16 & 0x00ff0000;
                match->new_report->push_back(*iter);
                heal |= static_cast<uint32_t>(*iter++) << 24 & 0xff000000;
                match->new_report->push_back(*iter);
                bool is_critical = static_cast<uint8_t>(*iter++) == 0x01;
                break;
            }
            default:
            {
            }
        }
    }
}

void report_converter::parse_coactors(boost::shared_ptr<match_info> match, std::vector<char>::iterator & iter){
    match->new_report->push_back(*iter);
    uint8_t coactor_num = static_cast<uint8_t>(*iter++);
    for(uint8_t i = 0; i < coactor_num; ++ i){
        match->new_report->push_back(*iter);
        uint8_t id = static_cast<uint8_t>(*iter++);
    }
}

