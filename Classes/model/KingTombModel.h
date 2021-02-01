//
//  KingTombModel.h
//  LibQSMY
//
//  Created by thedream on 14-8-1.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __LibQSMY__KingTombModel__
#define __LibQSMY__KingTombModel__

#include "Model.h"
#include "../AppMacros.h"
#include "GuildModel.h"

#define FIGHT_PLAY_MAX_NUM  3  //参加战斗的最多的人数
#define D_EVENT_KINGTOMBLIST_GET "kingtomblist_get"
#define D_EVENT_KINGTOMB_GET_ASSIST_BONUS_LIST "kingtomb get assist bonus list"
#define D_EVENT_KINGTOMB_GET_LOG "kingtomb get log"
#define D_EVENT_KINGTOMB_GET_RANK "kingtomb get rank"

//load from gameconfig
#define	TOMBS_NUM	"TOMBS_NUM"
#define	TOMBS_TIMELIMIT		"TOMBS_TIMELIMIT"
#define	TOMBS_FIND_YUAN	"TOMBS_FIND_YUAN"
#define	TOMBS_MAX_FIND_YUAN		"TOMBS_MAX_FIND_YUAN"
#define	TOMBS_SPEED_YUAN	"TOMBS_SPEED_YUAN"
#define	TOMBS_FREE_FIND_NUM	"TOMBS_FREE_FIND_NUM"
#define	KINGTOMB_OUT_PUT "KINGTOMB_OUT_PUT"
#define	KINGTOMB_HISTORY_UPDATE_TIME	"KINGTOMB_HISTORY_UPDATE_TIME"
#define	KINGTOMB_LIST_UPDATE_TIME	"KINGTOMB_LIST_UPDATE_TIME"
//魔界探险排行榜刷新时间
#define	KINGTOMB_RANK_REFRESH_TIME	"KINGTOMB_RANK_REFRESH_TIME"
#define KINGTOMB_RANK_REFRESH_INTERVAL	(D_CONFIG_UINT_FOR_KEY(KINGTOMB_RANK_REFRESH_TIME))
enum KingTombState
{
	kKingTombStateNone , //还没有人占领
	kKingTombStateIsRob ,
	kKingTombStateIsComplete ,
	kKingTombStateIsWork,
    kKingTombStateIsReady
};



class KingTombModel;
namespace kingTomb
{
	/**------------------------------ 排行榜数据----------------*/
	class kingTombRankInfo:public ValueObjcet{
		kingTombRankInfo();
		~kingTombRankInfo();
		CREATE_FUNC(kingTombRankInfo);
		D_INIT_DEFAULT(ValueObjcet);
		CC_SYNTHESIZE_PASS_BY_REF(std::string, m_name, Name);
		CC_SYNTHESIZE(uint32_t, m_level, Level);
		CC_SYNTHESIZE(uint32_t, m_fight, Fight);
		CC_SYNTHESIZE(uint32_t, m_count, Count);
		void parse(const Json::Value& val);
		friend class ::KingTombModel;
	};

    /**------------------------------ 额外奖励数据----------------*/
    class kingTombItemInfo:public ValueObjcet{
        kingTombItemInfo();
        CREATE_FUNC(kingTombItemInfo);
        ~kingTombItemInfo();
        virtual bool init();
        CC_SYNTHESIZE(uint32_t, m_id, ID);
        CC_SYNTHESIZE(uint32_t, m_Itemid, Itemid);
        CC_SYNTHESIZE(uint32_t, m_Itemtype, Itemtype);
        CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *,m_itemNumByLv,ItemNumByLv);
        
