//
//  soul_handler.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "soul_handler.h"
#include "protocol/common.pb.h"
#include "protocol/soul.pb.h"
#include "player_manager.h"
#include "soul/soul_pkg.h"
#include "soul/soul.h"
#include "cache_service.h"

using namespace cache;

void get_soul_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetSoul> request = boost::static_pointer_cast<proGetSoul>(message->message());
    boost::shared_ptr<proSoulRes> response(new proSoulRes);
    soul_ptr soul;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        soul = player->soul_pkg()->get(request->soul_id());
        soul->copy_to_soul_res(response);
        response->set_code(OPT_SUCCESS);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void get_souls_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetSouls> request = boost::static_pointer_cast<proGetSouls>(message->message());
    pro_souls_res_ptr response(new proSoulsRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        std::vector<uint32_t> list;
        list.reserve(request->souls_size());        
        for(BOOST_AUTO(pos,request->souls().begin());pos != request->souls().end(); ++pos){
            list.push_back(*pos);
        }
        
        player->soul_pkg()->copy_to_souls_res(response, list);
        
        response->set_code(OPT_SUCCESS);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void add_soul_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proAddSoul> request = boost::static_pointer_cast<proAddSoul>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        soul_ptr soul(new cache::soul(request->soul()));
        error_code code = player->soul_pkg()->add(soul);
        response->set_code(code);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void batch_add_soul_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proAddSouls> request = boost::static_pointer_cast<proAddSouls>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        std::vector<soul_ptr> list;
        list.reserve(request->souls_size());
        
        for(BOOST_AUTO(pos,request->souls().begin());pos != request->souls().end(); ++pos){
            list.push_back(soul_ptr(new cache::soul(*pos)));
        }
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        error_code code = player->soul_pkg()->add(list);
        response->set_code(code);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void remove_soul_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proRemoveSoul> request = boost::static_pointer_cast<proRemoveSoul>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        soul_ptr soul(new cache::soul(request->soul()));
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->soul_pkg()->remove(soul));
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void batch_remove_soul_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proRemoveSouls> request = boost::static_pointer_cast<proRemoveSouls>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        std::vector<soul_ptr> list;
        list.reserve(request->souls_size());
        
        for(BOOST_AUTO(pos,request->souls().begin());pos != request->souls().end(); ++pos){
            list.push_back(soul_ptr(new cache::soul(*pos)));
        }
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->soul_pkg()->remove(list));
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

