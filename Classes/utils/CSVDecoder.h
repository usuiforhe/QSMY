//
//  CSVDecoder.h
//  QSMY
//
//  Created by wanghejun on 13-4-1.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__CSVDecoder__
#define __QSMY__CSVDecoder__


#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>
#include "cocos2d.h"

class CSVDecoder :public cocos2d::CCObject {

public:
    
    typedef std::vector<std::string> CSVLine;
    typedef std::vector<CSVLine> CSV;
    typedef std::map<std::string, CSVLine> CSVMap;
    
    static CSVDecoder* create(const char* fileName)
    {
        
        CSVDecoder *pRet = new CSVDecoder();
        if (pRet && pRet->initWithFile(fileName))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(pRet);
        }
        return pRet;
    }
    
    
    static CSVDecoder* createWithBuffer(const char* buffer,unsigned int len)
    {
        
        CSVDecoder *pRet = new CSVDecoder();
        if (pRet && pRet->initWithBuffer(buffer,len))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(pRet);
        }
        return pRet;
    }
    
public:
    
    
    CSVDecoder()
    {
        
    }
    
    ~CSVDecoder()
    {
        
    }
    
    const CSV& getData();
    void getDataMapByKey(std::string key,CSVMap& map);
    
private:
    
    virtual bool initWithFile(const char* fileName);
    virtual bool initWithBuffer(const char* buffer, unsigned int len);
    
    CSVLine &parseLine(const char *buf,uint16_t keyCount = 65535);
    void getLines();
private:
    const char* fIn;
    CSV lines;
    CSVLine line;
    CSVLine key;
};

#endif /* defined(__QSMY__CSVDecoder__) */
