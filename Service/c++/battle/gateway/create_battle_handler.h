//
//  create_battle_handler.h
//  battle
//
//  Created by wangyanjun on 13-3-28.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__create_battle_handler__
#define __battle__create_battle_handler__

#include "i_service_handler.h"
#include "singleton.h"

namespace battle {
    class create_battle_handler:public i_service_handler<latte::tcp_conn_ptr>{
    public:
        ~create_battle_handler(){}
        
        virtual void operator ()(latte::tcp_conn_ptr  conn, latte::header_protobuf_message_ptr message);
    };
#define S_CREATE_BATTLE_HANDLER latte::singleton<create_battle_handler>::instance()
}

#endif /* defined(__battle__create_battle_handler__) */
