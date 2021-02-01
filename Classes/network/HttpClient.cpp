//
//  HttpClient.cpp
//  QSMY
//
//  Created by wanghejun on 13-4-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "HttpClient.h"
#include "AppConsts.h"
#include "HttpHandler.h"
#include "AppEventType.h"
#include "managers/GameManager.h"
#include "utils/Notification.h"
#include "managers/TipsManager.h"
#include "managers/PlatformManager.h"
#include "AppMacros.h"
#include "utils/URLUtil.h"
#include "utils/FileUtil.h"
#include "utils/base64.h"
#include "utils/md5.h"

USING_NS_CC;
USING_NS_CC_EXT;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
static const char* SYSTEM_SIGN ="ios";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
static const char* SYSTEM_SIGN ="android";
#else
static const char* SYSTEM_SIGN ="ios";
#endif


HttpClient::HttpClient()
:http(NULL)
,m_pHttpHandler(NULL)
,m_nScriptHandler(0)
,m_pBusyLayer(NULL)
,m_pFailedRequestList(NULL)
{
    
}

HttpClient::~HttpClient()
{
    CCHttpClient::destroyInstance();
    CC_SAFE_RELEASE(m_pHttpHandler);
    CC_SAFE_RELEASE(m_pBusyLayer);
    CC_SAFE_RELEASE(m_pFailedRequestList);
}

void HttpClient::setHttpHandler(HttpHandler *var)
{
    if(var) var->retain();
    if(m_pHttpHandler) m_pHttpHandler->release();
    m_pHttpHandler = var;
}

void HttpClient::setHttpHandler(int nHandler)
{
    if (m_nScriptHandler)
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nScriptHandler);
        LUALOG("[LUA] Remove HttpClient script handler: %d", m_nScriptHandler);
        m_nScriptHandler = 0;
    }
    m_nScriptHandler = nHandler;
    LUALOG("[LUA] Add HttpClient script handler: %d", m_nScriptHandler);
}

void HttpClient::reset()
{
    if(m_pFailedRequestList) m_pFailedRequestList->removeAllObjects();
}

std::string HttpClient::getShareSign(const char* parameter)
{
    std::string content;
    content.assign(CCString::createWithFormat("%d", DM_GET_USER_MODEL->getUserInfo()->getUserId())->getCString());
    if (parameter) {
        content.append(parameter);
    }
    content.append(SHARE_KEY);
    
    std::string sign = md5(content.c_str());
    return sign;
}



void HttpClient::retryFailedRequest()
{
    CCDictElement *pElement = NULL;
    HttpRequest* pRequest = NULL;
    CCDICT_FOREACH(m_pFailedRequestList, pElement)
    {
        pRequest = static_cast<HttpRequest*>(pElement->getObject());
        doRequset(pRequest);
    }
    m_pFailedRequestList->removeAllObjects();
}

void HttpClient::doRequset(HttpRequest* request)
{
    if(request->getShowBusy()) m_pBusyLayer->showBusy();
    http->send(request);
}

bool HttpClient::init()
{
    m_pBusyLayer = BusyLayer::create();
    CC_SAFE_RETAIN(m_pBusyLayer);
    
    m_pFailedRequestList = CCDictionary::create();
    CC_SAFE_RETAIN(m_pFailedRequestList);
    
    http = CCHttpClient::getInstance();
    if (CCFileUtils::sharedFileUtils()->isFileExist(COOKIE_PATH) == false) {
        FILE* pFile = fopen((COOKIE_PATH).c_str(), "wb+");
        if (pFile) {
            fclose(pFile);
            SystemUtil::addSkipBackupAttributeToItemAtPath(COOKIE_PATH);
        }
    }

    http->setCookie(COOKIE_PATH);
    http->setTimeoutForConnect(15);
    
    return true;
}
void HttpClient::doRequset(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData,const char* s,const char* m,bool showBusy)
{
    json_serializer ser;
    doRequset(pSelectorTarget,selector,autoSaveData,s,m,ser,showBusy);
}

void HttpClient::requestFromLua(cocos2d::LUA_FUNCTION listener,
                    const char* s,const char* m,const char* a, bool inBattle, bool autoSaveData,bool showBusy)
{
    
    std::string aStr(a);
    
    CCString *url = CCString::createWithFormat("%s/%s?s=%s&m=%s&a=%s&v=%s&sys=%s",_url.c_str(),API_INTERFACE,s,m,UrlEncode(aStr).c_str(),SystemUtil::getVersion().c_str(), SYSTEM_SIGN);
#if QSMY_DEBUG
    CCLOG("%s",url->getCString());
#endif
    HttpRequest* request = new HttpRequest();
    //attachCookie(request);
    
    Json::Value action;
    Json::Reader reader;
    reader.parse(a, action);
    json_serializer ser;
    ser<<SERVICE_METHOD<<GET_SERVICE_METHOD(s, m).c_str()<<ACTION<<action;
    
    std::string requestData = ser.toString();
    request->setShowBusy(showBusy);
    request->setAutoSaveData(autoSaveData);
    request->setCallback(listener);
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setRequestData(requestData.c_str(), requestData.length());

	if (inBattle)
		request->setResponseCallback(this, httpresponse_selector(HttpClient::onBattleRequestCompleted));
	else	
	    request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
    
	doRequset(request);
    request->release();
}

void HttpClient::doRequset(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData,const char* s,const char* m,json_serializer &a,bool showBusy)
{
    CCString *url = CCString::createWithFormat("%s/%s?s=%s&m=%s&a=%s&v=%s&sys=%s",_url.c_str(),API_INTERFACE,s,m,UrlEncode(a.toString()).c_str(),SystemUtil::getVersion().c_str(), SYSTEM_SIGN);
#ifdef QSMY_DEBUG
    CCLOG("%s",url->getCString());
#endif
    HttpRequest* request = new HttpRequest();
    //attachCookie(request);
    json_serializer ser;
    ser<<SERVICE_METHOD<<GET_SERVICE_METHOD(s, m).c_str()<<ACTION<<a;
    
    std::string requestData = ser.toString();
    request->setShowBusy(showBusy);
    request->setAutoSaveData(autoSaveData);
    request->setCallback(pSelectorTarget, selector);
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setRequestData(requestData.c_str(), requestData.length());
    request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
    doRequset(request);
    request->release();
}

void HttpClient::doBattleRequset(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData,const char* s,const char* m,json_serializer &a,bool showBusy)
{
    CCString *url = CCString::createWithFormat("%s/%s?s=%s&m=%s&a=%s&v=%s&sys=%s",_url.c_str(),API_INTERFACE,s,m,UrlEncode(a.toString()).c_str(),SystemUtil::getVersion().c_str(), SYSTEM_SIGN);
#ifdef QSMY_DEBUG
    CCLOG("%s",url->getCString());
#endif
    HttpRequest* request = new HttpRequest();
    //attachCookie(request);
    json_serializer ser;
    ser<<SERVICE_METHOD<<GET_SERVICE_METHOD(s, m).c_str()<<ACTION<<a;
    
    std::string requestData = ser.toString();
    request->setShowBusy(showBusy);
    
    request->setAutoSaveData(autoSaveData);
    request->setCallback(pSelectorTarget, selector);
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setRequestData(requestData.c_str(), requestData.length());
    request->setResponseCallback(this, httpresponse_selector(HttpClient::onBattleRequestCompleted));
    doRequset(request);
    request->release();
    
}

/*void HttpClient::attachCookie(cocos2d::extension::CCHttpRequest* request)
{
    std::vector<std::string> headers;
    std::string cookie = "Cookie:"+ DM_GET_USER_MODEL->getCookie();
    headers.push_back(cookie);
    request->setHeaders(headers);
}*/

