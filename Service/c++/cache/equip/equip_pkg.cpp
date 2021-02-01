//
//  equip_pkg.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "equip_pkg.h"
#include "equip.h"
#include "player.h"
#include "player_manager.h"

using namespace cache;

void equip_pkg::load(){
    SCOPED_CONNECT(scoped, conn);
    PREPARE_SQL(conn, stmt, "\
        SELECT `id`, `equip_id`, `exp`, `level`, `gem`, `pos_seq_id`, `type`, `color`, `refining`, `refining_exp` \
        FROM `T_USER_PACKAGE_EQUIP`\
        WHERE `user_id` = ?");
    stmt->setUInt(1, user_id_);
    sql::ResultSet * raw_rs = stmt->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    while(rs->next()){
        equip_ptr equip(
                        new cache::equip(
                                         rs->getUInt64(1),
                                         rs->getUInt(2),
                                         rs->getUInt(3),
                                         static_cast<uint16_t>(rs->getUInt(4)),
                                         rs->getUInt(5),
                                         rs->getUInt(6),
                                         static_cast<uint8_t>(rs->getUInt(7)),
                                         static_cast<uint8_t>(rs->getUInt(8)),
                                         static_cast<uint16_t>(rs->getUInt(9)),
										 static_cast<uint16_t>(rs->getUInt(10))
                                         )
                        );
        set(equip);
    }
    rs->close();
    stmt->close();
}

equip_ptr equip_pkg::get(uint64_t id){
    READ_LOCK(lock, mutex_);
    
    id_map_t::iterator pos = id_map_.find(id);
    equip_ptr equip;
    if(pos != id_map_.end()){
        return pos->second;        
    }
    return equip;
}

boost::shared_ptr<std::vector<equip_ptr> > equip_pkg::get_by_pos(uint32_t pos){
    READ_LOCK(lock, mutex_);
    
    pos_map_t::iterator iter = pos_map_.find(pos);
    if(iter != pos_map_.end()){
        return iter->second;
    }
    else{
        return boost::shared_ptr<std::vector<equip_ptr> >();
    }
}

error_code equip_pkg::add(equip_ptr equip){
    WRITE_LOCK(lock, mutex_);
    if(equip->id() > 0 && id_map_.find(equip->id()) != id_map_.end()){
        return ALREADY_HAS_EQUIP;
    }
    
    if(equip->id() > 0){
        async_update(equip);
    }
    else{
        if(!db_insert(equip)){
            return SYS_ERROR;
        }
        
        set(equip);
    }
    
    return OPT_SUCCESS;
}

error_code equip_pkg::add(std::vector<equip_ptr> &list){
    std::vector<equip_ptr> insert_list;
    std::vector<equip_ptr> update_list;
    insert_list.reserve(list.size());
    update_list.reserve(list.size());
    
    WRITE_LOCK(lock, mutex_);
    
    BOOST_FOREACH(equip_ptr equip, list){
        if(equip->id() == 0){
            insert_list.push_back(equip);
        }
        else{
            if(id_map_.find(equip->id()) != id_map_.end()){
                return ALREADY_HAS_EQUIP;
            }
            update_list.push_back(equip);
        }
    }
    
    if(!db_insert(insert_list)){
        return SYS_ERROR;
    }
    
    BOOST_FOREACH(equip_ptr equip, insert_list){
        set(equip);
    }
    
    BOOST_FOREACH(equip_ptr equip, update_list){
        async_update(equip);
    }
    
    return OPT_SUCCESS;
}

error_code equip_pkg::update(equip_ptr equip){
    WRITE_LOCK(lock, mutex_);
    if(id_map_.find(equip->id()) == id_map_.end()){
        return NO_SUCH_EQUIP;
    }
    async_update(equip);
    return OPT_SUCCESS;
}

error_code equip_pkg::update(std::vector<equip_ptr> &list){
    WRITE_LOCK(lock, mutex_);
    BOOST_FOREACH(equip_ptr equip, list){
        if(id_map_.find(equip->id()) == id_map_.end()){
            return NO_SUCH_EQUIP;
        }
    }
    
    BOOST_FOREACH(equip_ptr equip, list){
        async_update(equip);
    }
    
    return OPT_SUCCESS;
}

error_code equip_pkg::remove(std::vector<uint64_t> & list){
    WRITE_LOCK(lock, mutex_);
    BOOST_FOREACH(uint64_t id, list){        
        id_map_t::iterator pos = id_map_.find(id);
        if(pos == id_map_.end()){
            return NO_SUCH_EQUIP;
        }
    }
    BOOST_FOREACH(uint64_t id, list){        
        async_remove(id_map_[id]);
    }
    return OPT_SUCCESS;
}

error_code equip_pkg::remove(uint64_t id){
    WRITE_LOCK(lock, mutex_);
    id_map_t::iterator pos = id_map_.find(id);
    if(pos == id_map_.end()){
        return NO_SUCH_EQUIP;
    }
    async_remove(pos->second);
    
    return OPT_SUCCESS;
}

void equip_pkg::copy_to_equips_res(pro_equips_res_ptr res){
    READ_LOCK(lock, mutex_);
    BOOST_FOREACH(id_map_t::value_type val, id_map_){
        val.second->copy_to_proto(res->add_equips());
    }
}

//--------------------------------- private ---------------------------------
void equip_pkg::set(equip_ptr to_set){
    id_map_t::iterator pos = id_map_.find(to_set->id());
    if(pos == id_map_.end()){
        id_map_[to_set->id()] = to_set;
        set_pos(to_set);
    }
    else{
        equip_ptr equip = pos->second;
        WRITE_LOCK(lock, equip->mutex());
        bool change_pos = equip->pos() != to_set->pos();
        if(change_pos){
            unset_pos(equip);
        }
        equip->copy_from(*to_set);
        if(change_pos){
            set_pos(equip);
        }
    }
}

