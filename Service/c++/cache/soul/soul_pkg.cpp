//
//  soul_pkg.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "soul_pkg.h"
#include "soul.h"
#include "player.h"
#include "player_manager.h"

using namespace cache;

void soul_pkg::load(){
    SCOPED_CONNECT(scoped, conn);
    PREPARE_SQL(conn, stmt, "\
                SELECT `id`, `warrior_id`, `soul_num`, `protect_ts`\
                FROM `T_USER_PACKAGE_SOUL`\
                WHERE `user_id` = ?");
    stmt->setUInt(1, user_id_);
    sql::ResultSet * raw_rs = stmt->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    while(rs->next()){
        soul_ptr soul(
                      new cache::soul(
                                      rs->getUInt64(1),
                                      rs->getUInt(2),
                                      rs->getUInt(3),
                                      rs->getUInt(4)
                                      )
                      );
        map_[soul->soul_id()] = soul;
    }
    rs->close();
    stmt->close();
}

soul_ptr soul_pkg::get(uint32_t soul_id){
    READ_LOCK(lock, mutex_);
    map_t::iterator pos = map_.find(soul_id);
    if (pos != map_.end()) {
        return pos->second;
    }
    else{
        return soul_ptr(new soul(0, soul_id, 0, 0));
    }
}

error_code soul_pkg::add(soul_ptr soul){
    WRITE_LOCK(lock, mutex_);
    map_t::iterator pos = map_.find(soul->soul_id());
    if (pos != map_.end()) {
        pos->second->add(soul->num());
        async_update(pos->second);
    }
    else{
        if(!db_insert(soul)){
            return SYS_ERROR;
        }
        map_[soul->soul_id()] = soul;
    }
    return OPT_SUCCESS;
}

error_code soul_pkg::add(std::vector<soul_ptr> &list){
    std::vector<soul_ptr> insert_list;
    std::vector<soul_ptr> update_list;
    insert_list.reserve(list.size());
    update_list.reserve(list.size());
    
    WRITE_LOCK(lock, mutex_);
    
    BOOST_FOREACH(soul_ptr soul, list){
        map_t::iterator pos = map_.find(soul->soul_id());
        if (pos != map_.end()) {
            update_list.push_back(soul);
        }
        else{
            insert_list.push_back(soul);
        }
    }
    
    
    if(!db_insert(insert_list)){
        return SYS_ERROR;
    }
    
    BOOST_FOREACH(soul_ptr soul, insert_list){
        map_[soul->soul_id()] = soul;
    }
    
    BOOST_FOREACH(soul_ptr soul, update_list){
        soul_ptr p = map_[soul->soul_id()];
        p->add(soul->num());
        async_update(p);
    }
    
    return OPT_SUCCESS;
}

error_code soul_pkg::remove(soul_ptr soul){
    WRITE_LOCK(lock, mutex_);
    map_t::iterator pos = map_.find(soul->soul_id());
    if (pos != map_.end() && pos->second->deduct(soul)) {
        async_update(pos->second);
        return OPT_SUCCESS;
    }
    else{
        return NOT_ENOUGH_SOUL;
    }
}

error_code soul_pkg::remove(std::vector<soul_ptr> &list){
    WRITE_LOCK(lock, mutex_);
    BOOST_FOREACH(soul_ptr soul, list){
        map_t::iterator pos = map_.find(soul->soul_id());
        if(pos == map_.end() || !pos->second->is_enough(soul->num())){
            return NOT_ENOUGH_SOUL;
        }
    }
    BOOST_FOREACH(soul_ptr soul, list){
        soul_ptr p = map_[soul->soul_id()];
        p->deduct(soul);
        async_update(p);
    }
    return OPT_SUCCESS;
}

void soul_pkg::copy_to_souls_res(pro_souls_res_ptr res, std::vector<uint32_t> & souls){
    READ_LOCK(lock, mutex_);
    if(souls.size() == 0){
        BOOST_FOREACH(map_t::value_type val, map_){
            if(val.second->num() > 0)
            {
                val.second->copy_to_proto(res->add_souls());
            }
        }
    }
    else{
        BOOST_FOREACH(uint32_t id, souls){
            map_t::iterator pos = map_.find(id);
            if(pos != map_.end() && pos->second->num() > 0){
                pos->second->copy_to_proto(res->add_souls());
            }
        }
    }
}

//---------------------------- private ----------------------------

void soul_pkg::async_update(soul_ptr soul){
    OP_QUEUE_KEY(std::string key, std::stringstream & ss, "soul_pkg::db_update", user_id_, soul->id());
    SINGLETON(player_manager).op_queue().enque(key , boost::bind(&soul_pkg::db_update, shared_from_this(), soul));
}

bool soul_pkg::db_insert(soul_ptr soul){
    SCOPED_CONNECT(scoped, connection);
    connection->setAutoCommit(false);
    if(db_insert(connection.get(), soul)){
        connection->commit();
        connection->setAutoCommit(true);
        return true;
    }
    else{
        connection->rollback();
        connection->setAutoCommit(true);
        return false;
    }
}

bool soul_pkg::db_insert(sql::Connection *conn, soul_ptr soul){
    bool result(false);
    PREPARE_SQL(conn, stmt, "INSERT INTO `T_USER_PACKAGE_SOUL` \
                (`user_id`, `warrior_id`, `soul_num`, `protect_ts`)\
                VALUES \
                (?, ?, ?, ?);");
    stmt->setUInt(1, user_id_);
    stmt->setUInt(2, soul->soul_id());
    stmt->setUInt(3, soul->num());
    stmt->setUInt(4, soul->protect_ts());
    if(stmt->executeUpdate() == 0){
        stmt->close();
        return false;
    }
    stmt->close();
    
    PREPARE_SQL(conn, query, "SELECT LAST_INSERT_ID();")
    sql::ResultSet * raw_rs = query->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    if(rs->next()){
        soul->id(rs->getUInt64(1));
        result = true;
    }
    rs->close();
    query->close();
    
    return result;
}

bool soul_pkg::db_insert(std::vector<soul_ptr> &list){
    SCOPED_CONNECT(scoped, connection);
    connection->setAutoCommit(false);
    
    BOOST_FOREACH(soul_ptr soul, list){
        if(!db_insert(connection.get(), soul)){
            connection->rollback();
            connection->close();
            return false;
        }
    }
    
    connection->commit();
    connection->setAutoCommit(true);
    
    return true;
}

bool soul_pkg::db_update(soul_ptr soul){
    READ_LOCK(lock, soul->mutex());
    
    if(!soul->can_update())
    {
        return false;
    }
    
    bool result(false);
    SCOPED_CONNECT(scoped, connection);
    PREPARE_SQL(connection, stmt,"\
                UPDATE `T_USER_PACKAGE_SOUL` \
                SET `soul_num` = ?, \
                    `protect_ts` = ?\
                WHERE `id` = ?;");
    
    stmt->setUInt(1, soul->num());
    stmt->setUInt(2, soul->protect_ts());
    stmt->setUInt64(3, soul->id());
    
    stmt->executeUpdate();
    stmt->close();
    result = true;
    soul->last_update_ts(TIME_NOW.get_time_t());
    
    return result;
}
