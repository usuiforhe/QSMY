//
//  PlatformManager.h
//  QSMY
//
//  Created by wanghejun on 13-9-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PlatformManager__
#define __QSMY__PlatformManager__

#define TAIWANG_GOOGLE_LABLE "UserTaiwan"

#include "AppInclude.h"
#include "model/PlatformModel.h"
#include "scenes/login/LoginInclude.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PluginManager.h"
#include "ProtocolUser.h"
#include "ProtocolIAP.h"
using namespace cocos2d::plugin;


class MyUserActionResult : public UserActionListener
{
public:
    virtual void onActionResult(cocos2d::plugin::ProtocolUser* pPlugin, cocos2d::plugin::UserActionResultCode code, const char* msg);
};

class MyPayResultListener : public cocos2d::CCObject, public PayResultListener
{
public:
    virtual void onPayResult(PayResultCode ret, const char* msg, TProductInfo info);
    
    void onGetResult(const Json::Value &requestData, const Json::Value &responseData);
    void retryGetResult(float dt);
    
};

#endif


class PlatformManager:public cocos2d::CCObject
{
    CC_SYNTHESIZE(std::string, m_sUserId, UserId);
    
public:
    
    static PlatformManager* sharedPlatformManager(void);
    static void purgePlatformManager(void);
    
    bool init();
    bool login();
    bool loginByType(LoginType loginType);
    /**
     *	@brief	是否使用游客登录
     *
     *	@return
     */
    bool isGuess();

    void showUserCenter();
    void showFourm();

    bool buy(platform::RechargeInfo* pInfo, uint32_t billno,const std::string &token);
    
    bool openURL(const char* url,const char* sourceApplication);
    
    void showBindAccount();
    //发送应用程式邀请
    void sendAppInvite();
    
    bool isLogined();
    
    bool isInternal()
    {
        return m_bInternal;
    }
    
    
    /**
     *	@brief	获取登陆参数
     *
     *	@return	
     */
    std::string getLoginParam();
    
    /**
     *	@brief	获取登陆回调参数
     *
     *	@return
     */
    std::string getLoginCBParam();
    
    
    /**
     *	@brief	获取最近登录服务器参数
     *
     *	@return
     */
    std::string getServersParam();
    
    //uint32_t getUserId();
    
    /**
     *	@brief	设置LoginCB回调
     *
     *	@param 	val 	logincb 返回值
     */
    void setLoginCallBack(const Json::Value& val);
    
    
    /**
     *	@brief	给插件提供更多游戏信息
     *
     */
    void setServerInfoToPlugin();

	std::string getDefaultLoginParam();

	std::string getFacebookUserName();

	void saveAccountInfo(const Json::Value& val);
private:
    
    PlatformManager();
    ~PlatformManager();
    
    
private:
    bool m_bInternal;
    
};


#endif /* defined(__QSMY__PlatformManager__) */