void HttpClient::onHttpRequestCompleted(cocos2d::extension::CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
    {
        return;
    }
    HttpRequest * request = (HttpRequest*)response->getHttpRequest();
    if(request->getShowBusy()) m_pBusyLayer->hideBusy();
    
    Json::Value requestData;
    Json::Reader reader;
    Json::Value responseData;
    
    std::string requestStr = std::string(request->getRequestData(),request->getRequestDataSize());
    reader.parse(requestStr, requestData);
    
    
    if (!response->isSucceed())
    {
        CCLOG("response failed");
        CCLOG("error buffer: %s_%d", response->getErrorBuffer(),response->getResponseCode());
        CCLOG("%s", requestStr.c_str());
        if(!request->getShowBusy()) return;
        
        bool notify = m_pFailedRequestList->count()==0;
        
        m_pFailedRequestList->setObject(request, requestStr);
        
        if(notify)
        {
            NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_REQUEST_FAILD);
        }
        return;
    }
       
        
        
    std::string responseStr(response->getResponseData()->begin(),response->getResponseData()->end());

    CCLOG("%s",responseStr.c_str());
    
    reader.parse(responseStr, responseData);
    
    if(requestData[ACTION].isNull() || requestData[ACTION].isObject()) requestData[ACTION][UUID] = request->getUUID();
    Json::FastWriter writer;
    std::string requestStrUUID = writer.write(requestData);
   // CCLOG(requestStrUUID.c_str());
    m_pHttpHandler->parseResult(requestData,responseData,request->getAutoSaveData());   //如果自动保存返回值
    request->doCallback(requestData[ACTION],responseData); //自定义回调检查
    
    if(m_nScriptHandler)
    {
        cocos2d::CCLuaStack* stack = cocos2d::CCLuaEngine::defaultEngine()->getLuaStack();
        stack->clean();
        //stack->pushCCLuaValueDict(dict);
        stack->pushString(requestStrUUID.c_str());
        stack->pushString(responseStr.c_str());
        stack->pushBoolean(request->getAutoSaveData());
        stack->executeFunctionByHandler(m_nScriptHandler, 3);
    }
    
    request->doCallbackLua(requestStrUUID, responseStr);
}


void HttpClient::onHttpDataRequestCompleted(cocos2d::extension::CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
    {
        return;
    }
    
    HttpRequest * request = (HttpRequest*)response->getHttpRequest();
    if(request->getShowBusy()) m_pBusyLayer->hideBusy();
    
    Json::Value requestData;
    Json::Reader reader;
    Json::Value responseData;

    std::string requestStr = std::string(request->getRequestData(),request->getRequestDataSize());
    reader.parse(requestStr, requestData);
    
    
    if (!response->isSucceed())
    {
        CCLOG("response failed");
        CCLOG("error buffer: %s_%d", response->getErrorBuffer(),response->getResponseCode());
        CCLOG("%s", requestStr.c_str());
        if(!request->getShowBusy()) return;
        
        bool notify = m_pFailedRequestList->count()==0;
        
        m_pFailedRequestList->setObject(request, requestStr);
        
        if(notify)
        {
            NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_REQUEST_FAILD);
        }
        return;
    }
    
    std::string responseStr("{\"code\":\"0000\",\"data\":{},\"ts\":0}");
    reader.parse(responseStr, responseData);
    
    responseStr = std::string(response->getResponseData()->begin(),response->getResponseData()->end());
    
    Json::Value responseData2;
    reader.parse(responseStr, responseData2);
    if(responseData2.isNull())
    {
        responseData2["data"] = base64_encode(responseStr);
    }
    
    responseData["data"] = responseData2;
        
    if(requestData[ACTION].isNull() || requestData[ACTION].isObject()) requestData[ACTION][UUID] = request->getUUID();
    Json::FastWriter writer;
    std::string requestStrUUID = writer.write(requestData);
    //responseStr = writer.write(responseData);
    
    request->doCallback(requestData[ACTION],responseData); //自定义回调检查
}


void HttpClient::loginSetTokken(const char* param,CCObject* pSelectorTarget, SEL_CallFuncJson selector)
{
    HttpRequest* request = new HttpRequest();
    
    CCString *url = CCString::createWithFormat("%s/%s?%s&v=%s",DM_GET_PLATFORM_MODEL->getPlatformURL().c_str(),API_LOING_CB,param,SystemUtil::getVersion().c_str());
    CCLOG("%s",url->getCString());
    
    json_serializer ser;
    ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"loginSetTokken";
    
    std::string requestData = ser.toString();
    
    
    request->setUrl(url->getCString());
    request->setShowBusy(true);
    request->setAutoSaveData(true);
    
    request->setRequestData(requestData.c_str(), requestData.length());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setCallback(pSelectorTarget, selector);
    request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
    doRequset(request);
    request->release();
}


void HttpClient::login(const char* param,CCObject* pSelectorTarget, SEL_CallFuncJson selector)
{
    HttpRequest* request = new HttpRequest();
    CCString *url = NULL;

    if(PlatformManager::sharedPlatformManager()->isGuess()){
        url = CCString::createWithFormat("%s/%s?%s&v=%s&sys=%s",DM_GET_PLATFORM_MODEL->getPlatformURL().c_str(),GUESS_LOGIN,param,SystemUtil::getVersion().c_str(), SYSTEM_SIGN);
    }else{
        url = CCString::createWithFormat("%s/%s?%s&v=%s&sys=%s",DM_GET_PLATFORM_MODEL->getPlatformURL().c_str(),API_LOGIN,param,SystemUtil::getVersion().c_str(), SYSTEM_SIGN);
    }
    CCLOG("%s",url->getCString());
    
    json_serializer ser;
    ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"login";
    
    std::string requestData = ser.toString();

    
    request->setUrl(url->getCString());
    request->setShowBusy(true);
    request->setAutoSaveData(true);
    
    request->setRequestData(requestData.c_str(), requestData.length());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setCallback(pSelectorTarget, selector);
    request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
    doRequset(request);
    request->release();
}


void HttpClient::resetPass(const char* param,CCObject* pSelectorTarget, SEL_CallFuncJson selector)
{
	HttpRequest* request = new HttpRequest();
	CCString *url = NULL;

	url = CCString::createWithFormat("%s/%s?%s&v=%s",DM_GET_PLATFORM_MODEL->getPlatformURL().c_str(),API_RESET_PASS,param,SystemUtil::getVersion().c_str());
	CCLOG("%s",url->getCString());

	json_serializer ser;
	ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"login";

	std::string requestData = ser.toString();


	request->setUrl(url->getCString());
	request->setShowBusy(true);
	request->setAutoSaveData(true);

	request->setRequestData(requestData.c_str(), requestData.length());
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setCallback(pSelectorTarget, selector);
	request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
	doRequset(request);
	request->release();
}

void HttpClient::registerUser(const char* param,CCObject* pSelectorTarget, SEL_CallFuncJson selector)
{
	HttpRequest* request = new HttpRequest();
	CCString *url = NULL;

	url = CCString::createWithFormat("%s/%s?%s&v=%s",DM_GET_PLATFORM_MODEL->getAuthURL().c_str(),API_REGISTER,param,SystemUtil::getVersion().c_str());
	CCLOG("%s",url->getCString());

	json_serializer ser;
	ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"login";

	std::string requestData = ser.toString();


	request->setUrl(url->getCString());
	request->setShowBusy(true);
	request->setAutoSaveData(true);

	request->setRequestData(requestData.c_str(), requestData.length());
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setCallback(pSelectorTarget, selector);
	request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
	doRequset(request);
	request->release();
}

void HttpClient::resetPassTW(CCObject* pSelectorTarget, SEL_CallFuncJson selector, json_serializer &a)
{
	HttpRequest* request = new HttpRequest();
	CCString *url = NULL;

    CCLOG("param___%s",a.toString().c_str());
	url = CCString::createWithFormat("%s/%s?s=%s&m=%s&a=%s&v=%s&sys=%s",DM_GET_PLATFORM_MODEL->getAuthURL().c_str(),API_INTERFACE,LOGIN_RESETPW,UrlEncode(a.toString()).c_str(),SystemUtil::getVersion().c_str(), SYSTEM_SIGN);
	CCLOG("%s",url->getCString());

	json_serializer ser;
	ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"login";

	std::string requestData = ser.toString();


	request->setUrl(url->getCString());
	request->setShowBusy(true);
	request->setAutoSaveData(true);

	request->setRequestData(requestData.c_str(), requestData.length());
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setCallback(pSelectorTarget, selector);
	request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
	doRequset(request);
	request->release();
}

void HttpClient::registerUserTW(CCObject* pSelectorTarget, SEL_CallFuncJson selector, json_serializer &a)
{
	HttpRequest* request = new HttpRequest();
	CCString *url = NULL;
    CCLOG("param___%s",a.toString().c_str());
	url = CCString::createWithFormat("%s/%s?s=%s&m=%s&a=%s&v=%s&sys=%s",DM_GET_PLATFORM_MODEL->getAuthURL().c_str(),API_INTERFACE,LOGIN_REGISTER,UrlEncode(a.toString()).c_str(),SystemUtil::getVersion().c_str(), SYSTEM_SIGN);
	CCLOG("%s",url->getCString());

	json_serializer ser;
	ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"login";

	std::string requestData = ser.toString();


	request->setUrl(url->getCString());
	request->setShowBusy(true);
	request->setAutoSaveData(true);

	request->setRequestData(requestData.c_str(), requestData.length());
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setCallback(pSelectorTarget, selector);
	request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
	doRequset(request);
	request->release();
}

