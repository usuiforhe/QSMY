//
//  GuildModel.h
//  公会
//
//  Created by Eci on 13-10-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__GuildModel__
#define __QSMY__GuildModel__

#include "Model.h"
#include "UserModel.h"

#define GUILD_TXT_SEARCH_PLACEHOLDER	"SearchGuild"

#define GUILD_LEAVE_TIME	"GUILD_LEAVE_TIME"
#define GUILD_MALL_CELL_STEP	"GUILD_MALL_CELL_STEP"
#define GUILD_MALL_REFRESH_COST	"GUILD_MALL_REFRESH_COST"
#define GUILD_MALL_CELL_INIT	"GUILD_MALL_CELL_INIT"
#define GUILD_MALL_CELL_LOCK	"GUILD_MALL_CELL_LOCK"
#define GUILD_MALL_CELL_MAX	"GUILD_MALL_CELL_MAX"

 #define K_GUILD_QUESTION_INTERVAL		(D_CONFIG_UINT_FOR_KEY(GUILD_QUESTION_INTERVAL))
#define GUILD_NOTICE_TYPE_MAX	4
#define D_EVENT_GUILD_CONTRIBUTION	"guild contribution"
#define D_EVENT_GUILD_GET			"guild get"
#define D_EVENT_GUILD_GET_NOTICE	"guild get notice"
#define D_EVENT_GUILD_GET_LOG	"guild get log"


#define D_EVENT_GUILD_GET_LIST		"guild get list"
#define D_EVENT_GUILD_APPLY_LIST	"guild apply list"
#define D_EVENT_GUILD_DETAILS		"guild details"
#define D_EVENT_GUILD_TREE			"guild tree"
#define D_EVENT_GUILD_MALL_REFRESH	"guild mall refresh"
#define D_EVENT_GUILD_MALL_EXCHANGE	"guild mall exchange"
#define D_EVENT_GUILD_QUIT			"guild quit"
class GuildModel;

namespace guild {

#pragma mark 枚举类型
    
    enum GuildLogType						//	公会日志
	{
		GuildJoin = 1, //1 加入门派
		GuildOut,       //2 退出门派
		GuildKick,      //3 被踢出门派
		GuildUp,        //4 门派升级
        GuildChangeName,//5 掌门更换
        GuildDragon,    //6 苍龙现排名
		GuildUpKing,    //7 王道升级
		GuildUpKnight,  //8 侠道升级
	};
    

	enum GuildPosition						//	公会职位
	{
		GuildPositionRemoved = 0,
		GuildPositionMember,
		GuildPositionAssistant,
		GuildPositionPresident,
		GuildPositionNone = GuildPositionRemoved,
	};
	enum GuildNoticeType					//	公会跑马灯类型
	{
		GuildNoticeTypeNone = 0,
		GuildNoticeTypeAnswer = 1,
		GuildNoticeTypeOverlord = 2,
		GuildNoticeTypeChivalry = 3,
		GuildNoticeTypeSign = 4,
	};

	enum GuildJoinBossState
	{
		GuildJoinBossNotOpen	= 0,//未开启
		GuildJoinBossNotJoin	= 1,//今日还未参与苍龙现
		GuildJoinBossHaveJoined = 2,//今日已参与苍龙现
	};

#pragma mark 数据结构

