//
//  Lang.cpp
//  QSMY
//
//  Created by OnePiece on 13-5-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Lang.h"
#include "StringUtil.h"
#include "cocos2d.h"


static Lang *pSharedLang = NULL;
static std::string sFile("");

void Lang::setLangFile(const char* file)
{
    sFile = file;
}

#pragma mark - Lang
Lang::Lang()
{
    
}
Lang::~Lang()
{
}
Lang * Lang::sharedLang()
{
    if(!pSharedLang)
    {
        pSharedLang = new Lang();
        pSharedLang->init();
    }
    
    return pSharedLang;
}
void Lang::purgeLang()
{
    if(pSharedLang)
    {
        CC_SAFE_DELETE(pSharedLang);
        pSharedLang = NULL;
    }
}

void Lang::init()
{
    loadFile(sFile.c_str());
}

void Lang::loadFile(const char *fileName)
{
    parseLocal( CSVDecoder::create(fileName)->getData());
}

#pragma mark - - methods private
//int  Lang::getLanguageIndex()
//{
//    ccLanguageType currentLanguageType = CCApplication::sharedApplication()->getCurrentLanguage();
//    
//    return currentLanguageType;
//}
void Lang::parseLocal(const CSVDecoder::CSV &val)
{
//    int nLanguageIndex = getLanguageIndex();
//    if(nLanguageIndex>kLanguageChinese) nLanguageIndex=kLanguageChinese;
    
    for (uint32_t i=0; i<val.size(); ++i)
    {
        std::string source = val[i][0];
        //std::string target = val[i][nLanguageIndex+1];
        std::string target = val[i][1];
		replace(target, "\\n", "\n");
		replace(target, "\\x20", "\x20");
        map[source] = target;
    }
}
#define kMaxStringLen (1024*100)

#pragma mark - - methods public
std::string Lang::getLocalString(const char *string,...)
{
    std::string sourceString = getLocalString(std::string(string));
    va_list ap;
    va_start(ap, string);
    char* pBuf = (char*)malloc(kMaxStringLen);
    if (pBuf != NULL)
    {
        vsnprintf(pBuf, kMaxStringLen, sourceString.c_str(), ap);
        sourceString = pBuf;
        free(pBuf);
    }
    va_end(ap);
    
    return sourceString;
}

std::string Lang::getLocalString(const std::string & msg)
{
    LangMap::iterator itr = map.find(msg);
    if(itr!=map.end())
    {
		cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification(FIND_LOCAL_STRING, cocos2d::ccs(msg));
        return itr->second;
    }
    return msg;
}

std::string Lang::getLocalStringWithoutNoti(const std::string & msg)
{
	LangMap::iterator itr = map.find(msg);
	if(itr!=map.end())
	{
		return itr->second;
	}
	return msg;
}
