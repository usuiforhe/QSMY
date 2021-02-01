//
//  SpecialLottery.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SpecialLottery.h"
#include "../user/UserInfo.h"
#include "../../../managers/GameManager.h"
#include "../../../managers/ResourceManager.h"
#include "../Game.h"
#include "../common/AwardListView.h"
#include <algorithm>
#include "LotteryAnimation.h"
#include "managers/SoundManager.h"


static int sortDropFromCard10(const CCObject* p1, const CCObject* p2)
{
	drop::Drop *pDrop1 = ((drop::Drop *)p1);
	drop::Drop *pDrop2 = ((drop::Drop *)p2);
	if (pDrop1->getType() != pDrop2->getType())
	{
		return pDrop1->getType() > pDrop2->getType();
	}
	else if (pDrop1->getType() == drop::kDropTypeWarrior)
	{
		return pDrop1->getWarrior()->getBaseInfo()->getColorGrade() > pDrop2->getWarrior()->getBaseInfo()->getColorGrade();
	}
	else if (pDrop1->getType() == drop::kDropTypeSoul)
	{
		return pDrop1->getSoul()->getBaseInfo()->getColorGrade() > pDrop2->getSoul()->getBaseInfo()->getColorGrade();
	}
	return 0;
};

USING_NS_CC;
USING_NS_CC_EXT;

SpecialLottery::SpecialLottery()
:m_pLotteryModel(NULL)
,m_pEffect(NULL)
,m_pAnimate(NULL)
,m_nSoundID(0)
{
}

SpecialLottery::~SpecialLottery()
{
    UNBINDING_ALL(m_pLotteryModel, this);
    CC_SAFE_RELEASE(m_pLotteryModel);
}

bool SpecialLottery::init()
{
    if(!DLayer::init()) return false;
    m_pLotteryModel = DM_GET_LOTTERY_MODEL;
    CC_SAFE_RETAIN(m_pLotteryModel);

	BINDING_ONLY(m_pLotteryModel, this, SpecialLottery::onCardDraw , D_EVENT_LOTTERY_DRAW_FROMLUA);
    
    m_pLotteryModel->getCardInfo();
	m_iCurrWid = 0;
    return true;
}

void SpecialLottery::onCardDraw()
{
	if(!isRunning()) return;

	Json::Value requestData = m_pLotteryModel->getRequestData();
	Json::Value responseData = m_pLotteryModel->getResponseData();

	card::CardType eType = (card::CardType)responseData["card"]["type"].asUInt();

	//////////////////////十连抽的情况
	if (requestData["type"].asUInt() == card::kCardTypeSpecial10)
	{
		//显示掉落UI
		DropModel *pModel = DM_GET_DROP_MODEL;
		cocos2d::CCArray *pAwardList = pModel->getDropArray(requestData[UUID].asInt());

		if(!pAwardList || pAwardList->count() == 0) return;

		CCArray *pList = CCArray::create();

		for (unsigned int i = 0; i < pAwardList->count(); i++)
		{
			drop::Drop *pDrop = (drop::Drop *)pAwardList->objectAtIndex(i);
			if (pDrop->getType() == drop::kDropTypeSoul)
			{
				warrior::SoulInfo *pSoul = pDrop->getSoul();
				warrior::WarriorGrowInfo *pGrow = DM_GET_WARRIOR_MODEL->getWarriorGrowInfoByGrade(pSoul->getWid());
				unsigned int num = pSoul->getNum();
				while (num > 0)
				{
					num -= pGrow->getRctSoul();
					pList->addObject(drop::Drop::create(warrior::SoulInfo::create(pSoul->getWid(), pGrow->getRctSoul())));
				}
				continue;
			}
			pList->addObject(pDrop);
		}

		std::sort(pList->data->arr, pList->data->arr + pList->data->num, sortDropFromCard10);

		LotteryEffectTen *pEffect = dynamic_cast<LotteryEffectTen*>(m_pEffect);//LotteryEffectTen::create(pList , true);//传入限时英雄的标志
		if(pEffect)
		{
			pEffect->setData(pList);
			pEffect->showWarrior();
		}
		//pEffect->setCallBack(this, callfuncO_selector(SpecialLottery::onShowed));
		//m_pEffect = pEffect;
		//pEffect->setDelegate(this);
		//pEffect->showWarrior();
		//Game::sharedGame()->openPopup(pEffect);
		//NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asInt()));
		return;
	}
	//////////////////////十连抽结束 下面是一般情况
	//单抽显示内容
	LotteryEffect* pEffect = dynamic_cast<LotteryEffect*>(m_pEffect);
	if(pEffect)
	{
		if(responseData["drop"].isMember("warrior"))
		{
			warrior::WarriorCurInfo* pInfo = DM_GET_WARRIOR_MODEL->parseWarrior(responseData["drop"]["warrior"]);
			pEffect->setData(pInfo , eType);
			pEffect->setWarriorExistsState(false);
			m_iCurrWid = pInfo->getWid();
		}
		else if(responseData["drop"].isMember("soul"))
		{
			warrior::SoulInfo *pInfo = DM_GET_WARRIOR_MODEL->parseSoul(responseData["drop"]["soul"]);
			pEffect->setData(pInfo , eType);
			pEffect->setWarriorExistsState(true);
			m_iCurrWid = pInfo->getWid();
		}
		pEffect->setUserObject(cocos2d::CCInteger::create(requestData[UUID].asUInt()));
	}
}

