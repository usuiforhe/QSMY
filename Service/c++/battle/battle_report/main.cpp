//
//  main.cpp
//  battle_report
//
//  Created by 汪燕军 on 13-10-12.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "log/log.h"
#include "battle_prefix.h"
#include "config.h"
#include "server.hpp"
#include "report_converter.h"

using namespace battle_report;
int main(int argc, const char * argv[])
{
    report_converter converter;
    converter.convert_cup();
    
    return 0;
}

