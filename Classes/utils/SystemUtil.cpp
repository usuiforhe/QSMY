//
//  SystemUtil.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SystemUtil.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)

std::string SystemUtil::getUDID()
{
    return "";
}

void SystemUtil::openURL(const char* url)
{
    
}

bool SystemUtil::isNBDevice()
{
    return true;
}

std::string SystemUtil::getDeviceInfo()
{
    return "";
}

void SystemUtil::setIdleTimerDisabled(bool val)
{
    
}

void SystemUtil::sendLocalNotification(const char*content,double delaySecond)
{
    
}


void SystemUtil::cancelNotification()
{
    
}

std::string SystemUtil::getVersion()
{
    return "1.0.7";
}

NetworkStatus SystemUtil::getNetworkStatus()
{
    return NetworkWiFi;
}

void SystemUtil::hideMouseCursor()
{
    
}

void SystemUtil::updateGame(const char* url,const char* content)
{
    
}

double SystemUtil::availableMemory()
{
    return 0;
}

double SystemUtil::usedMemory()
{
    return 0;
}

double SystemUtil::getTotalMemory()
{
	return 0;
}

void SystemUtil::quitGame()
{
    CCLOG("quitGame");
}

bool SystemUtil::addSkipBackupAttributeToItemAtPath(std::string path)
{
    return true;
}

bool SystemUtil::isFileSkipedBackUp(std::string path)
{
    return true;
}

bool SystemUtil::copyStringToPasteboard(std::string content)
{
    return true;
}

#endif

