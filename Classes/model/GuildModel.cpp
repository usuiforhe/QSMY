//
//  GuildModel.cpp
//  公会
//
//  Created by Eci on 13-10-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "GuildModel.h"
#include "../network/HttpClient.h"
#include "../managers/GameManager.h"
#include <algorithm>
#include "utils/TimerUtil.h"

using namespace cocos2d;
using namespace guild;


static int sortApplicationsByFight(const CCObject* p1, const CCObject* p2)
{
	return ((guild::GuildApplication*)p1)->getFight() > ((guild::GuildApplication*)p2)->getFight();
};


static int sortUserByContribution(const CCObject *p1, const CCObject *p2)
{
	return ((guild::GuildUserBasics*)p1)->getContribution() > ((guild::GuildUserBasics*)p2)->getContribution();
}

static int sortLogByTime(const CCObject *p1, const CCObject *p2)
{
	return ((guild::GuildLog*)p1)->getTs() > ((guild::GuildLog*)p2)->getTs();
}

static int sortUserByContributionLevelAndLevel(const CCObject *p1, const CCObject *p2)
{
	guild::GuildUserBasics *user1 = (guild::GuildUserBasics *)p1;
	guild::GuildUserBasics *user2 = (guild::GuildUserBasics *)p2;
	if (0 == user1->getKnighthoodLevel())
	{
		return 1;
	}
	else if (0 == user2->getKnighthoodLevel())
	{
		return 0;
	}
	else if (user1->getKnighthoodLevel() == user2->getKnighthoodLevel())
	{
		return user1->getLevel() > user2->getLevel();
	}
	else
	{
		return user1->getKnighthoodLevel() > user2->getKnighthoodLevel();
	}
}

namespace guild
{
     GuildLog::GuildLog()
    :m_id(0)
    ,m_ts(0)
    ,m_type(0)
    ,m_params(NULL)
    {
        
    }
    
     GuildLog::~GuildLog(){
         CC_SAFE_RELEASE(m_params);
    }
    
    void GuildLog::parse(const Json::Value& val)
	{
		DJSON_UINT(m_id, val, "id");
		DJSON_UINT(m_ts, val, "ts");
		DJSON_UINT(m_type, val, "type");
        
        switch (m_type)
        {
            case GuildJoin:
                m_params = GuildLogJoin::create();
                break;
            case GuildOut:
                m_params = GuildLogOut::create();
                break;
            case GuildKick:
                m_params = GuildLogKick::create();
                break;
            case GuildUp:
                m_params = GuildLogUp::create();
                break;
            case GuildChangeName:
                m_params = GuildLogChangeName::create();
                break;
            case GuildDragon:
                m_params = GuildLogDragon::create();
                break;
            case GuildUpKing:
                m_params = GuildLogUpKing::create();
                break;
            case GuildUpKnight:
                m_params = GuildLogUpKnight::create();
                break;
            default:
                break;
        }
        if(m_params){
            CC_SAFE_RETAIN(m_params);
            m_params->parse(val["params"]);
        }
	}

    void GuildLogJoin::parse(const Json::Value& val){
        DJSON_STRING(m_new, val, "new");
    }
    void GuildLogOut::parse(const Json::Value& val){
        DJSON_STRING(m_out, val, "out");
    }
    void GuildLogKick::parse(const Json::Value& val){
        DJSON_STRING(m_out, val, "out");
        DJSON_STRING(m_act, val, "act");
    }
    void GuildLogUp::parse(const Json::Value& val){
        DJSON_UINT(m_newL,val ,"newLv");
    }
    void GuildLogChangeName::parse(const Json::Value& val){
        DJSON_STRING(m_old, val, "old");
        DJSON_STRING(m_new, val, "new");
    }
    void GuildLogDragon::parse(const Json::Value& val){
        DJSON_UINT(m_rank,val ,"rank");
    }
    void GuildLogUpKing::parse(const Json::Value& val){
        DJSON_UINT(m_newL,val ,"newLv");
    }
    void GuildLogUpKnight::parse(const Json::Value& val){
        DJSON_UINT(m_newL,val ,"newLv");
    }
    
    
	void GuildBasics::parse(const Json::Value& val)
	{
		DJSON_UINT(m_nGuildID, val, "id");
		DJSON_STRING(m_sName, val, "name");
		DJSON_UINT(m_iUserCount, val, "num");
		DJSON_UINT(m_iLevel, val, "lv");
		m_isApplied = false;
	}
	void GuildUserBasics::parse(const Json::Value& val)
	{
		m_iContribution = 0;
		m_iRemainContribution = 0;
		DJSON_UINT(m_nUserId, val, "uid");
		DJSON_STRING(m_sName, val, "name");
		DJSON_UINT(m_iContribution, val, "con_value");
		DJSON_UINT(m_iRemainContribution, val, "remain_con_value");
		DJSON_UINT(m_iKnighthoodLevel, val, "con_lv");
		DJSON_UINT(m_iLevel, val, "user_lv");
		DJSON_UINT(m_iLastLoginTs, val, "last_login_ts");
		DJSON_UINT(m_uLeaderID, val, "lid");
		DJSON_UINT(m_uLeaderDress, val, "ldid");
	}
	GuildJoinBossState GuildUserBasics::getGuildJoinBossState()
	{
		if(!DM_GET_GUILDBOSS_MODEL->getIsOpen() || DM_GET_GUILDBOSS_MODEL->getConfig()->getStartTime() > GameManager::sharedGameManager()->getServerTime())
		{
			return GuildJoinBossNotOpen;
		}
		uint32_t damageDaily = 0;
		cocos2d::CCArray *m_pMemberRank = DM_GET_GUILDBOSS_MODEL->getMemberRank();
		if(!m_pMemberRank)return GuildJoinBossNotOpen;
		for(uint32_t i = 0; i < m_pMemberRank->count();i++)
		{
			guildboss::GuildBossUser *m_pGuildBossUser = (guildboss::GuildBossUser *)(m_pMemberRank->objectAtIndex(i));
			if(m_nUserId == m_pGuildBossUser->getUserID())
			{
				damageDaily = m_pGuildBossUser->getDamageDaily();
				break;
			}
		}

		if(damageDaily > 0)
		{
			return GuildJoinBossHaveJoined;
		}else
		{
			return GuildJoinBossNotJoin;
		}
	}

	void GuildUser::parse(const Json::Value &val)
	{
		GuildUserBasics::parse(val);
		m_iContributionAdded = 0;
		DJSON_UINT(m_nGuildID, val, "gid");
		DJSON_UINT(m_nLastSign, val, "sign_ts");
		if (TimeUtil::getDiff(m_nLastSign ,TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),0)) < 0)
		{
			m_bIsSigned = true;
		}
		else
		{
			m_bIsSigned = false;
		}
		DJSON_UINT(m_nLastAnswer, val, "ques_ts");
		DJSON_UINT(m_nQuestionID, val, "ques_id");
		DJSON_UINT(m_nChivalryTime, val, "knight_ts");
		DJSON_UINT(m_nOverlordTime, val, "king_ts");
		DJSON_UINT(m_iChivalry, val, "knight_times");
		DJSON_UINT(m_iOverlord, val, "king_times");
	}
	void GuildApplication::parse(const Json::Value &val)
	{
		DJSON_UINT(m_nApplicationID, val, "id");
		DJSON_UINT(m_nGuildID, val, "gid");
		DJSON_UINT(m_nUserId, val, "uid");
		DJSON_STRING(m_sName, val, "name");
		DJSON_UINT(m_iLevel, val, "lv");
		DJSON_UINT(m_iFight, val, "fight");
	}
	void GuildNotice::parse(const Json::Value &val)
	{
		int type = 0;
		DJSON_UINT(type, val, "type");
		GuildNoticeType typeMap[5] = {GuildNoticeTypeNone, GuildNoticeTypeAnswer, GuildNoticeTypeOverlord, GuildNoticeTypeChivalry, GuildNoticeTypeSign};
		m_iType = typeMap[type];
		DJSON_STRING(m_sUserName, val, "name");
		DJSON_UINT(m_iContribution, val["args"], "value");
		DJSON_UINT(m_iItemNumber, val["args"], "num");
		if (GuildNoticeTypeOverlord == m_iType)
		{
			m_iItemNumber /= D_CONFIG_UINT_FOR_KEY(GUILD_OVERLORD_COINS_DIVISOR);
		}
	}
}

