//
//  GameManager.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "GameManager.h"
#include "DataManager.h"
#include "PlatformManager.h"
#include "network/HttpHandler.h"
#include "CCLuaEngine.h"
#include "lua_extensions.h"
#include "luaopen_LuaProxy.h"
#include "LuaQSMY.h"
#include "LuaDragonBone.h"
#include "LuaProxy/LuaLoader.h"

USING_NS_CC;

// singleton stuff
static GameManager *s_SharedGameManager = NULL;

GameManager* GameManager::sharedGameManager(void)
{
    if (!s_SharedGameManager)
    {
        s_SharedGameManager = new GameManager();
        s_SharedGameManager->init();
    }
    
    return s_SharedGameManager;
}


void GameManager::purgeGameManager(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedGameManager);
}

void GameManager::initScriptEngine()
{
    // register lua engine
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
    
    CCLuaStack *pStack = pEngine->getLuaStack();
    pStack->addLuaLoader(lua_loader);
    lua_State* L = pStack->getLuaState();
    //pStack->addSearchPath("scripts/");
    
    luaopen_LuaProxy(L);
    // load lua extensions
    luaopen_lua_extensions(L);
    luaopen_QSMY(L);
    luaopen_DragonBone(L);
    
    pEngine->executeString(readLuaScript("scripts/init.lua").c_str());
}


bool GameManager::init(void)
{
    SystemUtil::cancelNotification();
    m_startTime = new struct cc_timeval();
    m_pHttpClient = HttpClient::create();
    m_pHttpClient->retain();
    m_pHttpClient->setHttpHandler(HttpHandler::create());
    CCLOG("%s","GameManager::init");
    return true;
}

GameManager::GameManager(void)
:loaded(0)
,m_server_ts(0)
,m_startTime(NULL)
,m_bIsServerTimeSynced(false)
{
    
}

GameManager::~GameManager(void)
{
    CCLOG("deallocing GameManager %p", this);
    CC_SAFE_DELETE(m_startTime);
    DataManager::purgeSharedDataManager();
    CC_SAFE_RELEASE(m_pHttpClient);
}

void GameManager::goScene(SceneManager::sceneTag id)
{
    SceneManager::goScene(id);
}


void GameManager::initDatas()
{
    getHttpClient()->initData(this,callfuncJson_selector(GameManager::getDataComplete),true);
    getHttpClient()->getSpecialActivity();
}


void GameManager::getDataComplete(const Json::Value &request,const Json::Value &respones)
{
    if(respones["code"].asString().compare(SUC_CODE)!=0) return;
    checkTutorial();
}

void GameManager::checkTutorial()
{
    bool loadTutorial = false;
    do {
        CC_BREAK_IF(DM_GET_USER_MODEL->getUserInfo()->getUserLv()>DM_GET_TUTORIAL_MODEL->getMaxLevel());
        BINDING_ONLY(DM_GET_TUTORIAL_MODEL, this, GameManager::initTutorial, D_EVENT_TUTORIAL_INITED);
        DM_GET_TUTORIAL_MODEL->getRookieInfo();
        loadTutorial = true;
    } while (0);
    if(!loadTutorial)
    {
        goScene(SceneManager::kSceneTagGame);
    }
}

void GameManager::initTutorial()
{
    UNBINDING_ALL(DM_GET_TUTORIAL_MODEL, this);
    TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(GameManager::startGame), this, 0.f);
}

void GameManager::startGame(float dt)
{
    goScene(SceneManager::kSceneTagGame);
}



void GameManager::SyncGame()
{
    SystemUtil::cancelNotification();
    m_bIsServerTimeSynced = false;
    if(DM_GET_USER_MODEL->getLogin())
    {
        getHttpClient()->syncServerTime(this,callfuncJson_selector(GameManager::onSyncTime));
    }
}

void GameManager::onSyncTime(const Json::Value &request,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    CCTime::gettimeofdayCocos2d(m_startTime, NULL);
    m_server_ts = (time_t)responseData["ts"].asUInt();
    m_bIsServerTimeSynced = true;
    registerLocalNotification();
}

