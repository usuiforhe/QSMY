//
//  SoulHunterModel.cpp
//  狩魂
//
//  Created by liuxiaogang on 13-10-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoulHunterModel.h"
#include "../managers/GameManager.h"


USING_NS_CC;
using namespace soulhunter;


SoulHunterModel::SoulHunterModel()
:m_pHunterInfo(NULL)
,lastBattleInfo(NULL)
,m_pArrRivals(NULL)
,m_phunterDrops(NULL)
,m_pTargetWarrior(NULL)
,lastWid(0)
,curSoulHunerType(0)
{
    
}


SoulHunterModel::~SoulHunterModel()
{
	CC_SAFE_RELEASE(m_pHunterInfo);
	CC_SAFE_RELEASE(lastBattleInfo);
	CC_SAFE_RELEASE(m_pArrRivals);
	CC_SAFE_RELEASE(m_phunterDrops);
	CC_SAFE_RELEASE(m_pTargetWarrior);
}


bool SoulHunterModel::init()
{ 
    m_pHunterInfo = SoulHunterInfo::create();
    CC_SAFE_RETAIN(m_pHunterInfo);

	lastBattleInfo= SoulHunterLastBattleInfo::create();
	CC_SAFE_RETAIN(lastBattleInfo);
   
	m_pArrRivals = SoulHunterRivalArray::create();
	CC_SAFE_RETAIN(m_pArrRivals);

    return Model::init();
}


void SoulHunterModel::deductPower()
{
	if(m_pHunterInfo->m_pPower > 1)
	{
		--m_pHunterInfo->m_pPower;
	}
	else
	{
		m_pHunterInfo->m_pPower = 0;
	}
	postNotification(D_EVENT_SOULHUNTER_UPDATED);
}


void SoulHunterModel::parseSoulHunterInfo(const Json::Value & val)
{
	DJSON_UINT(m_pHunterInfo->m_pPower,val,"power");
	DJSON_UINT(m_pHunterInfo->m_pMaxPower,val,"maxPower");
	DJSON_UINT(m_pHunterInfo->m_pProtectTs,val,"protectTs");

    isSynced = true;
	postNotification(D_EVENT_SOULHUNTER_UPDATED);
}


void SoulHunterModel::parseSoulHunterRivalsInfo(const Json::Value & val)
{
	m_pArrRivals->removeAllObjects();
	for (uint32_t i=0; i<val.size(); ++i)					//解析数据
	{
		soulhunter::SoulHunterRivalInfo *rival = soulhunter::SoulHunterRivalInfo::create();
		rival->loadData(val[i]);
		m_pArrRivals->addObject(rival);
	}
	postNotification(D_EVENT_SOULHUNTER_RIVALS_UPDATED);
}


void SoulHunterModel::parseSoulHunterAwardInfo(const Json::Value &requestData,const Json::Value &responseData)
{
	DJSON_UINT( curSoulHunerType , responseData , "huntRes" );
	deductPower();										//减少剩余挑战次数次数
	setHunterDrops(DM_GET_DROP_MODEL->getDropArray(requestData[UUID].asUInt()));
	postNotification(D_EVENT_SOULHUNTER_SNATCH_FINISH);
}


void SoulHunterModel::setHunterDrops(cocos2d::CCArray *pSoulhunterDrops)
{
    CC_SAFE_RETAIN(pSoulhunterDrops);
	CC_SAFE_RELEASE(m_phunterDrops);
	m_phunterDrops = pSoulhunterDrops;
}


uint8_t SoulHunterModel::getCurSoulHunterType()
{
	return curSoulHunerType;
}


cocos2d::CCArray* SoulHunterModel::getHunterDrops()
{
	return m_phunterDrops;
}


void SoulHunterModel::doLastBattleRequest()
{
	if (lastBattleInfo)
	{
		uint32_t lastTid=lastBattleInfo->getLastTid();
		uint32_t lastWid=lastBattleInfo->getLastWid();
		uint8_t lastIsNpc=lastBattleInfo->getLastIsNpc();
		HTTP_CLIENT->huntAttack(lastTid,lastWid,lastIsNpc);
	}
}


void SoulHunterModel::handleRivalNotExist()
{
	postNotification(D_EVENT_SOULHUNTER_NOT_EXIST);	//刷新对手
}

void SoulHunterModel::handleWarriorNoMoreSnatch()
{
	//将wid加入数组
	insertWarroirNoMoreSnatch(m_pTargetWarrior);
}

void SoulHunterModel::seTargetWarrior(warrior::WarriorCurInfo * pWarrior)
{
    CC_SAFE_RETAIN(pWarrior);
	CC_SAFE_RELEASE(m_pTargetWarrior);
	m_pTargetWarrior = pWarrior;
}

void SoulHunterModel::insertWarroirNoMoreSnatch(warrior::WarriorCurInfo * pWarrior){
	if(pWarrior==NULL) return;
	m_pcanontSnatachWarriors.insert(pWarrior->getWid());
}

void SoulHunterModel::reset(){
	Model::reset();
	m_pcanontSnatachWarriors.clear();
}

bool SoulHunterModel::cannotSnatchWarroir(warrior::WarriorCurInfo * pWarrior){
    if(pWarrior==NULL) return false;
	if(m_pcanontSnatachWarriors.count(pWarrior->getWid())>0) return true;
	return false;
}


void SoulHunterModel::setLastBattle(uint32_t tid, uint32_t wid, uint8_t isNpc)	//设置上次战斗信息，用于再战请求
{
	lastBattleInfo->m_pLastTid	= tid;
	lastBattleInfo->m_pLastWid	= wid;
	lastBattleInfo->m_pLastIsNpc = isNpc;
}


void SoulHunterRivalInfo::loadData(const Json::Value &data)
{
	DJSON_UINT(     m_pUid,      data, "uid" );
	DJSON_CSTRING(  m_pName,     data, "name");
	DJSON_UINT(     m_pLv,       data, "lv"  );
	DJSON_UINT(     m_pLid,		 data, "lid" );
    DJSON_UINT(     m_nLdId,	 data, "ldid");
	DJSON_UINT(     m_pIsNpc,	 data, "isNpc" );
}
