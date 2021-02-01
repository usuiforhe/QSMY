//
//  Lottery.h
//  QSMY
//
//  Created by wanghejun on 13-7-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Lottery__
#define __QSMY__Lottery__

#include "AppInclude.h"
#include "components/components.h"
#include "CardButton.h"
#include "managers/DataManager.h"
#include "LotteryEffect.h"
#include "LotteryAnimation.h"

#define CARD_COUNT 3

class SoulExchangeDelegate
{
public:
	virtual void addHead(DLayer*) = 0;
};

class Lottery
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public LotteryDelegate
,public LotteryAnimationDelegate
{
    
public:
    Lottery();
    virtual ~Lottery();
    virtual bool init();
    
    //CREATE_FUNC(Lottery);
	static Lottery* create(SoulExchangeDelegate *);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
	virtual void onExit();
	virtual void onEnter();
	virtual void animationEnded(card::CardType eType);
	virtual void doCancel(card::CardType eType);
	virtual void doConfirm(card::CardType eType);

	void stopHeroSound(float delay);
	void playHeroSound(unsigned int wid);

	CC_SYNTHESIZE(SoulExchangeDelegate *, m_pDelegate, Delegate);
private:
    void updateView();
    void resetView();
    void menuCallBack(cocos2d::CCObject* sender);
    void onLottery(const Json::Value & requestData,const Json::Value &responseData);
	void onError(const Json::Value & requestData,const Json::Value &responseData);
    void onShowed(cocos2d::CCObject *exploreNode);
private:
    LotteryModel *m_pLotteryModel;
	
private:
    CardButton              *m_pCardButtons[CARD_COUNT];

	cocos2d::CCNode	*m_pEffectNode;
	cocos2d::CCNode	*m_pNodeMore;
	DButton	*m_pDBtnClose;

	void moreCallBack(cocos2d::CCObject *pObj);
	void closeMorePop(cocos2d::CCObject *pObj);

	void drawCard(card::CardType eType);
	//新入口 进入抽卡动画
	void showAnimation(card::CardType eType);
	//相应抽卡事件
	void onCardDraw();
	//更新显示
	void updateState();
	DPopup *m_pEffect;
public:
	void onceMore(DPopup *pView, card::CardType eType);
private:
	cocos2d::CCNode *m_pNodeFree;
	cocos2d::CCNode *m_pNodeGold;
	cocos2d::CCNode *m_pItemCost;

	LotteryAnimation *m_pAnimate;

	cocos2d::CCNode *m_pNodeRest;
	cocos2d::CCNode *m_pNodeNext;
	cocos2d::CCLabelBMFont *m_pBMFRest;

	cocos2d::CCLabelBMFont *m_pBMFPriceOne;
	cocos2d::CCLabelBMFont *m_pBMFPriceTen;

	cocos2d::CCNode				*m_pSoulNode;
	cocos2d::CCLabelBMFont	*m_pBMFSoulLeft;
	cocos2d::CCLabelBMFont	*m_pBMFSoulRight;

	void updateRest();

	cocos2d::CCMenuItemImage *m_pBtn[CARD_COUNT];

	DPopup *m_pMorePop;
	unsigned int m_nSoundID;

	bool m_bCurrentCardResponsed;
	bool m_bLotteryEffectTenShowed;
};

#endif /* defined(__QSMY__Lottery__) */
