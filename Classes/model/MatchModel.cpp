//
//  MatchModel.cpp
//  QSMY
//
//  Created by cll on 13-4-17.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MatchModel.h"
#include "managers/GameManager.h"
#include "utils/TimeUtil.h"
#include "managers/PlatformManager.h"

#define K_MATCH_LIVE_TIME (D_CONFIG_UINT_FOR_KEY(MATCH_LIVE_TIME))

using namespace match;

MatchModel::MatchModel()
:m_isSigned(false)
,m_matchInfoVec(NULL)
,m_VSInfoVec(NULL)
,m_betInfoVec(NULL)
,m_awardInfoVec(NULL)
,m_nlastUpdateTime(0)
,m_pMatchConfig(NULL)
,maxBet(0)
,m_iToSave(0)
,m_iMatchFinal(0)
{
	
}

MatchModel::~MatchModel()
{
    CC_SAFE_RELEASE(m_matchInfoVec);
	CC_SAFE_RELEASE(m_VSInfoVec);
	CC_SAFE_RELEASE(m_betInfoVec);
	CC_SAFE_RELEASE(m_awardInfoVec);
    CC_SAFE_RELEASE(m_pMatchConfig);
}


bool MatchModel::init()
{
	m_matchInfoVec = MatchInfoVec::create();
	m_VSInfoVec = VSInfoVec::create();
	m_betInfoVec = BetInfoVec::create();
	m_awardInfoVec = AwardInfoVec::create();
    m_pMatchConfig = match::MatchConfig::create();

	CC_SAFE_RETAIN(m_matchInfoVec);
	CC_SAFE_RETAIN(m_VSInfoVec);
	CC_SAFE_RETAIN(m_betInfoVec);
	CC_SAFE_RETAIN(m_awardInfoVec);
    CC_SAFE_RETAIN(m_pMatchConfig);
    parseMatchConfig( CSVDecoder::create(MATCH_CONFIG_CONFIG_PATH)->getData());
	m_pMatchConfig->parseMatchTime( CSVDecoder::create(MATCH_TIME_CONFIG_PATH)->getData());

	readSavedData();

	return Model::init();
}

void MatchModel::parseMatchConfig(const CSVDecoder::CSV& val)
{
    //m_pMatchConfig->m_nTotalBetTimes    =   atoi(val[0][0].c_str());
    m_pMatchConfig->m_nBetCost8         =   atoi(val[0][1].c_str());
    m_pMatchConfig->m_nBetCost4         =   atoi(val[0][2].c_str());
    m_pMatchConfig->m_nBetCost2         =   atoi(val[0][3].c_str());
    m_pMatchConfig->m_nMatch8StartTime  =   atoi(val[0][4].c_str());
    m_pMatchConfig->m_nMatch8EndTime    =   atoi(val[0][5].c_str());
    m_pMatchConfig->m_nMatch4EndTime    =   atoi(val[0][6].c_str());
    m_pMatchConfig->m_nMatch2EndTime    =   atoi(val[0][7].c_str());
	m_pMatchConfig->m_nSignStartTime    =   atoi(val[0][8].c_str());
	m_pMatchConfig->m_nMatch8ShowTime   =   atoi(val[0][9].c_str());
}


void MatchModel::signedUp()
{
    m_isSigned = true;
    if(isSynced)
	{
		postNotification(D_EVENT_MATCHINFO_UPDATE);
	}
	//弹出提示
	Tips_Alert(D_LOCAL_STRING("$beisaibaomingchenggong").c_str());
}

void MatchModel::mReward(uint32_t finalNum,uint32_t mid)
{
    MatchInfo *pMatch = getMatchInfo(finalNum, mid);
    pMatch->status = true;
    pMatch->postNotification(D_EVENT_MATCH_AWARD_STATUS_UPDATE);
}

match::MatchInfo* MatchModel::getMatchInfo(uint32_t finalNum,uint32_t mid)
{
    MatchInfo* pMath = NULL;
    CCObject*  pObj = NULL;
    CCARRAY_FOREACH(m_matchInfoVec, pObj)
    {
        pMath = (MatchInfo*)pObj;
        if(pMath->mid == mid && pMath->finalNum == finalNum)
        {
            return pMath;
        }
    }
    return NULL;
}

