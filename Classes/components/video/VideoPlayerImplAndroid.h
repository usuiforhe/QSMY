//
//  VideoPlayerImplAndroid.h
//  QSMY
//
//  Created by wanghejun on 13-9-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__VideoPlayerImplAndroid__
#define __QSMY__VideoPlayerImplAndroid__

#include "VideoPlayerImpl.h"
#include "cocos2d.h"

class VideoPlayerImplAndroid
:public VideoPlayerImpl
{
public:
    VideoPlayerImplAndroid(VideoPlayer* pVideoPlayer);
    ~VideoPlayerImplAndroid();
    void playVideo(const char*);
    void playEnd();
};

#endif /* defined(__QSMY__VideoPlayerImplAndroid__) */
