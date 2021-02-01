//
//  PvpModel.cpp
//  QSMY
//
//  Created by cll on 13-5-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PvpModel.h"
//#include "FormationModel.h"
//#include "WarriorModel.h"
#include "../managers/GameManager.h"
#include "OtherFormationModel.h"
#include "WarriorModel.h"
#include "ItemModel.h"

using namespace pvp;
USING_NS_CC;

//#define  PVP_TARGET_LIST_NUM		18			//TOP10+可看到的玩家总数

#define  K_PVP_TARGET_LEVEL_RATIO		(D_CONFIG_FLOAT_FOR_KEY(PVP_TARGET_LEVEL_RATIO))		//计算等级系数



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


#pragma mark AwardInfo

void AwardInfo::loadLine(CSVDecoder::CSVLine line)
{
    m_nId = atoi(line[0].c_str());
    m_eType = (pvp::AwardInfo::AwardType)atoi(line[1].c_str());
    m_nThreshold = atoi(line[2].c_str());
    m_sDesc = line[3].c_str();
    m_nItemId = atoi(line[4].c_str());
    m_nItemNum = atoi(line[5].c_str());
}

#pragma mark ChallengeRank

void ChallengeRank::loadData(const Json::Value &data)
{
	DJSON_UINT(m_nRank,data,"rank");
}

/*
 论剑积分：玩家每在一个排名上停留时间（按10分钟计数）即可获得相应的积分，积分产出规则见
 100名后：Int(Log（当前排名/15000,0.96）/4)+10
 前一百名：= CEILING(41+(100-当前排名)/20,1)
 前五十名：=CEILING(40+(100-当前排名)/10,1)
 前十名：=CEILING(49+(10-当前排名)^1.5,1)
 */
uint32_t ChallengeRank::getPointGrow(){
	//新规则 读表
	CCArray* datalist = DM_GET_PVP_MODEL->getArrArena();
	uint32_t grow = 0;
	CCObject* obj = NULL;
	CCARRAY_FOREACH(datalist, obj)
	{
		ArenaInfo * arenainfo = dynamic_cast<ArenaInfo *>(obj);
		uint32_t RankFirst = arenainfo->getRankFirst();
		uint32_t RankLast = arenainfo->getRankLast();

		if(m_nRank >= RankFirst && m_nRank <= RankLast)
		{
			//在区间内
			grow = arenainfo->getScoreLow() + (RankLast - m_nRank)*arenainfo->getScoreStep();
			break;
		}
	}
	return grow;



	/*uint32_t grow = 0;

	if (m_nRank > 100)
	{
	grow = (uint32_t)(log((float)m_nRank / K_PVP_TARGET_RANK_MAX ) / log(K_PVP_TARGET_LEVEL_RATIO) / 4) + 10;
	}
	else if (m_nRank > 50)
	{
	grow = ceil((float)(41 + (100 - m_nRank) / 20));
	}
	else if (m_nRank > 10)
	{
	grow = ceil((float)(40 + (100 - m_nRank) / 10));
	}
	else
	{
	grow = ceil((float)((49 + pow(float(10 - m_nRank), 1.5f))));
	}

	return grow/3;*/
}

#pragma mark ChallengeInfo

void ChallengeInfo::loadData(const Json::Value &data)
{
    ChallengeRank::loadData(data);
    
	DJSON_UINT(m_nBestRank, data, "bRank");
	DJSON_UINT(m_nPoint, data, "point");
	DJSON_UINT(m_nRemainChallengeTimes, data, "rct");
	DJSON_UINT(m_nRemainBossTimes, data, "rbt");
	DJSON_UINT(     m_nWin, data, "win" );
	DJSON_UINT(     m_nLose, data, "lose" );
    
	m_pDicAwards->removeAllObjects();
    const Json::Value & arr = data["rewards"];
    for(Json::Value::iterator iter(arr.begin()); iter != arr.end(); ++iter)
    {
        int id = (*iter).asInt();
		CCInteger  * pId = CCInteger::create(id);
		m_pDicAwards->setObject(pId, id);
    }
	isSynced = true;
}

void ChallengeInfo::getTargetRanks(std::deque<uint32_t> & ranks)
{
    ranks.clear();
    ranks.push_back(m_nRank);
    insertUpperRanks(ranks);
    insertLowerRanks(ranks);
}