void MatchModel::parseSignUp(const Json::Value &val)
{
	DJSON_BOOL(m_isSigned,val,"signed");
    DJSON_UINT(maxBet, val, "maxBet");
    
    if(isSynced)
	{
		postNotification(D_EVENT_MATCHINFO_UPDATE);
	}
}


void MatchModel::parseMatchs(const Json::Value &val,uint32_t lastUpdateTime)
{
    m_matchInfoVec->removeAllObjects();
	int numLargerThanK8 = 0;
	for (uint32_t i=0; i<val.size(); ++i)
	{
		MatchInfo *pMatch = parseMatch(val[i]);
		if (pMatch->finalNum > k8)
		{
			numLargerThanK8++;
		}

		if (pMatch->finalNum >= m_iMatchFinal)
		{
			pMatch->watched = true;
		}
		else
		{
			pMatch->watched = false;
		}
		
		m_matchInfoVec->addObject(pMatch);
	}
	m_nlastUpdateTime = lastUpdateTime;
	if (numLargerThanK8) m_pMatchConfig->roundLargerThanK8 = numLargerThanK8;
    if(isSynced)
	{
		postNotification(D_EVENT_MATCHLIST_UPDATE);
	}
}

MatchInfo* MatchModel::parseMatch(const Json::Value &val, MatchInfo *pMatch)
{
	if(pMatch == NULL)
	{
		pMatch = MatchInfo::create();
	}

	DJSON_UINT(pMatch->finalNum,val,"finalNum");
	DJSON_UINT(pMatch->mid,val,"mid");
	DJSON_BOOL(pMatch->win,val,"win");
	DJSON_BOOL(pMatch->status,val,"status");
	
	parseOppDesp(val["oppDisp"],pMatch->oppDesp);
	return pMatch;
}

/* 获取竞猜列表*/
void MatchModel::parseBets(const Json::Value &val)
{
	for(uint32_t i=0; i <val.size(); i++)
	{
		BetInfo * pBetInfo = getBetInfo(val[i]["finalNum"].asUInt(), val[i]["mid"].asUInt());
        if(pBetInfo)
        {
            parseBetResult(val[i],pBetInfo);
            continue;
        }
        pBetInfo = parseBetResult(val[i]);
		m_betInfoVec->addObject(pBetInfo);
	}

	if(isSynced)
	{
		postNotification(D_EVENT_MATCH_UPDATE_BETLIST);
	}

}

BetInfo* MatchModel::getBetInfo(uint32_t finalNum,uint32_t mid)
{

	CCObject* obj = NULL;
	BetInfo* pBetInfo = NULL;

	CCARRAY_FOREACH(m_betInfoVec, obj)
	{
		pBetInfo = (BetInfo*)obj;
		if( pBetInfo->mid == mid && pBetInfo->finalNum == finalNum)
		{
			return pBetInfo;
		}
	}

	return NULL;
}

/* 获取竞猜信息 */
void MatchModel::parseBet(const Json::Value &val)
{
	BetInfo *pBetInfo = getBetInfo(val["finalNum"].asUInt(),val["mid"].asUInt());

	if (!pBetInfo)  //正常状态下 只有下注才会增加新竞猜信息  竞猜界面只需要监听列表变化 领奖界面需要监听信息变化
	{
		pBetInfo = parseBetResult(val);
		m_betInfoVec->addObject(pBetInfo);
		postNotification(D_EVENT_MATCH_UPDATE_BETLIST);
	}
	else
	{
		parseBetResult(val, pBetInfo);
		pBetInfo->postNotification(D_EVENT_MATCH_UPDATE_BETINFO);
		//postNotification(D_EVENT_MATCH_UPDATE_BETLIST);
	}

}

BetInfo* MatchModel::parseBetResult(const Json::Value &val, BetInfo *pBetInfo)
{
	if(pBetInfo == NULL)
	{
		pBetInfo = BetInfo::create();
	}

	DJSON_UINT(pBetInfo->id,val,"id");
	DJSON_UINT(pBetInfo->finalNum,val,"finalNum");
	DJSON_UINT(pBetInfo->mid,val,"mid");
	DJSON_UINT(pBetInfo->betUser,val,"betUser");
	DJSON_UINT(pBetInfo->result,val,"result");
	DJSON_BOOL(pBetInfo->status,val,"status");
    pBetInfo->postNotification(D_EVENT_MATCH_UPDATE_BETINFO);
	return pBetInfo;
}

