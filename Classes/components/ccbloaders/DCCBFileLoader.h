//
//  DCCBFileLoader.h
//  QSMY
//
//  Created by wanghejun on 13-9-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DCCBFileLoader__
#define __QSMY__DCCBFileLoader__

#include "AppInclude.h"

/* Forward declaration. */
class DCCBFileLoader : public cocos2d::extension::CCNodeLoader {
public:
    virtual ~DCCBFileLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(DCCBFileLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(cocos2d::extension::CCBFile);
    
    virtual void onHandlePropTypeCCBFile(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char * pPropertyName, cocos2d::CCNode * pCCBFileNode, cocos2d::extension::CCBReader * pCCBReader);
    
    virtual cocos2d::CCNode * parsePropTypeCCBFile(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader);
};

#endif /* defined(__QSMY__DCCBFileLoader__) */