	// 门派商店
	class GuildMall : public ValueObjcet
	{
	public:
		GuildMall()
		:m_iRefreshCount( 0 )
		,m_goodIdArr( NULL )
		,m_exchangedIdArr( NULL )
		{

		};
		~GuildMall()
		{
			CC_SAFE_RELEASE_NULL( m_goodIdArr ) ;
			CC_SAFE_RELEASE_NULL( m_exchangedIdArr ) ;
		};
		CREATE_FUNC( GuildMall ) ;
		bool init()
		{
            if( !ValueObjcet::init() ) return false ;
			m_goodIdArr = cocos2d::CCArray::create();
			CC_SAFE_RETAIN( m_goodIdArr ) ;
			m_exchangedIdArr = cocos2d::CCArray::create();
			CC_SAFE_RETAIN( m_exchangedIdArr ) ;
			return true ;
		};
		// 添加到兑换列表
		void addExchanged( uint32_t exchangedID )
		{
			if ( m_exchangedIdArr )
			{
				m_exchangedIdArr->addObject( 
					cocos2d::CCString::createWithFormat( "%d", exchangedID ) ) ;
			}
		} ;
	protected:
		friend class ::GuildModel ;
		// 剩余刷新次数
		CC_SYNTHESIZE( int, m_iRefreshCount, RefreshCount ) ;
		// 商店兑换编号数组
		CC_SYNTHESIZE( cocos2d::CCArray *, m_goodIdArr, GoodIdArr ) ;
		// 已兑换格子编号数组
		CC_SYNTHESIZE( cocos2d::CCArray *, m_exchangedIdArr, ExchangedIdArr ) ;
	};

	class GuildTree : public ValueObjcet	//	天工树
	{
	public:
		CREATE_FUNC(GuildTree);
		D_INIT_DEFAULT(ValueObjcet);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(uint32_t, m_iKind, Kind);						//天工树类型（1王道	2侠道）
		CC_SYNTHESIZE(uint32_t, m_iLevel, Level);					//等级
		CC_SYNTHESIZE(uint64_t, m_iItemCount, ItemCount);			//物品/金钱数量
		CC_SYNTHESIZE(uint64_t, m_iItemCountRest, ItemCountRest);	//到达下一级剩余数量
	};
    


/**--------------------------日志数据 start--------------------------*/
    class GuildLogBase : public ValueObjcet{
    public:
        virtual void parse(const Json::Value& val) = 0;
    };
    
    class GuildLog : public GuildLogBase	//	日志
	{
        GuildLog();
        ~GuildLog();
	public:
		CREATE_FUNC(GuildLog);
		D_INIT_DEFAULT(GuildLogBase);
        virtual void parse(const Json::Value& val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(uint32_t, m_id, Id);
		CC_SYNTHESIZE(uint32_t, m_ts, Ts);
		CC_SYNTHESIZE(uint32_t, m_type, Type);
        CC_SYNTHESIZE(GuildLogBase*, m_params, Params);
	};
    
    class GuildLogJoin : public GuildLogBase
	{
	public:
		CREATE_FUNC(GuildLogJoin);
		D_INIT_DEFAULT(GuildLogBase);
        virtual void parse(const Json::Value& val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(std::string, m_new, username);
	};
    
    class GuildLogOut : public GuildLogBase
	{
	public:
		CREATE_FUNC(GuildLogOut);
		D_INIT_DEFAULT(GuildLogBase);
        virtual void parse(const Json::Value& val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(std::string, m_out, outname);
	};
    
    class GuildLogKick : public GuildLogBase
	{
	public:
		CREATE_FUNC(GuildLogKick);
		D_INIT_DEFAULT(GuildLogBase);
        virtual void parse(const Json::Value& val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(std::string, m_out, outname);
        CC_SYNTHESIZE(std::string, m_act, actname);
        
	};
    
    class GuildLogUp : public GuildLogBase
	{
	public:
		CREATE_FUNC(GuildLogUp);
		D_INIT_DEFAULT(GuildLogBase);
        virtual void parse(const Json::Value& val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(uint32_t, m_newL, newlv);
	};
    
    class GuildLogChangeName : public GuildLogBase
	{
	public:
		CREATE_FUNC(GuildLogChangeName);
		D_INIT_DEFAULT(GuildLogBase);
        virtual void parse(const Json::Value& val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(std::string, m_old, oldName);
        CC_SYNTHESIZE(std::string, m_new, newName);
	};
    
    class GuildLogDragon : public GuildLogBase
	{
	public:
		CREATE_FUNC(GuildLogDragon);
		D_INIT_DEFAULT(GuildLogBase);
        virtual void parse(const Json::Value& val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(uint32_t, m_rank, rank);
	};
    
    
    class GuildLogUpKing : public GuildLogBase	//	日志
	{
	public:
		CREATE_FUNC(GuildLogUpKing);
		D_INIT_DEFAULT(GuildLogBase);
        virtual void parse(const Json::Value& val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(uint32_t, m_newL, newlv);
	};
    
