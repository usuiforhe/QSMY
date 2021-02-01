//
//  SystemUtil.h
//  QSMY
//
//  Created by wanghejun on 13-9-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SystemUtil__
#define __QSMY__SystemUtil__

#include "AppInclude.h"

typedef enum
{
    NetworkNone = 0,
    NetworkWiFi,
    Network3G,
} NetworkStatus;

class SystemUtil
{
public:
    
    /**
     *	@brief	获取机器唯一编号
     *
     *	@return	机器唯一编号
     */
    static std::string getUDID();

    
    /**
     *	@brief	打开一个url
     *
     *	@param 	char* 	地址
     */
    static void openURL(const char*);

    /**
     *	@brief	判断设备性能
     *
     *	@return	是否是高性能的机器
     */
    static bool isNBDevice();
    
    /**
     *	@brief	获取设备型号
     *
     *	@return	设备型号
     */
    static std::string getDeviceInfo();


    
    /**
     *	@brief	防止自动锁屏
     *
     *	@param 	val 	是否防止自动锁屏
     */
    static void setIdleTimerDisabled(bool val);

    /**
     *	@brief	撤掉本地通知
     */
    static void cancelNotification();

    
    /**
     *	@brief	发送本地通知
     *
     *	@param 	char*content 	通知内容
     *	@param 	delaySecond 	延迟时间
     */
    static void sendLocalNotification(const char*content,double delaySecond);

    /**
     *	@brief	获取程序版本
     *
     *	@return	程序版本号
     */
    static std::string getVersion();

    
    /**
     *	@brief	获取网络状态
     *
     *	@return	网络状态
     */
    static NetworkStatus getNetworkStatus();

    /**
     *	@brief	隐藏鼠标
     */
    static void hideMouseCursor();

    
    /**
     *	@brief	强制更新游戏
     *
     *	@param 	url 	更新包地址
     *	@param 	NULL 	提示内容
     */
    static void updateGame(const char* url,const char* content = NULL);

    
    /**
     *	@brief	可用的内存
     *
     *	@return	内存大小
     */
    static double availableMemory();

    /**
     *	@brief	使用的内存
     *
     *	@return	内存大小
     */
    static double usedMemory();

	/**
     *	@brief	总内存
     *
     *	@return	内存大小
     */
    static double getTotalMemory();
    
    /**
     *	@brief	退出游戏
     */
    static void quitGame();
    
    //ios中设置文件为不被icloud和itunes备份属性
    static bool addSkipBackupAttributeToItemAtPath(std::string path);
    static bool isFileSkipedBackUp(std::string path);
    
    //复制字符串到剪贴板
    static bool copyStringToPasteboard(std::string content);
};

#endif /* defined(__QSMY__SystemUtil__) */
