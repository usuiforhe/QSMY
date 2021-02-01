//
//  Splash.h
//  QSMY
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Splash__
#define __QSMY__Splash__

#include "AppInclude.h"
#include "../BaseScenes.h"
#include <queue>

class Splash
:public BaseScenes
,public VideoPlayerDelegate
{
public:
    Splash();
    virtual ~Splash();
    
    CREATE_FUNC(Splash);
    virtual bool init();
	void onEnter();
    
    virtual bool dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches = NULL);
private:
    
    virtual void playNext();
    virtual void playEnd();
    
private:
    VideoPlayer* pPlayer;
    std::queue<std::string> videos;
    int         m_nStartTs;

};
#endif /* defined(__QSMY__Splash__) */
