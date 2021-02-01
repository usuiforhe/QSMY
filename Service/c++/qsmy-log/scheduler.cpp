//
//  scheduler.cpp
//  qsmy-log
//
//  Created by 汪燕军 on 13-5-21.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "scheduler.h"
#include "config.h"
#include "archive_task.h"
using namespace qsmy_log;

scheduler::~scheduler(){
    stop();
    delete work_;
    if(thread_){
        delete thread_;
    }
}

void scheduler::start(){
    second_offset_ = SINGLETON(config).archive_hour * 3600 + SINGLETON(config).archive_minute * 60;
    
    thread_ = new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
    
    boost::posix_time::ptime now = TIME_NOW.get();
    boost::posix_time::time_duration duration;
#ifdef DEBUG
    s_log.out_debug("today second offset:" + boost::lexical_cast<std::string>(now.time_of_day().total_seconds()));
#endif
    if(now.time_of_day().total_seconds() < second_offset_)
    {
        duration = boost::posix_time::ptime(boost::gregorian::day_clock::local_day(), boost::posix_time::seconds(second_offset_)) - now;
    }
    else
    {
        duration = boost::posix_time::ptime(boost::gregorian::day_clock::local_day() + boost::gregorian::days(1), boost::posix_time::seconds(second_offset_)) - now;
    }
    timer_.expires_from_now(duration);
    timer_.async_wait(boost::bind(&scheduler::invoke, this, _1));
#ifdef DEBUG
    s_log.out_debug("timer timeup after seconds:" + boost::lexical_cast<std::string>(duration.total_seconds()));
#endif
}

void scheduler::stop(){
    timer_.cancel();
    io_service_.stop();
    if(thread_){
        thread_->join();
    }
}

void scheduler::invoke(const boost::system::error_code & error){
    if(error){
        return;
    }
    
    try{
        SINGLETON(archive_task).run();
    }
    catch (boost::exception& e)
    {
        s_log.out_error("pili_log::scheduler::invoke throw exception. what:" + boost::diagnostic_information(e));
        
    }
    catch (std::exception& e)
    {
        s_log.out_error("pili_log::scheduler::invoke throw exception. what:" + boost::diagnostic_information(e));
    }

    boost::posix_time::time_duration duration = boost::posix_time::ptime(boost::gregorian::day_clock::local_day() + boost::gregorian::days(1), boost::posix_time::seconds(second_offset_)) - TIME_NOW.get();
    
    timer_.expires_from_now(duration);
    timer_.async_wait(boost::bind(&scheduler::invoke, this, _1));
#ifdef DEBUG
    s_log.out_debug("timer timeup after seconds:" + boost::lexical_cast<std::string>(duration.total_seconds()));
#endif
}
