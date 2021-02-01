//
//  SettlementModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SettlementModel.h"
#include "managers/DataManager.h"

using namespace settlement;


SettlementModel::SettlementModel()
:m_pSettlementInfoPve(NULL)
,m_pSettlementInfoMatch(NULL)
,m_pSettlementInfoPvp(NULL)
{

}

SettlementModel::~SettlementModel()
{
    CC_SAFE_RELEASE(m_pSettlementInfoPve);
    CC_SAFE_RELEASE(m_pSettlementInfoMatch);
	CC_SAFE_RELEASE(m_pSettlementInfoPvp);
}

bool SettlementModel::init()
{
    return Model::init();
}

void SettlementModel::setSettlementInfoPve(const Json::Value &val,int32_t dropId,const Json::Value &wonders, uint32_t mapId)
{
    CC_SAFE_RELEASE(m_pSettlementInfoPve);
    m_pSettlementInfoPve = SettlementInfoPve::create();
    CC_SAFE_RETAIN(m_pSettlementInfoPve);
    
    
    //结算经验和金币
    DJSON_INT(m_pSettlementInfoPve->exp, val, "exp");
    DJSON_INT(m_pSettlementInfoPve->wexp, val, "wExp");
    DJSON_INT(m_pSettlementInfoPve->coin, val, "coin");
    
    DJSON_INT(m_pSettlementInfoPve->grade, val, "grade");
    DJSON_BOOL(m_pSettlementInfoPve->inwar, val, "battle");
    m_pSettlementInfoPve->win          = m_pSettlementInfoPve->grade>0;
    
    m_pSettlementInfoPve->dropId = dropId;
    
    m_pSettlementInfoPve->mapId = mapId;
    if(wonders.isArray())
    {
        for (uint32_t i =0;i<wonders.size(); ++i) {
            parseWonder(m_pSettlementInfoPve->m_pWonders,wonders[i]);
        }
    }else
    {
        parseWonder(m_pSettlementInfoPve->m_pWonders,wonders);
    }
}

void SettlementModel::setSettlementInfoPvp(const Json::Value &val , unsigned int tid)
{
	CC_SAFE_RELEASE(m_pSettlementInfoPvp);
	m_pSettlementInfoPvp = SettlementInfoPvp::create();
	CC_SAFE_RETAIN(m_pSettlementInfoPvp);

	//对手的id 机器人为0
	m_pSettlementInfoPvp->m_uTargetID = tid;

	DJSON_STRING(m_pSettlementInfoPvp->m_sSelfName, val, "selfName");
	DJSON_STRING(m_pSettlementInfoPvp->m_sTargetName, val, "targetName");
	DJSON_UINT(m_pSettlementInfoPvp->m_iSelfFight, val, "selfFight");
	DJSON_UINT(m_pSettlementInfoPvp->m_iTargetFight, val, "targetFight");
	DJSON_INT(m_pSettlementInfoPvp->m_iEndurance, val, "endurance");
	DJSON_UINT(m_pSettlementInfoPvp->m_iPoint, val, "point");
	DJSON_UINT(m_pSettlementInfoPvp->m_iRewardyb, val, "rewardyb");
	DJSON_BOOL(m_pSettlementInfoPvp->m_bWin, val, "win");

	if(m_pSettlementInfoPvp->m_iRewardyb > 0){
		//史上最佳
		uint32_t newbest = 0;
		DJSON_UINT(newbest, val, "bRank");
		uint32_t oldbest = 0;
		oldbest = DM_GET_PVP_MODEL->getChallengeInfo()->getBestRank();
		uint32_t grow = oldbest - newbest;//grow should be greater than 0
		if(grow > 0){
			DM_GET_PVP_MODEL->setBestEver(true);
			DM_GET_PVP_MODEL->setBestRewardPoint(m_pSettlementInfoPvp->m_iRewardyb);
			DM_GET_PVP_MODEL->setNewBest(newbest);
			DM_GET_PVP_MODEL->setOldBest(oldbest);
		}		
	}
	//在UserInfo中更新耐力值
	DM_GET_USER_MODEL->getUserInfo()->changeEnduranceBy(m_pSettlementInfoPvp->m_iEndurance);
	
	//m_pSettlementInfoPvp->m_iSelfFight = val["selfFight"].asUInt();
	//uint32_t test = m_pSettlementInfoPvp->m_iSelfFight;
	////CCLog(val.asCString());
	//CCLog("-------------------");
	//CCString* str = CCString::createWithFormat("%d" , val["selfFight"].asUInt());
	//CCLog(str->getCString());
	//CCLog(m_pSettlementInfoPvp->m_sSelfName.c_str());
}

