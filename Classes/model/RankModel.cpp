#include "RankModel.h"
#include "utils/TimerUtil.h"
#include "managers/GameManager.h"

USING_NS_CC;
using namespace rank;

RankModel::RankModel()
	:m_pFightInfoList(NULL),
	m_pLevelInfoList(NULL),
	m_pMyFightInfo(NULL),
	m_pMyLevelInfo(NULL)
{

}
RankModel::~RankModel()
{
	CC_SAFE_RELEASE(m_pFightInfoList);
	CC_SAFE_RELEASE(m_pLevelInfoList);
	CC_SAFE_RELEASE(m_pMyLevelInfo);
	CC_SAFE_RELEASE(m_pMyFightInfo);
}
bool RankModel::init()
{
	m_pFightInfoList = CCArray::create();
	CC_SAFE_RETAIN(m_pFightInfoList);

	m_pLevelInfoList = CCArray::create();
	CC_SAFE_RETAIN(m_pLevelInfoList);

	m_pMyFightInfo = UserRankInfo::create();
	CC_SAFE_RETAIN(m_pMyFightInfo);

	m_pMyLevelInfo = UserRankInfo::create();
	CC_SAFE_RETAIN(m_pMyLevelInfo);
	//半个小时同步一次排行榜
	TimerUtil::sharedTimerUtil()->schedule(schedule_selector(RankModel::getRankInfo), this, 1800);
	//getRankInfo()
	return Model::init();
}

void RankModel::getRankInfo(float dt)
{
	HTTP_CLIENT->getRankInfo(this, callfuncJson_selector(RankModel::parseRankInfo));

}

void RankModel::parseRankInfo(const Json::Value &requestData, const Json::Value &responseData){
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	const Json::Value & data = responseData["data"];
	if(data.isMember("user_fight"))
	{
		parseRank(data["user_fight"], m_pMyFightInfo);
	}
	if(data.isMember("list_fight"))
	{
		m_pFightInfoList->removeAllObjects();
		const Json::Value & fightList = data["list_fight"];
		uint32_t iSize = fightList.size();
		UserRankInfo* pFightInfo = NULL;
		for(uint32_t i=0; i<iSize; i++)
		{
			pFightInfo = UserRankInfo::create();
			parseRank(fightList[i], pFightInfo);
			pFightInfo->m_pType = kForceRank;
			m_pFightInfoList->addObject(pFightInfo);
		}
	}
	if(data.isMember("user_level"))
	{
		parseRank(data["user_level"], m_pMyLevelInfo);
	}
	if(data.isMember("list_level"))
	{
		m_pLevelInfoList->removeAllObjects();
		const Json::Value & levelList = data["list_level"];
		uint32_t iSize = levelList.size();
		UserRankInfo* pLevelInfo = NULL;
		for(uint32_t i=0; i<iSize; i++)
		{
			pLevelInfo = UserRankInfo::create();
			parseRank(levelList[i], pLevelInfo);
			pLevelInfo->m_pType = kLevelRank;
			m_pLevelInfoList->addObject(pLevelInfo);
		}
	}
	this->postNotification(RANK_LIST_UPDATE);
}


UserRankInfo* RankModel::parseRank(const Json::Value & val, UserRankInfo* info)
{
	if(!info)
	{
		info = UserRankInfo::create();
	}
	DJSON_UINT(info->m_iID, val, "id");
	DJSON_UINT(info->m_iUserID, val, "userId");
	DJSON_UINT(info->m_iRank, val, "rank");
	DJSON_UINT(info->m_iNum, val, "num");
	DJSON_UINT(info->m_iVip, val, "vip");
	DJSON_STRING(info->m_iName, val, "name");
	info->isSynced = true;
	return info;
}

CCArray* RankModel::getRankListByType(rank::RankType type)
{
	CCArray* pRetList = NULL;
	switch(type)
	{
	case kForceRank:
		pRetList = m_pFightInfoList; break;
	case kLevelRank:
		pRetList = m_pLevelInfoList; break;

	}
	return pRetList;
}