void MatchModel::sortVS(VSInfoVec *pVSInfoVec)
{
    int i,j,length = pVSInfoVec->data->num;
    VSInfo ** x = (VSInfo**)pVSInfoVec->data->arr;
    VSInfo *tempItem;
    
    for(i=1; i<length; i++)
    {
        tempItem = x[i];
        j = i-1;
        
        
        while(j>=0 && ( tempItem->mid < x[j]->mid) )
        {
            x[j+1] = x[j];
            j = j-1;
        }
        x[j+1] = tempItem;
    }
}

void MatchModel::getVSListByFinalNum(uint32_t finalNum, VSInfoVec *vec)
{
	vec->removeAllObjects();

	if (m_pMatchConfig->getMatching(false) > finalNum)
	{
		return;
	}
	

	CCObject* obj = NULL;
	VSInfo* pVSInfo = NULL;

	CCARRAY_FOREACH(m_VSInfoVec, obj)
	{
		pVSInfo = (VSInfo*)obj;
		if(pVSInfo->finalNum == finalNum )
		{
			vec->addObject(pVSInfo);
		}
	}
    sortVS(vec);
}


void MatchModel::updateBetInfo(uint32_t finalNum,uint32_t mid)
{
	BetInfo *pBetInfo = getBetInfo(finalNum,mid);

	if (pBetInfo)
	{
		pBetInfo->status = true;
		pBetInfo->postNotification(D_EVENT_MATCH_UPDATE_BETINFO);
	}
}



void MatchModel::updateMatchRate(uint32_t finalNum,uint32_t mid,uint32_t rate1,uint32_t rate2)
{
    VSInfo* pVSInfo = getVSInfo(finalNum,mid);
    if(pVSInfo)
    {
        pVSInfo->m_pLeft->rate = rate1;
        pVSInfo->m_pRight->rate = rate2;
        pVSInfo->postNotification(D_EVENT_MATCH_UPDATE_VSINFO);
    }
}


VSInfo* MatchModel::getVSInfo(uint32_t finalNum,uint32_t mid)
{
	CCObject* obj = NULL;
	VSInfo* pVSInfo = NULL;

	CCARRAY_FOREACH(m_VSInfoVec, obj)
	{
		pVSInfo = (VSInfo*)obj;
		if(pVSInfo->mid == mid && pVSInfo->finalNum == finalNum)
		{
			return pVSInfo;
		}
	}

	return NULL;
}

OppDesp* MatchModel::parseOppDesp(const Json::Value &val, OppDesp * pOppDesp)
{
	if(pOppDesp == NULL)
	{
		pOppDesp = OppDesp::create();
	}

	DJSON_UINT(pOppDesp->lid,val,"lid");
    DJSON_UINT(pOppDesp->ldid,val, "ldid");
	DJSON_UINT(pOppDesp->lv,val,"lv");
	DJSON_CSTRING(pOppDesp->name,val,"name");

	return pOppDesp;
}


void MatchModel::parseFormation(const Json::Value &val, VSPlayer::FormationVec *pFormationVec)
{
	
	pFormationVec->removeAllObjects();

	for(uint32_t i=0; i <val.size(); i++)	
	{
		FormationInfo *pFormation = FormationInfo::create();

		DJSON_UINT(pFormation->lv,val[i],"lv");
		DJSON_UINT(pFormation->pid,val[i],"pid");
		DJSON_UINT(pFormation->wid,val[i],"sid");

		pFormationVec->addObject(pFormation);
	}
    sortPositions(pFormationVec);
}

void MatchModel::sortPositions(cocos2d::CCArray* pPositions)
{
    int i,j,length = pPositions->data->num;
    FormationInfo ** x = (FormationInfo**)pPositions->data->arr;
    FormationInfo *tempItem;
    
    for(i=1; i<length; i++)
    {
        tempItem = x[i];
        j = i-1;
        
        
        while(j>=0 && ( tempItem->pid < x[j]->pid) )
        {
            x[j+1] = x[j];
            j = j-1;
        }
        x[j+1] = tempItem;
    }
}


