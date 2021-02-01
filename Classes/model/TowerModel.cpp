//
//  TowerModel.cpp
//  千机楼
//
//  Created by ldr123 on 13-07-01.
//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "TowerModel.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "utils/TimeUtil.h"
#include "utils/TimerUtil.h"

using namespace tower;


bool TowerInfo::hasRound()
{
    return todayRound()>0;
}

uint32_t TowerInfo::todayRound()
{
    int round = D_CONFIG_UINT_FOR_KEY(TOWER_MAX_NUM) - rnum;
    round = MAX(round, 0);
    return (uint32_t)round;
}

TowerExchangeInfo::~TowerExchangeInfo() {}

#pragma mark - TowerModel -

TowerModel::TowerModel()
:m_curTowerInfo(NULL)
,m_curTowerAdd(NULL)
,m_curTowerStatus(NULL)
,m_arrTowerContinue(NULL)
,m_nRankUpdateTime(0)
,m_nMyRank(0)
,m_pRanks(NULL)
,m_pOldRanks(NULL)
,m_pArrExchange(NULL)
,m_bUpdateOldRank(false)
{}

TowerModel::~TowerModel()
{
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
	CC_SAFE_RELEASE(m_curTowerInfo);
	CC_SAFE_RELEASE(m_curTowerAdd);
	CC_SAFE_RELEASE(m_curTowerStatus);
	CC_SAFE_RELEASE(m_arrTowerContinue);
	CC_SAFE_RELEASE(m_pRanks);
	CC_SAFE_RELEASE(m_pOldRanks);
	CC_SAFE_RELEASE(m_pArrExchange);
}

bool TowerModel::init()
{
	m_curTowerInfo = tower::TowerInfo::create();
	CC_SAFE_RETAIN(m_curTowerInfo);

	m_curTowerAdd = tower::TowerAdd::create();
	CC_SAFE_RETAIN(m_curTowerAdd);

	m_curTowerStatus = tower::TowerStatus::create();
	CC_SAFE_RETAIN(m_curTowerStatus);

	m_arrTowerContinue = TowerContinueArr::create();
	CC_SAFE_RETAIN(m_arrTowerContinue);
    
    m_pRanks = cocos2d::CCArray::create();
    CC_SAFE_RETAIN(m_pRanks);

	m_pOldRanks = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pOldRanks);

	m_pArrExchange = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pArrExchange);

	parseConinue(CSVDecoder::create("data/tower/towercontinue")->getData());
	loadExchangeInfo(CSVDecoder::create("data/tower/tower_exchange")->getData());

	dailyReset(0);

	return Model::init();
}

void TowerModel::tower_get(const Json::Value &val , bool notify)
{
	//m_curTowerInfo->bNeedShowBuff = false;

	DJSON_UINT(m_curTowerInfo->mid,		val, "mid");
	DJSON_UINT(m_curTowerInfo->mnum,	val, "mnum");
	DJSON_UINT(m_curTowerInfo->oldnum,	val, "oldnum");
	DJSON_UINT(m_curTowerInfo->scores,	val, "scores");
	DJSON_UINT(m_curTowerInfo->rnum,	val, "rnum");
	DJSON_UINT(m_curTowerInfo->rstatus, val, "rstatus");
	DJSON_UINT(m_curTowerInfo->item_lv, val, "item_lv");
	DJSON_UINT(m_curTowerInfo->_continue,val,"continue");
	DJSON_UINT(m_curTowerInfo->lid,		val, "lid");
	DJSON_UINT(m_curTowerInfo->lastHard, val, "last_hard");
	DJSON_UINT(m_curTowerInfo->points, val, "points");

	if (val.isMember("m_item"))
	{
		const Json::Value &valItem = val["m_item"];
		if (valItem.size() > 0)
		{
			DM_GET_ITEM_MODEL->parseItem(valItem, m_curTowerInfo->pItemInfo);
		}
	}

	if (val.isMember("buffstr"))
	{
		const Json::Value &valBuff = val["buffstr"];
		m_curTowerInfo->arrBuff->removeAllObjects();

		m_curTowerInfo->bNeedShowBuff = false;
		uint32_t valSize = valBuff.size();
		for (uint32_t i=0; i<valSize; ++i)
		{
			const Json::Value &valBuffTmp = valBuff[i];
			tower::TowerBuff *pObj = tower::TowerBuff::create();

			DJSON_UINT(pObj->id,	valBuffTmp, "id");
			DJSON_UINT(pObj->value, valBuffTmp, "value");
			DJSON_UINT(pObj->bid,	valBuffTmp, "bid");
			DJSON_UINT(pObj->cost,	valBuffTmp, "cost");

			m_curTowerInfo->arrBuff->addObject(pObj);

			m_curTowerInfo->bNeedShowBuff = true;
		}
	}

    isSynced = true;
    
    if(notify) postNotification(D_EVENT_TOWER_INFO_UPDATE);
	
}

void TowerModel::AddTowerScores(int nScore)
{
	if (nScore != 0)
	{
		m_curTowerInfo->scores += nScore;
	}
}