void HttpClient::bindAccount(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector, json_serializer &a)
{
	HttpRequest* request = new HttpRequest();
	CCString *url = NULL;
	CCLOG("param___%s",a.toString().c_str());
	url = CCString::createWithFormat("%s/%s?s=%s&m=%s&a=%s&v=%s&sys=%s",DM_GET_PLATFORM_MODEL->getAuthURL().c_str(),API_INTERFACE,BIND_ACCOUNT,UrlEncode(a.toString()).c_str(),SystemUtil::getVersion().c_str(), SYSTEM_SIGN);
	CCLOG("%s",url->getCString());

	json_serializer ser;
	ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"login";

	std::string requestData = ser.toString();


	request->setUrl(url->getCString());
	request->setShowBusy(true);
	request->setAutoSaveData(true);

	request->setRequestData(requestData.c_str(), requestData.length());
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setCallback(pSelectorTarget, selector);
	request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
	doRequset(request);
	request->release();
}

void HttpClient::checkMaint()
{
    HttpRequest* request = new HttpRequest();
    CCString *url = CCString::createWithFormat("%s/%s?v=%s",_url.c_str(),API_CHECKMAINT,SystemUtil::getVersion().c_str());
    CCLOG("%s",url->getCString());
    
    json_serializer ser;
    ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"checkMaint";
    
    std::string requestData = ser.toString();
    
    request->setUrl(url->getCString());
    request->setShowBusy(false);
    request->setAutoSaveData(false);
    
    request->setRequestData(requestData.c_str(), requestData.length());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
    doRequset(request);
    request->release();
}

void HttpClient::getMyServerList(const char* param,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector)
{
    HttpRequest* request = new HttpRequest();
    
    CCString *url = CCString::createWithFormat("%s/%s?s=%s&m=%s&a=%s&sys=%s",DM_GET_PLATFORM_MODEL->getAuthURL().c_str(),API_INTERFACE,USER_MYSERVETLIT,param, SYSTEM_SIGN);
    
#ifdef QSMY_DEBUG
    CCLOG("%s",url->getCString());
#endif
    
    json_serializer ser;
    ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"getMyServerList";
    std::string requestData = ser.toString();
    
    request->setUrl(url->getCString());
    request->setShowBusy(true);
    request->setAutoSaveData(true);
    
    request->setRequestData(requestData.c_str(), requestData.length());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setCallback(pSelectorTarget, selector);
    request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
    doRequset(request);
    request->release();
}


void HttpClient::getServerList(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector)
{
    HttpRequest* request = new HttpRequest();
    
//#if QSMY_DEBUG
//    CCString *url = CCString::createWithFormat("%s/%s?p=%s&s=%s&v=%s",DM_GET_PLATFORM_MODEL->getConfig().getCheckVersionURL().c_str(),API_SERVER_LIST, "mock",sys,SystemUtil::getVersion().c_str());
//#else
    CCString *url = CCString::createWithFormat("%s/%s?p=%s&s=%s&v=%s",DM_GET_PLATFORM_MODEL->getAuthURL().c_str(),API_SERVER_LIST,DM_GET_PLATFORM_MODEL->getConfig().getPlatform().c_str(),SYSTEM_SIGN,SystemUtil::getVersion().c_str());
//#endif
    
    CCLOG("%s",url->getCString());
    
    json_serializer ser;
    ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"getServerList";
    std::string requestData = ser.toString();
    
    request->setUrl(url->getCString());
    request->setShowBusy(true);
    request->setAutoSaveData(true);
    
    request->setRequestData(requestData.c_str(), requestData.length());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setCallback(pSelectorTarget, selector);
    request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpDataRequestCompleted));
    doRequset(request);
    request->release();
}

void HttpClient::getAnnounce( cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector )
{
	CCString* url = CCString::createWithFormat("%s", DM_GET_PLATFORM_MODEL->getAnnounceURL().c_str());
    CCLOG("Announce url:%s",url->getCString());

	 json_serializer ser;
	 ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"getAnnounce";
	 std::string requestData = ser.toString();

	 HttpRequest* request = new HttpRequest();
	 request->setUrl(url->getCString());
	 request->setShowBusy(true);
	 request->setAutoSaveData(true);

	 request->setRequestData(requestData.c_str(), requestData.length());
	 request->setRequestType(CCHttpRequest::kHttpGet);
	 request->setCallback(pSelectorTarget, selector);
	 request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpRequestCompleted));
	 doRequset(request);
	 request->release();
}

void HttpClient::getVersion(CCObject* pSelectorTarget, SEL_CallFuncJson selector)
{
    HttpRequest* request = new HttpRequest();
    
//#if QSMY_DEBUG
//    CCString *url = CCString::createWithFormat("%s/%s?p=%s&s=%s&v=%s",DM_GET_PLATFORM_MODEL->getConfig().getCheckVersionURL().c_str(),API_CHECK_VERSION, "mock",sys,SystemUtil::getVersion().c_str());
//#else
    CCString *url = CCString::createWithFormat("%s/%s?p=%s&s=%s&v=%s",DM_GET_PLATFORM_MODEL->getConfig().getCheckVersionURL().c_str(),API_CHECK_VERSION,DM_GET_PLATFORM_MODEL->getConfig().getPlatform().c_str(),SYSTEM_SIGN,SystemUtil::getVersion().c_str());
//#endif
    
    CCLOG("%s",url->getCString());
    
    json_serializer ser;
    ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"getVersion";
    std::string requestData = ser.toString();
    
    request->setUrl(url->getCString());
    request->setShowBusy(true);
    request->setAutoSaveData(true);
    
    request->setRequestData(requestData.c_str(), requestData.length());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setCallback(pSelectorTarget, selector);
    request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpDataRequestCompleted));
    doRequset(request);
    request->release();
}

void HttpClient::getFileVersion(CCObject* pSelectorTarget, SEL_CallFuncJson selector,const char* version_url)
{
    HttpRequest* request = new HttpRequest();
    
    CCString *url = CCStringMake(version_url);
    CCLOG("%s",url->getCString());
    
    json_serializer ser;
    ser<<SERVICE_METHOD<<GET_SERVICE_METHOD("", "").c_str()<<ACTION<<"getFileVersion";
    
    std::string requestData = ser.toString();
    
    
    request->setUrl(url->getCString());
    request->setShowBusy(true);
    request->setAutoSaveData(true);
    
    request->setRequestData(requestData.c_str(), requestData.length());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setCallback(pSelectorTarget, selector);
    request->setResponseCallback(this, httpresponse_selector(HttpClient::onHttpDataRequestCompleted));
    doRequset(request);
    request->release();
}


void HttpClient::onBattleRequestCompleted(cocos2d::extension::CCHttpClient *sender, CCHttpResponse *response)
{
    
    if (!response)
    {
        return;
    }
    

    std::string responseStr(response->getResponseData()->begin(), response->getResponseData()->end());
    if (responseStr.find("error") != std::string::npos) {
#ifdef QSMY_DEBUG
        CCMessageBox(responseStr.c_str(), "战斗数据错误，请联系俞洋。");
#else
        CCMessageBox(responseStr.c_str(), "數據錯誤，請截屏聯繫客服。");
#endif
        onHttpRequestCompleted(sender,response);
        return;
    }

    
    if (!response->isSucceed())
    {
        onHttpRequestCompleted(sender,response);
        return;
    }
    
    
    std::vector<char> vec = *response->getResponseData();
    
    std::vector<char>::iterator iter = vec.begin();
	if (iter == vec.end())
	{
		CCLOG("HttpClient::onBattleRequestCompleted vec empty");
	}

    uint16_t len;
    len = static_cast<uint16_t>(*iter++) &0x00ff;
    len |= (static_cast<uint16_t>(*iter++) << 8) & 0xff00;
    std::vector<char> battle(iter,iter+len);
    std::vector<char> json(iter+len,vec.end());
    
    response->setResponseData(&json);
    
    m_pHttpHandler->onBattleData(battle);
    onHttpRequestCompleted(sender,response);
}


#pragma mark----------------成就系統----------------------
void HttpClient::getTaskAward(uint32_t task_id, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData , bool showBusy)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,TASK_GETAWARD,s<<"task_id"<<task_id);
}

void HttpClient::getTaskScoreAward(uint32_t index, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData , bool showBusy)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,TASK_GETSCOREAWARD,s<<"id"<<index);
}

void HttpClient::getTaskList(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData , bool showBusy)
{
	doRequset(pSelectorTarget,selector,autoSaveData,TASK_GETLIST,showBusy);
}

#pragma mark -------------------用户信息---------------------
/******************  user info ***********************/
void HttpClient::getUserInfo(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData,bool showBusy)
{
	doRequset(pSelectorTarget,selector,autoSaveData,USER_GET,showBusy);
}


void HttpClient::registerToPlat(cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData,bool showBusy)
{
    doRequset(pSelectorTarget,selector,autoSaveData,USER_REGISTERTOPLAT,showBusy);
}


