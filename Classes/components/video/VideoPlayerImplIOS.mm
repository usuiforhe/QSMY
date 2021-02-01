//
//  VideoPlayerImplIOS.m
//  QSMY
//
//  Created by wanghejun on 13-9-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#import "VideoPlayerImplIOS.h"
#include "VideoPlayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import "EAGLView.h"

USING_NS_CC;

@implementation VideoPlayerImplOCIOS

@synthesize mediaPlayer = _mediaPlayer;
@synthesize videoPlayer = _videoPlayer;

-(id) init: (void*) __videoPlayer
{
    self = [super init];
    self.videoPlayer = __videoPlayer;
    return self;
}


-(void) playVideo:(NSURL*) url
{
    UIView *pView = [EAGLView sharedEGLView];
    MPMoviePlayerController *player = [[[MPMoviePlayerController alloc] initWithContentURL:url] autorelease];
    self.mediaPlayer = player;
    player.view.frame = pView.frame;
    player.fullscreen = YES;
    player.scalingMode = MPMovieScalingModeAspectFit;
    player.controlStyle = MPMovieControlStyleNone;
    player.useApplicationAudioSession = YES;
    
    [player prepareToPlay];
    [player play];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                                        selector:@selector(playEnd)
     name:MPMoviePlayerPlaybackDidFinishNotification object:nil];
  
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationWillEnterForeground)
                                                 name:UIApplicationWillEnterForegroundNotification object:nil];
    
    
    [pView addSubview:player.view];
}

- (void)applicationWillEnterForeground {
    
    if(self.mediaPlayer!=nil)
    {
        [self.mediaPlayer prepareToPlay];
        [self.mediaPlayer play];
    }
}

-(void)stateChanged
{
    
}

-(void)playEnd
{
    [self.mediaPlayer.view removeFromSuperview];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:MPMoviePlayerPlaybackDidFinishNotification
                                                  object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:UIApplicationWillEnterForegroundNotification
                                                  object:nil];
    
    ((VideoPlayerImplIOS*)_videoPlayer)->playEnd();
    
}

- (void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:MPMoviePlayerPlaybackDidFinishNotification
                                                  object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:UIApplicationWillEnterForegroundNotification
                                                  object:nil];
    [_mediaPlayer.view removeFromSuperview];
    [_mediaPlayer stop];
    [_mediaPlayer release];
    [super dealloc];
}

@end




#pragma mark -

VideoPlayerImplIOS::VideoPlayerImplIOS(VideoPlayer* pVideoPlayer)
:VideoPlayerImpl(pVideoPlayer)
,player(NULL)
{
    
}

void VideoPlayerImplIOS::playEnd()
{
    if(m_pDelegate)
    {
        m_pDelegate->playEnd();
    }
}

VideoPlayerImplIOS::~VideoPlayerImplIOS()
{
    if(player) [player release];
}

void VideoPlayerImplIOS::playVideo(const char*_url)
{
    if(player) [player release];
    player = [[VideoPlayerImplOCIOS alloc] init:this];
    NSURL* url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:_url]];
    [player playVideo:url];
}


VideoPlayerImpl* __createSystemVideoPlayer(VideoPlayer* pVideoPlayer)
{
    return new VideoPlayerImplIOS(pVideoPlayer);
}

#endif //#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
