//
//  LotteryModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "LotteryModel.h"
#include "managers/GameManager.h"
#include "utils/TimeUtil.h"

USING_NS_CC;


bool card::CardInfo::isCost()
{
    return m_nCD == 0 || m_nTimes<=0 || getWaitTime()>0;
}

int32_t card::CardInfo::getWaitTime()
{
    int32_t wait = m_nCD - (GameManager::sharedGameManager()->getServerTime()-m_nTs);
    return MAX(0,wait);
}

LotteryModel::LotteryModel()
:m_pCardInfos(NULL)
,m_uRestForOrange(0)
,m_uRestForSpecial(0)
{
    
}

LotteryModel::~LotteryModel()
{
    CC_SAFE_RELEASE(m_pCardInfos);
}

bool LotteryModel::init()
{
    m_pCardInfos = CCDictionary::create();
    CC_SAFE_RETAIN(m_pCardInfos);
    loadData();
    return Model::init();
}

void LotteryModel::loadData()
{
    parseInfo( CSVDecoder::create(LOTTERY_CONFIG_PATH)->getData());
}

void LotteryModel::parseInfo(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
    {
        card::CardInfo* pCardInfo = card::CardInfo::create();
        card::CardType type = (card::CardType)atoi(val[i][0].c_str());
        pCardInfo->setType(type);
        pCardInfo->setCD(atoi(val[i][2].c_str()));
        pCardInfo->setFreeTimes(atoi(val[i][3].c_str()));
        m_pCardInfos->setObject(pCardInfo, type);
    }
}

card::CardInfo* LotteryModel::getCardInfoByType(card::CardType type)
{
    return (card::CardInfo*)m_pCardInfos->objectForKey(type);
}

void LotteryModel::updateCards(const Json::Value &val)
{
    for (uint32_t i=0; i<val.size(); ++i) {
        updateCard(val[i]);
    }
    isSynced = true;
    postNotification(D_EVENT_LOTTERY_INITED);
}

void LotteryModel::updateCard(const Json::Value &val)
{
    card::CardType type = (card::CardType)val["type"].asInt();
    
    card::CardInfo* pCard = getCardInfoByType(type);
    if(!pCard)
    {
        CCLOG("error card!");
        return;
    }
    if(val.isMember("times")) pCard->setTimes(val["times"].asUInt());
    if(val.isMember("ts")) pCard->setTs(val["ts"].asUInt());
    if(val.isMember("price")) pCard->setPrice(val["price"].asUInt());
    if(val.isMember("is_paid")) pCard->setIsPaid(val["is_paid"].asBool());
    
    pCard->isSynced = true;
    pCard->postNotification(D_EVENT_LOTTERY_CARDINFO_CHANGED);
    
}

unsigned int LotteryModel::getSpecialHeroCD()
{
	card::CardInfo* pCard = getCardInfoByType(card::kCardTypeSpecial);
	return pCard->getCD();
}

time_t LotteryModel::getMinCD()
{
    CCDictElement * pElement = NULL;
    card::CardInfo* pCard = NULL;
    
    time_t now = GameManager::sharedGameManager()->getServerTime();
    time_t cd = TimeUtil::getDiff(now, TimeUtil::getStartTimeOfTomorrow(now));

     
    CCDICT_FOREACH(m_pCardInfos, pElement)
    {
        pCard = static_cast<card::CardInfo*>(pElement->getObject());
        if(pCard)
        {
            if((pCard->getTimes()==0 && pCard->getFreeTimes()>1) || pCard->getCD() == 0) continue;
            time_t temp_cd = pCard->getWaitTime();
            cd=temp_cd<cd?temp_cd:cd;
        }
    }
    return cd;
}

void LotteryModel::getCardInfo()
{
    if(!isSynced)
    {
        HTTP_CLIENT->getCardInfo();
    }
}

unsigned int LotteryModel::getFreeCount()
{
	CCDictElement * pElement = NULL;
	card::CardInfo* pCard = NULL;

	time_t now = GameManager::sharedGameManager()->getServerTime();
	unsigned int ct = 0;


	CCDICT_FOREACH(m_pCardInfos, pElement)
	{
		pCard = static_cast<card::CardInfo*>(pElement->getObject());
		if(pCard)
		{
			if((pCard->getTimes()==0 && pCard->getFreeTimes()>1) || pCard->getCD() == 0) continue;
			if(pCard->getType() == card::kCardTypeSpecial || pCard->getType() == card::kCardTypeSpecial10) continue;//限时英雄不算
			time_t temp_cd = pCard->getWaitTime();
			if (temp_cd == 0)
			{
				ct++;
			}
			
		}
	}
	return ct;
}

void LotteryModel::onDrawCard(const Json::Value &requestData,const Json::Value &responseData)
{
	//现在全都要处理
	this->m_RequestData = requestData;
	this->m_ResponseData = responseData;
	//先仅处理限时抽卡的情况，type5，6的情况
	//普通抽卡不做处理
	if(responseData["card"]["type"] == card::kCardTypeSpecial || responseData["card"]["type"] == card::kCardTypeSpecial10)
	{
		postNotification(D_EVENT_LOTTERY_DRAW_FROMLUA);
	}
	else
	{
		postNotification(D_EVENT_LOTTERY_DRAW);
	}
}