void HttpClient::randUserName(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData,bool showBusy)
{
	doRequset(pSelectorTarget,selector,autoSaveData,USER_RAND_NAME,showBusy);
}

void HttpClient::userPrivileges(cocos2d::CCObject* pSelectorTarget /* = NULL */,SEL_CallFuncJson selector /* = NULL */, bool autoSaveData /* = true */,bool showBusy /* = true */)
{
	doRequset(pSelectorTarget,selector,autoSaveData,USER_PRIVILEGES,showBusy);
}


#pragma mark -------------------阵容---------------------
/******************  formation ***********************/
void HttpClient::getFormations(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    doRequset(pSelectorTarget,selector,autoSaveData,FORMATION_GET);
}


void HttpClient::joinFormation(uint32_t wid,uint32_t id,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
    json_serializer s;
    doRequset(pSelectorTarget,selector,autoSaveData,FORMATION_JOIN,s<<"wid"<<wid<<"id"<<id);
}

void HttpClient::changeFormation(uint32_t id1,uint32_t id2,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
    
    json_serializer s;
    doRequset(pSelectorTarget,selector,autoSaveData,FORMATION_CHANGE,s<<"id1"<<id1<<"id2"<<id2);
}

void HttpClient::getOtherFormation(uint32_t othid,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,FORMATION_GETOTHER,s<<"uid"<<othid);
}

void HttpClient::equipWarrior(uint32_t fid,uint32_t eid,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,FORMATION_EQUIP,s<<"eid"<<eid<<"fid"<<fid);
}

void HttpClient::quickEquipWarrior(uint32_t fid,Json::Value& eids,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,FORMATION_QUICKEQUIP,s<<"eids"<<eids<<"fid"<<fid);
}


void HttpClient::upgradeSkill(uint32_t id,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,SKILL_LEVELUP,s<<"wid"<<id);
}
#pragma mark -------------------弟子---------------------
/******************  warrior ***********************/
void HttpClient::getWarriors(cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
    doRequset(pSelectorTarget,selector,autoSaveData,WARRIOR_GET);
}

void HttpClient::getWarrior(uint32_t id,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
    json_serializer s;
    doRequset(pSelectorTarget,selector,autoSaveData,WARRIOR_GETONE,s<<"wid"<<id);
}


void HttpClient::promoteWarrior(uint32_t id,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,WARRIOR_UP,s<<"wid"<<id);
}

void HttpClient::inheritWarrior(uint32_t fid,uint32_t tid, uint32_t type, cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,WARRIOR_INH,s<<"fid"<<fid<<"tid"<<tid<<"type"<<type);
}


void HttpClient::upgradeWarrior( uint32_t warriorId, uint32_t itemId, uint32_t itemCount, cocos2d::CCObject* pSelectorTarget /*= NULL*/,SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/ )
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, WARRIOR_UPGRADE, s << "wid" << warriorId << "iid" << itemId << "num" << itemCount);
}

void HttpClient::developWarrior(uint32_t id, uint32_t type, cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,WARRIOR_DEV,s<<"wid"<<id<<"type"<<type);
}

void HttpClient::confDevelopWarrior(uint32_t id, uint32_t act, cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,WARRIOR_DEV_NOT,s<<"wid"<<id<<"act"<<act);
}

void HttpClient::setDress(uint32_t wid,uint32_t dressId,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData,bool showBusy)
{
    json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,WARRIOR_SET_DRESS,s<<"wid"<<wid<<"dressId"<<dressId,showBusy);
}

void HttpClient::unDress(uint32_t wid,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData,bool showBusy)
{
    setDress(wid,0,pSelectorTarget,selector,autoSaveData,showBusy);
}

#pragma mark----------------魂魄----------------------

void HttpClient::recruitSoul(uint32_t id,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,SOUL_RECRUIT,s<<"wid"<<id);
}

void HttpClient::getSouls(cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,SOUL_GET);
}

void HttpClient::getSoul(uint32_t id,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector, bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,SOUL_GETONE,s<<"wid"<<id);
}



#pragma mark -------------------装备---------------------
/******************  equip ***********************/
void HttpClient::getEquips(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    doRequset(pSelectorTarget,selector,autoSaveData,EQUIP_GETEQUIPS);
}

void HttpClient::getEquip(uint32_t id,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    doRequset(pSelectorTarget,selector,autoSaveData,EQUIP_GETEQUIP,s<<"id"<<id);
}

/**
*	@brief	装备强化
*	@param 	id 	装备ID
*/
void HttpClient::enhanceEquip(uint32_t id,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,EQUIP_ENHANCE,s<<"id"<<id);
}

/**
*	@brief	装备自动强化
*	@param 	id 	装备ID
*/
void HttpClient::autoEnhanceEquip(uint32_t id,cocos2d::CCObject* pSelectorTarget , SEL_CallFuncJson selector,bool autoSaveData )
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,EQUIP_AUTOENHANCE,s<<"id"<<id);
}
/**
*	@brief	装备出售
*	@param 	ids 	装备ID数组  -- 暂时改为单个id
*/
//void HttpClient::sellEquips(uint32_t ids, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData )
//{
//	json_serializer s;
//	doRequset(pSelectorTarget,selector,autoSaveData,EQUIP_SELLEQUIP,s<<"id"<<ids);
//}

void HttpClient::sellEquips(Json::Value& items, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData )
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,EQUIP_SELLEQUIP,s<<"ids"<<items);
}

void HttpClient::refineEquip(uint32_t equipId, uint32_t itemId,uint32_t num, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,EQUIP_REFINE,s<<"equipId"<<equipId<<"itemId"<<itemId<<"num"<<num);
}

void HttpClient::decomposeEquip(Json::Value& items, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,EQUIP_DECOMPOSE,s<<"ids"<<items);
}

void HttpClient::treasureBurn(unsigned int id , Json::Value& items, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData )
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,TREASURE_BURN,s<<"id"<<id<<"items"<<items);
}

#pragma mark -------------------背包---------------------
/******************  inventory ***********************/
void HttpClient::getItems(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    doRequset(pSelectorTarget,selector,autoSaveData,ITEM_GETITEMS);
}

void HttpClient::getItem(uint32_t id,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,ITEM_GETITEM,s<<"id"<<id);
}

void HttpClient::useItem(uint32_t id,const char* newName, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData,uint32_t num)
{
	json_serializer s;
	if (newName)
	{
		s << "id" << id << "newName" << newName << "num" << num;
	}
	else
	{
		s << "id" << id << "newName" << "" << "num" << num;
	}
	//如果开箱子掉落东西，不合并
	s << "noSort" << true;
	
	doRequset(pSelectorTarget,selector,autoSaveData,ITEM_USEITEM,s);	//FIXME  newName
}

void HttpClient::useItem(uint32_t id,uint32_t num,const char* newName)
{
	useItem(id,newName,NULL,NULL,true,num);
}

#pragma mark -------------------商城---------------------
/******************  mall ***********************/


void HttpClient::getProducts(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,PRODUCT_GETS);
}
	

void HttpClient::buyProduct(uint32_t id,uint32_t num, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,PRODUCT_BUY,s<<"id"<<id<<"num"<<num);
}

void HttpClient::buyPow(uint32_t num, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,BUY_POWER,s<<"num"<<num);
}

void HttpClient::buy200RedPacketProduct(unsigned int redPacketId, unsigned int productId, cocos2d::CCObject* pSelectorTarget/* = NULL*/, SEL_CallFuncJson selector/* = NULL*/, bool autoSaveData/* = true*/)
{
    json_serializer s;
    doRequset(pSelectorTarget,selector,autoSaveData,BUY_200_RED_PACKET,s<<"id"<<redPacketId<<"mallId"<<productId);
}
    

#pragma mark -------------------pve---------------------
/******************  pve ***********************/
void HttpClient::getMaps(uint32_t cid,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    doRequset(pSelectorTarget,selector,autoSaveData,PVE_GET,s<<"cid"<<cid);
}

void HttpClient::enterMap(uint32_t mid,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    doBattleRequset(pSelectorTarget,selector,autoSaveData,PVE_EXEC,s<<"mid"<<mid);
}

void HttpClient::getGradeAward(uint32_t cid,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    doRequset(pSelectorTarget,selector,autoSaveData,PVE_GRADEAWARD,s<<"cid"<<cid);
}

void HttpClient::getChapters(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    doRequset(pSelectorTarget,selector,autoSaveData,CHAPTER_GET,true);
}

void HttpClient::getElites(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,PVE_ELITE_GET);
}

void HttpClient::collectChapterBox(uint32_t cid,uint8_t vip)
{
    json_serializer s;
    
    doRequset(NULL,NULL,true,CHAPTER_COLLECT_BOX,s<<"cid"<<cid<<"vip"<<vip);
}

void HttpClient::pve_rush(uint32_t mid, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,PVE_RUSH,s<<"mid"<<mid);
}

