//
//  sys_utility_model.h
//  qsmy-log
//
//  Created by 汪燕军 on 13-6-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __qsmy_log__sys_utility_model__
#define __qsmy_log__sys_utility_model__

#include "log_prefix.h"
#include "net/message.h"

namespace qsmy_log {
    class sys_utility_model{
    public:
        sys_utility_model(){
            
        }
        
        ~sys_utility_model(){}
        
        void save();
        
        static boost::shared_ptr<sys_utility_model> parse(const Json::Value & jLog);
        
    private:
        std::string date_;
        uint32_t user_id_;
        std::string column_;
    };
}

#endif /* defined(__qsmy_log__sys_utility_model__) */