void SpecialLottery::onLottery()
{
    if(!isRunning()) return;

	Json::Value requestData = m_pLotteryModel->getRequestData();
	Json::Value responseData = m_pLotteryModel->getResponseData();

	if (m_pAnimate)
	{
		m_pAnimate->closePopup();
		m_pAnimate = NULL;
	}
	card::CardType eType = (card::CardType)responseData["card"]["type"].asUInt();
	m_pAnimate = LotteryAnimation::create(eType);
	m_pAnimate->setDelegate(this);
	Game::sharedGame()->openPopup(m_pAnimate);

	if (m_pEffect)
	{
		m_pEffect->closePopup();
		m_pEffect = NULL;
	}
	//////////////////////十连抽的情况
	if (requestData["type"].asUInt() == card::kCardTypeSpecial10)
	{
		//显示掉落UI
		DropModel *pModel = DM_GET_DROP_MODEL;
		cocos2d::CCArray *pAwardList = pModel->getDropArray(requestData[UUID].asInt());

		if(!pAwardList || pAwardList->count() == 0) return;

		CCArray *pList = CCArray::create();

		for (unsigned int i = 0; i < pAwardList->count(); i++)
		{
			drop::Drop *pDrop = (drop::Drop *)pAwardList->objectAtIndex(i);
			if (pDrop->getType() == drop::kDropTypeSoul)
			{
				warrior::SoulInfo *pSoul = pDrop->getSoul();
				warrior::WarriorGrowInfo *pGrow = DM_GET_WARRIOR_MODEL->getWarriorGrowInfoByGrade(pSoul->getWid());
				unsigned int num = pSoul->getNum();
				while (num > 0)
				{
					num -= pGrow->getRctSoul();
					pList->addObject(drop::Drop::create(warrior::SoulInfo::create(pSoul->getWid(), pGrow->getRctSoul())));
				}
				continue;
			}
			pList->addObject(pDrop);
		}

		std::sort(pList->data->arr, pList->data->arr + pList->data->num, sortDropFromCard10);

		LotteryEffectTen *pEffect = LotteryEffectTen::create(pList , true);//传入限时英雄的标志

		pEffect->setCallBack(this, callfuncO_selector(SpecialLottery::onShowed));
		m_pEffect = pEffect;
		pEffect->setDelegate(this);
		pEffect->showWarrior();
		Game::sharedGame()->openPopup(pEffect);
		//NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asInt()));
		return;
	}
	//////////////////////十连抽结束 下面是一般情况

    LotteryEffect* pEffect = NULL;
    if(responseData["drop"].isMember("warrior"))
    {
        warrior::WarriorCurInfo* pInfo = DM_GET_WARRIOR_MODEL->parseWarrior(responseData["drop"]["warrior"]);
        pEffect = LotteryEffect::create(pInfo, eType);
		pEffect->setWarriorExistsState(false);
		m_iCurrWid = pInfo->getWid();
    }
    else if(responseData["drop"].isMember("soul"))
    {
        warrior::SoulInfo *pInfo = DM_GET_WARRIOR_MODEL->parseSoul(responseData["drop"]["soul"]);
        pEffect = LotteryEffect::create(pInfo, eType);
        pEffect->setWarriorExistsState(true);
		m_iCurrWid = pInfo->getWid();
    }

	m_pEffect = pEffect;

    if(pEffect)
    {
		pEffect->setDelegate(this);
        pEffect->setCallBack(this, callfuncO_selector(SpecialLottery::onShowed));
        pEffect->setUserObject(cocos2d::CCInteger::create(requestData[UUID].asUInt()));
        Game::sharedGame()->openPopup(pEffect);
	}
}

void SpecialLottery::onShowed(cocos2d::CCObject *exploreNode)
{
	m_pEffect = NULL;
//     CCNode* pEffect = dynamic_cast<CCNode*>(exploreNode);
//     NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_REMOVE,pEffect->getUserObject());
}


void SpecialLottery::onceMore(DPopup *pView, card::CardType eType)
{
//	pView->closePopup();
	drawCard(eType);
}

