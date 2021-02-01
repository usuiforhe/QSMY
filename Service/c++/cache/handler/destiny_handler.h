//
//  destiny_handler.h
//  cache
//
//  Created by 汪燕军 on 13-5-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__destiny_handler__
#define __cache__destiny_handler__

#include "cache_prefix.h"
#include "i_service_handler.h"
#include "protocol/destiny.pb.h"

namespace cache{
    //获得单个天命
    SERVICE_HANDLER(get_destiny_handler, latte::tcp_conn_ptr);
    //获得天命列表
    SERVICE_HANDLER(get_destinies_handler, latte::tcp_conn_ptr);
    //保存天命
    SERVICE_HANDLER(save_destiny_handler, latte::tcp_conn_ptr);
    //提升天命
    SERVICE_HANDLER(improve_destiny_handler, latte::tcp_conn_ptr);
}

#endif /* defined(__cache__destiny_handler__) */
