//
//  aes.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-24.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "aes.h"
#include "openssl/aes.h"

std::string EncodeAES(const std::string& strPass,const std::string& strSrc)
{
    AES_KEY aes;
    AES_set_encrypt_key((const unsigned char*)strPass.c_str(), 128, &aes);
    
    std::string strRet("");
    std::string str = strSrc;
    int padding = 0;
    if(str.length() % AES_BLOCK_SIZE>0)
    {
        padding = AES_BLOCK_SIZE -str.length() % AES_BLOCK_SIZE;
    }
    
    while (--padding >= 0) {
        str += '\0';
    }
    
    unsigned long len = str.length()/AES_BLOCK_SIZE;
    unsigned char out[AES_BLOCK_SIZE]={0};
    for(unsigned int i = 0; i<len; ++i)
    {
        std::string str16 = str.substr(i*AES_BLOCK_SIZE,AES_BLOCK_SIZE);
        memset(out, 0, AES_BLOCK_SIZE);
        AES_encrypt((const unsigned char*)str16.c_str(),out,&aes);
        strRet.append((const char*)out,AES_BLOCK_SIZE);
    }
    
    unsigned char in[AES_BLOCK_SIZE]= {0};
    memset(out, 0, AES_BLOCK_SIZE);
    unsigned int strLen = strSrc.length();
    in[0] = strLen & 0xFF;
    in[1] = strLen>>8 & 0xFF;
    in[2] = strLen>>16 & 0xFF;
    in[3] = strLen>>24 & 0xFF;
    
    AES_encrypt(in,out,&aes);
    strRet.append((const char*)out,AES_BLOCK_SIZE);
    
    return strRet;
}


std::string DecodeAES(const std::string& strPass,const std::string& str)
{
    AES_KEY aes;
    AES_set_decrypt_key((const unsigned char*)strPass.c_str(), 128, &aes);
    
    std::string strRet("");
    
    unsigned long len =str.length()/AES_BLOCK_SIZE;
    unsigned int strLen = 0;
    for(unsigned int i = 0; i<len; ++i)
    {
        std::string str16 = str.substr(i*AES_BLOCK_SIZE,AES_BLOCK_SIZE);
        if(str16.length()==0) continue;
        unsigned char out[AES_BLOCK_SIZE]={0};
        AES_decrypt((const unsigned char*)str16.c_str(),out,&aes);
        if(i==len-1)
        {
            strLen = out[0] | out[1]<<8 | out[2]<<16 | out[3]<<24;
        }else
        {
            strRet.append((const char*)out,AES_BLOCK_SIZE);
        }
    }
    return std::string(strRet.c_str(),strLen);
}