//
//  AnalyticsManager.cpp
//  统计
//
//  Created by Eci on 13-11-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "AnalyticsManager.h"
#include "utils/SystemUtil.h"
#include "utils/md5.h"
#include "DataManager.h"

USING_NS_CC;
using namespace plugin;

// singleton stuff
static AnalyticsManager *s_SharedAnalyticsManager = NULL;

AnalyticsManager* AnalyticsManager::sharedAnalyticsManager(void)
{
    if (!s_SharedAnalyticsManager)
    {
        s_SharedAnalyticsManager = new AnalyticsManager();
        s_SharedAnalyticsManager->init();
    }
    
    return s_SharedAnalyticsManager;
}


void AnalyticsManager::purgeAnalyticsManager(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedAnalyticsManager);
}

AnalyticsManager::AnalyticsManager()
:m_pOrderID("")
,m_isSysInited(false)
{
};



#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
AnalyticsManager::~AnalyticsManager() {}
bool AnalyticsManager::init()
{
    return false;
}

void AnalyticsManager::logErrorWithMsg(const char * pErrorID, const char *pMessage/* = NULL*/) {}
void AnalyticsManager::logEventWithMsg(const char * pEventID, const char *pMessage/* = NULL*/) {}
void AnalyticsManager::logEventWithParamMap(const char * pEventID, cocos2d::plugin::LogEventParamMap& paramMap) {}
void AnalyticsManager::logTimedEventBegin(const char * pEventID) {}
void AnalyticsManager::logTimedEventEnd(const char * pEventID) {}


void AnalyticsManager::initAccount(const char * pAccountID){}

void AnalyticsManager::setAccountName(const char * pAccountName){}

void AnalyticsManager::setAccountType(TDGAAccountType accountType){}

void AnalyticsManager::setAccountLevel(int level){}

void AnalyticsManager::setAccountGender(TDGAGender gender){}

void AnalyticsManager::setAccountAge(int age){}

void AnalyticsManager::setGameServer(const char *pGameServer){}

void AnalyticsManager::missionBegin(const char * pMissionID) {}
void AnalyticsManager::missionCompleted(const char * pMissionID) {}
void AnalyticsManager::missionFailed(const char * pMissionID, const char * pCause/* = NULL*/) {}
void AnalyticsManager::chargeRequst(const char * orderID,
                  const char * iapID,
                  double currencyAmount,
                  const char * currencyType,
                  double virtualCurrencyAmount,
                  const char * paymentType)
{}
void AnalyticsManager::chargeSuccess(const char * orderID) {}
void AnalyticsManager::reward(double virtualCurrencyAmount, const char* pReason) {}
void AnalyticsManager::purchaseItem(const char *pItem, int number, double price) {}
void AnalyticsManager::useItem(const char *pItem, int number) {}
void AnalyticsManager::dropItem(const char *pItem, int number) {}
void AnalyticsManager::tutorialStep(uint32_t iStep) {}
void AnalyticsManager::login(const char * pAccountName, int level) {}
void AnalyticsManager::logout(const char * pAccountName, int level) {}
void AnalyticsManager::callFuncWithParam(const char* funcName, PluginParam* param, ...){}

void AnalyticsManager::viewBegin(const char *sView) {}
void AnalyticsManager::viewEnd(const char *sView) {}
#else


AnalyticsManager::~AnalyticsManager()
{
	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAnalytics();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		ProtocolAnalytics* pAnalytics = dynamic_cast<ProtocolAnalytics*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (pAnalytics)
		{
			pAnalytics->stopSession();
			PluginManager::getInstance()->unloadPlugin(sKey.c_str());
		}
		
	}
}