void MatchModel::parseVSMatchs(const Json::Value &val)
{
	//m_VSInfoVec->removeAllObjects();

	MatchFinalCat finalCat = m_pMatchConfig->getMatching(false);
	MatchFinalCat finalCatLive = m_pMatchConfig->getMatching(true);

	for(uint32_t i=0; i <val.size(); i++)
	{
        VSInfo * pVSInfo = getVSInfo(val[i]["finalNum"].asUInt(), val[i]["mid"].asUInt());
        if(pVSInfo)
        {
            parseVSMatch(val[i],pVSInfo);
            //continue;
        }
		else
		{
			pVSInfo = parseVSMatch(val[i]);
			m_VSInfoVec->addObject(pVSInfo);
		}


		int num = 7 - pVSInfo->finalNum + pVSInfo->mid;
		if (m_iToSave & 1 << num)
		{
			pVSInfo->m_isWatched = true;
		}
		else
		{
			pVSInfo->m_isWatched = false;
		}
		pVSInfo->refreshWithFinalCat(finalCatLive);

		if (pVSInfo->finalNum < finalCatLive)
		{
			int leftNum = 7 - pVSInfo->finalNum * 2 + pVSInfo->mid * 2 - 1;
			int rightNum = 7 - pVSInfo->finalNum * 2 + pVSInfo->mid * 2;
            
            if (pVSInfo->getIsSwitched()) {
                leftNum += 1;
                rightNum -= 1;
            }
            
			if (leftNum >= 0 && m_iToSave & 1 << leftNum)
			{
				pVSInfo->m_pLeft->visible = true;
			}
			if (rightNum >= 0 && m_iToSave & 1 << rightNum)
			{
				pVSInfo->m_pRight->visible = true;
			}
		}
		if (pVSInfo->finalNum == k8 && pVSInfo->finalNum <= finalCat)
		{
			pVSInfo->m_pLeft->visible = true;
			pVSInfo->m_pRight->visible = true;
		}
		
	}
	if (isSynced)
	{
		postNotification(D_EVENT_MATCH_UPDATE_VSLIST);
	}
	
}

VSInfo* MatchModel::parseVSMatch(const Json::Value &val, VSInfo* pVSInfo)
{
	if(pVSInfo == NULL)
	{
		pVSInfo = VSInfo::create();
	}
	DJSON_UINT(pVSInfo->finalNum,val,"finalNum");
	DJSON_UINT(pVSInfo->mid,val,"mid");
	DJSON_UINT(pVSInfo->winUser,val,"winUser");
 /*   
    pVSInfo->m_pLeft->win = pVSInfo->winUser==kVSLeft && pVSInfo->isWatched();
    pVSInfo->m_pLeft->lose = pVSInfo->winUser==kVSRight && pVSInfo->isWatched();
    pVSInfo->m_pRight->win = pVSInfo->winUser==kVSRight && pVSInfo->isWatched();
    pVSInfo->m_pRight->lose = pVSInfo->winUser==kVSLeft && pVSInfo->isWatched();
*/ 
	DJSON_UINT(pVSInfo->m_pLeft->rate,val,"rate1");
	DJSON_UINT(pVSInfo->m_pRight->rate,val,"rate2");
    
    parseOppDesp(val["disp1"],pVSInfo->m_pLeft->m_pDesp);
    parseOppDesp(val["disp2"],pVSInfo->m_pRight->m_pDesp);
	
	parseFormation(val["detail1"],pVSInfo->m_pLeft->m_pDetail);
	parseFormation(val["detail2"],pVSInfo->m_pRight->m_pDetail);

	if (pVSInfo->finalNum < 8)
	{
		uint32_t parentFinalNum = pVSInfo->finalNum * 2;
		uint32_t leftMid = pVSInfo->mid * 2 - 1;
		//uint32_t rightMid = pVSInfo->mid * 2;
		VSInfo *pLeft = getVSInfo(parentFinalNum, leftMid);
		//VSInfo *pRight = getVSInfo(parentFinalNum, rightMid);
		if (pLeft->winUser != 0/* && pRight->winUser != 0*/)
		{
			VSPlayer *pLeftPlayer = pLeft->winUser == 1 ? pLeft->m_pLeft : pLeft->m_pRight;
			//VSPlayer *pRightPlayer = pRight->winUser == 1 ? pRight->m_pLeft : pRight->m_pRight;
			CCLOG("%s,%s", pLeftPlayer->m_pDesp->name.c_str(), pVSInfo->m_pLeft->m_pDesp->name.c_str());
			if (strcmp(pLeftPlayer->m_pDesp->name.c_str(), pVSInfo->m_pLeft->m_pDesp->name.c_str()) != 0)
			{
// 				VSPlayer *pPlayer = pVSInfo->m_pRight;
// 				pVSInfo->m_pRight = pVSInfo->m_pLeft;
// 				pVSInfo->m_pLeft = pPlayer;
// 				if (pVSInfo->winUser)
// 				{
// 					pVSInfo->winUser = 3 - pVSInfo->winUser;
// 				}
				pVSInfo->setIsSwitched(true);
				//pVSInfo->winUser = 3 - pLeft->winUser;
			}
			
		}
		
	}
	
    pVSInfo->postNotification(D_EVENT_MATCH_UPDATE_VSINFO);
	return pVSInfo;
}

