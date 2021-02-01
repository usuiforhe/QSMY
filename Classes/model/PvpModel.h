//
//  PvpModel.h
//  论剑
//  Created by cll on 13-5-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PvpModel__
#define __QSMY__PvpModel__

#include "Model.h"
#include "DropModel.h"

#define  D_EVENT_PVPINFO_UPDATE		"update pvpInfo"
#define  D_EVENT_PVPLIST_UPDATE		"update targetlist"
#define  D_EVENT_PVP_UPDATE_TOPLIST		"update toplist"
#define  D_EVENT_PVP_UPDATE_AWARDLIST	"update awardlist"
#define  D_EVENT_PVP_INIT	"update init"
#define	D_EVENT_PVP_EXMALL_UPDATE	"update exmall"

#define  K_PVP_TARGET_RANK_MAX		D_CONFIG_UINT_FOR_KEY(PVP_TARGET_RANK_MAX)		//排名上限

namespace otherFormation
{
    class RobotFormationInfo;
}

class PvpModel;

namespace pvp
{
	class ArenaInfo:public ValueObjcet
	{
	public:
		ArenaInfo():m_rankFirst(0),m_rankLast(0),m_scoreLow(0),m_scoreStep(0){};

		CREATE_FUNC(ArenaInfo);
		D_INIT_DEFAULT(ValueObjcet);

	protected:
		CC_SYNTHESIZE(uint32_t, m_rankFirst, RankFirst);   //rank区间
		CC_SYNTHESIZE(uint32_t, m_rankLast, RankLast);   //
		CC_SYNTHESIZE(uint32_t, m_scoreLow, ScoreLow);   //rank区间内 score最小值
		CC_SYNTHESIZE(uint32_t, m_scoreStep, ScoreStep);  //delta
	protected:
		friend class ::PvpModel;
	};

	//论剑兑换商城等级升级信息
	class MallLvInfo:public ValueObjcet
	{
	public:
		MallLvInfo():m_MallLvl(0),m_NeedPoint(0){};
		CREATE_FUNC(MallLvInfo);
		D_INIT_DEFAULT(ValueObjcet);

	protected:
		CC_SYNTHESIZE(uint32_t, m_MallLvl, MallLvl);
		CC_SYNTHESIZE(int, m_NeedPoint, NeedPoint);
	protected:
		friend class ::PvpModel;
	};


#pragma mark ----------论剑奖励信息-----------
	//论剑奖励信息
	class AwardInfo:public ValueObjcet
	{
	public:
		enum AwardType
		{
			kAwardTypeExchange = 1,     //兑换奖励
			kAwardTypeRank = 2,			//突破排名奖励
		};
        
	public:
		AwardInfo():m_nId(0),m_eType(kAwardTypeExchange),m_nThreshold(0),m_sDesc(""),m_nItemId(0),m_nItemNum(0){};
        
		CREATE_FUNC(AwardInfo);
		D_INIT_DEFAULT(ValueObjcet);
        
        void loadLine(CSVDecoder::CSVLine line);
        
	protected:
        CC_SYNTHESIZE(uint16_t, m_nId, Id);                 //奖励编号
        CC_SYNTHESIZE(AwardType, m_eType, Type);            //类型
        CC_SYNTHESIZE(uint32_t, m_nThreshold, Threshold);   //领取门槛
        CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sDesc, Desc);          //描述
        CC_SYNTHESIZE(uint32_t, m_nItemId, ItemId);         //奖励道具
        CC_SYNTHESIZE(uint32_t, m_nItemNum, ItemNum);       //奖励道具数        
	};

#pragma mark ------------排名类--------------
    class ChallengeRank
    {
    public:
		ChallengeRank():m_nRank(0)
			,m_nWin(0), m_nLose(0)
		{
        }
        virtual ~ChallengeRank(){
            
        }
        
        /**
         *	@brief	加载数据
         *
         *	@param 	data 	数据
         */
        virtual void loadData(const Json::Value & data);

        
        /**
         *	@brief 获得积分增长
         *
         *	@return	每10分钟积分增长值
         */
        virtual uint32_t getPointGrow();
        
    protected:
		CC_SYNTHESIZE(uint32_t, m_nRank, Rank);  //排名
		CC_SYNTHESIZE(uint32_t, m_nWin, Win);
		CC_SYNTHESIZE(uint32_t, m_nLose, Lose)
    };

