//
//  GuildBossModel.h
//  公会
//
//  Created by Eci on 14-2-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__GuildBossModel__
#define __QSMY__GuildBossModel__

#define D_EVENT_GUILDBOSS_GET_TARGETS "guildboss_get_targets"
#define D_EVENT_GUILDBOSS_GET_RANK "guildboss_get_rank"
#define D_EVENT_GUILDBOSS_REFRESH "guildboss_refresh"
#define D_EVENT_GUILDBOSS_WOUNDED_LIST "guildboss_wounded_list"
#define D_EVENT_GUILDBOSS_FIGHT_LIST "guildboss_fight_list"
#define D_EVENT_GUILDBOSS_ROB_SETTLE "guildboss_rob_settle"
#define D_EVENT_GUILDBOSS_NOTICE "guildboss_notice"
#define D_EVENT_GUILDBOSS_ALL_SETTLE "guildboss_all_settle"
#define D_EVENT_GUILDBOSS_AUTO_ATTACK	"guildboss_auto_attack"
#define D_EVENT_GUILDBOSS_ATTACK "guildboss_attack"
#define D_EVENT_GUILDBOSS_ERROR_NO_GUILD	"guildboss_error_no_guild"
#define D_EVENT_GUILDBOSS_ADD_DAMAGE	"guildboss_add_damage"


#define GUILDBOSS_GUILD_RANK_PAGE_SIZE 10

#include "Model.h"

static const char * stringForUIntWith10K(uint32_t num)
{
	if (num < 100000)
	{
		return D_CSTRING_FROM_UINT(num);
	}
	return cocos2d::CCString::createWithFormat("%u%s", num / 10000, D_LOCAL_STRING("TenThousand").c_str())->getCString();
}

static const char * stringForIntWith10K(int num)
{
	if (num < 100000 && num > -100000)
	{
		return D_CSTRING_FROM_INT(num);
	}
	return cocos2d::CCString::createWithFormat("%d%s", num / 10000, D_LOCAL_STRING("TenThousand").c_str())->getCString();
}

class GuildBossModel;

namespace guildboss {

	enum RankChangeType
	{
		RankChangeTypeNone = 0,
		RankChangeTypeDown = 1,
		RankChangeTypeUp = 2,
		RankChangeTypeMax = 3,
	};

	enum GuildBossState
	{
		GuildBossStateClose = -2,
		GuildBossStatePreparing = -1,
		GuildBossStateBoss = 0,
		GuildBossStateRob = 1,
		GuildBossStateDragon = GuildBossStateBoss,
		GuildBossStateFight = GuildBossStateRob,
	};

#pragma mark - GuildBoss -
	class GuildBossDamage;

	class GuildBossBasic : public ValueObjcet
	{
	public:
		GuildBossBasic();
		CREATE_FUNC(GuildBossBasic);
		D_INIT_DEFAULT(ValueObjcet);