int MatchModel::checkTime()
{
    if(m_nlastUpdateTime==0) return 0; //如果没有刷新时间 立即刷新
    
    
    unsigned int now = GameManager::sharedGameManager()->getServerTime();
    
    if(m_nlastUpdateTime<=TimeUtil::getStartTimeOfToday(now)) return 0;//如果刷新时间是昨天 立即刷新
    
    //如果刷新时间 和当前时间不在一个时间段 立即刷新
    unsigned int nMatch8StartTime = TimeUtil::getTimeToday(now,getMatchConfig()->m_nMatch8StartTime);
    if(m_nlastUpdateTime<=nMatch8StartTime && now>nMatch8StartTime) return 0;
    
    unsigned int nMatch8EndTime = TimeUtil::getTimeToday(now,getMatchConfig()->m_nMatch8EndTime);
    if(m_nlastUpdateTime<=nMatch8EndTime && now>nMatch8EndTime) return 0;
    
    
    unsigned int nMatch4EndTime = TimeUtil::getTimeToday(now,getMatchConfig()->m_nMatch4EndTime);
    if(m_nlastUpdateTime<=nMatch4EndTime && now>nMatch4EndTime) return 0;
    
    unsigned int nMatch2EndTime = TimeUtil::getTimeToday(now,getMatchConfig()->m_nMatch2EndTime);
    if(m_nlastUpdateTime<=nMatch2EndTime && now>nMatch2EndTime) return 0;
    
    
    if(now>nMatch2EndTime)return TimeUtil::getStartTimeOfTomorrow(now)-now+1; //零点刷新
    if(now>nMatch4EndTime)return nMatch2EndTime-now;
    if(now>nMatch8EndTime)return nMatch4EndTime-now;
    if(now>nMatch8StartTime)return nMatch8EndTime-now;
    
    return -1;
}


int MatchModel::refreshTime()
{
	unsigned int now = GameManager::sharedGameManager()->getServerTime();
	unsigned int nMatch2EndTime = TimeUtil::getTimeToday(now,getMatchConfig()->m_nMatch2EndTime);
	if(now>nMatch2EndTime)return nMatch2EndTime + K_MATCH_LIVE_TIME - now;
	unsigned int nMatch4EndTime = TimeUtil::getTimeToday(now,getMatchConfig()->m_nMatch4EndTime);
	if(now>nMatch4EndTime)return nMatch4EndTime + K_MATCH_LIVE_TIME - now;
	unsigned int nMatch8EndTime = TimeUtil::getTimeToday(now,getMatchConfig()->m_nMatch8EndTime);
	if(now>nMatch8EndTime)return nMatch8EndTime + K_MATCH_LIVE_TIME - now;
	unsigned int nMatch8ShowTime = TimeUtil::getTimeToday(now,getMatchConfig()->m_nMatch8ShowTime);
    if(now>nMatch8ShowTime)return nMatch8EndTime - now;
	if(now>m_pMatchConfig->getEndTimeOfMatch(m_pMatchConfig->roundLargerThanK8))return nMatch8ShowTime - now;
	for (int i = m_pMatchConfig->roundLargerThanK8; i >= 1; i--)
	{
		if (now > m_pMatchConfig->getStartTimeOfMatch(i))
		{
			return m_pMatchConfig->getEndTimeOfMatch(i) - now;
		}
		
	}
	if (now > TimeUtil::getStartTimeOfToday(now)) return m_pMatchConfig->getStartTimeOfMatch(1) - now;
	
	return -1;
}

