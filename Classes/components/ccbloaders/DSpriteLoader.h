//
//  DSpriteLoader.h
//  CCB精灵加载类
//
//  Created by wanghejun on 13-7-5.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DSpriteLoader__
#define __QSMY__DSpriteLoader__

#include "AppInclude.h"
#include "../DSprite.h"

class DSpriteLoader : public cocos2d::extension::CCSpriteLoader {
    
public:
    DSpriteLoader():m_bRectReaded(false){}
    virtual ~DSpriteLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(DSpriteLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(DSprite);
    virtual void onHandlePropTypeCheck(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, bool pCheck, cocos2d::extension::CCBReader * pCCBReader);
    virtual void onHandlePropTypePoint(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, cocos2d::CCPoint pPoint, cocos2d::extension::CCBReader * pCCBReader);
    virtual void onHandlePropTypeSize(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, cocos2d::CCSize pSize, cocos2d::extension::CCBReader * pCCBReader);
protected:
    bool m_bRectReaded;
    cocos2d::CCRect m_Rect;
    void setRect(DSprite*);
};

#endif /* defined(__QSMY__DSpriteLoader__) */
