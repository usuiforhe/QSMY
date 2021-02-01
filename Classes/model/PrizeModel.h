//
//  PrizeModel.h
//  QSMY
//
//  Created by 汪燕军 on 13-9-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PrizeModel__
#define __QSMY__PrizeModel__

#include "Model.h"

#define D_EVENT_PRIZE_INIT     "prize init"         //从服务端同步数据完成
#define D_EVENT_PRIZE_UPDATE   "prize update"       //奖励数据发生变化

#define D_EVENT_CHARGE_PRIZE_INIT     "chargePrize init"         //从服务端同步数据完成
#define D_EVENT_CHARGE_PRIZE_UPDATE   "chargePrize update"       //累充奖励数据发生变化

namespace prize
{
    typedef enum prizeItemCheckerType
    {
        kPrizeItemCheckerNone = 0,
        kPrizeItemCheckerLoginDays = 1,     //累计登录天数
        kPrizeItemCheckerLevel = 2,         //玩家等级
        kPrizeItemCheckerTotalCharge = 3,   //累充金额
    }
    prizeItemCheckerType;
    
    class PrizeItem;
    /**
     *	@brief	奖励检查器
     */
    struct IPrizeItemChecker:cocos2d::CCObject

    {
        virtual ~IPrizeItemChecker(){}
        
        virtual bool init(){return true;};
        /**
         *	@brief	是否满足领取的条件
         *
         *	@param 	threshold 	领取门槛
         *
         *	@return	是否满足
         */
        virtual bool isQualified(uint32_t threshold) = 0;
    };
    
    class PrizeItem: public ValueObjcet
    {
    public:
        CREATE_FUNC(PrizeItem);
        D_INIT_DEFAULT(ValueObjcet);
        
        PrizeItem()
        :m_nId(0)
        ,m_nThreshold(0)
        ,m_nDropType(0)
        ,m_nDropId(0)
        ,m_nDropNum(0)
        ,m_checkerType(kPrizeItemCheckerNone)
        ,m_pPrizeItemChecker(NULL)
        ,m_sDesc("")
        {
        };
        
        virtual ~PrizeItem(){
            CC_SAFE_RELEASE(m_pPrizeItemChecker);
        };
        
        /**
         *	@brief	从行加载
         *
         *	@param 	line 	数据行
         */
        virtual void loadLine(const CSVDecoder::CSVLine & line);
        
        virtual bool isQualified();
        
    protected:
        /**
         *	@brief	奖励编号
         */
        CC_SYNTHESIZE_READONLY(uint32_t, m_nId, Id);
        /**
         *	@brief	领取门槛
         */
        CC_SYNTHESIZE_READONLY(uint32_t, m_nThreshold, Threshold);
        /**
         *	@brief	奖励掉落类型 @drop::DropType类型
         */
        CC_SYNTHESIZE_READONLY(uint32_t, m_nDropType, DropType);
        /**
         *	@brief	奖励掉落编号
         */
        CC_SYNTHESIZE_READONLY(uint32_t, m_nDropId, DropId);
        /**
         *	@brief	奖励掉落数量
         */
        CC_SYNTHESIZE_READONLY(uint32_t, m_nDropNum, DropNum);
        /**
         *	@brief	奖励检查器类型
         */
        CC_SYNTHESIZE_READONLY(prizeItemCheckerType, m_checkerType, CheckerType);
        /**
         *	@brief	完成条件说明
         */
        CC_SYNTHESIZE_READONLY(std::string, m_sDesc, Desc);
        /**
         *	@brief	奖励检查器
         */
        IPrizeItemChecker * m_pPrizeItemChecker;
    };
    
    struct PrizeItemCheckerLevel:public IPrizeItemChecker
    {
        virtual ~PrizeItemCheckerLevel(){};
        
        CREATE_FUNC(PrizeItemCheckerLevel);
        
        /**
         *	@brief	是否满足领取的条件
         *
         *	@param 	threshold 	领取门槛
         *
         *	@return	是否满足
         */
        virtual bool isQualified(uint32_t threshold);
    };
    
    struct PrizeItemCheckerLoginDays:public IPrizeItemChecker
    {
        virtual ~PrizeItemCheckerLoginDays(){};
        
        CREATE_FUNC(PrizeItemCheckerLoginDays);
        /**
         *	@brief	是否满足领取的条件
         *
         *	@param 	threshold 	领取门槛
         *
         *	@return	是否满足
         */
        virtual bool isQualified(uint32_t threshold);
    };
}

#pragma mark ----------------PrizeModel-------------------
class PrizeModel:public Model
{
public:
    CREATE_FUNC(PrizeModel);
    
    PrizeModel();
    
    virtual ~PrizeModel();
    
    virtual bool init();
    
    /**
     *	@brief	从服务端同步数据
     */
    void syncFromServer();
    
    /**
     *	@brief	加载数据
     *
     *	@param 	data 	加载数据
     */
    void parsePrize(const Json::Value & data);
    
    /**
     *	@brief	奖励是否已领取
     *
     *	@param 	pPrizeItem 	奖励项
     *
     *	@return	是否已领取
     */
    bool isPrizeDrawn(prize::PrizeItem * pPrizeItem);
    
    /**
     *	@brief	获取未领取的奖励列表
     *
     *	@return	未领取的奖励列表
     */
    cocos2d::CCArray * getNotDrawnPrizeList();
    
    /**
     *	@brief	累计登录奖励领取完成
     *
     *	@param 	days 	奖励编号
     */
    void onTLoginPrizeDrawn(uint32_t days);
    
    /**
     *	@brief	升级奖励领取完成
     *
     *	@param 	days 	奖励编号
     */
    void onLevelPrizeDrawn(uint32_t level);
    
    /**
     *	@brief	是否所有奖励已领取完成
     *
     *	@return	是否已领取
     */
    bool isDone();

    /**
     *	@brief	获取可领取奖励的数量
     *
     *	@return	数量
     */
    uint32_t getQualifiedNum();

private:
    /**
     *	@brief	加载奖励数据
     */
    void loadPrizeData();

private:
    /**
     *	@brief	奖励列表
     */
    cocos2d::CCArray * m_pPrizeList;
    /**
     *	@brief	已领取的等级奖励 二进制 等级=bit位,如5级为2^4&m_nDrawnLoginDays
     */
    uint32_t m_nDrawnLevel;
    /**
     *	@brief	已领取的累计登录天数奖励 二进制，累计登陆天数=bit位 如3天为2^2&m_nDrawnLoginDays
     */
    uint32_t m_nDrawnLoginDays;
    /**
     *	@brief	累计登录天数
     */
    CC_SYNTHESIZE_READONLY(uint32_t, m_nTotalLoginDays, TotalLoginDays);
    
    /**
     *	@brief	是否已完成
     */
    bool m_bIsDone;
public:
	bool isXueNvDrawn();
};

#endif /* defined(__QSMY__PrizeModel__) */
