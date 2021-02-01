//
//  SceneManager.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SceneManager.h"
#include "../scenes/login/Login.h"
#include "../scenes/game/Game.h"
#include "../scenes/splash/Splash.h"

USING_NS_CC;

static SceneManager::sceneTag cTag = SceneManager::kSceneTagNil;

void SceneManager::goScene(SceneManager::sceneTag id)
{
    if(cTag==id) return;
    cTag = id;
    CCLayer* layer;
    switch (id) {
        case SceneManager::kSceneTagSplash:
            layer = Splash::create();
            break;
        case SceneManager::kSceneTagLogin:
            layer = Login::create();
            break;
        case SceneManager::kSceneTagGame:
            layer = Game::sharedGame();
            break;
        default:
            layer = CCLayerColor::create(ccc4(1, 1, 1, 1));
            break;
    }

	CCScene* newScene = NULL;
	if (id == kSceneTagGame)
	{
		newScene = CCTransitionFadeUp::create(1.0f , SceneManager::wrap(layer));
	}
	else
	{
		newScene = SceneManager::wrap(layer);
	}
	
    SceneManager::mGoScene(newScene);
}

void SceneManager::mGoScene(CCScene *scene)
{
    CCDirector* director = CCDirector::sharedDirector();
    
    if (director->getRunningScene())
    {
        director->replaceScene(scene);
    }
    else
    {
        director->runWithScene(scene);
    }
}

CCScene* SceneManager::wrap(CCLayer *layer)
{
    CCScene* newScene = CCScene::create();
    newScene->addChild(layer);
    return newScene;

}

