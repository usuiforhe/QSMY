//
//  cup_scheduler.h
//  battle
//
//  Created by 汪燕军 on 13-4-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__cup_scheduler__
#define __battle__cup_scheduler__

#include "battle_prefix.h"

namespace battle{
    class cup_scheduler{
    public:
        cup_scheduler()
        :io_service_(),
        work_(new boost::asio::io_service::work(io_service_)),
        timer_(io_service_),
        second_offset_(0){
            
        }
        
        ~cup_scheduler();
        
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

#define S_CUP_SCHEDULER latte::singleton<cup_scheduler>::instance()

#endif /* defined(__battle__cup_scheduler__) */
