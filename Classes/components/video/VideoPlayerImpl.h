//
//  VideoPlayerImpl.h
//  QSMY
//
//  Created by wanghejun on 13-9-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef QSMY_VideoPlayerImpl_h
#define QSMY_VideoPlayerImpl_h

#include "AppInclude.h"
#include "VideoPlayer.h"

class VideoPlayerImpl
:public cocos2d::CCObject
{
    CC_SYNTHESIZE(VideoPlayerDelegate*, m_pDelegate, Delegate);
    
public:
    
    VideoPlayerImpl(VideoPlayer* pVideoPlayer)
    :m_pVideoPlayer(pVideoPlayer)
    ,m_pDelegate(NULL)
    {
        
    }
    virtual ~VideoPlayerImpl()
    {
        
    }
    
    virtual void playVideo(const char*) = 0;
protected:
    VideoPlayer* m_pVideoPlayer;
};

// This method must be implemented at each subclass of VideoPlayerImpl.
extern VideoPlayerImpl* __createSystemVideoPlayer(VideoPlayer* pVideoPlayer);

#endif
