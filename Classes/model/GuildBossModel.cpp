//
//  GuildBossModel.cpp
//  公会
//
//  Created by Eci on 14-2-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "GuildBossModel.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include <algorithm>
#include "utils/TimerUtil.h"

//#define WEEKDAY_SHOWING_LAST_WEEK 1
#define OTHER_ANIMATION_PLAY_INTV 0.5f
#define DAY_TO_SECOND 86400

using namespace cocos2d;
using namespace guildboss;


static int sortTargetsByDamage(const CCObject* p1, const CCObject* p2)
{
	return ((guildboss::GuildBossUserBasic*)p1)->getDamageForRobSuccess() > ((guildboss::GuildBossUserBasic*)p2)->getDamageForRobSuccess();
};

static int sortMemberWithRescueCount(const CCObject* p1, const CCObject* p2)
{
	return ((guildboss::GuildBossUserBasic*)p1)->getRescueCount() > ((guildboss::GuildBossUserBasic*)p2)->getRescueCount();
}




template<class _Pr> inline
	void stdSortWithCCArray(cocos2d::CCArray *pArr, _Pr _Pred)
{
	std::sort(pArr->data->arr, pArr->data->arr + pArr->data->num, _Pred);
}


namespace guildboss
{
#pragma mark - GuildBoss -

	GuildBossBasic::GuildBossBasic()
		: m_nGuildID(0)
		, m_iGuildFight(0)
		, m_iDamage(0)
		, m_iLastDamage(0)
		, m_nLastRank(0)
		, m_sGuildName()
		, m_iLevel(0)
	{

	}

	void GuildBossBasic::parse(const Json::Value &val)
	{
		DJSON_UINT(m_nGuildID, val, "guildId");
		DJSON_UINT(m_iGuildFight, val, "fight");
		DJSON_UINT(m_iDamage, val, "dmg");
		DJSON_UINT(m_iLastDamage, val, "lastDmg");
		DJSON_UINT(m_nLastRank, val, "rank");
		DJSON_STRING(m_sGuildName, val, "name");
		DJSON_UINT(m_iLevel, val, "lv");
	}

	GuildBoss::GuildBoss()
		: m_nLatestWeekTime(0)
		, m_nTargetID(0)
		, m_nDeclareTime(0)
		, m_sTargetName()
		, m_iDamageDaily(0)
		, m_bDeclaredToday(false)
		, m_nStep(0)
	{

	}

	void GuildBoss::parse(const Json::Value& val)
	{
		GuildBossBasic::parse(val);
		DJSON_UINT(m_nLatestWeekTime, val, "week");
		DJSON_UINT(m_nTargetID, val, "target");
		DJSON_UINT(m_nDeclareTime, val, "declareDate");
		DJSON_STRING(m_sTargetName, val, "targetName");
		DJSON_UINT(m_iDamageDaily, val, "dmgDaily");
		uint32_t nServerTime = GameManager::sharedGameManager()->getServerTime();
		uint32_t nStartOfToday = TimeUtil::getStartTimeOfToday(nServerTime);
		m_bDeclaredToday = nStartOfToday <= m_nDeclareTime;
		DJSON_UINT(m_nStep, val, "step");
	}

	void GuildBoss::addDamage(GuildBossDamage *pDamage)
	{
		m_iDamage += pDamage->getDamage();
	}

	GuildBossUserBasic::GuildBossUserBasic()
		: m_nUserID(0)
		, m_nGuildID(0)
		, m_iUserFight(0)
		, m_iTotalDamage(0)
		, m_iDamageForRob(0)
		, m_iDamageForRobBase(0)
		, m_iLevel(0)
		, m_sGuildName()
		, m_sUserName()
		, m_iLeaderID(0)
		, m_iLeaderDress(0)
		, m_iRobBlood(0)
		, m_uRescueCount(0)
		, m_iDamageDaily(0)
	{

	}

	void GuildBossUserBasic::parse(const Json::Value &val)
	{
		DJSON_UINT(m_nUserID, val, "userId");
		DJSON_UINT(m_nGuildID, val, "guildId");
		DJSON_UINT(m_iUserFight, val, "fight");
		DJSON_UINT(m_iTotalDamage, val, "dmgTotal");
		DJSON_UINT(m_iDamageForRob, val, "dmgForRob");
		DJSON_UINT(m_iDamageForRobBase, val, "dmgForRobBase");

		DJSON_UINT(m_iLevel, val, "userLv");
		DJSON_STRING(m_sGuildName, val, "gname");
		DJSON_STRING(m_sUserName, val, "uname");
		DJSON_UINT(m_iLeaderID, val, "lid");
		DJSON_UINT(m_iLeaderDress, val, "ldid");
		DJSON_UINT(m_iRobBlood, val, "robBlood");
		DJSON_UINT(m_uRescueCount, val, "rescueTimes");
		DJSON_UINT(m_iDamageDaily, val, "dmgDaily");
	}

	GuildBossUser::GuildBossUser()
		: m_iRound(1)
		, m_uAnger(0)
		, m_nLatestWeekTime(0)
		, m_nLatestDateTime(0)
		, m_iCurrentFight(0)
		, m_nAttackTs(0)
		, m_nAvailTimes(0)
		, m_nPayedTimes(0)
		, m_nNextTime(0)
	{

	}

	void GuildBossUser::parse(const Json::Value &val)
	{
		GuildBossUserBasic::parse(val);
		DJSON_UINT(m_nLatestWeekTime, val, "week");
		DJSON_UINT(m_nLatestDateTime, val, "date");
		DJSON_UINT(m_iCurrentFight, val, "curFight");
		DJSON_UINT(m_iRound, val, "round");
		DJSON_UINT(m_nAttackTs, val, "attackTs");
		DJSON_UINT(m_nAvailTimes, val, "availTimes");
		DJSON_UINT(m_nPayedTimes, val, "paidTimes");
		DJSON_UINT(m_uAnger, val, "bonusScores");
		m_nNextTime = 0;
	}

	void GuildBossUser::addDamage(GuildBossDamage *pDamage)
	{
		CCLOG("%u+%u", m_iDamageDaily, pDamage->getDamage());
		m_iDamageDaily += pDamage->getDamage();
	}