GuildModel::GuildModel():
	m_pGuildLevelMap(NULL),
	m_pGuildKnighthoodMap(NULL),
	m_pTreeChivalryLevelMap(NULL),
	m_pTreeOverlordLevelMap(NULL),
	m_pGuildCoinsMap(NULL),
	m_pTreeChivalrySkillMap(NULL),
	m_pTreeOverlordSkillMap(NULL),
	m_pGuildQuestionMap(NULL),
	m_pGuildMarqueeMap(NULL),
	m_pSelfInfo(NULL),
	m_pGuildInfo(NULL),
	m_pGuildList(NULL),
	m_pGuildApplication(NULL),
	m_pSelfApplication(NULL),
	m_pGuildNotice(NULL),
	m_pGuildUser(NULL),
	m_pOtherGuildMap(NULL),
	m_pOtherGuildUsersMap(NULL),
	m_pTreeChivalry(NULL),
	m_pTreeOverlord(NULL),
	m_pGuildLastLoginDescMap(NULL),
	m_pGuildMall( NULL ) ,
	m_pGuildMallMap( NULL ),
    m_pGuildLog(NULL)
{
}

GuildModel::~GuildModel()
{
	CC_SAFE_RELEASE(m_pGuildLevelMap);
	CC_SAFE_RELEASE(m_pGuildKnighthoodMap);
    CC_SAFE_RELEASE(m_pTreeChivalryLevelMap);
    CC_SAFE_RELEASE(m_pTreeOverlordLevelMap);
	CC_SAFE_RELEASE(m_pGuildCoinsMap);
	CC_SAFE_RELEASE(m_pTreeChivalrySkillMap);
	CC_SAFE_RELEASE(m_pTreeOverlordSkillMap);
	CC_SAFE_RELEASE(m_pGuildQuestionMap);
	CC_SAFE_RELEASE(m_pGuildMarqueeMap);
	CC_SAFE_RELEASE(m_pSelfInfo);
	CC_SAFE_RELEASE(m_pGuildInfo);
	CC_SAFE_RELEASE(m_pGuildList);
	CC_SAFE_RELEASE(m_pGuildApplication);
	CC_SAFE_RELEASE(m_pSelfApplication);
	CC_SAFE_RELEASE(m_pGuildNotice);
	CC_SAFE_RELEASE(m_pGuildUser);
	CC_SAFE_RELEASE(m_pOtherGuildMap);
	CC_SAFE_RELEASE(m_pOtherGuildUsersMap);
	CC_SAFE_RELEASE(m_pTreeChivalry);
	CC_SAFE_RELEASE(m_pTreeOverlord);
	CC_SAFE_RELEASE(m_pGuildLastLoginDescMap);
	CC_SAFE_RELEASE(m_pGuildMall ) ;
	CC_SAFE_RELEASE(m_pGuildMallMap ) ;
    CC_SAFE_RELEASE(m_pGuildLog);
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildModel::resetGuild), this);
}

bool GuildModel::init()
{

	m_pGuildInfo = GuildInfo::create();
	m_pGuildInfo->retain();
	m_pGuildInfo->m_nGuildID = 0;

	m_pSelfInfo = GuildUser::create();
	//默认值
	m_pSelfInfo->m_nGuildID = 0;
	CC_SAFE_RETAIN(m_pSelfInfo);
	m_pGuildLevelMap = CCDictionary::create();
	CC_SAFE_RETAIN(m_pGuildLevelMap);
	m_pGuildKnighthoodMap = CCDictionary::create();
	CC_SAFE_RETAIN(m_pGuildKnighthoodMap);
	m_pTreeChivalryLevelMap = CCDictionary::create();
    CC_SAFE_RETAIN(m_pTreeChivalryLevelMap);
	m_pTreeOverlordLevelMap = CCDictionary::create();
    CC_SAFE_RETAIN(m_pTreeOverlordLevelMap);
	m_pGuildCoinsMap = CCDictionary::create();
	CC_SAFE_RETAIN(m_pGuildCoinsMap);
	m_pTreeChivalrySkillMap = CCDictionary::create();
	CC_SAFE_RETAIN(m_pTreeChivalrySkillMap);
	m_pTreeOverlordSkillMap = CCDictionary::create();
	CC_SAFE_RETAIN(m_pTreeOverlordSkillMap);
	m_pGuildQuestionMap = CCDictionary::create();
	CC_SAFE_RETAIN(m_pGuildQuestionMap);
	m_pGuildMarqueeMap = CCDictionary::create();
	CC_SAFE_RETAIN(m_pGuildMarqueeMap);
	m_pGuildLastLoginDescMap = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pGuildLastLoginDescMap);
	m_pGuildMallMap = cocos2d::CCDictionary::create() ;
	CC_SAFE_RETAIN( m_pGuildMallMap ) ;
    
    m_pGuildLog = cocos2d::CCArray::create();
    CC_SAFE_RETAIN(m_pGuildLog);

	LoadGuildLevelData(CSVDecoder::create("data/guild/guild_level")->getData(), m_pGuildLevelMap);
	LoadGuildKnighthoodLevelData(CSVDecoder::create("data/guild/guild_knighthood")->getData(), m_pGuildKnighthoodMap);
	LoadGuildLevelData(CSVDecoder::create("data/guild/tree_chivalry_level")->getData(), m_pTreeChivalryLevelMap);
	LoadGuildLevelData(CSVDecoder::create("data/guild/tree_overlord_level")->getData(), m_pTreeOverlordLevelMap);
	LoadGuildCoinsData(CSVDecoder::create("data/guild/guild_coins")->getData());
	LoadGuildSkillData(CSVDecoder::create("data/guild/tree_chivalry_skill")->getData(), m_pTreeChivalrySkillMap);
	LoadGuildSkillData(CSVDecoder::create("data/guild/tree_overlord_skill")->getData(), m_pTreeOverlordSkillMap);
	LoadGuildQuestionData(CSVDecoder::create("data/guild/guild_question")->getData());
	LoadGuildMarqueeData(CSVDecoder::create("data/guild/guild_marquee")->getData());
	LoadGuildLastLoginDesc(CSVDecoder::create("data/guild/guild_lastlogindesc")->getData(),m_pGuildLastLoginDescMap);
	LoadGuildMallData( CSVDecoder::create("data/guild/guild_mall")->getData(), m_pGuildMallMap ) ;

	m_pMaxLevelChivalry = m_pTreeChivalryLevelMap->allKeys()->count();
	m_pMaxLevelOverlord = m_pTreeOverlordLevelMap->allKeys()->count();

	TimerUtil::sharedTimerUtil()->schedule(schedule_selector(GuildModel::resetGuild), this, D_CONFIG_UINT_FOR_KEY(GUILD_REFRESH_INTERVAL));

    return Model::init();
}


