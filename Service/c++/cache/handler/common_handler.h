//
//  common_handler.h
//  cache
//
//  Created by 汪燕军 on 13-5-8.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__common_handler__
#define __cache__common_handler__

#include "cache_prefix.h"
#include "i_service_handler.h"
#include "protocol/common.pb.h"

namespace cache{
    //获得单个装备
    SERVICE_HANDLER(default_handler, latte::tcp_conn_ptr);
    
    //获得单个装备
    SERVICE_HANDLER(async_update_handler, latte::tcp_conn_ptr);
}

#endif /* defined(__cache__common_handler__) */
