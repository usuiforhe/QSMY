//
//  i_service_handler.h
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef cache_i_service_handler_h
#define cache_i_service_handler_h

#include "cache_prefix.h"
#include "net/protobuf_dispatcher.h"

namespace cache {
    template<typename T>
    struct i_service_handler{
        virtual ~i_service_handler(){
            
        }
        
        virtual void operator ()(T conn, latte::header_protobuf_message_ptr message) = 0;
    };
    
    
#define SERVICE_HANDLER(NAME, T) \
    class NAME:public i_service_handler<T>{\
    public:\
        NAME():name_(#NAME){\
        }\
        \
        ~NAME(){\
        }\
        \
        virtual void operator ()(T conn, latte::header_protobuf_message_ptr message);\
    \
    private:\
        std::string name_;\
    };
}


#endif
