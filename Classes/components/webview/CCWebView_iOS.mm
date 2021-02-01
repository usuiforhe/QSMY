//
//  CCWebView_iOS.m
//  CCXWebview
//
//  Created by Vincent on 12-11-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "CCWebView_iOS.h"
#import "EAGLView.h"
#include "cocos2d.h"
#include "AppMacros.h"

@implementation CCWebView_iOS

static CGPoint convertDesignCoordToScreenCoord(const cocos2d::CCPoint& designCoord, bool bInRetinaMode)
{
    cocos2d::CCEGLViewProtocol* eglView = cocos2d::CCEGLView::sharedOpenGLView();
    float viewH = (float)[[EAGLView sharedEGLView] getHeight];
    
    cocos2d::CCPoint visiblePos = ccp(designCoord.x * eglView->getScaleX(), designCoord.y * eglView->getScaleY());
    cocos2d::CCPoint screenGLPos = ccpAdd(visiblePos, eglView->getViewPortRect().origin);
    
    CGPoint screenPos = CGPointMake(screenGLPos.x, viewH - screenGLPos.y);
    
    if (bInRetinaMode)
    {
        screenPos.x = screenPos.x / 2.0f;
        screenPos.y = screenPos.y / 2.0f;
    }
    
    CCLOG("[WebView] pos x = %f, y = %f", screenGLPos.x, screenGLPos.y);
    return screenPos;
}

- (void)showWebView_x:(float)x y:(float)y width:(float) width height:(float)height
{
    if (!m_webview)
    {
        
        float retinaFactor = [[EAGLView sharedEGLView] contentScaleFactor];
        bool bInRetinaMode = D_FLOAT_EQUALS(retinaFactor, 2.0f);
        CGPoint point = convertDesignCoordToScreenCoord(cocos2d::CCPoint(x,y),bInRetinaMode);
        
        cocos2d::CCEGLViewProtocol* eglView = cocos2d::CCEGLView::sharedOpenGLView();
        CGSize controlSize = CGSizeMake(width * eglView->getScaleX(),height * eglView->getScaleY());
        
        if (bInRetinaMode)
        {
            controlSize.width /= 2.0f;
            controlSize.height /= 2.0f;
        }
        
        m_webview = [[UIWebView alloc] initWithFrame:CGRectMake(point.x, point.y, controlSize.width , controlSize.height)];
        [m_webview setDelegate:self];
        
        [[EAGLView sharedEGLView] addSubview:m_webview];
        [m_webview release];
        
        m_webview.backgroundColor = [UIColor clearColor];
        m_webview.opaque = NO;
        
        for (UIView *aView in [m_webview subviews])
        {
            if ([aView isKindOfClass:[UIScrollView class]])
            {
                UIScrollView* scView = (UIScrollView *)aView;
                
                //                [(UIScrollView *)aView setShowsVerticalScrollIndicator:NO]; //右侧的滚动条 （水平的类似）
                [scView setShowsHorizontalScrollIndicator:NO];
                //                scView.bounces = NO;
                
                for (UIView *shadowView in aView.subviews)
                {
                    if ([shadowView isKindOfClass:[UIImageView class]])
                    {
                        shadowView.hidden = YES;  //上下滚动出边界时的黑色的图片 也就是拖拽后的上下阴影
                    }
                }
            }
        }
        
        loading = [[UIActivityIndicatorView alloc] initWithFrame:CGRectMake(0.f, 0.f, 32.f, 32.f)];
        [loading setCenter:CGPointMake(controlSize.width*0.5f , controlSize.height*0.5f)];
        [loading setActivityIndicatorViewStyle:UIActivityIndicatorViewStyleWhite];
        [m_webview addSubview:loading];
        [loading release];
    }
}



- (void)updateURL:(const char*)url
{
    NSString *request = [NSString stringWithUTF8String:url];
    [m_webview loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:request]
                                            cachePolicy:NSURLRequestReloadIgnoringLocalCacheData
                                        timeoutInterval:60]];
}

- (void) dealloc
{
    [self removeWebView];
    [super dealloc];
}

- (void)removeWebView
{
    if(m_webview)[m_webview removeFromSuperview];
    m_webview = nil;
}

#pragma mark - WebView
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
//    NSURL *requestURL = [[request URL] retain];
//    if(([[requestURL scheme] isEqualToString:@"http"] ||[[requestURL scheme] isEqualToString:@"https"] ||[[requestURL scheme] isEqualToString:@"mailto"]) && (navigationType == UIWebViewNavigationTypeLinkClicked))
//    {
//        return ![[UIApplication sharedApplication] openURL:[requestURL autorelease]];
//    }
//    [requestURL release];
    return YES;
}

- (void)webViewDidStartLoad:(UIWebView *)webView
{
    [loading startAnimating];
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    [loading stopAnimating];
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    [loading stopAnimating];
}

@end
