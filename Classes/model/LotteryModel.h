//
//  LotteryModel.h
//  QSMY
//
//  Created by wanghejun on 13-7-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__LotteryModel__
#define __QSMY__LotteryModel__


#define D_EVENT_LOTTERY_CARDINFO_CHANGED "card info changed"
#define D_EVENT_LOTTERY_INITED           "card inited"
//响应lua请求后，通知C++界面显示抽卡动画
#define D_EVENT_LOTTERY_DRAW_FROMLUA           "card draw from lua"
//普通抽卡
#define D_EVENT_LOTTERY_DRAW           "card draw"

#include "Model.h"

namespace card
{
    
    enum CardType   //抽卡类型 1:百里挑一 2: 千载难逢 3: 万众瞩目
    {
        kCardTypeEasy = 1,
        kCardTypeNormal = 2,
        kCardTypeHard = 3,
		kCardTypeHard10 = 4,
		kCardTypeSpecial = 5,//限时英雄x1
		kCardTypeSpecial10 = 6,//限时英雄x10
    };
    
    class CardInfo:public ValueObjcet
    {
        CC_SYNTHESIZE(uint32_t, m_nTs, Ts);//最后抽奖时间
        CC_SYNTHESIZE(uint32_t, m_nTimes, Times);//剩余免费次数
        CC_SYNTHESIZE(uint32_t, m_nPrice, Price);//价格
        
        CC_SYNTHESIZE(CardType, m_nType, Type);//类型 client
        CC_SYNTHESIZE(uint32_t, m_nCD, CD);//冷却时间 client 秒
        CC_SYNTHESIZE(uint32_t, m_nFreeTimes, FreeTimes);//最大免费次数 client
        
        CC_SYNTHESIZE(bool, m_bIsPaid, IsPaid);//是否对该卡付过费
    public:
        CardInfo()
        :m_nTs(0)
        ,m_nTimes(0)
        ,m_nPrice(0)
        ,m_nCD(0)
        ,m_nFreeTimes(0)
        ,m_bIsPaid(false)
        {
            
        }
        virtual ~CardInfo(){
        }
        
        CREATE_FUNC(CardInfo);
        D_INIT_DEFAULT(ValueObjcet);
        
        //是否需要付费
        bool isCost();
        
        int32_t getWaitTime(); //获取等待时间
        
    };
}

class LotteryModel : public Model
{
    CC_SYNTHESIZE(unsigned int, m_uRestForOrange, RestForOrange);
	//限时英雄
	CC_SYNTHESIZE(unsigned int, m_uRestForSpecial, RestForSpecial);

	//model中存储请求和响应的信息
	CC_SYNTHESIZE(Json::Value, m_ResponseData, ResponseData);
	CC_SYNTHESIZE(Json::Value, m_RequestData, RequestData);

public:
    LotteryModel();
    virtual ~LotteryModel();
    virtual bool init();
    
    /**
     *	@brief	获取最小能免费抽卡的cd
     *
     *	@return	最小能免费抽卡的cd
     */
    time_t getMinCD();
	unsigned int getFreeCount();

	//lua获取限时英雄抽卡CD
	unsigned int getSpecialHeroCD();

    void getCardInfo();
	//限时抽卡响应
	void onDrawCard(const Json::Value &requestData,const Json::Value &responseData);

public:
    card::CardInfo* getCardInfoByType(card::CardType type);
    void updateCard(const Json::Value &val);
    void updateCards(const Json::Value &val);
private:
    void parseInfo(const CSVDecoder::CSV &val);
    void loadData();
private:
    cocos2d::CCDictionary   *m_pCardInfos;
};


#endif /* defined(__QSMY__LotteryModel__) */