		void parse(const Json::Value &val);
	protected:
		friend class ::GuildBossModel;
		CC_SYNTHESIZE_READONLY(uint32_t, m_nGuildID, GuildID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iGuildFight, GuildFight);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iDamage, Damage);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iLastDamage, LastDamage);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nLastRank, LastRank);

		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sGuildName, GuildName);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iLevel, Level);;
	private:
	};

	class GuildBoss : public GuildBossBasic
	{
	public:
		GuildBoss();
		CREATE_FUNC(GuildBoss);
		D_INIT_DEFAULT(GuildBossBasic);

		void parse(const Json::Value& val);
		void addDamage(GuildBossDamage *pDamage);
	protected:
		friend class ::GuildBossModel;
		CC_SYNTHESIZE_READONLY(uint32_t, m_nLatestWeekTime, LatestWeekTime);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nTargetID, TargetID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDeclareTime, DeclareTime);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sTargetName, TargetName);
		CC_SYNTHESIZE(bool, m_bDeclaredToday, IsDeclaredToday);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iDamageDaily, DamageDaily);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nStep, Step);
	private:
	};
	class GuildBossUserBasic : public ValueObjcet
	{
	public:
		GuildBossUserBasic();
		CREATE_FUNC(GuildBossUserBasic);
		D_INIT_DEFAULT(ValueObjcet);

		void parse(const Json::Value &val);
		uint32_t getLeaderDress() {return m_iLeaderDress != 0 ? D_FIX_DRESS_ID(m_iLeaderDress) : m_iLeaderID;};
	protected:
		friend class ::GuildBossModel;
		CC_SYNTHESIZE_READONLY(uint32_t, m_nUserID, UserID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nGuildID, GuildID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iUserFight, UserFight);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iTotalDamage, TotalDamage);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iDamageForRob, DamageForRob);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iDamageForRobBase, DamageForRobBase);

		CC_SYNTHESIZE_READONLY(uint32_t, m_iLevel, Level);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sGuildName, GuildName);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sUserName, UserName);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iLeaderID, LeaderID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iRobBlood, RobBlood);
		CC_SYNTHESIZE_READONLY(uint32_t, m_uRescueCount, RescueCount);

		CC_SYNTHESIZE_READONLY(uint32_t, m_iDamageForRobSuccess, DamageForRobSuccess);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nRescueRank, RescueRank);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iDamageDaily, DamageDaily);
	protected:
		uint32_t m_iLeaderDress;
	};

	class GuildBossUser : public GuildBossUserBasic
	{
	public:
		GuildBossUser();
		void parse(const Json::Value &val);
		void addDamage(GuildBossDamage *pDamage);
		CREATE_FUNC(GuildBossUser);
		D_INIT_DEFAULT(GuildBossUserBasic);
	protected:
		friend class ::GuildBossModel;
		CC_SYNTHESIZE_READONLY(uint32_t, m_nLatestWeekTime, LatestWeekTime);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nLatestDateTime, LatestDateTime);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iCurrentFight, CurrentFight);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iRound, Round);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nAttackTs, AttackTs);

		CC_SYNTHESIZE_READONLY(uint32_t, m_iDamageAdded, DamageAdded);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nNextTime, NextTime);

		CC_SYNTHESIZE_READONLY(uint32_t, m_nAvailTimes, AvailTimes);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nPayedTimes, PayedTimes);
		
		CC_SYNTHESIZE_READONLY(uint32_t, m_uCritical, Critical);
		CC_SYNTHESIZE_READONLY(uint32_t, m_uAnger, Anger);
	};

	class GuildWoundedUser : public ValueObjcet
	{
	public:
		CREATE_FUNC(GuildWoundedUser);
		D_INIT_DEFAULT(ValueObjcet);
		GuildWoundedUser();
		virtual ~GuildWoundedUser();
	protected:
		friend class ::GuildBossModel;
		CC_SYNTHESIZE_READONLY(uint32_t, m_nUserID, UserID);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iFight, Fight);
		CC_SYNTHESIZE_RETAIN(GuildBossUserBasic *, m_pBasicInfo, BasicInfo);
	private:
	};

	class GuildHatred : public ValueObjcet
	{
	public:
		CREATE_FUNC(GuildHatred);
		D_INIT_DEFAULT(ValueObjcet);
	protected:
		friend class ::GuildBossModel;
		CC_SYNTHESIZE_READONLY(uint32_t, m_nGuildID, GuildID);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sGuildName, GuildName);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iDamageForRob, DamageForRob);
	};

	class GuildBossConfig : public ValueObjcet
	{
	public:
		CREATE_FUNC(GuildBossConfig);
		D_INIT_DEFAULT(ValueObjcet);
	protected:
		friend class ::GuildBossModel;
		CC_SYNTHESIZE_READONLY(uint32_t, m_nApplyDeadline, ApplyDeadline);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nStartTime, StartTime);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nStartTimeToday, StartTimeToday);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nBossInterval, BossInterval);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nRobInterval, RobInterval);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nPointInterval, PointInterval)
		CC_SYNTHESIZE_READONLY(uint32_t, m_nPointsCount, PointsCount);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nStarRateNormal, StarRateNormal);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nStarRateWounded, StarRateWounded);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nEndTime, EndTime);//show settle
		CC_SYNTHESIZE_READONLY(uint32_t, m_nCloseTime, CloseTime);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nAttackCost, AttackCost);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nFightCost, FightCost);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nRescueCost, RescueCost);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nNoticeInterval, NoticeInterval);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nBossCD, BossCD);
		CC_SYNTHESIZE_READONLY(float, m_fAnimationInterval, AnimationInterval);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nNotificationTime, NotitficationTime);
		CC_SYNTHESIZE_READONLY(float, m_fRefreshCD, RefreshCD);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nApplyStartTime, ApplyStartTime);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nBossFreeTimes, BossFreeTimes);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nBossPayedTimes, BossPayedTimes);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nRobFreeTimes, RobFreeTimes);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nRobPayedTimes, RobPayedTimes);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nRobBloodMax, RobBloodMax);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nAngerMax, AngerMax);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sDeclareError, DeclareError);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sBossPeriod, BossPeriod);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sRobPeriod, RobPeriod);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nRankStart, RankStart);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nRankEnd, RankEnd);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sRankError, RankError);
		CC_SYNTHESIZE_READONLY(float, m_fRobPercent, RobPercent);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nRobBonus, RobBonus);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sMailTime, MailTime);
	};
	
	enum GuildBossNoticeType
	{
		GuildBossNoticeTypNone,
		GuildBossNoticeTypeAttack,
		GuildBossNoticeTypeRescue,
		GuildBossNoticeTypeSelfRescued,
	};

	class GuildBossNotice : public ValueObjcet
	{
	public:

		CREATE_FUNC(GuildBossNotice);
		D_INIT_DEFAULT(ValueObjcet);
		void parse(const Json::Value& val);
	protected:
		friend class ::GuildBossModel;
		CC_SYNTHESIZE_READONLY(GuildBossNoticeType, m_eType, Type);
		CC_SYNTHESIZE_READONLY(std::string, m_sUserName, UserName);
		CC_SYNTHESIZE_READONLY(std::string, m_sTargetName, TargetName);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iDamage, Damage);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iRound, Round);
		CC_SYNTHESIZE_READONLY(bool, m_bAuto, IsAuto);
	};
	class GuildBossRobResult : public ValueObjcet
	{
	public:
		GuildBossRobResult();
		CREATE_FUNC(GuildBossRobResult);
		D_INIT_DEFAULT(ValueObjcet);
		void parse(const Json::Value& val);
	protected:
		friend class ::GuildBossModel;
		CC_SYNTHESIZE(int, m_iRobDamage, RobDamage);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDamageOld, DamageOld);
		CC_SYNTHESIZE_READONLY(int, m_iDamage, Damage);
		CC_SYNTHESIZE_READONLY(bool, m_bWin, IsWin);
	};
	class GuildBossAllResult : public ValueObjcet
	{
	public:
		GuildBossAllResult();
		CREATE_FUNC(GuildBossAllResult);
		D_INIT_DEFAULT(ValueObjcet);
		void parse(const Json::Value& val);
	protected:
		friend class ::GuildBossModel;
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDamageAllSelf, DamageAllSelf);
		CC_SYNTHESIZE_READONLY(int, m_nDamageRobSelf, DamageRobSelf);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDate, Date);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDamageGotSelf, DamageGotSelf);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDamageAllGuild, DamageAllGuild);
		CC_SYNTHESIZE_READONLY(int, m_nDamageRobGuild, DamageRobGuild);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDamageGotGuild, DamageGotGuild);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDamageAllTarget, DamageAllTarget);
		CC_SYNTHESIZE_READONLY(int, m_nDamageRobTarget, DamageRobTarget);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDamageGotTarget, DamageGotTarget);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iYesterdayBlood, YesterdayBlood);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sTargetName, TargetName);
	};
	class GuildBossDamage : public ValueObjcet
	{
	public:
		CREATE_FUNC(GuildBossDamage);
		D_INIT_DEFAULT(ValueObjcet);
		void parse(const Json::Value& val);
	protected:
		friend class ::GuildBossModel;
		CC_SYNTHESIZE_READONLY(uint32_t, m_nDamage, Damage);
		CC_SYNTHESIZE_READONLY(uint32_t, m_nCritical, Critical);
	};
	typedef std::map<uint32_t, cocos2d::ccColor3B> ColorMap;
}

