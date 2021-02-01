//
//  CSVDecoder.cpp
//  QSMY
//
//  Created by wanghejun on 13-4-1.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "CSVDecoder.h"
#include "AppConsts.h"

#ifdef CRYPT
#include "aes.h"
#endif

using namespace std;

bool CSVDecoder::initWithFile(const char* fileName)
{
    std::string string(fileName);
    std::string ending(".txt");
    if(string.length() < ending.length() || (string.compare(string.length() - ending.length(), ending.length(), ending) != 0))
    {
        string+=ending;
    }
    string+=SUFFIX;
    
    unsigned long size = 0;
    char* pBuffer = (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(string.c_str(), "rb", &size);
    if (pBuffer != NULL && size > 0)
    {
        initWithBuffer(pBuffer,size);
    }
    CC_SAFE_DELETE_ARRAY(pBuffer);
    return true;
    
}
bool CSVDecoder::initWithBuffer(const char* buffer,unsigned int len)
{
    std::string string(buffer,len);
#ifdef CRYPT
    string = DecodeAES(DATA_PASSWORD, string);
#endif
    fIn = string.c_str();
    getLines();
    return true;
}


void CSVDecoder::getLines()
{
    if (*(fIn+0)==(char)0xefU
        && *(fIn+1)==(char)0xbbU
        && *(fIn+2)==(char)0xbfU )
    {
        fIn += 3;
    }
    stringstream ss(fIn);
    std::string temp;
    
    
    getline(ss, temp, '\r');
    key = parseLine(temp.c_str());
    uint16_t keyCount =  key.size();    //去掉key（第一行）这一行,并计算key个数
    
    
    while(getline(ss, temp, '\r')){
        CSVLine &s = parseLine(temp.c_str(),keyCount);
        if(s.size()>=keyCount) lines.push_back(s);
    }
}

CSVDecoder::CSVLine& CSVDecoder::parseLine(const char *buf,uint16_t keyCount)
{
    line.clear();
    bool inQuote(false);
	bool referTo = false;
	char refer[1024] = {0};
	int r = 0;
    char field[1024] = {0};//保存字段
    int j = 0;
    int len = strlen(buf);//字符串长度
    for(int i = 0; i < len; i++){//遍历
        if(buf[i] == '\n') continue;
        if(!inQuote && !referTo)
        {
            if(buf[i] == '\"'){//遇到双引号
                inQuote = true;
                continue;
            }
			else if (buf[i] == '$' && line.size() > 0)
			{
				referTo = true;
				refer[0] = '$';
				r = 1;
				continue;
			}
            if(buf[i] == '\t'){//如果是\t
                field[j] = '\0';
                line.push_back(field);
                if(line.size()>=keyCount)
                {
                    return line;
                }
                j = 0;//下一字段temp重新开始计数
            }else{
                field[j++] = buf[i];//往temp里填充数据
            }
        }
		else if (referTo)
		{
			refer[r++] = buf[i];
			if (buf[i + 1] != '\t')
			{
				continue;
			}
			refer[r] = '\0';
			referTo = false;
			const std::string loc = Lang::sharedLang()->getLocalStringWithoutNoti(refer);
			//CCLOG("%s", loc.c_str());
			for (int k = 0; k < strlen(loc.c_str()); k++)
			{
				field[j++] = loc[k];
			}
        }
		else
        {
            if(buf[i] == '\"'){//遇到双引号
                if(i+1 < len && buf[i+1] == '\"'){//如果下一个字符未出界 并且下一字符也是双引号
                    field[j++] = '\"';//往temp中填充一个双引号
                    i++;//跳过对下一个字符的判断
                }else{//否则,双引号计数
                    inQuote =false;
                }
            }else
            {
                field[j++] = buf[i];//往temp里填充数据
            }
        }
    }
    
    field[j]='\0';
    line.push_back(field);
    return line;
}

const CSVDecoder::CSV& CSVDecoder::getData()
{
    return lines;
}

void CSVDecoder::getDataMapByKey(std::string key,CSVDecoder::CSVMap& map)
{
    map.clear();
    uint32_t index = key.find(key);
    for (unsigned int i=0; i<lines.size(); ++i) {
        map[lines[i][index]] = lines[i];
    }
}

