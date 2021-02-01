//
//  warrior_handler.h
//  cache
//
//  Created by 汪燕军 on 13-5-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__warrior_handler__
#define __cache__warrior_handler__

#include "cache_prefix.h"
#include "i_service_handler.h"
#include "protocol/warrior.pb.h"

namespace cache{
    //获得单个单位
    SERVICE_HANDLER(get_warrior_handler, latte::tcp_conn_ptr);
    //获得单位列表
    SERVICE_HANDLER(get_warriors_handler, latte::tcp_conn_ptr);
    //添加单位
    SERVICE_HANDLER(add_warrior_handler, latte::tcp_conn_ptr);
    //更新单位
    SERVICE_HANDLER(update_warrior_handler, latte::tcp_conn_ptr);
    //批量更新单位
    SERVICE_HANDLER(batch_update_warrior_handler, latte::tcp_conn_ptr);
    //删除单位
    SERVICE_HANDLER(remove_warrior_handler, latte::tcp_conn_ptr);
    //批量删除单位
    SERVICE_HANDLER(batch_remove_warrior_handler, latte::tcp_conn_ptr);
    //分解单位
    SERVICE_HANDLER(decompose_warrior_handler, latte::tcp_conn_ptr);
    //批量分解单位
    SERVICE_HANDLER(batch_decompose_warrior_handler, latte::tcp_conn_ptr);
}

#endif /* defined(__cache__warrior_handler__) */
