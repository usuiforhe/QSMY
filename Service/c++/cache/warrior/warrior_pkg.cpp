//
//  warrior_pkg.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "warrior_pkg.h"
#include "player.h"
#include "player_manager.h"

using namespace cache;

void warrior_pkg::load(){
    SCOPED_CONNECT(scoped, conn);
    PREPARE_SQL(conn, stmt, "\
                SELECT `id`, `warrior_id`, `warrior_lv`, `warrior_exp`, `warrior_up`, `attack_add`, `defence_add`, `hp_add`, `potential`, `skill_exp`, `skill_lv`, `grade`, `develop_item_num` , `dress_id` \
                FROM `T_USER_WARRIOR`\
                WHERE `user_id` = ?");
    stmt->setUInt(1, user_id_);
    sql::ResultSet * raw_rs = stmt->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    while(rs->next()){
        warrior_ptr warrior(
                            new cache::warrior(
                                               rs->getUInt64(1),
                                               rs->getUInt(2),
                                               static_cast<uint16_t>(rs->getUInt(3)),
                                               rs->getUInt(4),
                                               static_cast<uint8_t>(rs->getUInt(5)),
                                               rs->getUInt(6),
                                               rs->getUInt(7),
                                               rs->getUInt(8),
                                               rs->getUInt(9),
                                               rs->getUInt(10),
                                               static_cast<uint16_t>(rs->getUInt(11)),
                                               static_cast<uint8_t>(rs->getUInt(12)),
                                               rs->getUInt(13),
                                               rs->getUInt(14)
                                               )
                            );
        map_.insert(std::make_pair(warrior->sid(), warrior));
    }
    rs->close();
    stmt->close();
}

warrior_ptr warrior_pkg::get(uint32_t sid){
    READ_LOCK(lock, mutex_);
    
    map_t::iterator pos = map_.find(sid);
    if(pos != map_.end()){
        return pos->second;
    }
    else{
        return warrior_ptr();
    }
}

error_code warrior_pkg::add(warrior_ptr warrior){
    WRITE_LOCK(lock, mutex_);
    
    map_t::iterator pos = map_.find(warrior->sid());
    if(pos != map_.end()){
        return WARRIOR_EXIST;
    }
    
    if(!db_insert(warrior)){
        return SYS_ERROR;
    }
    map_.insert(std::make_pair(warrior->sid(), warrior));
    return OPT_SUCCESS;
}

error_code warrior_pkg::update(warrior_ptr warrior){
    WRITE_LOCK(lock, mutex_);
    
    map_t::iterator pos = map_.find(warrior->sid());
    if(pos == map_.end()){
        return WARRIOR_NOT_EXIST;
    }
    
    pos->second->copy_from(*warrior);
    async_update(pos->second);
    
    return OPT_SUCCESS;
}

error_code warrior_pkg::update(std::vector<warrior_ptr> &list){
    WRITE_LOCK(lock, mutex_);
    BOOST_FOREACH(warrior_ptr warrior, list){        
        map_t::iterator pos = map_.find(warrior->sid());
        if(pos == map_.end()){
            return WARRIOR_NOT_EXIST;
        }
    }
    
    BOOST_FOREACH(warrior_ptr warrior, list){
        warrior_ptr w = map_[warrior->sid()];
        w->copy_from(*warrior);
        async_update(w);
    }
    
    return OPT_SUCCESS;
}

void warrior_pkg::async_update(warrior_ptr warrior){    
    OP_QUEUE_KEY(std::string key, std::stringstream & ss, "warrior_pkg::db_update", user_id_, warrior->id());
    SINGLETON(player_manager).op_queue().enque(key , boost::bind(&warrior_pkg::db_update, shared_from_this(), warrior));
}

error_code warrior_pkg::remove(uint32_t sid){
    WRITE_LOCK(lock, mutex_);
    
    map_t::iterator pos = map_.find(sid);
    if(pos == map_.end()){
        return WARRIOR_NOT_EXIST;
    }
    warrior_ptr warrior = pos->second;
    map_.erase(pos);    
    async_remove(warrior->id());
    
    return OPT_SUCCESS;
}

error_code warrior_pkg::remove(const std::vector<uint32_t> &list){
    WRITE_LOCK(lock, mutex_);
    BOOST_FOREACH(uint32_t sid, list){
        map_t::iterator pos = map_.find(sid);
        if(pos == map_.end()){
            return WARRIOR_NOT_EXIST;
        }
    }
    
    BOOST_FOREACH(uint32_t sid, list){
        map_t::iterator pos = map_.find(sid);
        warrior_ptr warrior = pos->second;
        map_.erase(pos);
        async_remove(warrior->id());        
    }
    
    return OPT_SUCCESS;
}

