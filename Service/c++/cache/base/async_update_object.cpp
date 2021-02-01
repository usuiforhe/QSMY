//
//  async_update_object.cpp
//  cache
//
//  Created by 汪燕军 on 13-12-17.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "async_update_object.h"
#include "cache_service.h"
using namespace cache;

bool async_update_object::can_update()
{
    return SINGLETON(cache_service).stopped() || last_update_ts_ + SINGLETON(config).write_delay <= TIME_NOW.get_time_t();
}

