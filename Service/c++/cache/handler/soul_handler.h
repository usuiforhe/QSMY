//
//  soul_handler.h
//  cache
//
//  Created by 汪燕军 on 13-5-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__soul_handler__
#define __cache__soul_handler__

#include "cache_prefix.h"
#include "i_service_handler.h"
#include "protocol/soul.pb.h"

namespace cache{
    //获得单个道具
    SERVICE_HANDLER(get_soul_handler, latte::tcp_conn_ptr);
    
    //获得道具列表
    SERVICE_HANDLER(get_souls_handler, latte::tcp_conn_ptr);
    
    //添加道具
    SERVICE_HANDLER(add_soul_handler, latte::tcp_conn_ptr);
    
    //批量添加道具
    SERVICE_HANDLER(batch_add_soul_handler, latte::tcp_conn_ptr);
    
    //删除道具
    SERVICE_HANDLER(remove_soul_handler, latte::tcp_conn_ptr);
    
    //批量删除道具
    SERVICE_HANDLER(batch_remove_soul_handler, latte::tcp_conn_ptr);
}

#endif /* defined(__cache__soul_handler__) */