	GuildWoundedUser::GuildWoundedUser()
		: m_pBasicInfo(NULL)
	{

	}

	GuildWoundedUser::~GuildWoundedUser()
	{
		CC_SAFE_RELEASE(m_pBasicInfo);
	}

	void GuildBossNotice::parse(const Json::Value& val)
	{
		DJSON_STRING(m_sUserName, val, "name");			// for damage type
		if (val.isMember("args"))
		{
			DJSON_UINT(m_iDamage, val["args"], "dmg");
			uint32_t iType = 0;
			DJSON_UINT(iType, val["args"], "type");
			m_eType = (GuildBossNoticeType)iType;
			DJSON_UINT(m_iRound, val["args"], "round");
			DJSON_STRING(m_sTargetName, val["args"], "rescue_name");	// for rescue type
			DJSON_BOOL(m_bAuto, val["args"], "auto");
		}
	}

	GuildBossRobResult::GuildBossRobResult()
		:m_iRobDamage(-1)
		,m_nDamageOld(0)
		,m_iDamage(0)
	{

	}

	void GuildBossRobResult::parse(const Json::Value& val)
	{
		m_nDamageOld = 0;
		m_iDamage = 0;
		DJSON_UINT(m_nDamageOld, val, "dmgOld");
		DJSON_INT(m_iDamage, val, "dmg");
		m_iRobDamage = -1;
		m_bWin = m_iDamage > 0;
	}

	GuildBossAllResult::GuildBossAllResult()
		: m_nDamageRobSelf(0)
		, m_nDamageGotSelf(0)
		, m_nDamageAllSelf(0)
		, m_nDamageRobGuild(0)
		, m_nDamageGotGuild(0)
		, m_nDamageAllGuild(0)
		, m_nDamageRobTarget(0)
		, m_nDamageGotTarget(0)
		, m_nDamageAllTarget(0)
		, m_nDate(0)
		, m_iYesterdayBlood(0)
		, m_sTargetName()
	{

	}

	void GuildBossAllResult::parse(const Json::Value& val)
	{
		m_nDamageRobSelf = 0;
		m_nDamageGotSelf = 0;
		DJSON_UINT(m_nDate, val["myDmg"], "date");
		uint32_t uServerTime = GameManager::sharedGameManager()->getServerTime();
		if (uServerTime > m_nDate)
		{
			DJSON_INT(m_nDamageRobSelf, val["myDmg"], "robDmgDaily");
			DJSON_UINT(m_nDamageGotSelf, val["myDmg"], "dmgDaily");
			DJSON_UINT(m_iYesterdayBlood, val["myDmg"], "rob_blood");
		}
		m_nDamageAllSelf = m_nDamageGotSelf - m_nDamageRobSelf;
		DJSON_INT(m_nDamageRobGuild, val["myGuild"], "robDmgDaily");
		DJSON_UINT(m_nDamageGotGuild, val["myGuild"], "dmgDaily");
		m_nDamageAllGuild = m_nDamageGotGuild - m_nDamageRobGuild;
		DJSON_INT(m_nDamageRobTarget, val["targetGuild"], "robDmgDaily");
		DJSON_UINT(m_nDamageGotTarget, val["targetGuild"], "dmgDaily");
		m_nDamageAllTarget = m_nDamageGotTarget - m_nDamageRobTarget;
		DJSON_STRING(m_sTargetName, val["targetGuild"], "name");
	}

	void GuildBossDamage::parse(const Json::Value& val)
	{
		DJSON_UINT(m_nDamage, val, "dmg");
		DJSON_UINT(m_nCritical, val, "critical");
	}
}

GuildBossModel::GuildBossModel()
	:m_pConfig(NULL)
	,m_pBoss(NULL)
	,m_pBossSelf(NULL)
	,m_pWoundedUsers(NULL)
	,m_pTargets(NULL)
	,m_pMemberRank(NULL)
	,m_pRescueRank(NULL)
	,m_pGuildRank(NULL)
	,m_pAttackTargets(NULL)
	,m_pDeclaredUsers(NULL)
	,m_pHatredGuilds(NULL)
	,m_pNotices(NULL)
	,m_pRescueNotices(NULL)
	,m_pRobResult(NULL)
	,m_pAllResult(NULL)
	,m_pDamage(NULL)
	,m_iStartOfNotice(0)
	,m_bSelfInfoSynced(false)
	,m_bWoundedSynced(false)
	,m_bMemberRankSynced(false)
	,m_bGuildRankSynced(false)
	,m_bHatredSynced(false)
	,m_bTargetsSynced(false)
	,m_bSettleSynced(false)
	,m_bAttackTargetsSynced(false)
	,m_bJoined(false)
	,m_vPositionForDamage()
	,m_bDeclaredShown(false)
	,m_nLastSynced(0)
	,m_bIsOpen(false)
	,m_bDragonShowing(false)
	,m_pRobTarget(NULL)
	,m_bSettleData(true)
	,m_bNewJoin(false)
	,m_nNoticeLastSynced(0)
{
}

GuildBossModel::~GuildBossModel()
{
	//TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossModel::syncNotices), this);
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossModel::resetGuildBoss), this);
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossModel::newDayCalculate), this);
	CC_SAFE_RELEASE(m_pConfig);
	CC_SAFE_RELEASE(m_pBoss);
	CC_SAFE_RELEASE(m_pBossSelf);
	CC_SAFE_RELEASE(m_pWoundedUsers);
	CC_SAFE_RELEASE(m_pTargets);
	CC_SAFE_RELEASE(m_pMemberRank);
	CC_SAFE_RELEASE(m_pRescueRank);
	CC_SAFE_RELEASE(m_pGuildRank);
	CC_SAFE_RELEASE(m_pAttackTargets);
	CC_SAFE_RELEASE(m_pDeclaredUsers);
	CC_SAFE_RELEASE(m_pHatredGuilds);
	CC_SAFE_RELEASE(m_pNotices);
	CC_SAFE_RELEASE(m_pRescueNotices);
	CC_SAFE_RELEASE(m_pRobResult);
	CC_SAFE_RELEASE(m_pAllResult);
	CC_SAFE_RELEASE(m_pDamage);
}

