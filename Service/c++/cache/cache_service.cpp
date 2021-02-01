//
//  cache_service.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "cache_service.h"
#include "player_manager.h"
#include "handler/common_handler.h"
#include "handler/equip_handler.h"
#include "handler/item_handler.h"
#include "handler/soul_handler.h"
#include "handler/warrior_handler.h"
#include "handler/destiny_handler.h"
#include "handler/dress_handler.h"

using namespace cache;

cache_service::cache_service()
:connection_pool_(new latte::mysql_pool(SINGLETON(config).mysql_conn_pool_max)), stopped_(false)
{
    thread_pool_ = boost::make_shared<latte::thread_pool>(2, SINGLETON(config).handle_thread_count);
    thread_pool_->set_thread_exit_callback(boost::bind<void>(&cache_service::before_thread_exit, this));
    init();
}

cache_service::~cache_service(){
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

void cache_service::start(){
    stopped_ = false;
    sql::mysql::get_driver_instance()->threadInit();
    SINGLETON(player_manager).init();
    tcp_server_->start();
}

void cache_service::stop(){
    stopped_ = true;
    if(tcp_server_ != NULL){
        tcp_server_->close();
    }
    if(thread_pool_){
        thread_pool_->join();
    }
    SINGLETON(player_manager).dispose();    
}

void cache_service::send(latte::tcp_conn_ptr connection, latte::header_protobuf_message_ptr message){
    try {
        codec_->send(connection, message);
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "cache_service::send throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "cache_service::send throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());
    }
}

void cache_service::handle_message(latte::tcp_conn_ptr connection, latte::header_protobuf_message_ptr message){
#if TIME_STATIC
    thread_pool_->queue_work_item(boost::bind(&cache_service::wrap_handle, this, connection, message));
#else
    thread_pool_->queue_work_item(boost::bind(&latte::protobuf_dispatcher<latte::tcp_conn_ptr>::on_message, dispatcher_, connection, message));
#endif
}

void cache_service::wrap_handle(latte::tcp_conn_ptr connection, latte::header_protobuf_message_ptr message){
    clock_t t_start, t_end;
    t_start = clock();
    dispatcher_->on_message(connection, message);
    t_end = clock();
    
    std::stringstream & ss = latte::fast_stringstream::ss();
    ss << "request type is: " << message->message()->GetTypeName() << ". handled in " << (double)(t_end - t_start)*1000/CLOCKS_PER_SEC << "ms";
    s_log.out_string(ss.str());
}