void GuildModel::syncGuildApplyList(){
    HTTP_CLIENT->guildApplyList();
}


void GuildModel::syncFromServer()
{	
	//当玩家未在帮会（等待申请时）每次都请求
	if(!isSynced || !m_pSelfInfo->getGuildID())
	{
		HTTP_CLIENT->guildGet();
	}
	else
	{
		postNotification(D_EVENT_GUILD_GET);
	}
}

void GuildModel::parse(const Json::Value &val)
{
	parseGuildInfo(val["guild"]);
	parseGuildSelfInfo(val["guildUser"]);
	refreshContribution(val);
	setOverlordTimes(m_pSelfInfo->getOverlord());
	postNotification(D_EVENT_GUILD_GET);
	parseGuildMall( val["guildMall"] ) ;
	isSynced = true;
}

void GuildModel::parseGuildSelfInfo(const Json::Value &val)
{
	m_pSelfInfo->parse(val);
/*	for (int i = 0; i < m_pGuildKnighthoodMap->allKeys()->count(); i++)
	{
		guild::GuildKnighthoodLevel *level = (guild::GuildKnighthoodLevel *)m_pGuildKnighthoodMap->objectForKey(i + 1);
		if (m_pSelfInfo->getContribution() < level->totalExp)
		{
			m_pSelfInfo->setNextExp(level->totalExp - m_pSelfInfo->getContribution());
			//m_pSelfInfo->setKnighthoodLevel(level->level);
			m_pSelfInfo->setKnighthoodName(level->name);
			break;
		}
		
	}*/
	if (m_pSelfInfo->m_nUserId == m_pGuildInfo->m_nLeaderID)
	{
		m_pSelfInfo->setKnighthoodName(std::string(D_LOCAL_STRING("GuildLeader")));
		m_pSelfInfo->setNextExp(0);
		m_pSelfInfo->m_iPosition = GuildPositionPresident;
	}
	else if (m_pSelfInfo->m_nUserId == m_pGuildInfo->m_nAssistant1
		|| m_pSelfInfo->m_nUserId == m_pGuildInfo->m_nAssistant2
		|| m_pSelfInfo->m_nUserId == m_pGuildInfo->m_nAssistant3)
	{
		m_pSelfInfo->m_iPosition = GuildPositionAssistant;
	}
	else
	{
		m_pSelfInfo->m_iPosition = GuildPositionMember;
	}
	if (0 == m_pSelfInfo->getQuestionID())
	{
		m_pQuestion = NULL;
	}
	else
	{
		m_pQuestion = (guild::GuildQuestion *)m_pGuildQuestionMap->objectForKey(m_pSelfInfo->getQuestionID()/* % (QUESTION_MAX + 1)*/);
	}

	//setSelfContribution(m_pSelfInfo->m_iContribution);
}

void GuildModel::parseGuildInfo(const Json::Value &val)
{
	DJSON_UINT(m_pGuildInfo->m_nGuildID, val, "id");
	m_pGuildInfo->m_bIsCreatedToday = false;
	if (0 == m_pGuildInfo->m_nGuildID)
	{
		return;
	}
	m_pGuildInfo->m_iContributionAdded = 0;
	DJSON_STRING(m_pGuildInfo->m_sName, val, "name");
	DJSON_UINT(m_pGuildInfo->m_nFounderID, val, "founder_id");
	DJSON_UINT(m_pGuildInfo->m_nLeaderID, val, "leader_id");
	DJSON_UINT(m_pGuildInfo->m_iUserCount, val, "num");
	DJSON_UINT(m_pGuildInfo->m_iLevel, val, "lv");
	DJSON_UINT(m_pGuildInfo->m_iContribution, val, "con_value");
	DJSON_STRING(m_pGuildInfo->m_sAnnouncement, val, "announcement");
	DJSON_UINT(m_pGuildInfo->m_nAssistant1, val, "assis_1");
	DJSON_UINT(m_pGuildInfo->m_nAssistant2, val, "assis_2");
	DJSON_UINT(m_pGuildInfo->m_nAssistant3, val, "assis_3");

	DJSON_UINT(m_pGuildInfo->m_nCreateTs, val, "create_ts");
	uint32_t serverTime = GameManager::sharedGameManager()->getServerTime();
	uint32_t startOfToday = TimeUtil::getStartTimeOfToday(serverTime);
	if (m_pGuildInfo->m_nCreateTs > startOfToday)
	{
		m_pGuildInfo->m_bIsCreatedToday = true;
	}

	//setGuildContribution(m_pGuildInfo->m_iContribution);
	parseTree(val);
	if (val.isMember("arUser"))
	{
		parseGuildUser(val["arUser"]);
	}
}


void GuildModel::parseOtherGuildInfo(const Json::Value &val)
{
	uint32_t guildID = 0;
	DJSON_UINT(guildID, val, "id");
	if (0 == guildID)
	{
		return;
	}
	if (NULL == m_pOtherGuildMap)
	{
		m_pOtherGuildMap = CCDictionary::create();
		m_pOtherGuildMap->retain();
	}
	guild::GuildInfo *guildInfo = NULL;
	if (m_pOtherGuildMap->allKeys() && m_pOtherGuildMap->allKeys()->count() > 0)
	{
		guildInfo = (guild::GuildInfo *)m_pOtherGuildMap->objectForKey(guildID);
	}
	if (NULL == guildInfo)
	{
		guildInfo = GuildInfo::create();
		guildInfo->setGuildID(guildID);
		m_pOtherGuildMap->setObject(guildInfo, guildID);
	}
	DJSON_STRING(guildInfo->m_sName, val, "name");
	DJSON_UINT(guildInfo->m_nFounderID, val, "founder_id");
	DJSON_UINT(guildInfo->m_nLeaderID, val, "leader_id");
	DJSON_UINT(guildInfo->m_iUserCount, val, "num");
	DJSON_UINT(guildInfo->m_iLevel, val, "lv");
	DJSON_UINT(guildInfo->m_iContribution, val, "con_value");
	DJSON_STRING(guildInfo->m_sAnnouncement, val, "announcement");
	DJSON_UINT(guildInfo->m_nAssistant1, val, "assis_1");
	DJSON_UINT(guildInfo->m_nAssistant2, val, "assis_2");
	DJSON_UINT(guildInfo->m_nAssistant3, val, "assis_3");

	parseOtherGuildUser(val["arUser"], guildID);
	postNotification(D_EVENT_GUILD_DETAILS);
}