bool GuildBossModel::init()
{
	CC_SAFE_RELEASE(m_pBoss);
	m_pBoss = GuildBoss::create();
	m_pBoss->m_nGuildID = 0;
	CC_SAFE_RETAIN(m_pBoss);
	CC_SAFE_RELEASE(m_pBossSelf);
	m_pBossSelf = GuildBossUser::create();
	CC_SAFE_RETAIN(m_pBossSelf);

	m_pRobResult = GuildBossRobResult::create();
	CC_SAFE_RETAIN(m_pRobResult);

	loadConfig();
	m_pAllResult = GuildBossAllResult::create();
	CC_SAFE_RETAIN(m_pAllResult);
	m_pAllResult->m_iYesterdayBlood = m_pConfig->m_nRobBloodMax;
	loadPositions(CSVDecoder::create("data/guildboss_damage_position")->getData());
	loadColor(CSVDecoder::create("data/guildboss_anger_color")->getData());

	TimerUtil::sharedTimerUtil()->schedule(schedule_selector(GuildBossModel::resetGuildBoss), this, D_CONFIG_UINT_FOR_KEY(GUILDBOSS_REFRESH_INTERVAL));

	newDaySchedule();

    return Model::init();
}

static void getPartsFromStr(const char *str, int a[], int l)
{
	a[0] = atoi(str);
	int len = strlen(str);
	int index = 1;
	for (int i = 0; (i < len && index < l); i++)
	{
		if (str[i] == '-' || str[i] == ':')
		{
			a[index++] = atoi(str + i + 1);
		}
	}
}

void GuildBossModel::loadConfig()
{
	if (!m_pConfig)
	{
		m_pConfig = GuildBossConfig::create();
		CC_SAFE_RETAIN(m_pConfig);
	}
	uint32_t nServerTime = GameManager::sharedGameManager()->getServerTime();
	const char * sDeadline = D_CONFIG_CSTRING_FOR_KEY(GUILDBOSS_APPLY_DEADLINE);
	int t[3] = {0, 0, 0};
	getPartsFromStr(sDeadline, t, 3);
	m_pConfig->m_nApplyDeadline = TimeUtil::getTimeToday(nServerTime, t[0], t[1]);
	const char * sStartTime = D_CONFIG_CSTRING_FOR_KEY(GUILDBOSS_START_TIME);
	int s[3] = {0, 0, 0};
	getPartsFromStr(sStartTime, s, 3);
	m_pConfig->m_nStartTime = TimeUtil::getTimeToday(nServerTime, s[0], s[1]);
	m_pConfig->m_nBossInterval = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_BOSS_INTERVAL);
	m_pConfig->m_nRobInterval = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_ROB_INTERVAL);
	m_pConfig->m_nPointInterval = m_pConfig->m_nBossInterval + m_pConfig->m_nRobInterval;
	m_pConfig->m_nPointsCount = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_POINTS_COUNT);
	m_pConfig->m_nStarRateNormal = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_STAR_RATE_NORMAL);
	m_pConfig->m_nStarRateWounded = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_STAR_RATE_WOUNDED);
	m_pConfig->m_nEndTime = m_pConfig->m_nStartTime + m_pConfig->m_nPointInterval * m_pConfig->m_nPointsCount;
	const char * sCloseTime = D_CONFIG_CSTRING_FOR_KEY(GUILDBOSS_CLOSE_TIME);
	int c[3] = {0, 0, 0};
	getPartsFromStr(sCloseTime, c, 3);
	m_pConfig->m_nCloseTime = TimeUtil::getTimeToday(nServerTime, c[0], c[1], c[2]);
	m_pConfig->m_nStartTimeToday = m_pConfig->m_nStartTime - TimeUtil::getStartTimeOfToday(nServerTime);
	m_pConfig->m_nAttackCost = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_ATTACK_COST);
	m_pConfig->m_nFightCost = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_FIGHT_COST);
	m_pConfig->m_nRescueCost = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_RESCUE_COST);
	m_pConfig->m_nNoticeInterval = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_NOTICE_INTERVAL);
	m_pConfig->m_nBossCD = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_ATTACK_BOSS_CD);
	m_pConfig->m_fAnimationInterval = D_CONFIG_FLOAT_FOR_KEY(GUILDBOSS_ATTACK_ANIMATION_INTERVAL);
	const char * sNotiTime = D_CONFIG_CSTRING_FOR_KEY(GUILDBOSS_NOTIFICATION_TIME);
	int n[3] = {0, 0, 0};
	getPartsFromStr(sNotiTime, n, 3);
	m_pConfig->m_nNotificationTime = TimeUtil::getTimeToday(nServerTime, n[0], n[1]);
	m_pConfig->m_fRefreshCD = D_CONFIG_FLOAT_FOR_KEY(GUILDBOSS_REFRESH_CD);
	const char * sApplyStart = D_CONFIG_CSTRING_FOR_KEY(GUILDBOSS_APPLY_START_TIME);
	int a[3] = {0, 0, 0};
	getPartsFromStr(sApplyStart, a, 3);
	m_pConfig->m_nApplyStartTime = TimeUtil::getTimeToday(nServerTime, a[0], a[1], a[2]);
	m_pConfig->m_nBossFreeTimes = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_BOSS_FREE_TIMES);
	m_pConfig->m_nBossPayedTimes = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_BOSS_PAYED_TIMES);
	m_pConfig->m_nRobFreeTimes = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_ROB_FREE_TIMES);
	m_pConfig->m_nRobPayedTimes = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_ROB_PAYED_TIMES);
	m_pConfig->m_nRobBloodMax = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_ROB_BLOOD_MAX);
	m_pConfig->m_nAngerMax = D_CONFIG_UINT_FOR_KEY(GUILDBOSS_ANGER_MAX);
	m_pConfig->m_sDeclareError = D_LOCAL_STRING("GuildBossCannotDeclare"/*, sApplyStart, sDeadline*/);
	m_pConfig->m_sBossPeriod = D_CONFIG_CSTRING_FOR_KEY(GUILDBOSS_BOSS_PERIOD);
	m_pConfig->m_sRobPeriod = D_CONFIG_CSTRING_FOR_KEY(GUILDBOSS_ROB_PERIOD);
	const char * sRankStart = D_CONFIG_CSTRING_FOR_KEY(GUILDBOSS_RANK_START);
	int rs[3] = {0, 0, 0};
	getPartsFromStr(sRankStart, rs, 3);
	m_pConfig->m_nRankStart = TimeUtil::getTimeToday(nServerTime, rs[0], rs[1], rs[2]);
	const char * sRankEnd = D_CONFIG_CSTRING_FOR_KEY(GUILDBOSS_RANK_END);
	int re[3] = {0, 0, 0};
	getPartsFromStr(sRankEnd, re, 3);
	m_pConfig->m_nRankEnd = TimeUtil::getTimeToday(nServerTime, re[0], re[1], re[2]);
	m_pConfig->m_sRankError = D_LOCAL_STRING("%sGuildBossRankError%s", sRankStart, sRankEnd);
	m_pConfig->m_fRobPercent = D_CONFIG_FLOAT_FOR_KEY(GUILDBOSS_ROB_PERCENT);
	m_pConfig->m_nRobBonus = D_CONFIG_FLOAT_FOR_KEY(GUILDBOSS_ROB_BONUS);
	m_pConfig->m_sMailTime = D_CONFIG_CSTRING_FOR_KEY(GUILDBOSS_MAIL_TIME);
}

