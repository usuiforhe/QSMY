//
//  PlatformManager.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PlatformManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "utils/SystemUtil.h"
#include "utils/TimerUtil.h"
#include "utils/URLUtil.h"
#include "scenes/login/LoginInclude.h"
#include "json/json.h"

USING_NS_CC;

static bool m_bPaying = false;

// singleton stuff
static PlatformManager *s_SharedPlatformManager = NULL;

PlatformManager* PlatformManager::sharedPlatformManager(void)
{
    if (!s_SharedPlatformManager)
    {
        s_SharedPlatformManager = new PlatformManager();
        s_SharedPlatformManager->init();
    }
    
    return s_SharedPlatformManager;
}


void PlatformManager::purgePlatformManager(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedPlatformManager);
}

PlatformManager::PlatformManager()
:m_bInternal(false)
{
    
}


std::string PlatformManager::getDefaultLoginParam()
{
    std::string userName(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME, "").c_str());
    if (userName.compare("") == 0) {
        userName.assign(m_sUserId);
    }
    
    if (userName.length() == 0) {
        CCLOG("username length = 0, in getDefaultLoginParam");
    }
    
	return CCString::createWithFormat("u=%s&p=%s&c=%s&et=%s", userName.c_str(),CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PASSWORD,"").c_str(),SystemUtil::getUDID().c_str(),UrlEncode(SystemUtil::getDeviceInfo()).c_str())->getCString();
}



#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
bool PlatformManager::init() {m_bInternal = true; return true;}
bool PlatformManager::login(){ return false;}
bool PlatformManager::loginByType(LoginType loginType){ return false;}
bool PlatformManager::buy(platform::RechargeInfo* pInfo, uint32_t billno,const std::string &token){ return false;}
bool PlatformManager::isLogined(){return true;}
std::string PlatformManager::getLoginParam()
{
    return getDefaultLoginParam();
}

std::string PlatformManager::getServersParam()
{
	json_serializer a;
	std::string platId = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_PLATID,"");
	std::string platform = DM_GET_PLATFORM_MODEL->getConfig().getServerPlatformId();
	if(isGuess())
	{
		platform = "1";
	}
	a<<"platId"<<platId.c_str()<<"plat"<<platform.c_str();
	return a.toString();
}

void PlatformManager::sendAppInvite(){}

std::string PlatformManager::getLoginCBParam()
{
    return getDefaultLoginParam();
}

void PlatformManager::setServerInfoToPlugin()
{
    
}

void PlatformManager::showBindAccount(){}


bool PlatformManager::isGuess()
{
	if(CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE) == kQuickLogin) return true;
    return false;
}

//uint32_t PlatformManager::getUserId(){return 0;}

void PlatformManager::showUserCenter(){}
PlatformManager::~PlatformManager(){ m_bPaying = false;}

void PlatformManager::showFourm(){}

void PlatformManager::setLoginCallBack(const Json::Value& val){};
void PlatformManager::saveAccountInfo(const Json::Value& val){};
std::string PlatformManager::getFacebookUserName(){return "";}
#else


bool PlatformManager::init()
{
    CCLOG("User Plugin:%s", DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str());
	ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
	if (NULL != pUser)
	{
#ifdef QSMY_DEBUG
		pUser->setDebugMode(true);
#endif
		pUser->configDeveloperInfo(DM_GET_PLATFORM_MODEL->getParameter());
        
		pUser->setActionListener(new MyUserActionResult());
        
        //to do check
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (pUser->isUseInternalLogin()) {
            m_bInternal = true;
        }
//#endif
	}else{
		m_bInternal = true;
	}
	ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getIAP().c_str()));
	if (NULL != pIAP)
	{
#ifdef QSMY_DEBUG
		pIAP->setDebugMode(true);
#endif
		pIAP->configDeveloperInfo(DM_GET_PLATFORM_MODEL->getParameter());
        
		pIAP->setResultListener(new MyPayResultListener());
	}
	return true;
}

//第一章結束彈出用戶綁定提示 台灣版本
void PlatformManager::showBindAccount(){
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(pUser && !m_bInternal){
         pUser->callFuncWithParam("bindAccount",NULL);
    }
}
//应用程式邀请
void PlatformManager::sendAppInvite(){
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(pUser){
        pUser->callFuncWithParam("sendAppInvite",NULL);
    }
}

