//
//  base64.h
//  QSMY
//
//  Created by wanghejun on 13-9-24.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__base64__
#define __QSMY__base64__

#include <string>

std::string base64_encode(const std::string& input);
std::string base64_decode(const std::string& input);

#endif /* defined(__QSMY__base64__) */