void GameManager::registerLocalNotification()
{
    if(!TimeUtil::isInited()) return;
    uint32_t ts = getServerTime();
    SystemUtil::sendLocalNotification(D_LOCAL_STRING("EatTips").c_str(),
                                      TimeUtil::getDiff(ts,
                                                        TimeUtil::getTimeToday(ts,12,1)));
    SystemUtil::sendLocalNotification(D_LOCAL_STRING("EatTips").c_str(),
                                      TimeUtil::getDiff(ts,
                                                        TimeUtil::getTimeToday(ts,17,1)));
	SystemUtil::sendLocalNotification(D_LOCAL_STRING("EatTips").c_str(),
                                      TimeUtil::getDiff(ts,
                                                        TimeUtil::getTimeToday(ts,21,1)));
// 	GuildBossModel *pBoss = DM_GET_GUILDBOSS_MODEL;
// 	if (pBoss->getIsOpen())
// 	{
// 		SystemUtil::sendLocalNotification(D_LOCAL_STRING("GuildBossWillAppear").c_str(),
// 			TimeUtil::getDiff(ts,
// 			pBoss->getConfig()->getNotitficationTime()));
// 	}
	
}


unsigned int GameManager::getTimer()
{
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    return now.tv_sec - m_startTime->tv_sec;
};

unsigned int GameManager::getServerTime()
{
    return m_server_ts + getTimer();
};

double GameManager::getServerTimeMilliSecond()
{
    return (double)m_server_ts*1000.0+getTimerMilliSecond();
}

double GameManager::getTimerMilliSecond()
{
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    return CCTime::timersubCocos2d(m_startTime, &now);
}


void GameManager::login()
{
	//如果内部登录用getDefaultLoginParam获取登录参数， 平台登录用getLoginParam
    std::string pram;
    if (PlatformManager::sharedPlatformManager()->isInternal()) {
        pram.assign(PlatformManager::sharedPlatformManager()->getDefaultLoginParam());
    }
    else
    {
        pram.assign(PlatformManager::sharedPlatformManager()->getLoginParam());
    }
    
	HTTP_CLIENT->login(pram.c_str(),this,callfuncJson_selector(GameManager::logined));
}

void GameManager::registerUser(const char* const paramStr)
{
	HTTP_CLIENT->registerUser(paramStr,this,callfuncJson_selector(GameManager::logined));
}

void GameManager::logined(const Json::Value &request,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) 
	{
		if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)
		{
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_LOGIN_FAILD);
		}
		return;
	}
	onSyncTime(request,responseData);
	std::string platId;
	if (responseData.isMember("data") && responseData["data"].isMember("plat_id"))
	{
		platId = responseData["data"]["plat_id"].asCString();
		//PlatformManager::sharedPlatformManager()->setUserId(platId);
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_PLATID, platId);
	}
    if(responseData["data"].isMember("account_type"))
	{
		PlatformManager::sharedPlatformManager()->saveAccountInfo(responseData["data"]);
	}
    
    std::string facebookName("");
    if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)
    {
		facebookName = PlatformManager::sharedPlatformManager()->getFacebookUserName();
        CCUserDefault::sharedUserDefault()->setStringForKey(KEY_FACEBOOK_NAME, facebookName);
    }
	
	DM_GET_USER_MODEL->setLogin(true);
    DM_GET_PLATFORM_MODEL->saveMyCurrentServer();
	std::string pDeUserName = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME,"");
	std::string pDePassword = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PASSWORD,"");
	std::string pEmail = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_EMAIL,"");
	std::string pUniqueId = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_UNIQUEID,"");
	int iLoginType = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE, kLogin);
	CCLOG("logined user:%s, %s, %d, %s(fbname)",pDeUserName.c_str(),pDePassword.c_str(), iLoginType,facebookName.c_str());
	DM_GET_PLATFORM_MODEL->saveLoginedUser(pDeUserName.c_str(), pDePassword.c_str(), iLoginType, pEmail.c_str(), facebookName.c_str(), pUniqueId.c_str(), platId.c_str());

	initDatas();
}