void warrior_pkg::copy_to_warriors_res(pro_warriors_res_ptr res){
    READ_LOCK(lock, mutex_);
    
    BOOST_FOREACH(map_t::value_type val, map_){
        val.second->copy_to_proto(res->add_warriors());
    }
}

//---------------------------- private ----------------------------
void warrior_pkg::async_remove(uint64_t id){   
    OP_QUEUE_KEY(std::string key, std::stringstream & ss, "warrior_pkg::db_remove", user_id_, id);
    SINGLETON(player_manager).op_queue().enque(key , boost::bind(&warrior_pkg::db_remove, shared_from_this(), id));
}

bool warrior_pkg::db_insert(warrior_ptr warrior){
    bool result(false);
    SCOPED_CONNECT(scoped, conn);
    PREPARE_SQL(conn, stmt, "INSERT INTO `T_USER_WARRIOR` \
                (`user_id`, `warrior_id`, `warrior_lv`, `warrior_exp`, `warrior_up`, `attack_add`, `defence_add`, `hp_add`, `potential`, `skill_exp`, `skill_lv`, `grade`, `develop_item_num`, `dress_id`)\
                VALUES \
                (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
    stmt->setUInt(1, user_id_);
    stmt->setUInt(2, warrior->sid());
    stmt->setUInt(3, warrior->level());
    stmt->setUInt(4, warrior->exp());
    stmt->setUInt(5, warrior->up());
    stmt->setUInt(6, warrior->attack_add());
    stmt->setUInt(7, warrior->defense_add());
    stmt->setUInt(8, warrior->hp_add());
    stmt->setUInt(9, warrior->potential());
    stmt->setUInt(10, warrior->skill_exp());
    stmt->setUInt(11, warrior->skill_lv());
    stmt->setUInt(12, warrior->grade());
    stmt->setUInt(13, warrior->develop_item_num());
    stmt->setUInt(14, warrior->dress_id());
    
    if(stmt->executeUpdate() == 0){
        stmt->close();
        return false;
    }
    stmt->close();
    
    PREPARE_SQL(conn, query, "SELECT LAST_INSERT_ID();")
    sql::ResultSet * raw_rs = query->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    if(rs->next()){
        warrior->id(rs->getUInt64(1));
        result = true;
    }
    rs->close();
    query->close();
    
    return result;
}

bool warrior_pkg::db_update(warrior_ptr warrior){
    READ_LOCK(lock, warrior->mutex());
    
    if(!warrior->can_update())
    {
        return false;
    }
    
    bool result(false);
    SCOPED_CONNECT(scoped, connection);
    PREPARE_SQL(connection, stmt,"\
                UPDATE `T_USER_WARRIOR` \
                SET `warrior_lv` = ?, \
                `warrior_exp` = ?, \
                `warrior_up` = ?, \
                `attack_add` = ?, \
                `defence_add` = ?, \
                `hp_add` = ?, \
                `potential` = ?, \
                `skill_exp` = ?, \
                `skill_lv` = ?, \
                `develop_item_num` = ?, \
                `dress_id` = ?, \
                `update_ts` = NOW() \
                WHERE `id` = ?;");
    
    stmt->setUInt(1, warrior->level());
    stmt->setUInt(2, warrior->exp());
    stmt->setUInt(3, warrior->up());
    stmt->setUInt(4, warrior->attack_add());
    stmt->setUInt(5, warrior->defense_add());
    stmt->setUInt(6, warrior->hp_add());
    stmt->setUInt(7, warrior->potential());
    stmt->setUInt(8, warrior->skill_exp());
    stmt->setUInt(9, warrior->skill_lv());
    stmt->setUInt(10, warrior->develop_item_num());
    stmt->setUInt(11, warrior->dress_id());
    stmt->setUInt64(12, warrior->id());
    
    stmt->executeUpdate();
    stmt->close();
    result = true;
    warrior->last_update_ts(TIME_NOW.get_time_t());
    
    return result;
}

bool warrior_pkg::db_remove(uint64_t id){    
    bool result(false);
    SCOPED_CONNECT(scoped, connection);
    PREPARE_SQL(connection, stmt,"\
                DELETE FROM `T_USER_WARRIOR` \
                WHERE `id` = ?;");
    
    stmt->setUInt64(1, id);
    
    stmt->executeUpdate();
    stmt->close();
    result = true;
    
    return result;    
}
