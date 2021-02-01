//
//  i_service_handler.h
//  battle
//
//  Created by wangyanjun on 13-3-28.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef battle_i_service_handler_h
#define battle_i_service_handler_h

#include "battle_prefix.h"
#include "net/protobuf_dispatcher.h"

namespace battle {
    template<typename T>
    struct i_service_handler{
        virtual ~i_service_handler(){
            
        }
        
        virtual void operator ()(T conn, latte::header_protobuf_message_ptr message) = 0;
    };
}

#endif