void GuildModel::parseOtherGuildUser(const Json::Value &val, unsigned int guildID)
{

	if (NULL == m_pOtherGuildUsersMap)
	{
		m_pOtherGuildUsersMap = CCDictionary::create();
		m_pOtherGuildUsersMap->retain();
	}
	CCArray *guildUsers = NULL;
	if (m_pOtherGuildUsersMap->allKeys() && m_pOtherGuildUsersMap->allKeys()->count() > 0)
	{
		guildUsers = (CCArray *)m_pOtherGuildUsersMap->objectForKey(guildID);
	}
	if (NULL == guildUsers)
	{
		guildUsers = CCArray::create();
		m_pOtherGuildUsersMap->setObject(guildUsers, guildID);
	}


	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		guild::GuildUserBasics *data = GuildUserBasics::create();
		data->parse(*i);
		if (0 == data->m_iKnighthoodLevel)
		{
			data->setKnighthoodName(D_LOCAL_STRING("GuildLeader"));
		}
		else
		{
			guild::GuildKnighthoodLevel *level = (guild::GuildKnighthoodLevel *)m_pGuildKnighthoodMap->objectForKey(data->m_iKnighthoodLevel);
			data->setKnighthoodName(level->name);
		}
		if (data->m_nUserId == m_pGuildInfo->m_nLeaderID)
		{
			data->setKnighthoodName(std::string(D_LOCAL_STRING("GuildLeader")));
			data->m_iPosition = GuildPositionPresident;
		}
		else if (data->m_nUserId == m_pGuildInfo->m_nAssistant1
			|| data->m_nUserId == m_pGuildInfo->m_nAssistant2
			|| data->m_nUserId == m_pGuildInfo->m_nAssistant3)
		{
			data->m_iPosition = GuildPositionAssistant;
		}
		else
		{
			data->m_iPosition = GuildPositionMember;
		}
		guildUsers->addObject(data);
	}
	std::sort(guildUsers->data->arr, guildUsers->data->arr + guildUsers->data->num, sortUserByContributionLevelAndLevel);
}

void GuildModel::syncGuildByID(uint32_t guildID)
{
	if (NULL == m_pOtherGuildMap 
		|| NULL == m_pOtherGuildMap->allKeys()
		|| NULL == m_pOtherGuildUsersMap 
		|| NULL == m_pOtherGuildUsersMap->allKeys())
	{
		HTTP_CLIENT->guildDetails(guildID);
	}
	else if (NULL == m_pOtherGuildMap->objectForKey(guildID) || NULL == m_pOtherGuildUsersMap->objectForKey(guildID))
	{
		HTTP_CLIENT->guildDetails(guildID);
	}
	else
	{
		postNotification(D_EVENT_GUILD_DETAILS);
	}
}

guild::GuildInfo *GuildModel::getOtherGuild(uint32_t guildID)
{
	guild::GuildInfo *guildInfo = (guild::GuildInfo *)m_pOtherGuildMap->objectForKey(guildID);
	if (NULL == guildInfo)
	{
		char errorStr[255];
		sprintf(errorStr, "Guild %d is NULL", guildID);
		CCAssert(guildInfo != NULL, errorStr);
	}
	return guildInfo;
}

cocos2d::CCArray *GuildModel::getOtherGuildUsers(uint32_t guildID)
{
	CCArray *guildUsers = (CCArray *)m_pOtherGuildUsersMap->objectForKey(guildID);
	if (NULL == guildUsers)
	{
		char errorStr[255];
		sprintf(errorStr, "Guild %d is NULL", guildID);
		CCAssert(guildUsers != NULL, errorStr);
	}
	return guildUsers;
}

void GuildModel::parseGuildList(const Json::Value &val, uint32_t startNum)
{
	if (NULL == m_pGuildList)
	{
		m_pGuildList = CCArray::create();
		m_pGuildList->retain();
	}
	else
	{
		if (0 == startNum)
		{
			m_pGuildList->removeAllObjects();
		}
	}
	if (startNum < m_pGuildList->count())
	{
		return;
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		guild::GuildBasics *data = GuildBasics::create();
		data->parse(*i);
		if (m_pSelfApplication)
		{
			for (uint32_t i = 0; i < m_pSelfApplication->count(); i++)
			{
				guild::GuildApplication *apply = (guild::GuildApplication *)m_pSelfApplication->objectAtIndex(i);
				if (apply->getGuildID() == data->getGuildID())
				{
					data->m_isApplied = true;
					break;
				}
			}
		}
		m_pGuildList->addObject(data);
	}
	postNotification(D_EVENT_GUILD_GET_LIST);
}

void GuildModel::parseSelfApplication(const Json::Value &val)
{
	if (NULL == m_pSelfApplication)
	{
		m_pSelfApplication = CCArray::create();
		m_pSelfApplication->retain();
	}
	else
	{
		m_pSelfApplication->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		guild::GuildApplication *data = GuildApplication::create();
		data->parse(*i);
		m_pSelfApplication->addObject(data);
	}
}

void GuildModel::parseGuildApplication(const Json::Value &val)
{
	if (NULL == m_pGuildApplication)
	{
		m_pGuildApplication = CCArray::create();
		m_pGuildApplication->retain();
	}
	else
	{
		m_pGuildApplication->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		guild::GuildApplication *data = GuildApplication::create();
		data->parse(*i);
		m_pGuildApplication->addObject(data);
	}

	std::sort(m_pGuildApplication->data->arr, m_pGuildApplication->data->arr + m_pGuildApplication->data->num, sortApplicationsByFight);

	postNotification(D_EVENT_GUILD_APPLY_LIST);
}

guild::GuildNotice * GuildModel::parseOneNotice(const Json::Value &val)
{
	guild::GuildNotice *data = GuildNotice::create();
	data->parse(val);
	CCString *formatStr = (CCString *)m_pGuildMarqueeMap->objectForKey(data->getType());
	if (!formatStr)
	{
		return NULL;
	}
	CCString *noticeStr = NULL;
	if (data->m_iType == GuildNoticeTypeAnswer || data->m_iType == GuildNoticeTypeSign)
	{
		noticeStr = CCString::createWithFormat(formatStr->getCString(), data->getUserName().c_str(), data->getContribution());
	}
	else
	{
		noticeStr = CCString::createWithFormat(formatStr->getCString(), data->getUserName().c_str(), data->getItemNumber(), data->getContribution());
	}
	data->setMarquee(noticeStr->getCString());
	return data;
}


void GuildModel::parseGuildLog(const Json::Value &val)
{
    m_pGuildLog->removeAllObjects();
    CCArray *list = CCArray::create();
    CC_SAFE_RETAIN(list);
    
    for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
		guild::GuildLog *data = GuildLog::create();
		data->parse(*i);
        list->addObject(data);
	}
    std::sort(list->data->arr, list->data->arr + list->data->num, sortLogByTime);
    CCObject* obj = NULL;
    guild::GuildLog* guildlog = NULL;
    
    

    
    int nowYear = 0;
    int nowMonth = 0;
    int nowDay = 0;
    int index = 0;
    
	CCARRAY_FOREACH(list, obj){
        guildlog = (guild::GuildLog*)obj;
        uint32_t ts = guildlog->getTs();
        time_t time = (time_t)ts;
        struct tm * ptm;
        ptm = gmtime(&time);
        
        if(nowYear!=ptm->tm_year+1900 ||nowMonth!=ptm->tm_mon+1 ||nowDay!=ptm->tm_mday){
            nowYear = ptm->tm_year+1900;
            nowMonth = ptm->tm_mon+1;
            nowDay = ptm->tm_mday;
            m_pGuildLog->addObject(CCArray::create());
            index++;
        }
        CCArray * arr = (CCArray *)m_pGuildLog->objectAtIndex(index -1 );
        arr->addObject(guildlog);
    }
    
	postNotification(D_EVENT_GUILD_GET_LOG);
}


