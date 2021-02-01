//
//  item_handler.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "item_handler.h"
#include "protocol/common.pb.h"
#include "protocol/item.pb.h"
#include "player_manager.h"
#include "item/item_pkg.h"
#include "item/item.h"
#include "cache_service.h"

using namespace cache;

void get_item_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetItem> request = boost::static_pointer_cast<proGetItem>(message->message());
    boost::shared_ptr<proItemRes> response(new proItemRes);
    item_ptr item;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY        
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        item = player->item_pkg()->get(request->item_id());
        item->copy_to_item_res(response);
        response->set_code(OPT_SUCCESS);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void get_items_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetItems> request = boost::static_pointer_cast<proGetItems>(message->message());
    pro_items_res_ptr response(new proItemsRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY        
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        player->item_pkg()->copy_to_items_res(response);
        response->set_code(OPT_SUCCESS);        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void add_item_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proAddItem> request = boost::static_pointer_cast<proAddItem>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY        
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        item_ptr item(new cache::item(request->item()));
        error_code code = player->item_pkg()->add(item);
        response->set_code(code);        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void batch_add_item_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proAddItems> request = boost::static_pointer_cast<proAddItems>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        std::vector<item_ptr> list;
        list.reserve(request->items_size());
        
        for(BOOST_AUTO(pos,request->items().begin());pos != request->items().end(); ++pos){
            list.push_back(item_ptr(new cache::item(*pos)));
        }
        
        SERVICE_HANDLER_TRY        
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        error_code code = player->item_pkg()->add(list);        
        response->set_code(code);        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void remove_item_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proRemoveItem> request = boost::static_pointer_cast<proRemoveItem>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        item_ptr item(new cache::item(request->item()));
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->item_pkg()->remove(item));
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void batch_remove_item_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proRemoveItems> request = boost::static_pointer_cast<proRemoveItems>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        std::vector<item_ptr> list;
        list.reserve(request->items_size());
        
        for(BOOST_AUTO(pos,request->items().begin());pos != request->items().end(); ++pos){
            list.push_back(item_ptr(new cache::item(*pos)));
        }
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->item_pkg()->remove(list));
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