void TowerModel::ClearTowerBuff()
{
	m_curTowerInfo->arrBuff->removeAllObjects();
	m_curTowerInfo->bNeedShowBuff = false;
}

void TowerModel::tower_add(const Json::Value &val)
{
	DJSON_DOUBLE(m_curTowerAdd->fHp,	val, "hp");
	DJSON_DOUBLE(m_curTowerAdd->fAtt,	val, "att");
	DJSON_DOUBLE(m_curTowerAdd->fDef, val, "def");
	DJSON_DOUBLE(m_curTowerAdd->fSpeed, val, "speed");
	DJSON_DOUBLE(m_curTowerAdd->fDodge,	val, "dodge");
	DJSON_DOUBLE(m_curTowerAdd->fCrit,	val, "crit");
	DJSON_DOUBLE(m_curTowerAdd->fBlock, val, "block");

	postNotification(D_EVENT_TOWER_ADD_UPDATE);
}

tower::NavigationType TowerModel::GetCurrentType()
{
	if (m_curTowerStatus)
	{
		return m_curTowerStatus->navType;
	}

	return tower::KNavUnknow;
}

void TowerModel::SetCurrentType(tower::NavigationType navType)
{
	if (m_curTowerStatus)
	{
		m_curTowerStatus->navType = navType;
	}
}

void TowerModel::SetInBattle(bool bIn)
{
	if (m_curTowerStatus)
	{
		m_curTowerStatus->bInBattle = bIn;
	}
}

bool TowerModel::IsInBattle()
{
	if (m_curTowerStatus)
	{
		return m_curTowerStatus->bInBattle;
	}

	return false;
}

bool TowerModel::IsWin()
{
	if (m_curTowerStatus && m_curTowerStatus->pFightResult)
	{
		return m_curTowerStatus->pFightResult->bWin;
	}

	return false;
}

void TowerModel::SetFightResult(const Json::Value& val)
{
	if (!m_curTowerStatus)
	{
		return;
	}

	if (!m_curTowerStatus->pFightResult)
	{
		return;
	}

	DJSON_BOOL(m_curTowerStatus->pFightResult->bWin,	val, "win");
	m_curTowerStatus->bInBattle = true;
	m_curTowerStatus->pFightResult->arrBuffs->removeAllObjects();
	const Json::Value &valBuff = val["buff"];
	uint32_t valBuffSize = valBuff.size();
	for (uint32_t i=0; i<valBuffSize; ++i)
	{
		const Json::Value &valBuffTmp = valBuff[i];
		tower::TowerBuff *pObj = tower::TowerBuff::create();

		DJSON_UINT(pObj->id,	valBuffTmp, "id");
		DJSON_UINT(pObj->value, valBuffTmp, "value");
		DJSON_UINT(pObj->bid,	valBuffTmp, "bid");
		DJSON_UINT(pObj->cost,	valBuffTmp, "cost");

		m_curTowerStatus->pFightResult->arrBuffs->addObject(pObj);
	}
	//postNotification(D_EVENT_TOWER_INFO_UPDATE); 设置结果之后切到战场 不需要通知变化
    
}

void TowerModel::SetNeedChooseBuf(bool bNeed)
{
	if (m_curTowerInfo)
	{
		m_curTowerInfo->bNeedShowBuff = bNeed;
	}
}

bool TowerModel::IsNeedChooseBuf()
{
	if (m_curTowerInfo)
	{
		return m_curTowerInfo->bNeedShowBuff;
	}

	return false;
}

uint32_t TowerModel::getNeedCoins(tower::TowerContinueData * pContinueData)
{
    return pContinueData->uNeedCoins * (uint32_t)(powf(1.12f, (3*DM_GET_USER_MODEL->getUserInfo()->getUserLv()-1)/3.0f));
}

void TowerModel::parseConinue(const CSVDecoder::CSV &val)
{
	m_arrTowerContinue->removeAllObjects();
	for (uint32_t i=0; i<val.size(); ++i)
	{
		int iNum = atoi(val[i][0].c_str());
		if (iNum == 0)
		{
			continue;
		}

		tower::TowerContinueData *pData = tower::TowerContinueData::create();
		if (pData)
		{
			pData->uContinueCount	= (uint16_t)iNum;
			pData->uVipLev			= (uint16_t)(atoi(val[i][1].c_str()));
			pData->uNeedCoins		= (uint16_t)(atoi(val[i][2].c_str()));
			pData->uNeedPoint		= (uint16_t)(atoi(val[i][3].c_str()));
			pData->uGetBuff			= (uint16_t)(atof(val[i][4].c_str())*100);

			m_arrTowerContinue->addObject(pData);
		}
	}
}

void TowerModel::loadExchangeInfo(const CSVDecoder::CSV &val)
{
	m_pArrExchange->removeAllObjects();
	for (uint32_t i=0; i<val.size(); ++i)
	{
		TowerExchangeInfo *pInfo = TowerExchangeInfo::create();
		pInfo->m_uID = atoi(val[i][0].c_str());
		pInfo->m_uNeedPoint = atoi(val[i][1].c_str());
		pInfo->m_eType = (drop::DropType)atoi(val[i][2].c_str());
		pInfo->m_uItemID = atoi(val[i][3].c_str());
		pInfo->m_uNum = atoi(val[i][4].c_str());
		m_pArrExchange->addObject(pInfo);
	}
}

