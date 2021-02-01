//
//  battle_service.cpp
//  battle
//
//  Created by wangyanjun on 13-3-27.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "battle_service.h"
#include "config.h"
#include "default_handler.h"
#include "create_battle_handler.h"
//#include "game/game_context.h"

using namespace battle;

battle_service::battle_service():thread_pool_(2, S_CONFIG.get_battle_thread_count()){
    init();
}

battle_service::~battle_service(){
    if(tcp_server_ != NULL){
        delete tcp_server_;
    }
    if(codec_ != NULL){
        delete codec_;
    }
    if(dispatcher_ != NULL){
        delete dispatcher_;
    }
}

void battle_service::start(){
    //S_GAME_CONTEXT.start();
    tcp_server_->start();
}

void battle_service::stop(){
    //S_GAME_CONTEXT.stop();
    thread_pool_.abort();
    if(tcp_server_ != NULL){
        tcp_server_->close();
    }
}

void battle_service::send(latte::tcp_conn_ptr connection, latte::header_protobuf_message_ptr message){
    try {
        codec_->send(connection, message);
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "battle_service::send throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "battle_service::send throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());
    }
}

void battle_service::handle_message(latte::tcp_conn_ptr connection, latte::header_protobuf_message_ptr message){
    thread_pool_.queue_work_item(boost::bind(&latte::protobuf_dispatcher<latte::tcp_conn_ptr>::on_message, dispatcher_, connection, message));
}

//------------------------------ private ------------------------------
void battle_service::init(){
    codec_ = new latte::protobuf_codec();
;
    latte::on_tcp_message_t callback = boost::bind(&battle_service::handle_message, this, _1, _2);
    
    codec_->set_on_message(callback);
    
    dispatcher_ = new latte::protobuf_dispatcher<latte::tcp_conn_ptr>(boost::bind<void>(S_DEFAULT_HANDLER, _1, _2));
    
    tcp_server_ = new latte::tcp_server(S_CONFIG.get_port(), false, S_CONFIG.get_io_thread_count()),
    
    tcp_server_->on_receive(boost::bind<void>(&latte::protobuf_codec::on_message, codec_, _1));
    
    codec_->register_type("battle.scene_meta_data", 1);
    codec_->register_type("battle.report_meta_data", 2);
    codec_->register_type("battle.hp_report_meta_data", 3);
    
    dispatcher_->register_callback<battle::scene_meta_data >(boost::bind<void>(S_CREATE_BATTLE_HANDLER, _1, _2));
}