#pragma mark GuildBoss

void GuildBossModel::parseWoundedUsers(const Json::Value &val)
{
	if (NULL == m_pWoundedUsers)
	{
		m_pWoundedUsers = CCArray::create();
		CC_SAFE_RETAIN(m_pWoundedUsers);
	}
	else
	{
		m_pWoundedUsers->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		GuildWoundedUser *pUser = GuildWoundedUser::create();
		DJSON_UINT(pUser->m_nUserID, (*i), "uid");
		if (pUser->m_nUserID == m_pBossSelf->getUserID())
		{
			continue;
		}
		DJSON_UINT(pUser->m_iFight, (*i), "fight");
		m_pWoundedUsers->addObject(pUser);
		for (uint32_t j = 0; j < m_pMemberRank->count(); j++)
		{
			GuildBossUserBasic *pBasic = (GuildBossUserBasic *)m_pMemberRank->objectAtIndex(j);
			if (pUser->m_nUserID == pBasic->m_nUserID)
			{
				pUser->setBasicInfo(pBasic);
				break;
			}
		}
	}
	m_bWoundedSynced = true;
	postNotification(D_EVENT_GUILDBOSS_WOUNDED_LIST);
}

void GuildBossModel::removeWounded(const Json::Value &request)
{
	bool isAll = request["all"].asBool();
	if (isAll)
	{
		rescueAll();
		return;
	}
	uint32_t userID = request["wounded"].asUInt();
	for (uint32_t i = 0; i < m_pWoundedUsers->count(); i++)
	{
		GuildWoundedUser *pUser = (GuildWoundedUser *)m_pWoundedUsers->objectAtIndex(i);
		if (pUser->getUserID() == userID)
		{
			m_pWoundedUsers->removeObjectAtIndex(i);
			break;
		}
	}
	postNotification(D_EVENT_GUILDBOSS_WOUNDED_LIST);
}

void GuildBossModel::rescueAll()
{
	m_pWoundedUsers->removeAllObjects();
	postNotification(D_EVENT_GUILDBOSS_WOUNDED_LIST);
}

void GuildBossModel::parseInformation(const Json::Value &val)
{
	parseOpenTs(val);
	m_pBoss->parse(val["guildBossDetail"]);
	m_pBossSelf->parse(val["guildBossUserDetail"]);
	m_bSelfInfoSynced = true;
	isSynced = true;
	m_bJoined = true;
	m_nLastSynced = GameManager::sharedGameManager()->getServerTime();
	checkDeclared();
	postNotification(D_EVENT_GUILDBOSS_REFRESH);
}

void GuildBossModel::parseTargets(const Json::Value &request, const Json::Value &val)
{
	if (NULL == m_pTargets)
	{
		m_pTargets = CCArray::create();
		CC_SAFE_RETAIN(m_pTargets);
	}
	if (!request["id"].asUInt())
	{
		m_pTargets->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		GuildBossBasic *pGuild = GuildBossBasic::create();
		pGuild->parse(*i);
		m_pTargets->addObject(pGuild);
	}
	m_bTargetsSynced = true;
	postNotification(D_EVENT_GUILDBOSS_GET_TARGETS);
}

void GuildBossModel::parseMemberRank(const Json::Value &val)
{
	if (NULL == m_pMemberRank)
	{
		m_pMemberRank = CCArray::create();
		CC_SAFE_RETAIN(m_pMemberRank);
	}
	else
	{
		m_pMemberRank->removeAllObjects();
	}
	if (NULL == m_pRescueRank)
	{
		m_pRescueRank = CCArray::create();
		CC_SAFE_RETAIN(m_pRescueRank);
	}
	else
	{
		m_pRescueRank->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		GuildBossUserBasic *pUser = GuildBossUserBasic::create();
		pUser->parse(*i);
		m_pMemberRank->addObject(pUser);
	}
	m_pRescueRank->addObjectsFromArray(m_pMemberRank);
	stdSortWithCCArray(m_pRescueRank, sortMemberWithRescueCount);
	if (m_pRescueRank->count())
	{
		uint32_t iRank = 1;
		GuildBossUserBasic *pLastUser = (GuildBossUserBasic *)m_pRescueRank->objectAtIndex(0);
		pLastUser->m_nRescueRank = 1;
		for (int i = 1; i < m_pRescueRank->count(); i++)
		{
			GuildBossUserBasic *pUser = (GuildBossUserBasic *)m_pRescueRank->objectAtIndex(i);
			if (pUser->m_uRescueCount != pLastUser->m_uRescueCount)
			{
				iRank = i + 1;
			}
			pUser->m_nRescueRank = iRank;
			pLastUser = pUser;
		}
	}
	m_bMemberRankSynced = true;
	postNotification(D_EVENT_GUILDBOSS_GET_RANK);
}

