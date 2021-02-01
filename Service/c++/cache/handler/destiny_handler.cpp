//
//  destiny_handler.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "destiny_handler.h"
#include "protocol/common.pb.h"
#include "protocol/destiny.pb.h"
#include "player_manager.h"
#include "destiny/destiny_pkg.h"
#include "destiny/destiny.h"
#include "cache_service.h"
#include "warrior/warrior_pkg.h"
#include "soul/soul_pkg.h"

using namespace cache;

void get_destiny_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetDestiny> request = boost::static_pointer_cast<proGetDestiny>(message->message());
    boost::shared_ptr<proDestinyRes> response(new proDestinyRes);
    destiny_ptr destiny;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        destiny = player->destiny_pkg()->get(request->destiny_id());
        if(destiny){
            destiny->copy_to_destiny_res(response);
            response->set_code(OPT_SUCCESS);
        }
        else{
            response->set_code(DESTINY_NOT_EXIST);
        }
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void get_destinies_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetDestinies> request = boost::static_pointer_cast<proGetDestinies>(message->message());
    pro_destinies_res_ptr response(new proDestiniesRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        uint32_t type(0);
        if(request->has_type()){
            type = request->type();
        }
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        player->destiny_pkg()->copy_to_destinies_res(response, type);
        response->set_code(OPT_SUCCESS);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void save_destiny_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proSaveDestiny> request = boost::static_pointer_cast<proSaveDestiny>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        destiny_ptr destiny(new cache::destiny(request->destiny()));
        error_code code = player->destiny_pkg()->save(destiny);
        response->set_code(code);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void improve_destiny_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proImproveDestiny> request = boost::static_pointer_cast<proImproveDestiny>(message->message());
    
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        std::vector<soul_ptr> list;
        list.reserve(request->consume_souls_size());
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        bool checked(true);
        soul_pkg_ptr spkg = player->soul_pkg();
        
        for(BOOST_AUTO(pos, request->consume_souls().begin()); pos != request->consume_souls().end(); ++pos){
            if(spkg->get(pos->soul_id())->num() < pos->num()){
                response->set_code(NOT_ENOUGH_SOUL);
                checked = false;
                break;
            }
            soul_ptr soul = boost::make_shared<cache::soul>(0, pos->soul_id(), pos->num());
            list.push_back(soul);
        }
        
        if(checked){
            destiny_ptr destiny = boost::make_shared<cache::destiny>(request->destiny());
            error_code code = player->destiny_pkg()->save(destiny);
            if(code == OPT_SUCCESS)
            {
                spkg->remove(list);
            }
            
            response->set_code(code);
        }
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}
