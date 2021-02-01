//
//  SigninModel.cpp
//  公会
//
//  Created by Eci on 13-10-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "SigninModel.h"
#include "../network/HttpClient.h"
#include "../managers/GameManager.h"
#include <algorithm>
#include "utils/TimerUtil.h"

using namespace cocos2d;
using namespace signin;

namespace signin
{
}

SigninModel::SigninModel()
	:m_pDictSigninAward(NULL)
	,m_uWeek(0)
	,m_uDay(0)
{
}

SigninModel::~SigninModel()
{
	CC_SAFE_RELEASE(m_pDictSigninAward);
}

bool SigninModel::init()
{
	m_pDictSigninAward = CCDictionary::create();
	CC_SAFE_RETAIN(m_pDictSigninAward);

	LoadSigninAward( CSVDecoder::create(SIGNIN_CONFIG_PATH)->getData());
	return Model::init();
}

void SigninModel::LoadSigninAward(const CSVDecoder::CSV &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		SigninAward *pData = SigninAward::create();
		pData->m_uWeek = atoi(val[i][0].c_str());
		pData->m_uDay = atoi(val[i][1].c_str());
		pData->m_uItem0ID = atoi(val[i][2].c_str());
		pData->m_uItem0Num = atoi(val[i][3].c_str());
		pData->m_uItem0Type = atoi(val[i][4].c_str());
		pData->m_uItem1ID = atoi(val[i][5].c_str());
		pData->m_uItem1Num = atoi(val[i][6].c_str());
		pData->m_uItem1Type = atoi(val[i][7].c_str());
		pData->m_bDouble = atoi(val[i][8].c_str());

		CCArray *pArr = dynamic_cast<CCArray *>(m_pDictSigninAward->objectForKey(pData->m_uWeek));
		if (!pArr)
		{
			pArr = CCArray::create();
			m_pDictSigninAward->setObject(pArr, pData->m_uWeek);
		}
		pArr->addObject(pData);
	}
}

CCArray *SigninModel::getSigninAward()
{
	CCArray *pArr = CCArray::create();
	CCArray *pAward = dynamic_cast<CCArray *>(m_pDictSigninAward->objectForKey(m_uWeek));
	if (pAward != NULL)
	{
		pArr->addObjectsFromArray(pAward);
	}
	return pArr;
}

void SigninModel::parseData(const Json::Value &val)
{
	DJSON_UINT(m_uWeek, val, "signRound");
	DJSON_UINT(m_uDay, val, "signTimes");
	DJSON_UINT(m_uSignTs, val, "signTs");
	isSynced = true;
	postNotification(D_EVENT_SIGNIN_UPDATE);
}

bool SigninModel::isSignedToday()
{
	uint32_t now = GameManager::sharedGameManager()->getServerTime();
	uint32_t sot = TimeUtil::getStartTimeOfToday(now);
	return m_uSignTs > sot;
}