class GuildBossModel : public Model
{
public:
	GuildBossModel();
	~GuildBossModel();
	virtual bool init();
#pragma mark GuildBoss
public:
	void loadConfig();
	void parseInformation(const Json::Value &val);
	void parseWoundedUsers(const Json::Value &val);
	void removeWounded(const Json::Value &request);
	void rescueAll();
	void parseTargets(const Json::Value &request, const Json::Value &val);
	void parseMemberRank(const Json::Value &val);
	void parseGuildRank(const Json::Value &request, const Json::Value &val);
	void parseAttackTargets(const Json::Value &val);
	void parseHatredGuilds(const Json::Value &val);
	void parseDamage(const Json::Value &val);
	void parseRobDamage(const Json::Value &val);
	void parseDeclaredTarget(const Json::Value &val);
	void parseNotices(const Json::Value &val);
	void parseRobResult(const Json::Value &val);
	void parseAllResult(const Json::Value &val);
	void parseDamageArray(const Json::Value &val);
	void parseAutoDamage(const Json::Value &val);
	void parseOpenTs(const Json::Value &val);
	void errorNoGuild();
protected:
	CC_SYNTHESIZE_READONLY(guildboss::GuildBossConfig *, m_pConfig, Config);
	CC_SYNTHESIZE_READONLY(guildboss::GuildBoss *, m_pBoss, Boss);
	CC_SYNTHESIZE_READONLY(guildboss::GuildBossUser *, m_pBossSelf, BossSelf);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pWoundedUsers, WoundedUsers);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pTargets, Targets);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pMemberRank, MemberRank);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pRescueRank, RescueRank);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pGuildRank, GuildRank);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pAttackTargets, AttackTargets);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pDeclaredUsers, DeclaredUsers);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pHatredGuilds, HatredGuilds);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pNotices, Notices);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pRescueNotices, RescueNotices);
	CC_SYNTHESIZE_READONLY(guildboss::GuildBossRobResult *, m_pRobResult, RobResult);
	CC_SYNTHESIZE_READONLY(guildboss::GuildBossAllResult *, m_pAllResult, AllResult);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pDamage, Damage);

	CC_SYNTHESIZE_READONLY(bool, m_bIsOpen, IsOpen);
	CC_SYNTHESIZE_READONLY(uint32_t, m_uOpenTs, OpenTs);

	CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<cocos2d::CCPoint>, m_vPositionForDamage, PositionForDamage);
	CC_SYNTHESIZE_READONLY(bool, m_bJoined, Joined);
	CC_SYNTHESIZE(bool, m_bDeclaredShown, DeclaredShown);

	CC_SYNTHESIZE(bool, m_bDragonShowing, DragonShowing);

	CC_SYNTHESIZE(guildboss::GuildBossUserBasic *, m_pRobTarget, RobTarget);

	CC_SYNTHESIZE(bool, m_bSettleData, HasSettleData);
	CC_SYNTHESIZE(bool, m_bNewJoin, IsNewJoin);

	CC_SYNTHESIZE_READONLY_PASS_BY_REF(guildboss::ColorMap, m_mAngerColor, AngerColor);
