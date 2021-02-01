//
//  TowerModel.h
//  千机楼
//
//  Created by ldr123 on 13-07-01.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TowerModel__
#define __QSMY__TowerModel__

#include "Model.h"
#include "ItemModel.h"

#define D_EVENT_TOWER_INFO_UPDATE		"update tower info"
#define D_EVENT_TOWER_ADD_UPDATE		"update tower add"
#define D_EVENT_TOWER_RANK_UPDATE		"update tower rank"
#define D_EVENT_TOWER_OLD_RANK_UPDATE	"update tower old rank"

class TowerModel;
namespace tower
{
	enum NavigationType
	{
		KNavUnknow = -1,
		KNavHome,
		kNavDifficulty,
		kNavChooseBuff,
		kNavContinue,
		kNavRank,
		kNavReward,
		kNavFailed,
		kNavRecharge,
		kNavInteral,
		kNavMax
	}; 

	enum StatusType
	{
		StatusTypeStart = 0,
		StatusTypeFight = 1,
		StatusTypeFailed = 2,
		StatusTypeContinue = 3,
	};

	class TowerRankInfo :public ValueObjcet
	{
	public:
		TowerRankInfo():m_Pos(0), m_Lev(0), m_strName(""), m_lId(0), m_todayNum(0), m_totalNum(0),m_ldid(0){}
	
		CREATE_FUNC(TowerRankInfo);
		D_INIT_DEFAULT(ValueObjcet);
        
        uint32_t getAvatarId() const
        {
            return m_ldid!=0?D_FIX_DRESS_ID(m_ldid):m_lId;
        }
        
	public:
		uint32_t	m_id;
		uint16_t	m_Pos;
		uint16_t	m_Lev;
		std::string m_strName;
		uint32_t	m_lId;
        uint32_t    m_ldid;
		uint16_t	m_todayNum;
		uint16_t	m_totalNum;
	};

	class TowerBuff : public ValueObjcet
	{
	public:
		TowerBuff() : id(0), bid(0), value(0), cost(0){}
	public:
		CREATE_FUNC(TowerBuff);
		D_INIT_DEFAULT(ValueObjcet);
	public:
		uint16_t id;	//buff对应的唯一id
		uint16_t bid;	//1血，2防，3攻，4速，5暴击，6格挡，7躲闪
		uint16_t value;	//效果值
		uint16_t cost;	//该buff消耗的能量值
	};

	class TowerInfo : public ValueObjcet
	{
	public:
		TowerInfo():mid(0),mnum(0),oldnum(0),scores(0), rnum(0), rstatus(0), item_lv(0),arrBuff(NULL), pItemInfo(NULL),bNeedShowBuff(false)
		,points(0)
		{}
		~TowerInfo()
		{
			CC_SAFE_RELEASE(arrBuff);
			CC_SAFE_RELEASE(pItemInfo);
		}
	public:
		CREATE_FUNC(TowerInfo);
		bool init()
		{
            if(!ValueObjcet::init()) return false;
			arrBuff = cocos2d::CCArray::create();
			CC_SAFE_RETAIN(arrBuff);

			pItemInfo = item::ItemInfo::create();
			CC_SAFE_RETAIN(pItemInfo);

			return true;
		}
	public:
		uint16_t mid;		//层id
		uint16_t mnum;		//当日通过的最高楼层数
		uint16_t oldnum;	//昨日通过的最高楼层数
		uint16_t scores;	//积累的能量值
		uint16_t rnum;		//轮数
		uint16_t rstatus;	//本轮状态
		uint16_t item_lv;	//道具掉落在哪个地方
		uint16_t lastHard;  //困难难度三星最高关卡
		uint32_t points;

		item::ItemInfo*	pItemInfo;

		cocos2d::CCArray *arrBuff;

		uint16_t _continue;
		uint32_t lid;
		uint32_t id;
		bool	bNeedShowBuff;
        
        /**
         *	@brief	检查是否有轮数
         *
         *	@return	是否有轮数
         */
        bool hasRound();

        uint32_t todayRound();

	};

	class TowerAdd : public ValueObjcet
	{
	public:
		TowerAdd():fHp(0.f), fAtt(0.f), fDef(0.f), fSpeed(0.f), fDodge(0.f), fCrit(0.f), fBlock(0.f){}
	public:
		CREATE_FUNC(TowerAdd);
		D_INIT_DEFAULT(ValueObjcet);
	public: 
		double fHp;
		double fAtt;	//攻击
		double fDef;	//防御
		double fSpeed;	//速度
		double fDodge;	//闪避
		double fCrit;	//暴击
		double fBlock;	//格挡
	};

	class TowerFightResult : public ValueObjcet
	{
	public:
		typedef cocos2d::CCArray	TowerBuffs;
	public:
		TowerFightResult():bWin(false)
		{
			arrBuffs = TowerBuffs::create();
			CC_SAFE_RETAIN(arrBuffs);
		}

		~TowerFightResult()
		{
			CC_SAFE_RELEASE(arrBuffs);
		}
	public:
		CREATE_FUNC(TowerFightResult);
		D_INIT_DEFAULT(ValueObjcet);
	public:
		bool	bWin;
		TowerBuffs	*arrBuffs;
	};

	class TowerStatus : public ValueObjcet
	{
	public:
		TowerStatus():navType(tower::KNavUnknow), bInBattle(false)
		{
			pFightResult = TowerFightResult::create();
			CC_SAFE_RETAIN(pFightResult);
		}

