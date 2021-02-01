//
//  md5.cpp
//  QSMY
//
//  Created by wanghejun on 14-2-12.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "md5.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <Windows.h>
#include <tchar.h>

#define PROTO_LIST(list) list

typedef struct
{
	ULONG i[2];
	ULONG buf[4];
	unsigned char in[64];
	unsigned char digest[16];
} MD5_CTX;
typedef void (WINAPI * PMD5Init) PROTO_LIST((MD5_CTX *));
typedef void (WINAPI * PMD5Update) PROTO_LIST((MD5_CTX *, const unsigned char *, unsigned int));
typedef void (WINAPI * PMD5Final) PROTO_LIST((MD5_CTX *));

static const char *Hex2ASC(const BYTE *Hex, int len)
{
	static char ASC[4096 * 2];
	int i;
	for (i = 0; i < len; i++)
	{
		ASC[i * 2] = "0123456789ABCDEF"[Hex[i] >> 4];
		ASC[i * 2 + 1] = "0123456789ABCDEF"[Hex[i] & 0x0F];
	}
	ASC[i * 2] = '\0';
	return ASC;
}
static const char *ToMd5(const char *str, unsigned int len)
{
	MD5_CTX ctx;
	const unsigned char *buf = reinterpret_cast<const unsigned char *>(str);
	//int len = strlen(str);
	HINSTANCE hDLL;
	if ((hDLL = LoadLibrary(_T("advapi32.dll"))) > 0)
	{
		PMD5Init MD5Init = (PMD5Init)GetProcAddress(hDLL, "MD5Init");
		PMD5Update MD5Update = (PMD5Update)GetProcAddress(hDLL, "MD5Update");
		PMD5Final MD5Final = (PMD5Final)GetProcAddress(hDLL, "MD5Final");
		MD5Init(&ctx);
		MD5Update(&ctx, buf, len);
		MD5Final(&ctx);
	}
	return Hex2ASC(ctx.digest, 16);
	
}
#else
#include <openssl/md5.h>
#endif

std::string md5(const std::string& input)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    std::string rt = ToMd5(input.c_str(), input.length());
#else
	unsigned char buffer[16] = {'\0'};
    char temp[3] = {'\0'};
    std::string rt("");
    MD5((const unsigned char *)input.c_str(),input.length(),buffer);
    for (uint8_t i(0); i<16; ++i) {
        sprintf(temp, "%02x",buffer[i]);
        rt+=temp;
    }
#endif
    return rt;
}