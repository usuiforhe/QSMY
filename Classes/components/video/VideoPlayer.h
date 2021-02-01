//
//  VideoPlayer.h
//  QSMY
//
//  Created by wanghejun on 13-9-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__VideoPlayer__
#define __QSMY__VideoPlayer__

#include "AppInclude.h"


class VideoPlayerImpl;

class VideoPlayerDelegate
{
public:
    virtual ~VideoPlayerDelegate() {};
    virtual void playEnd() = 0;
};

    
class VideoPlayer
:public cocos2d::CCObject
{
    CC_SYNTHESIZE_READONLY(VideoPlayerDelegate*, m_pDelegate, Delegate);
    
public:
    VideoPlayer();
    virtual ~VideoPlayer();
    CREATE_FUNC(VideoPlayer);
    bool init();
    
    void playVideo(const char*);
    
    void setDelegate(VideoPlayerDelegate* pDelegate);
    
private:
    VideoPlayerImpl *m_pVideoPlayerImpl;
};

#endif /* defined(__QSMY__VideoPlayer__) */