void MatchModel::reset()
{
    Model::reset();
    if(m_VSInfoVec) m_VSInfoVec->removeAllObjects();
    if(m_betInfoVec) m_betInfoVec->removeAllObjects();
}

void MatchModel::watchReplay(match::VSInfo *pVsInfo)
{
	if (NULL == pVsInfo)
	{
		return;
	}
	
	watchReplay(pVsInfo->finalNum, pVsInfo->mid);
}

void MatchModel::watchReplay(match::MatchInfo *pMatchInfo)
{
	if (NULL == pMatchInfo)
	{
		return;
	}

	watchReplay(pMatchInfo->finalNum, pMatchInfo->mid);
}

void MatchModel::watchReplay(uint32_t finalNum,uint32_t mid)
{
	match::MatchInfo *pMatchInfo = getMatchInfo(finalNum, mid);
	match::VSInfo *pVsInfo = getVSInfo(finalNum, mid);
	
	CCUserDefault *pUserDefault = CCUserDefault::sharedUserDefault();

	if (pMatchInfo && pMatchInfo->watched == false)
	{
		pMatchInfo->watched = true;
		pMatchInfo->postNotification(D_EVENT_MATCH_AWARD_STATUS_UPDATE);
		m_iMatchFinal = pMatchInfo->finalNum;
	}

	if (pVsInfo && pVsInfo->isWatched() == false)
	{
		pVsInfo->setWatched(true);
		int num = 7 - pVsInfo->finalNum + pVsInfo->mid;
		m_iToSave |= 1 << num;
		VSInfo * pAnotherVSInfo = getVSInfo(pVsInfo->finalNum / 2, (pVsInfo->mid + 1) / 2);
		if (pAnotherVSInfo)
		{
            int modMid = pAnotherVSInfo->getIsSwitched() ? 0 : 1;
			if (pVsInfo->mid % 2 == modMid)
			{
				pAnotherVSInfo->m_pLeft->visible = true;
			}
			else
			{
				pAnotherVSInfo->m_pRight->visible = true;
			}
			pAnotherVSInfo->postNotification(D_EVENT_MATCH_REPLAY);
		}
	}
	pUserDefault->setIntegerForKey(KEY_MATCH_WATCHED, m_iMatchFinal);
	pUserDefault->setIntegerForKey(KEY_VS_MATCH_WATCHED, m_iToSave);
	pUserDefault->setIntegerForKey(KEY_MATCH_WATCHED_TIME, GameManager::sharedGameManager()->getServerTime());
	pUserDefault->flush();
	
}

MatchFinalCat MatchModel::getMatching()
{
	MatchFinalCat finalCat = m_pMatchConfig->getMatching(false);
	MatchFinalCat finalCatLive = m_pMatchConfig->getMatching(true);
	if (finalCat == finalCatLive || finalCat == k8)
	{
		return finalCat;
	}
	else
	{
		VSInfoVec *pVSInfoVec = VSInfoVec::create();
		getVSListByFinalNum(finalCatLive, pVSInfoVec);
		for (int i = 0; i < pVSInfoVec->count(); i++)
		{
			VSInfo *pVSInfo = (VSInfo *)pVSInfoVec->objectAtIndex(i);
			if (pVSInfo->isWatched() == false)
			{
				return finalCatLive;
			}
		}
		return finalCat;
	}
}

void MatchModel::refreshWatchedMatch()
{
	MatchFinalCat finalCat = m_pMatchConfig->getMatching();
	for (int i = 0; i < m_matchInfoVec->count(); i++)
	{
		MatchInfo *pMatchInfo = (MatchInfo *)m_matchInfoVec->objectAtIndex(i);
		pMatchInfo->refreshWithFinalCat(finalCat);
	}
	for (int i = 0; i < m_VSInfoVec->count(); i++)
	{
		VSInfo *pVSInfo = (VSInfo *)m_VSInfoVec->objectAtIndex(i);
		pVSInfo->refreshWithFinalCat(finalCat);
		if (pVSInfo->finalNum == k8 && pVSInfo->finalNum <= finalCat)
		{
			pVSInfo->m_pLeft->visible = true;
			pVSInfo->m_pRight->visible = true;
		}
	}
	postNotification(D_EVENT_MATCHLIST_UPDATE);
    postNotification(D_EVENT_MATCH_UPDATE_VSLIST);
}

