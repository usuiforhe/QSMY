//
//  main.cpp
//  performance_test
//
//  Created by 汪燕军 on 13-9-4.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "login_test.h"

using namespace performance_test;
int main(int argc, const char * argv[])
{
    login_test test;
    test.start();
    
    while(true)
    {
        std::string line;
        std::getline(std::cin, line);
        
        if (line == "exit" || line == "quit" || line == "bye" || line == "q")
        {
            break;
        }
        
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }

    test.stop();
    
    return 0;
}