        friend class ::KingTombModel;
    };
    
    /**------------------------------ vip 开放对应矿数据----------------*/
    class kingTombVipInfo:public ValueObjcet{
        kingTombVipInfo();
        CREATE_FUNC(kingTombVipInfo);
        ~kingTombVipInfo();
        bool init();
        CC_SYNTHESIZE(uint32_t, m_vip, Vip);
        CC_SYNTHESIZE(uint32_t, m_tombId, TombId);
        friend class ::KingTombModel;
    };
    
    /**------------------------------ 矿类型数据----------------*/
    class kingTombTypeInfo:public ValueObjcet{
        kingTombTypeInfo();
        CREATE_FUNC(kingTombTypeInfo);
        ~kingTombTypeInfo();
        bool init();
        CC_SYNTHESIZE(uint32_t, m_type, Type);
        CC_SYNTHESIZE(std::string, m_name, Name);
        CC_SYNTHESIZE(uint32_t, m_fightNum, FightNum);
        friend class ::KingTombModel;
    };
    
    
    class kingTombBaseInfo:public ValueObjcet{
        CC_SYNTHESIZE(uint32_t, m_type, Type);
        CC_SYNTHESIZE(double, m_coinOutput, CoinOutput);//生产速度
        CC_SYNTHESIZE(uint32_t, m_occupyTs, OccupyTs);
        CC_SYNTHESIZE(uint32_t, m_extraOutputType, ExtraOutputType);//额外获得的物品类型
        CC_SYNTHESIZE(uint32_t, m_speedUpTs, SpeedUpTs);
        CC_SYNTHESIZE(uint32_t, m_endTs, EndTs);
        
    public:
        kingTombBaseInfo();
        ~kingTombBaseInfo();
        void parse(const Json::Value& val);
        CREATE_FUNC(kingTombBaseInfo);
        D_INIT_DEFAULT(ValueObjcet);
        
    public:
        uint32_t getSpeedNum();
        uint32_t getOutputNum();
        uint32_t getOutputItemNum();
        friend class ::KingTombModel;
    };
    
    
    /**------------------------------ 后端返回矿数据----------------*/
    class kingTombInfo:public ValueObjcet
    {
        kingTombInfo();
        ~kingTombInfo();
        void parse(const Json::Value& val);
        CREATE_FUNC(kingTombInfo);
        D_INIT_DEFAULT(ValueObjcet);
        CC_SYNTHESIZE(uint32_t, m_id, Id);
        
        CC_SYNTHESIZE(uint32_t ,m_searchTs, SearchTs);  //搜索时间
        CC_SYNTHESIZE(uint32_t, m_endTs, EndTs);
        CC_SYNTHESIZE(uint32_t, m_occupyId, OccupyId);//额外获得的物品类型
        CC_SYNTHESIZE(bool, m_isRob, isRob);  //是否被抢夺了
        CC_SYNTHESIZE_READONLY(cocos2d::CCArray *,m_defendUserData,defendUserData);
        CC_SYNTHESIZE_READONLY(cocos2d::CCArray *,m_attackUserData,attackUserData);
        CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_bonus, Bonus);			// 被强了的掉落信息
        
        
        CC_SYNTHESIZE(bool, m_isFight, IsFight);//是否正在战斗中
        
        
    private:
        kingTombBaseInfo *m_pMyInfo;
        kingTombBaseInfo *m_pOtherInfo;
        
        
    public:
        uint32_t getState();
        
        kingTombBaseInfo * getMyInfo();
        kingTombBaseInfo * getOtherInfo();
        
        
        bool theKingTombIsUser();
        bool theKingTombIsWork();
		//是否是极品箱子
		bool theKingTombIsNewType()
		{
			return getMyInfo()->getType() > 5;
		};
        
        bool isTimeLimit();
        uint32_t getSpeedUpYuan();
        friend class ::KingTombModel;
    };
    
    
    /**------------------------------ 矿战斗人物数据----------------*/
    class userData:public ValueObjcet
    {
    public:
        userData();
        ~userData();
        CREATE_FUNC(userData);
        void parse(const Json::Value& val);
        D_INIT_DEFAULT(ValueObjcet);
        CC_SYNTHESIZE(std::string, m_name, Name);
        CC_SYNTHESIZE(uint32_t, m_sid, Sid);
        CC_SYNTHESIZE(uint32_t, m_lv, Lv);
        CC_SYNTHESIZE(bool, m_die, Die);  //是否死亡了
        CC_SYNTHESIZE(uint32_t, m_id, ID);
        
        friend class ::KingTombModel;
    };
    
    
    /**------------------------------ 已经助战过的玩家数据----------------*/
	class AssistBonusInfo : public ValueObjcet
	{
	public:
		AssistBonusInfo();
		~AssistBonusInfo();
		CREATE_FUNC(AssistBonusInfo);
        
		void parse(const Json::Value& val);
        
		CC_SYNTHESIZE(unsigned int, assistLv, AssistLv);					// 助战对象级别
		CC_SYNTHESIZE(std::string, assistName, AssistName);					// 助战对象名字
		CC_SYNTHESIZE(unsigned int, treasureType, TreasureType);			// 宝库名字
		CC_SYNTHESIZE(unsigned int, addTs, AddTs);
		CC_SYNTHESIZE(unsigned int, id, Id);
		CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, bonusData, BonusData);	// 助战奖励信息
        
		friend class ::KingTombModel;
	};
    
	class LogInfo : public ValueObjcet
	{
	public:
		LogInfo();
		~LogInfo();
		CREATE_FUNC(LogInfo);
        
		void parse(const Json::Value& val);
        
		CC_SYNTHESIZE(unsigned int, treasureType, TreasureType);			// 宝库名字
		CC_SYNTHESIZE(std::string, name, Name);								// 强盗名字
		CC_SYNTHESIZE(unsigned int, lv, Lv);								// 强盗级别
		CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, bonus, Bonus);			// 助战奖励信息
		CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, lose, Lose);				// 助战奖励信息
		CC_SYNTHESIZE(unsigned int, addTs, AddTs);
        
		friend class ::KingTombModel;
	};
	
    
}


