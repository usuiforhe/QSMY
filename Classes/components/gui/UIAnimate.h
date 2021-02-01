//
//  UIAnimate.h
//  QSMY
//
//  Created by wanghejun on 13-12-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__UIAnimate__
#define __QSMY__UIAnimate__

#include "AppInclude.h"
#include "managers/ResourceManager.h"

class UIAnimateLoader;
class UIAnimate;

class UIAnimateDelegate
{
public:
	virtual void animationComplete(UIAnimate *pAnimate) = 0;
};

class UIAnimate
:public cocos2d::CCNodeRGBA
,public cocos2d::extension::CCNodeLoaderListener
{
	CC_SYNTHESIZE_READONLY(sp::Armature *, m_pArmature, Armature);
public:
    CREATE_FUNC(UIAnimate);
    UIAnimate();
    virtual ~UIAnimate();
    D_INIT_DEFAULT(CCNodeRGBA);
    friend class UIAnimateLoader;
    virtual void setOpacity(GLubyte opacity);
	void playAtIndex(uint32_t idx);
	void playWithName(const char *name);
	void playRandomly();
	bool isRunningAnimation();

	void replaceNodeInBone(CCNode *pNode, const char *pBoneName, uint32_t idx = 0);
	
	void stop(); //tolua

	void playStill();//tolua
#pragma mark CCNodeLoaderListener
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

	CC_SYNTHESIZE(UIAnimateDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sAnimateName, AnimateName);
private:
	bool m_bAutoPlay;
	bool m_bUsingBatchNode;
	bool m_bMaxZOrder;

	void animationComplete(sp::Armature * target, const char* event,const char* data);
	bool m_bRunningAnimation;
};


class UIAnimateLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(UIAnimateLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(UIAnimate);
    virtual void onHandlePropTypeString(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, const char * pString, cocos2d::extension::CCBReader * pCCBReader);
	virtual void onHandlePropTypeCheck(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, bool pCheck, cocos2d::extension::CCBReader * pCCBReader);
};


#endif /* defined(__QSMY__UIAnimate__) */
