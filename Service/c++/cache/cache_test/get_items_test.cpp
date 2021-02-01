//
//  get_items_test.cpp
//  cache
//
//  Created by 汪燕军 on 13-9-4.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "get_items_test.h"
using namespace cache_test;
using namespace cache;

message_sender::message_sender()
{    
    client_ = boost::shared_ptr<latte::tcp_client>(latte::tcp_client::create("10.10.1.152", 12333));
    client_->connect();
    
    codec_ = boost::shared_ptr<latte::protobuf_codec>(new latte::protobuf_codec());
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
}

message_sender::~message_sender()
{
    client_->close();
}

latte::header_protobuf_message_ptr message_sender::send_and_wait_response(latte::header_protobuf_message_ptr message)
{    
    boost::shared_ptr<latte::message> response = client_->send_and_wait_response(codec_->to_message(message));
    
    if(response)
    {
        latte::header_protobuf_message_ptr buf = codec_->parse(response);
        return buf;
    }
    return latte::header_protobuf_message_ptr();
}

item_test::item_test()
:thread_pool_(10, 100)
,sender_()
{
    
}

item_test::~item_test()
{
    
}

void item_test::start()
{
    for(uint32_t i = 1; i <= 0; ++i)
    {
        thread_pool_.queue_work_item(boost::bind(&item_test::get_items, this, i));
    }
    
    std::stringstream & ss = latte::fast_stringstream::ss();
    boost::unordered_map<int, int> map;
    ss << "size: " << map.size() << ", empty:" << map.empty() << std::endl;
    map.reserve(100);
    ss << "size: " << map.size() << ", empty:" << map.empty() << std::endl;
    s_log.out_string(ss.str());
}

void item_test::stop()
{
    thread_pool_.join();
}

void item_test::get_items(uint32_t user_id)
{
    boost::shared_ptr<cache::proGetItems> getItems = boost::shared_ptr<cache::proGetItems>(new cache::proGetItems);
    getItems->set_user_id(user_id);
    latte::header_protobuf_message_ptr msg = latte::header_protobuf_message_ptr(new latte::header_protobuf_message(getItems, 0, false));
    
    latte::header_protobuf_message_ptr response = sender_.send_and_wait_response(msg);
    
    if(response)
    {
        boost::shared_ptr<cache::proItemsRes> res = boost::static_pointer_cast<cache::proItemsRes>(response->message());
        std::stringstream & ss = latte::fast_stringstream::ss();
        ss << "items of user " << user_id << " is:" << std::endl;
        
        for(BOOST_AUTO(pos,res->items().begin());pos != res->items().end(); ++pos){
            ss << "\titem_id:" << (uint32_t)(pos->item_id()) << ", num:" << (uint32_t)pos->num() <<std::endl;
        }
        
        s_log.out_string(ss.str());
    }
}



