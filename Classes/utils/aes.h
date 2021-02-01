//
//  aes.h
//  QSMY
//
//  Created by wanghejun on 13-9-24.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__aes__
#define __QSMY__aes__

#include <string>

std::string EncodeAES(const std::string& strPass,const std::string& str);
std::string DecodeAES(const std::string& strPass,const std::string& str);

#endif /* defined(__QSMY__aes__) */