void GuildModel::parseGuildNotice(const Json::Value &val, GuildNoticeType iType /* = GuildNoticeTypeNone */)
{
	if (NULL == m_pGuildNotice)
	{
		m_pGuildNotice = CCDictionary::create();
		m_pGuildNotice->retain();
/*		for (int i = 0; i < GUILD_NOTICE_TYPE_MAX + 1; i++)
		{
			CCArray *pNotice = CCArray::create();
			m_pGuildNotice->addObject(pNotice);
		}*/
	}
	CCArray *pNotice = (CCArray *)m_pGuildNotice->objectForKey(iType);
	if (NULL == pNotice)
	{
		pNotice = CCArray::create();
		m_pGuildNotice->setObject(pNotice, iType);
	}
	else
	{
		pNotice->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		guild::GuildNotice *data = parseOneNotice(*i);
		if (data)
		{
			pNotice->addObject(data);
		}
	}
	postNotification(D_EVENT_GUILD_GET_NOTICE);
}

void GuildModel::parseGuildUser(const Json::Value &val)
{
	if (NULL == m_pGuildUser)
	{
		m_pGuildUser = CCArray::create();
		m_pGuildUser->retain();
	}
	else
	{
		m_pGuildUser->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		guild::GuildUserBasics *data = GuildUserBasics::create();
		data->parse(*i);
		if (0 != data->m_iKnighthoodLevel)
		{
			guild::GuildKnighthoodLevel *level = (guild::GuildKnighthoodLevel *)m_pGuildKnighthoodMap->objectForKey(data->m_iKnighthoodLevel);
			data->setKnighthoodName(level->name);
		}
		if (data->m_nUserId == m_pGuildInfo->m_nLeaderID)
		{
			data->setKnighthoodName(std::string(D_LOCAL_STRING("GuildLeader")));
			data->m_iPosition = GuildPositionPresident;
		}
		else if (data->m_nUserId == m_pGuildInfo->m_nAssistant1
			|| data->m_nUserId == m_pGuildInfo->m_nAssistant2
			|| data->m_nUserId == m_pGuildInfo->m_nAssistant3)
		{
			data->m_iPosition = GuildPositionAssistant;
		}
		else
		{
			data->m_iPosition = GuildPositionMember;
		}
		m_pGuildUser->addObject(data);
	}
	std::sort(m_pGuildUser->data->arr, m_pGuildUser->data->arr + m_pGuildUser->data->num, sortUserByContribution);
}

void GuildModel::LoadGuildLevelData(const CSVDecoder::CSV &val, CCDictionary *dataMap)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		guild::GuildLevel *pData = guild::GuildLevel::create();
		pData->level = atoi(val[i][0].c_str());
		/************************************************************************/
		/* 11.11编译不通过                                                                     */
		/************************************************************************/
		pData->nextExp = atol(val[i][1].c_str());
		pData->totalExp = atol(val[i][2].c_str());
		dataMap->setObject(pData, pData->level);
	}
}

void GuildModel::LoadGuildKnighthoodLevelData(const CSVDecoder::CSV &val, cocos2d::CCDictionary *dataMap)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		guild::GuildKnighthoodLevel *pData = guild::GuildKnighthoodLevel::create();
		pData->level = atoi(val[i][0].c_str());
		pData->nextExp = atoi(val[i][1].c_str());
		pData->totalExp = atoi(val[i][2].c_str());
		pData->name = val[i][3];
		dataMap->setObject(pData, pData->level);
	}
}

void GuildModel::LoadGuildCoinsData(const CSVDecoder::CSV &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		guild::GuildCoins *pData = guild::GuildCoins::create();
		pData->times = atoi(val[i][0].c_str());
		pData->coins = atoi(val[i][1].c_str());
		pData->contribution = atoi(val[i][2].c_str());
		m_pGuildCoinsMap->setObject(pData, pData->times);
	}
}

void GuildModel::LoadGuildSkillData(const CSVDecoder::CSV &val, CCDictionary *dataMap)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		guild::GuildTreeSkill *pData = guild::GuildTreeSkill::create();
		pData->level = atoi(val[i][0].c_str());
		pData->skillID = atoi(val[i][1].c_str());
		pData->skillValue = atof(val[i][2].c_str());
		pData->knighthood = atoi(val[i][3].c_str());
		pData->text = val[i][4];
		dataMap->setObject(pData, pData->level);
	}
}

void GuildModel::LoadGuildLastLoginDesc(const CSVDecoder::CSV &val, cocos2d::CCArray *dataMap)
{
	dataMap->removeAllObjects();
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		guild::GuildLastLoginDesc *pData = guild::GuildLastLoginDesc::create();
		pData->minInveter = atoi(val[i][0].c_str());
		pData->lastLoginDesc = val[i][1];
		dataMap->addObject(pData);
	}	
}


std::string GuildModel::getLastLoginDescByInveter(guild::GuildUserBasics *userInfo)
{
	std::string lastLoginDesc = "";
	uint32_t unowServerTime = GameManager::sharedGameManager()->getServerTime();
	uint32_t inveter = unowServerTime - userInfo->getLastLoginTs();
    
	if( unowServerTime < userInfo->getLastLoginTs() )//当前服务器时间比上次登录时间还小，时差就默认为0
		inveter = 0;
    
    if(userInfo->getUserId()==DM_GET_USER_MODEL->getUserInfo()->getUserId()){
        inveter = 0;
    }
    
	for (uint32_t i = 0; i < m_pGuildLastLoginDescMap->count(); ++i)
	{
		guild::GuildLastLoginDesc *pData = (guild::GuildLastLoginDesc *)m_pGuildLastLoginDescMap->objectAtIndex(i);
		if(inveter >= pData->minInveter)
		{
			lastLoginDesc = pData->lastLoginDesc;
			break;
		}
	}	
	return lastLoginDesc;
}

void GuildModel::LoadGuildQuestionData(const CSVDecoder::CSV &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		guild::GuildQuestion *pData = guild::GuildQuestion::create();
		pData->questionID = atoi(val[i][0].c_str());
		pData->rightAnswer = atoi(val[i][1].c_str());
		pData->questionStr = val[i][2];
		pData->answer1Str = val[i][3];
		pData->answer2Str = val[i][4];
		m_pGuildQuestionMap->setObject(pData, pData->questionID);
	}
}

void GuildModel::LoadGuildMarqueeData(const CSVDecoder::CSV &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		uint32_t marqueeID = atoi(val[i][0].c_str());
		CCString *marqueeFormat = CCString::create(val[i][1]);
		m_pGuildMarqueeMap->setObject(marqueeFormat, marqueeID);
	}
}

std::string GuildModel::GetKnighthoodNameByLevel(uint32_t iLevel)
{
	GuildKnighthoodLevel *level = (GuildKnighthoodLevel *)m_pGuildKnighthoodMap->objectForKey(iLevel);
	return level->name;
}


void GuildModel::refreshContribution(const Json::Value &val)
{
	//"guild":{"con_value":3355,"lv":6},"guildUser":{"sign_ts":1383380617,"con_lv":1,"con_value":100}}
	uint32_t preLevel = m_pGuildInfo->m_iLevel ;
	uint32_t lastSelfContribution = m_pSelfInfo->m_iContribution;
	uint32_t lastGuildContribution = m_pGuildInfo->m_iContribution;
	DJSON_UINT(m_pGuildInfo->m_iLevel, val["guild"], "lv");
	DJSON_UINT(m_pGuildInfo->m_iContribution, val["guild"], "con_value");
	DJSON_UINT(m_pSelfInfo->m_iLevel, val["guildUser"], "con_lv");
	DJSON_UINT(m_pSelfInfo->m_iContribution, val["guildUser"], "con_value");
	DJSON_UINT( m_pSelfInfo->m_iRemainContribution, val["guildUser"], "remain_con_value" ) ;
	m_pSelfInfo->m_iContributionAdded = m_pSelfInfo->m_iContribution - lastSelfContribution;
	m_pGuildInfo->m_iContributionAdded = m_pGuildInfo->m_iContribution - lastGuildContribution;
	if (0 == m_pGuildInfo->m_iLevel)
	{
		return;
	}
	if ( m_pGuildInfo->m_iLevel > preLevel )
	{
		HTTP_CLIENT->guildGet() ;
	}
	refreshContribution();
}


