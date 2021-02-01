//
//  log_queue.h
//  qsmy-log
//
//  Created by 汪燕军 on 13-11-27.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __qsmy_log__log_queue__
#define __qsmy_log__log_queue__

#include "log_prefix.h"
#include "config.h"

namespace qsmy_log
{
#define BUFFER_SIZE 100
    class log_model;
    class log_queue
    {
    public:
        log_queue():
        stopped_(true),
        mutex_(),
        queue_(),
        work_thread_(),
        buffer_(),
        connection_(),
        last_table_("")
        {
            buffer_.reserve(BUFFER_SIZE);
        }
        
        ~log_queue()
        {
            stop();
        }
        
        void enque(boost::shared_ptr<log_model> log);
        void enque(std::vector<boost::shared_ptr<log_model> > & log);

        
        void start();
        
        void stop();
        
    private:
        bool stopped_;
        boost::mutex mutex_;
        std::queue<boost::shared_ptr<log_model> > queue_;
        boost::shared_ptr<boost::thread> work_thread_;
        std::vector<boost::shared_ptr<log_model> > buffer_;
        
        boost::shared_ptr<sql::Connection> connection_;
        
        std::string last_table_;
        
        void work();
        
        /**
         *	@brief	执行一个循环
         *
         *	@return	是否马上执行下一个循环
         */
        bool run_once();

    };
}

#endif /* defined(__qsmy_log__log_queue__) */
