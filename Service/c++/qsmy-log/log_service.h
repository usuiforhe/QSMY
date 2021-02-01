//
//  log_service.h
//  qsmy-log
//
//  Created by 汪燕军 on 13-5-21.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __qsmy_log__log_service__
#define __qsmy_log__log_service__

#include "log_prefix.h"
#include "net/udp_server.h"
#include "config.h"
#include "net/message.h"
#include "thread/thread_pool.h"
#include "scheduler.h"
#include "log_queue.h"

namespace qsmy_log {
    class log_service{
    public:
        /**
         *	@brief	构造函数
         *
         *	@return
         */
        log_service();

        /**
         *	@brief	析构函数
         *
         *	@return
         */
        ~log_service();
        
        /**
         *	@brief	开始
         */
        void start();
        
        /**
         *	@brief	停止
         */
        void stop();
        
        /**
         *	@brief	处理消息
         *
         *	@param 	remote_endpoint 	远程端口
         *	@param 	message 	消息
         */
        void handle_message(const boost::asio::ip::udp::endpoint & remote_endpoint, boost::shared_ptr<latte::message> message);
        
        void before_thread_exit();
        
    private:
        latte::udp_server * udp_server_;
        boost::shared_ptr<latte::thread_pool> thread_pool_;
        scheduler scheduler_;
        log_queue log_queue_;
        
        typedef enum target_type{
            target_log = 1,
            target_logs = 2,
            target_sys_utility = 3
        } target_type;
    };
}

#endif /* defined(__qsmy_log__log_service__) */
