//
//  ImageLoader.h
//  QSMY
//
//  Created by wanghejun on 14-2-12.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__ImageLoader__
#define __QSMY__ImageLoader__

#include "AppInclude.h"

class ImageLoader:public cocos2d::CCNode {
    
public:
    
    ImageLoader();
    virtual ~ImageLoader();
    
    static ImageLoader* create(const char* path);
    bool init(const char* path);
    
private:
    void onDownLoadComplete();
    
private:
    std::string localFile;
    
};

#endif /* defined(__QSMY__ImageLoader__) */
