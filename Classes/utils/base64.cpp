#include "base64.h"
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <Windows.h>
#include <WinDef.h>
#include <WinCrypt.h>
#include <atlstr.h>
#include <vector>
#else
#include "openssl/pem.h"
#endif



std::string base64_encode(const std::string& input)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	DWORD nLen = 0;
	CryptBinaryToStringA((const BYTE *)input.c_str(), input.size(), CRYPT_STRING_BASE64, NULL, &nLen);
	CStringA ret;
	if (nLen)
	{
		std::vector<char> buf(nLen);
		CryptBinaryToStringA((const BYTE *)input.c_str(), input.size(), CRYPT_STRING_BASE64, &buf[0], &nLen);
		ret = &buf[0];
		ret.Replace("\r\n", "");
	}
	std::string rt(ret.GetBuffer());
#else
	BIO *bmem, *b64;
	BUF_MEM *bptr;

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new(BIO_s_mem());
	bmem = BIO_push(b64, bmem);

	BIO_write(bmem, input.c_str(), input.length());
	BIO_flush(bmem);
	BIO_get_mem_ptr(bmem, &bptr);
	std::string rt(bptr->data,bptr->length-1);
	BIO_free_all(bmem);
#endif
    return rt;
}

std::string base64_decode(const std::string& input)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	DWORD nLen = 0;
	CryptStringToBinaryA(input.c_str(), 0, CRYPT_STRING_BASE64, NULL, &nLen, NULL, NULL);
	CStringA ret;
	if (nLen)
	{
		std::vector<BYTE> buf(nLen + 1);
		CryptStringToBinaryA(input.c_str(), 0, CRYPT_STRING_BASE64, &buf[0], &nLen, NULL, NULL);
		ret = &buf[0];
	}
	std::string rt(ret.GetBuffer());
	
#else
    BIO *bmem, *b64;
    
    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new_mem_buf((void*)input.c_str(), input.length());
    bmem = BIO_push(b64, bmem);
    
    char* buffer = (char*)malloc(input.length());
    memset(buffer, 0, input.length());
    size_t len = BIO_read(bmem, buffer, input.length());
    
    std::string rt(buffer,len);
    free(buffer);
    
    BIO_free_all(bmem);
#endif
    return rt;
}