void GuildModel::refreshContribution()
{
	if (guild::GuildPositionPresident != m_pSelfInfo->getPosition())
	{
		GuildKnighthoodLevel *level = (GuildKnighthoodLevel *)m_pGuildKnighthoodMap->objectForKey(m_pSelfInfo->m_iKnighthoodLevel);
		GuildKnighthoodLevel *nextLevel = (GuildKnighthoodLevel *)m_pGuildKnighthoodMap->objectForKey(m_pSelfInfo->m_iKnighthoodLevel + 1);
		if (nextLevel)
		{
			while (nextLevel && nextLevel->totalExp <= m_pSelfInfo->m_iContribution)
			{
				m_pSelfInfo->m_iKnighthoodLevel += 1;
				level = (GuildKnighthoodLevel *)m_pGuildKnighthoodMap->objectForKey(m_pSelfInfo->m_iKnighthoodLevel);
				nextLevel = (GuildKnighthoodLevel *)m_pGuildKnighthoodMap->objectForKey(m_pSelfInfo->m_iKnighthoodLevel + 1);
			}
		}
		if (NULL == nextLevel)
		{
			m_pSelfInfo->m_iNextExp = 0;
		}
		else
		{
			m_pSelfInfo->m_iNextExp = nextLevel->totalExp - m_pSelfInfo->m_iContribution;
		}
		m_pSelfInfo->setKnighthoodName(level->name);
	}
	GuildLevel *level = (GuildLevel *)m_pGuildLevelMap->objectForKey(m_pGuildInfo->m_iLevel + 1);
	if (NULL == level)
	{
		m_pGuildInfo->m_iContributionLevel = 0;
		m_pGuildInfo->m_iContributionNext = 0;
	}
	else
	{
		level = (GuildLevel *)m_pGuildLevelMap->objectForKey(m_pGuildInfo->m_iLevel);
		m_pGuildInfo->m_iContributionLevel = m_pGuildInfo->m_iContribution - level->totalExp;
		m_pGuildInfo->m_iContributionNext = level->nextExp;
	}
}

void GuildModel::parseTree(const Json::Value &val)
{
	if (NULL == m_pTreeOverlord)
	{
		m_pTreeOverlord = guild::GuildTree::create();
		m_pTreeOverlord->retain();
		m_pTreeOverlord->m_iKind = 1;
	}
	if (NULL == m_pTreeChivalry)
	{
		m_pTreeChivalry = guild::GuildTree::create();
		m_pTreeChivalry->retain();
		m_pTreeChivalry->m_iKind = 2;
	}
	DJSON_UINT(m_pTreeOverlord->m_iLevel, val, "king_lv");
	double fItemCount = 0;
	DJSON_DOUBLE(fItemCount, val, "king_con_value");
	m_pTreeOverlord->m_iItemCount = (uint64_t)fItemCount;
	GuildLevel *level = (GuildLevel *)m_pTreeOverlordLevelMap->objectForKey(m_pTreeOverlord->m_iLevel);
	m_pTreeOverlord->m_iItemCountRest = level->nextExp + level->totalExp - m_pTreeOverlord->m_iItemCount;
	DJSON_UINT(m_pTreeChivalry->m_iLevel, val, "knight_lv");
	DJSON_UINT(m_pTreeChivalry->m_iItemCount, val, "knight_con_value");
	level = (GuildLevel *)m_pTreeChivalryLevelMap->objectForKey(m_pTreeChivalry->m_iLevel);
	m_pTreeChivalry->m_iItemCountRest = level->nextExp + level->totalExp - m_pTreeChivalry->m_iItemCount;
	postNotification(D_EVENT_GUILD_TREE);
}


//////////////////////////////////////////// 门派商店 /////////////////////////////////////////////
void GuildModel::LoadGuildMallData( const CSVDecoder::CSV &val, CCDictionary *dataMap )
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		guild::GuildMallGoods *pData = guild::GuildMallGoods::create() ;
		pData->id = atoi( val[i][0].c_str() ) ;
		pData->cost = atoi( val[i][1].c_str() ) ;
		pData->type = atoi( val[i][2].c_str() ) ;
		pData->goodsID = atoi( val[i][3].c_str() ) ;
		pData->num = atoi( val[i][4].c_str() ) ;
		dataMap->setObject( pData, pData->id ) ;
	}	
}

void GuildModel::parseGuildMall( const Json::Value &val, int remainConValue )
{
	if ( m_pGuildMall == NULL )
	{
		m_pGuildMall = guild::GuildMall::create() ;
		CC_SAFE_RETAIN( m_pGuildMall ) ;
	}
	// 刷新当前帮贡
	if ( remainConValue >= 0 )
	{
		m_pSelfInfo->m_iRemainContribution = remainConValue ;
	}
	if ( !val.isNull() )
	{
		DJSON_INT( m_pGuildMall->m_iRefreshCount, val, "refresh_count" ) ;
		m_pGuildMall->m_goodIdArr->removeAllObjects() ;
		m_pGuildMall->m_exchangedIdArr->removeAllObjects() ;
		for ( uint32_t i = 0; i < val["items"].size(); i++ )
		{
			m_pGuildMall->m_goodIdArr->addObject( 
				CCString::createWithFormat( "%d", val["items"][i].asInt() ) ) ;
		}
		for ( uint32_t i = 0; i < val["exchange_members"].size(); i++ )
		{
			m_pGuildMall->m_exchangedIdArr->addObject( 
				CCString::createWithFormat( "%d", val["exchange_members"][i].asInt() ) ) ;
		}
	}
	if ( m_pGuildMall->m_iRefreshCount <= 0 )
	{
		m_pGuildMall->m_iRefreshCount = 0 ; 
	}
	postNotification( D_EVENT_GUILD_MALL_REFRESH ) ;
}

guild::GuildMallGoods *GuildModel::getMallGoodsById( uint32_t id )
{
	return (guild::GuildMallGoods *)m_pGuildMallMap->objectForKey( id ) ;
}

//是否能取代会长
bool GuildModel::canReplaceBoss(){
    CCObject* obj = NULL;
    guild::GuildUserBasics* pGuildUserBasics = NULL;
	CCARRAY_FOREACH(m_pGuildUser, obj)
	{
		pGuildUserBasics = (guild::GuildUserBasics*)obj;
        if(pGuildUserBasics->getPosition()==GuildPositionPresident){
            uint64_t leftTime = GameManager::sharedGameManager()->getServerTime() - pGuildUserBasics->getLastLoginTs();
			if(leftTime>D_CONFIG_FLOAT_FOR_KEY(GUILD_LEAVE_TIME))
                return true;
        }
	}
    return false;
}

void GuildModel::getGuildLogs(){
    
}



