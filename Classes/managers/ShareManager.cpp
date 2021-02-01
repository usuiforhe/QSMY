//
//  ShareManager.cpp
//  QSMY
//
//  Created by heven on 14-4-15.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "ShareManager.h"
#include "DataManager.h"
#include "../scenes/game/account/ShareAward.h"
#include "../scenes/game/Game.h"

USING_NS_CC;
using namespace plugin;

#define APPID "AppID"
#define SHARE_URL "ShareURL"


static bool m_bsharing(false);

// singleton stuff
static ShareManager *s_SharedShareManager = NULL;

ShareManager* ShareManager::sharedShareManager(void)
{
    if (!s_SharedShareManager)
    {
        s_SharedShareManager = new ShareManager();
        s_SharedShareManager->init();
    }
    
    return s_SharedShareManager;
}


void ShareManager::purgeShareManager(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedShareManager);
}

ShareManager::ShareManager()
:m_pSharePlugins(NULL)
{
    
};


#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
ShareManager::~ShareManager()
{
    m_bsharing = false;
}
bool ShareManager::init()
{
    return false;
}

void ShareManager::share(const char* msg, const char* imgPath)
{
	ShareAward* pShareAward = ShareAward::create(msg, imgPath, m_pSharePlugins);
    Game::sharedGame()->openPopup(pShareAward);
}

void ShareManager::doShare(const char* pluginName,const char* pShareMsg,const char* pImgPath)
{
    
}

void ShareManager::doInvite(const char *pluginName, const char *pShareMsg, const char *openUrl)
{

}

std::string ShareManager::getInvitedFriendsId(const char *pluginName, const char *pShareMsg)
{
	return "";
}

#else


ShareManager::~ShareManager()
{
	CCObject* obj = NULL;
	CCARRAY_FOREACH(m_pSharePlugins, obj)
	{
		share::ShareInfo* pShareInfo = static_cast<share::ShareInfo*>(obj);
        if (!pShareInfo)
		{
			continue;
		}
		PluginManager::getInstance()->unloadPlugin(pShareInfo->getKey().c_str());
		
	}
    CC_SAFE_RELEASE(m_pSharePlugins);
}


bool ShareManager::init()
{
    bool ret = false;
    
    CC_SAFE_RELEASE(m_pSharePlugins);
    m_pSharePlugins = CCArray::create();
    CC_SAFE_RETAIN(m_pSharePlugins);
    
	CCArray* pArray = DM_GET_SHARE_MODEL->getArrayForShare();
	CCObject* obj = NULL;
	CCARRAY_FOREACH(pArray, obj)
	{
		share::ShareInfo* pShareInfo = (share::ShareInfo*)obj;
		if (!pShareInfo)
		{
			continue;
		}

		ProtocolShare* pShare = dynamic_cast<ProtocolShare*>(PluginManager::getInstance()->loadPlugin(pShareInfo->getKey().c_str()));
		if (!pShare)
		{
			continue;
		}
        
#ifdef QSMY_DEBUG
		pShare->setDebugMode(true);
#endif
		TShareDeveloperInfo info;
		info[APPID] = pShareInfo->getAppKey();
        info[SHARE_URL] = pShareInfo->getShareUrl();
		pShare->configDeveloperInfo(info);
		pShare->setResultListener(new MyShareResultListener());
        m_pSharePlugins->addObject(pShareInfo);
		ret = true;
	}
    return ret;
}

void ShareManager::share(const char* msg, const char* openUrl)
{
	ShareAward* pShareAward = ShareAward::create(msg, openUrl, m_pSharePlugins);
	Game::sharedGame()->openPopup(pShareAward);
}

void ShareManager::doShare(const char* pluginName,const char* pShareMsg,const char* openUrl)
{
    //if(m_bsharing) return;
    m_bsharing = true;
    cocos2d::plugin::TShareInfo info;
	info["SharedText"] = pShareMsg;
	info["SharedURL"] = openUrl;
    
    ProtocolShare* pShare = dynamic_cast<ProtocolShare*>(PluginManager::getInstance()->loadPlugin(pluginName));
    if(pShare)
    {
        pShare->share(info);
    }
}

void ShareManager::doInvite(const char *pluginName, const char *pShareMsg, const char *openUrl)
{
    m_bsharing = true;
    cocos2d::plugin::TShareInfo info;
	info["SharedText"] = pShareMsg;
	info["SharedURL"] = openUrl;
    
    ProtocolShare* pShare = dynamic_cast<ProtocolShare*>(PluginManager::getInstance()->loadPlugin(pluginName));
    if(pShare)
    {
        pShare->invite(info);
    }
}

std::string ShareManager::getInvitedFriendsId(const char *pluginName, const char *pShareMsg)
{
    ProtocolShare* pShare = dynamic_cast<ProtocolShare*>(PluginManager::getInstance()->loadPlugin(pluginName));
    if(pShare)
    {
        return pShare->getInvitedFriendsId();
    }
    return "";
}

void MyShareResultListener::onShareResult(cocos2d::plugin::ShareResultCode ret, const char* msg)
{
    if(!m_bsharing) return;
    m_bsharing = false;
    std::string msgTag("INVITE_MSG_TAG");
    //发送分享成功消息 game接收之后调用领奖
    switch (ret) {
        case kShareSuccess:
            if (msgTag.compare(msg) == 0)
            {
                NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_INVITE_SUC);
            }
            else
            {
                NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHARED_SUC);
            }
            
            break;
        default:
            CCLOG("share failed:%s,%d",msg,ret);
            Tips_Alert(D_LOCAL_STRING("ShareFailed!").c_str());
            break;
    }
}


#endif