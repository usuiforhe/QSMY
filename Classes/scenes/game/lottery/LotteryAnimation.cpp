//
//  LotteryAnimation.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "LotteryAnimation.h"
#include "../Game.h"

USING_NS_CC;
using namespace sp;

LotteryAnimation::LotteryAnimation()
:m_pAnimateLottery(NULL)
,m_pAnimateBack(NULL)
,m_pAnimateShine(NULL)
,m_pDelegate(NULL)
,m_pBack(NULL)
,m_pTouchNode(NULL)
,m_bTouching(false)
{
}

LotteryAnimation::~LotteryAnimation()
{
	CC_SAFE_RELEASE(m_pAnimateLottery);
	CC_SAFE_RELEASE(m_pAnimateBack);
	CC_SAFE_RELEASE(m_pAnimateShine);
	CC_SAFE_RELEASE(m_pBack);
	CC_SAFE_RELEASE(m_pTouchNode);
}

LotteryAnimation* LotteryAnimation::create(card::CardType eType)
{
    LotteryAnimation* pRet = new LotteryAnimation();
    if(pRet&& pRet->init(eType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

bool LotteryAnimation::init(card::CardType eType)
{
	if(!DPopup::init()) return false;

	m_eType = eType;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_shop/PL_chouka_ani");
	m_pAnimateBack->setVisible(true);
	m_pAnimateLottery->stop();
	m_pAnimateLottery->setVisible(false);
	m_pBack->setTarget(this, menu_selector(LotteryAnimation::callBackCancel));
    return true;
}

void LotteryAnimation::callBackCancel(CCObject*)
{
	if (m_pDelegate)
	{
		m_pDelegate->doCancel(m_eType);
	}
}

void LotteryAnimation::onEnter()
{
	DPopup::onEnter();
}

bool LotteryAnimation::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateLottery", UIAnimate *, m_pAnimateLottery);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateBack", UIAnimate *, m_pAnimateBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateShine", UIAnimate *, m_pAnimateShine);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBack", CCMenuItemImage *, m_pBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchNode", CCNode *, m_pTouchNode);
    return false;
}

bool LotteryAnimation::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLOG("+++++++++++++++LotteryAnimation::ccTouchBegan");
	bool ret = DPopup::ccTouchBegan(pTouch, pEvent);
	CCRect touchArea = CCRectMake(m_pTouchNode->getPositionX() , m_pTouchNode->getPositionY() , m_pTouchNode->getContentSize().width , m_pTouchNode->getContentSize().height);
	
	bool inArea = touchArea.containsPoint(pTouch->getLocation());
	if (ret && inArea)
	{
		doConfirm();

		m_pAnimateBack->setVisible(false);
		m_pAnimateBack->stop();
		m_pAnimateLottery->setVisible(true);
		m_pAnimateLottery->playAtIndex(0);
		m_pAnimateLottery->setDelegate(this);
		m_bTouching = true;
		playShine();

		
		/*this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.0f), 
			CCCallFunc::create(this, callfunc_selector(LotteryAnimation::doConfirm))));		*/
	}
	return ret;
}

void LotteryAnimation::doConfirm()
{
	if (m_pDelegate)
	{
		m_pDelegate->doConfirm(m_eType);
	}
}

void LotteryAnimation::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLOG("+++++++++++++++LotteryAnimation::ccTouchCancelled");
	m_pAnimateLottery->playAtIndex(2);
	m_pAnimateLottery->setDelegate(this);
	DPopup::ccTouchCancelled(pTouch, pEvent);
	m_bTouching = false;
	m_pAnimateShine->stop();
	m_pAnimateShine->setDelegate(NULL);
	Game::sharedGame()->setTouchEnable(false);
}

void LotteryAnimation::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLOG("+++++++++++++++LotteryAnimation::ccTouchEnded");
	if(m_bTouching)
	{
		m_pAnimateLottery->playAtIndex(2);
		m_pAnimateLottery->setDelegate(this);
		this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(1.3f), 
			CCCallFunc::create(this, callfunc_selector(LotteryAnimation::animationEnd))));
		DPopup::ccTouchEnded(pTouch, pEvent);
		m_bTouching = false;
		m_pAnimateShine->stop();
		m_pAnimateShine->setDelegate(NULL);
		m_pAnimateShine->setVisible(false);
		Game::sharedGame()->setTouchEnable(false);
	}	
}

void LotteryAnimation::animationComplete(UIAnimate *pAnimate)
{
	if (pAnimate == m_pAnimateShine && m_bTouching)
	{
		playShine();
	}
	else if (m_bTouching)
	{
		m_pAnimateLottery->playAtIndex(1);
	}
	else
	{
		m_pAnimateLottery->setDelegate(NULL);
		Game::sharedGame()->setTouchEnable(true);
		if (m_pDelegate)
		{
			m_pDelegate->animationEnded(m_eType);
		}
	}
}

void LotteryAnimation::animationEnd()
{
	animationComplete(m_pAnimateLottery);
}

void LotteryAnimation::playShine()
{
	float rx = CCRANDOM_0_1() * D_DESIGN_SIZE_WIDTH;
	float ry = CCRANDOM_0_1() * D_DESIGN_SIZE_HEIGHT;
	m_pAnimateShine->setPosition(rx, ry);
	m_pAnimateShine->setDelegate(this);
	m_pAnimateShine->playAtIndex(0);
}