void ChallengeInfo::getTargetRanks(std::deque<uint32_t> & ranks, const Json::Value &val)
{
	ranks.clear();
	ranks.push_back(m_nRank);
	for(Json::Value::iterator iter(val.begin()); iter != val.end(); ++iter)
	{
		uint32_t rank = (*iter).asUInt();
		if (rank < m_nRank)
		{
			ranks.push_front(rank);
		}
		else
		{
			ranks.push_back(rank);
		}
	}
}

//在队列头部添加可挑战的排名
void ChallengeInfo::insertUpperRanks(std::deque<uint32_t> & ranks){
    uint32_t rank = m_nRank;
    
    for (uint16_t i = 0; i < D_CONFIG_UINT_FOR_KEY(PVP_UPPER_RANK_NUM); ++i)
    {
        if (rank > 100)
        {
            rank = (uint32_t)(rank * K_PVP_TARGET_LEVEL_RATIO - (i == 0 ? 2 : 1));
        }
        else if (rank > 25)
        {
            rank -= (i == 0 ? 6 : 5);
        }
        else if (rank > 10)
        {
            rank -= (i == 0 ? 4 : 3);
            if (rank < 10)
            {
                rank = 10;
            }
        }
        else
        {
            if (rank <= 1)
            {
                break;
            }
            rank -= 1;
        }
        
        ranks.push_front(rank);
    }
}

//在队列尾部添加后面的排名
void ChallengeInfo::insertLowerRanks(std::deque<uint32_t> & ranks){
    uint32_t rank = m_nRank;
    
    for (uint16_t i = 0; i < D_CONFIG_UINT_FOR_KEY(PVP_LOWER_RANK_NUM); ++i)
    {
        if (rank > 100)
        {
            rank = (uint32_t)((rank + 1) / K_PVP_TARGET_LEVEL_RATIO);
            if(rank > K_PVP_TARGET_RANK_MAX)
            {
                break;
            }
        }
        else if (rank > 25)
        {
            rank += 5;
        }
        else if (rank > 10)
        {
            rank += 3;
        }
        else
        {
            rank += 1;
        }
        
        ranks.push_back(rank);
    }
}

bool ChallengeInfo::bossVisible()
{
	return m_nRank <= D_CONFIG_UINT_FOR_KEY(PVP_RANK_SHOW_BOSS);
}

bool ChallengeInfo::bossChallengable()
{
	return m_nRank <= D_CONFIG_UINT_FOR_KEY(PVP_RANK_CHANLLENGE_BOSS);
}

#pragma mark ChTargetInfo

ChTargetInfo::~ChTargetInfo()
{
    CC_SAFE_RELEASE_NULL(m_pRobotFormation);
}

void ChTargetInfo::loadData(const Json::Value &data)
{
    ChallengeRank::loadData(data);
    
	DJSON_UINT(     m_nUserId,      data, "uid" );
	DJSON_CSTRING(  m_sName,        data, "name");
	DJSON_UINT(     m_nLevel,       data, "lv"  );
	DJSON_UINT(     m_nVipLevel,       data, "vip"  );
	DJSON_UINT(     m_nLeaderId,    data, "lid" );
    DJSON_UINT(     m_nLdid,        data, "ldid");
	DJSON_UINT(     m_nLeaderLevel, data, "llv" );
	DJSON_UINT(     m_nWin, data, "win" );
	DJSON_UINT(     m_nLose, data, "lose" );
}

ChTargetInfo * ChTargetInfo::createRobot(uint32_t rank)
{
    ChTargetInfo * target = ChTargetInfo::create();
    target->m_nRank = rank;
    target->setRobotFormation();
    target->setRobotLevel();
    
    return target;
}

ChTargetInfo * ChTargetInfo::createSelf(ChallengeInfo* user)
{
    ChTargetInfo * target = ChTargetInfo::create();
    target->m_nUserId = DM_GET_USER_MODEL->getUserInfo()->getUserId();
    target->m_sName = DM_GET_USER_MODEL->getUserInfo()->getUserName();
    target->m_nRank = user->getRank();
    target->m_nLevel = DM_GET_USER_MODEL->getUserInfo()->getUserLv();
	target->m_nVipLevel = DM_GET_USER_MODEL->getUserInfo()->getVipLevel();
    target->m_nLeaderId = DM_GET_FORMATION_MODEL->getFormationLeader();
    target->m_nLeaderLevel = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(target->m_nLeaderId)->getWarriorLv();
	target->m_nWin = user->getWin();
	target->m_nLose = user->getLose();
    
    return target;
}

