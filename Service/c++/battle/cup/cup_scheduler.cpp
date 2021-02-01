//
//  cup_scheduler.cpp
//  battle
//
//  Created by 汪燕军 on 13-4-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "cup_scheduler.h"
#include "cup.h"
#include "config.h"
#include "mysql_driver.h"
using namespace battle;

cup_scheduler::~cup_scheduler(){
    delete work_;
    delete thread_;
}

void cup_scheduler::start(){
    if(!S_CONFIG.get_enable_cup())
    {
        return;
    }
    
    second_offset_ = S_CONFIG.get_cup_run_hour() * 3600 + S_CONFIG.get_cup_run_minute() * 60;
    
    thread_ = new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
    
    boost::posix_time::ptime now = TIME_NOW.get();    
    boost::posix_time::time_duration duration;
#if DEBUG
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
    timer_.async_wait(boost::bind(&cup_scheduler::invoke, this, _1));
#if DEBUG
    s_log.out_debug("timer timeup after seconds:" + boost::lexical_cast<std::string>(duration.total_seconds()));
#endif
}

void cup_scheduler::stop(){
    if(!S_CONFIG.get_enable_cup())
    {
        return;
    }
    timer_.cancel();
    io_service_.stop();
    thread_->join();
}

void cup_scheduler::invoke(const boost::system::error_code & error){
    if(error){
        return;
    }
    
    std::tm today = boost::gregorian::to_tm(boost::gregorian::day_clock::local_day());
    bool success(false);
    while(!success){
        try{
            cup c(std::mktime(&today));
            c.init();
            c.run();
            success = true;
        }
        catch (boost::exception& e)
        {
            s_log.out_error("cup_scheduler::invoke throw exception. what:" + boost::diagnostic_information(e));            
        }
        catch (std::exception& e)
        {
            s_log.out_error("cup_scheduler::invoke throw exception. what:" + std::string(e.what()));
        }
        catch(...){
            s_log.out_error("cup_scheduler::invoke throw exception.");
        }
    }
    
    sql::mysql::get_mysql_driver_instance()->threadEnd();
    
    boost::posix_time::time_duration duration = boost::posix_time::ptime(boost::gregorian::day_clock::local_day() + boost::gregorian::days(1), boost::posix_time::seconds(second_offset_)) - TIME_NOW.get();
    
    timer_.expires_from_now(duration);
    timer_.async_wait(boost::bind(&cup_scheduler::invoke, this, _1));
    s_log.out_string("cup next run after seconds:" + boost::lexical_cast<std::string>(duration.total_seconds()));
}