PlatformManager::~PlatformManager()
{
    m_bPaying = false;
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(pUser)
    {
        MyUserActionResult* _listener = static_cast<MyUserActionResult*>(pUser->getActionListener());
        CC_SAFE_DELETE(_listener);
        PluginManager::getInstance()->unloadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str());
    }
    
    
    ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getIAP().c_str()));
    if(pIAP)
    {
        MyPayResultListener* _listener = static_cast<MyPayResultListener*>(pIAP->getResultListener());
        CC_SAFE_DELETE(_listener);
        PluginManager::getInstance()->unloadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getIAP().c_str());
    }
}

void PlatformManager::setLoginCallBack(const Json::Value& val)
{
    //m_sUserId = val["plat_id"].asCString();
	//CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_PLATID, m_sUserId);
    std::string tokken = CCString::createWithFormat("e=%s&s=%s",val["plat_auth"]["expire"].asString().c_str(),val["plat_auth"]["sig"].asString().c_str())->getCString();
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_TOKKEN,tokken);
	CCUserDefault::sharedUserDefault()->flush();
    saveAccountInfo(val);
    StringMap map;
    if(val.isMember("access_token")) map["access_token"] = val["access_token"].asCString();
    if(val.isMember("refresh_token")) map["refresh_token"] = val["refresh_token"].asCString();
    if(val.isMember("expires_in")) map["expires_in"] = val["expires_in"].asCString();
    if(val.isMember("plat_name")) map["plat_name"] = val["plat_name"].asCString();
    map["refresh_token_url"] = CCString::createWithFormat("%s/%s",DM_GET_PLATFORM_MODEL->getPlatformURL().c_str(),API_REFRESH_TOKEN)->getCString();
    cocos2d::plugin::PluginParam param(map);
    
    ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getIAP().c_str()));
    if(pIAP) pIAP->callFuncWithParam("GameLoginCB",&param,NULL);
    
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(pUser) pUser->callFuncWithParam("GameLoginCB",&param,NULL);
}

void PlatformManager::saveAccountInfo(const Json::Value& val)
{
	if(val.isMember("account_type"))
	{	
		const char* pAccountType = val["account_type"].asCString();
		LoginType pType = kLogin;
		if(strcmp(pAccountType,"facebook") == 0)
		{
			pType = kFacebookLogin;
		} else if(strcmp(pAccountType,"google") == 0)
		{
			pType = kGoogelPlusLogin;
		} else if(strcmp(pAccountType,"guess") == 0)
		{
			pType = kQuickLogin;
		}else if(strcmp(pAccountType,"wegame") == 0)
		{
			pType = kLogin;
		}
		CCLOG("logined user--saved AccountInfo  %s, %d", pAccountType, pType);
		CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE, pType);
	}
	CCUserDefault::sharedUserDefault()->flush();
}

//uint32_t PlatformManager::getUserId()
//{
//    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
//    if(!pUser) return 0;
//    return pUser->callIntFuncWithParam("currentUserId", NULL);
//}



std::string PlatformManager::getFacebookUserName()
{
	std::string facebookName("");
	ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
	if(pUser)
	{
		facebookName = pUser->callStringFuncWithParam("getFacebookName",NULL);
	}

	return facebookName;
}

std::string PlatformManager::getLoginParam()
{
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(!pUser) return getDefaultLoginParam();
	if(isGuess() && m_bInternal){
		m_sUserId = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_PLATID,"");
	} else {
		m_sUserId = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME,"");
	}
    
    return cocos2d::CCString::createWithFormat("u=%s&p=%s&c=%s&et=%s&%s",
                                               m_sUserId.c_str(),
                                               CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PASSWORD,"").c_str(),
                                               SystemUtil::getUDID().c_str(),
                                               UrlEncode(SystemUtil::getDeviceInfo()).c_str(),
                                               CCUserDefault::sharedUserDefault()->getStringForKey(KEY_TOKKEN).c_str())->getCString();
}