void ChTargetInfo::setRobotFormation()
{
    uint32_t robotId(0);
	uint32_t robotIndex = D_CONFIG_UINT_FOR_KEY(PVP_ROBOT_FORMATION_INDEX);
	if (m_nRank < 10)
	{
        robotId = robotIndex + m_nRank;
    }
	else if(m_nRank <= 32)
	{
        robotId = robotIndex + 10 + m_nRank % 10;
    }
	else if(m_nRank <= 168)
	{
        robotId = robotIndex + 20 + m_nRank % 10;
    }
	else if(m_nRank <= 793)
	{
        robotId = robotIndex + 30 + m_nRank % 10;
    }
	else if(m_nRank <= 5631)
	{
        robotId = robotIndex + 40 + m_nRank % 10;
    }
	else if(m_nRank <= K_PVP_TARGET_RANK_MAX)
    {
        robotId = robotIndex + 50 + m_nRank % 10;
    }
    else
    {
        robotId = robotIndex + 50 + K_PVP_TARGET_RANK_MAX % 10;
    }
    m_pRobotFormation = DM_GET_OTHERFORMATION_MODEL->getRobotFormationByFid(robotId);
    CC_SAFE_RETAIN(m_pRobotFormation);
    m_sName = m_pRobotFormation->name;
    m_nLeaderId = m_pRobotFormation->getLeaderId();
}

void ChTargetInfo::setRobotLevel()
{    
	//修改参数 与服务器端保持一致
	if(m_nRank <= 200)
    {
		m_nLevel = floor(log((float)m_nRank/K_PVP_TARGET_RANK_MAX)/log(K_PVP_TARGET_LEVEL_RATIO)/10 + 7);
    }
	else if(m_nRank <= K_PVP_TARGET_RANK_MAX)
	{
		//int max = K_PVP_TARGET_RANK_MAX;
		//float ratio = K_PVP_TARGET_LEVEL_RATIO;
		//float log1 = log((float)m_nRank/K_PVP_TARGET_RANK_MAX);
		//float log2 = log(K_PVP_TARGET_LEVEL_RATIO);
		//float divide = log1/log2/12;
		//int level = floor(divide + 1);

		m_nLevel = floor(log((float)m_nRank/K_PVP_TARGET_RANK_MAX)/log(K_PVP_TARGET_LEVEL_RATIO)/12 + 1);
    }
	else
    {
		m_nLevel = 6;
    }
    m_nLeaderLevel = m_nLevel * 3;
}

#pragma mark PvpModel

PvpModel::PvpModel()
:m_pChallengeInfo(NULL)
,m_pArrTargets(NULL)
,m_pDicTopRanks(NULL)
,m_pArrAwards(NULL)
,m_nUserIndex(0)
,m_pArrChMall(NULL)
,m_pArrArena(NULL)
,m_uRefreshTs(0)
,m_BestEver(false)
,m_uTargetID(0)
,m_pMallUpData(NULL)
{
    
}

PvpModel::~PvpModel()
{
    CC_SAFE_RELEASE_NULL(m_pChallengeInfo);
    CC_SAFE_RELEASE_NULL(m_pArrTargets);
    CC_SAFE_RELEASE_NULL(m_pDicTopRanks);
    CC_SAFE_RELEASE_NULL(m_pArrAwards);
	CC_SAFE_RELEASE_NULL(m_pArrChMall);
	CC_SAFE_RELEASE_NULL(m_pArrArena);
	CC_SAFE_RELEASE_NULL(m_pMallUpData);
}

bool PvpModel::init()
{    
    m_pChallengeInfo = ChallengeInfo::create();
    CC_SAFE_RETAIN(m_pChallengeInfo);
    
    m_pArrTargets = cocos2d::CCArray::create();
    CC_SAFE_RETAIN(m_pArrTargets);
    
    m_pDicTopRanks = CCDictionary::create();
    CC_SAFE_RETAIN(m_pDicTopRanks);
    
    m_pArrAwards = AwardArray::create();
    CC_SAFE_RETAIN(m_pArrAwards);
    
	m_pArrChMall = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pArrChMall);

	m_pArrArena = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pArrArena);
	m_pMallUpData = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pMallUpData);

	LoadArenaData (CSVDecoder::create(ARENA_CONFIG_PATH)->getData());
	LoadMallLvlData(CSVDecoder::create(PVP_MALL_LVL_UP_CONFIG_PATH)->getData());

	loadAwardData();

	uint32_t nServerTime = GameManager::sharedGameManager()->getServerTime();
	const char * sPrizeTime = D_CONFIG_CSTRING_FOR_KEY("PVP_PRIZE_TIME");
	int pt[3] = {0, 0, 0};
	getPartsFromStr(sPrizeTime, pt, 3);
	m_uPrizeTime = TimeUtil::getTimeToday(nServerTime, pt[0], pt[1]) - TimeUtil::getStartTimeOfToday(nServerTime);
    
    return Model::init();
}