//公会成员是否能取代会长职位
uint32_t GuildModel::guildUserCanReplace(){
    uint32_t myId = DM_GET_USER_MODEL->getUserInfo()->getUserId();
    
    
    //我是助理 直接取代会长
    if(myId==m_pGuildInfo->getAssistant1() ||myId==m_pGuildInfo->getAssistant2() ||myId==m_pGuildInfo->getAssistant3()){
        return 1;
    }
    
    bool assistantIsNotOnline = false;
    
    CCObject* obj = NULL;
	guild::GuildUserBasics* pGuildUserBasics = NULL;

    uint32_t maxContribution = 0;
    uint32_t myContribution = 0;
    
	CCARRAY_FOREACH(m_pGuildUser, obj)
	{
		pGuildUserBasics = (guild::GuildUserBasics*)obj;
        uint64_t leftTime = GameManager::sharedGameManager()->getServerTime() - pGuildUserBasics->getLastLoginTs();
        if(pGuildUserBasics->getUserId() == m_pGuildInfo->getAssistant1() || pGuildUserBasics->getUserId() == m_pGuildInfo->getAssistant2()|| pGuildUserBasics->getUserId() == m_pGuildInfo->getAssistant3()){
			if(leftTime<D_CONFIG_FLOAT_FOR_KEY(GUILD_LEAVE_TIME)){
                assistantIsNotOnline = true;
            }
        }
        
        if(pGuildUserBasics->getUserId()==myId){
            leftTime = GameManager::sharedGameManager()->getServerTime();
        }
        
        if(pGuildUserBasics->getUserId() != m_pGuildInfo->getAssistant1() && pGuildUserBasics->getUserId() == m_pGuildInfo->getAssistant2()&& pGuildUserBasics->getUserId() == m_pGuildInfo->getAssistant3() &&pGuildUserBasics->getUserId() != m_pGuildInfo->getLeaderID()){
            uint64_t leftTime = GameManager::sharedGameManager()->getServerTime() - pGuildUserBasics->getLastLoginTs();
            if(pGuildUserBasics->getUserId() == myId){
                myContribution = pGuildUserBasics->getContribution();
            }
            if(leftTime<D_CONFIG_FLOAT_FOR_KEY(GUILD_LEAVE_TIME)){
                if(pGuildUserBasics->getContribution()>maxContribution)
                    maxContribution = pGuildUserBasics->getContribution();
            }
        }
	}
    
    //我不是助理 助理在线
    if(assistantIsNotOnline){
        return 2;
    }
    if(maxContribution==myContribution){
        return 1;
    }else{
        return 3;
    }

}



void GuildModel::setOverlordTimes(uint32_t iTimes)
{
	CCAssert(m_pSelfInfo != NULL && m_pGuildInfo != NULL, "information of guild is lost");
/*	if (iTimes > m_pSelfInfo->m_iOverlord)
	{
		guild::GuildCoins *coins = (guild::GuildCoins *)m_pGuildCoinsMap->objectForKey(iTimes);
		setSelfContribution(m_pSelfInfo->getContribution() + coins->contribution);
		setGuildContribution(m_pGuildInfo->getContribution() + coins->contribution);
	}*/
	m_pSelfInfo->m_iOverlord = iTimes;
	guild::GuildCoins *coins = (guild::GuildCoins *)m_pGuildCoinsMap->objectForKey(iTimes + 1);
	if (NULL == coins)
	{
		m_pSelfInfo->m_iCoinsNeeded = 0;
	}
	else
	{
		m_pSelfInfo->m_iCoinsNeeded = coins->coins;
	}
}

guild::GuildTreeSkill *GuildModel::getTreeSkillByLevelAndType(uint32_t iLevel, uint32_t iType)
{
	if (1 == iType)
	{
		return (guild::GuildTreeSkill *)m_pTreeOverlordSkillMap->objectForKey(iLevel);
	}
	else
	{
		return (guild::GuildTreeSkill *)m_pTreeChivalrySkillMap->objectForKey(iLevel);
	}
}

void GuildModel::reset()
{
	isSynced = false;
	if (m_pGuildNotice)
	{
		m_pGuildNotice->removeAllObjects();
		m_pGuildNotice->release();
		m_pGuildNotice = NULL;
	}
	if (m_pGuildApplication)
	{
		m_pGuildApplication->removeAllObjects();
		m_pGuildApplication->release();
		m_pGuildApplication = NULL;
	}
	if (m_pSelfApplication)
	{
		m_pSelfApplication->removeAllObjects();
	}
// 	if (m_pGuildUser)
// 	{
// 		m_pGuildUser->removeAllObjects();
// 	}
	if (m_pGuildList)
	{
		m_pGuildList->removeAllObjects();
	}
	if (m_pOtherGuildMap)
	{
		m_pOtherGuildMap->removeAllObjects();
	}
	if (m_pOtherGuildUsersMap)
	{
		m_pOtherGuildUsersMap->removeAllObjects();
	}
}

std::string GuildModel::getPositionByType(guild::GuildPosition iPostion)
{
	if (guild::GuildPositionAssistant == iPostion)
	{
		return D_LOCAL_STRING("GuildAssistant");
	}
	else if (guild::GuildPositionPresident == iPostion)
	{
		return D_LOCAL_STRING("GuildLeader");
	}
	else
	{
		return D_LOCAL_STRING("GuildMember");
	}
}

const guild::GuildUserBasics* GuildModel::getGuildUserBasicsByUserId(uint32_t userId) const
{
	if (!m_pGuildUser)
	{
		return NULL;
	}

	CCObject* obj = NULL;
	guild::GuildUserBasics* pGuildUserBasics = NULL;
	CCARRAY_FOREACH(m_pGuildUser, obj)
	{
		pGuildUserBasics = (guild::GuildUserBasics*)obj;
		if (pGuildUserBasics && pGuildUserBasics->getUserId() == userId)
		{
			return pGuildUserBasics;
		}
	}

	return NULL;
}

CCArray *GuildModel::getGuildNoticeByType(guild::GuildNoticeType iType)
{
	if (!m_pSelfInfo->getGuildID())
	{
		return NULL;
	}
	if (NULL == m_pGuildNotice || NULL == m_pGuildNotice->objectForKey((int)iType))
	{
		HTTP_CLIENT->guildGetNotice((int)iType);
		return NULL;
	}
	return (CCArray *)m_pGuildNotice->objectForKey((int)iType);
}


void GuildModel::addGuildNotice(guild::GuildNotice *pNotice, guild::GuildNoticeType iType /* = guild::GuildNoticeTypeNone */)
{
	CCArray *pNoticeArray = getGuildNoticeByType(iType);
	if (NULL == pNoticeArray)
	{
		return;
	}
	int noticeMax = 2;
	if (GuildNoticeTypeNone == iType)
	{
		noticeMax = 3;
	}
	while (pNoticeArray->count() >= noticeMax)
	{
		pNoticeArray->removeLastObject();
	}
	pNoticeArray->insertObject(pNotice, 0);
}

void GuildModel::addGuildNotice(const Json::Value &noticeData)
{
	guild::GuildNotice *pNotice = parseOneNotice(noticeData);
	if (!pNotice)
	{
		return;
	}
	addGuildNotice(pNotice, pNotice->getType());
	addGuildNotice(pNotice);
	postNotification(D_EVENT_GUILD_GET_NOTICE);
}

void GuildModel::resetGuild(float dt)
{
	reset();
}

void GuildModel::quitGuild()
{
	getSelfInfo()->setGuildID(0);
	reset();
	postNotification(D_EVENT_GUILD_QUIT);
	HTTP_CLIENT->guildGet();
}