void TowerModel::getExchangeArray(CCArray *pArr)
{
	if (NULL == pArr)
	{
		return;
	}
	pArr->removeAllObjects();
	pArr->addObjectsFromArray(m_pArrExchange);
}

void TowerModel::updateTowerRank(bool force)
{
    bool update(true);
    do {
        CC_BREAK_IF(force);
        CC_BREAK_IF(m_nRankUpdateTime==0);
        CC_BREAK_IF(TimeUtil::getDiff(m_nRankUpdateTime, GameManager::sharedGameManager()->getServerTime())>=TOWER_RANK_REFRESH_INTERVAL);
        update = false;
    } while (0);
    
    if(update) HTTP_CLIENT->towerRank();
}

void TowerModel::updateTowerOldRank()
{
	if (m_bUpdateOldRank)
	{
		return;
	}
	
	HTTP_CLIENT->towerOldRank();
}

void TowerModel::updateTowerRanks(const Json::Value& ranks,uint32_t myRank,time_t lastUpdateTime)
{
    m_pRanks->removeAllObjects();
    
	uint32_t valSize = ranks.size();

	int nLastRank = 0;
	int nLastMNum = -1;
	for (uint32_t i=0; i<valSize; ++i)
	{
		tower::TowerRankInfo *pTmpInfo = tower::TowerRankInfo::create();
		const Json::Value &val1 = ranks[i];
		DJSON_UINT(pTmpInfo->m_id,			val1, "uid");
		DJSON_UINT(pTmpInfo->m_Lev,			val1, "lv");
		DJSON_UINT(pTmpInfo->m_lId,			val1, "lid");
        DJSON_UINT(pTmpInfo->m_ldid,        val1, "ldid");
		DJSON_UINT(pTmpInfo->m_todayNum,	val1, "mnum");
		DJSON_UINT(pTmpInfo->m_totalNum,	val1, "inrank");
		DJSON_STRING(pTmpInfo->m_strName,	val1, "name");

		if (nLastMNum != pTmpInfo->m_todayNum)
		{
			nLastRank++;

			nLastMNum = pTmpInfo->m_todayNum;
		}
		pTmpInfo->m_Pos = nLastRank;

		m_pRanks->addObject(pTmpInfo);
	}
    m_nMyRank = myRank;
    m_nRankUpdateTime = lastUpdateTime;
    
    postNotification(D_EVENT_TOWER_RANK_UPDATE);
}

void TowerModel::updateTowerOldRanks(const Json::Value& ranks)
{
	m_bUpdateOldRank = true;

	m_pOldRanks->removeAllObjects();

	uint32_t valSize = ranks.size();

	int nLastRank = 0;
	int nLastMNum = -1;
	for (uint32_t i=0; i<valSize; ++i)
	{
		tower::TowerRankInfo *pTmpInfo = tower::TowerRankInfo::create();
		const Json::Value &val1 = ranks[i];
		DJSON_UINT(pTmpInfo->m_id,			val1, "uid");
		DJSON_UINT(pTmpInfo->m_Lev,			val1, "lv");
		DJSON_UINT(pTmpInfo->m_lId,			val1, "lid");
		DJSON_UINT(pTmpInfo->m_ldid,        val1, "ldid");
		DJSON_UINT(pTmpInfo->m_todayNum,	val1, "mnum");
		DJSON_UINT(pTmpInfo->m_totalNum,	val1, "inrank");
		DJSON_STRING(pTmpInfo->m_strName,	val1, "name");

		if (nLastMNum != pTmpInfo->m_todayNum)
		{
			//nLastRank++;

			nLastMNum = pTmpInfo->m_todayNum;
		}
		nLastRank++;
		pTmpInfo->m_Pos = nLastRank;

		m_pOldRanks->addObject(pTmpInfo);
	}

	postNotification(D_EVENT_TOWER_OLD_RANK_UPDATE);
}

void TowerModel::reset()
{
	// 不接受零点重置
}

void TowerModel::dailyReset(float dt)
{
	isSynced = false;
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
	time_t now = GameManager::sharedGameManager()->getServerTime();
	TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(TowerModel::dailyReset), this,TimeUtil::getDiff(now, TimeUtil::getStartTimeOfTomorrow(now)));
}

/**
 *	@brief	缓存战斗信息
 *
 */
void TowerModel::cacheBattleInfo()
{
    //DM_GET_BATTLE_MODEL->serilization(m_battleInfo);
}

bool TowerModel::isCanReplayBattle()
{
    return m_battleInfo.size()>0;
}

/**
 *	@brief	重播战斗
 *
 */
void TowerModel::replayBattle()
{
    if(m_battleInfo.size()==0) return;
    DM_GET_BATTLE_MODEL->parseInfo(m_battleInfo);
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE,cocos2d::CCInteger::create(battle::kSkipBattleTypeSkip));
}


