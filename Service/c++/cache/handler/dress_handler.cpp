//
//  dress_handler.cpp
//  cache
//
//  Created by 汪燕军 on 14-3-4.
//  Copyright (c) 2014年 汪燕军. All rights reserved.
//

#include "dress_handler.h"
#include "protocol/common.pb.h"
#include "protocol/dress.pb.h"
#include "player_manager.h"
#include "dress/dress_pkg.h"
#include "cache_service.h"

using namespace cache;

void get_dress_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetDress> request = boost::static_pointer_cast<proGetDress>(message->message());
    boost::shared_ptr<proDressRes> response(new proDressRes);
    dress_ptr dress;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        dress = player->dress_pkg()->get(request->dress_id());
        if(dress){
            dress->copy_to_dress_res(response);
            response->set_code(OPT_SUCCESS);
        }
        else{
            response->set_code(DRESS_NOT_EXIST);
        }
        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void get_dresses_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetDresses> request = boost::static_pointer_cast<proGetDresses>(message->message());
    pro_dresses_res_ptr response(new proDressesRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        player->dress_pkg()->copy_to_dresses_res(response);
        response->set_code(OPT_SUCCESS);
        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void get_dresses_by_warrior_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetDressesByWarrior> request = boost::static_pointer_cast<proGetDressesByWarrior>(message->message());
    pro_dresses_res_ptr response(new proDressesRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        for(BOOST_AUTO(pos,request->warrior_list().begin());pos != request->warrior_list().end(); ++pos){
            boost::shared_ptr<std::vector<dress_ptr> > list(player->dress_pkg()->get_by_warrior(*pos));
            if(list){
                BOOST_FOREACH(dress_ptr dress, *list){
                    dress->copy_to_proto(response->add_dresses());
                }
            }
        }
        response->set_code(OPT_SUCCESS);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void add_dress_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proAddDress> request = boost::static_pointer_cast<proAddDress>(message->message());
    pro_dress_res_ptr response(new proDressRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        dress_ptr dress(new cache::dress(request->dress()));
        error_code code = player->dress_pkg()->add(dress);
        if(code == OPT_SUCCESS){
            dress->copy_to_dress_res(response);
        }
        response->set_code(code);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void update_dress_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proUpdateDress> request = boost::static_pointer_cast<proUpdateDress>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        dress_ptr dress(new cache::dress(request->dress()));
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        error_code code = player->dress_pkg()->update(dress);
        response->set_code(code);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void remove_dress_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proRemoveDress> request = boost::static_pointer_cast<proRemoveDress>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->dress_pkg()->remove(request->dress_id()));
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}