bool AnalyticsManager::init()
{
    bool ret = false;

	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAnalytics();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		CCString *sVal = static_cast<CCString *>(pEle->getObject());
		ProtocolAnalytics* pAnalytics = dynamic_cast<ProtocolAnalytics*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (!pAnalytics)
		{
			continue;
		}
        
#ifdef QSMY_DEBUG
		pAnalytics->setDebugMode(true);
#endif
        
		cocos2d::plugin::PluginParam param(DM_GET_PLATFORM_MODEL->getConfig().getPlatform().c_str());
		pAnalytics->callFuncWithParam("setChannel",&param,NULL);

		cocos2d::plugin::PluginParam channelId(DM_GET_PLATFORM_MODEL->getConfig().getChannelId().c_str());
		pAnalytics->callFuncWithParam("setChannelId",&channelId,NULL);

		PluginParam enableParam(true);
		pAnalytics->callFuncWithParam("setSessionReportsOnCloseEnabled", &enableParam, NULL);
		pAnalytics->callFuncWithParam("setCrashReportingEnabled", &enableParam, NULL);

		pAnalytics->startSession(sVal->getCString());
		ret = true;
	}
    return ret;
}

//std::string AnalyticsManager::getDeviceId()
//{
//
//	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAnalytics();
//	CCDictElement *pEle = NULL;
//    std::string rt;
//	CCDICT_FOREACH(pDict, pEle)
//	{
//		std::string sKey = pEle->getStrKey();
//		ProtocolAnalytics* pAnalytics = dynamic_cast<ProtocolAnalytics*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
//		if (!pAnalytics)
//		{
//			continue;
//		}
//        rt = pAnalytics->callStringFuncWithParam("getDeviceId",NULL);
//		if(!rt.empty()) return rt;
//	}
//    /**************************************************/
//    //简单粗暴的模拟一个
//#define DEVICE_ID "KEY_GAMEID"
//    rt = CCUserDefault::sharedUserDefault()->getStringForKey(DEVICE_ID, "");
//    if(!rt.empty()) return rt;
//    
//    struct cc_timeval now;
//    CCTime::gettimeofdayCocos2d(&now, NULL);
//    rt = CCString::createWithFormat("%d-%d",now.tv_sec,now.tv_usec)->getCString();
//    rt = md5(rt);
//    CCUserDefault::sharedUserDefault()->setStringForKey(DEVICE_ID, rt);
//    CCUserDefault::sharedUserDefault()->flush();
//    return rt;
//}

void AnalyticsManager::logErrorWithMsg(const char * pErrorID, const char *pMessage/* = NULL*/)
{
	if (!m_isSysInited) return;
	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAnalytics();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		ProtocolAnalytics* pAnalytics = dynamic_cast<ProtocolAnalytics*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (!pAnalytics)
		{
			continue;
		}
		if (NULL == pMessage) {
			pAnalytics->logError(pErrorID, "");
		}
		else
		{
			pAnalytics->logError(pErrorID, pMessage);
		}
	}
}
void AnalyticsManager::logEventWithMsg(const char * pEventID, const char *pMessage/* = NULL*/)
{
	if (!m_isSysInited) return;
	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAnalytics();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		ProtocolAnalytics* pAnalytics = dynamic_cast<ProtocolAnalytics*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (!pAnalytics)
		{
			continue;
		}
		if (NULL == pMessage) {
			pAnalytics->logEvent(pEventID);
		}
		else
		{
			LogEventParamMap paramMap;
			paramMap["msg"] = pMessage;
			pAnalytics->logEvent(pEventID, &paramMap);
		}
	}
}
void AnalyticsManager::logEventWithParamMap(const char * pEventID, LogEventParamMap& paramMap)
{
	if (!m_isSysInited) return;
	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAnalytics();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		ProtocolAnalytics* pAnalytics = dynamic_cast<ProtocolAnalytics*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (!pAnalytics)
		{
			continue;
		}
		pAnalytics->logEvent(pEventID, &paramMap);
	}
}
void AnalyticsManager::logTimedEventBegin(const char * pEventID)
{
	if (!m_isSysInited) return;
	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAnalytics();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		ProtocolAnalytics* pAnalytics = dynamic_cast<ProtocolAnalytics*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (!pAnalytics)
		{
			continue;
		}
		pAnalytics->logTimedEventBegin(pEventID);
	}
}
void AnalyticsManager::logTimedEventEnd(const char * pEventID)
{
	if (!m_isSysInited) return;
	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAnalytics();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		ProtocolAnalytics* pAnalytics = dynamic_cast<ProtocolAnalytics*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (!pAnalytics)
		{
			continue;
		}
		pAnalytics->logTimedEventEnd(pEventID);
	}
}