void GuildBossModel::parseGuildRank(const Json::Value &request, const Json::Value &val)
{
	if (NULL == m_pGuildRank)
	{
		m_pGuildRank = CCArray::create();
		CC_SAFE_RETAIN(m_pGuildRank);
	}
	if (!request["id"].asUInt())
	{
		m_pGuildRank->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		GuildBossBasic *pUser = GuildBossBasic::create();
		pUser->parse(*i);
		m_pGuildRank->addObject(pUser);
	}
	m_bGuildRankSynced = true;
	postNotification(D_EVENT_GUILDBOSS_GET_RANK);
}

void GuildBossModel::parseAttackTargets(const Json::Value &val)
{
	if (NULL == m_pAttackTargets)
	{
		m_pAttackTargets = CCArray::create();
		CC_SAFE_RETAIN(m_pAttackTargets);
	}
	else
	{
		m_pAttackTargets->removeAllObjects();
	}
	if (NULL == m_pDeclaredUsers)
	{
		m_pDeclaredUsers = CCArray::create();
		CC_SAFE_RETAIN(m_pDeclaredUsers);
	}
	else
	{
		m_pDeclaredUsers->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		GuildBossUserBasic *pUser = GuildBossUserBasic::create();
		pUser->parse(*i);

		pUser->m_iDamageForRobSuccess = m_pConfig->m_nRobBonus;
		if (pUser->m_iDamageForRob >= pUser->m_iDamageForRobBase * m_pConfig->m_fRobPercent)
		{
			pUser->m_iDamageForRobSuccess += pUser->m_iDamageForRobBase * m_pConfig->m_fRobPercent;
		}

		m_pAttackTargets->addObject(pUser);
		if (pUser->getGuildID() == m_pBoss->getTargetID())
		{
			m_pDeclaredUsers->addObject(pUser);
		}
	}
	m_bAttackTargetsSynced = true;
	stdSortWithCCArray(m_pAttackTargets, sortTargetsByDamage);
	stdSortWithCCArray(m_pDeclaredUsers, sortTargetsByDamage);
	postNotification(D_EVENT_GUILDBOSS_FIGHT_LIST);
}

void GuildBossModel::parseHatredGuilds(const Json::Value &val)
{
	if (NULL == m_pHatredGuilds)
	{
		m_pHatredGuilds = CCArray::create();
		CC_SAFE_RETAIN(m_pHatredGuilds);
	}
	else
	{
		m_pHatredGuilds->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		GuildHatred *pGuild = GuildHatred::create();
		DJSON_UINT(pGuild->m_nGuildID, (*i), "guild_id");
		DJSON_STRING(pGuild->m_sGuildName, (*i), "name");
		DJSON_UINT(pGuild->m_iDamageForRob, (*i), "dmg");
		m_pHatredGuilds->addObject(pGuild);
	}
	m_bHatredSynced = true;
	postNotification(D_EVENT_GUILDBOSS_GET_RANK);
}

void GuildBossModel::parseDamage(const Json::Value &val)
{
	DJSON_UINT(m_pBossSelf->m_nNextTime, val, "cdEnd");
	DJSON_UINT(m_pBossSelf->m_iRobBlood, val["guildBossUserDetail"], "robBlood");
	DJSON_UINT(m_pBossSelf->m_iDamageForRobBase, val["guildBossUserDetail"], "dmgForRobBase");
	DJSON_UINT(m_pBossSelf->m_uAnger, val["guildBossUserDetail"], "bonusScores");
	CCLOG("%u", m_pBossSelf->m_uAnger);
	parseDamageArray(val["dmgObj"]);
	if (m_pBossSelf->m_nAvailTimes > 0)
	{
		m_pBossSelf->m_nAvailTimes -= 1;
	}
	else
	{
		m_pBossSelf->m_nPayedTimes -= 1;
	}
	postNotification(D_EVENT_GUILDBOSS_ATTACK);
}

void GuildBossModel::parseRobDamage(const Json::Value &val)
{
	DJSON_INT(m_pRobResult->m_iRobDamage, val, "robDmg");
	DJSON_UINT(m_pBossSelf->m_nNextTime, val, "cdEnd");
	DJSON_UINT(m_pBossSelf->m_iRobBlood, val, "robBlood");
	bool isWin = false;
	DJSON_BOOL(isWin, val, "isWin");
	if (m_pRobTarget)
	{
		if (isWin && m_pRobTarget->m_iRobBlood)
		{
			m_pRobTarget->m_iRobBlood -= 1;
		}
		DJSON_UINT(m_pRobTarget->m_iDamageForRob, val, "DmgForRob");
		m_pRobTarget = NULL;
	}
	if (m_pBossSelf->m_nAvailTimes > 0)
	{
		m_pBossSelf->m_nAvailTimes -= 1;
	}
	else
	{
		m_pBossSelf->m_nPayedTimes -= 1;
	}
}

void GuildBossModel::parseDeclaredTarget(const Json::Value &val)
{
	m_pBoss->m_nTargetID = val["guildId"].asUInt();
	m_pBoss->m_nDeclareTime = GameManager::sharedGameManager()->getServerTime();
	m_pBoss->m_bDeclaredToday = true;
	for (uint32_t i = 0; i < m_pTargets->count(); i++)
	{
		GuildBossBasic *pGuild = (GuildBossBasic *)m_pTargets->objectAtIndex(i);
		if (pGuild->getGuildID() == m_pBoss->m_nTargetID)
		{
			m_pBoss->m_sTargetName = pGuild->getGuildName();
			break;
		}
	}
	m_bHatredSynced = false;
	postNotification(D_EVENT_GUILDBOSS_REFRESH);
}

