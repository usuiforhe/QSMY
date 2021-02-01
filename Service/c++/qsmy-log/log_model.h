//
//  log_model.h
//  qsmy-log
//
//  Created by 汪燕军 on 13-5-21.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __qsmy_log__log_model__
#define __qsmy_log__log_model__

#include "log_prefix.h"
#include "config.h"

namespace qsmy_log{
    class log_model{
    public:
        
        ~log_model(){
            
        }
        
        void setParams(sql::PreparedStatement * stmt);
        
        static boost::shared_ptr<log_model> parse(const Json::Value & jLog);
    private:
        std::string trans_id_;
        uint8_t sequence_;
        uint8_t category_;
        uint32_t user_id_;
        std::string title_;
        std::string content_;
        uint16_t operate_id_;
        uint32_t log_ts_;
        uint32_t related1_;
        uint32_t related2_;
        uint32_t related3_;
        uint32_t related4_;
        bool user_call_;
        
        log_model();        
    };
}

#endif /* defined(__qsmy_log__log_model__) */
