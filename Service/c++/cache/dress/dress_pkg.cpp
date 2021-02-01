//
//  dress_pkg.cpp
//  cache
//
//  Created by 汪燕军 on 14-3-4.
//  Copyright (c) 2014年 汪燕军. All rights reserved.
//

#include "dress_pkg.h"
#include "player.h"
#include "player_manager.h"

using namespace cache;

void dress_pkg::load(){
    SCOPED_CONNECT(scoped, conn);
    PREPARE_SQL(conn, stmt, "\
                SELECT `id`, `dress_id`, `warrior_id`, `level` \
                FROM `T_USER_PACKAGE_DRESS`\
                WHERE `user_id` = ?");
    stmt->setUInt(1, user_id_);
    sql::ResultSet * raw_rs = stmt->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    while(rs->next()){
        dress_ptr dress(
                        new cache::dress(
                                         rs->getUInt64(1),
                                         rs->getUInt(2),
                                         rs->getUInt(3),
                                         static_cast<uint16_t>(rs->getUInt(4))
                                         )
                        );
        map_.insert(std::make_pair(dress->dress_id(), dress));
        warrior_map_t::iterator iter = warrior_map_.find(dress->warrior_id());
        boost::shared_ptr<std::vector<dress_ptr> > list_ptr;
        if(iter == warrior_map_.end())
        {
            list_ptr = boost::shared_ptr<std::vector<dress_ptr> >(new std::vector<dress_ptr>);
            warrior_map_.insert(std::make_pair(dress->warrior_id(), list_ptr));
        }
        else
        {
            list_ptr = iter->second;
        }
        list_ptr->push_back(dress);
    }
    rs->close();
    stmt->close();
}

dress_ptr dress_pkg::get(uint32_t dress_id){
    READ_LOCK(lock, mutex_);
    
    map_t::iterator pos = map_.find(dress_id);
    if(pos != map_.end()){
        return pos->second;
    }
    else{
        return dress_ptr();
    }
}

boost::shared_ptr<std::vector<dress_ptr> > dress_pkg::get_by_warrior(uint32_t warrior_id)
{
    READ_LOCK(lock, mutex_);
    warrior_map_t::iterator iter = warrior_map_.find(warrior_id);
    if(iter == warrior_map_.end())
    {
        return boost::shared_ptr<std::vector<dress_ptr> >();
    }
    else
    {
        return iter->second;
    }
}

error_code dress_pkg::add(dress_ptr dress){
    WRITE_LOCK(lock, mutex_);
    
    map_t::iterator pos = map_.find(dress->dress_id());
    if(pos != map_.end()){
        return DRESS_EXIST;
    }
    
    if(!db_insert(dress)){
        return SYS_ERROR;
    }
    map_.insert(std::make_pair(dress->dress_id(), dress));
    warrior_map_t::iterator iter = warrior_map_.find(dress->warrior_id());
    boost::shared_ptr<std::vector<dress_ptr> > list_ptr;
    if(iter == warrior_map_.end())
    {
        list_ptr = boost::shared_ptr<std::vector<dress_ptr> >(new std::vector<dress_ptr>);
        warrior_map_.insert(std::make_pair(dress->warrior_id(), list_ptr));
    }
    else
    {
        list_ptr = iter->second;
    }
    
    list_ptr->push_back(dress);
    
    return OPT_SUCCESS;
}

error_code dress_pkg::update(dress_ptr dress){
    WRITE_LOCK(lock, mutex_);
    
    map_t::iterator pos = map_.find(dress->dress_id());
    if(pos == map_.end()){
        return DRESS_NOT_EXIST;
    }
    
    pos->second->copy_from(*dress);
    async_update(pos->second);
    
    return OPT_SUCCESS;
}

void dress_pkg::async_update(dress_ptr dress){
    OP_QUEUE_KEY(std::string key, std::stringstream & ss, "dress_pkg::db_update", user_id_, dress->id());
    SINGLETON(player_manager).op_queue().enque(key , boost::bind(&dress_pkg::db_update, shared_from_this(), dress));
}

error_code dress_pkg::remove(uint32_t dress_id){
    WRITE_LOCK(lock, mutex_);
    
    map_t::iterator pos = map_.find(dress_id);
    if(pos == map_.end()){
        return DRESS_NOT_EXIST;
    }
    dress_ptr dress = pos->second;
    map_.erase(pos);
    
    warrior_map_t::iterator iter = warrior_map_.find(dress->warrior_id());
    if(iter != warrior_map_.end())
    {
        for(BOOST_AUTO(d_pos, iter->second->begin()); d_pos != iter->second->end(); ++d_pos)
        {
            if((*d_pos)->dress_id() == dress->dress_id())
            {
                iter->second->erase(d_pos);
                if(iter->second->empty())
                {
                    warrior_map_.erase(iter);
                }
                break;
            }
        }
    }
    
    async_remove(dress->id());
    
    return OPT_SUCCESS;
}

void dress_pkg::copy_to_dresses_res(pro_dresses_res_ptr res){
    READ_LOCK(lock, mutex_);
    
    BOOST_FOREACH(map_t::value_type val, map_){
        val.second->copy_to_proto(res->add_dresses());
    }
}

//---------------------------- private ----------------------------
void dress_pkg::async_remove(uint64_t id){
    OP_QUEUE_KEY(std::string key, std::stringstream & ss, "dress_pkg::db_remove", user_id_, id);
    SINGLETON(player_manager).op_queue().enque(key , boost::bind(&dress_pkg::db_remove, shared_from_this(), id));
}

bool dress_pkg::db_insert(dress_ptr dress){
    bool result(false);
    SCOPED_CONNECT(scoped, conn);
    PREPARE_SQL(conn, stmt, "INSERT INTO `T_USER_PACKAGE_DRESS` \
                (`user_id`, `dress_id`, `warrior_id`, `level`)\
                VALUES \
                (?, ?, ?, ?);");
    stmt->setUInt(1, user_id_);
    stmt->setUInt(2, dress->dress_id());
    stmt->setUInt(3, dress->warrior_id());
    stmt->setUInt(4, dress->level());
    
    if(stmt->executeUpdate() == 0){
        stmt->close();
        return false;
    }
    stmt->close();
    
    PREPARE_SQL(conn, query, "SELECT LAST_INSERT_ID();")
    sql::ResultSet * raw_rs = query->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    if(rs->next()){
        dress->id(rs->getUInt64(1));
        result = true;
    }
    rs->close();
    query->close();
    
    return result;
}

bool dress_pkg::db_update(dress_ptr dress){
    READ_LOCK(lock, dress->mutex());
    
    if(!dress->can_update())
    {
        return false;
    }
    
    bool result(false);
    SCOPED_CONNECT(scoped, connection);
    PREPARE_SQL(connection, stmt,"\
                UPDATE `T_USER_PACKAGE_DRESS` \
                SET `level` = ? \
                WHERE `id` = ?;");
    
    stmt->setUInt(1, dress->level());
    stmt->setUInt64(2, dress->id());
    
    stmt->executeUpdate();
    stmt->close();
    result = true;
    dress->last_update_ts(TIME_NOW.get_time_t());
    
    return result;
}

bool dress_pkg::db_remove(uint64_t id){
    bool result(false);
    SCOPED_CONNECT(scoped, connection);
    PREPARE_SQL(connection, stmt,"\
                DELETE FROM `T_USER_PACKAGE_DRESS` \
                WHERE `id` = ?;");
    
    stmt->setUInt64(1, id);
    
    stmt->executeUpdate();
    stmt->close();
    result = true;
    
    return result;
}
