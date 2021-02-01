//
//  ShareManager.h
//  分享管理
//
//  Created by heven on 14-4-15.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__ShareManager__
#define __QSMY__ShareManager__

#include "AppInclude.h"
#include "PluginManager.h"
#include "ProtocolShare.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

class MyShareResultListener : public cocos2d::CCObject, public cocos2d::plugin::ShareResultListener
{
public:
    
	virtual void onShareResult(cocos2d::plugin::ShareResultCode ret, const char* msg);
    
};

#endif

class ShareManager:public cocos2d::CCObject
{
    
public:
    
    static ShareManager* sharedShareManager(void);
    static void purgeShareManager(void);
    
    bool init();
    
    /**
     *	@brief	发送分享信息
     *
     */
	void share(const char* msg, const char* openUrl);
    
    /**
     *	@brief	直接发送分享信息
     *
     *	@param 	pluginName 	插件名
     *
     *	@param 	pShareMsg 	分享内容
     *
     *	@param 	openUrl 	分享打开链接
     */
    void doShare(const char* pluginName,const char* pShareMsg,const char* openUrl);
    
    
    /**
     *	@brief	直接发送邀请信息（目前只有facebook用到）
     *
     *	@param 	pluginName 	插件名
     *
     *	@param 	pShareMsg 	分享内容
     *
     *	@param 	openUrl 	分享打开链接
     */
    void doInvite(const char* pluginName, const char* pShareMsg, const char* openUrl);
    
    std::string getInvitedFriendsId(const char* pluginName, const char* pShareMsg);

private:
	ShareManager();
	~ShareManager();
    
private:
    cocos2d::CCArray* m_pSharePlugins;//初始化成功的分享插件列表
};


#endif /* defined(__QSMY__ShareManager__) */