void SettlementModel::parseWonder(cocos2d::CCArray* pWonders,const Json::Value &val)
{
    if(val.isNull()) return;
    WonderInfo* pWonderInfo = WonderInfo::create();
    int32_t nWonderType = 0;
    DJSON_INT(nWonderType, val, "type");
    pWonderInfo->wonderType = (WonderType)nWonderType;
    pWonders->addObject(pWonderInfo);
    
    DJSON_INT(pWonderInfo->productId, val["product"], "id");
    DJSON_UINT(pWonderInfo->start, val, "start");
	DJSON_UINT(pWonderInfo->end, val, "end");
	if (val.isMember("item"))
	{
		DJSON_UINT(pWonderInfo->itemID, val["item"], "itemId");
		DJSON_UINT(pWonderInfo->itemNum, val["item"], "num");
	}
	DJSON_UINT(pWonderInfo->exp, val, "exp");
	if (val.isMember("product"))
	{
		DJSON_INT(pWonderInfo->price, val["product"], "unit_price");
	}

	if (val.isMember("bossWarriorId"))
	{
		DJSON_INT(pWonderInfo->bossId, val, "bossWarriorId");
	}

	if (val.isMember("bossLevel"))
	{
		DJSON_INT(pWonderInfo->bossLevel, val, "bossLevel");
	}
    
	DJSON_UINT(pWonderInfo->wonderID, val, "id");
}

void SettlementModel::setSettlementInfoMatch(const match::MatchInfo *pMatchInfo)
{
    if (!pMatchInfo) {
        return;
    }
    CC_SAFE_RELEASE(m_pSettlementInfoMatch);
    m_pSettlementInfoMatch = SettlementInfoMatch::create();
    CC_SAFE_RETAIN(m_pSettlementInfoMatch);
    
    if (pMatchInfo->win)
    {
        m_pSettlementInfoMatch->setUserName(DM_GET_USER_MODEL->getUserInfo()->getUserName());
        m_pSettlementInfoMatch->setUserLevel(DM_GET_USER_MODEL->getUserInfo()->getUserLv());
		m_pSettlementInfoMatch->setWarriorID(DM_GET_FORMATION_MODEL->getPositionInfoByIndex(0)->getWarriorId());
		uint32_t leaderID = DM_GET_FORMATION_MODEL->getPositionInfoByIndex(0)->getWarriorId();
		warrior::WarriorCurInfo *pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(
			leaderID);
		m_pSettlementInfoMatch->setAvatarID(leaderID);
		if (pWarrior && pWarrior->getAvatarId())
		{
			m_pSettlementInfoMatch->setAvatarID(pWarrior->getAvatarId());
		}
		
    }
    else
    {
        m_pSettlementInfoMatch->setUserName(pMatchInfo->oppDesp->name);
        m_pSettlementInfoMatch->setUserLevel(pMatchInfo->oppDesp->lv);
        m_pSettlementInfoMatch->setWarriorID(pMatchInfo->oppDesp->lid);
        m_pSettlementInfoMatch->setAvatarID(pMatchInfo->oppDesp->getAvatarId());
    }
    m_pSettlementInfoMatch->setRank(pMatchInfo->finalNum / 2);
}

void SettlementModel::setSettlementInfoMatch(const match::VSInfo *pVSInfo)
{
    if (!pVSInfo || !pVSInfo->winUser) {
        return;
    }
    CC_SAFE_RELEASE(m_pSettlementInfoMatch);
    m_pSettlementInfoMatch = SettlementInfoMatch::create();
    CC_SAFE_RETAIN(m_pSettlementInfoMatch);
    
    match::VSPlayer *pPlayer = NULL;
    if (1 == pVSInfo->winUser) {
        pPlayer = pVSInfo->m_pLeft;
    }
    else
    {
        pPlayer = pVSInfo->m_pRight;
    }
    m_pSettlementInfoMatch->setUserName(pPlayer->m_pDesp->name);
    m_pSettlementInfoMatch->setUserLevel(pPlayer->m_pDesp->lv);
    m_pSettlementInfoMatch->setWarriorID(pPlayer->m_pDesp->lid);
	m_pSettlementInfoMatch->setRank(pVSInfo->finalNum / 2);
    m_pSettlementInfoMatch->setAvatarID(pPlayer->m_pDesp->getAvatarId());
}