void PvpModel::LoadArenaData(const CSVDecoder::CSV &val)
{
	m_pArrArena->removeAllObjects();
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		//不用读取全列
		ArenaInfo *pData = ArenaInfo::create();
		pData->m_rankFirst = atoi(val[i][0].c_str());
		pData->m_rankLast = atoi(val[i][1].c_str());
		pData->m_scoreLow = atoi(val[i][3].c_str());
		pData->m_scoreStep = atoi(val[i][4].c_str());
		m_pArrArena->addObject(pData);
		CCLog(CCString::createWithFormat("-------------------------------%d" , pData->m_scoreStep)->getCString());
	}
}

void PvpModel::LoadMallLvlData(const CSVDecoder::CSV &val)
{
	m_pMallUpData->removeAllObjects();
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		MallLvInfo *pData = MallLvInfo::create();
		pData->m_MallLvl = atoi(val[i][0].c_str());
		pData->m_NeedPoint = atoi(val[i][1].c_str());
		m_pMallUpData->addObject(pData);
	}
}

void PvpModel::getInitInfo()
{
    isSynced = false;
	HTTP_CLIENT->initPvpInfo();
}

void PvpModel::onInitComplete(const Json::Value & json)
{
    parseChallengeInfo(json["challenge"]);
    parseRanks(json);
	parseChMall(json["chMall"]);
    isSynced = true;
    
	postNotification(D_EVENT_PVP_INIT);
}

void PvpModel::onInfoComplete(const Json::Value & jsonInfo)
{
    parseChallengeInfo(jsonInfo);
}

void PvpModel::onTargetsComplete(const Json::Value & jsonTargets)
{
    isSynced = true;
    parseTargets(jsonTargets);
}

/**
 *	@brief	挑战东皇太一完成
 *
 *	@param 	requestUid 	请求唯一编号
 *	@param 	grade       评分
 */
void PvpModel::onChallengeBossComplete(uint32_t requestUid)
{
    if(m_nGrade > 0)
    {
        showDrop(requestUid);
    }
    
    m_pChallengeInfo->deductBossTimes();
    m_pChallengeInfo->deductChallengeTimes();
    postNotification(D_EVENT_PVPINFO_UPDATE);
    
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE,CCInteger::create(battle::kSkipBattleTypePVP));
}

/**
 *	@brief	挑战试炼木人完成
 *
 *	@param 	requestUid 	请求唯一编号
 *	@param 	grade       评分
 */
void PvpModel::onChallengeNpcComplete(uint32_t requestUid)
{
    if(m_nGrade > 0)
    {
        showDrop(requestUid);
    }    
    
    m_pChallengeInfo->deductChallengeTimes();
    postNotification(D_EVENT_PVPINFO_UPDATE);
    
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE,CCInteger::create(battle::kSkipBattleTypePVP));
}

void PvpModel::onChallengeTargetComplete(uint32_t requestUid, const Json::Value & jsonInfo, const Json::Value & jsonTargets)
{
    if(m_nGrade > 0)
    {
        showDrop(requestUid);
        
        parseChallengeInfo(jsonInfo);
        parseTargets(jsonTargets);        
    }
    else
    {
        m_pChallengeInfo->deductChallengeTimes();
        postNotification(D_EVENT_PVPINFO_UPDATE);
    }
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE,CCInteger::create(battle::kSkipBattleTypePVP));
}

void PvpModel::loadAwardData(){
    m_pArrAwards->removeAllObjects();
    CSVDecoder::CSV csv =  CSVDecoder::create(PVP_AWARD_INFO_CONFIG_PATH)->getData();
    
    for(CSVDecoder::CSV::iterator iter(csv.begin()); iter != csv.end(); ++iter)
    {
        AwardInfo * pAwardInfo = AwardInfo::create();
        pAwardInfo->loadLine(*iter);
        m_pArrAwards->addObject(pAwardInfo);
    }
}

void PvpModel::showDrop(uint32_t requestUid)
{
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW, CCInteger::create(requestUid));
}

void PvpModel::parseChallengeInfo(const Json::Value &val)
{
    m_pChallengeInfo->loadData(val);
    postNotification(D_EVENT_PVPINFO_UPDATE);
}

