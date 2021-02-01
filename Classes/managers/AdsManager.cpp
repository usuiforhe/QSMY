//
//  AdsManager.cpp
//  广告
//
//  Created by Eci on 13-11-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "AdsManager.h"
#include "utils/SystemUtil.h"
#include "utils/md5.h"
#include "DataManager.h"

#define APPID "AppID"

USING_NS_CC;
using namespace plugin;

// singleton stuff
static AdsManager *s_SharedAdsManager = NULL;

AdsManager* AdsManager::sharedAdsManager(void)
{
    if (!s_SharedAdsManager)
    {
        s_SharedAdsManager = new AdsManager();
        s_SharedAdsManager->init();
    }
    
    return s_SharedAdsManager;
}


void AdsManager::purgeAdsManager(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedAdsManager);
}

AdsManager::AdsManager()
    :m_bShowingAds(false)
    ,m_bEnableAd(false)
    ,m_pHelperNode(NULL)
{
};



#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
AdsManager::~AdsManager()
{
    CC_SAFE_RELEASE(m_pHelperNode);
}
bool AdsManager::init()
{
    return false;
}

void AdsManager::showAds(float fDuration /* = 0 */) {}
void AdsManager::hideAds() {}

void AdsManager::callFuncWithParam(const char* funcName, PluginParam* param, ...){}
#else


AdsManager::~AdsManager()
{
	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAds();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		ProtocolAds* pAds = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (pAds)
		{
			TAdsInfo info;
			pAds->hideAds(info);
			PluginManager::getInstance()->unloadPlugin(sKey.c_str());
		}
		
	}
    CC_SAFE_RELEASE(m_pHelperNode);
}


bool AdsManager::init()
{
    bool ret = false;

	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAds();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		CCString *sVal = static_cast<CCString *>(pEle->getObject());
		ProtocolAds* pAds = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (!pAds)
		{
			continue;
		}
        
#ifdef QSMY_DEBUG
		pAds->setDebugMode(true);
#endif
		TAdsDeveloperInfo info;
		info[APPID] = sVal->getCString();
		pAds->configDeveloperInfo(info);
		pAds->setAdsListener(new MyAdsListener());
		ret = true;
	}
    if(m_pHelperNode)
    {
        m_pHelperNode->cleanup();
        m_pHelperNode->onExit();
        CC_SAFE_RELEASE(m_pHelperNode);
    }
    m_pHelperNode = CCNode::create();
    CC_SAFE_RETAIN(m_pHelperNode);
    m_pHelperNode->onEnter();
    return ret;
}

void AdsManager::showAds(float fDuration /* = 0 */)
{
    if(!m_bEnableAd || m_bShowingAds) return;
	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAds();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		ProtocolAds* pAds = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (!pAds)
		{
			continue;
		}
		TAdsInfo info;
		pAds->showAds(info);
	}
    
    if (!D_FLOAT_EQUALS(fDuration, 0.f))
    {
        CCAction* pAction = CCSequence::create(CCDelayTime::create(fDuration),
                                               CCCallFunc::create(this, callfunc_selector(AdsManager::hideAds)),
                                               NULL
                                               );
        m_pHelperNode->runAction(pAction);
        return;
    }
	m_bShowingAds = true;
}

void AdsManager::hideAds()
{
	m_bShowingAds = false;
	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAds();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		ProtocolAds* pAds = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (!pAds)
		{
			continue;
		}
		TAdsInfo info;
		pAds->hideAds(info);
	}
    m_pHelperNode->stopAllActions();
}

void AdsManager::callFuncWithParam(const char* funcName, PluginParam* param, ...)
{
	CCDictionary *pDict = DM_GET_PLATFORM_MODEL->getDictForAds();
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(pDict, pEle)
	{
		std::string sKey = pEle->getStrKey();
		ProtocolAds* pAds = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin(sKey.c_str()));
		if (!pAds)
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

		pAds->callFuncWithParam(funcName, allParams);
	}
}

void MyAdsListener::onAdsResult(AdsResultCode code, const char* msg)
{

}

#endif

