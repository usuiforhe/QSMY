//
//  LoadingLayer.h
//  加载动画
//
//  Created by wanghejun on 13-8-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
//	for battle only

#ifndef __QSMY__LoadingLayer__
#define __QSMY__LoadingLayer__

#include "AppInclude.h"
#include "components/components.h"

class LoadingLayer;

class LoadingLayerDelegate
{
public:
	virtual void loadingLayerTouched(LoadingLayer *pLayer) = 0;
};

class LoadingLayer
:public DLayerModal
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public UIAnimateDelegate
{
	CC_SYNTHESIZE(LoadingLayerDelegate *, m_pDelegate, Delegate);
public:
    
    CREATE_FUNC(LoadingLayer);
    virtual bool init();
    D_TOUCH_REGISTER_DEFAULT( DTOUCH_DISABLE_TOUCH_PRIORITY );
    
    static LoadingLayer* sharedLoadingLayer(void);
    static void purgeLoadingLayer(void);
    virtual void onEnter();
	virtual void onExit();

	void setWarriorIds(unsigned int wid0, unsigned int wid1);

	void setMenuEnabled(bool bEnabled);

	virtual void animationComplete(UIAnimate *pAnimate);

	void setLoadingPercent(float pct);
private:
    LoadingLayer();
    virtual ~LoadingLayer();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler					onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler	onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName) {return NULL;}

private:
    cocos2d::CCLabelTTF* m_pTips;
	UIAnimate *m_pAnimateLeft;
	UIAnimate *m_pAnimateRight;
	UIAnimate *m_pAnimateCenter;
	UIAnimate *m_pAnimateCenterBegin;
	unsigned int m_uWid0;
	unsigned int m_uWid1;
	bool m_bMenuEnabled;

	void setBattleWarriors();

	void callBackTouch(cocos2d::CCObject *pObj);

	void updateTouched(float dt);

	float m_fPercent;

	void showPercentAni(float num);
};

#endif /* defined(__QSMY__LoadingLayer__) */