void HttpClient::pve_rush_clearcd(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,PVE_RUSH_CLEARCD);
}

void HttpClient::pve_onceMore(uint32_t mid, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,PVE_ONCEMORE,s<<"mid"<<mid);
}

void HttpClient::getPveActivityAward(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,PVE_ACTIVITYAWARD);
}

#pragma mark -------------------杯赛---------------------
/******************  CUP杯赛 ***********************/

/*杯赛初始化*/
void HttpClient::initCup(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,CUP_INIT);
}
    
/*获得杯赛个人信息*/
void HttpClient::getCupInfo(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,CUP_GET);
}

/*获得杯赛个人历史*/
void HttpClient::getCupProcess(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,CUP_PROCESS);
}

/*获得杯赛对阵列表*/
void HttpClient::getCupMatchTable(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,CUP_MTABLE);
}

/*获得杯赛竞猜记录*/
void HttpClient::getCupBets(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,CUP_BETS);
}

/**
*	@brief	获得比赛战报
*	@param 	finalNum	几强赛(8/4/2)
*   @param  mid	比赛ID
*/
void HttpClient::getCupMatchReplay(uint32_t finalNum, uint32_t mid, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doBattleRequset(pSelectorTarget,selector,true,CUP_MREP,s<<"finalNum"<<finalNum<<"mid"<<mid,autoSaveData);
}

/*报名杯赛*/
void HttpClient::signUpCup(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,CUP_SIGNUP);
}

/**
*	@brief	获取比赛奖励
*	@param 	finalNum	几强赛(8/4/2)
*   @param  mid	比赛ID
*/
void HttpClient::getCupMatchPrize(uint32_t finalNum, uint32_t mid, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,CUP_MREWARD,s<<"finalNum"<<finalNum<<"mid"<<mid);
}

/**
*	@brief	杯赛竞猜
*	@param 	finalNum	几强赛(8/4/2)
*   @param  mid	比赛ID
*   @param  betUser	竞猜胜利方 1:一号玩家 2:二号玩家
*/
void HttpClient::doCupBet(uint32_t finalNum, uint32_t mid, uint32_t betUser, cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,CUP_BET,s<<"finalNum"<<finalNum<<"mid"<<mid<<"betUser"<<betUser);
}

/**
*	@brief	获得竞猜奖励
*	@param 	id	竞猜ID
*   @param  mid	比赛ID
*/
void HttpClient::getCupBetPrize(uint32_t id, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,CUP_BETPRIZE,s<<"id"<<id);
}


#pragma mark -------------------论剑---------------------
/******************  论剑 ***********************/

/*初始化*/
void HttpClient::initPvpInfo(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,PVP_INIT);
}
    
/*获得论剑信息*/
void HttpClient::getPvpInfo(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,PVP_GETINFO);
}

/**
*	@brief	获得奖励
*   @param  rank    自己排名
*	@param 	reward	奖励编号
*/
void HttpClient::getPvpAward(uint32_t rank, uint32_t reward, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,PVP_GETREWARD,s<<"rank"<<rank<<"reward"<<reward);
}

/*获得对手列表*/
void HttpClient::getPvpList(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,PVP_GETLIST);
}

/**
*	@brief	挑战对手
*  @param  tid     对手id
*	@param 	tRank	对手排名
*  @param  sRank	自己排名
*/
void HttpClient::chPvpTarget(uint32_t tid, uint32_t tRank,uint32_t sRank, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	if(tRank < sRank)
	{
		doBattleRequset(pSelectorTarget,selector,autoSaveData,PVP_CHPVP,s<<"tid"<<tid<<"tRank"<<tRank<<"sRank"<<sRank,true);
	} else 
	{
		doBattleRequset(pSelectorTarget,selector,autoSaveData,PVP_CHPVPLOW,s<<"tid"<<tid<<"tRank"<<tRank<<"sRank"<<sRank,true);
	}
	
}
/**
*	@brief	挑战试炼木人
*  @param  rank     自己排名
*/
void HttpClient::chPvpNPC(uint32_t rank, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doBattleRequset(pSelectorTarget,selector,autoSaveData,PVP_CHNPC,s<<"rank"<<rank,true);
}
/**
*	@brief	挑战东皇太一
*  @param  rank     自己排名
*/
void HttpClient::chPvpBoss(uint32_t rank, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doBattleRequset(pSelectorTarget,selector,autoSaveData,PVP_CHBOSS,s<<"rank"<<rank,true);
}

void HttpClient::challengeRefresh(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,PVP_REFRESH,s);
}

void HttpClient::challengeExchange(uint32_t idx, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	//doRequset(pSelectorTarget,selector,autoSaveData,PVP_EXCHANGE,s << "index" << idx);
	doRequset(pSelectorTarget,selector,autoSaveData,PVP_EXCHANGE,s << "id" << idx);
}

#pragma mark -------------------千层塔---------------------
/******************  千层塔 ***********************/

void HttpClient::towerGet(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, TOWER_GET);
}

void HttpClient::towerFight(uint32_t lv,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector)
{
	json_serializer s;
	doBattleRequset(pSelectorTarget,selector,false,TOWER_FIGHT,s<<"lv"<<lv);
}

void HttpClient::towerStart(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, TOWER_START);
}

void HttpClient::towerGoon(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, TOWER_GOON);
}

void HttpClient::towerSetBuff(uint32_t id, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, TOWER_SETBUFF, s<<"id"<<id);
}

void HttpClient::towerEnd(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, TOWER_END);
}

void HttpClient::towerRank(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, TOWER_RANK);
}

void HttpClient::towerOldRank(cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	doRequset(pSelectorTarget, selector, autoSaveData, TOWER_OLD_RANK);
}

void HttpClient::towerReward(unsigned int id, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,TOWER_REWARD,s<<"id"<<id);
}

#pragma mark -------------------天命---------------------
/******************  天命 ***********************/
/**
 *	@brief	获取天命列表
 */
void HttpClient::getDestinys(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    doRequset(pSelectorTarget,selector,autoSaveData,DESTINY_GET);
}

/**
 *	@brief	提升天命
 *
 *	@param 	did 	天命ID
 *	@param 	wid     消耗的弟子编号
 *	@param 	soul    消耗的魂魄编号
 */

void HttpClient::improveDestinys(uint32_t did, Json::Value &warriors , Json::Value &souls , cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,false,DESTINY_IMPROVE,s<<"id"<<did<<"warriors"<<warriors<<"souls"<<souls);
}


void HttpClient::destinyReward(uint32_t did, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,DESTINY_GETREWARD,s<<"id"<<did);
}


#pragma mark -------------------活跃度---------------------
/******************  活跃度 ***********************/

/**
 *  @breif  获取活跃度
 */
void HttpClient::getActiveness(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    doRequset(pSelectorTarget,selector,autoSaveData,ACTIVENESS_GET);
}

/**
 *	@brief	获取活跃度奖励
 *
 *	@param 	point 	活跃度点数
 */
void HttpClient::getActivenessAward(uint32_t point,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,ACTIVENESS_GETAWARD,s<<"point"<<point);
}

void HttpClient::getLoginRewardInfo(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,GETLOGINAWARD_INFO);
}

void HttpClient::getLoginReward(uint32_t id, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,LOGIN_REWARD, s<<"id"<<id);
}



#pragma mark -------------------邮件---------------------
/******************  邮件 ***********************/
/**
*  @breif  获取邮件列表
*/
void HttpClient::getMails(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	doRequset(pSelectorTarget,selector,autoSaveData,MAIL_GETMAILS);
}
	
/**
*	@brief	获取邮件附件
*
*	@param 	id 	邮件id
*/
void HttpClient::pickUpMail(uint32_t id,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,MAIL_PICKUP, s<<"id"<<id);
}

/**
*  @breif  获取新邮件数
*/
void HttpClient::getNewMails(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData,bool showBusy)
{
	doRequset(pSelectorTarget,selector,autoSaveData,MAIL_GETNEWS,showBusy);
}

void HttpClient::getAllMailsItem( cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/,bool showBusy /*= true*/ )
{
	doRequset(pSelectorTarget, selector, autoSaveData, MAIL_RECIVE_ALL_ITEM, showBusy);
}


void HttpClient::deleteMail(cocos2d::CCArray *pMails,cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	Json::Value maills(Json::arrayValue);

	CCObject *obj(NULL);
	CCARRAY_FOREACH(pMails , obj)
	{
		CCInteger *id = static_cast<CCInteger*>(obj);
		maills.append(id->getValue());
	}

	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,MAIL_DELETE_MAIL, s<<"mailId"<<maills);
}

#pragma mark -------------------抽卡---------------------
/******************  抽卡 ***********************/

