//
//  login_test.h
//  performance_test
//
//  Created by 汪燕军 on 13-9-4.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __performance_test__login_test__
#define __performance_test__login_test__

#include "thread/thread_pool.h"
#include "util.h"

namespace performance_test
{
    class login_test
    {
    public:
        login_test();
        
        ~login_test();
        
        void start();
        
        void stop();
        
        void login(uint32_t id);
        
    private:
        latte::thread_pool thread_pool_;
    };
}

#endif /* defined(__performance_test__login_test__) */