		~TowerStatus()
		{
			CC_SAFE_RELEASE(pFightResult);
		}
	public:
		CREATE_FUNC(TowerStatus);
		D_INIT_DEFAULT(ValueObjcet);
	public: 
		tower::NavigationType	navType;
		bool					bInBattle;
		TowerFightResult*		pFightResult;
	};

	class TowerContinueData : public ValueObjcet
	{
	public:
		TowerContinueData():uContinueCount(0), uVipLev(0), uNeedCoins(0), uNeedPoint(0), uGetBuff(0){}
	public:
		CREATE_FUNC(TowerContinueData);
		D_INIT_DEFAULT(ValueObjcet);
	public: 
		uint16_t uContinueCount;
		uint16_t uVipLev;
		uint16_t uNeedCoins;
		uint16_t uNeedPoint;
		uint16_t uGetBuff;
	};

	class TowerDropData : public ValueObjcet
	{
	public:
		TowerDropData():uType(0), uId(0), uNum(0){}
	public:
		CREATE_FUNC(TowerDropData);
		D_INIT_DEFAULT(ValueObjcet);
	public: 
		uint16_t uType;	//1:道具 2:礼包 3:装备 4:魂魄 5:弟子 6：金币 :7：礼券 8：商品
		uint32_t uId;	//道具或者礼包等的id
		uint32_t uNum;	//道具或者礼包等数量，如果type是金币，则代表金币数量
	};

	class TowerExchangeInfo : public ValueObjcet
	{
		CC_SYNTHESIZE_READONLY(unsigned int, m_uID, ID);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uNeedPoint, NeedPoint);
		CC_SYNTHESIZE_READONLY(drop::DropType, m_eType, Type);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uItemID, ItemID);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uNum, Num);
	public:
		TowerExchangeInfo()
			: m_uID(0)
			, m_uNeedPoint(0)
			, m_eType(drop::kDropTypeNone)
			, m_uItemID(0)
			, m_uNum(0)
		{

		}
		~TowerExchangeInfo();
		CREATE_FUNC(TowerExchangeInfo);
		D_INIT_DEFAULT(ValueObjcet);
	protected:
		friend class ::TowerModel;
	};
};

class TowerModel : public Model
{
    CC_SYNTHESIZE_READONLY(uint32_t, m_nMyRank, MyRank);            //自己的排行榜
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pRanks, Ranks);     //排行榜信息
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pOldRanks, OldRanks);    //昨日排行榜信息
    
public:
	typedef cocos2d::CCArray TowerContinueArr;
	typedef cocos2d::CCArray TowerDropData;
    
public:
	TowerModel();
	virtual ~TowerModel();
	virtual bool init();
public:
    
    /**
     *	@brief	更新千层塔信息
     *
     *	@param 	notify 	是否通知变化
     */
	void tower_get(const Json::Value &val ,bool notify = true);
	void tower_add(const Json::Value &val);
    
    uint32_t getNeedCoins(tower::TowerContinueData * pContinueData);
	void parseConinue(const CSVDecoder::CSV &val);
	void loadExchangeInfo(const CSVDecoder::CSV &val);
    

    /**
     *	@brief	缓存战斗信息
     *
     */
    void cacheBattleInfo();
    
    /**
     *	@brief	重播战斗
     *
     */
    void replayBattle();
    
    /**
     *	@brief	是否可以重播战斗
     *
     */
    bool isCanReplayBattle();
    
    
    /**
     *	@brief	更新千机楼排行榜
     *
     *	@param 	force 	是否强制刷新
     */
    void updateTowerRank(bool force = false);

	/**
     *	@brief	更新千机楼昨日排行榜
     *
     */
    void updateTowerOldRank();


    /**
     *	@brief	更新千机楼排行榜回调
     *
     *	@param 	ranks 	排行榜信息
     *	@param 	myRank 	自己的排名
     *	@param 	lastUpdateTime 	最后更新时间
     */
    void updateTowerRanks(const Json::Value& ranks,uint32_t myRank,time_t lastUpdateTime);

	/**
     *	@brief	更新千机楼昨日排行榜回调
     *
     *	@param 	ranks 	排行榜信息
     */
    void updateTowerOldRanks(const Json::Value& ranks);

    virtual void reset();

	virtual void dailyReset(float dt);

public:
	tower::TowerInfo*		GetTowerInfo()	{return m_curTowerInfo;}
	tower::TowerAdd*		GetTowerAdd()	{return m_curTowerAdd;}
	tower::NavigationType	GetCurrentType();
	void					SetCurrentType(tower::NavigationType);
	bool					IsInBattle();
	void					SetInBattle(bool);
	bool					IsWin();
	void					SetFightResult(const Json::Value& val);
	tower::TowerStatus*		GetTowerStatus(){return m_curTowerStatus;}
	TowerContinueArr*		GetTowerContinueArr(){return m_arrTowerContinue;}
	void					SetNeedChooseBuf(bool);
	bool					IsNeedChooseBuf();
	void					AddTowerScores(int nScore);
	void					ClearTowerBuff();

	void getExchangeArray(cocos2d::CCArray *pArr);
private:
	tower::TowerInfo*	m_curTowerInfo;
	tower::TowerAdd	*	m_curTowerAdd;
	tower::TowerStatus*	m_curTowerStatus;
	TowerContinueArr*	m_arrTowerContinue;
    std::vector<char>   m_battleInfo;
    
    time_t              m_nRankUpdateTime;  //排行榜更新时间    

	cocos2d::CCArray*   m_pArrExchange;
    bool m_bUpdateOldRank;
};

#endif /* defined(__QSMY__TowerModel__) */