void GuildBossModel::parseNotices(const Json::Value &val)
{
	if (NULL == m_pNotices)
	{
		m_pNotices = CCArray::create();
		CC_SAFE_RETAIN(m_pNotices);
	}
	else
	{
		m_pNotices->removeAllObjects();
	}
	if (NULL == m_pRescueNotices)
	{
		m_pRescueNotices = CCArray::create();
		CC_SAFE_RETAIN(m_pRescueNotices);
	}
	else
	{
		m_pRescueNotices->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		GuildBossNotice *pNotice = GuildBossNotice::create();
		pNotice->parse((*i));
		m_iStartOfNotice++;
		GuildBossNoticeType eType = pNotice->getType();
		if (pNotice->getUserName().compare(m_pBossSelf->getUserName()) == 0 && GuildBossNoticeTypeAttack == eType)
		{
			continue;
		}
		if (GuildBossNoticeTypeRescue == eType)
		{
			m_pRescueNotices->addObject(pNotice);
		}
		else
		{
			m_pNotices->addObject(pNotice);
		}
		if (pNotice->getUserName().compare(m_pBossSelf->getUserName()) == 0  && GuildBossNoticeTypeRescue == eType)
		{
			pNotice->m_eType = GuildBossNoticeTypeSelfRescued;
		}
	}
	m_nNoticeLastSynced = GameManager::sharedGameManager()->getServerTime();
	resetNoticeSchedule();
	if (m_pNotices->count() || m_pRescueNotices->count())
	{
		postNotification(D_EVENT_GUILDBOSS_NOTICE);
	}
}

void GuildBossModel::parseRobResult(const Json::Value &val)
{
	m_pRobResult->parse(val);
	DJSON_UINT(m_pBossSelf->m_iRobBlood, val, "robBlood");
	m_pBossSelf->m_iDamageForRobBase = 0;
	postNotification(D_EVENT_GUILDBOSS_REFRESH);
	postNotification(D_EVENT_GUILDBOSS_ROB_SETTLE);
}

void GuildBossModel::parseAllResult(const Json::Value &val)
{
	m_pAllResult->parse(val);
	
	m_bSettleSynced = true;
	postNotification(D_EVENT_GUILDBOSS_ALL_SETTLE);
}

void GuildBossModel::parseAutoDamage(const Json::Value &val)
{
	if (m_pBossSelf->m_uAnger >= m_pConfig->m_nAngerMax)
	{
		m_pBossSelf->m_uAnger -= m_pConfig->m_nAngerMax;
	}
	else
	{
		m_pBossSelf->m_uAnger = 0;
	}
	if (val.isMember("guildBossUserDetail"))
	{
		DJSON_UINT(m_pBossSelf->m_uAnger, val["guildBossUserDetail"], "bonusScores");
	}
	parseDamageArray(val["dmgObj"]);
	postNotification(D_EVENT_GUILDBOSS_AUTO_ATTACK);
}

void GuildBossModel::parseDamageArray(const Json::Value &val)
{
	if (NULL == m_pDamage)
	{
		m_pDamage = CCArray::create();
		CC_SAFE_RETAIN(m_pDamage);
	}
	else
	{
		m_pDamage->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		GuildBossDamage *pDamage = GuildBossDamage::create();
		pDamage->parse(*i);
		m_pDamage->addObject(pDamage);
	}

}

void GuildBossModel::parseOpenTs(const Json::Value &val)
{
	DJSON_BOOL(m_bIsOpen, val, "bossOpen");
	DJSON_UINT(m_uOpenTs, val, "bossOpenTs");
}

void GuildBossModel::errorNoGuild()
{
	postNotification(D_EVENT_GUILDBOSS_ERROR_NO_GUILD);
}

int GuildBossModel::getIndexOfLivingPoint()
{
	if (m_pBoss->m_nStep > 0)
	{
		return m_pBoss->m_nStep - 1;
	}
	return 6;
	//return (m_pBossSelf->m_iRound - 1) / 2;
}

GuildBossState GuildBossModel::getStateOfLivingPoint()
{
	uint32_t uServerTime = GameManager::sharedGameManager()->getServerTime();
	if (!m_bIsOpen || uServerTime < m_pConfig->m_nApplyDeadline || uServerTime > m_pConfig->m_nEndTime)
	{
		return GuildBossStateClose;
	}
	else if (uServerTime < m_pConfig->m_nStartTime)
	{
		return GuildBossStatePreparing;
	}
	uint32_t uBossCountDown = this->getCountingDownOfBoss();
	if (uBossCountDown > 0)
	{
		return GuildBossStateBoss;
	}
	else
	{
		return GuildBossStateRob;
	}
}

uint32_t GuildBossModel::getCountingDownOfBoss()
{
	uint32_t uServerTime = GameManager::sharedGameManager()->getServerTime();
	if (uServerTime > m_pConfig->m_nStartTime + m_pConfig->m_nBossInterval)
	{
		return 0;
	}
	return m_pConfig->m_nStartTime + m_pConfig->m_nBossInterval - uServerTime;
}

uint32_t GuildBossModel::getCountingDownOfRob()
{
	uint32_t uServerTime = GameManager::sharedGameManager()->getServerTime();
	if (uServerTime > m_pConfig->m_nEndTime)
	{
		return 0;
	}
	return m_pConfig->m_nEndTime - uServerTime;
}

int GuildBossModel::getCountingDownOfLivingPoint()
{
	uint32_t nServerTime = GameManager::sharedGameManager()->getServerTime();
	if (nServerTime < m_pConfig->m_nStartTime)
	{
		return m_pConfig->m_nStartTime - nServerTime;
	}
	else if (nServerTime > m_pConfig->m_nEndTime)
	{
		return m_pConfig->m_nStartTime + DAY_TO_SECOND - nServerTime;
	}
	return 0;
}

int GuildBossModel::getCountindDownOfDeadLine()
{
	// fixme to change name
	uint32_t nServerTime = GameManager::sharedGameManager()->getServerTime();
	if (nServerTime < m_pConfig->getStartTime())
	{
		return m_pConfig->getStartTime() - nServerTime;
	}
	uint32_t nDeadLineOfTomorrow = m_pConfig->getStartTime() + 86400;
	if (nServerTime > m_pConfig->getCloseTime())
	{
		return nDeadLineOfTomorrow - nServerTime;
	}
	return -1;
}

guildboss::GuildBossNotice *GuildBossModel::topNoticeRetained()
{
	if (!m_pNotices || !m_pNotices->count())
	{
		//syncNotices(0.f);
		return NULL;
	}
	guildboss::GuildBossNotice *pNotice = (guildboss::GuildBossNotice *)m_pNotices->objectAtIndex(0);
	CC_SAFE_RETAIN(pNotice);
	m_pNotices->removeObjectAtIndex(0);
	//m_pBoss->m_iDamage += pNotice->m_iDamage;
	return pNotice;
}

