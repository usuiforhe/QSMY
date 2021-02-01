//
//  warrior_handler.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "warrior_handler.h"
#include "protocol/common.pb.h"
#include "protocol/warrior.pb.h"
#include "player_manager.h"
#include "warrior/warrior_pkg.h"
#include "warrior/warrior.h"
#include "soul/soul_pkg.h"
#include "cache_service.h"

using namespace cache;

void get_warrior_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetWarrior> request = boost::static_pointer_cast<proGetWarrior>(message->message());
    boost::shared_ptr<proWarriorRes> response(new proWarriorRes);
    warrior_ptr warrior;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        warrior = player->warrior_pkg()->get(request->sid());
        if(warrior){
            warrior->copy_to_warrior_res(response);
            response->set_code(OPT_SUCCESS);
        }
        else{
            response->set_code(WARRIOR_NOT_EXIST);
        }
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void get_warriors_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proGetWarriors> request = boost::static_pointer_cast<proGetWarriors>(message->message());
    boost::shared_ptr<proWarriorsRes> response(new proWarriorsRes);
    warrior_ptr warrior;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        if(request->sids_size() == 0){
            player->warrior_pkg()->copy_to_warriors_res(response);
        }
        else{            
            for(BOOST_AUTO(pos,request->sids().begin());pos != request->sids().end(); ++pos){
                warrior_ptr warrior = player->warrior_pkg()->get(*pos);
                if(warrior){
                    warrior->copy_to_proto(response->add_warriors());
                }
            }
        }
        response->set_code(OPT_SUCCESS);
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void add_warrior_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proAddWarrior> request = boost::static_pointer_cast<proAddWarrior>(message->message());
    boost::shared_ptr<proRes> response(new proRes);
    warrior_ptr warrior;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        warrior_ptr warrior = boost::make_shared<cache::warrior>(request->warrior());
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->warrior_pkg()->add(warrior));
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void update_warrior_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proUpdateWarrior> request = boost::static_pointer_cast<proUpdateWarrior>(message->message());
    boost::shared_ptr<proRes> response(new proRes);
    warrior_ptr warrior;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        warrior_ptr warrior = boost::make_shared<cache::warrior>(request->warrior());
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->warrior_pkg()->update(warrior));
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void batch_update_warrior_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proUpdateWarriors> request = boost::static_pointer_cast<proUpdateWarriors>(message->message());
    pro_res_ptr response(new proRes);
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        std::vector<warrior_ptr> list;
        list.reserve(request->warriors_size());
        
        for(BOOST_AUTO(pos,request->warriors().begin());pos != request->warriors().end(); ++pos){
            list.push_back(warrior_ptr(new cache::warrior(*pos)));
        }
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->warrior_pkg()->update(list));
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void remove_warrior_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proRemoveWarrior> request = boost::static_pointer_cast<proRemoveWarrior>(message->message());
    boost::shared_ptr<proRes> response(new proRes);
    warrior_ptr warrior;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->warrior_pkg()->remove(request->sid()));
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));    
}

void batch_remove_warrior_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){    
    boost::shared_ptr<proRemoveWarriors> request = boost::static_pointer_cast<proRemoveWarriors>(message->message());
    boost::shared_ptr<proRes> response(new proRes);
    warrior_ptr warrior;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        std::vector<uint32_t> list;
        list.reserve(request->sids_size());
        
        for(BOOST_AUTO(pos, request->sids().begin()); pos != request->sids().end(); ++pos){
            list.push_back(*pos);
        }
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        response->set_code(player->warrior_pkg()->remove(list));
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void decompose_warrior_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proDecomposeWarrior> request = boost::static_pointer_cast<proDecomposeWarrior>(message->message());
    boost::shared_ptr<proRes> response(new proRes);
    warrior_ptr warrior;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        
        SCOPED_LOCK(lock, player->mutex());
        
        if(!player->warrior_pkg()->get(request->warrior().sid())){
            response->set_code(WARRIOR_NOT_EXIST);
        }
        else{
            error_code code = player->soul_pkg()->add(boost::make_shared<soul>(0, request->warrior().sid(), request->warrior().soul_num()));
            if(code == OPT_SUCCESS){
                player->warrior_pkg()->remove(request->warrior().sid());
            }
            response->set_code(code);
        }
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}

void batch_decompose_warrior_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    boost::shared_ptr<proDecomposeWarriors> request = boost::static_pointer_cast<proDecomposeWarriors>(message->message());
    boost::shared_ptr<proRes> response(new proRes);
    warrior_ptr warrior;
    if(!request){
        response->set_code(INVALID_ARGUMENT);
    }
    else{
        std::vector<uint32_t> wlist;
        std::vector<soul_ptr> slist;
        wlist.reserve(request->warriors_size());
        slist.reserve(request->warriors_size());
        
        SERVICE_HANDLER_TRY
        player_ptr player = SINGLETON(player_manager).get_player(request->user_id());
        warrior_pkg_ptr pkg = player->warrior_pkg();
        
        bool checked(true);
        
        SCOPED_LOCK(lock, player->mutex());
        
        for(BOOST_AUTO(pos, request->warriors().begin()); pos != request->warriors().end(); ++pos){            
            if(!pkg->get(pos->sid())){
                checked = false;
                response->set_code(WARRIOR_NOT_EXIST);
                break;
            }
            wlist.push_back(pos->sid());
            slist.push_back(boost::make_shared<soul>(0, pos->sid(), pos->soul_num()));
        }
        if(checked){
            error_code code = player->soul_pkg()->add(slist);
            if(code == OPT_SUCCESS){
                pkg->remove(wlist);
            }
            response->set_code(code);
        }
        
        SERVICE_HANDLER_CATCH(response)
    }
    
    SINGLETON(cache_service).send(conn, latte::header_protobuf_message_ptr(new latte::header_protobuf_message(response, message->sync_id(), true)));
}
