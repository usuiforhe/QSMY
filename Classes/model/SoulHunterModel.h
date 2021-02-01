//
//  SoulHunterModel.h
//  狩魂
//
//  Created by liuxiaogang on 13-10-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SoulHunterModel__
#define __QSMY__SoulHunterModel__

#include "Model.h"
#include "WarriorModel.h"

#define D_EVENT_SOULHUNTER_POWER_CHANGED "power changed"						//剩余挑战次数变化
#define D_EVENT_SOULHUNTER_UPDATED "hunter updated"								//个人狩魂信息变化
#define D_EVENT_SOULHUNTER_RIVALS_UPDATED "rivals update"						//对手列表变化
#define D_EVENT_SOULHUNTER_SNATCH_FINISH "snatch finish"						//抢夺战斗结束
#define D_EVENT_SOULHUNTER_NOT_EXIST "not exsit"                                //来迟一步刷新对手列表

#define SOUL_HUNTER_TYPE_SUCCESS	1											//战斗胜利并抢到魂魄
#define SOUL_HUNTER_TYPE_FAIL		2											//战斗胜利但未抢到魂魄
#define SOUL_HUNTER_TYPE_DEFEATE	3											//战斗失败

class SoulHunterModel;

namespace soulhunter
{
    
    class SoulHunterInfo:public ValueObjcet
    {
    public:
        SoulHunterInfo()
		:m_pPower(0)
		,m_pMaxPower(0)
		,m_pProtectTs(0)
        {
        }
        
		~SoulHunterInfo()
		{
		}
		CREATE_FUNC(SoulHunterInfo);
		D_INIT_DEFAULT(ValueObjcet);
		CC_SYNTHESIZE_READONLY(uint32_t, m_pPower, Power);
		CC_SYNTHESIZE_READONLY(uint32_t, m_pMaxPower, MaxPower);
		CC_SYNTHESIZE_READONLY(uint32_t, m_pProtectTs, ProtectTs);
        
		friend class::SoulHunterModel;
    };
    
    
	class SoulHunterRivalInfo:public ValueObjcet
	{
	public:
		SoulHunterRivalInfo()
        :m_pUid(0)
        ,m_pName("")
        ,m_pLv(0)
        ,m_pLid(0)
        ,m_pIsNpc(0)
        ,m_nLdId(0)
		{
		}
        
		~SoulHunterRivalInfo()
		{
		}
        
        uint32_t getAvatarId()
        {
            return m_nLdId!=0?D_FIX_DRESS_ID(m_nLdId):m_pLid;
        }
        
		CREATE_FUNC(SoulHunterRivalInfo);
		D_INIT_DEFAULT(ValueObjcet);
		CC_SYNTHESIZE_READONLY(uint32_t, m_pUid, Uid);
		CC_SYNTHESIZE_READONLY(std::string, m_pName, Name);
		CC_SYNTHESIZE_READONLY(uint32_t, m_pLv, Lv);
		CC_SYNTHESIZE_READONLY(uint32_t, m_pLid, Lid);
		CC_SYNTHESIZE_READONLY(uint8_t, m_pIsNpc, IsNpc);
        CC_SYNTHESIZE_READONLY(uint32_t, m_nLdId, LdId);
        
	public:
        
        virtual void loadData(const Json::Value & data);
        
	};
    
    
	class SoulHunterLastBattleInfo:public ValueObjcet
	{
	public:
		SoulHunterLastBattleInfo()
        :m_pLastTid(0)
        ,m_pLastWid(0)
        ,m_pLastIsNpc(0)
		{
		}
        
		~SoulHunterLastBattleInfo()
		{
		}
        
		CREATE_FUNC(SoulHunterLastBattleInfo);
		D_INIT_DEFAULT(ValueObjcet);
		CC_SYNTHESIZE_READONLY(uint32_t, m_pLastTid, LastTid);
		CC_SYNTHESIZE_READONLY(uint32_t, m_pLastWid, LastWid);
		CC_SYNTHESIZE_READONLY(uint8_t, m_pLastIsNpc, LastIsNpc);
        
		friend class::SoulHunterModel;
	};
}


class SoulHunterModel : public Model
{
	typedef cocos2d::CCArray SoulHunterRivalArray;
	typedef cocos2d::CCDictionary SoulHunterRivalDic;
public:
    
    SoulHunterModel();
    virtual ~SoulHunterModel();
    virtual bool init();
    void loadData();
    
public:
	soulhunter::SoulHunterLastBattleInfo		*lastBattleInfo;
public:
    void parseSoulHunterInfo(const Json::Value & val);
	void parseSoulHunterRivalsInfo(const Json::Value & val);
	void parseSoulHunterAwardInfo(const Json::Value &requestData,const Json::Value &responseData);
	void deductPower();														//扣除挑战次数
	void setHunterDrops(cocos2d::CCArray *m_phunterDrops);
private:
    std::set<uint32_t>          m_pcanontSnatachWarriors;					//已经抢了5个魂魄的弟子
	cocos2d::CCArray			*m_phunterDrops;
	warrior::WarriorCurInfo		*m_pTargetWarrior;
	uint32_t					lastWid;
	uint8_t						curSoulHunerType;							//狩魂进入战斗后返回的类型
	CC_SYNTHESIZE_READONLY(SoulHunterRivalArray *, m_pArrRivals, Rivals);
	CC_SYNTHESIZE_READONLY(soulhunter::SoulHunterInfo*, m_pHunterInfo,SoulHunterInfo);
public:
	uint8_t getCurSoulHunterType();
	void handleRivalNotExist();//抢夺的魂魄不存在了
	void handleWarriorNoMoreSnatch();//抢夺的魂魄已经抢了5个
	cocos2d::CCArray* getHunterDrops();
	void seTargetWarrior(warrior::WarriorCurInfo * pWarrior);
	void insertWarroirNoMoreSnatch(warrior::WarriorCurInfo * pWarrior);
	void reset();
	bool cannotSnatchWarroir(warrior::WarriorCurInfo * pWarrior);

	void doLastBattleRequest();
	void setLastBattle(uint32_t tid, uint32_t wid, uint8_t isNpc);
};



#endif /* defined(__QSMY__SoulHunterModel__) */
