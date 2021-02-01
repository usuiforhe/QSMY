//
//  TimeUtil.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TimeUtil.h"
#include "StringUtil.h"

#define MONTHS_PER_YEAR  12
#define DAYS_PER_WEEK    7
#define HOURS_PER_DAY    24
#define MINUTES_PER_HOUR 60
#define SECONDS_PER_MINUTE 60

#define WEEKDAY_AT_FIRST_DAY	4

#define MILLIS_PER_SECOND 1000
#define MILLIS_PER_MINUTE SECONDS_PER_MINUTE * MILLIS_PER_SECOND
#define MILLIS_PER_HOUR MINUTES_PER_HOUR * MILLIS_PER_MINUTE
#define MILLIS_PER_DAY HOURS_PER_DAY * MILLIS_PER_HOUR

static bool inited = false;

bool TimeUtil::isInited()
{
    return inited;
}


void TimeUtil::init(const char* tz)
{
    static char* evn = NULL;
    if(evn!=NULL) free(evn);
    size_t len = strlen(tz)+4;
    evn = (char*)malloc(len);
    memset(evn, 0, len);
    sprintf(evn, "TZ=%s",tz);
#ifndef _WIN32
    putenv(evn);
#endif
    inited = true;
}

unsigned int TimeUtil::getTimeToday(unsigned int _time,int h,int m,int s)
{
    time_t time = (time_t)_time;
    struct tm* pTm = localtime(&time);
    pTm->tm_hour = h;
    pTm->tm_min = m;
    pTm->tm_sec = s;
    //struct timezone tz;
    //gettimeofday(NULL, &tz);
    unsigned int rt = mktime(pTm);
    //CCLOG("----%s",formatTime(rt,"%D:%H:%M:%S"));
	return rt;
}

int TimeUtil::getDiff(unsigned int from, unsigned int to)
{
    return to - from;
}

const char* TimeUtil::formatTime(unsigned int time,const char*format)
{
    static char str[255]={0};
    time_t t = (time_t)time;
    struct tm* pTm = localtime(&t);
    strftime(str, 255, format, pTm);
    return str;
}

unsigned int TimeUtil::getTimeDiffToTomorrow(unsigned int _time)
{
    return getStartTimeOfTomorrow(_time)-_time;
}

const char* TimeUtil::getSeconds2String(int sec,const char* format,const char* token,const char* num_format, bool chineseToken)
{
    int d = 0;
    int h = 0;
    int m = 0;
    int s = 0;
    
    static char str[255]={0};
    
    std::string formatStr = std::string(format);
    
    if(formatStr.find("%D")!=std::string::npos)
    {
        // 获取天数
        d = floorf(sec/SECONDS_PER_MINUTE/MINUTES_PER_HOUR/HOURS_PER_DAY);
        // 取余，余下的数值就是时、分、秒
        sec %= SECONDS_PER_MINUTE*MINUTES_PER_HOUR*HOURS_PER_DAY;
    }
    if(formatStr.find("%H")!=std::string::npos)
    {
        // 获取小时数
        h = floorf(sec/SECONDS_PER_MINUTE/MINUTES_PER_HOUR);
        // 取余，余下的数值就是分、秒
        sec %= SECONDS_PER_MINUTE*MINUTES_PER_HOUR;
    }
    if(formatStr.find("%M")!=std::string::npos)
    {
        // 获取分钟数
        m = floorf(sec/SECONDS_PER_MINUTE);
        sec %= SECONDS_PER_MINUTE;
    }
    if(formatStr.find("%S")!=std::string::npos)
    {
        // 取余，余下的数值就是秒
        s = sec;
    }
    
    
    std::vector<std::string> strs;
    
    split(formatStr,":",strs);
    
    std::vector<std::string> rets;
    std::vector<std::string>::iterator itr;
    for (itr=strs.begin(); itr!=strs.end(); ++itr)
    {
        memset(str, 0, sizeof(str));
        if(itr->compare("%D")==0)
        {
            sprintf(str,num_format,d);
            rets.push_back(str);
			if(chineseToken)rets.push_back(D_LOCAL_STRING("Time_Day"));
        }else if(itr->compare("%H")==0)
        {
            sprintf(str,num_format,h);
            rets.push_back(str);
			if(chineseToken)rets.push_back(D_LOCAL_STRING("Time_Hour"));
        }else if(itr->compare("%M")==0)
        {
            sprintf(str,num_format,m);
            rets.push_back(str);
			if(chineseToken)rets.push_back(D_LOCAL_STRING("Time_Minute"));
        }else if(itr->compare("%S")==0)
        {
            sprintf(str,num_format,s);
            rets.push_back(str);
			if(chineseToken)rets.push_back(D_LOCAL_STRING("Time_Second"));
        }
    }
    std::string ret = join(rets,chineseToken ? "" : token);
    memset(str, 0, sizeof(str));
    memcpy(str, ret.c_str(),ret.size());
    return str;
}

unsigned int TimeUtil::getStartTimeOfTomorrow(unsigned int now)
{
    now += HOURS_PER_DAY*MINUTES_PER_HOUR*SECONDS_PER_MINUTE;
    return getStartTimeOfToday(now);
}

unsigned int TimeUtil::getStartTimeOfToday(unsigned int now)
{
    return getTimeToday(now);
}

unsigned int TimeUtil::getHour(unsigned int time)
{
    return getDiff(getTimeToday(time), time) / (MINUTES_PER_HOUR * SECONDS_PER_MINUTE);
}

uint8_t TimeUtil::getWeekday(uint32_t time)
{
	int d = floorf(time/SECONDS_PER_MINUTE/MINUTES_PER_HOUR/HOURS_PER_DAY);
	return (WEEKDAY_AT_FIRST_DAY + d) % 7;
}

const std::string TimeUtil::getCompleteTimeString( unsigned int second )
{
	time_t _time = second;
	tm* s_time = localtime(&_time);
	cocos2d::CCString* timeString = cocos2d::CCString::createWithFormat("%4d-%02d-%02d %02d:%02d", 
																						s_time->tm_year + 1900, 
																						s_time->tm_mon + 1,
																						s_time->tm_mday,
																						s_time->tm_hour,
																						s_time->tm_min);
	return std::string(timeString->getCString());
}
