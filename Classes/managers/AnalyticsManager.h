//
//  AnalyticsManager.h
//  统计
//
//  Created by Eci on 13-11-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__AnalyticsManager__
#define __QSMY__AnalyticsManager__

#include "AppInclude.h"
#include "PluginManager.h"
#include "ProtocolAnalytics.h"



// 帐户类型
typedef enum {
    kAccountAnonymous = 0,      // 匿名帐户
    kAccountRegistered = 1,     // 显性注册帐户
    kAccountSianWeibo = 2,      // 新浪微博
    kAccountQQ = 3,             // QQ帐户
    kAccountTencentWeibo = 4,   // 腾讯微博
    kAccountND91 = 5,           // 91帐户
    kAccountType1 = 11,         // 预留1
    kAccountType2 = 12,         // 预留2
    kAccountType3 = 13,         // 预留3
    kAccountType4 = 14,         // 预留4
    kAccountType5 = 15,         // 预留5
    kAccountType6 = 16,         // 预留6
    kAccountType7 = 17,         // 预留7
    kAccountType8 = 18,         // 预留8
    kAccountType9 = 19,         // 预留9
    kAccountType10 = 20         // 预留10
} TDGAAccountType;

// 性别
typedef enum {
    kGenderUnknown = 0,     // 未知
    kGenderMale = 1,        // 男
    kGenderFemale = 2       // 女
} TDGAGender;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
using namespace cocos2d::plugin;

#endif


class AnalyticsManager:public cocos2d::CCObject
{
    
public:
    
    static AnalyticsManager* sharedAnalyticsManager(void);
    static void purgeAnalyticsManager(void);
    
    bool init();
    
    void initAccount(const char * pAccountID);                      //done
    void setAccountName(const char * pAccountName);                 //done
    void setAccountLevel(int level);                                //done
    void setGameServer(const char *pGameServer);                    //done
    void setAccountAge(int age);                                    //not used
    void setAccountType(TDGAAccountType accountType);               //not used
    void setAccountGender(TDGAGender gender);                       //not used
    void logErrorWithMsg(const char * pErrorID, const char *pMessage = NULL);       //done
    void logEventWithMsg(const char * pEventID, const char *pMessage = NULL);
    void logEventWithParamMap(const char * pEventID, cocos2d::plugin::LogEventParamMap& paramMap);
    void logTimedEventBegin(const char * pEventID);
    void logTimedEventEnd(const char * pEventID);
    void setLatitudeAndLongitude(double latitude, double longitude) {};             //not used
    void setVerboseLogEnabled() {};                                                 //not used
    void missionBegin(const char * pMissionID);
    void missionCompleted(const char * pMissionID);
    void missionFailed(const char * pMissionID, const char * pCause = NULL);
    void chargeRequst(const char * orderID,                                         //done
                      const char * iapID,
                      double currencyAmount,
                      const char * currencyType,
                      double virtualCurrencyAmount,
                      const char * paymentType);
    void chargeSuccess(const char * orderID = NULL);                                //done
    void reward(double virtualCurrencyAmount, const char* pReason);                 //not used
    void purchaseItem(const char *pItem, int number, double price = 0);
    void useItem(const char *pItem, int number);
    
    void dropItem(const char *pItem, int number);
	void sellItem(const char *pItem, int number) {};								//not used

	void tutorialStep(uint32_t iStep);

    void login(const char * pAccountName, int level);
    void logout(const char * pAccountName, int level);
    
    void viewBegin(const char *sView);
    void viewEnd(const char *sView);
    
	void callFuncWithParam(const char* funcName, cocos2d::plugin::PluginParam* param, ...);
    
    //std::string getDeviceId();
private:
    std::string m_pOrderID;

	AnalyticsManager();
	~AnalyticsManager();
protected:
	CC_SYNTHESIZE(bool, m_isSysInited, SysInited);
};


#endif /* defined(__QSMY__AnalyticsManager__) */
