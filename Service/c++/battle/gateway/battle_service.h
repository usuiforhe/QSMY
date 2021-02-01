//
//  battle_service.h
//  battle
//
//  Created by wangyanjun on 13-3-27.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__battle_service__
#define __battle__battle_service__

#include "battle_prefix.h"
#include "singleton.h"
#include "net/tcp_server.h"
#include "meta/scene_meta_data.pb.h"
#include "net/protobuf_dispatcher.h"
#include "net/protobuf_codec.h"
#include "thread/thread_pool.h"

namespace battle {
    class battle_service{
    public:
        battle_service();
        
        ~battle_service();
        
        void start();
        
        void stop();
        
        void send(latte::tcp_conn_ptr connection, latte::header_protobuf_message_ptr message);
        
        void handle_message(latte::tcp_conn_ptr connection, latte::header_protobuf_message_ptr message);
        
    private:
        latte::tcp_server * tcp_server_;
        latte::protobuf_codec * codec_;
        latte::protobuf_dispatcher<latte::tcp_conn_ptr> * dispatcher_;
        latte::thread_pool thread_pool_;
        
        void init();
    };
#define S_BATTLE_SERVICE latte::singleton<battle_service>::instance()
}

#endif /* defined(__battle__battle_service__) */
