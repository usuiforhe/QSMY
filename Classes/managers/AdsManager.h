//
//  AdsManager.h
//  广告
//
//  Created by Eci on 13-11-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__AdsManager__
#define __QSMY__AdsManager__

#include "AppInclude.h"
#include "PluginManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "ProtocolAds.h"

class MyAdsListener : public cocos2d::CCObject, public cocos2d::plugin::AdsListener
{
    
public:
    
	virtual void onAdsResult(cocos2d::plugin::AdsResultCode code, const char* msg);

};

#endif


class AdsManager:public cocos2d::CCObject
{
    
    CC_SYNTHESIZE(bool, m_bEnableAd, EnableAd);
    
public:
    
    static AdsManager* sharedAdsManager(void);
    static void purgeAdsManager(void);
    
    bool init();

	void showAds(float fDuration = 0.0f);
	void hideAds();

	void callFuncWithParam(const char* funcName, cocos2d::plugin::PluginParam* param, ...);
    
private:
	AdsManager();
	~AdsManager();

	bool m_bShowingAds;
    cocos2d::CCNode *m_pHelperNode;
protected:
};


#endif /* defined(__QSMY__AdsManager__) */
