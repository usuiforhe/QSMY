//
//  default_handler.cpp
//  battle
//
//  Created by wangyanjun on 13-3-28.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "default_handler.h"

using namespace battle;

void default_handler::operator ()(latte::tcp_conn_ptr conn, latte::header_protobuf_message_ptr message){
    std::stringstream & ss = latte::fast_stringstream::ss();
    ss << "received protobuf message, no specified handler registered. message is:" << message->message()->SerializeAsString();
    s_log.out_string(ss.str());
}
