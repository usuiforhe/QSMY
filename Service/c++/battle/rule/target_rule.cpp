//
//  target_rule.cpp
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "target_rule.h"

using namespace battle;

/**
 ------------------
 |2 |1 |    |1 |2 |
 ------------------
 |4 |3 |    |3 |4 |
 ------------------
 |6 |5 |    |5 |6 |
 ------------------
 |8 |7 |    |7 |8 |
 ------------------
 |10|9 |    |9 |10|
 ------------------
 |12|11|    |11|12|
 ------------------
 |14|13|    |13|14|
 ------------------
 
 位置编号如上图
 攻击目标规则为
 1. 优先攻击排数离自己最近的单位
 2. 偶数位置单位受其前排相邻三排以内的单位保护
 3. 不允许攻击受保护单位
 
 以下攻击优先级由该规则生成，事先生成可节省寻找目标的性能损耗
*/
target_rule::target_rule(){
    for(uint8_t i = 1; i <= 14; ++i){
        std::list<uint8_t> * target_list = new std::list<uint8_t>();
        std::list<uint8_t> * protectee_list = new std::list<uint8_t>();
        std::list<uint8_t> * protector_list = new std::list<uint8_t>();
        
        uint8_t behind_pos = i;
        if((i&0x01) == 1){            
            behind_pos += 1;
            if(i > 1)
            {
                protectee_list->push_back(i - 1);
            }
            
            protectee_list->push_back(i + 1);
            
            if(i < 12){
                protectee_list->push_back(i + 3);
            }
            protectee_list_map_.insert(std::make_pair(i, protectee_list));
        }
        else{
            if(i > 3)
            {
                protector_list->push_back(i - 3);
            }
            
            protector_list->push_back(i - 1);
            
            if(i < 14)
            {
                protector_list->push_back(i + 1);
            }
            protector_list_map_.insert(std::make_pair(i, protector_list));
        }
        
        for(uint8_t j = 0; j < 14; j += 2){
            if(j == 0){
                target_list->push_back(behind_pos - 1);
                target_list->push_back(behind_pos);
            }
            else{
                if(behind_pos > j){
                    target_list->push_back(behind_pos - j - 1);
                    target_list->push_back(behind_pos - j);
                }
                if(behind_pos + j < 15){
                    target_list->push_back(behind_pos + j - 1);
                    target_list->push_back(behind_pos +  j);
                }
            }
        }
        target_list_map_.insert(std::make_pair(i, target_list));
    }
}

target_rule::~target_rule(){
    for(BOOST_AUTO(pos, target_list_map_.begin()); pos != target_list_map_.end(); ++pos){
        delete pos->second;
    }
    for(BOOST_AUTO(pos, protectee_list_map_.begin()); pos != protectee_list_map_.end(); ++pos){
        delete pos->second;
    }
    for(BOOST_AUTO(pos, protector_list_map_.begin()); pos != protector_list_map_.end(); ++pos){
        delete pos->second;
    }
}

std::list<uint8_t> * target_rule::get_target_list(uint8_t position){
    BOOST_AUTO(pos,target_list_map_.find(position));
    if(pos != target_list_map_.end())
    {
        return pos->second;
    }
    return NULL;
}

std::list<uint8_t> * target_rule::get_protectee_list(uint8_t position){
    if(position % 2 == 0){
        return NULL;
    }
    BOOST_AUTO(pos,protectee_list_map_.find(position));
    if(pos != protectee_list_map_.end())
    {
        return pos->second;
    }
    return NULL;
}

std::list<uint8_t> * target_rule::get_protector_list(uint8_t position){
    if(position % 2 == 1){
        return NULL;
    }
    BOOST_AUTO(pos,protector_list_map_.find(position));
    if(pos != protector_list_map_.end())
    {
        return pos->second;
    }
    return NULL;
}
