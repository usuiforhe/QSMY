//
//  log_service.cpp
//  qsmy-log
//
//  Created by 汪燕军 on 13-5-21.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "log_service.h"
#include "log_model.h"
#include "sys_utility_model.h"

using namespace qsmy_log;

log_service::log_service()
: udp_server_(NULL)
, thread_pool_(boost::make_shared<latte::thread_pool>(2, SINGLETON(config).handle_thread_count))
, scheduler_()
, log_queue_(){
    thread_pool_->set_thread_exit_callback(boost::bind<void>(&log_service::before_thread_exit, this));
}

log_service::~log_service(){    
    if(udp_server_ != NULL){
        delete udp_server_;
    }
}

void log_service::start(){
    log_queue_.start();
    latte::udp_server::on_receive_t callback = boost::bind(&log_service::handle_message, this, _1, _2);
    udp_server_ = new latte::udp_server(SINGLETON(config).port, callback);
    scheduler_.start();
}

void log_service::stop(){
    if(udp_server_ != NULL){
        udp_server_->close();
    }
    scheduler_.stop();
    
    thread_pool_->join();
    log_queue_.stop();
}

void log_service::handle_message(const boost::asio::ip::udp::endpoint &remote_endpoint, boost::shared_ptr<latte::message> message){    
    try{
        Json::Value root;
        Json::Reader reader;
        boost::asio::streambuf buf;
        buf.sputn(message->get_data().get(), message->get_size());
        std::istream stream(&buf);
        if(!reader.parse(stream, root)){
            return;
        }
        if(!root.isMember("target") || !(root["target"].isInt() || root["target"].isUInt())){
            return;
        }
        uint32_t target = root["target"].asUInt();
        switch(target){
            case target_log:
            {
                Json::Value jLog = root["log"];
                boost::shared_ptr<log_model> model = log_model::parse(jLog);
                if(model){
                    log_queue_.enque(model);
                }
                break;
            }
            case target_logs:
            {
                Json::Value jLogs = root["logs"];
                
                std::vector<boost::shared_ptr<log_model> > vec;
                
                for(BOOST_AUTO(pos, jLogs.begin()); pos != jLogs.end(); ++pos){
                    boost::shared_ptr<log_model> model = log_model::parse(*pos);
                    if(model)
                    {
                        vec.push_back(model);
                    }
                }
                
                log_queue_.enque(vec);
                break;
            }
            case target_sys_utility:
            {
                boost::shared_ptr<sys_utility_model> model = sys_utility_model::parse(root);
                if(model){
                    thread_pool_->queue_work_item(boost::bind(&sys_utility_model::save, model));
                }
                break;
            }
        }    }
    catch(...){
    }
}

void log_service::before_thread_exit()
{
    sql::mysql::get_driver_instance()->threadInit();
    sql::mysql::get_driver_instance()->threadEnd();
}
