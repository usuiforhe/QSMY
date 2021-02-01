//
//  PLAdaptiveScale9Sprite.h
//  QSMY
//
//  Created by Eci on 14-7-1.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__PLAdaptiveScale9Sprite__
#define __QSMY__PLAdaptiveScale9Sprite__

#include "AppInclude.h"

class PLAdaptiveScale9SpriteLoader;

class PLAdaptiveScale9Sprite
:public cocos2d::extension::CCScale9Sprite
,public cocos2d::extension::CCNodeLoaderListener
{
public:
    CREATE_FUNC(PLAdaptiveScale9Sprite);
    PLAdaptiveScale9Sprite();
    virtual ~PLAdaptiveScale9Sprite();
    D_INIT_DEFAULT(cocos2d::extension::CCScale9Sprite);

protected:
	friend class PLAdaptiveScale9SpriteLoader;
	float m_fFlexibleHeightUp;
	float m_fFlexibleHeightDown;
	float m_fFlexibleHeight;
	void setFlexibleHeightUp(float var);
	void setFlexibleHeightDown(float var);
	void setFlexibleHeight(float var);
    
#pragma mark CCNodeLoaderListener
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
};


class PLAdaptiveScale9SpriteLoader : public cocos2d::extension::CCScale9SpriteLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(PLAdaptiveScale9SpriteLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(PLAdaptiveScale9Sprite);
	virtual void onHandlePropTypeFloat(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, float pFloat, cocos2d::extension::CCBReader * pCCBReader);
	virtual void onHandlePropTypeSize(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, cocos2d::CCSize pSize, cocos2d::extension::CCBReader * pCCBReader);
};


#endif /* defined(__QSMY__PLAdaptiveScale9Sprite__) */
