//
//  LotteryAnimation.h
//  QSMY
//
//  Created by Eci on 14-8-22.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__LotteryAnimation__
#define __QSMY__LotteryAnimation__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/DataManager.h"

class LotteryAnimationDelegate
{
public:
	virtual void animationEnded(card::CardType eType) = 0;
	virtual void doCancel(card::CardType eType) = 0;
	virtual void doConfirm(card::CardType eType) = 0;
};

class LotteryAnimation
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
,public UIAnimateDelegate
{
	CC_SYNTHESIZE(LotteryAnimationDelegate *, m_pDelegate, Delegate);
public:
    
    LotteryAnimation();
    virtual ~LotteryAnimation();
    
    static LotteryAnimation* create(card::CardType eType);
    virtual bool init(card::CardType eType);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	virtual void animationComplete(UIAnimate *pAnimate);
	virtual void animationEnd();

	void doConfirm();
	virtual void onEnter();
private:
	UIAnimate *m_pAnimateLottery;
	UIAnimate *m_pAnimateBack;
	UIAnimate *m_pAnimateShine;

	cocos2d::CCMenuItemImage	*m_pBack;
	cocos2d::CCNode						*m_pTouchNode;
	card::CardType m_eType;
	bool m_bTouching;
	void playShine();
	void callBackCancel(CCObject*);
};

#endif /* defined(__QSMY__LotteryAnimation__) */
