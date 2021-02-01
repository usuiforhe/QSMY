//
//  VideoPlayerImplIOS.h
//  QSMY
//
//  Created by wanghejun on 13-9-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef QSMY_VideoPlayerImplIOS_h

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)


#define QSMY_VideoPlayerImplIOS_h

#import <Foundation/Foundation.h>
#import <MediaPlayer/MediaPlayer.h>
#include "VideoPlayerImpl.h"
#include "cocos2d.h"

@interface VideoPlayerImplOCIOS : NSObject

@property(nonatomic, strong) MPMoviePlayerController* mediaPlayer;
@property(nonatomic, assign) void* videoPlayer;

-(id) init:(void*) videoPlayer;
-(void) playVideo:(NSURL*) url;
@end


class VideoPlayerImplIOS
:public VideoPlayerImpl
{
public:
    VideoPlayerImplIOS(VideoPlayer* pVideoPlayer);
    ~VideoPlayerImplIOS();
    void playVideo(const char*);
    void playEnd();
private:
    VideoPlayerImplOCIOS* player;
};


#endif  // (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#endif  //QSMY_VideoPlayerImplIOS_h