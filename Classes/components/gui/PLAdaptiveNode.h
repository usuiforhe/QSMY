//
//  PLAdaptiveNode.h
//  QSMY
//
//  Created by Eci on 14-6-18.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__PLAdaptiveNode__
#define __QSMY__PLAdaptiveNode__

#include "AppInclude.h"

class PLAdaptiveNodeLoader;

class PLAdaptiveNode
:public cocos2d::CCNode
,public cocos2d::extension::CCNodeLoaderListener
{
public:
    CREATE_FUNC(PLAdaptiveNode);
    PLAdaptiveNode();
    virtual ~PLAdaptiveNode();
    D_INIT_DEFAULT(CCNode);

protected:
	friend class PLAdaptiveNodeLoader;
	float m_fMoveWithResolutionDiff;
    float m_fFlexibleHeight;
	void setMoveWithResolutionDiff(float var);
    void setFlexibleHeight(float var);
#pragma mark CCNodeLoaderListener
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
};


class PLAdaptiveNodeLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(PLAdaptiveNodeLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(PLAdaptiveNode);
	virtual void onHandlePropTypeFloat(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, float pFloat, cocos2d::extension::CCBReader * pCCBReader);
	virtual void onHandlePropTypeSize(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, cocos2d::CCSize pSize, cocos2d::extension::CCBReader * pCCBReader);
};


#endif /* defined(__QSMY__PLAdaptiveNode__) */
