//
//  VideoPlayerImplTODO.h
//  QSMY
//
//  Created by wanghejun on 13-9-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__VideoPlayerImplTODO__
#define __QSMY__VideoPlayerImplTODO__

#include "VideoPlayerImpl.h"
#include "cocos2d.h"

class VideoPlayerImplTODO
:public VideoPlayerImpl
{
public:
    VideoPlayerImplTODO(VideoPlayer* pVideoPlayer);
    ~VideoPlayerImplTODO();
    void playVideo(const char*);
    void playEnd();
};

#endif /* defined(__QSMY__VideoPlayerImplTODO__) */
