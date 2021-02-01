//
//  Splash.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Splash.h"
#include "VisibleRect.h"
#include "managers/SceneManager.h"
#include "utils/SystemUtil.h"

USING_NS_CC;

Splash::Splash()
:pPlayer(NULL)
,m_nStartTs(0)
{
    
}

Splash::~Splash()
{
    if(pPlayer)
    {
        pPlayer->setDelegate(NULL);
    }
    CC_SAFE_RELEASE(pPlayer);
}

bool Splash::init()
{
    if(!BaseScenes::init())
    {
        return false;
    }
    const CSVDecoder::CSV& pVideoData =  CSVDecoder::create(VIDEO_CONFIG_PATH)->getData();
    for (uint32_t i=0; i<pVideoData.size(); ++i)
	{
		videos.push(pVideoData[i][0]);
	}
    
    pPlayer = VideoPlayer::create();
    pPlayer->setDelegate(this);
    CC_SAFE_RETAIN(pPlayer);
    
    return true;

}

bool Splash::dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches)
{
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    if(!CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_VIDEO_SKIP,false) && now.tv_sec-m_nStartTs < 2) return false;
    playEnd();
    return false;
}

void Splash::onEnter()
{
	BaseScenes::onEnter();
	playNext();
}

void Splash::playNext()
{
    if(videos.size()==0)
    {
        setTouchEnabled(false);
        CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_VIDEO_SKIP, true);
        CCUserDefault::sharedUserDefault()->flush();
        if(pPlayer)
        {
            pPlayer->setDelegate(NULL);
        }
        CC_SAFE_RELEASE_NULL(pPlayer);
        SceneManager::goScene(SceneManager::kSceneTagLogin);
        return;
    }
    std::string video = videos.front();
    videos.pop();
    pPlayer->playVideo(video.c_str());
    if(videos.size()==0)
    {
        struct cc_timeval now;
        CCTime::gettimeofdayCocos2d(&now, NULL);
        m_nStartTs = now.tv_sec;
    }
}

void Splash::playEnd()
{
    playNext();
}