void PvpModel::parseRanks(const Json::Value &val)
{
    setRanksAndTargets(val, true);
}

void PvpModel::parseTargets(const Json::Value &val)
{
    setRanksAndTargets(val, false);
}

void PvpModel::setRanksAndTargets(const Json::Value & val, bool resetTopRank)
{
    std::deque<uint32_t> ranks;
    m_pChallengeInfo->getTargetRanks(ranks, val["chAvailRanks"]);
    
    cocos2d::CCDictionary * dic = cocos2d::CCDictionary::create();

	Json::Value targetVal = val["chTargets"];
	if (val.isMember("chRanks"))
	{
		targetVal = val["chRanks"];
	}
	
    //解析数据
    for(Json::Value::iterator iter(targetVal.begin()); iter != targetVal.end(); ++iter)
    {
        pvp::ChTargetInfo * target = pvp::ChTargetInfo::create();
        target->loadData(*iter);
        dic->setObject(target, target->getRank());
    }
    
    bool topRankChanged(false);
    if(resetTopRank)
    {
        //重置排行榜
        m_pDicTopRanks->removeAllObjects();
    }
    
    pvp::ChTargetInfo * self = pvp::ChTargetInfo::createSelf(m_pChallengeInfo);
    //设置排行榜
    for(size_t rank(1); rank <= D_CONFIG_UINT_FOR_KEY(PVP_TOPLIST_NUM); ++rank)
    {
        cocos2d::CCObject * obj = dic->objectForKey(rank);
        ChTargetInfo * target = NULL;
        
        //如果玩家在排行榜中，则将玩家放入排行榜
        if(rank == m_pChallengeInfo->getRank())
        {
            target = self;
        }
        else
        {
            if(obj != NULL)
            {
                target = (pvp::ChTargetInfo*) obj;
            }
            else if(resetTopRank)
            {
                target = pvp::ChTargetInfo::createRobot(rank);
            }
        }
        
        if(target != NULL)
        {
            m_pDicTopRanks->setObject(target, rank);
            topRankChanged = true;
        }
    }
    
    //设置目标
	unsigned int userindex = 0;
    m_pArrTargets->removeAllObjects();
    for(std::deque<uint32_t>::iterator iter(ranks.begin()); iter != ranks.end(); ++iter)
    {
        uint32_t rank(*iter);
		//玩家排在第几位
		if(rank > 10) userindex++;
        cocos2d::CCObject * obj = dic->objectForKey(rank);
        ChTargetInfo * target = NULL;
        //将自己插入目标列表
        if(rank == m_pChallengeInfo->getRank())
        {
            target = self;
			if(rank > 10)
			{
				m_nUserIndex = (userindex>=1 ? userindex - 1 : 0);
			}
			else{
				//玩家在前十名内
				m_nUserIndex = rank - 1;//m_pArrTargets->count();
			}            
        }
        else
        {
            if(obj != NULL)
            {
                target = (pvp::ChTargetInfo*) obj;
            }
            else
            {
                target = pvp::ChTargetInfo::createRobot(rank);
            }
        }
        
        m_pArrTargets->addObject(target);
    }
	postNotification(D_EVENT_PVPLIST_UPDATE);
    if(topRankChanged)
	{
        postNotification(D_EVENT_PVP_UPDATE_TOPLIST);
    }
}

void PvpModel::getTopRanks(cocos2d::CCDictionary* dic)
{
    if(dic == NULL) return;
    dic->removeAllObjects();
    cocos2d::CCDictElement* obj = NULL;
	pvp::ChTargetInfo* pTarget = NULL;
    
	CCDICT_FOREACH(m_pDicTopRanks, obj)
	{
		pTarget = (pvp::ChTargetInfo*)obj->getObject();
		dic->setObject(pTarget, obj->getIntKey());
	}
}

void PvpModel::getTarges(cocos2d::CCArray* arr)
{
    if(arr == NULL) return;
    arr->removeAllObjects();
    cocos2d::CCObject * obj;
    CCARRAY_FOREACH(m_pArrTargets, obj)
    {
        arr->addObject(obj);
    }
}

AwardInfo * PvpModel::getAward(uint32_t id)
{
	CCObject* obj = NULL;
	pvp::AwardInfo* pAwardInfo = NULL;

	CCARRAY_FOREACH(m_pArrAwards, obj)
	{
		pAwardInfo = (pvp::AwardInfo*)obj;
		if(pAwardInfo->getId() == id)
			return pAwardInfo;
	}

	return NULL;
}