void SpecialLottery::showAnimation(card::CardType eType)
{
	if (m_pAnimate)
	{
		m_pAnimate->closePopup();
		m_pAnimate = NULL;
	}
	m_pAnimate = LotteryAnimation::create(eType);
	m_pAnimate->setDelegate(this);
	Game::sharedGame()->openPopup(m_pAnimate);

	if (m_pEffect)
	{
		m_pEffect->closePopup();
		m_pEffect = NULL;
	}

	if (eType == card::kCardTypeSpecial10)
	{	
		LotteryEffectTen *pEffect = LotteryEffectTen::create(true);
		pEffect->setCallBack(this, callfuncO_selector(SpecialLottery::onShowed));
		m_pEffect = pEffect;
		pEffect->setDelegate(this);
		//pEffect->showWarrior();
		Game::sharedGame()->openPopup(pEffect);
	}
	else
	{
		//预先加载单抽（有界面无数据）
		LotteryEffect* pEffect = LotteryEffect::create();
		m_pEffect = pEffect;
		pEffect->setDelegate(this);
		pEffect->setCallBack(this, callfuncO_selector(SpecialLottery::onShowed));
		Game::sharedGame()->openPopup(pEffect);	
	}

}

void SpecialLottery::drawCard(card::CardType eType , int cost)
{
	if(eType<card::kCardTypeSpecial || eType>card::kCardTypeSpecial10)
	{
		CCLOG("error type");
		return;
	}
	card::CardInfo* pCard = m_pLotteryModel->getCardInfoByType(eType);
	if (!pCard)
	{
		return;
	}


	if(pCard->isCost() && pCard->getType() > card::kCardTypeEasy  && pCard->getPrice()>DM_GET_USER_MODEL->getUserInfo()->getPoints())
	{
		if (m_pEffect)
		{
			m_pEffect->closePopup();
			m_pEffect = NULL;
		}
		//closeMorePop(NULL);
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
		return;
	}

	if(cost == 0 && pCard->isCost())
	{
		//不能正常免费！重新请求！
		Tips_Alert(D_LOCAL_STRING("SpecialHeroFreeError").c_str());
		HTTP_CLIENT->activityGet();
		HTTP_CLIENT->getCardInfo();
		onClose();
		return;
	}
	showAnimation(eType);
	//这里不使用回调，在exeFunc里面处理回调
	//HTTP_CLIENT->drawCard(pCard->getType(),pCard->isCost());//,this,callfuncJson_selector(SpecialLottery::onLottery));
}

void SpecialLottery::doConfirm(card::CardType eType)
{

	if(eType<card::kCardTypeSpecial || eType>card::kCardTypeSpecial10)
	{
		CCLOG("error type");
		return;
	}
	card::CardInfo* pCard = m_pLotteryModel->getCardInfoByType(eType);
	if (!pCard)
	{
		return;
	}


	if(pCard->isCost() && pCard->getType() > card::kCardTypeEasy  && pCard->getPrice()>DM_GET_USER_MODEL->getUserInfo()->getPoints())
	{
		if (m_pEffect)
		{
			m_pEffect->closePopup();
			m_pEffect = NULL;
		}
		//closeMorePop(NULL);
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
		return;
	}
	//这里不使用回调，在exeFunc里面处理回调
	//HTTP_CLIENT->drawCard(pCard->getType(),pCard->isCost());//,this,callfuncJson_selector(SpecialLottery::onLottery));
	HTTP_CLIENT->drawCard(pCard->getType(),pCard->isCost(),this,callfuncJson_selector(SpecialLottery::onError));
}

void SpecialLottery::onError(const Json::Value & requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0)
	{
		doCancel(card::kCardTypeEasy);
	}
}

void SpecialLottery::doCancel(card::CardType eType)
{
	if (m_pAnimate)
	{
		m_pAnimate->closePopup();
		m_pAnimate = NULL;
	}
	if (m_pEffect)
	{
		m_pEffect->closePopup();
		m_pEffect = NULL;
	}
	//确保返回时，界面可以点击
	Game::sharedGame()->setTouchEnable(true);
}

void SpecialLottery::animationEnded(card::CardType eType)
{
//	card::CardInfo* pCard = m_pLotteryModel->getCardInfoByType(eType);
	playHeroSound(m_iCurrWid);
	if (m_pAnimate)
	{
		m_pAnimate->closePopup();
		m_pAnimate = NULL;
	}
}

void SpecialLottery::onClose()
{
	//隐藏底部导航
	Game::sharedGame()->setNavigatorVisable(false);
	removeFromParent();
}

void SpecialLottery::playHeroSound(unsigned int wid)
{
	stopHeroSound(0.f);
	if(wid == 0) return;
	sound::Sound* pSound = DM_GET_SOUND_MODEL->getUnitSound(wid, 0, "voice");
	if(NULL == pSound) return;
	m_nSoundID = SoundManager::sharedSoundManager()->playSFX(CCString::createWithFormat(pSound->getName().c_str()));
	scheduleOnce(schedule_selector(SpecialLottery::stopHeroSound), pSound->getDuration());
}

void SpecialLottery::stopHeroSound(float delay)
{
	m_iCurrWid = 0;
	if (m_nSoundID) {
		SoundManager::sharedSoundManager()->stopSFX(m_nSoundID);
		m_nSoundID = 0;
	}
}