void HttpClient::getCardInfo(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData/* = true */)
{
    doRequset(pSelectorTarget, selector, autoSaveData, CARD_GET);
}

void HttpClient::drawCard(card::CardType type,bool cost,cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData/* = true */)
{
    json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,CARD_DRAW,s<<"type"<<type<<"cost"<<cost);
}

#pragma mark -------------------许愿---------------------
/******************  许愿  **********************/

void HttpClient::getWishesInfo(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData/* = true */)
{
    doRequset(pSelectorTarget, selector, autoSaveData, WISH_GET);
}

void HttpClient::wish(cocos2d::CCArray *pWishes, bool isTen, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    Json::Value wishes(Json::arrayValue);
    CCObject * obj(NULL);
    CCARRAY_FOREACH(pWishes, obj)
    {
        CCInteger * warriorId = dynamic_cast<CCInteger*>(obj);
        if(warriorId != NULL)
        {
            wishes.append(warriorId->getValue());
        }
    }
    json_serializer s;
    
    doRequset(pSelectorTarget, selector, autoSaveData, WISH_WISH, s << "wishes" << wishes << "isTen" << (isTen ? 1 : 0));
}

void HttpClient::wishChangeWarrior(cocos2d::CCArray *pWishes, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */, bool autoSaveData /* = true */)
{
	Json::Value wishes(Json::arrayValue);
	CCObject * obj(NULL);
	CCARRAY_FOREACH(pWishes, obj)
	{
		CCInteger * warriorId = dynamic_cast<CCInteger*>(obj);
		if(warriorId != NULL)
		{
			wishes.append(warriorId->getValue());
		}
	}
	json_serializer s;

	doRequset(pSelectorTarget, selector, autoSaveData, WISH_CHANGEWARRIOR, s << "wishes" << wishes, false);//静默同步
}

#pragma mark----------------炼化----------------------

void HttpClient::burn(cocos2d::CCArray *pList,cocos2d::CCArray* soulList, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData , bool showBusy)
{
	Json::Value list(Json::arrayValue);
	CCObject * obj(NULL);
	CCInteger * warriorId = NULL;
	CCARRAY_FOREACH(pList, obj)
	{
		warriorId = dynamic_cast<CCInteger*>(obj);
		if(warriorId != NULL)
		{
			Json::Value val(Json::objectValue);
			val[CCString::createWithFormat("%d",warriorId->getValue())->getCString()] = 1;
			list.append(val);
		}
	}
	CCARRAY_FOREACH(soulList, obj)
	{
		warriorId = dynamic_cast<CCInteger*>(obj);
		if(warriorId != NULL)
		{
			Json::Value val(Json::objectValue);
			val[CCString::createWithFormat("%d",warriorId->getValue())->getCString()] = 2;
			list.append(val);
		}
	}
	json_serializer s;

	doRequset(pSelectorTarget, selector, autoSaveData, BURN_BURN, s << "wids" << list );
}

void HttpClient::getAutoList(uint32_t num , cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData , bool showBusy)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,BURN_AUTOLIST,s<<"num"<<num);
}

#pragma mark----------------重生----------------------
void HttpClient::getRebirthDrop(uint32_t wid,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector , bool autoSaveData ,bool showBusy)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,REBIRTH_DROP,s<<"wid"<<wid);
}

void HttpClient::reBirth(uint32_t wid,cocos2d::CCObject* pSelectorTarget,SEL_CallFuncJson selector , bool autoSaveData ,bool showBusy)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,REBIRTH_REBORN,s<<"wid"<<wid);
}

#pragma mark----------------新手引导----------------------
/******************  新手引导 ***********************/
/**
 *	@brief	获取新手数据
 *
 *	@param 	NULL
 *	@param 	NULL
 *	@param 	true
 */
void HttpClient::getRookieInfo(cocos2d::CCObject* pSelectorTarget/* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData/* = true */)
{
    doRequset(pSelectorTarget, selector, autoSaveData, ROOKIE_GET);
}


/**
 *	@brief	完成新手
 *
 *	@param 	id 	新手编号
 *	@param 	wid 弟子编号//	选弟子步骤选择的弟子，仅该步需要此字段
 */
void HttpClient::finishRookie(uint32_t id, uint32_t wid, cocos2d::CCObject* pSelectorTarget/* = NULL */, SEL_CallFuncJson selector/* = NULL */,bool autoSaveData/* = true */)
{
    json_serializer s;
    if(wid>0)
    {
        s << "id" << id << "wid" << wid;
    }else
    {
        s << "id" << id;
    }
    doRequset(pSelectorTarget, selector, autoSaveData, ROOKIE_FINISH, s);
}

void HttpClient::finishRookie(uint32_t id, const char* name, cocos2d::CCObject* pSelectorTarget/* = NULL */, SEL_CallFuncJson selector/* = NULL */,bool autoSaveData/* = true */)
{
    json_serializer s;
    s << "id" << id << "name" << name;
    doRequset(pSelectorTarget, selector, autoSaveData, ROOKIE_FINISH, s);
}


#pragma mark----------------奖励页面----------------------

void HttpClient::activityGetAward(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    doRequset(pSelectorTarget, selector, autoSaveData, ACTIVITY_GET_AWARD, s);
}

/**
 *	@brief	领取累计登录奖励
 *
 *	@param 	id 	奖励编号
 */
void HttpClient::activityTLoginAward(uint32_t days, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    s << "d" << days;
    doRequset(pSelectorTarget, selector, autoSaveData, ACTIVITY_TLOGIN_AWARD, s);
}

/**
 *	@brief	领取等级奖励
 *
 *	@param 	id 	奖励编号
 */
void HttpClient::activityLvUpAward(uint32_t lv, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    s << "lv" << lv;
    doRequset(pSelectorTarget, selector, autoSaveData, ACTIVITY_LVUP_AWARD, s);
}

#pragma mark -------------------累计充值--------------------

/**
 *	@brief	领取累计登录奖励
 *
 *	@param 	id 	奖励编号
 */
void HttpClient::activityChargeAward(uint32_t awardId, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    s << "id" << awardId;
    doRequset(pSelectorTarget, selector, autoSaveData, ACTIVITY_CHARGE_AWARD, s);
}

#pragma mark - 活动信息 -

void HttpClient::activityGet(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, ACTIVITY_GET, s);
}

void HttpClient::activityShare(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
    json_serializer s;
    unsigned int ts = GameManager::sharedGameManager()->getServerTime();
    std::string sign = getShareSign();
    s <<"ts" << ts << "sign" << sign.c_str();
	doRequset(pSelectorTarget, selector, autoSaveData, ACTIVITY_SHARE, s);
}

#pragma mark -------------------系统---------------------

void HttpClient::syncServerTime(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    doRequset(pSelectorTarget,selector,autoSaveData,SYSTEM_SYNCTIME);
}

void HttpClient::initData(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    doRequset(pSelectorTarget, selector, autoSaveData, SYSTEM_INIT, s);
}

void HttpClient::getSpecialActivity(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    doRequset(pSelectorTarget, selector, autoSaveData, ACTIVITY_GET_SEPCIAL, s);
}

void HttpClient::getNotices(uint32_t list, uint32_t time, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    s << "list" << list << "time" << time;
    doRequset(pSelectorTarget, selector, autoSaveData, SYSTEM_NOTICE, s, false);
}


#pragma mark-
#pragma mark 充值
#pragma mark-

void HttpClient::getChargeOrder(const char* m,uint32_t pid, const char* amount, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    s << "pid" << pid << "amount" << amount;
    doRequset(pSelectorTarget, selector, autoSaveData, CHARGE_GET_ORDER,m, s);
}


void HttpClient::getChargeResult(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    doRequset(pSelectorTarget, selector, autoSaveData, CHARGE_GET_RESULT, s);
}


#pragma mark-
#pragma mark 设置
#pragma mark-

void HttpClient::addFeedBack(const char* content, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    s << "content" << content;
    doRequset(pSelectorTarget, selector, autoSaveData, ADD_FEEDBACK, s);
}


void HttpClient::exchangeCode(const char* code,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData)
{
    json_serializer s;
    s << "code" << code;
    doRequset(pSelectorTarget, selector, autoSaveData, EX_CODE, s);
}




#pragma mark-
#pragma mark 公会
#pragma mark-

void HttpClient::guildCreate(const char *name, cocos2d::CCObject *pSelectorTarget/* = NULL*/, SEL_CallFuncJson selector/* = NULL*/,bool autoSaveData/* = true*/)
{
	json_serializer s;
	s << "name" << name;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_CREATE, s);
}

void HttpClient::guildApply(uint32_t guildID, uint32_t action, cocos2d::CCObject *pSelectorTarget/* = NULL*/, SEL_CallFuncJson selector/* = NULL*/,bool autoSaveData/* = true*/)
{
	json_serializer s;
	s << "gid" << guildID << "act" << action;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_APPLY, s);
}

