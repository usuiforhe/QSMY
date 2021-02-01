//
//  SpecialLottery.h
//  QSMY
//
//  Created by wanghejun on 13-7-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SpecialLottery__
#define __QSMY__SpecialLottery__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/DataManager.h"
#include "LotteryEffect.h"
#include "LotteryAnimation.h"

class SpecialLottery
	:public DLayer
	,public LotteryDelegate
	,public LotteryAnimationDelegate
{

public:
	SpecialLottery();
	virtual ~SpecialLottery();
	virtual bool init();

	CREATE_FUNC(SpecialLottery);

	virtual void animationEnded(card::CardType eType);
	virtual void doCancel(card::CardType eType);
	virtual void doConfirm(card::CardType eType);
	//新入口 进入抽卡动画
	void showAnimation(card::CardType eType);

	void onCardDraw();
	void onLottery();
	void onceMore(DPopup *pView, card::CardType eType);
	void onClose();
	void onShowed(cocos2d::CCObject *exploreNode);
	void drawCard(card::CardType eType , int cost = 1);
	void stopHeroSound(float);
	void playHeroSound(unsigned int wid);
	void onError(const Json::Value & requestData,const Json::Value &responseData);
private:
	LotteryModel *m_pLotteryModel;
	DPopup *m_pEffect;
	LotteryAnimation *m_pAnimate;
	unsigned int m_nSoundID;
};

#endif /* defined(__QSMY__SpecialLottery__) */
