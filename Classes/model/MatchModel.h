//
//  MatchModel.h
//  杯赛
//  Created by cll on 13-4-17.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MatchModel__
#define __QSMY__MatchModel__

#include "Model.h"
#include "utils/CSVDecoder.h"
#include "utils/TimeUtil.h"

#define  D_EVENT_MATCHINFO_UPDATE   "update matchInfo"
#define  D_EVENT_MATCHLIST_UPDATE   "update matchList"

#define  D_EVENT_MATCH_AWARD_STATUS_UPDATE   "update match award status"

#define  D_EVENT_MATCH_UPDATE_VSLIST      "update VSList"
#define  D_EVENT_MATCH_UPDATE_VSINFO      "update vs info"

#define  D_EVENT_MATCH_UPDATE_BETLIST     "update betList"    //正常状态下 只有下注才会增加新竞猜信息  竞猜界面只需要监听列表变化 领奖界面需要监听信息变化
#define  D_EVENT_MATCH_UPDATE_BETINFO     "update betInfo"    //正常状态下 只有下注才会增加新竞猜信息  竞猜界面只需要监听列表变化 领奖界面需要监听信息变化
#define  D_EVENT_MATCH_REPLAY			  "match replay"

class MatchModel;

namespace match {
    
    enum VS //对阵双方
    {
        kVSNone = 0,
        kVSLeft = 1,
        kVSRight = 2
    };
    
    //竞猜结果 0:未赛 1:猜中 2:未猜中
    enum BetResult
    {
        kBetResultNone = 0,
        kBetResultWin = 1,
        kBetResultFailed = 2
    };
    
    enum MatchFinalCat
    {
		k16 = 16,	//8强之前
        k8 = 8, //8强
        k4 = 4, //4强
        k2 = 2, //决赛
		k1 = 1,	//决赛之后
    };
    
	/*对手信息*/
	class OppDesp:public ValueObjcet
	{
	public:
		OppDesp():lid(0),lv(0),name(""),ldid(0){};
		CREATE_FUNC(OppDesp);
		D_INIT_DEFAULT(ValueObjcet);
        uint32_t getAvatarId()
        {
            return ldid!=0?D_FIX_DRESS_ID(ldid):lid;
        }
	public:
		uint32_t lid;					//队长编号
        uint32_t ldid;                  //队长皮肤编号
		uint16_t lv;					//等级
		std::string name;				//名字
	};
    
	/*奖品信息*/
	class AwardInfo:public ValueObjcet
	{
	public:
		AwardInfo():id(0),num(0),lv(0),name(""){};
		CREATE_FUNC(AwardInfo);
		D_INIT_DEFAULT(ValueObjcet);
        
	public:
		uint32_t id;					//编号
		uint32_t num;					//数目
		uint16_t lv;					//等级
		std::string name;				//名字
	};
    
    
	/*比赛历史信息*/
    class MatchInfo:public ValueObjcet
    {
	public:
		MatchInfo():finalNum(0),mid(1),oppDesp(NULL),status(false),win(false),watched(true){};
		~MatchInfo()
		{
			CC_SAFE_RELEASE_NULL(oppDesp);
		};
        
		CREATE_FUNC(MatchInfo);
        
        
		bool init ()
		{
            if(!ValueObjcet::init()) return false;
			oppDesp = OppDesp::create();
			CC_SAFE_RETAIN(oppDesp);
			return true;
		};
        
		void refreshWithFinalCat(MatchFinalCat finalCat);
		//D_INIT_DEFAULT(ValueObjcet);
        
	public:
		uint32_t finalNum;				//几强赛
		uint32_t mid;					//比赛id
		OppDesp *oppDesp;				//对手信息
		
		bool status;					//是否已领取奖励 0：未领取 1:已领取
		bool win;						//是否胜利 1:胜利 0:失败
		bool watched;
    };
    
	/*阵容信息*/
	class FormationInfo:public ValueObjcet
	{
	public:
		FormationInfo():lv(0),pid(0),wid(0){};
		CREATE_FUNC(FormationInfo);
		D_INIT_DEFAULT(ValueObjcet);
        
	public:
		uint16_t lv;					//单位等级
		uint8_t pid;					//位置
		uint32_t wid;					//单位编号
	};
    
	/*竞猜信息*/
	class BetInfo:public ValueObjcet
	{
	public:
		BetInfo():id(0),finalNum(0),mid(1),betUser(1),result(1),status(false){};
		CREATE_FUNC(BetInfo);
		D_INIT_DEFAULT(ValueObjcet);
        