void HttpClient::guildDealApply(uint32_t appID, uint32_t action, cocos2d::CCObject *pSelectorTarget/* = NULL*/, SEL_CallFuncJson selector/* = NULL*/,bool autoSaveData/* = true*/)
{
	json_serializer s;
	s << "id" << appID << "act" << action;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_DEAL_APPLY, s);
}

void HttpClient::guildGet(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_GET, s);
}

void HttpClient::guildGetList(uint32_t guildNum, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "id" << guildNum;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_GET_LIST, s);
}

void HttpClient::guildSearch(const char *name, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "name" << name;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_SEARCH, s);
}

void HttpClient::guildPosition(uint32_t userID, uint32_t positionID, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "user" << userID << "pid" << positionID;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_POSITION, s);
}

void HttpClient::guildQuit(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_QUIT, s);
}

void HttpClient::guildSign(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_SIGN, s);
}

void HttpClient::guildQues(uint32_t answerID, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "ans" << answerID;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_QUES, s);
}

void HttpClient::guildDetails(uint32_t guildID, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "gid" << guildID;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_DETAILS, s);
}

void HttpClient::guildUpdate(const char *content, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "content" << content;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_UPDATE, s);
}

void HttpClient::guildApplyList(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_APPLYLIST, s);
}


void HttpClient::guildUpgradeTree(uint32_t type, uint32_t num, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "type" << type << "num" << num;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_UPGRADE_TREE, s);
}

void HttpClient::guildGetNotice(uint32_t noticeType, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "type" << noticeType;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_GET_NOTICE, s);
}


void HttpClient::guildMsg(uint32_t userID, const char *msg, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "user" << userID << "msg" << msg;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_MSG, s);
}

// 【帮会商店】刷新商店
void HttpClient::guildMallRefresh( cocos2d::CCObject *pSelectorTarget, 
	SEL_CallFuncJson selector, bool autoSaveData)
{
	json_serializer s;
	doRequset( pSelectorTarget, selector, autoSaveData, GUILD_MALL_REFRESH, s ) ;
}

// 【帮会商店】兑换物品
void HttpClient::guildMallExchange( uint32_t exchangeID, 
	cocos2d::CCObject *pSelectorTarget, 
	SEL_CallFuncJson selector, bool autoSaveData ) 
{
	json_serializer s;
	s << "exchangeId" << exchangeID ;
	doRequset( pSelectorTarget, selector, autoSaveData, GUILD_MALL_EXCHANGE, s ) ;
}

void HttpClient::guildLog(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */){
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_LOG, s);
}

void HttpClient::guildSubstitute(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILD_SUBSTITUTE, s);
}

#pragma mark-工会战-

void HttpClient::guildBossGet(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_GET, s);
}

void HttpClient::guildBossDeclareWar(uint32_t guildID, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "guildId" << guildID;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_DECLARE_WAR, s);
}

void HttpClient::guildBossDeclareTargets(uint32_t selfGuildID, uint32_t guildID, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "gid" << selfGuildID;
	s << "id" << guildID;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_DECLARE_TARGETS, s);
}

void HttpClient::guildBossStart(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_START, s);
}

void HttpClient::guildBossGetWoundedList(uint32_t selfGuildID, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "gid" << selfGuildID;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_GET_WOUNDED_LIST, s);
}

void HttpClient::guildBossMemberRank(uint32_t selfGuildID, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "gid" << selfGuildID;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_MEMBER_RANK, s);
}

void HttpClient::guildBossGuildRank(uint32_t selfGuildID, uint32_t guildID, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "id" << guildID;
	s << "gid" << selfGuildID;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_GUILD_RANK, s);
}

void HttpClient::guildBossGetTargetList(uint32_t selfGuildID, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "gid" << selfGuildID;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_GET_TARGET_LIST, s);
}

void HttpClient::guildBossGetHatredList(uint32_t selfGuildID, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "gid" << selfGuildID;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_GET_HATRED_LIST, s);
}

void HttpClient::guildBossAttackBoss(bool isPayed, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "isPay" << isPayed;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_ATTACK_BOSS, s);
}

void HttpClient::guildBossAutoAttackBoss(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_AUTO_ATTACK_BOSS, s);
}

void HttpClient::guildBossRob(bool isPayed, uint32_t tid, uint32_t dmg, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "isPay" << isPayed;
	s << "tid" << tid;
	s << "dmg" << dmg;
	doBattleRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_ROB, s);
}

void HttpClient::guildBossRobSettle(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_ROB_SETTLE, s);
}

void HttpClient::guildBossRescue(uint32_t userID, bool isAll, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "wounded" << userID;
	s << "all" << isAll;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_RESCUE, s);
}

void HttpClient::guildBossGetNotice(uint32_t selfGuildID, uint32_t time, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "gid" << selfGuildID;
	s << "time" << time;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_GET_NOTICE, s, false);
}

void HttpClient::guildBossDailySettle(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, GUILDBOSS_DAILY_SETTLE, s, false);
}

#pragma mark - 皇陵宝藏 -

//【皇陵寻宝】获取搜索信息
void HttpClient::getSearchDragonValley(unsigned int userId, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_GETSEARCH,s<<"userId"<<userId);
}

//皇陵寻宝】获取宝库信息
void HttpClient::getOccupyDragonValley(unsigned int userId, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_GETOCCUPY,s<<"userId"<<userId);
}


//【皇陵寻宝】获取宝库列表信息
void HttpClient::getOccupyListDragonValley(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, DRAGONVALLEY_GETOCCUPYLIST);
}




//【皇陵寻宝】搜索
void HttpClient::searchDragonValley(unsigned int pos, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_SEARCH,s<<"id"<<pos);
}

//皇陵寻宝】加速
void HttpClient::speedUpDragonValley(unsigned int pos, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_SPEEDUP,s<<"id"<<pos);
}

//【皇陵寻宝】获取占领结束或者被抢占的宝库的奖励
void HttpClient::occupyFinishDragonValley(unsigned int pos, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_OCCUPYFINISH,s<<"id"<<pos);
}

//【皇陵寻宝】获取助战奖励列表
void HttpClient::getAssistBonusListDragonValley(cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	doRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_GETASSISTBONUSLIST);
}

//【皇陵寻宝】获取助战奖励
void HttpClient::getAssistBonusDragonValley(unsigned int id, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_GETASSISTBONUS,s<<"id"<<id);
}

//【皇陵寻宝】获取所有助战奖励
void HttpClient::getAllAssistBonusDragonValley(cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	doRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_GETALLASSISTBONUS);
}

//【皇陵寻宝】抢夺
void HttpClient::battleStartDragonValley(unsigned int pos,Json::Value& assist, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "id" << pos << "assist" << assist ;

	CCLog("JsonJsonJson~~%s",s.toString().c_str());

	doBattleRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_BATTLESTART,s);


	//    json_serializer s;
	//    
	//	Json::Value buffs(Json::arrayValue);
	//	CCDictElement * pEle(NULL);
	//	CCDICT_FOREACH(pWarriors, pEle)
	//	{
	//		friends::DungeonWarrior * pWarrior = dynamic_cast<friends::DungeonWarrior *>(pEle->getObject());
	//		if(pWarrior != NULL)
	//		{
	//			Json::Value buffJson = pWarrior->getBuffJson();
	//			if (buffJson.type() != Json::nullValue)
	//			{
	//				buffs.append(buffJson);
	//			}
	//		}
	//	}
	//    
	//    s << "id" << dungeonId << "friendId" << friendId << "wid" << friendWid << "buffs" << buffs;
	//	CCLOG("%s", s.toString().c_str());
	//    doBattleRequset(pSelectorTarget, selector, autoSaveData, FRIEND_GO,s);

}

//【皇陵寻宝】获取采矿完成或者被抢的记录
void HttpClient::getLogDragonValley(cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	doRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_GETLOG);
}

//【皇陵寻宝】个人排行
void HttpClient::getUserRank(unsigned int index , cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "num" << index;
	doRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_USERRANK , s);
}

//【皇陵寻宝】公会排行
void HttpClient::getGuildRank(cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/)
{
	doRequset(pSelectorTarget,selector,autoSaveData,DRAGONVALLEY_GUILDRANK);
}

#pragma mark-狩魂-

void HttpClient::huntGet(cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, SOUL_HUNTER_GET);
}

void HttpClient::huntTargets(uint32_t wId,cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "wid" << wId;
	doRequset(pSelectorTarget, selector, autoSaveData, SOUL_HUNTER_TARGETS,s);
}

void HttpClient::huntAttack(uint32_t tId,uint32_t wId, uint8_t isNpc, cocos2d::CCObject *pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "tid" << tId << "wid" << wId << "isNpc" << isNpc;
	doBattleRequset(pSelectorTarget, selector, autoSaveData, SOUL_HUNTER_ATTACK, s);
}

