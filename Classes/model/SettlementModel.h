//
//  SettlementModel.h
//  QSMY
//
//  Created by wanghejun on 13-8-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SettlementModel__
#define __QSMY__SettlementModel__

#include "Model.h"
#include "MatchModel.h"

namespace settlement
{
    
    enum SettlementType
    {
        kSettlementNone,
        kSettlementPve,
        kSettlementMatch,
		kSettlementPvp
    };
    
    //奇遇类型 1:高人指点 2:发现宝藏 3:不期而遇 4:云游商人 5:路见不平 6:英雄挑战（恶鬼罗刹） 7:发现宝箱
    enum WonderType
    {
        kWonderTypeNone     = 0,
        kWonderTypeSuperior = 1,
        kWonderTypeTreasure = 2,
        kWonderTypeWarrior  = 3,
        kWonderTypeTrader   = 4,
        kWonderTypeFight    = 5,
		kWonderTypeBoss     = 6,
		kWonderTypeChest    = 7,
        kWonderTypeMax,
    };
    
    class WonderInfo :public ValueObjcet
    {
    public:
        WonderInfo()
        :wonderType(kWonderTypeNone)
        ,productId(0)
		,start(0)
		,end(0)
		,itemID(0)
		,itemNum(0)
		,wonderID(0)
		,exp(0)
		,price(0)
        ,bossId(0)
        ,bossLevel(0)
        {
            
        }
        
        virtual ~WonderInfo()
        {
            
        }
        
        CREATE_FUNC(WonderInfo);
        D_INIT_DEFAULT(ValueObjcet);
        
    public:
        WonderType wonderType;
        uint32_t    productId;
		uint32_t	start;
		uint32_t	end;
		uint32_t	itemID;
		uint32_t	itemNum;
		uint32_t	wonderID;
		uint32_t	exp;
		uint32_t	price;
		uint32_t	bossId;
		uint32_t	bossLevel;
    };
    
    class SettlementInfo :public ValueObjcet
    {
    public:
        SettlementInfo()
        :m_type(kSettlementNone)
        {};
        virtual ~SettlementInfo(){};
        
        bool init(SettlementType type)
        {
            if(!ValueObjcet::init()) return false;
            m_type = type;
            return true;
        }
    public:
        SettlementType m_type;
    };
    
    
    
    
    class SettlementInfoPve :public SettlementInfo
    {
        
    public:
        SettlementInfoPve()
        :grade(0)
        ,inwar(false)
        ,win(false)
        ,exp(0)
        ,wexp(0)
        ,coin(0)
        ,dropId(-1)
        ,m_pWonders(NULL)
        ,m_pWarriorIsLevelUp(NULL)
        ,m_bCanSkipBattle(false)
        ,mapId(0)
        {}
        
        CC_SYNTHESIZE(bool, m_bCanSkipBattle, CanSkipBattle);
        
        virtual ~SettlementInfoPve()
        {
            CC_SAFE_RELEASE(m_pWonders);
            CC_SAFE_RELEASE(m_pWarriorIsLevelUp);
        }
        
        CREATE_FUNC(SettlementInfoPve);
        bool init()
        {
            if(!SettlementInfo::init(kSettlementPve)) return false;
            m_pWonders = cocos2d::CCArray::create();
            CC_SAFE_RETAIN(m_pWonders);
            
            m_pWarriorIsLevelUp = cocos2d::CCDictionary::create();
            CC_SAFE_RETAIN(m_pWarriorIsLevelUp);
            return true;
        }
        
        bool inwar;          //是否发生战斗
        bool win;            //是否胜利
        uint8_t grade;      //结果评分
        
        uint32_t    exp;    //玩家经验
        uint32_t    wexp;   //弟子经验
        uint32_t    coin;   //金币掉落
        
        
        int32_t dropId;
        cocos2d::CCArray* m_pWonders;
        
        uint32_t mapId;
        
        bool isHasWonder()
        {
            return m_pWonders->count()>0;
        }
        