guildboss::GuildBossNotice *GuildBossModel::topRescueNoticeRetained()
{
	if (!m_pRescueNotices || !m_pRescueNotices->count())
	{
		//syncNotices(0.f);
		return NULL;
	}
	guildboss::GuildBossNotice *pNotice = (guildboss::GuildBossNotice *)m_pRescueNotices->objectAtIndex(0);
	CC_SAFE_RETAIN(pNotice);
	m_pRescueNotices->removeObjectAtIndex(0);
	//m_pBoss->m_iDamage += pNotice->m_iDamage;
	return pNotice;
}

void GuildBossModel::syncNotices(float dt)
{
	uint32_t uNow = GameManager::sharedGameManager()->getServerTime();
	if (uNow - m_nNoticeLastSynced >= m_pConfig->m_nNoticeInterval)
	{
		HTTP_CLIENT->guildBossGetNotice(m_pBoss->getGuildID() , m_nNoticeLastSynced);
	}
}

void GuildBossModel::resetStageData()
{
	m_pRobResult->m_iRobDamage = -1;
	m_bSelfInfoSynced = false;
	m_bWoundedSynced = false;
	m_bMemberRankSynced = false;
	m_bGuildRankSynced = false;
	m_bHatredSynced = false;
	m_bAttackTargetsSynced = false;
	m_iStartOfNotice = 0;
	if (m_pDamage)
	{
		m_pDamage->removeAllObjects();
	}
	if (m_pNotices)
	{
		m_pNotices->removeAllObjects();
	}
	if (m_pRescueNotices)
	{
		m_pRescueNotices->removeAllObjects();
	}
	GuildBossState eState = getStateOfLivingPoint();
	m_pBossSelf->m_nAvailTimes = 0;
	m_pBossSelf->m_nPayedTimes = 0;
	if (eState == GuildBossStateBoss)
	{
		m_pBossSelf->m_nAvailTimes = m_pConfig->m_nBossFreeTimes;
		m_pBossSelf->m_nPayedTimes = m_pConfig->m_nBossPayedTimes;
	}
	else if (eState == GuildBossStateRob)
	{
		m_pBossSelf->m_nAvailTimes = m_pConfig->m_nRobFreeTimes;
		m_pBossSelf->m_nPayedTimes = m_pConfig->m_nRobPayedTimes;
	}
	m_bSettleSynced = false;
	CCLOG("resetStageData");
}

void GuildBossModel::syncSelfInfo()
{
// 	if (m_pBossSelf->m_iRound % 2 != getStateOfNextPoint()) //round % 2 = 1 for boss, 0 for rob -------- getStateOfNextPoint = 0 for boss, 1 for rob
// 	{
// 		m_bSelfInfoSynced = true;
// 	}
// 	else
// 	{
// 		m_bSelfInfoSynced = false;
	// 	}
	guildboss::GuildBossState eState = getStateOfLivingPoint();
	switch (eState)
	{
	case GuildBossStateBoss:
		{
			if (m_pBossSelf->m_iRound != 1)
			{
				m_bSelfInfoSynced = false;
			}
			break;
		}
	case GuildBossStateRob:
		{
			if (m_pBossSelf->m_iRound != 2)
			{
				m_bSelfInfoSynced = false;
			}
			break;
		}
	default:
		{
			if (m_pBossSelf->m_iRound != 0)
			{
				m_bSelfInfoSynced = false;
			}
			break;
		}
	}
	if (!m_bSelfInfoSynced)
	{
		HTTP_CLIENT->guildBossGet();
	}
	else
	{
		postNotification(D_EVENT_GUILDBOSS_REFRESH);
	}
}

void GuildBossModel::syncWounded(bool force /* = false */)
{
	if (!m_bWoundedSynced || force)
	{
		HTTP_CLIENT->guildBossGetWoundedList(m_pBoss->getGuildID());
	}
	else
	{
		postNotification(D_EVENT_GUILDBOSS_WOUNDED_LIST);
	}
}

void GuildBossModel::syncMemberRank()
{
	if (!m_bMemberRankSynced)
	{
		HTTP_CLIENT->guildBossMemberRank(m_pBoss->getGuildID());
	}
	else
	{
		postNotification(D_EVENT_GUILDBOSS_GET_RANK);
	}
}

void GuildBossModel::syncGuildRank(bool loading /* = false */)
{
	if (!m_bGuildRankSynced)
	{
		HTTP_CLIENT->guildBossGuildRank(m_pBoss->getGuildID(), 0);
	}
	else if (loading)
	{
		HTTP_CLIENT->guildBossGuildRank(m_pBoss->getGuildID(), m_pGuildRank->count());
	}
	else
	{
		postNotification(D_EVENT_GUILDBOSS_GET_RANK);
	}
}

void GuildBossModel::syncHatred()
{
	if (!m_bHatredSynced)
	{
		HTTP_CLIENT->guildBossGetHatredList(m_pBoss->getGuildID());
	}
	else
	{
		postNotification(D_EVENT_GUILDBOSS_GET_RANK);
	}
}

void GuildBossModel::resetGuildBoss(float dt)
{
	isSynced = false;
	m_bTargetsSynced = false;
}

void GuildBossModel::syncTargets(bool loading /* = false */)
{
	if (!m_bTargetsSynced)
	{
		HTTP_CLIENT->guildBossDeclareTargets(m_pBoss->getGuildID(), 0);
	}
	else if (loading)
	{
		HTTP_CLIENT->guildBossDeclareTargets(m_pBoss->getGuildID(), m_pTargets->count());
	}
	else
	{
		postNotification(D_EVENT_GUILDBOSS_GET_TARGETS);
	}
}

void GuildBossModel::syncSettle()
{
	if (!m_bSettleSynced)
	{
		HTTP_CLIENT->guildBossDailySettle();
	}
	else
	{
		postNotification(D_EVENT_GUILDBOSS_ALL_SETTLE);
	}
}

