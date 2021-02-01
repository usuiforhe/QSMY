//
//  CardButton.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "CardButton.h"
#include "utils/TimerUtil.h"
#include "utils/TimeUtil.h"
#include "managers/ResourceManager.h"
#include "managers/DataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

CardButton::CardButton()
:m_pTimes(NULL)
,m_pFree(NULL)
,m_pFee(NULL)
,m_pCD(NULL)
,m_pPrice(NULL)
,m_pCard(NULL)
//,m_pNoTimes(NULL)
,m_pCDNode(NULL)
,m_fWaitTime(0.f)
,m_pFirstNBCard(NULL)
,m_pItemIcon(NULL)
,m_pPointIcon(NULL)
,m_pNodeItem(NULL)
,m_pBMFOwned(NULL)
,m_pItemCost2(NULL)
,m_pItemCost3(NULL)
,m_pPointCost(NULL)
{
    
}


CardButton::~CardButton()
{
    CC_SAFE_RELEASE(m_pTimes);
    CC_SAFE_RELEASE(m_pFree);
    CC_SAFE_RELEASE(m_pFee);
    CC_SAFE_RELEASE(m_pCD);
    CC_SAFE_RELEASE(m_pPrice);
//    CC_SAFE_RELEASE(m_pNoTimes);
    CC_SAFE_RELEASE(m_pCDNode);
    CC_SAFE_RELEASE(m_pFirstNBCard);
	CC_SAFE_RELEASE(m_pItemIcon);
	CC_SAFE_RELEASE(m_pPointIcon);
	CC_SAFE_RELEASE(m_pNodeItem);
	CC_SAFE_RELEASE(m_pBMFOwned);
	CC_SAFE_RELEASE(m_pItemCost2);
	CC_SAFE_RELEASE(m_pItemCost3);
	CC_SAFE_RELEASE(m_pPointCost);
    SAFE_RELEASE_UNBINDING_ALL(m_pCard, this);
}


bool CardButton::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTimes",  CCLabelBMFont*, this->m_pTimes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFree", CCNode *, this->m_pFree);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFee", CCNode *, this->m_pFee);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCD", CCLabelBMFont *, this->m_pCD);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPrice", CCLabelBMFont *, this->m_pPrice);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoTimes", CCSprite *, this->m_pNoTimes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCDNode", CCNode *, this->m_pCDNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFirstNBCard", CCNode *, this->m_pFirstNBCard);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPointIcon", CCSprite*, this->m_pPointIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemIcon", CCNode*, this->m_pItemIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeItem", CCNode*, this->m_pNodeItem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFOwned", CCLabelBMFont*, this->m_pBMFOwned);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemCost3", CCNode*, this->m_pItemCost3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemCost2", CCNode*, this->m_pItemCost2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPointCost", CCNode*, this->m_pPointCost);
    return false;
}

void CardButton::update(float dt)
{
    m_fWaitTime-=dt;
    if(m_fWaitTime<0.f)
    {
		//在VIEW中计算的倒计时够了！

		//model view 同步 是否真的够了
		m_fWaitTime = (float)m_pCard->getWaitTime();
		if(m_fWaitTime>0.f)
		{
			//继续计时吧
			return;
		}


		//倒计时真的够了！
        m_fWaitTime=0.f;
        TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
		//更新model中的当前次数 刚够一次 肯定是1
		m_pCard->setTimes(1);
        updateInfo();
		//及时退出 不要再显示CD倒计时了
		return;
    }
	//倒计时还没够，继续倒计时
    m_pCD->setString(TimeUtil::getSeconds2String(m_fWaitTime));
}

void CardButton::setCard(card::CardInfo* pCard)
{
    SAFE_RELEASE_UNBINDING_ALL(m_pCard, this);
    CC_SAFE_RETAIN(pCard);
    m_pCard = pCard;
    BINDING_EXEC(m_pCard, this, CardButton::updateInfo, D_EVENT_LOTTERY_CARDINFO_CHANGED);
}

void CardButton::onExit()
{
    TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
}

void CardButton::updateInfo()
{
	//错误处理
    if(!m_pCard){
        m_pFee->setVisible(false);
        return;
    }

	//更新可用免费次数 时间到了可以免费次数+到1
	m_fWaitTime = (float)m_pCard->getWaitTime();
	if(m_fWaitTime<=0.f){
		if(m_pCard->getType() == card::kCardTypeNormal || m_pCard->getType() == card::kCardTypeHard){
			m_pCard->setTimes(1);
		}		
	}

	//是否显示CD
	if (m_pCard->getCD() == 0){
		//不存在CD可能性的 卡
		m_pCDNode->setVisible(false);
	}
	else{
		//有可能CD的卡
		m_pCDNode->setVisible(m_pCard->isCost());
	}

    //是否免费
    m_pFee->setVisible(m_pCard->isCost());
    m_pFree->setVisible(!m_pFee->isVisible());

	//处理3个type的卡显示
	if (m_pCard->getType() == card::kCardTypeEasy) //fixme 都写死了，改成读配置
	{
		m_pPointIcon->setVisible(false);
		m_pItemIcon->setVisible(true);
		m_pNodeItem->setVisible(true);
		 m_pBMFOwned->setString(CCString::createWithFormat("%d",DM_GET_ITEM_MODEL->getItemNum(ZHAO_MU_LING_ID))->getCString());
		 m_pPrice->setString(D_CONFIG_CSTRING_FOR_KEY("ZHAOMULING_COST"));
	}
	else
	{
		m_pPointIcon->setVisible(true);
		m_pItemIcon->setVisible(false);
		m_pNodeItem->setVisible(false);
		 m_pPrice->setString(CCString::createWithFormat("%d",m_pCard->getPrice())->getCString());
	}
	//处理新加的两种招募令
	if(m_pCard->getType() == card::kCardTypeNormal)
	{
		unsigned int itemnum = DM_GET_ITEM_MODEL->getItemNum(NORMAL_CARD_ITEM_ID);
		m_pItemCost2->setVisible(itemnum > 0);
		m_pPointCost->setVisible(!m_pItemCost2->isVisible());
	}
	if(m_pCard->getType() == card::kCardTypeHard)
	{
		unsigned int itemnum = DM_GET_ITEM_MODEL->getItemNum(HARD_CARD_ITEM_ID);
		m_pItemCost3->setVisible(itemnum > 0);
		m_pPointCost->setVisible(!m_pItemCost3->isVisible());
	}

	//定时器，处理CD中的时间
    if(m_fWaitTime>0.f)
    {
        TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
        TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
    }
}