std::string PlatformManager::getLoginCBParam()
{
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(!pUser) return getDefaultLoginParam();
	std::string uid = "";
    std::string email = "";
	if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)
	{
		int iLoginType = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE, kLogin);
		uid = pUser->callStringFuncWithParam("getUserId", NULL);
		email = pUser->callStringFuncWithParam("getUsrEmail", NULL);
		std::string platId = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_PLATID,"");
		if(iLoginType == kFacebookLogin || iLoginType == kGoogelPlusLogin){
            platform::UserLoginInfo* pUserInfo = DM_GET_PLATFORM_MODEL->getLoginedUserByName(platId.c_str(), iLoginType);
			if(pUserInfo == NULL)
			{
				platId = "";
			} else
			{
				if(uid.empty()) uid = pUserInfo->getUniqueId();
				if(email.empty()) email = pUserInfo->getEmail();
			}
		}
        
		if(!uid.empty()){
			CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_UNIQUEID, uid);
		}
		if(!email.empty()){
			CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_EMAIL, email);
		} 
		CCUserDefault::sharedUserDefault()->flush();
        
        return cocos2d::CCString::createWithFormat("u=%s&c=%s&et=%s&t=%s&lt=%d&unique_id=%s&email=%s",
                                                   platId.c_str(),SystemUtil::getUDID().c_str(),
                                                   UrlEncode(SystemUtil::getDeviceInfo()).c_str(),
                                                   pUser->getSessionID().c_str(),
                                                   CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE),
                                                   uid.c_str(),
                                                   email.c_str()
                                                   )->getCString()
                                                    ;
	}
    
    return cocos2d::CCString::createWithFormat("u=%s&c=%s&et=%s&t=%s&lt=%d&email=%s",
                                               m_sUserId.c_str(),SystemUtil::getUDID().c_str(),
                                               UrlEncode(SystemUtil::getDeviceInfo()).c_str(),
                                               pUser->getSessionID().c_str(), CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE), uid.c_str())->getCString();
}

std::string PlatformManager::getServersParam()
{
   /* if(m_bInternal)
    {
        return cocos2d::CCString::createWithFormat("u=%s&p=%s",
                                                   CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME, "").c_str(),
                                                   DM_GET_PLATFORM_MODEL->getConfig().getServerPlatformId().c_str())->getCString();
    }
	std::string platform = DM_GET_PLATFORM_MODEL->getConfig().getServerPlatformId();
	if(isGuess())
	{
		platform = "1";
	}
	if(m_sUserId.empty()){
		m_sUserId = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME,"");
	}
    return cocos2d::CCString::createWithFormat("u=%s&p=%s",
                                               m_sUserId.c_str(),
                                               platform.c_str())->getCString();*/
	json_serializer a;
	std::string platId = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_PLATID,"");
	std::string platform = DM_GET_PLATFORM_MODEL->getConfig().getServerPlatformId();
	if(isGuess())
	{
		platform = "1";
	}
	a<<"platId"<<platId.c_str()<<"plat"<<platform.c_str();
	return a.toString();
}

void PlatformManager::showUserCenter()
{
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(!pUser || m_bInternal) return;
    pUser->callFuncWithParam("showCenter", NULL);
}


bool PlatformManager::login()
{
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(!pUser || m_bInternal) return false;
    pUser->login();
    return true;
}

bool PlatformManager::loginByType(LoginType loginType)
{
	cocos2d::plugin::PluginParam param((int)loginType);
	ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
	if(!pUser) return false;
	pUser->callFuncWithParam("loginWithType",&param,NULL);
	return true;
}

bool PlatformManager::isGuess(){
	if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)
	{
		if(CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE, kLogin) == kQuickLogin){ return true;}
		else return false;
	}
	
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(!pUser) return false; //没有平台 直接设置为非游客状态
    return pUser->callBoolFuncWithParam("isGuessLogin", NULL);
}

bool PlatformManager::isLogined(){
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(!pUser || m_bInternal) return true; //没有平台 直接登陆状态
    return pUser->isLogined();
}

bool PlatformManager::buy(platform::RechargeInfo* pRechargeInfo, uint32_t billno,const std::string &token)
{
    CCLOG("IAP Plugin:%s", DM_GET_PLATFORM_MODEL->getConfig().getIAP().c_str());
	ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getIAP().c_str()));
    if(!pIAP) return false;
    
    
    if (pIAP->isPaying()) {
        Tips_Alert(D_LOCAL_STRING("NowIsPaying"));
        return false;
    }
    
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_PAY_START);
    m_bPaying = true;
    TProductInfo pInfo;
    pInfo["ID"] = pRechargeInfo->getIdentifier();//商品ID
    if (pRechargeInfo->getDes().length() > 0) {
        pInfo["name"] = CCString::createWithFormat("%s",pRechargeInfo->getDes().c_str())->getCString();
    }
    else
    {
        pInfo["name"] = CCString::createWithFormat("%d",pRechargeInfo->getPoint())->getCString();//商品名称(商品名称用的商品价格代表)
    }
    
    pInfo["desc"] = pRechargeInfo->getRechargeName();//商品描述（商品描述 用于平台显示）
    pInfo["price"] = pRechargeInfo->getPrice();//商品价格
    pInfo["billNO"] = CCString::createWithFormat("%d",billno)->getCString();//游戏订单号
	pInfo["token"] = token;
	pInfo["platformUserId"] = m_sUserId;//用户在平台的账号id
    pIAP->payForProduct(pInfo);
    return true;
}