        bool isWarriorLevelup(uint32_t id)
        {
            cocos2d::CCBool* b = (cocos2d::CCBool*)m_pWarriorIsLevelUp->objectForKey(id);
            if(b) return b->getValue();
            return false;
        }
        
        void setWarriorisLevelup(uint32_t id,bool val)
        {
            m_pWarriorIsLevelUp->setObject(cocos2d::CCBool::create(val), id);
        }
        
    private:
        cocos2d::CCDictionary* m_pWarriorIsLevelUp;
        
    };
    
    class SettlementInfoMatch : public SettlementInfo
    {
    public:
        SettlementInfoMatch()
        :m_sUserName()
        ,m_iUserLevel(0)
        ,m_nRank(0)
        ,m_nWarriorID(0)
        {
            
        }
        CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sUserName, UserName);
        CC_SYNTHESIZE(uint32_t, m_iUserLevel, UserLevel);
        CC_SYNTHESIZE(uint32_t, m_nRank, Rank);
        CC_SYNTHESIZE(uint32_t, m_nWarriorID, WarriorID);
        CC_SYNTHESIZE(uint32_t, m_nLdId, AvatarID);
        CREATE_FUNC(SettlementInfoMatch);
        bool init()
        {
            if(!SettlementInfo::init(kSettlementMatch)) return false;
            return true;
        }
    };

	class SettlementInfoPvp : public SettlementInfo
	{
	public:
		SettlementInfoPvp()
			:m_sSelfName()
			,m_sTargetName()
			,m_iSelfFight(0)
			,m_iTargetFight(0)
			,m_iEndurance(0)
			,m_iPoint(0)
			,m_iRewardyb(0)
			,m_bWin(true)
			,m_uTargetID(0)
		{

		}
		virtual ~SettlementInfoPvp()
		{

		}
		/*CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sSelfName, SelfName);
		CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sTargetName, TargetName);
		CC_SYNTHESIZE(uint32_t, m_iSelfFight, SelfFight);
		CC_SYNTHESIZE(uint32_t, m_iTargetFight, TargetFight);
		CC_SYNTHESIZE(int32_t, m_iEndurance, Endurance);
		CC_SYNTHESIZE(uint32_t, m_iPoint, Point);
		CC_SYNTHESIZE(uint32_t, m_iRewardyb, Rewardyb);*/

		std::string m_sSelfName;
		std::string m_sTargetName;
		uint32_t m_iSelfFight;
		uint32_t m_iTargetFight;
		int32_t m_iEndurance;
		uint32_t m_iPoint;
		uint32_t m_iRewardyb;
		bool m_bWin;
		unsigned int m_uTargetID;


		CREATE_FUNC(SettlementInfoPvp);
		bool init()
		{
			if(!SettlementInfo::init(kSettlementPvp)) return false;
			return true;
		}
	};

}

class SettlementModel :public Model
{
    CC_SYNTHESIZE_READONLY(settlement::SettlementInfoPve *, m_pSettlementInfoPve, SettlementInfoPve)
    CC_SYNTHESIZE_READONLY(settlement::SettlementInfoMatch *, m_pSettlementInfoMatch, SettlementInfoMatch)
	CC_SYNTHESIZE_READONLY(settlement::SettlementInfoPvp *, m_pSettlementInfoPvp, SettlementInfoPvp)
public:
    SettlementModel();
    virtual ~SettlementModel();
    virtual bool init();
    void setSettlementInfoPve(const Json::Value &val,int32_t dropId,const Json::Value &wonders, uint32_t mapId);
	void setSettlementInfoPvp(const Json::Value &val , unsigned int target_id);
    void setSettlementInfoMatch(const match::MatchInfo *pMatchInfo);
    void setSettlementInfoMatch(const match::VSInfo *pVSInfo);
private:
    void parseWonder(cocos2d::CCArray* pWonders,const Json::Value &val);
};

#endif /* defined(__QSMY__SettlementModel__) */
