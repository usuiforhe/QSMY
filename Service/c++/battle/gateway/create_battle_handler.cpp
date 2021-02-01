//
//  create_battle_handler.cpp
//  battle
//
//  Created by wangyanjun on 13-3-28.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "create_battle_handler.h"
#include "meta/scene_meta_data.pb.h"
#include "game/game_context.h"
#include "gateway/battle_service.h"
#include "builder/scene_builder.h"
#include "game/scene.h"
#include "game/report.h"

using namespace battle;

void create_battle_handler::operator()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    if(message->is_response())
    {
        return;
    }
#if TIME_STATIC
    clock_t t_start, t_end;
    t_start = clock();
#endif
    boost::shared_ptr<scene_meta_data> data = boost::static_pointer_cast<scene_meta_data>(message->message());
    battle::scene * scene = S_SCENE_BUILDER.build(*data);
    if(scene != NULL){
        try{
            scene->execute();
            if(message->sync_id()){
                boost::shared_ptr<google::protobuf::Message> response = scene->get_report()->to_message();
                latte::header_protobuf_message_ptr pm(new latte::header_protobuf_message(response, message->sync_id(), true));
                
                S_BATTLE_SERVICE.send(conn, pm);
            }
            
#if TIME_STATIC
            t_end = clock();
            
            std::stringstream & ss = latte::fast_stringstream::ss();
            ss << "battle elapsed:" << (double)(t_end - t_start)*1000/CLOCKS_PER_SEC << "ms" << std::endl;
            s_log.out_string(ss.str());
#endif
        }
        catch (boost::exception &e)
        {
            std::stringstream &ss = latte::fast_stringstream::ss();
            ss << "create_battle_handler() throws exceptions. what:" + boost::diagnostic_information(e);
            s_log.out_error(ss.str());
        }
        catch (std::exception &e)
        {
            std::stringstream &ss = latte::fast_stringstream::ss();
            ss << "create_battle_handler() throws exceptions. what:" + boost::diagnostic_information(e);
            s_log.out_error(ss.str());
        }
        delete scene;
    }
//
//    boost::shared_ptr<scene_meta_data> data = boost::static_pointer_cast<scene_meta_data>(message->message());
//    battle_request * request = new battle::battle_request(conn, message->sync_id(), data);
//    S_GAME_CONTEXT.add_request(request);
}
