//
//  default_handler.h
//  battle
//
//  Created by wangyanjun on 13-3-28.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__default_handler__
#define __battle__default_handler__

#include "i_service_handler.h"
#include "singleton.h"

namespace battle {
    class default_handler:public i_service_handler<latte::tcp_conn_ptr>{
    public:
        ~default_handler(){}
        
        virtual void operator ()(latte::tcp_conn_ptr  conn, latte::header_protobuf_message_ptr message);
    };
#define S_DEFAULT_HANDLER latte::singleton<default_handler>::instance()
}
#endif /* defined(__battle__default_handler__) */