void equip_pkg::set_pos(equip_ptr equip){
    if(equip->pos() == 0){
        return;
    }
    
    pos_map_t::iterator iter = pos_map_.find(equip->pos());
    if(iter == pos_map_.end()){
        boost::shared_ptr<std::vector<equip_ptr> > list(new std::vector<equip_ptr>);
        list->push_back(equip);
        pos_map_.insert(std::make_pair(equip->pos(), list));
    }
    else{
        iter->second->push_back(equip);
    }
}

void equip_pkg::async_update(equip_ptr equip){    
    set(equip);
    equip_ptr p(id_map_[equip->id()]);
    OP_QUEUE_KEY(std::string key, std::stringstream & ss, "equip_pkg::db_update", user_id_, p->id());
    SINGLETON(player_manager).op_queue().enque(key , boost::bind(&equip_pkg::db_update, shared_from_this(), p));
}

void equip_pkg::async_remove(equip_ptr equip){
    id_map_.erase(equip->id());
    unset_pos(equip);
    
    OP_QUEUE_KEY(std::string key, std::stringstream & ss, "equip_pkg::db_remove", user_id_, equip->id());
    SINGLETON(player_manager).op_queue().enque(key , boost::bind(&equip_pkg::db_remove, shared_from_this(), equip->id()));    
}

void equip_pkg::unset_pos(equip_ptr equip){
    if(equip->pos() == 0){
        return;
    }
    
    pos_map_t::iterator pos_iter = pos_map_.find(equip->pos());
    if(pos_iter != pos_map_.end()){
        for (BOOST_AUTO(iter, pos_iter->second->begin()); iter != pos_iter->second->end(); ++iter) {
            if((*iter)->id() == equip->id()){
                pos_iter->second->erase(iter);
                break;
            }
        }
    }
}

bool equip_pkg::db_insert(equip_ptr equip){
    SCOPED_CONNECT(scoped, connection);
    connection->setAutoCommit(false);
    if(db_insert(connection.get(), equip)){
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

bool equip_pkg::db_insert(std::vector<equip_ptr> & list){
    SCOPED_CONNECT(scoped, connection);
    connection->setAutoCommit(false);
    
    BOOST_FOREACH(equip_ptr equip, list){
        if(!db_insert(connection.get(), equip)){
            connection->rollback();
            connection->setAutoCommit(true);
            return false;
        }
    }
    
    connection->commit();
    connection->setAutoCommit(true);
    
    return true;
}

bool equip_pkg::db_insert(sql::Connection * conn, equip_ptr equip){
    bool result(false);    
    PREPARE_SQL(conn, stmt, "INSERT INTO `T_USER_PACKAGE_EQUIP` \
                (`user_id`, `equip_id`, `exp`, `level`, `gem`, `pos_seq_id`, `type`, `color`, `refining`, `refining_exp`)\
                VALUES \
                (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
    stmt->setUInt(1, user_id_);
    stmt->setUInt(2, equip->sid());
    stmt->setUInt(3, equip->exp());
    stmt->setUInt(4, equip->level());
    stmt->setUInt(5, equip->gem());
    stmt->setUInt(6, equip->pos());
    stmt->setUInt(7, equip->type());
    stmt->setUInt(8, equip->color());
    stmt->setUInt(9, equip->refining());
	stmt->setUInt(10, equip->refining_exp());
    if(stmt->executeUpdate() == 0){
        stmt->close();
        return false;
    }
    stmt->close();
    
    PREPARE_SQL(conn, query, "SELECT LAST_INSERT_ID();")
    sql::ResultSet * raw_rs = query->executeQuery();
    std::auto_ptr<sql::ResultSet> rs(raw_rs);
    if(rs->next()){
        equip->id(rs->getUInt64(1));
        result = true;
    }
    rs->close();
    query->close();
    
    return result;
}

bool equip_pkg::db_update(equip_ptr equip){
    READ_LOCK(lock, equip->mutex());
        
    if(!equip->can_update())
    {
        return false;
    }
    
    bool result(false);
    SCOPED_CONNECT(scoped, connection);
    PREPARE_SQL(connection, stmt,"\
    UPDATE `T_USER_PACKAGE_EQUIP` \
    SET `exp` = ?,\
        `level` = ?,\
        `gem` = ?,\
        `pos_seq_id` = ?, \
		`refining` = ?, \
        `refining_exp` = ? \
    WHERE `id` = ?;");
    
    stmt->setUInt(1, equip->exp());
    stmt->setUInt(2, equip->level());
    stmt->setUInt(3, equip->gem());
    stmt->setUInt(4, equip->pos());
    stmt->setUInt(5, equip->refining());
	stmt->setUInt(6, equip->refining_exp());
    stmt->setUInt64(7, equip->id());
    
    stmt->executeUpdate();
    stmt->close();
    result = true;
    equip->last_update_ts(TIME_NOW.get_time_t());
    
    return result;
}

bool equip_pkg::db_remove(uint64_t id){    
    bool result(false);
    SCOPED_CONNECT(scoped, connection);
    PREPARE_SQL(connection, stmt,"\
    DELETE \
    FROM `T_USER_PACKAGE_EQUIP` \
    WHERE `id` = ?;");
    
    stmt->setUInt64(1, id);
    
    stmt->executeUpdate();
    stmt->close();
    result = true;
    return result;    
}