void AnalyticsManager::initAccount(const char * pAccountID)
{
    if (pAccountID) {
        cocos2d::plugin::PluginParam param(pAccountID);
        callFuncWithParam("setAccountWithID",&param,NULL);
		callFuncWithParam("setAccountID", &param, NULL);
    }
}

void AnalyticsManager::setAccountName(const char * pAccountName)
{
    if (pAccountName) {
        cocos2d::plugin::PluginParam param(pAccountName);
        callFuncWithParam("setAccountName",&param,NULL);
    }
}

void AnalyticsManager::setAccountType(TDGAAccountType accountType)
{
    if (accountType) {
        cocos2d::plugin::PluginParam param(accountType);
        callFuncWithParam("setAccountType",&param,NULL);
    }
}

void AnalyticsManager::setAccountLevel(int level)
{
    if (level) {
        cocos2d::plugin::PluginParam param(level);
		callFuncWithParam("setLevel",&param,NULL);
    }
}

void AnalyticsManager::setAccountGender(TDGAGender gender)
{
    if (gender) {
        cocos2d::plugin::PluginParam param(gender);
        callFuncWithParam("setGender",&param,NULL);
    }
}

void AnalyticsManager::setAccountAge(int age)
{
    if (age) {
        cocos2d::plugin::PluginParam param(age);
        callFuncWithParam("setAge",&param,NULL);
    }
}

void AnalyticsManager::setGameServer(const char *pGameServer)
{
    if (pGameServer) {
        cocos2d::plugin::PluginParam param(pGameServer);
        callFuncWithParam("setServer",&param,NULL);
    }
}


void AnalyticsManager::missionBegin(const char * pMissionID)
{
    if (pMissionID) {
        logTimedEventBegin(pMissionID);
    }
}
void AnalyticsManager::missionCompleted(const char * pMissionID)
{
    if (pMissionID) {
        logTimedEventEnd(pMissionID);
    }
}
void AnalyticsManager::missionFailed(const char * pMissionID, const char * pCause/* = NULL*/)
{
    if (pMissionID) {
        cocos2d::plugin::PluginParam param(pMissionID);
        cocos2d::plugin::PluginParam paramCause(pCause);
        callFuncWithParam("onFailed",&param, &paramCause, NULL);
    }
}
void AnalyticsManager::chargeRequst(const char * orderID,
                                    const char * iapID,
                                    double currencyAmount,
                                    const char * currencyType,
                                    double virtualCurrencyAmount,
                                    const char * paymentType)
{
    if (orderID) {
        m_pOrderID = orderID;
        PluginParam param0 = PluginParam(orderID);
        PluginParam param1 = PluginParam(iapID);
        PluginParam param2 = PluginParam((float)currencyAmount);
        PluginParam param3 = PluginParam(currencyType);
        PluginParam param4 = PluginParam((float)virtualCurrencyAmount);
        PluginParam param5 = PluginParam(paymentType);
        callFuncWithParam("onChargeRequst",&param0, &param1, &param2, &param3, &param4, &param5, NULL);
    }
}
void AnalyticsManager::chargeSuccess(const char * orderID/* = NULL*/)
{
    cocos2d::CCString *userID = CCString::createWithFormat("%u", DM_GET_USER_MODEL->getUserInfo()->getUserId());
    cocos2d::plugin::PluginParam paramA(userID->getCString());
    if (orderID) {
        cocos2d::plugin::PluginParam param(orderID);
        callFuncWithParam("onChargeSuccess",&param,&paramA,NULL);
    }
    else if (m_pOrderID.length())
    {
        cocos2d::plugin::PluginParam param(m_pOrderID.c_str());
        callFuncWithParam("onChargeSuccess",&param,&paramA,NULL);
    }
}
void AnalyticsManager::reward(double virtualCurrencyAmount, const char* pReason)
{
    PluginParam param0 = PluginParam((float)virtualCurrencyAmount);
    PluginParam param1 = PluginParam(pReason);
    callFuncWithParam("onReward",&param0, &param1, NULL);
}
void AnalyticsManager::purchaseItem(const char *pItem, int number, double price /* = 0*/)
{
    if (pItem) {
        cocos2d::CCString *userID = CCString::createWithFormat("%u", DM_GET_USER_MODEL->getUserInfo()->getUserId());
        cocos2d::plugin::PluginParam paramA(userID->getCString());
        PluginParam param0 = PluginParam(pItem);
        PluginParam param1 = PluginParam(number);
        PluginParam param2 = PluginParam((float)price);
        callFuncWithParam("onPurchase",&param0, &param1, &param2, &paramA, NULL);
    }
}
void AnalyticsManager::useItem(const char *pItem, int number)
{
    if (pItem) {
        PluginParam param0 = PluginParam(pItem);
        PluginParam param1 = PluginParam(number);
        callFuncWithParam("onUse",&param0, &param1, NULL);
    }
}

