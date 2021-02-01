//
//  scheduler.h
//  qsmy-log
//
//  Created by 汪燕军 on 13-5-21.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __qsmy_log__scheduler__
#define __qsmy_log__scheduler__

#include "log_prefix.h"

namespace qsmy_log{
    class scheduler{
    public:
        scheduler()
        :io_service_(),
        work_(new boost::asio::io_service::work(io_service_)),
        timer_(io_service_),
        second_offset_(0){
            
        }
        
        ~scheduler();
        
        void start();
        
        void stop();
        
        void invoke(const boost::system::error_code & error);
        
    private:
        boost::asio::io_service io_service_;
		boost::asio::io_service::work * work_;
		boost::asio::deadline_timer timer_;
        uint32_t second_offset_;
		boost::thread * thread_;
        
    };
}

#endif /* defined(__qsmy_log__scheduler__) */