	public:
		uint32_t id;					//竞猜编号
		uint32_t finalNum;				//几强赛
		uint32_t mid;					//比赛id
		uint8_t betUser;				//竞猜胜利方 0未知 1一号玩家 2二号玩家
		uint8_t result;					//竞猜结果 0:未赛 1:猜中 2:未猜中
		bool status;					//状态 0:未领取奖励 1:已领取奖励
	};
    
    
    
    
    /*对阵单位信息*/
    class VSPlayer:public ValueObjcet
    {
    public:
        typedef cocos2d::CCArray FormationVec;
    public:
        VSPlayer()
        :win(false)
        ,lose(false)
        ,m_pDesp(NULL)
        ,m_pDetail(NULL)
        ,rate(0)
		,visible(false)
        {
            
        }
        virtual ~VSPlayer()
        {
            CC_SAFE_RELEASE(m_pDesp);
            CC_SAFE_RELEASE(m_pDetail);
        }
        
        CREATE_FUNC(VSPlayer);
        
        bool init()
        {
            if(!ValueObjcet::init()) return false;
			m_pDesp = OppDesp::create();
			CC_SAFE_RETAIN(m_pDesp);
			m_pDetail = FormationVec::create();
			CC_SAFE_RETAIN(m_pDetail);
            return true;
        }
    public:
        bool                win;   //是否胜 //有可能还没开始
        bool                lose;  //是否负   //有可能还没开始
        OppDesp             *m_pDesp;
        FormationVec        *m_pDetail;
        uint32_t            rate;
		bool				visible;
    };
    
	/*对阵信息*/
	class VSInfo:public ValueObjcet
	{
        
	public:
		VSInfo()
        :finalNum(0)
        ,mid(1)
        ,winUser(0)
        ,m_pLeft(NULL)
        ,m_pRight(NULL)
		,m_isWatched(false)
		,m_bSwitched(false)
        {};
		
		virtual ~VSInfo()
		{
			CC_SAFE_RELEASE(m_pLeft);
			CC_SAFE_RELEASE(m_pRight);
		};
        
		CREATE_FUNC(VSInfo);
        
		bool init ()
		{
            if(!ValueObjcet::init()) return false;
			m_pLeft = VSPlayer::create();
			CC_SAFE_RETAIN(m_pLeft);
			m_pRight = VSPlayer::create();
			CC_SAFE_RETAIN(m_pRight);
			return true;
		};
        
		bool isWatched()
		{
			return m_isWatched;
		}

		void setWatched(bool watched);
		void refreshWithFinalCat(MatchFinalCat finalCat);
		CC_SYNTHESIZE(bool, m_bSwitched, IsSwitched);
	public:
		uint32_t finalNum;				//几强赛
		uint32_t mid;					//比赛id
		uint8_t winUser;				//胜利方 0未知 1一号玩家 2二号玩家
		VSPlayer *m_pLeft;              //对阵玩家一
        VSPlayer *m_pRight;             //对阵玩家二
	protected:
		friend class ::MatchModel;
		bool m_isWatched;				//是否观看过
	};
    
    class MatchConfig : public ValueObjcet
    {
    public:
        MatchConfig()
        :m_nBetCost8(0)
        //,m_nTotalBetTimes(0)
        ,m_nBetCost4(0)
        ,m_nBetCost2(0)
        ,m_nMatch8StartTime(0)
        ,m_nMatch8EndTime(0)
        ,m_nMatch4EndTime(0)
        ,m_nMatch2EndTime(0)
        ,m_nSignStartTime(0)
		,m_pMatchTime(NULL)
        {
        }
        virtual ~MatchConfig()
        {
            CC_SAFE_RELEASE(m_pMatchTime);
        }
        
        CREATE_FUNC(MatchConfig);
        D_INIT_DEFAULT(ValueObjcet);
        
        //uint8_t m_nTotalBetTimes;   //每日总竞猜机会
        uint8_t m_nBetCost8;        //8强下注额度
        uint8_t m_nBetCost4;        //4强下注额度
        uint8_t m_nBetCost2;        //决赛下注额度
        uint8_t m_nMatch8StartTime; //8强开始时间（数据请求时间）
        uint8_t m_nMatch8EndTime;   //8强结束时间
        uint8_t m_nMatch4EndTime;   //4强结束时间
        uint8_t m_nMatch2EndTime;   //决赛结束时间
		uint8_t m_nSignStartTime;   //报名开始时间
		uint8_t m_nMatch8ShowTime;	//8强显示时间
		cocos2d::CCDictionary *m_pMatchTime;
		uint32_t roundLargerThanK8;
        
        uint8_t getBetCost(MatchFinalCat val);

        std::string getMatchEndTime(MatchFinalCat val);
        
        /*获取正在进行的比赛*/
		MatchFinalCat getMatching(bool withLiveTime = false);
		void parseMatchTime(const CSVDecoder::CSV&);

