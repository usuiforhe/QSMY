//
//  login_test.cpp
//  performance_test
//
//  Created by 汪燕军 on 13-9-4.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "login_test.h"

using namespace performance_test;

login_test::login_test()
:thread_pool_(100, 100)
{
    
}

login_test::~login_test()
{
}

void login_test::start()
{    
    for(uint32_t i = 0; i < 10; ++i)
    {
        thread_pool_.queue_work_item(boost::bind(&login_test::login, this, i));
    }
}

void login_test::stop()
{
    thread_pool_.join();
}

void login_test::login(uint32_t id)
{
    std::stringstream & ss = latte::fast_stringstream::ss();
    boost::mt19937 rng(id);
    boost::uniform_smallint<> ui(0, 1000);

    for(int i = 0; i < 10; ++i)
    {
        ss << id << ":" << ui(rng) << std::endl;
    }
    s_log.out_string(ss.str());
//    std::stringstream & ss = latte::fast_stringstream::ss();
//    ss << "curl http://10.10.1.152/port/login.php?u=" << user_name;
//    std::string cmd = ss.str();
//    std::system(cmd.c_str());
//    
//    ss.clear();
//    ss << user_name << "logined." << std::endl;
//    s_log.out_string(ss.str());
}