//------------------------------ private ------------------------------
void cache_service::init(){
    codec_ = new latte::protobuf_codec();
    latte::on_tcp_message_t callback = boost::bind(&cache_service::handle_message, this, _1, _2);
    codec_->set_on_message(callback);
    
    dispatcher_ = new latte::protobuf_dispatcher<latte::tcp_conn_ptr>(boost::bind<void>(SINGLETON(default_handler), _1, _2));
    
    tcp_server_ = new latte::tcp_server(SINGLETON(config).port, false, SINGLETON(config).io_thread_count),    
    tcp_server_->on_receive(boost::bind<void>(&latte::protobuf_codec::on_message, codec_, _1));    
    
    codec_->register_type("cache.proGetEquip", 1);
    codec_->register_type("cache.proGetEquips", 2);
    codec_->register_type("cache.proGetEquipsByPos", 3);
    codec_->register_type("cache.proAddEquip", 4);
    codec_->register_type("cache.proAddEquips", 5);
    codec_->register_type("cache.proUpdateEquip", 6);
    codec_->register_type("cache.proUpdateEquips", 7);
    codec_->register_type("cache.proRemoveEquip", 8);
    codec_->register_type("cache.proRemoveEquips", 9);
    codec_->register_type("cache.proRes", 10);
    codec_->register_type("cache.proEquipRes", 11);
    codec_->register_type("cache.proEquipsRes", 12);
    codec_->register_type("cache.proGetItem", 13);
    codec_->register_type("cache.proGetItems", 14);
    codec_->register_type("cache.proAddItem", 15);
    codec_->register_type("cache.proAddItems", 16);
    codec_->register_type("cache.proRemoveItem", 17);
    codec_->register_type("cache.proRemoveItems", 18);
    codec_->register_type("cache.proItemRes", 19);
    codec_->register_type("cache.proItemsRes", 20);
    codec_->register_type("cache.proGetSoul", 21);
    codec_->register_type("cache.proGetSouls", 22);
    codec_->register_type("cache.proAddSoul", 23);
    codec_->register_type("cache.proAddSouls", 24);
    codec_->register_type("cache.proRemoveSoul", 25);
    codec_->register_type("cache.proRemoveSouls", 26);
    codec_->register_type("cache.proSoulRes", 27);
    codec_->register_type("cache.proSoulsRes", 28);
    codec_->register_type("cache.proGetWarrior", 29);
    codec_->register_type("cache.proGetWarriors", 30);
    codec_->register_type("cache.proAddWarrior", 31);
    codec_->register_type("cache.proUpdateWarrior", 32);
    codec_->register_type("cache.proRemoveWarrior", 33);
    codec_->register_type("cache.proRemoveWarriors", 34);
    codec_->register_type("cache.proWarriorRes", 35);
    codec_->register_type("cache.proWarriorsRes", 36);
    codec_->register_type("cache.proGetDestiny", 37);
    codec_->register_type("cache.proGetDestinies", 38);
    codec_->register_type("cache.proSaveDestiny", 39);
    codec_->register_type("cache.proImproveDestiny", 40);
    codec_->register_type("cache.proDestinyRes", 41);
    codec_->register_type("cache.proDestiniesRes", 42);
    codec_->register_type("cache.proDecomposeWarrior", 43);
    codec_->register_type("cache.proDecomposeWarriors", 44);
    codec_->register_type("cache.proUpdateWarriors", 45);        
    codec_->register_type("cache.proAsyncUpdate", 46);
    codec_->register_type("cache.proGetDress", 47);
    codec_->register_type("cache.proGetDresses", 48);
    codec_->register_type("cache.proGetDressesByWarrior", 49);
    codec_->register_type("cache.proAddDress", 50);
    codec_->register_type("cache.proUpdateDress", 51);
    codec_->register_type("cache.proRemoveDress", 52);
    codec_->register_type("cache.proDressRes", 53);
    codec_->register_type("cache.proDressesRes", 54);
    
    dispatcher_->register_callback<cache::proGetEquip>(boost::bind<void>(SINGLETON(get_equip_handler), _1, _2));
    dispatcher_->register_callback<cache::proGetEquips>(boost::bind<void>(SINGLETON(get_equips_handler), _1, _2));
    dispatcher_->register_callback<cache::proGetEquipsByPos>(boost::bind<void>(SINGLETON(get_equips_by_pos_handler), _1, _2));
    dispatcher_->register_callback<cache::proAddEquip>(boost::bind<void>(SINGLETON(add_equip_handler), _1, _2));
    dispatcher_->register_callback<cache::proAddEquips>(boost::bind<void>(SINGLETON(batch_add_equip_handler), _1, _2));
    dispatcher_->register_callback<cache::proUpdateEquip>(boost::bind<void>(SINGLETON(update_equip_handler), _1, _2));
    dispatcher_->register_callback<cache::proUpdateEquips>(boost::bind<void>(SINGLETON(batch_update_equip_handler), _1, _2));
    dispatcher_->register_callback<cache::proRemoveEquip>(boost::bind<void>(SINGLETON(remove_equip_handler), _1, _2));
    dispatcher_->register_callback<cache::proRemoveEquips>(boost::bind<void>(SINGLETON(batch_remove_equip_handler), _1, _2));
    
    dispatcher_->register_callback<cache::proGetItem>(boost::bind<void>(SINGLETON(get_item_handler), _1, _2));
    dispatcher_->register_callback<cache::proGetItems>(boost::bind<void>(SINGLETON(get_items_handler), _1, _2));
    dispatcher_->register_callback<cache::proAddItem>(boost::bind<void>(SINGLETON(add_item_handler), _1, _2));
    dispatcher_->register_callback<cache::proAddItems>(boost::bind<void>(SINGLETON(batch_add_item_handler), _1, _2));
    dispatcher_->register_callback<cache::proRemoveItem>(boost::bind<void>(SINGLETON(remove_item_handler), _1, _2));
    dispatcher_->register_callback<cache::proRemoveItems>(boost::bind<void>(SINGLETON(batch_remove_item_handler), _1, _2));
    
    dispatcher_->register_callback<cache::proGetSoul>(boost::bind<void>(SINGLETON(get_soul_handler), _1, _2));
    dispatcher_->register_callback<cache::proGetSouls>(boost::bind<void>(SINGLETON(get_souls_handler), _1, _2));
    dispatcher_->register_callback<cache::proAddSoul>(boost::bind<void>(SINGLETON(add_soul_handler), _1, _2));
    dispatcher_->register_callback<cache::proAddSouls>(boost::bind<void>(SINGLETON(batch_add_soul_handler), _1, _2));
    dispatcher_->register_callback<cache::proRemoveSoul>(boost::bind<void>(SINGLETON(remove_soul_handler), _1, _2));
    dispatcher_->register_callback<cache::proRemoveSouls>(boost::bind<void>(SINGLETON(batch_remove_soul_handler), _1, _2));
    
    dispatcher_->register_callback<cache::proGetWarrior>(boost::bind<void>(SINGLETON(get_warrior_handler), _1, _2));
    dispatcher_->register_callback<cache::proGetWarriors>(boost::bind<void>(SINGLETON(get_warriors_handler), _1, _2));
    dispatcher_->register_callback<cache::proAddWarrior>(boost::bind<void>(SINGLETON(add_warrior_handler), _1, _2));
    dispatcher_->register_callback<cache::proUpdateWarrior>(boost::bind<void>(SINGLETON(update_warrior_handler), _1, _2));
    dispatcher_->register_callback<cache::proRemoveWarrior>(boost::bind<void>(SINGLETON(remove_warrior_handler), _1, _2));
    dispatcher_->register_callback<cache::proRemoveWarriors>(boost::bind<void>(SINGLETON(batch_remove_warrior_handler), _1, _2));
    dispatcher_->register_callback<cache::proDecomposeWarrior>(boost::bind<void>(SINGLETON(decompose_warrior_handler), _1, _2));
    dispatcher_->register_callback<cache::proDecomposeWarriors>(boost::bind<void>(SINGLETON(batch_decompose_warrior_handler), _1, _2));
    dispatcher_->register_callback<cache::proUpdateWarriors>(boost::bind<void>(SINGLETON(batch_update_warrior_handler), _1, _2));
    
    dispatcher_->register_callback<cache::proGetDestiny>(boost::bind<void>(SINGLETON(get_destiny_handler), _1, _2));
    dispatcher_->register_callback<cache::proGetDestinies>(boost::bind<void>(SINGLETON(get_destinies_handler), _1, _2));
    dispatcher_->register_callback<cache::proSaveDestiny>(boost::bind<void>(SINGLETON(save_destiny_handler), _1, _2));
    dispatcher_->register_callback<cache::proImproveDestiny>(boost::bind<void>(SINGLETON(improve_destiny_handler), _1, _2));
    
    
    dispatcher_->register_callback<cache::proAsyncUpdate>(boost::bind<void>(SINGLETON(async_update_handler), _1, _2));
    
    dispatcher_->register_callback<cache::proGetDress>(boost::bind<void>(SINGLETON(get_dress_handler), _1, _2));
    dispatcher_->register_callback<cache::proGetDresses>(boost::bind<void>(SINGLETON(get_dresses_handler), _1, _2));
    dispatcher_->register_callback<cache::proGetDressesByWarrior>(boost::bind<void>(SINGLETON(get_dresses_by_warrior_handler), _1, _2));
    dispatcher_->register_callback<cache::proAddDress>(boost::bind<void>(SINGLETON(add_dress_handler), _1, _2));
    dispatcher_->register_callback<cache::proUpdateDress>(boost::bind<void>(SINGLETON(update_dress_handler), _1, _2));
    dispatcher_->register_callback<cache::proRemoveDress>(boost::bind<void>(SINGLETON(remove_dress_handler), _1, _2));
}

void cache_service::before_thread_exit()
{
    sql::mysql::get_driver_instance()->threadInit();
    sql::mysql::get_driver_instance()->threadEnd();
}