void GuildModel::changeSelfApplication(uint32_t guildID, uint32_t guildAction)
{
	for (uint32_t i = 0; i < m_pGuildList->count(); i++)
	{
		guild::GuildBasics *pData = (guild::GuildBasics *)m_pGuildList->objectAtIndex(i);
		if (pData->getGuildID() == guildID)
		{
			pData->setIsApplied(1 == guildAction);
			pData->isSynced = true;
			break;
		}
	}
	if (1 == guildAction)
	{
		guild::GuildApplication *application = guild::GuildApplication::create();
		application->setGuildID(guildID);
		m_pSelfApplication->addObject(application);
	}
	else
	{
		uint32_t removeIndex = 0;
		for (uint32_t i = 0; i < m_pSelfApplication->count(); i++)
		{
			guild::GuildApplication *pData = (guild::GuildApplication *)m_pSelfApplication->objectAtIndex(i);
			if (pData->getGuildID() == guildID)
			{
				removeIndex = i;
				break;
			}
		}
		m_pSelfApplication->removeObjectAtIndex(removeIndex);
	}
}


void GuildModel::selfReplaceBoss(uint32_t king){
    uint32_t myId = DM_GET_USER_MODEL->getUserInfo()->getUserId();
    guild::GuildUserBasics* selfUserInfo = NULL;
    guild::GuildUserBasics* otherUserInfo = NULL;
    
	CCObject* obj = NULL;
	guild::GuildUserBasics* pGuildUserBasics = NULL;
	CCARRAY_FOREACH(m_pGuildUser, obj)
	{
		pGuildUserBasics = (guild::GuildUserBasics*)obj;
        if(pGuildUserBasics->getUserId() == myId){
            selfUserInfo = pGuildUserBasics;
        }
        if(pGuildUserBasics->getPosition() == guild::GuildPositionPresident)
            otherUserInfo = pGuildUserBasics;
	}
    
    selfUserInfo->isSynced = true;
    otherUserInfo->isSynced = true;
    
    guild::GuildPosition mypos =  selfUserInfo->getPosition();
    selfUserInfo->setPosition(guild::GuildPositionPresident);
    otherUserInfo->setPosition(mypos);
    
    m_pGuildInfo->setLeaderID(m_pSelfInfo->getUserId());
    
    if(m_pGuildInfo->getAssistant1()==m_pSelfInfo->getUserId()){
        m_pGuildInfo->setAssistant1(otherUserInfo->getUserId());
    }else if(m_pGuildInfo->getAssistant2()==m_pSelfInfo->getUserId()){
        m_pGuildInfo->setAssistant2(otherUserInfo->getUserId());
    }else if(m_pGuildInfo->getAssistant3()==m_pSelfInfo->getUserId()){
        m_pGuildInfo->setAssistant3(otherUserInfo->getUserId());
    }
    
    
    m_pSelfInfo->setPosition(guild::GuildPositionPresident);
    m_pSelfInfo->setKnighthoodLevel(0);
    m_pSelfInfo->setKnighthoodName(std::string(D_LOCAL_STRING("GuildLeader")));
    
    selfUserInfo->setPosition(guild::GuildPositionPresident);
    selfUserInfo->setKnighthoodLevel(0);
    selfUserInfo->setKnighthoodName(std::string(D_LOCAL_STRING("GuildLeader")));
    
    
    otherUserInfo->setKnighthoodLevel(king);
    guild::GuildKnighthoodLevel *level = (guild::GuildKnighthoodLevel *)m_pGuildKnighthoodMap->objectForKey(king);
    if(level){
        otherUserInfo->setKnighthoodName(level->name);
    }
    
    
    
    postNotification(D_EVENT_GUILD_GET);
}


void GuildModel::dealGuildApplication(uint32_t applicationID, uint32_t guildAction)
{
	uint32_t removeIndex = 0;
	for (uint32_t i = 0; i < m_pGuildApplication->count(); i++)
	{
		guild::GuildApplication *pData = (guild::GuildApplication *)m_pGuildApplication->objectAtIndex(i);
		if (pData->getApplicationID() == applicationID)
		{
			removeIndex = i;
			//if (guildAction == 1)
			//{
			//	guild::GuildUserBasics *pUser = guild::GuildUserBasics::create();
			//	pUser->setUserId(pData->getUserId());
			//	pUser->setName(pData->getName());
			//	pUser->setLevel(pData->getLevel());
			//	pUser->setContribution(0);
			//	//pUser->setKnighthoodLevel(1);
			//	pUser->setKnighthoodName(GetKnighthoodNameByLevel(1));
			//	m_pGuildUser->addObject(pUser);
			//	postNotification(D_EVENT_GUILD_GET);
			//}

			break;
		}
	}
	m_pGuildApplication->removeObjectAtIndex(removeIndex);
    postNotification(D_EVENT_GUILD_APPLY_LIST);
}

void GuildModel::changePosition(const Json::Value &requestData, const Json::Value &responseData)
{
	guild::GuildPosition position = (guild::GuildPosition)requestData["pid"].asUInt();
	uint32_t userID = requestData["user"].asUInt();
	if (guild::GuildPositionAssistant == position)
	{
		if (m_pGuildInfo->getAssistant1() == 0)
		{
			m_pGuildInfo->setAssistant1(userID);
		}
		else if (m_pGuildInfo->getAssistant2() == 0)
		{
			m_pGuildInfo->setAssistant2(userID);
		}
		else if (m_pGuildInfo->getAssistant3() == 0)
		{
			m_pGuildInfo->setAssistant3(userID);
		}
	}
	else
	{
		if (m_pGuildInfo->getAssistant1() == userID)
		{
			m_pGuildInfo->setAssistant1(0);
		}
		else if (m_pGuildInfo->getAssistant2() == userID)
		{
			m_pGuildInfo->setAssistant2(0);
		}
		else if (m_pGuildInfo->getAssistant3() == userID)
		{
			m_pGuildInfo->setAssistant3(0);
		}
	}
	guild::GuildUserBasics *pUser = NULL;
	int index = 0;
	for (uint32_t i = 0; i < m_pGuildUser->count(); i++)
	{
		pUser = (guild::GuildUserBasics *)m_pGuildUser->objectAtIndex(i);
		if (pUser->getUserId() == userID)
		{
			index = i;
			break;
		}
	}
	if (pUser)
	{
		pUser->setPosition(position);
		pUser->isSynced = true;
		if (guild::GuildPositionPresident == position)
		{
			pUser->setKnighthoodName(D_LOCAL_STRING("GuildLeader"));
			m_pSelfInfo->setPosition(guild::GuildPositionMember);
			uint32_t contributionLevel = 1;
			DJSON_UINT(contributionLevel, responseData["guildUser"], "con_lv");
			m_pSelfInfo->setKnighthoodLevel(contributionLevel);
			refreshContribution();
			m_pGuildInfo->setLeaderID(userID);
			guild::GuildUserBasics *selfUser = NULL;
			for (uint32_t i = 0; i < m_pGuildUser->count(); i++)
			{
				selfUser = (guild::GuildUserBasics *)m_pGuildUser->objectAtIndex(i);
				if (selfUser->getUserId() == m_pSelfInfo->getUserId())
				{
					selfUser->isSynced = true;
					selfUser->setPosition(guild::GuildPositionMember);
					selfUser->setKnighthoodName(m_pSelfInfo->getKnighthoodName());
					break;
				}
			}
		}
		else if (guild::GuildPositionRemoved == position)
		{
			m_pGuildUser->removeObjectAtIndex(index);
		}
	}
	postNotification(D_EVENT_GUILD_GET);
}

void GuildModel::errorNoGuild()
{
	getSelfInfo()->setGuildID(0);
	reset();
	HTTP_CLIENT->guildGet();
	postNotification(D_EVENT_GUILDBOSS_ERROR_NO_GUILD);
}
