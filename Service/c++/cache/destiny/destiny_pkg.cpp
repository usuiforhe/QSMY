//
//  destiny_pkg.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "destiny_pkg.h"
#include "player.h"
#include "player_manager.h"

using namespace cache;

void destiny_pkg::load(){
    SCOPED_CONNECT(scoped, conn);
    PREPARE_SQL(conn, stmt, "\
                SELECT `id`, `destiny_id`, `type`, `level`, `reward_ts`, `num1`, `num2`, `num3`, `num4`, `num5`, `num6`\
                FROM `T_USER_PACKAGE_DESTINY`\
                WHERE `user_id` = ?");
    stmt->setUInt(1, user_id_);
    sql::ResultSet * raw_rs = stmt->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    while(rs->next()){
        destiny_ptr destiny(
                            new cache::destiny(
                                               rs->getUInt64(1),
                                               rs->getUInt(2),
                                               static_cast<uint8_t>(rs->getUInt(3)),
                                               static_cast<uint16_t>(rs->getUInt(4)),
                                               rs->getUInt(5),
                                               static_cast<uint16_t>(rs->getUInt(6)),
                                               static_cast<uint16_t>(rs->getUInt(7)),
                                               static_cast<uint16_t>(rs->getUInt(8)),
                                               static_cast<uint16_t>(rs->getUInt(9)),
                                               static_cast<uint16_t>(rs->getUInt(10)),
                                               static_cast<uint16_t>(rs->getUInt(11))
                                               )
                            );
        add(destiny);
    }
    rs->close();
    stmt->close();
}

destiny_ptr destiny_pkg::get(uint32_t destiny_id){
    READ_LOCK(lock, mutex_);
    id_map_t::iterator pos = id_map_.find(destiny_id);
    if(pos != id_map_.end()){
        return pos->second;
    }
    else{
        return destiny_ptr();
    }
}

error_code destiny_pkg::save(destiny_ptr destiny){
    WRITE_LOCK(lock, mutex_);
    id_map_t::iterator pos = id_map_.find(destiny->destiny_id());
    if(pos != id_map_.end()){
        pos->second->copy_from(*destiny);
        OP_QUEUE_KEY(std::string key, std::stringstream & ss, "destiny_pkg::db_update", user_id_, pos->second->id());
        SINGLETON(player_manager).op_queue().enque(key , boost::bind(&destiny_pkg::db_update, shared_from_this(), pos->second));
    }
    else{
        if(!db_insert(destiny)){
            return SYS_ERROR;
        }
        add(destiny);
    }
    return OPT_SUCCESS;
}

void destiny_pkg::copy_to_destinies_res(pro_destinies_res_ptr res, uint8_t type){
    READ_LOCK(lock, mutex_);
    if(type == 0){
        BOOST_FOREACH(id_map_t::value_type val, id_map_){
            val.second->copy_to_proto(res->add_destinies());
        }
    }
    else{
        type_map_t::iterator pos = type_map_.find(type);
        if(pos == type_map_.end()){
            return;
        }
        BOOST_FOREACH(destiny_ptr destiny, *(pos->second)){
            destiny->copy_to_proto(res->add_destinies());
        }
    }
}

//---------------------------- private ----------------------------

void destiny_pkg::add(destiny_ptr destiny){
    id_map_.insert(std::make_pair(destiny->destiny_id(), destiny));
    
    type_map_t::iterator pos = type_map_.find(destiny->type());
    if(pos == type_map_.end()){
        boost::shared_ptr<std::vector<destiny_ptr> > list = boost::make_shared<std::vector<destiny_ptr> >();
        list->push_back(destiny);
        type_map_.insert(std::make_pair(destiny->type(), list));
    }
    else{
        pos->second->push_back(destiny);
    }
}

bool destiny_pkg::db_insert(destiny_ptr destiny){    
    bool result(false);
    SCOPED_CONNECT(scoped, conn);
    PREPARE_SQL(conn, stmt, "INSERT INTO `T_USER_PACKAGE_DESTINY` \
                (`user_id`, `destiny_id`, `type`, `level`, `reward_ts`, `num1`, `num2`, `num3`, `num4`, `num5`, `num6`)\
                VALUES \
                (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
    stmt->setUInt(1, user_id_);
    stmt->setUInt(2, destiny->destiny_id());
    stmt->setUInt(3, destiny->type());
    stmt->setUInt(4, destiny->level());
    stmt->setUInt(5, destiny->reward_ts());
    stmt->setUInt(6, destiny->num1());
    stmt->setUInt(7, destiny->num2());
    stmt->setUInt(8, destiny->num3());
    stmt->setUInt(9, destiny->num4());
    stmt->setUInt(10, destiny->num5());
    stmt->setUInt(11, destiny->num6());
    
    if(stmt->executeUpdate() == 0){
        stmt->close();
        return false;
    }
    stmt->close();
    
    PREPARE_SQL(conn, query, "SELECT LAST_INSERT_ID();")
    sql::ResultSet * raw_rs = query->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    if(rs->next()){
        destiny->id(rs->getUInt64(1));
        result = true;
    }
    rs->close();
    query->close();   
    
    return result;
}

bool destiny_pkg::db_update(destiny_ptr destiny){
    READ_LOCK(lock, destiny->mutex());
    
    
    if(!destiny->can_update())
    {
        return false;
    }
    
    bool result(false);
    SCOPED_CONNECT(scoped, connection);
    PREPARE_SQL(connection, stmt,"\
                UPDATE `T_USER_PACKAGE_DESTINY` \
                SET `level` = ?, \
                    `reward_ts` = ?, \
                    `num1` = ?, \
                    `num2` = ?, \
                    `num3` = ?, \
                    `num4` = ?, \
                    `num5` = ?, \
                    `num6` = ? \
                WHERE `id` = ?;");
    
    stmt->setUInt(1, destiny->level());
    stmt->setUInt(2, destiny->reward_ts());
    stmt->setUInt(3, destiny->num1());
    stmt->setUInt(4, destiny->num2());
    stmt->setUInt(5, destiny->num3());
    stmt->setUInt(6, destiny->num4());
    stmt->setUInt(7, destiny->num5());
    stmt->setUInt(8, destiny->num6());
    stmt->setUInt64(9, destiny->id());
    
    stmt->executeUpdate();
    stmt->close();
    result = true;
    destiny->last_update_ts(TIME_NOW.get_time_t());
    
    return result;    
}
