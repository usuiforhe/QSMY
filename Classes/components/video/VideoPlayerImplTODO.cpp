//
//  VideoPlayerImplTODO.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "VideoPlayerImplTODO.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)

USING_NS_CC;

VideoPlayerImplTODO::VideoPlayerImplTODO(VideoPlayer* pVideoPlayer)
:VideoPlayerImpl(pVideoPlayer)
{
    
}

void VideoPlayerImplTODO::playEnd()
{
    if(m_pDelegate)
    {
        m_pDelegate->playEnd();
    }
}

VideoPlayerImplTODO::~VideoPlayerImplTODO()
{
    
}

void VideoPlayerImplTODO::playVideo(const char*_url)
{
    playEnd();
}


VideoPlayerImpl* __createSystemVideoPlayer(VideoPlayer* pVideoPlayer)
{
    return new VideoPlayerImplTODO(pVideoPlayer);
}
#endif //#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)