public:
	int getIndexOfLivingPoint();
	int getCountingDownOfLivingPoint();
	guildboss::GuildBossState getStateOfLivingPoint();
	uint32_t getCountingDownOfBoss();
	uint32_t getCountingDownOfRob();

	int getCountindDownOfDeadLine();
	guildboss::GuildBossNotice *topNoticeRetained();
	guildboss::GuildBossNotice *topRescueNoticeRetained();

	void resetGuildBoss(float dt);

	void syncNotices(float dt);

	void resetStageData();
	void syncSelfInfo();
	void syncWounded(bool force = false);
	void syncMemberRank();
	void syncGuildRank(bool loading = false);
	void syncHatred();
	void syncTargets(bool loading = false);
	void syncSettle();
	void syncAttackTargets();

	void loadPositions(const CSVDecoder::CSV &val);
	void loadColor(const CSVDecoder::CSV &val);
	cocos2d::CCPoint randomPosition();

	bool isShowingLastWeek();

	void addDamageAndRemove();

	void rescued();

	bool checkCanDeclare();

	void resetNoticeSchedule();

	float getPercentOfSelfAnger();
	bool isSelfAngry();
	bool canShowRank();

	bool isSettleSynced() {return m_bSettleSynced;};
private:
	uint32_t m_iStartOfNotice;
	bool m_bSelfInfoSynced;
	bool m_bWoundedSynced;
	bool m_bMemberRankSynced;
	bool m_bGuildRankSynced;
	bool m_bHatredSynced;
	bool m_bTargetsSynced;
	bool m_bSettleSynced;
	bool m_bAttackTargetsSynced;

	uint32_t m_nLastSynced;

	uint32_t m_nNoticeLastSynced;

	void newDaySchedule();
	void newDayCalculate(float dt);
	void checkDeclared();

};
#endif