void MatchModel::readSavedData()
{
	CCUserDefault *pUserDefault = CCUserDefault::sharedUserDefault();

	uint32_t timeSaved = pUserDefault->getIntegerForKey(KEY_MATCH_WATCHED_TIME, 0);
	if(timeSaved > TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),0) && timeSaved <= GameManager::sharedGameManager()->getServerTime())
	{
		m_iToSave = pUserDefault->getIntegerForKey(KEY_VS_MATCH_WATCHED, 0);
		m_iMatchFinal = pUserDefault->getIntegerForKey(KEY_MATCH_WATCHED, 0);
	}
	else
	{
		m_iToSave = 0;
		m_iMatchFinal = 1 << 16;
	}

}

void MatchModel::getMatchInfoVec(MatchInfoVec *arr)
{
    if(arr==NULL) return;
	arr->removeAllObjects();
	MatchFinalCat finalCat = m_pMatchConfig->getMatching();
	MatchFinalCat finalCatLive = m_pMatchConfig->getMatching(true);
    CCObject* obj;
    CCARRAY_FOREACH(m_matchInfoVec, obj)
    {
		match::MatchInfo *pMatch = dynamic_cast<match::MatchInfo *>(obj);
		if (pMatch->finalNum < finalCat)
		{
			continue;
		}

        arr->addObject(obj);
    }
	if (0 == arr->count())
	{
		return;
	}
	
	match::MatchInfo *pMatch = dynamic_cast<match::MatchInfo *>(arr->objectAtIndex(0));
	pMatch->watched |= pMatch->finalNum > finalCatLive;
	for (int i = 1; i < arr->count(); i++)
	{
		match::MatchInfo *pMatch = dynamic_cast<match::MatchInfo *>(arr->objectAtIndex(i));
		pMatch->watched = true;
	}
	
	
}

uint32_t MatchModel::getMatchInfoCount()
{
    return m_matchInfoVec->count();
}

void MatchModel::inited()
{
	isSynced = true;
	postNotification(D_EVENT_MATCHLIST_UPDATE);
	postNotification(D_EVENT_MATCH_UPDATE_BETLIST);
	postNotification(D_EVENT_MATCHINFO_UPDATE);
	postNotification(D_EVENT_MATCH_UPDATE_VSLIST);
}

#pragma mark - 

uint8_t MatchConfig::getBetCost(MatchFinalCat val)
{
    switch (val) {
        case k8:
            return m_nBetCost8;
        case k4:
            return m_nBetCost4;
        case k2:
            return m_nBetCost2;
        default:
            break;
    }
    return  0;
}

std::string MatchConfig::getMatchEndTime(MatchFinalCat val)
{
    switch (val) {
        case k8:
            return cocos2d::CCString::createWithFormat("%02d:00",m_nMatch8EndTime)->getCString();
        case k4:
            return cocos2d::CCString::createWithFormat("%02d:00",m_nMatch4EndTime)->getCString();
        case k2:
            return cocos2d::CCString::createWithFormat("%02d:00",m_nMatch2EndTime)->getCString();
        default:
            break;
    }
    return  "";
}

/*获取正在进行的比赛*/
MatchFinalCat MatchConfig::getMatching(bool withLiveTime/* = false*/)
{
	uint32_t liveTime = 0;
	if (withLiveTime)
	{
		liveTime = K_MATCH_LIVE_TIME;
	}
	if(GameManager::sharedGameManager()->getServerTime()>=TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),m_nMatch2EndTime) + liveTime) return k1;
    if(GameManager::sharedGameManager()->getServerTime()>=TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),m_nMatch4EndTime) + liveTime) return k2;
    if(GameManager::sharedGameManager()->getServerTime()>=TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),m_nMatch8EndTime) + liveTime) return k4;
    //if(GameManager::sharedGameManager()->getServerTime()>=TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),m_nMatch8StartTime)) return k8;
	if (!withLiveTime && GameManager::sharedGameManager()->getServerTime()>=TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),m_nMatch8ShowTime)) return k8;
	if (withLiveTime && GameManager::sharedGameManager()->getServerTime()>=TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),m_nMatch8EndTime)) return k8;
	unsigned int now = GameManager::sharedGameManager()->getServerTime();
	for (int i = roundLargerThanK8; i >= 1; i--)
	{
		if (!withLiveTime && now >= getStartTimeOfMatch(i))
		{
			return MatchFinalCat(8 << (roundLargerThanK8 - i + 1));
		}
		else if (withLiveTime && now >= getEndTimeOfMatch(i))
		{
			return MatchFinalCat(8 << (roundLargerThanK8 - i + 1));
		}
	}
	

	/*int minRound = withLiveTime ? 1 : 0;
	
	for (int i = roundLargerThanK8; i >= minRound ; i--)
	{
		match::MatchTime *pData = (match::MatchTime *)m_pMatchTime->objectForKey(i + minRound);
		if (NULL == pData)
		{
			return k8;
		}
		
		if (GameManager::sharedGameManager()->getServerTime()>=TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),pData->hour, pData->minute))
		{
			return MatchFinalCat(8 << (roundLargerThanK8 - i));
		}
		
	}*/
	
    return MatchFinalCat(1 << 16);
}


