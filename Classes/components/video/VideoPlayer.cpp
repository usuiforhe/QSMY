//
//  VideoPlayer.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "VideoPlayer.h"
#include "VideoPlayerImpl.h"

USING_NS_CC;

VideoPlayer::VideoPlayer()
:m_pVideoPlayerImpl(NULL)
,m_pDelegate(NULL)
{
    
}

VideoPlayer::~VideoPlayer()
{
    CC_SAFE_DELETE(m_pVideoPlayerImpl);
}

bool VideoPlayer::init()
{
    m_pVideoPlayerImpl = __createSystemVideoPlayer(this);
    return true;
}


void VideoPlayer::playVideo(const char* url)
{
    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(url);
    m_pVideoPlayerImpl->playVideo(path.c_str());
}

void VideoPlayer::setDelegate(VideoPlayerDelegate *pDelegate)
{
    m_pDelegate = pDelegate;
    if (m_pVideoPlayerImpl != NULL)
    {
        m_pVideoPlayerImpl->setDelegate(pDelegate);
    }
   
}