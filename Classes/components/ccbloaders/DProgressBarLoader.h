//
//  DProgressBarLoader.h
//  进度组件ccbloader
//
//  Created by wanghejun on 13-7-1.
//  Copyright (c) 2013年 thedream. All rights reserved.
                                            //
#ifndef __QSMY__DPROGRESSBARLOADER_H_
#define __QSMY__DPROGRESSBARLOADER_H_

#include "AppInclude.h"
#include "../DProgressBar.h"


/* Forward declaration. */

class DProgressBarLoader : public cocos2d::extension::CCNodeLoader {
    public:
        virtual ~DProgressBarLoader() {};
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(DProgressBarLoader, loader);

    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(DProgressBar);

    virtual void onHandlePropTypeSpriteFrame(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char * pPropertyName, cocos2d::CCSpriteFrame * pCCSpriteFrame, cocos2d::extension::CCBReader * pCCBReader);
};

#endif/* defined(__QSMY__DPROGRESSBARLOADER_H_) */