class KingTombModel : public Model
{
	CC_SYNTHESIZE(uint32_t, m_pSearchTimes, SearchTimes);
    CC_SYNTHESIZE(uint32_t, m_RewordPos, RewordPos);
	CC_SYNTHESIZE(bool, m_bHaveNewAssistBonus, HaveNewAssistBonus);
	CC_SYNTHESIZE(bool, m_bHaveNewLog, HaveNewLog);
    
    CC_SYNTHESIZE(bool, m_dragonWin, DragonWin);
    CC_SYNTHESIZE(bool, m_dragonEnd, DragonEnd);
    
    CC_SYNTHESIZE(bool, m_pStopAin, StopAin);
	//排行榜相关
	CC_SYNTHESIZE(cocos2d::CCArray*, m_pGuildRankList, GuildRankList);
	CC_SYNTHESIZE(cocos2d::CCArray*, m_pUserRankList, UserRankList);
    
public:
    
    KingTombModel();
    virtual ~KingTombModel();
    virtual bool init();
    
public:
    void getModeData();
    
    void search(unsigned int pos);
    void speedUp(unsigned int pos);
    void getReword(unsigned int pos);
    
    
    
    void battleStartDragonValley(unsigned int pos,Json::Value& assist);
    
    
    void parseOccupyList(const Json::Value& val);
    
    kingTomb::kingTombInfo* getKingTombInfoByPos(unsigned int pos);
    void removeKingTombInfoByPos(unsigned int pos ,bool backUser);
    void changeKingTombInfo(const Json::Value& val,bool delay = false);
    
    kingTomb::kingTombVipInfo * getTombVipInfo(uint32_t pos);
    kingTomb::kingTombTypeInfo * getTombTypeInfo(uint32_t type);
    kingTomb::kingTombItemInfo * getTombItemInfo(uint32_t m_id);
    
    
    
    bool isOpenTombByPos(int pos);
    
    void parseKingTombVipInfo(const CSVDecoder::CSV &val);
    void parseKingTombTypeInfo(const CSVDecoder::CSV &val);
    void parseKingTombItemInfo(const CSVDecoder::CSV &val);
    
    void parseAssistUserIds(const Json::Value& val);
    
    
    void addAssistUserId();
    void removeAllAssistUserId();
    
    
    void addSeleteUse(guild::GuildUserBasics *pdata,uint32_t pos);
    void removeSeleteUse();
    
    guild::GuildUserBasics* getSeleteUseByPos(uint32_t pos);
    
	// 同步助战奖励列表
	void syncAssistBonusList();
	// 同步历史记录列表
	void syncLog();
    
	// 解析数据
	void parseAssistBonusList(const Json::Value& val);
	void parseLog(const Json::Value& val);
	void removeAssistBonusById(unsigned int id);
	void removeAllAssistBonus();
	//相应排行榜列表（帮内帮外）
	void parseRank(const Json::Value& val , bool isUserRank);
	//相应极品信息
	void parseMeetRare(const Json::Value& val);
	//更新排行榜
	void syncRankList(bool isUserRank);
    
	void getAssistBonusList(cocos2d::CCArray* pArr) const;
	void getLogList(cocos2d::CCArray* pArr) const;
    
    unsigned int getFreeFindNum();
    unsigned int getFindYuan();
    bool isAssistUser(uint32_t uid);
    bool isSeleteUser(uint32_t uid);
    
    void removemAssistUser(kingTomb::kingTombInfo *pInfo);
    
private:
    cocos2d::CCArray *m_pOccupyList;
    cocos2d::CCDictionary* m_pVipInfos;
    cocos2d::CCDictionary* m_pTypeInfos;
    cocos2d::CCDictionary* m_pKingTombItems;
    
    
    
    //已经助战过的人的列表
    cocos2d::CCDictionary* m_pAllAssistUserIds;
    cocos2d::CCDictionary* m_pSeleteUser;
	cocos2d::CCArray* m_pAssistBonusList;
	cocos2d::CCArray* m_pLogList;

	//排行榜相关
	unsigned int m_uUserRankTs;
	unsigned int m_uGuildRankTs;
	//今日极品宝库
	std::string m_uTodayUserName;
	std::string m_uTodayGuildName;
	unsigned int m_uTodayType;
	//今日的是否已经被挖到
	bool m_bTodayFound;
    
	unsigned int m_nLastAssistBonusListTime;
    unsigned int m_nLastKingTomeListTime;
    
	unsigned int m_nLastLogTime;
};
#endif /* defined(__LibQSMY__KingTombModel__) */