    class GuildLogUpKnight : public GuildLogBase	//	日志
	{
	public:
		CREATE_FUNC(GuildLogUpKnight);
		D_INIT_DEFAULT(GuildLogBase);
        virtual void parse(const Json::Value& val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(uint32_t, m_newL, newlv);
	};
/**--------------------------日志数据 end--------------------------*/

    
    
    
    
	class GuildBasics : public ValueObjcet	//	公会信息
	{
	public:
		CREATE_FUNC(GuildBasics);
		D_INIT_DEFAULT(ValueObjcet);
		void parse(const Json::Value& val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(uint32_t, m_nGuildID, GuildID);			//公会编号
		CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sName, Name);	//名称
		CC_SYNTHESIZE(uint32_t, m_iLevel, Level);				//等级
		CC_SYNTHESIZE(uint32_t, m_iUserCount, UserCount);		//玩家数量
		CC_SYNTHESIZE(bool, m_isApplied, IsApplied);			//当前用户是否申请
	};
	class GuildInfo : public GuildBasics	//	公会详情
	{
	public:
		CREATE_FUNC(GuildInfo);
		D_INIT_DEFAULT(ValueObjcet);
		void parse(const Json::Value& val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(uint32_t, m_nFounderID, FounderID);						//创建者ID
		CC_SYNTHESIZE(uint32_t, m_nLeaderID, LeaderID);							//掌门ID
		CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sAnnouncement, Announcement);	//公告
		CC_SYNTHESIZE(uint32_t, m_iContributionLevel, ContributionLevel);		//当前等级经验值
		CC_SYNTHESIZE(uint32_t, m_iContributionNext, ContributionNext);			//下一级经验值
		CC_SYNTHESIZE(uint32_t, m_nAssistant1, Assistant1);						//助理ID
		CC_SYNTHESIZE(uint32_t, m_nAssistant2, Assistant2);						//助理ID
		CC_SYNTHESIZE(uint32_t, m_nAssistant3, Assistant3);						//助理ID
		CC_SYNTHESIZE(uint32_t, m_iContribution, Contribution);					//帮贡
		CC_SYNTHESIZE(uint32_t, m_iContributionAdded, ContributionAdded);		//新增帮贡
		CC_SYNTHESIZE_READONLY(uint32_t, m_nCreateTs, CreateTs);
		CC_SYNTHESIZE_READONLY(bool, m_bIsCreatedToday, IsCreatedToday);
	};
	class GuildUserBasics : public ValueObjcet	//	成员信息
	{
	public:
		GuildUserBasics()
			: m_nUserId(0)
			, m_sName("")
			, m_iContribution(0)
			, m_iRemainContribution(0)
			, m_iPosition(GuildPositionMember)
			, m_sKnighthoodName("")
			, m_iKnighthoodLevel(0)
			, m_iLevel(0)
			, m_iLastLoginTs(0)
			, m_uLeaderID(0)
			, m_uLeaderDress(0)
		{

		}
		virtual ~GuildUserBasics() {};
		CREATE_FUNC(GuildUserBasics);
		D_INIT_DEFAULT(ValueObjcet);
		void parse(const Json::Value& val);
		GuildJoinBossState getGuildJoinBossState();
		uint32_t getLeaderDress() {return m_uLeaderDress != 0 ? D_FIX_DRESS_ID(m_uLeaderDress) : m_uLeaderID;};
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(uint32_t, m_nUserId, UserId);									//玩家编号
		CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sName, Name);						//名字
		CC_SYNTHESIZE(uint32_t, m_iContribution, Contribution);						//帮贡
		CC_SYNTHESIZE(uint32_t, m_iRemainContribution, RemainContribution);			//剩余帮贡
		CC_SYNTHESIZE(GuildPosition, m_iPosition, Position);						//任职
		CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sKnighthoodName, KnighthoodName);	//爵位名称
		CC_SYNTHESIZE(uint32_t, m_iKnighthoodLevel, KnighthoodLevel);				//爵位等级
		CC_SYNTHESIZE(uint32_t, m_iLevel, Level);									//等级
		CC_SYNTHESIZE(uint32_t, m_iLastLoginTs, LastLoginTs);						//上次登录时间（秒）
		uint32_t m_uLeaderID;
		uint32_t m_uLeaderDress;
	};
	class GuildUser : public GuildUserBasics	//	成员详情（用户自身）
	{
	public:
		CREATE_FUNC(GuildUser);
		D_INIT_DEFAULT(ValueObjcet);
		void parse(const Json::Value &val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(uint32_t, m_nGuildID, GuildID);						//公会编号
		CC_SYNTHESIZE(uint32_t, m_nLastSign, LastSign);						//签到时间
		CC_SYNTHESIZE(uint32_t, m_nLastAnswer, LastAnswer);					//答题时间
		CC_SYNTHESIZE(uint32_t, m_nQuestionID, QuestionID);					//题目编号
		CC_SYNTHESIZE(uint32_t, m_nChivalryTime, ChivalryTime);				//侠道时间
		CC_SYNTHESIZE(uint32_t, m_nOverlordTime, OverlordTime);				//王道时间
		CC_SYNTHESIZE(uint32_t, m_iChivalry, Chivalry);						//侠道次数
		CC_SYNTHESIZE(uint32_t, m_iOverlord, Overlord);						//王道次数
		CC_SYNTHESIZE(bool, m_bIsSigned, IsSigned);							//是否签到
		CC_SYNTHESIZE(uint32_t, m_iNextExp, NextExp);						//剩余经验
		CC_SYNTHESIZE(uint32_t, m_iCoinsNeeded, CoinsNeeded);				//王道钱币
		CC_SYNTHESIZE(uint32_t, m_iContributionAdded, ContributionAdded);	//新增帮贡
	private:
		cocos2d::CCArray *m_pApplications;
	private:
	};
	class GuildApplication : public ValueObjcet	//	公会申请
	{
	public:

		CREATE_FUNC(GuildApplication);
		D_INIT_DEFAULT(ValueObjcet);
		void parse(const Json::Value &val);
	protected:
		CC_SYNTHESIZE(uint32_t, m_nApplicationID, ApplicationID);	//申请编号
		CC_SYNTHESIZE(uint32_t, m_nGuildID, GuildID);				//公会编号
		CC_SYNTHESIZE(uint32_t, m_nUserId, UserId);					//玩家编号
		CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sName, Name);		//玩家名称
		CC_SYNTHESIZE(uint32_t, m_iLevel, Level);					//玩家等级
		CC_SYNTHESIZE(uint32_t, m_iFight, Fight);					//玩家战力
	};
	class GuildNotice : public ValueObjcet		//	公会跑马灯
	{
	public:

		CREATE_FUNC(GuildNotice);
		D_INIT_DEFAULT(ValueObjcet);
		void parse(const Json::Value &val);
	protected:
		friend class ::GuildModel;
		CC_SYNTHESIZE(GuildNoticeType, m_iType, Type);					//跑马灯类型
		CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sUserName, UserName);	//用户名称
		CC_SYNTHESIZE(uint32_t, m_iContribution, Contribution);			//帮贡
		CC_SYNTHESIZE(uint32_t, m_iItemNumber, ItemNumber);				//物品/金钱数量
		CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sMarquee, Marquee);	//跑马灯文字
	};

#pragma mark 本地配置基本信息

	class GuildLevel : public ValueObjcet
	{
	public:
        
        CREATE_FUNC(GuildLevel);
        D_INIT_DEFAULT(ValueObjcet);
		uint32_t level;		//等级
		uint64_t nextExp;	//当前等级对应最大经验
		uint64_t totalExp;	//当前等级累计经验
	};
	class GuildKnighthoodLevel : public GuildLevel
	{
	public:

		CREATE_FUNC(GuildKnighthoodLevel);
		D_INIT_DEFAULT(ValueObjcet);
		std::string name;
	};
	class GuildCoins : public ValueObjcet
	{
	public:
		
        CREATE_FUNC(GuildCoins);
        D_INIT_DEFAULT(ValueObjcet);
		uint32_t times;			//次数
		uint32_t coins;			//捐献钱币
		uint32_t contribution;	//帮贡
	};
	class GuildTreeSkill : public ValueObjcet
	{
	public:
		
        CREATE_FUNC(GuildTreeSkill);
        D_INIT_DEFAULT(ValueObjcet);
		uint32_t level;			//等级
		uint32_t skillID;		//技能
		float skillValue;		//数值
		uint32_t knighthood;	//爵位
		std::string text;		//文字
	};
	class GuildQuestion : public ValueObjcet
	{
	public:
		
        CREATE_FUNC(GuildQuestion);
        D_INIT_DEFAULT(ValueObjcet);
		uint32_t questionID;			//问题ID
		uint32_t rightAnswer;			//正确答案
		std::string questionStr;		//问题
		std::string answer1Str;			//答案1
		std::string answer2Str;			//答案2
	};
	class GuildLastLoginDesc : public ValueObjcet
	{
	public:

		CREATE_FUNC(GuildLastLoginDesc);
		D_INIT_DEFAULT(ValueObjcet);
		uint32_t minInveter;			//最小时间差（秒）
		std::string lastLoginDesc;		//上次登录描述
	};
	// 门派商店物品基础配置数据
	class GuildMallGoods : public ValueObjcet
	{
	public:
		CREATE_FUNC( GuildMallGoods ) ;
		D_INIT_DEFAULT( ValueObjcet ) ;
		uint32_t id ;
		uint32_t cost ;
		uint32_t goodsID ;
		uint32_t type ;
		uint32_t num ; 
	};
}

class GuildModel : public Model
{
public:
	GuildModel();
	~GuildModel();
	virtual bool init();

#pragma mark 解析数据
	void parse(const Json::Value &val);
	void parseGuildInfo(const Json::Value &val);
	void parseGuildList(const Json::Value &val, uint32_t startNum = 0);
	void parseGuildApplication(const Json::Value &val);
	void parseSelfApplication(const Json::Value &val);
	void parseGuildSelfInfo(const Json::Value &val);
	guild::GuildNotice *parseOneNotice(const Json::Value &val);
    void parseGuildLog(const Json::Value &val);
    void parseGuildNotice(const Json::Value &val, guild::GuildNoticeType iType = guild::GuildNoticeTypeNone);
	void parseGuildUser(const Json::Value &val);
	void parseOtherGuildInfo(const Json::Value &val);
	void parseOtherGuildUser(const Json::Value &val, uint32_t guildID);
	void parseTree(const Json::Value &val);
	void parseGuildMall( const Json::Value &val, int remainConValue = -1 ) ;
	void errorNoGuild();

#pragma mark 同步
    void syncGuildApplyList();

	void syncFromServer();
	void syncGuildByID(uint32_t guildID);
#pragma mark 本地配置信息

private:
	cocos2d::CCDictionary *m_pGuildLevelMap;
	cocos2d::CCDictionary *m_pGuildKnighthoodMap;
	cocos2d::CCDictionary *m_pTreeChivalryLevelMap;
	cocos2d::CCDictionary *m_pTreeOverlordLevelMap;
	cocos2d::CCDictionary *m_pGuildCoinsMap;
	cocos2d::CCDictionary *m_pTreeChivalrySkillMap;
	cocos2d::CCDictionary *m_pTreeOverlordSkillMap;
	cocos2d::CCDictionary *m_pGuildQuestionMap;
	cocos2d::CCDictionary *m_pGuildMarqueeMap;
	cocos2d::CCArray *m_pGuildLastLoginDescMap;
	cocos2d::CCDictionary *m_pGuildMallMap ;