void PlatformManager::showFourm()
{
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(!pUser || m_bInternal) return;
    cocos2d::plugin::PluginParam param(DM_GET_PLATFORM_MODEL->getConfig().getFourmURL().c_str());
    pUser->callFuncWithParam("showFourm",&param,NULL);
}

void PlatformManager::setServerInfoToPlugin()
{
    StringMap pInfo;
	pInfo["userId"] = m_sUserId;
    pInfo["serverId"] = DM_GET_PLATFORM_MODEL->getCurrentServer()->getID();
    pInfo["serverUrl"] = DM_GET_PLATFORM_MODEL->getPlatformURL();
    
    cocos2d::plugin::PluginParam param(pInfo);
    ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getIAP().c_str()));
    if(pIAP) pIAP->callFuncWithParam("setServerInfo",&param,NULL);
    
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(pUser && !m_bInternal) pUser->callFuncWithParam("setServerInfo",&param,NULL);
}

bool PlatformManager::openURL(const char* url,const char* sourceApplication){
    ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str()));
    if(!pUser)
    return false;
    cocos2d::plugin::PluginParam param(url);
    cocos2d::plugin::PluginParam param1(sourceApplication);
    return pUser->callBoolFuncWithParam("openFaceBookURL",&param,&param1,NULL);
}

void MyUserActionResult::onActionResult(ProtocolUser* pPlugin, UserActionResultCode code, const char* msg)
{
    //char userStatus[1024] = { 0 };
    switch (code)
    {
        case kLoginSucceed:
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_LOGIN_SUC);
        case kLoginFailed:
        //sprintf(userStatus, "User of \"%s\" login %s", pPlugin->getPluginName(), (code == kLoginSucceed)? "Successed" : "Failed");
        break;
        case kLogoutSucceed:
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_LOGOUT_SUC);
        //sprintf(userStatus, "User of \"%s\" logout", pPlugin->getPluginName());
        //SceneManager::goScene(SceneManager::kSceneTagLogin);
        break;
        case kFetchingFacebookEmail:
            NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_FETCHING_FACEBOOK_EMAIL);
        break;
        case kFetchFacebookEmailDone:
            NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_FETCH_FACEBOOK_EMAIL_DONE);
        break;
        default:
        break;
    }
    //CCMessageBox(msg, userStatus);
    
    // get session ID
    // std::string sessionID = pPlugin->getSessionID();
    //CCLOG("User Session ID of plugin %s is : %s", pPlugin->getPluginName(), sessionID.c_str());
    
    //std::string strStatus = pPlugin->isLogined() ? "online" : "offline";
    //CCLOG("User status of plugin %s is : %s", pPlugin->getPluginName(), strStatus.c_str());
}

static int retryCount = 0;
static PayResultCode orderState = kPayStart;

#define MAX_RETRY_COUNT 1
#define RETRY_DELAY 5 //秒

void MyPayResultListener::onPayResult(PayResultCode ret, const char* msg, TProductInfo info)
{
    orderState = ret;
    switch (ret) {
        case  kPaySuccess:
        case kPayCheckOrder:
        retryCount = 0;
        HTTP_CLIENT->getChargeResult(this,callfuncJson_selector(MyPayResultListener::onGetResult));
        break;
        case  kPayFail:
        Tips_Alert(D_LOCAL_STRING("PayFailed"));
        break;
        case  kPayCancel:
        break;
        case  kPayTimeOut:
        break;
        default:
        break;
    }
    if(m_bPaying)
    {
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_PAY_COMPLETE);
        m_bPaying = false;
    }
    //CCLOG("%d_%s",ret,msg);
}

void MyPayResultListener::onGetResult(const Json::Value &requestData, const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0 || !responseData["data"].isMember("cAmount") || 0 == responseData["data"]["cAmount"].asUInt())
    {
        if (orderState == kPayCheckOrder) {
            return;
        }
        
        if(retryCount<MAX_RETRY_COUNT)
        {
            HTTP_CLIENT->getBusyLayer()->showBusy();
            retryCount++;
            TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(MyPayResultListener::retryGetResult), this, RETRY_DELAY);
        }
    }
}

void MyPayResultListener::retryGetResult(float dt)
{
    HTTP_CLIENT->getBusyLayer()->hideBusy();
    HTTP_CLIENT->getChargeResult();
}

#endif

