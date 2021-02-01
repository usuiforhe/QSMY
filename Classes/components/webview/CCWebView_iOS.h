//
//  CCWebView_iOS.h
//
//  Created by Vincent on 12-11-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "cocos2d.h"

@interface CCWebView_iOS : NSObject <UIWebViewDelegate>
{
    UIWebView* m_webview;
    UIActivityIndicatorView *loading;
}

- (void)showWebView_x:(float)x y:(float)y width:(float) width height:(float)height;

- (void)updateURL:(const char*)url;

- (void)removeWebView;

@end