#pragma mark ---------玩家论剑信息------------
    class ChallengeInfo:public ValueObjcet, public ChallengeRank
    {
    public:
        ChallengeInfo():
            ChallengeRank(),
            m_nBestRank(0),
            m_nPoint(0),
            m_nRemainChallengeTimes(0),
            m_nRemainBossTimes(0)
        {
            
        }
        
        ~ChallengeInfo()
        {
            CC_SAFE_RELEASE_NULL(m_pDicAwards);
        }
        
        CREATE_FUNC(ChallengeInfo);
        
        /**
         *	@brief	加载数据
         *
         *	@param 	data 	数据
         */
        virtual void loadData(const Json::Value & data);
        
        void getTargetRanks(std::deque<uint32_t> & ranks);
		void getTargetRanks(std::deque<uint32_t> & ranks, const Json::Value &val);
                
		/**
		 *	@brief	初始化
		 *
		 *	@return	是否成功
		 */
		virtual bool init()
		{
            if(!ValueObjcet::init()) return false;
			m_pDicAwards = cocos2d::CCDictionary::create();
			CC_SAFE_RETAIN(m_pDicAwards);
			return true;
		};
        
        /**
         *	@brief	判断奖励是否已领取
         *
         *	@param 	awardId 	奖励编号
         *
         *	@return	是否已领取
         */
        inline bool isAwardDrawn(uint32_t awardId){
			return m_pDicAwards->objectForKey(awardId) != NULL;
        }
        
        /**
         *	@brief	领取奖励
         *
         *	@param 	awardId 	奖励编号
         */
        inline void drawAward(uint32_t awardId)

        {
            m_pDicAwards->setObject(cocos2d::CCInteger::create(awardId), awardId);
        }
        
        /**
         *	@brief  是否可以看见东皇太一
         *
         *	@return	是否可见
         */
        bool bossVisible();
        
        /**
         *	@brief	是否有资格挑战东皇太一
         *
         *	@return	是否有资格
         */
        bool bossChallengable();
        
        /**
         *	@brief	扣除积分
         *
         *	@param 	point 	积分
         */
        inline void deductPoint(uint32_t point)
        {
            if(m_nPoint >= point)
            {
                m_nPoint -= point;
            }
            else
            {
                m_nPoint = 0;
            }
        }
        
        /**
         *	@brief	扣除挑战次数
         */
        inline void deductChallengeTimes()

        {
            if(m_nRemainChallengeTimes > 1)
            {
                --m_nRemainChallengeTimes;
            }
            else
            {
                m_nRemainChallengeTimes = 0;
            }
        }
        
        /**
         *	@brief	扣除boss挑战次数
         */
        inline void deductBossTimes()

        {
            if(m_nRemainBossTimes > 1)
            {
                --m_nRemainBossTimes;
            }
            else
            {
                m_nRemainBossTimes = 0;
            }
        }
        
        inline uint32_t getMaxBoxTimes()
        {
            switch(m_nRank)
            {
                case 1:
                    return 5;
                case 2:
                    return 4;
                case 3:
                    return 3;
                case 4:
                    return 2;
                case 5:
                    return 1;
                default:
                    return 0;
            }
        }
        
    private:
        void insertLowerRanks(std::deque<uint32_t> & ranks);
        void insertUpperRanks(std::deque<uint32_t> & ranks);
        
    protected:
        CC_SYNTHESIZE(uint32_t, m_nBestRank, BestRank);                           //最佳排名
        CC_SYNTHESIZE(uint32_t, m_nPoint, Point);                                 //当前积分
        CC_SYNTHESIZE(uint32_t, m_nRemainChallengeTimes, RemainChallengeTimes);   //剩余论剑挑战次数
        CC_SYNTHESIZE(uint32_t, m_nRemainBossTimes, RemainBossTimes);             //剩余挑战东皇太一的次数
        /**
         *	@brief	已领取的奖励列表
         */
        cocos2d::CCDictionary * m_pDicAwards;

		friend class ::PvpModel;
    };

