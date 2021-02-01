//
//  log_model.cpp
//  qsmy-log
//
//  Created by 汪燕军 on 13-5-21.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "log_model.h"

using namespace qsmy_log;

log_model::log_model(){
    
}

void log_model::setParams(sql::PreparedStatement *stmt)
{
    stmt->setString(1, trans_id_);
    stmt->setUInt(2, sequence_);
    stmt->setUInt(3, category_);
    stmt->setUInt(4, user_id_);
    stmt->setString(5, title_);
    stmt->setString(6, content_);
    stmt->setUInt(7, operate_id_);
    stmt->setUInt(8, log_ts_);
    stmt->setUInt(9, related1_);
    stmt->setUInt(10, related2_);
    stmt->setUInt(11, related3_);
    stmt->setUInt(12, related4_);
    stmt->setBoolean(13, user_call_);
}

boost::shared_ptr<log_model> log_model::parse(const Json::Value & jLog){
    try{
        std::string trans_id = jLog["transId"].asString();
        uint8_t sequence = static_cast<uint8_t>(jLog["seq"].asUInt());
        uint8_t category = static_cast<uint8_t>(jLog["category"].asUInt());
        uint32_t user_id = jLog["userId"].asUInt();
        std::string title = jLog["title"].asString();
        std::string content = jLog["content"].asString();
        uint16_t operate_id = static_cast<uint16_t>(jLog["operateId"].asUInt());
        uint32_t log_ts = jLog["logTs"].asUInt();
        uint32_t related1 = jLog["related1"].asUInt();
        uint32_t related2 = jLog["related2"].asUInt();
        uint32_t related3 = jLog["related3"].asUInt();
        uint32_t related4 = jLog["related4"].asUInt();
        bool user_call = jLog["userCall"].asBool();
        
        boost::shared_ptr<log_model> model(new log_model);
        model->trans_id_ = trans_id;
        model->sequence_ = sequence;
        model->category_ = category;
        model->user_id_ = user_id;
        model->title_= title;
        model->content_ = content;
        model->operate_id_ = operate_id;
        model->log_ts_ = log_ts;
        model->related1_ = related1;
        model->related2_ = related2;
        model->related3_ = related3;
        model->related4_ = related4;
        model->user_call_ = user_call;
        
        return model;
    }
    catch(...){
        return boost::shared_ptr<log_model>();
    }
}
