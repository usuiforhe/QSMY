//
//  item_pkg.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "item_pkg.h"
#include "item.h"
#include "player.h"
#include "player_manager.h"

using namespace cache;

void item_pkg::load(){
    SCOPED_CONNECT(scoped, conn);
    PREPARE_SQL(conn, stmt, "\
                SELECT `id`, `item_id`, `item_num`\
                FROM `T_USER_PACKAGE_ITEM`\
                WHERE `user_id` = ?");
    stmt->setUInt(1, user_id_);
    sql::ResultSet * raw_rs = stmt->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    while(rs->next()){
        item_ptr item(
                        new cache::item(
                                         rs->getUInt64(1),
                                         rs->getUInt(2),
                                         rs->getUInt(3)
                                         )
                        );
        map_[item->item_id()] = item;
    }
    rs->close();
    stmt->close();
}

item_ptr item_pkg::get(uint32_t item_id){
    READ_LOCK(lock, mutex_);
    map_t::iterator pos = map_.find(item_id);
    if (pos != map_.end()) {
        return pos->second;
    }
    else{
        return item_ptr(new item(0, item_id, 0));
    }
}

error_code item_pkg::add(item_ptr item){
    WRITE_LOCK(lock, mutex_);
    map_t::iterator pos = map_.find(item->item_id());
    if (pos != map_.end()) {
        pos->second->add(item->num());
        async_update(pos->second);
    }
    else{
        if(!db_insert(item)){
            return SYS_ERROR;
        }
        map_[item->item_id()] = item;
    }
    return OPT_SUCCESS;
}

error_code item_pkg::add(std::vector<item_ptr> &list){    
    std::vector<item_ptr> insert_list;
    std::vector<item_ptr> update_list;
    insert_list.reserve(list.size());
    update_list.reserve(list.size());
    
    WRITE_LOCK(lock, mutex_);
    
    BOOST_FOREACH(item_ptr item, list){
        map_t::iterator pos = map_.find(item->item_id());
        if (pos != map_.end()) {
            update_list.push_back(item);
        }
        else{
            insert_list.push_back(item);
        }
    }
    
    
    if(!db_insert(insert_list)){
        return SYS_ERROR;
    }
    
    BOOST_FOREACH(item_ptr item, insert_list){
        map_[item->item_id()] = item;
    }
    
    BOOST_FOREACH(item_ptr item, update_list){
        item_ptr p = map_[item->item_id()];
        p->add(item->num());
        async_update(p);
    }
    
    return OPT_SUCCESS;
}

error_code item_pkg::remove(item_ptr item){
    WRITE_LOCK(lock, mutex_);
    map_t::iterator pos = map_.find(item->item_id());
    if (pos != map_.end() && pos->second->deduct(item->num())) {
        async_update(pos->second);
        return OPT_SUCCESS;
    }
    else{
        return NOT_ENOUGH_ITEM;
    }
}

error_code item_pkg::remove(std::vector<item_ptr> &list){
    WRITE_LOCK(lock, mutex_);
    BOOST_FOREACH(item_ptr item, list){
        map_t::iterator pos = map_.find(item->item_id());
        if(pos == map_.end() || !pos->second->is_enough(item->num())){            
            return NOT_ENOUGH_ITEM;
        }
    }
    BOOST_FOREACH(item_ptr item, list){
        item_ptr p = map_[item->item_id()];
        p->deduct(item->num());
        async_update(p);
    }
    return OPT_SUCCESS;
}

void item_pkg::copy_to_items_res(pro_items_res_ptr res){
    READ_LOCK(lock, mutex_);
    BOOST_FOREACH(map_t::value_type val, map_){
        if(val.second->num() > 0)
        {
            val.second->copy_to_proto(res->add_items());
        }
    }
}

//---------------------------- private ----------------------------

void item_pkg::async_update(item_ptr item){    
    OP_QUEUE_KEY(std::string key, std::stringstream & ss, "item_pkg::db_update", user_id_, item->id());
    SINGLETON(player_manager).op_queue().enque(key , boost::bind(&item_pkg::db_update, shared_from_this(), item));
}

void item_pkg::async_remove(uint64_t id){
    OP_QUEUE_KEY(std::string key, std::stringstream & ss, "item_pkg::db_remove", user_id_, id);
    SINGLETON(player_manager).op_queue().enque(key , boost::bind(&item_pkg::db_remove, shared_from_this(), id));
}

bool item_pkg::db_insert(item_ptr item){
    SCOPED_CONNECT(scoped, connection);
    connection->setAutoCommit(false);
    if(db_insert(connection.get(), item)){
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

bool item_pkg::db_insert(sql::Connection *conn, item_ptr item){    
    bool result(false);
    PREPARE_SQL(conn, stmt, "INSERT INTO `T_USER_PACKAGE_ITEM` \
                (`user_id`, `item_id`, `item_num`)\
                VALUES \
                (?, ?, ?);");
    stmt->setUInt(1, user_id_);
    stmt->setUInt(2, item->item_id());
    stmt->setUInt(3, item->num());
    if(stmt->executeUpdate() == 0){
        stmt->close();
        return false;
    }
    stmt->close();
    
    PREPARE_SQL(conn, query, "SELECT LAST_INSERT_ID();")
    sql::ResultSet * raw_rs = query->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    if(rs->next()){
        item->id(rs->getUInt64(1));
        result = true;
    }
    rs->close();
    query->close();
    
    return result;
}

bool item_pkg::db_insert(std::vector<item_ptr> &list){
    SCOPED_CONNECT(scoped, connection);
    connection->setAutoCommit(false);
    
    BOOST_FOREACH(item_ptr item, list){
        if(!db_insert(connection.get(), item)){
            connection->rollback();
            connection->setAutoCommit(true);
            return false;
        }
    }
    
    connection->commit();
    connection->setAutoCommit(true);
    return true;
}

bool item_pkg::db_update(item_ptr item){
    READ_LOCK(lock, item->mutex());
    
    if(!item->can_update())
    {
        return false;
    }
    
    bool result(false);
    SCOPED_CONNECT(scoped, connection);
    PREPARE_SQL(connection, stmt,"\
                UPDATE `T_USER_PACKAGE_ITEM` \
                SET `item_num` = ?\
                WHERE `id` = ?;");
    
    stmt->setUInt(1, item->num());
    stmt->setUInt64(2, item->id());
    
    stmt->executeUpdate();
    stmt->close();
    result = true;
    item->last_update_ts(TIME_NOW.get_time_t());
    
    return result;
}

bool item_pkg::db_remove(uint64_t id){    
    bool result(false);
    SCOPED_CONNECT(scoped, connection);
    PREPARE_SQL(connection, stmt,"\
                DELETE \
                FROM `T_USER_PACKAGE_ITEM` \
                WHERE `id` = ?;");
    
    stmt->setUInt64(1, id);
    
    stmt->executeUpdate();
    stmt->close();
    result = true;
    return result;
}
