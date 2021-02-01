//
//  SceneManager.h
//  场景切换管理
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SceneManager__
#define __QSMY__SceneManager__

#include "AppInclude.h"

class SceneManager
{
public:
    enum sceneTag
    {
        kSceneTagSplash = 0,
        kSceneTagLogin,
        kSceneTagGame,
        kSceneTagNil
    };
    
    static void goScene(SceneManager::sceneTag id = kSceneTagSplash);
    
private :
    static void mGoScene(cocos2d::CCScene* scene);
    static cocos2d::CCScene* wrap(cocos2d::CCLayer *layer);
};



#endif /* defined(__QSMY__SceneManager__) */


