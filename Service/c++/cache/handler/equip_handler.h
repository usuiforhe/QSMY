//
//  equip_handler.h
//  cache
//
//  Created by 汪燕军 on 13-5-8.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__equip_handler__
#define __cache__equip_handler__

#include "cache_prefix.h"
#include "i_service_handler.h"
#include "protocol/equip.pb.h"

namespace cache{    
    //获得单个装备
    SERVICE_HANDLER(get_equip_handler, latte::tcp_conn_ptr);
    
    //获得装备列表
    SERVICE_HANDLER(get_equips_handler, latte::tcp_conn_ptr);
    
    //根据阵容位置获得装备列表
    SERVICE_HANDLER(get_equips_by_pos_handler, latte::tcp_conn_ptr);
    
    //添加装备
    SERVICE_HANDLER(add_equip_handler, latte::tcp_conn_ptr);
    
    //批量添加装备
    SERVICE_HANDLER(batch_add_equip_handler, latte::tcp_conn_ptr);
    
    //更新装备
    SERVICE_HANDLER(update_equip_handler, latte::tcp_conn_ptr);
    
    //批量更新装备
    SERVICE_HANDLER(batch_update_equip_handler, latte::tcp_conn_ptr);
    
    //删除装备
    SERVICE_HANDLER(remove_equip_handler, latte::tcp_conn_ptr);
    
    //批量删除装备
    SERVICE_HANDLER(batch_remove_equip_handler, latte::tcp_conn_ptr);
}

#endif /* defined(__cache__equip_handler__) */