void MatchConfig::parseMatchTime(const CSVDecoder::CSV& val)
{
	if (NULL == m_pMatchTime)
	{
		m_pMatchTime = CCDictionary::create();
		m_pMatchTime->retain();
	}
	m_pMatchTime->removeAllObjects();
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		match::MatchTime *pData = match::MatchTime::create();
		pData->round = atoi(val[i][0].c_str());
		pData->hour = atoi(val[i][1].c_str());
		pData->minute = atoi(val[i][2].c_str());
		m_pMatchTime->setObject(pData, pData->round);
	}
	roundLargerThanK8 = val.size();
}

uint32_t MatchConfig::getStartTimeOfMatch(uint8_t round)
{
	if (round > roundLargerThanK8)
	{
		switch (round - roundLargerThanK8)
		{
		case 1:
			return TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),m_nMatch8EndTime);
			break;
		case 2:
			return TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),m_nMatch4EndTime);
			break;
		case 3:
			return TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),m_nMatch2EndTime);
			break;
		default:
			return TimeUtil::getStartTimeOfTomorrow(GameManager::sharedGameManager()->getServerTime());
		}
	}
	else
	{
		MatchTime *pData = (MatchTime *)m_pMatchTime->objectForKey(round);
		if (pData)
		{
			return TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime(),pData->hour, pData->minute);
		}
		else
		{
			return -1;
		}
	}
	
}

uint32_t MatchConfig::getEndTimeOfMatch(uint8_t round)
{
	if (round >= roundLargerThanK8)
	{
		return getStartTimeOfMatch(round) + K_MATCH_LIVE_TIME;
	}
	else
	{
		return getStartTimeOfMatch(round + 1);
	}
}

#pragma mark - VSInfo

void VSInfo::setWatched(bool watched)
{
	if (watched)
	{

		m_pLeft->win = winUser==kVSLeft;
		m_pLeft->lose = winUser==kVSRight;
		m_pRight->win = winUser==kVSRight;
		m_pRight->lose = winUser==kVSLeft;
		if (m_isWatched == false)
		{
			m_isWatched = true;
			postNotification(D_EVENT_MATCH_REPLAY);
		}
	}
	else
	{
		m_pLeft->win = false;
		m_pLeft->lose = false;
		m_pRight->win = false;
		m_pRight->lose = false;
		m_isWatched = false;
	}
}

void VSInfo::refreshWithFinalCat(MatchFinalCat finalCat)
{
	if (this->m_isWatched || this->finalNum > finalCat)
	{
		this->m_pLeft->visible = true;
		this->m_pRight->visible = true;
		this->setWatched(true);
		return;
	}
	else if (this->finalNum == finalCat)
	{
		bool toPost = false;
		if (this->m_pLeft->visible == false || this->m_pRight->visible == false)
		{
			toPost = true;
		}
		this->m_pLeft->visible = true;
		this->m_pRight->visible = true;
		this->setWatched(false);
		if (toPost)
		{
			postNotification(D_EVENT_MATCH_REPLAY);
		}
	}
	else
	{
		this->m_pLeft->visible = false;
		this->m_pRight->visible = false;
		this->setWatched(false);
	}
}

#pragma mark - MatchInfo

void MatchInfo::refreshWithFinalCat(MatchFinalCat finalCat)
{
	if (this->watched)
	{
		return;
	}
	
	if (this->finalNum > finalCat)
	{
		watched = true;
		this->postNotification(D_EVENT_MATCH_AWARD_STATUS_UPDATE);
	}
	else
	{
		watched = false;
	}
}