		uint32_t getStartTimeOfMatch(uint8_t round);
		uint32_t getEndTimeOfMatch(uint8_t round);

	};
	class MatchTime : public ValueObjcet
	{
	public:

		CREATE_FUNC(MatchTime);
		D_INIT_DEFAULT(ValueObjcet);
		uint32_t round;
		uint32_t hour;
		uint32_t minute;
	};
}


class MatchModel : public Model
{
	
public:
	typedef cocos2d::CCArray MatchInfoVec;
    typedef cocos2d::CCArray VSInfoVec;
    typedef cocos2d::CCArray BetInfoVec;
	//typedef cocos2d::CCDictionary BetInfoMap;
	typedef cocos2d::CCArray AwardInfoVec;
    
	CC_SYNTHESIZE_READONLY(bool, m_isSigned,IsSigned);		//是否已参加明日比赛
    
public:
    
	MatchModel();
    virtual ~MatchModel();
    virtual bool init();

	void inited();
    
    /* 报名成功 */
    void signedUp();
    
	/* 是否报名 */
	void parseSignUp(const Json::Value &val);
    
	/* 获取比赛信息列表 */
	void parseMatchs(const Json::Value &val ,uint32_t lastUpdateTime);
	
	/* 获取对阵列表 */
	void parseVSMatchs(const Json::Value &val);
    
	/* 获取竞猜列表 */
	void parseBets(const Json::Value &val);
    
	/* 获取竞猜信息 */
	void parseBet(const Json::Value &val);
    
	/* 获取比赛对阵列表 */
	void getVSListByFinalNum(uint32_t finalNum, VSInfoVec *vec);
    
	/* 获取竞猜信息 */
	match::BetInfo* getBetInfo(uint32_t finalNum,uint32_t mid);
    
	/* 更新竞猜信息 */
	void updateBetInfo(uint32_t finalNum,uint32_t mid);
    
	/* 获取比赛对阵列表 */
	match::VSInfo* getVSInfo(uint32_t finalNum,uint32_t mid);
    
	void getMatchInfoVec(MatchInfoVec *arr);
    
    uint32_t getMatchInfoCount();
    
    /**
     *	@brief	获取更新时间检查
     *
     *	@return	0立即刷新 正值倒计时刷新 负值无动作
     */
	int checkTime();
	int refreshTime();

    
    uint8_t getRemainBetTimes()
    {
        //return m_pMatchConfig->m_nTotalBetTimes - m_betInfoVec->count();
        return maxBet - m_betInfoVec->count();
    }
    
    void mReward(uint32_t finalNum,uint32_t mid);
    
    match::MatchInfo* getMatchInfo(uint32_t finalNum,uint32_t mid);
    
    match::MatchConfig* getMatchConfig() { return m_pMatchConfig; }
    
    
    void updateMatchRate(uint32_t finalNum,uint32_t mid,uint32_t rate1,uint32_t rate2);
    
    void reset();

	void watchReplay(match::VSInfo *pVsInfo);
	void watchReplay(match::MatchInfo *pMatchInfo);
	void watchReplay(uint32_t finalNum,uint32_t mid);

	match::MatchFinalCat getMatching();
	void refreshWatchedMatch();

    
private:
	match::MatchInfo* parseMatch(const Json::Value &val, match::MatchInfo* pMatchInfo = NULL);	//获取个人比赛信息
	match::OppDesp* parseOppDesp(const Json::Value &val, match::OppDesp* pOppDesp = NULL);		//获取对手信息
    void parseFormation(const Json::Value &val, match::VSPlayer::FormationVec *pFormationVec);	//获取阵型信息
    
	match::BetInfo* parseBetResult(const Json::Value &val, match::BetInfo* pBetInfo = NULL);	//获取竞猜结果
	match::VSInfo* parseVSMatch(const Json::Value &val, match::VSInfo* pVSInfo = NULL);			//获取淘汰赛对阵信息
    
    void parseMatchConfig(const CSVDecoder::CSV&);
    
    void sortPositions(cocos2d::CCArray* pPositions);
    
    void sortVS(VSInfoVec *vec);

	void readSavedData();
private:
    
    MatchInfoVec *m_matchInfoVec;		//个人比赛历史信息
	VSInfoVec *m_VSInfoVec;				//淘汰赛对阵信息
	BetInfoVec *m_betInfoVec;			//竞猜信息
	AwardInfoVec *m_awardInfoVec;		//奖励信息  暂时没有
    match::MatchConfig *m_pMatchConfig; //杯赛配置信息
    
private:
    
    uint32_t m_nlastUpdateTime;
    uint32_t maxBet;                    //竞猜次数上限
	int m_iToSave;						//存储已观看的8/4/2强赛
	int m_iMatchFinal;					//存储已观看的个人比赛
};





#endif /* defined(__QSMY__MatchModel__) */
