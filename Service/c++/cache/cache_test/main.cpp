//
//  main.cpp
//  cache_test
//
//  Created by 汪燕军 on 13-9-3.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "get_items_test.h"

using namespace cache_test;

int main(int argc, const char * argv[])
{
    item_test test;
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

