//
//  GameManager.h
//  游戏内容管理类
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__GameManager__
#define __QSMY__GameManager__

#include "AppInclude.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "../network/HttpClient.h"
#include "json/json.h"
#include "CCLuaEngine.h"

class GameManager:public cocos2d::CCObject
{
    
    //CC_SYNTHESIZE_READONLY(DataManager*, m_pDataManager, DataManager);
    CC_SYNTHESIZE_READONLY(HttpClient*, m_pHttpClient, HttpClient);
    
public:
    /** returns a shared instance of the gameManager */
    static GameManager* sharedGameManager(void);
    static void purgeGameManager(void);
    void goScene(SceneManager::sceneTag id);
private:
    GameManager(void);
    ~GameManager(void);
    bool init(void);
    
public:
    
    void initDatas();

#pragma mark 确定服务器时间和客户端运行时间
    
    //确定服务器时间和客户端运行时间
public:
    
    void initScriptEngine();
    
    /**
     *	@brief	获取服务端当前时间
     *
     *	@return	秒
     */
    unsigned int getServerTime();
    
    
    /**
     *	@brief	获取服务端当前时间
     *
     *	@return	毫秒
     */
    double getServerTimeMilliSecond();

    
    /**
     *	@brief	获取客户端运行时间
     *
     *	@return	秒
     */
    unsigned int getTimer();
    
    
    /**
     *	@brief	获取客户端运行时间
     *
     *	@return	毫秒
     */
    double  getTimerMilliSecond();

    
    
    /**
     *	@brief	同步游戏后台
     */
    void SyncGame();

    
    
    /**
     *	@brief	判断服务器时间是否同步
     *
     *	@return	
     */
    bool isServerTimeSynced()

    {
        return m_bIsServerTimeSynced;
    }

	void login();

	void registerUser(const char* const);

	void logined(const Json::Value &request,const Json::Value &respones);   
    
    void registerLocalNotification();
    
private:
    void onSyncTime(const Json::Value &request,const Json::Value &respones);
    
    void checkTutorial();
    void initTutorial();
    void startGame(float dt);
    
private:
    void getDataComplete(const Json::Value &request,const Json::Value &respones);
    uint8_t loaded;
    time_t m_server_ts;
    struct cocos2d::cc_timeval* m_startTime;
    bool m_bIsServerTimeSynced;
};

#endif /* defined(__QSMY__GameManager__) */
