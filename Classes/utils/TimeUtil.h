//
//  TimeUtil.h
//  QSMY
//
//  Created by wanghejun on 13-7-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TimeUtil__
#define __QSMY__TimeUtil__

#include "AppInclude.h"

class TimeUtil
{
    
public:
    
    static void init(const char* tz);
    
    static bool isInited();
    
    /**
     *	@brief	获取参数now当天的几点几分的time_t
     *
     *	@param 	now 要获取的当前时间
     *	@param 	h 	时
     *	@param 	m 	分
     *	@param 	s 	秒
     *
     *	@return 时间戳
     */
    static unsigned int getTimeToday(unsigned int time,int h = 0,int m = 0,int s = 0);

    /**
     *	@brief	获取时间差
     *
     *	@param 	from 	从
     *	@param 	to 	到
     *
     *	@return	秒
     */
    static int getDiff(unsigned int from, unsigned int to);
    
    
    static const char* formatTime(unsigned int time,const char* format = "%H:%M:%S");

	static const std::string getCompleteTimeString(unsigned int second);
    
    static unsigned int getStartTimeOfTomorrow(unsigned int now);

    //获取某天的开始时间
    static unsigned int getStartTimeOfToday(unsigned int now);
    
    static unsigned int getTimeDiffToTomorrow(unsigned int time);
    
    
    static const char* getSeconds2String(int sec,const char* format = "%H:%M:%S",const char* token = ":" ,const char* num_format ="%02d", bool chineseToken = false);
    
    /**
     *	@brief	获得时间对应的小时
     *
     *	@param 	time 	时间
     *
     *	@return	小时
     */
    static unsigned int getHour(unsigned int time);

	static uint8_t getWeekday(uint32_t time);
};

#endif /* defined(__QSMY__TimeUtil__) */