#pragma mark -----------对手信息------------- 
    
	//论剑对手信息
	class ChTargetInfo:public ValueObjcet, public ChallengeRank
	{
    public:
		CREATE_FUNC(ChTargetInfo);
                
        /**
         *	@brief	创建机器人
         *
         *	@param 	rank 	排名
         *
         *	@return	目标
         */
        static ChTargetInfo * createRobot(uint32_t rank);

        static ChTargetInfo * createSelf(ChallengeInfo* user);
        
        
	public:
		ChTargetInfo():ChallengeRank(),m_nVipLevel(0),m_nUserId(0),m_sName(""),m_nLevel(1),m_nLeaderId(0),m_nLeaderLevel(1), m_pRobotFormation(NULL),m_nLdid(0)
        {
        };
        
        ~ChTargetInfo();
        
		D_INIT_DEFAULT(ValueObjcet);
        
        /**
         *	@brief	加载数据
         *
         *	@param 	data 	数据
         */
        virtual void loadData(const Json::Value & data);
        
        /**
		 *	@brief	是否机器人
		 *
		 *	@return	是否机器人
		 */
		bool isRobot()
		{
			return m_nUserId == 0;
		};
        
        uint32_t getAvatarId()
        {
            return m_nLdid!=0?D_FIX_DRESS_ID(m_nLdid):m_nLeaderId;
        }
        
    private:        
        /**
         *	@brief	获取机器人编号
         *
         *	@return	获取机器人编号
         */
        void setRobotFormation();
        /**
         *	@brief	获取机器人等级
         *
         *	@return	机器人等级
         */
        void setRobotLevel();
        
        
	protected:
		CC_SYNTHESIZE(uint32_t, m_nVipLevel, VipLevel);             //玩家VIP等级
        CC_SYNTHESIZE(uint32_t, m_nUserId, UserId);             //玩家编号
        CC_SYNTHESIZE(std::string, m_sName, Name);              //名字
        CC_SYNTHESIZE(uint32_t, m_nLevel, Level);               //等级        
        CC_SYNTHESIZE(uint32_t, m_nLeaderId, LeaderId);         //获取队长编号
        CC_SYNTHESIZE_READONLY(uint32_t, m_nLdid, LdId);        //皮肤编号
        CC_SYNTHESIZE(uint32_t, m_nLeaderLevel, LeaderLevel);   //队长等级
        CC_SYNTHESIZE_READONLY(otherFormation::RobotFormationInfo*, m_pRobotFormation, RobotFormation);           //机器人阵型
	};

	class ChMallItem:public ValueObjcet
	{
	public:
		enum BuyType
		{
			kBuyTypeRank = 1,   //积分购买
			kBuyTypePoint = 2,	//点券购买
		};

	public:
		CREATE_FUNC(ChMallItem);
		D_INIT_DEFAULT(ValueObjcet);
		ChMallItem()
			: m_nItemID(0)
			, m_eItemType(drop::kDropTypeNone)
			, m_nItemNum(0)
			, m_nCost(0)
			, m_bStatus(false)
			, m_nID(0)
			, m_nBuyType(kBuyTypeRank)
		{

		}
		~ChMallItem()
		{

		}
	protected:
		friend class ::PvpModel;
		CC_SYNTHESIZE_READONLY(unsigned int, m_nItemID, ItemID);
		CC_SYNTHESIZE_READONLY(drop::DropType, m_eItemType, ItemType);
		CC_SYNTHESIZE_READONLY(unsigned int, m_nItemNum, ItemNum);
		CC_SYNTHESIZE_READONLY(unsigned int, m_nCost, Cost);
		CC_SYNTHESIZE_READONLY(unsigned int, m_nID, ID);
		//新加一个类型，判断积分购买（1）还是点券购买（2）
		CC_SYNTHESIZE_READONLY(BuyType, m_nBuyType, BuyType);
		CC_SYNTHESIZE(bool, m_bStatus,Status);
	};
}
#pragma mark ----------PVP信息------------

class PvpModel : public Model
{
public:
	typedef cocos2d::CCArray AwardArray;
	typedef cocos2d::CCDictionary RankDic;

public:
    typedef enum kPvpType
    {
        kPvpTypeTarget = 1,     //挑战排名
        kPvpTypeNpc = 2,        //挑战试炼木人
        kPvpTypeBoss = 3        //挑战东皇太一
    } kPvpType;
    
public:
    PvpModel();
    virtual ~PvpModel();

    virtual bool init();
    
    void getInitInfo();
    
    /**
     *	@brief	获取排名
     *
     *	@param 	dic 	排名列表
     */
    void getTopRanks(cocos2d::CCDictionary* dic);
    
    
    