/* 领取奖励 */
void PvpModel::drawAward(uint32_t requestUid, const pvp::AwardInfo * pAward)
{
	if (pAward->getType() == AwardInfo::kAwardTypeExchange)
	{
		m_pChallengeInfo->deductPoint(pAward->getThreshold());
		postNotification(D_EVENT_PVPINFO_UPDATE);
	}
	else if (pAward->getType() == AwardInfo::kAwardTypeRank)
	{
        m_pChallengeInfo->drawAward(pAward->getId());
        postNotification(D_EVENT_PVP_UPDATE_AWARDLIST);
	}
    showDrop(requestUid);
}

void PvpModel::handleError(const Json::Value &request, const Json::Value &response)
{
    if(response["code"].asString().compare(D_CONFIG_CSTRING_FOR_KEY(PVP_REFRESH_TARGET_ERROR))==0)
    {
        getInitInfo();
    }
	//else{
	//	DM_GET_ERRORTIP_MODEL->showErrorTips(response["code"].asString());
	//}
	/*else if(response["code"].asString().compare("2210")==0)
	{
		Tips_Error("Endurence is not enough");
	}*/
}

void PvpModel::parseChMall(const Json::Value & val)
{
	DJSON_UINT(m_uRefreshTs, val, "refresh_ts");
	DJSON_UINT(m_uMallLv, val, "lv");
	DJSON_UINT(m_uExchangePoint, val, "exp");
	m_pArrChMall->removeAllObjects();
	for (Json::ValueIterator i = val["items"].begin(); i != val["items"].end(); i++)
	{
		Json::Value v = *i;
		ChMallItem *pItem = ChMallItem::create();
		DJSON_UINT(pItem->m_nItemID, v, "id");
		DJSON_UINT(pItem->m_nItemNum, v, "num");
		DJSON_UINT(pItem->m_nCost, v, "cost");
		DJSON_UINT(pItem->m_nID, v, "cId");

		uint32_t buytype = 0;
		DJSON_UINT(buytype, v, "buyType");
		pItem->m_nBuyType = pvp::ChMallItem::BuyType(buytype);

		unsigned int st = 0;
		DJSON_UINT(st, v, "status");
		pItem->m_bStatus = st;

		uint32_t type = 0;
		DJSON_UINT(type, v, "type");
		pItem->m_eItemType = drop::DropType(type);

		m_pArrChMall->addObject(pItem);
	}
	//乱序
	if(m_pArrChMall->objectAtIndex(0))
	{
		CCObject *first = m_pArrChMall->objectAtIndex(0);
		int randIdx = (int)(CCRANDOM_0_1() * (m_pArrChMall->count() - 1));
		m_pArrChMall->removeObject(first);
		m_pArrChMall->insertObject(first , randIdx);
	}
	

	postNotification(D_EVENT_PVP_EXMALL_UPDATE);
}

void PvpModel::exchanged(const Json::Value & request, const Json::Value & response)
{
	//条目ID
	uint32_t id = request["id"].asUInt();
	//cost
	uint32_t cost = 0;
	CCObject* obj = NULL;
	CCARRAY_FOREACH(m_pArrChMall, obj)
	{
		pvp::ChMallItem * pItem = dynamic_cast<pvp::ChMallItem *>(obj);
		if(pItem->getID() == id){
			cost = (pItem->getBuyType() == ChMallItem::kBuyTypeRank ? pItem->getCost() : 0);
			break;
		}
	}
	//reduce
	m_pChallengeInfo->m_nPoint -= cost;
	postNotification(D_EVENT_PVPINFO_UPDATE);
	parseChMall(response["chMall"]);

	/*uint32_t idx = request["index"].asUInt();
	pvp::ChMallItem *pItem = dynamic_cast<pvp::ChMallItem *>(m_pArrChMall->objectAtIndex(idx));
	m_pChallengeInfo->m_nPoint -= pItem->m_nCost;
	parseChMall(response["chMall"]);*/
}

//根据商城等级获取升级所需积分 -1达到最大等级
int PvpModel::getMallLvInfo(uint32_t lv)
{
	MallLvInfo* info = NULL;
	int num = 0;
	for(int i=0;i<m_pMallUpData->count();i++)
	{
		info = static_cast<MallLvInfo*>(m_pMallUpData->objectAtIndex(i));
		if(info->getMallLvl() == lv)
		{
			num = info->getNeedPoint();
			break;
		}
	}

	return num;
}