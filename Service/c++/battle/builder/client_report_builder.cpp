//
//  client_report_builder.cpp
//  battle
//
//  Created by wangyanjun on 13-3-28.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "client_report_builder.h"
#include "util.h"
#include "config.h"

using namespace battle;

std::string client_report_builder::build(battle::scene *scene, battle::report *report){
    std::vector<char> bin;
    bin.reserve(1000);
    build_scene(scene, bin);
    force * left = scene->get_left_force();
    force * right = scene->get_right_force();
    if(left->get_is_winner()){
        bin.push_back(left->get_id());
    }
    else{
        bin.push_back(right->get_id());
    }
    
    bin.push_back(2);
    build_force(left, bin);
    build_force(right, bin);
    
    build_behaviors(report, bin);
    
    std::string result;
    result.append(bin.data(), bin.size());
#if DEBUG
//    s_log.out_debug("battle report is:\n" + result);
    s_log.out_debug("json report is:\n" + Json::FastWriter().write(*report->to_json()));
#endif
    return result;
}

void client_report_builder::build_scene(battle::scene *scene, std::vector<char> &bin){
    //关卡编号
    uint32_t sid = scene->get_sid();
    bin.push_back(static_cast<char>(sid & 0x000000ff));
    bin.push_back(static_cast<char>((sid & 0x0000ff00) >> 8));
    bin.push_back(static_cast<char>((sid & 0x00ff0000) >> 16));
    bin.push_back(static_cast<char>((sid & 0xff000000) >> 24));
    
    //地图编号
    uint32_t map_id = scene->get_map_id();
    bin.push_back(static_cast<char>(map_id & 0x000000ff));
    bin.push_back(static_cast<char>((map_id & 0x0000ff00) >> 8));
    bin.push_back(static_cast<char>((map_id & 0x00ff0000) >> 16));
    bin.push_back(static_cast<char>((map_id & 0xff000000) >> 24));
    
    //评分
    bin.push_back(scene->get_report()->grade_);
}

void client_report_builder::build_force(battle::force * force, std::vector<char> &bin){
    //势力编号
    bin.push_back(force->get_id());

    const std::string & name = force->get_name();
    //势力名长度
    bin.push_back(static_cast<char>(name.size()));
    //势力名
    for(size_t i = 0; i < name.size(); ++i){
        bin.push_back(name[i]);
    }
    
    //速度
    uint32_t speed = force->get_total_speed();
    bin.push_back(static_cast<char>(speed & 0x000000ff));
    bin.push_back(static_cast<char>((speed & 0x0000ff00) >> 8));
    bin.push_back(static_cast<char>((speed & 0x00ff0000) >> 16));
    bin.push_back(static_cast<char>((speed & 0xff000000) >> 24));
    
    battle::user * user = force->get_users().front();

    //玩家编号
    uint32_t user_id = user->get_user_id();
    bin.push_back(static_cast<char>(user_id & 0x000000ff));
    bin.push_back(static_cast<char>((user_id & 0x0000ff00) >> 8));
    bin.push_back(static_cast<char>((user_id & 0x00ff0000) >> 16));
    bin.push_back(static_cast<char>((user_id & 0xff000000) >> 24));
    
    //是否怪物
    bin.push_back(user->get_is_mob() ? 0x01 : 0x00);
    
    //玩家等级
    uint16_t level = user->get_level();
    bin.push_back(static_cast<char>(level & 0x000000ff));
    bin.push_back(static_cast<char>((level & 0x0000ff00) >> 8));
    
    const force::unit_map_t & unit_map = force->get_unit_map();

    //单位数量
    bin.push_back(static_cast<char>(unit_map.size() & 0x000000ff));

    bool enable_unit_hide(S_CONFIG.get_enable_unit_hide());
    
    //单位列表
    BOOST_FOREACH(force::unit_map_t::value_type value, unit_map){
        //单位编号
        bin.push_back(static_cast<char>(value.second->get_id()));
        
        //单位静态编号
        uint32_t unit_sid = value.second->get_sid();
        bin.push_back(static_cast<char>(unit_sid & 0x000000ff));
        bin.push_back(static_cast<char>((unit_sid & 0x0000ff00) >> 8));
        bin.push_back(static_cast<char>((unit_sid & 0x00ff0000) >> 16));
        bin.push_back(static_cast<char>((unit_sid & 0xff000000) >> 24));
        
        //单位等级
        uint16_t unit_level = value.second->level;
        bin.push_back(static_cast<char>(unit_level & 0x000000ff));
        bin.push_back(static_cast<char>((unit_level & 0x0000ff00) >> 8));
        
        //单位最大生命值
        int32_t max_hp = value.second->max_hp;
        bin.push_back(static_cast<char>(max_hp & 0x000000ff));
        bin.push_back(static_cast<char>((max_hp & 0x0000ff00) >> 8));
        bin.push_back(static_cast<char>((max_hp & 0x00ff0000) >> 16));
        bin.push_back(static_cast<char>((max_hp & 0xff000000) >> 24));
        
        //单位初始生命值
        int32_t hp = value.second->init_hp;
        bin.push_back(static_cast<char>(hp & 0x000000ff));
        bin.push_back(static_cast<char>((hp & 0x0000ff00) >> 8));
        bin.push_back(static_cast<char>((hp & 0x00ff0000) >> 16));
        bin.push_back(static_cast<char>((hp & 0xff000000) >> 24));
        
        //单位站位
        bin.push_back(static_cast<char>(value.second->get_position_id()));
        
        //单位占位数量
        bin.push_back(static_cast<char>((value.second->size) & 0x000000ff));
        
        //战斗力系数
        uint32_t fight_factor = static_cast<uint32_t>(value.second->get_fight_factor() * 10000);
        bin.push_back(static_cast<char>(fight_factor & 0x000000ff));
        bin.push_back(static_cast<char>((fight_factor & 0x0000ff00) >> 8));
        bin.push_back(static_cast<char>((fight_factor & 0x00ff0000) >> 16));
        bin.push_back(static_cast<char>((fight_factor & 0xff000000) >> 24));
        //是否boss
        bin.push_back(static_cast<char>(value.second->is_boss ? 0x01 : 0x00));
        
        uint32_t dress_id = value.second->dress_id;
        bin.push_back(static_cast<char>(dress_id & 0x000000ff));
        bin.push_back(static_cast<char>((dress_id & 0x0000ff00) >> 8));
        bin.push_back(static_cast<char>((dress_id & 0x00ff0000) >> 16));
        bin.push_back(static_cast<char>((dress_id & 0xff000000) >> 24));
        
        if(enable_unit_hide)
        {
            //是否显示
            bin.push_back(static_cast<char>(value.second->is_show ? 0x01 : 0x00));
        }
    }
}

void client_report_builder::build_behaviors(battle::report *report, std::vector<char> &bin){
    //行动数量
    uint16_t behavior_count = report->behaviors_.size();
    bin.push_back(static_cast<char>(behavior_count & 0x000000ff));
    bin.push_back(static_cast<char>((behavior_count & 0x0000ff00) >> 8));
    //行动列表
    BOOST_FOREACH(base_behavior * val, report->behaviors_){
        val->append_to_bin(bin);
    }
}