    /**
     *	@brief	获取目标列表
     *
     *	@param 	arr 	目标列表
     */
    void getTarges(cocos2d::CCArray* arr);


    
    /**
     *	@brief	获取初始化数据完成
     */
    void onInitComplete(const Json::Value & json);
    /**
     *	@brief	获取论剑信息完成
     */
    void onInfoComplete(const Json::Value & jsonInfo);
    /**
     *	@brief	获取目标列表完成
     */
    void onTargetsComplete(const Json::Value & jsonTargets);
        
    /**
     *	@brief	挑战东皇太一完成
     *
     *	@param 	requestUid 	请求唯一编号
     */
    void onChallengeBossComplete(uint32_t requestUid);
    
    /**
     *	@brief	挑战试炼木人完成
     *
     *	@param 	requestUid 	请求唯一编号
     */
    void onChallengeNpcComplete(uint32_t requestUid);
    
    /**
     *	@brief	挑战排名完成
     *
     *  @param 	requestUid 	请求唯一编号
     *	@param 	jsonInfo 	玩家信息
     *	@param 	jsonTargets 可以看见的目标列表
     */
    void onChallengeTargetComplete(uint32_t requestUid, const Json::Value & jsonInfo, const Json::Value & jsonTargets);
    
    /**
     *	@brief	挑战排名完成
     *
     *  @param 	requestUid 	请求唯一编号
     *	@param 	jsonInfo 	玩家信息
     *	@param 	jsonTargets 可以看见的目标列表
     */
    void onChallengeTargetComplete(const Json::Value & jsonInfo, const Json::Value & jsonTargets);
   
	void parseChallengeInfo(const Json::Value &val);
	void parseRanks(const Json::Value &val);	
	void parseTargets(const Json::Value &val);     

	void parseChMall(const Json::Value & val);
	void exchanged(const Json::Value & request, const Json::Value & response);
    
	pvp::AwardInfo * getAward(uint32_t id);

	/*领取奖励*/
	void drawAward(uint32_t requestUid, const pvp::AwardInfo * pAward);

    void handleError(const Json::Value & request, const Json::Value & response);

	int getMallLvInfo(uint32_t lv);

	//结算显示target_id,默认和机器人是0，不做显示
	CC_SYNTHESIZE(unsigned int, m_uTargetID, TargetID);
	//是否积分成为史上最强
	CC_SYNTHESIZE(bool, m_BestEver, BestEver);
	CC_SYNTHESIZE(uint32_t, m_OldBest, OldBest);
	CC_SYNTHESIZE(uint32_t, m_NewBest, NewBest);
	CC_SYNTHESIZE(uint32_t, m_BestRewardPoint, BestRewardPoint);
private:
    /**
     *	@brief	加载奖励数据
     */
    void loadAwardData();
    
    /**
     *	@brief	显示掉落
     *
     *	@param 	requestUid 	请求编号
     */
    void showDrop(uint32_t requestUid);

    
    /**
     *	@brief	设置排行榜及目标列表
     *
     *	@param 	resetTopRank 	是否重置排行榜
     */
    void setRanksAndTargets(const Json::Value & val, bool resetTopRank);   
    
protected:
    CC_SYNTHESIZE_READONLY(pvp::ChallengeInfo*, m_pChallengeInfo, ChallengeInfo);       //玩家论剑信息
    cocos2d::CCArray * m_pArrTargets;                                                   //目标列表
    cocos2d::CCDictionary * m_pDicTopRanks;                                             //排行榜
    CC_SYNTHESIZE_READONLY(AwardArray *, m_pArrAwards, Awards);                         //奖励列表
    CC_SYNTHESIZE(uint16_t, m_nGrade, Grade);                                           //战斗评分
    CC_SYNTHESIZE_READONLY(uint32_t, m_nUserIndex, UserIndex);                          //玩家在论剑列表中的位置

	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pArrChMall, ArrChMall);
	CC_SYNTHESIZE_READONLY(unsigned int, m_uRefreshTs, RefreshTs);
	CC_SYNTHESIZE_READONLY(unsigned int, m_uPrizeTime, PrizeTime);
	CC_SYNTHESIZE_READONLY(unsigned int, m_uMallLv, MallLv); //商店等级
	CC_SYNTHESIZE_READONLY(unsigned int, m_uExchangePoint,ExchangePoint); //已兑换的积分

	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pArrArena, ArrArena);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pMallUpData, MallUpData);
	void LoadArenaData(const CSVDecoder::CSV &val);
	void LoadMallLvlData(const CSVDecoder::CSV &val);
};

#endif /* defined(__QSMY__PvpModel__) */