void AnalyticsManager::dropItem(const char *pItem, int number)
{
    LogEventParamMap map;
    map["item"] = CCString::createWithFormat("%s", pItem)->getCString();
    map["count"] = CCString::createWithFormat("%d", number)->getCString();
    this->logEventWithParamMap("onDrop",map);
}

void AnalyticsManager::tutorialStep(uint32_t iStep)
{
	logEventWithMsg("Tutorial", CCString::createWithFormat("%u", iStep)->getCString());
}


void AnalyticsManager::login(const char * pAccountName, int level)
{
    if (pAccountName) {
        PluginParam param0 = PluginParam(pAccountName);
        PluginParam param1 = PluginParam(level);
        callFuncWithParam("roleLoginWithAccount",&param0, &param1, NULL);
    }
}

void AnalyticsManager::logout(const char * pAccountName, int level)
{
    if (pAccountName) {
        PluginParam param0 = PluginParam(pAccountName);
        PluginParam param1 = PluginParam(level);
        callFuncWithParam("roleLogoutWithAccount",&param0, &param1, NULL);
    }
}


void AnalyticsManager::viewBegin(const char *sView)
{
    if (sView) {
        cocos2d::plugin::PluginParam param(sView);
        callFuncWithParam("viewBegin",&param,NULL);
    }
}
void AnalyticsManager::viewEnd(const char *sView)
{
    if (sView) {
        cocos2d::plugin::PluginParam param(sView);
        callFuncWithParam("viewEnd",&param,NULL);
    }
}

void AnalyticsManager::callFuncWithParam(const char* funcName, PluginParam* param, ...)
{
	if (!m_isSysInited)
	{
		return;
	}
	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAnalytics();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		ProtocolAnalytics* pAnalytics = dynamic_cast<ProtocolAnalytics*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (!pAnalytics)
		{
			continue;
		}
		std::vector<PluginParam*> allParams;
		if (NULL != param)
		{
			allParams.push_back(param);

			va_list argp;
			PluginParam* pArg = NULL;
			va_start( argp, param );
			while (1)
			{
				pArg = va_arg(argp, PluginParam*);
				if (pArg == NULL)
					break;

				allParams.push_back(pArg);
			}
			va_end(argp);
		}

		pAnalytics->callFuncWithParam(funcName, allParams);
	}
}

#endif