	void LoadGuildKnighthoodLevelData(const CSVDecoder::CSV &val, cocos2d::CCDictionary *dataMap);
	void LoadGuildLevelData(const CSVDecoder::CSV &val, cocos2d::CCDictionary *dataMap);
	void LoadGuildCoinsData(const CSVDecoder::CSV &val);
	void LoadGuildSkillData(const CSVDecoder::CSV &val, cocos2d::CCDictionary *dataMap);
	void LoadGuildQuestionData(const CSVDecoder::CSV &val);
	void LoadGuildMarqueeData(const CSVDecoder::CSV &val);
	void LoadGuildLastLoginDesc(const CSVDecoder::CSV &val, cocos2d::CCArray *dataMap);
	void LoadGuildMallData( const CSVDecoder::CSV &val, cocos2d::CCDictionary *dataMap ) ;

protected:

#pragma mark 变量

	CC_SYNTHESIZE_RETAIN(guild::GuildUser*, m_pSelfInfo, SelfInfo);
	CC_SYNTHESIZE_RETAIN(guild::GuildInfo*, m_pGuildInfo, GuildInfo);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, m_pGuildList, GuildList);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, m_pGuildApplication, GuildApplication);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, m_pSelfApplication, SelfApplication);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, m_pGuildUser, GuildUser);
    
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, m_pGuildLog, GuildLog);

    
	CC_SYNTHESIZE_READONLY(guild::GuildQuestion *, m_pQuestion, GuildQuestion);
	CC_SYNTHESIZE_READONLY(guild::GuildTree *, m_pTreeChivalry, TreeChivalry);
	CC_SYNTHESIZE_READONLY(guild::GuildTree *, m_pTreeOverlord, TreeOverlord);
	CC_SYNTHESIZE_READONLY(uint32_t, m_pMaxLevelChivalry, MaxLevelChivalry);
	CC_SYNTHESIZE_READONLY(uint32_t, m_pMaxLevelOverlord, MaxLevelOverlord);
	CC_SYNTHESIZE_READONLY( guild::GuildMall *, m_pGuildMall, GuildMall ) ;

#pragma mark getter

public:
	guild::GuildInfo *getOtherGuild(uint32_t guildID);
	cocos2d::CCArray *getOtherGuildUsers(uint32_t guildID);
	cocos2d::CCArray *getGuildNoticeByType(guild::GuildNoticeType iType);
	guild::GuildTreeSkill *getTreeSkillByLevelAndType(uint32_t iLevel, uint32_t iType);
	std::string GetKnighthoodNameByLevel(uint32_t iLevel);
	std::string getPositionByType(guild::GuildPosition iPostion);
	const guild::GuildUserBasics* getGuildUserBasicsByUserId(uint32_t userId) const;
	std::string getLastLoginDescByInveter(guild::GuildUserBasics *userInfo);
	guild::GuildMallGoods *getMallGoodsById( uint32_t id ) ;
    
    //公会成员是否能取代会长职位
    uint32_t guildUserCanReplace();
    //是否能取代会长
    bool canReplaceBoss();
    
    void getGuildLogs();

    
private:
	cocos2d::CCDictionary *m_pOtherGuildMap;
	cocos2d::CCDictionary *m_pOtherGuildUsersMap;
	cocos2d::CCDictionary* m_pGuildNotice;

#pragma mark 刷新数据
public:
	void refreshContribution(const Json::Value &val);
	void refreshContribution();
	void setOverlordTimes(uint32_t iTimes);
	void addGuildNotice(guild::GuildNotice *pNotice, guild::GuildNoticeType iType = guild::GuildNoticeTypeNone);
	void addGuildNotice(const Json::Value &noticeData);
	void changeSelfApplication(uint32_t guildID, uint32_t guildAction);	// guildAction = 1 申请 2 取消
	
    void selfReplaceBoss(uint32_t king);

    void dealGuildApplication(uint32_t applicationID, uint32_t guildAction);	// guildAction = 1 同意 2 拒绝
	void changePosition(const Json::Value &requestData, const Json::Value &responseData);
	virtual void reset();
	void resetGuild(float dt);
	void quitGuild();
};
#endif