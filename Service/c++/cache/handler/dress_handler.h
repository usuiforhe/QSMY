//
//  dress_handler.h
//  cache
//
//  Created by 汪燕军 on 14-3-4.
//  Copyright (c) 2014年 汪燕军. All rights reserved.
//

#ifndef __cache__dress_handler__
#define __cache__dress_handler__

#include "cache_prefix.h"
#include "i_service_handler.h"
#include "protocol/dress.pb.h"

namespace cache{
    //获得时装
    SERVICE_HANDLER(get_dress_handler, latte::tcp_conn_ptr);
    
    //获得时装列表
    SERVICE_HANDLER(get_dresses_handler, latte::tcp_conn_ptr);
    
    //根据弟子获得时装列表
    SERVICE_HANDLER(get_dresses_by_warrior_handler, latte::tcp_conn_ptr);
    
    //添加时装
    SERVICE_HANDLER(add_dress_handler, latte::tcp_conn_ptr);
    
    //更新时装
    SERVICE_HANDLER(update_dress_handler, latte::tcp_conn_ptr);
    
    //删除时装
    SERVICE_HANDLER(remove_dress_handler, latte::tcp_conn_ptr);
}


#endif /* defined(__cache__dress_handler__) */
