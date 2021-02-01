//
//  sys_utility_model.cpp
//  qsmy-log
//
//  Created by 汪燕军 on 13-6-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "sys_utility_model.h"
#include "config.h"
using namespace qsmy_log;

void sys_utility_model::save(){
    CONNECT_DB(conn);
    PREPARE_SQL(conn, stmt, "INSERT INTO `T_LOG_SYS_UTIL`   \
                SET `date` = ?,                             \
                `user_id` = ?,                              \
                " + column_ + " = 1                         \
                ON DUPLICATE KEY                            \
                UPDATE " + column_ + " = 1;"
                );
    stmt->setDateTime(1, date_);
    stmt->setUInt(2, user_id_);
    stmt->executeUpdate();
    stmt->close();
    conn->close();
}

boost::shared_ptr<sys_utility_model> sys_utility_model::parse(const Json::Value & jLog){
    try{
        std::string date = jLog["date"].asString();
        uint32_t user_id = jLog["userId"].asUInt();
        std::string column = jLog["sys"].asString();
        
        boost::shared_ptr<sys_utility_model> model(new sys_utility_model);
        model->date_ = date;
        model->user_id_ = user_id;
        model->column_ = column;
        
        return model;
    }
    catch(...){
        return boost::shared_ptr<sys_utility_model>();
    }
}
