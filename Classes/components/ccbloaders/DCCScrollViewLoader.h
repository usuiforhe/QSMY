//
//  DCCScrollViewLoader.h
//  QSMY
//
//  Created by wanghejun on 13-10-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DCCScrollViewLoader__
#define __QSMY__DCCScrollViewLoader__

#include "AppInclude.h"

class DCCScrollViewLoader : public cocos2d::extension::CCScrollViewLoader {
public:
    virtual ~DCCScrollViewLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(DCCScrollViewLoader, loader);
    
protected:
    virtual cocos2d::CCNode * parsePropTypeCCBFile(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader);
};

#endif /* defined(__QSMY__DCCScrollViewLoader__) */
