//
//  common_handler.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-8.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "common_handler.h"
#include "protocol/common.pb.h"
#include "cache_service.h"
#include "player_manager.h"
#include "common/updater.h"

using namespace cache;

void default_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proRes> response(new proRes);
    response->set_code(INVALID_ARGUMENT);
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void async_update_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message)
{
    boost::shared_ptr<proAsyncUpdate> request = boost::static_pointer_cast<proAsyncUpdate>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        player->updater()->request_update(request);
        response->set_code(OPT_SUCCESS);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

