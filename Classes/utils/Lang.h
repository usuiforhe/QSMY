//
//  Lang.h
//  QSMY
//
//  Created by OnePiece on 13-5-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Lang__
#define __QSMY__Lang__

#include "CSVDecoder.h"
#include "AppInclude.h"

#pragma mark - Lang
class Lang : public cocos2d::CCObject
{
#pragma mark - - init etc.
public:
    Lang();
    ~Lang();
    void init();
    static Lang * sharedLang();
    static void purgeLang();
    static void setLangFile(const char* file);
    
#pragma mark - - variable
private:
    typedef std::map<std::string, std::string> LangMap;
    LangMap map;
    
#pragma mark - - methods public
private:
    //int  getLanguageIndex();
    void parseLocal(const CSVDecoder::CSV &val);
    void loadFile(const char *fileName);
#pragma mark - - methods public
public:
    std::string getLocalString(const char *string,...);
	std::string getLocalString(const std::string & msg);
	std::string getLocalStringWithoutNoti(const std::string & msg);
};

#endif /* defined(__QSMY__Lang__) */