#pragma mark - 好友 -

void HttpClient::challengeFriend(uint32_t friendId , cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "fid" << friendId;
	doBattleRequset(pSelectorTarget,selector,autoSaveData,FRIEND_CHALLENGE,s,true);
	//doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_CHALLENGE , s);
}

void HttpClient::recommendFriend(unsigned int index , cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;

	//it's not num , it's a fuxking index 
	s << "num" << index;
	doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_RECOMMEND , s);
}

void HttpClient::searchFriend(uint32_t friendId,cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
    json_serializer s;
    s << "fid" << friendId;
    doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_SEARCH,s);
}

void HttpClient::addFriend(uint32_t friendId, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
    json_serializer s;
    s << "fid" << friendId;
    doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_SEND_REQUEST,s);
}


void HttpClient::acceptFriend(uint32_t friendId, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
    json_serializer s;
    s << "fid" << friendId;
    doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_ACCEPT,s);
}

void HttpClient::refuseFriend(uint32_t friendId, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
    json_serializer s;
    s << "fid" << friendId;
    doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_REFUSE,s);
}

void HttpClient::refuseAllFriend(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_REFUSE_ALL);
}

void HttpClient::deleteFriend(unsigned int fid, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	Json::Value Ids(Json::arrayValue);
	Ids.append(fid);
	/*CCObject *pObj = NULL;
	CCARRAY_FOREACH(friendIds, pObj)
	{
		friends::FriendInfo *pInfo = static_cast<friends::FriendInfo *>(pObj);
		Ids.append(pInfo->getFriendID());
	}*/
	json_serializer s;

	doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_DELETE_FRIEND, s << "fids" << Ids <<"fid"<<fid);
}

void HttpClient::friendSendMail(uint32_t friendId, const char *msg, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_SEND_MAIL, s << "friendId" << friendId << "msg" << msg);
}

void HttpClient::friendDeleteMail(uint32_t id, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget,selector,autoSaveData,FRIEND_DELETE_MAIL, s<<"mailId"<<id);
}

void HttpClient::friendSendGift(uint32_t friendId, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_SEND_GIFT, s << "fid" << friendId);
}

void HttpClient::friendReceiveGift(uint32_t friendId, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_RECEIVE_GIFT, s << "fid" << friendId);
}

void HttpClient::friendGetNewRequest(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_GET_NEW_REQUEST);
}

void HttpClient::getFriendRequests(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_GET_REQUESTS);
}

void HttpClient::getFriends(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
    doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_GETFRIENDS);
}

void HttpClient::FriendDungeon(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
    doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_DUNGEON);
}

void HttpClient::resetFriendDungeon(uint32_t dungeonId,cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
    json_serializer s;
    s << "id" << dungeonId;
    doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_RESET_DUNGEON,s);
}

void HttpClient::receiveAndSendGiftAll(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_RECEIVE_AND_SEND);
}


/**
 *	@brief	进去好友副本战斗
 *
 *	@param 	dungeonId 	好友副本编号
 *	@param 	friendId 	用户编号
 *	@param 	friendWid 	好友弟子编号
 *
 */
void HttpClient::goFriendDungeonBattle(uint32_t dungeonId,uint32_t friendId,uint32_t friendWid, cocos2d::CCDictionary *pWarriors,cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
    json_serializer s;

	Json::Value buffs(Json::arrayValue);
	CCDictElement * pEle(NULL);
	CCDICT_FOREACH(pWarriors, pEle)
	{
		friends::DungeonWarrior * pWarrior = dynamic_cast<friends::DungeonWarrior *>(pEle->getObject());
		if(pWarrior != NULL)
		{
			Json::Value buffJson = pWarrior->getBuffJson();
			if (buffJson.type() != Json::nullValue)
			{
				buffs.append(buffJson);
			}
		}
	}

    s << "id" << dungeonId << "friendId" << friendId << "wid" << friendWid << "buffs" << buffs;
	CCLOG("%s", s.toString().c_str());
    doBattleRequset(pSelectorTarget, selector, autoSaveData, FRIEND_GO,s);
}


/**
 *	@brief	扫荡好友副本
 *
 *	@param 	dungeonId 	好友副本编号
 *	@param 	friendId 	用户编号
 */
void HttpClient::rushFriendDungeonBattle(uint32_t dungeonId,uint32_t friendId, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */,bool autoSaveData /* = true */)
{
    json_serializer s;
    s << "id" << dungeonId << "friendId" << friendId;
    doRequset(pSelectorTarget, selector, autoSaveData, FRIEND_RUSH,s);
}

void HttpClient::wonderTrader(unsigned int id, unsigned int buy, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "id" << id << "buy" << buy;
	doRequset(pSelectorTarget, selector, autoSaveData, WONDER_TRADER, s);
}

void HttpClient::wonderSuperior(unsigned int id, unsigned int dbl, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "id" << id << "double" << dbl;
	doRequset(pSelectorTarget, selector, autoSaveData, WONDER_SUPERIOR, s);
}

void HttpClient::wonderTreasure(unsigned int id, unsigned int dbl, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "id" << id << "double" << dbl;
	doRequset(pSelectorTarget, selector, autoSaveData, WONDER_TREASURE, s);
}

void HttpClient::wonderWarrior(unsigned int id, unsigned int buy, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "id" << id << "buy" << buy;
	doRequset(pSelectorTarget, selector, autoSaveData, WONDER_WARRIOR, s);
}

void HttpClient::wonderFight(unsigned int id, unsigned int gg, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "id" << id << "gg" << gg;
	doBattleRequset(pSelectorTarget, selector, autoSaveData, WONDER_FIGHT, s);
}

void HttpClient::wonderBox(unsigned int id, unsigned int gg, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "id" << id << "buy" << gg;
	doRequset(pSelectorTarget, selector, autoSaveData, WONDER_BOX, s);
}

void HttpClient::wonderFightBoss(unsigned int id, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
    json_serializer s;
	s << "id" << id << "gg" << 1;
    doBattleRequset(pSelectorTarget, selector, autoSaveData, WONDER_FIGHT_BOSS, s);
}

void HttpClient::signinGet(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */, bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, SIGNIN_GET,s);
}

void HttpClient::signinSignin(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */, bool autoSaveData /* = true */)
{
	json_serializer s;
	doRequset(pSelectorTarget, selector, autoSaveData, SIGNIN_SIGNIN,s);
}

void HttpClient::getOnlineAward(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */, bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, ACTIVITY_ONLINEAWARD);
}

/**
*	@brief	发送工会聊天信息
*
*	@param 	msg 	聊天内容
*/
void HttpClient::chatSendGuild(const char* msg, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "msg" << msg;
	doRequset(pSelectorTarget, selector, autoSaveData, CHAT_SEND_GUILD, s);
}

/**
*	@brief	获取工会聊天信息
*
*	@param 	gid 	工会编号
*	@param 	id 	请求的下一条消息编号
*/
void HttpClient::chatGetGuild(unsigned int gid, unsigned int id, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "gid" << gid << "id" << id;
	doRequset(pSelectorTarget, selector, autoSaveData, CHAT_GET_GUILD, s, false);
}

void HttpClient::chatSendWorld(const char* msg, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "msg" << msg;
	doRequset(pSelectorTarget, selector, autoSaveData, CHAT_SEND_WORLD, s);
}

void HttpClient::chatGetWorld(unsigned int id, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "id" << id;
	doRequset(pSelectorTarget, selector, autoSaveData, CHAT_GET_WORLD, s, false);
}

void HttpClient::chatCheckNew(unsigned int wLid, unsigned int gLid, unsigned int gid, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/, bool autoSaveData /*= true*/)
{
	json_serializer s;
	s << "wLid" << wLid << "gLid" << gLid << "gid" << gid;
	doRequset(pSelectorTarget, selector, autoSaveData, CHAT_CHK_NEW, s, false);
}

#pragma mark ----------------排行榜------------------------
void HttpClient::getRankInfo(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */, bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, RANK_GETINFO);
}

#pragma mark ----------------小伙伴------------------------
void HttpClient::getGundam(cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */, bool autoSaveData /* = true */)
{
	doRequset(pSelectorTarget, selector, autoSaveData, FORMATION_GETGUNDAM);
}
void HttpClient::joinGundam(uint32_t posId, uint32_t wId, cocos2d::CCObject* pSelectorTarget /* = NULL */, SEL_CallFuncJson selector /* = NULL */, bool autoSaveData /* = true */)
{
	json_serializer s;
	s << "pos" << posId << "wid" << wId ;
	doRequset(pSelectorTarget, selector, autoSaveData, FORMATION_JOINMASTER, s, false);
}