void GuildBossModel::syncAttackTargets()
{
	if (!m_bAttackTargetsSynced)
	{
		HTTP_CLIENT->guildBossGetTargetList(m_pBoss->getGuildID());
	}
	else
	{
		postNotification(D_EVENT_GUILDBOSS_FIGHT_LIST);
	}
}

void GuildBossModel::loadPositions(const CSVDecoder::CSV &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		int posX = atoi(val[i][1].c_str());
		int posY = atoi(val[i][2].c_str());
		m_vPositionForDamage.push_back(CCPointMake(posX, posY));
	}
}

void GuildBossModel::loadColor(const CSVDecoder::CSV &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		int percentage = atoi(val[i][0].c_str());
		int r = atoi(val[i][1].c_str());
		int g = atoi(val[i][2].c_str());
		int b = atoi(val[i][3].c_str());
		m_mAngerColor[percentage] = ccc3(r, g, b);
	}
}

cocos2d::CCPoint GuildBossModel::randomPosition()
{
	int randIdx = CCRANDOM_0_1() * (m_vPositionForDamage.size() - 1);
	return m_vPositionForDamage[randIdx];
}

bool GuildBossModel::isShowingLastWeek()
{
	uint32_t uServerTime = GameManager::sharedGameManager()->getServerTime();
	return getIndexOfLivingPoint() == 0 && uServerTime < m_pConfig->m_nApplyDeadline;
}

void GuildBossModel::addDamageAndRemove()
{
	if (!m_pDamage || !m_pDamage->count())
	{
		return;
	}
	guildboss::GuildBossDamage *pDamage = (guildboss::GuildBossDamage *)m_pDamage->objectAtIndex(0);
	if (!pDamage)
	{
		return;
	}
	m_pBoss->addDamage(pDamage);
	m_pBossSelf->addDamage(pDamage);
	m_pDamage->removeObjectAtIndex(0);
	//postNotification(D_EVENT_GUILDBOSS_ADD_DAMAGE);
}

void GuildBossModel::newDaySchedule()
{
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossModel::newDayCalculate), this);
	uint32_t now = GameManager::sharedGameManager()->getServerTime();
	uint32_t newDayDelay = TimeUtil::getStartTimeOfTomorrow(now) - now;

	TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(GuildBossModel::newDayCalculate), this, newDayDelay);
}

void GuildBossModel::newDayCalculate(float dt)
{
	m_pBoss->m_nStep += 1;
	m_pBoss->m_nStep %= 7;
	m_bJoined = false;
	loadConfig();
	if (!m_bIsOpen)
	{
		if (m_uOpenTs <= TimeUtil::getStartTimeOfToday(GameManager::sharedGameManager()->getServerTime()))
		{
			m_bIsOpen = true;
		}
	}
	checkDeclared();

	newDaySchedule();

	resetStageData();

	m_bSettleData = true;
	m_bNewJoin = false;

	postNotification(D_EVENT_GUILDBOSS_REFRESH);
}

void GuildBossModel::checkDeclared()
{
	uint32_t nServerTime = GameManager::sharedGameManager()->getServerTime();
	uint32_t nStartOfToday = TimeUtil::getStartTimeOfToday(nServerTime);
	m_pBoss->m_bDeclaredToday = nStartOfToday <= m_pBoss->m_nDeclareTime;
	if (nServerTime > m_pConfig->m_nApplyDeadline)
	{
		m_pBoss->m_bDeclaredToday = true;
	}
// 	uint32_t nServerTime = GameManager::sharedGameManager()->getServerTime();
// 	uint32_t nCloseOfYesterday = m_pConfig->m_nCloseTime - 86400;
// 	uint32_t nDeadLineOfTomorrow = m_pConfig->m_nApplyDeadline + 86400;
// 	if (nServerTime < m_pConfig->m_nApplyDeadline)
// 	{
// 		m_pBoss->m_bDeclaredToday = nCloseOfYesterday <= m_pBoss->m_nDeclareTime && m_pConfig->m_nApplyDeadline > m_pBoss->m_nDeclareTime;
// 	}
// 	else if (nServerTime < m_pConfig->m_nCloseTime)
// 	{
// 		m_pBoss->m_bDeclaredToday = true;
// 	}
// 	else
// 	{
// 		m_pBoss->m_bDeclaredToday = nDeadLineOfTomorrow > m_pBoss->m_nDeclareTime && m_pConfig->m_nCloseTime <= m_pBoss->m_nDeclareTime;
// 	}
// 	m_pBoss->m_bDeclaredToday |= nServerTime > m_pConfig->getApplyDeadline() && nServerTime < m_pConfig->m_nCloseTime;
}

void GuildBossModel::rescued()
{
	m_pBossSelf->m_iRobBlood = m_pConfig->m_nRobBloodMax;
}

bool GuildBossModel::checkCanDeclare()
{
	uint32_t uServerTime = GameManager::sharedGameManager()->getServerTime();
	return uServerTime < m_pConfig->getApplyDeadline() && uServerTime >= m_pConfig->getApplyStartTime();
}

void GuildBossModel::resetNoticeSchedule()
{
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossModel::syncNotices), this);
	uint32_t noticeCount = 0;
	if (m_pNotices)
	{
		noticeCount = m_pNotices->count();
	}
	uint32_t uDelay = OTHER_ANIMATION_PLAY_INTV * noticeCount;
	if (uDelay < m_pConfig->m_nNoticeInterval)
	{
		uDelay = m_pConfig->m_nNoticeInterval;
	}
	if (this->getDragonShowing())
	{
		TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(GuildBossModel::syncNotices), this, uDelay);
	}
}

float GuildBossModel::getPercentOfSelfAnger()
{
	if (m_pBossSelf->m_uAnger > m_pConfig->m_nAngerMax)
	{
		return 1.f;
	}
	return (float)m_pBossSelf->m_uAnger / m_pConfig->m_nAngerMax;
}

bool GuildBossModel::isSelfAngry()
{
	return m_pBossSelf->m_uAnger >= m_pConfig->m_nAngerMax;
}

bool GuildBossModel::canShowRank()
{
	uint32_t uNow = GameManager::sharedGameManager()->getServerTime();
	return (uNow < m_pConfig->m_nRankStart || uNow > m_pConfig->m_nRankEnd);
}
