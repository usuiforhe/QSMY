//
//  equip_handler.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-8.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "equip_handler.h"
#include "protocol/common.pb.h"
#include "protocol/equip.pb.h"
#include "player_manager.h"
#include "equip/equip_pkg.h"
#include "equip/equip.h"
#include "cache_service.h"

using namespace cache;

void get_equip_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetEquip> request = boost::static_pointer_cast<proGetEquip>(message->message());
    boost::shared_ptr<proEquipRes> response(new proEquipRes);
    equip_ptr equip;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        equip = player->equip_pkg()->get(request->id());
        if(equip){
            equip->copy_to_equip_res(response);
            response->set_code(OPT_SUCCESS);
        }
        else{
            response->set_code(NO_SUCH_EQUIP);
        }
        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void get_equips_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){    
    boost::shared_ptr<proGetEquips> request = boost::static_pointer_cast<proGetEquips>(message->message());
    pro_equips_res_ptr response(new proEquipsRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        player->equip_pkg()->copy_to_equips_res(response);
        response->set_code(OPT_SUCCESS);
        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void get_equips_by_pos_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetEquipsByPos> request = boost::static_pointer_cast<proGetEquipsByPos>(message->message());
    pro_equips_res_ptr response(new proEquipsRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY        
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        for(BOOST_AUTO(pos,request->pos_list().begin());pos != request->pos_list().end(); ++pos){
            boost::shared_ptr<std::vector<equip_ptr> > list(player->equip_pkg()->get_by_pos(*pos));
            if(list){
                BOOST_FOREACH(equip_ptr equip, *list){
                    equip->copy_to_proto(response->add_equips());
                }
            }
        }
        response->set_code(OPT_SUCCESS);        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void add_equip_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proAddEquip> request = boost::static_pointer_cast<proAddEquip>(message->message());
    pro_equip_res_ptr response(new proEquipRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY        
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        equip_ptr equip(new cache::equip(request->equip()));
        error_code code = player->equip_pkg()->add(equip);
        if(code == OPT_SUCCESS){
            equip->copy_to_equip_res(response);
        }
        response->set_code(code);        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));    
}

void batch_add_equip_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proAddEquips> request = boost::static_pointer_cast<proAddEquips>(message->message());
    pro_equips_res_ptr response(new proEquipsRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        std::vector<equip_ptr> list;
        list.reserve(request->equips_size());
        
        for(BOOST_AUTO(pos,request->equips().begin());pos != request->equips().end(); ++pos){
            list.push_back(equip_ptr(new cache::equip(*pos)));
        }
        
        SERVICE_HANDLER_TRY        
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        error_code code = player->equip_pkg()->add(list);
        
        if(code == OPT_SUCCESS){
            BOOST_FOREACH(equip_ptr equip, list){
                equip->copy_to_proto(response->add_equips());
            }
        }
        
        response->set_code(code);        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void update_equip_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proUpdateEquip> request = boost::static_pointer_cast<proUpdateEquip>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        equip_ptr equip(new cache::equip(request->equip()));
        
        SERVICE_HANDLER_TRY        
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        error_code code = player->equip_pkg()->update(equip);
        response->set_code(code);        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void batch_update_equip_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proUpdateEquips> request = boost::static_pointer_cast<proUpdateEquips>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        std::vector<equip_ptr> list;
        list.reserve(request->equips_size());
        
        for(BOOST_AUTO(pos,request->equips().begin());pos != request->equips().end(); ++pos){
            list.push_back(equip_ptr(new cache::equip(*pos)));
        }
        
        SERVICE_HANDLER_TRY        
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->equip_pkg()->update(list));        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void remove_equip_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proRemoveEquip> request = boost::static_pointer_cast<proRemoveEquip>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY        
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->equip_pkg()->remove(request->equip_id()));
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void batch_remove_equip_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proRemoveEquips> request = boost::static_pointer_cast<proRemoveEquips>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        std::vector<uint64_t> list;
        list.reserve(request->equip_ids_size());
        for(BOOST_AUTO(pos,request->equip_ids().begin());pos != request->equip_ids().end(); ++pos){
            list.push_back(*pos);
        }
        
        SERVICE_HANDLER_TRY        
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->equip_pkg()->remove(list));        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));    
}
