//
//  SystemUtil.mm
//  QSMY
//
//  Created by wanghejun on 13-9-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "utils/SystemUtil.h"
#import "sys/utsname.h"
#import <SystemConfiguration/SystemConfiguration.h>
#import <sys/sysctl.h>
#import <mach/mach.h>
#import "TalkingDataGA.h"
#import <AdSupport/AdSupport.h>

std::string SystemUtil::getUDID()
{
    //NSLog(@"%@", [TalkingDataGA getDeviceId]);
    NSString* talkingId = [TalkingDataGA getDeviceId];
    NSLog(@"talkingId: %@", talkingId);
    
//talkingdata 獲取的设备号在官方接口前面加了一个“0”
//    NSString* idfaString = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
//    NSLog(@"idfaString: %@", idfaString);
    return [talkingId UTF8String];
}

void SystemUtil::openURL(const char* url)
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s",url]]];
}

std::string SystemUtil::getDeviceInfo()
{
    struct utsname systemInfo;
    uname(&systemInfo);
    std::string ret = systemInfo.machine;
    ret.append("_");
    ret.append(systemInfo.sysname);
    ret.append("_");
    ret.append(systemInfo.release);
    return ret;
}

bool SystemUtil::isNBDevice()
{
    struct utsname systemInfo;
    uname(&systemInfo);
    
    std::string deviceString(systemInfo.machine);
    return deviceString.find("iPhone1")==std::string::npos
    &&  deviceString.find("iPhone2")==std::string::npos
    &&  deviceString.find("iPhone3")==std::string::npos
    &&  deviceString.find("iPhone4")==std::string::npos
    &&  deviceString.find("iPod1")==std::string::npos
    &&  deviceString.find("iPod2")==std::string::npos
    &&  deviceString.find("iPod3")==std::string::npos
    &&  deviceString.find("iPod4")==std::string::npos
    &&  deviceString.find("iPod5")==std::string::npos
    &&  deviceString.find("iPad1")==std::string::npos
    &&  deviceString.find("iPad2")==std::string::npos;
    //&&  deviceString.find("x86_64")==std::string::npos;
    //    return deviceString.find("iPhone4") != std::string::npos
    //           || deviceString.find("iPod4") != std::string::npos
    //           || deviceString.find("iPad2") != std::string::npos
    //           || deviceString.find("i386") != std::string::npos
    //           || deviceString.find("x86_64") != std::string::npos;
}

void SystemUtil::setIdleTimerDisabled(bool val)
{
    [[UIApplication sharedApplication] setIdleTimerDisabled:val?YES:NO];
}

void SystemUtil::sendLocalNotification(const char*content,double delaySecond)
{
    UILocalNotification * notification = [[[UILocalNotification alloc] init] autorelease];
    if(notification!=nil)
    {
        notification.fireDate = [NSDate dateWithTimeIntervalSinceNow:delaySecond];
        notification.repeatInterval  = kCFCalendarUnitDay;
        notification.alertBody = [NSString stringWithUTF8String:content];
        notification.applicationIconBadgeNumber = 1;
        notification.soundName = UILocalNotificationDefaultSoundName;
        [[UIApplication sharedApplication] scheduleLocalNotification:notification];
    }
}


void SystemUtil::cancelNotification()
{
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
}

std::string SystemUtil::getVersion()
{
    /*return [[NSString stringWithFormat:@"%@.%@"
             ,[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"]
             ,[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"]] UTF8String];*/
    NSLog(@"%@", [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"]);
    return [[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"] UTF8String];
}

NetworkStatus SystemUtil::getNetworkStatus()
{
    SCNetworkReachabilityRef reachablility = SCNetworkReachabilityCreateWithName(NULL, "www.thedream.cc");
    NetworkStatus ret = NetworkNone;
    SCNetworkReachabilityFlags flags;
    if (SCNetworkReachabilityGetFlags(reachablility, &flags)) {
        if ((flags & kSCNetworkReachabilityFlagsReachable) != 0) {
            if ((flags & kSCNetworkReachabilityFlagsConnectionRequired) == 0) {
                ret = NetworkWiFi;
            }
            if ((((flags & kSCNetworkReachabilityFlagsConnectionOnDemand) != 0) ||
                 (flags & kSCNetworkReachabilityFlagsConnectionOnTraffic) != 0))
            {
                if ((flags & kSCNetworkReachabilityFlagsInterventionRequired) == 0) {
                    ret = NetworkWiFi;
                }
            }
            if ((flags & kSCNetworkReachabilityFlagsIsWWAN) == kSCNetworkReachabilityFlagsIsWWAN) {
                ret = Network3G;
            }
        }
    }
    return ret;
}

void SystemUtil::hideMouseCursor()
{
    
}

void SystemUtil::updateGame(const char* url,const char* content)
{
    openURL(url);
}

double SystemUtil::availableMemory()
{
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
    if(kernReturn != KERN_SUCCESS)
    {
        return NSNotFound;
    }
    return vm_page_size*vmStats.free_count/1024.0/1024.0;
}

double SystemUtil::usedMemory()
{
    task_basic_info_32_data_t taskInfo;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
    kern_return_t kernReturn = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&taskInfo, &infoCount);
    if(kernReturn != KERN_SUCCESS)
    {
        return NSNotFound;
    }
    return taskInfo.resident_size/1024.0/1024.0;
}

double SystemUtil::getTotalMemory()
{
    return 0;
}

void SystemUtil::quitGame()
{
    exit(0);
}

bool SystemUtil::addSkipBackupAttributeToItemAtPath(std::string path)
{
    NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:path.c_str()]];
    
    
    if ([[NSFileManager defaultManager] fileExistsAtPath:[url path]] == NO) {
        return false;
    }
    
    NSError *error = nil;
    BOOL success = [url setResourceValue:[NSNumber numberWithBool:YES] forKey:NSURLIsExcludedFromBackupKey error:&error];
    
    if (!success) {
        NSLog(@"Error excluding %@ from backup %@", [url lastPathComponent], error);
    }
    
    return success;
}

bool SystemUtil::isFileSkipedBackUp(std::string path)
{
     NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:path.c_str()]];
    if ([[NSFileManager defaultManager] fileExistsAtPath:[url path]] == NO) {
        return false;
    }
    
    NSError *error = nil;
    id flag = nil;
    [url getResourceValue:&flag forKey:NSURLIsExcludedFromBackupKey error:&error];
    
    return [flag boolValue];
}

bool SystemUtil::copyStringToPasteboard(std::string content)
{
    UIPasteboard* pasteboard = [UIPasteboard generalPasteboard];
    [pasteboard setString:[NSString stringWithUTF8String:content.c_str()]];
    
    return true;
}