//
//  LoginRewardModel.cpp
//  活动
//
//  created by ldr123 on 13-7-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "LoginRewardModel.h"
#include "DropModel.h"
#include "managers/GameManager.h"
#include "managers/PlatformManager.h"
#include <vector>
#include <algorithm>

USING_NS_CC;

LoginRewardModel::LoginRewardModel()
:loginTimes(0)
,loginAward(0)
,tLoginAward(0)
,lvAward(0)
,m_pLoginReward(NULL)
,m_pWelcomeList(NULL)
,m_bWelcomeOpen(false)
{}
LoginRewardModel::~LoginRewardModel()
{
	CC_SAFE_RELEASE(m_pLoginReward);
	CC_SAFE_RELEASE(m_pWelcomeList);
}

bool LoginRewardModel::init()
{
	m_pLoginReward = LoginRewardArr::create();
	CC_SAFE_RETAIN(m_pLoginReward);

	m_pWelcomeList = CCArray::create();
	CC_SAFE_RETAIN(m_pWelcomeList);

	ParseLoginRewardData( CSVDecoder::create(LOGINREWARD_CONFIG_PATH)->getData());
	LoadWelcomeData(CSVDecoder::create(WELCOME_CONFIG_PATH)->getData());
	return Model::init();
}

void LoginRewardModel::ParseActivityInfo(const Json::Value& val)
{
	DJSON_UINT2(this, val, loginTimes);
	DJSON_UINT2(this, val, loginAward);
	
	if (loginTimes > MAX_LOGIN_REWARD_DAYS)
	{
		loginTimes = MAX_LOGIN_REWARD_DAYS;
	}

	//NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_ACTIVITY_UPDATE);
}

void LoginRewardModel::ParseLoginRewardFromJson(const Json::Value& val)
{
	DJSON_UINT2(this, val["data"]["loginAward"], loginTimes);
	DJSON_UINT2(this, val["data"]["loginAward"], loginAward);
	if (loginTimes > MAX_LOGIN_REWARD_DAYS)
	{
		loginTimes = MAX_LOGIN_REWARD_DAYS;
	}
    isSynced = true;
}

loginreward::WelcomeData*	LoginRewardModel::getWelcomeData()
{
	loginreward::WelcomeData* pReturnData = NULL;
	//当前时间
	unsigned int timenow = GameManager::sharedGameManager()->getServerTime();
	//今日零点时间
	unsigned int timetoday = TimeUtil::getStartTimeOfToday(timenow);
	//今日是否已经登录过游戏
	bool logined = false;
	int lastlogin = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_START_TIME_OF_DAY, 0);
	if(lastlogin == timetoday)
	{
		logined = true;
	}
	else
	{
		CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_START_TIME_OF_DAY, timetoday);
	}

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(m_pWelcomeList, pObj)
	{
		loginreward::WelcomeData* pData = dynamic_cast<loginreward::WelcomeData*>(pObj);
		if(pData->m_type == 1)
		{
			//日期类

			//如果日期类的今天显示过了，那么显示时间类
			if(logined) continue;

			CCLOG("%u" , pData->m_dateTs);
			CCLOG("%u" , timetoday);
			if(pData->m_dateTs == timetoday)
			{
				pReturnData = pData;
				break;
			}
		}
		else
		{
			//时间类
			if(timenow < timetoday)
			{
				CCLOG("Time ERROR:LoginRewardModel::getWelcomeData");
				break;
			}
				
			//正常情况
			unsigned int deltatime = timenow - timetoday;//当日秒数
			unsigned int begintime = pData->m_hourBegin * 3600 + pData->m_minBegin * 60;
			unsigned int endtime = pData->m_hourEnd * 3600 + pData->m_minEnd * 60;
			if(deltatime >= begintime && deltatime <= endtime)
			{
				pReturnData = pData;
				break;
			}
		}
	}
	return pReturnData;
}

void LoginRewardModel::LoadWelcomeData(const CSVDecoder::CSV &val)
{
	//m_pLoginReward->removeAllObjects();

	//当前时间
	unsigned int timenow = GameManager::sharedGameManager()->getServerTime();
	//今日零点时间
	unsigned int timetoday = TimeUtil::getStartTimeOfToday(timenow);
	//第1行 开关
	int isOpen = atoi(val[0][0].c_str());
	m_bWelcomeOpen = (isOpen == 1);
	//从第2行开始读 
	for (uint32_t i=1; i<val.size(); ++i)
	{
		loginreward::WelcomeData *pData = loginreward::WelcomeData::create();
		pData->m_id = atoi(val[i][0].c_str());
		pData->m_type = atoi(val[i][1].c_str());
		if(pData->m_type == 1)
		{
			//日期
			std::string datestr = val[i][2];
			uint16_t m_year = 0;
			uint16_t m_month = 0;
			uint16_t m_day = 0;

			char* source = NULL;
			char* token = NULL;

			source = const_cast<char*>(datestr.c_str());
			token = strtok(source , "/");
			while(token != NULL)
			{
				if(m_year == 0) m_year = atoi(token);
				else (m_month == 0 ? m_month = atoi(token) : m_day = atoi(token));
				token = strtok(NULL , "/");
			}
			time_t time = (time_t)timetoday;
			struct tm *pTm = localtime(&time);
			pTm->tm_year = m_year - 1900;
			pTm->tm_mon = m_month - 1;
			pTm->tm_mday = m_day;

			pData->m_dateTs = mktime(pTm);
		}
		else
		{
			//时间
			std::string timestr = val[i][3];
			char* source = NULL;
			char* token = NULL;
			source = const_cast<char*>(timestr.c_str());
			token = strtok(source , ":");
			while(token != NULL)
			{
				if(pData->m_hourBegin == 0) pData->m_hourBegin = atoi(token);
				else pData->m_minBegin = atoi(token);
				token = strtok(NULL , ":");
			}

			std::string timestr2 = val[i][4];
			char* source2 = NULL;
			char* token2 = NULL;
			source2 = const_cast<char*>(timestr2.c_str());
			token2 = strtok(source2 , ":");
			while(token2 != NULL)
			{
				if(pData->m_hourEnd == 0) pData->m_hourEnd = atoi(token2);
				else pData->m_minEnd = atoi(token2);
				token2 = strtok(NULL , ":");
			}
		}

		pData->m_imgIndex = atoi(val[i][5].c_str());
		pData->m_soundPath = val[i][6];
		pData->m_content = D_LOCAL_STRING(val[i][7]);

		m_pWelcomeList->addObject(pData);
	}
}

void LoginRewardModel::ParseLoginRewardData(const CSVDecoder::CSV &val)
{
	m_pLoginReward->removeAllObjects();
	for (uint32_t i=0; i<val.size(); ++i)
	{
		int iNum = atoi(val[i][0].c_str());
		if (iNum == 0)
		{
			continue;
		}

		loginreward::LoginRewardData *pData = loginreward::LoginRewardData::create();
		if (pData)
		{
			pData->price_type	= iNum;
			pData->price_number = (uint32_t)(atol(val[i][1].c_str()));
			pData->icon_id		= (uint32_t)(atoi(val[i][2].c_str()));
			pData->days			= (uint32_t)(atoi(val[i][3].c_str()));

			m_pLoginReward->addObject(pData);
		}
	}
}

void LoginRewardModel::getLoginAwardResult(uint16_t days, uint16_t uDropType, uint32_t uDropValue, uint16_t userLv, loginreward::LoginRewardResult &l)
{
	l.clear();
	cocos2d::CCObject *pObj = NULL;
	loginreward::LoginRewardData *pData = NULL;

	std::vector<loginreward::LoginRewardData*> vecFakeData;
	CCARRAY_FOREACH(m_pLoginReward, pObj)
	{
		pData = (loginreward::LoginRewardData*)pObj;
		if (pData)
		{
			if (pData->days != days)
			{
				continue;
			}

			bool bFake = true;
			int nResult = 0;
			if (pData->price_type == uDropType)
			{
				if (pData->price_type == drop::kDropTypeCoin)//金币
				{
					nResult = pData->price_number*((int)((powf(1.12f, (3*userLv-1)/3.f))));
				}
				else
				{
					nResult = pData->price_number;
				}

				if (nResult == uDropValue)
				{
					bFake = false;
				}
			}

			if (bFake)
			{
				vecFakeData.push_back(pData);
			}
			else
			{
				l.uTrueID = pData->icon_id;
				l.uTrueValue = uDropValue;
			}
		}
	}

	std::random_shuffle(vecFakeData.begin(), vecFakeData.end());
	int nPos = 0;
	for (std::vector<loginreward::LoginRewardData*>::iterator si=vecFakeData.begin();si!=vecFakeData.end(); ++si)
	{
		loginreward::LoginRewardData* pData = *si;
		if (pData && pData->icon_id!=l.uTrueID)
		{
			bool bFind = false;
			for (int i=0; i<nPos; ++i)
			{
				if (l.uFakeID[i] == pData->icon_id)
				{
					bFind = true;
					break;
				}
			}

			if (!bFind)
			{
				l.uFakeID[nPos] = pData->icon_id;

				int nResult = 0;
				if (pData->price_type == drop::kDropTypeCoin)//金币
				{
					nResult = pData->price_number*((int)((powf(1.12f, (3*userLv-1)/3.f))));
				}
				else
				{
					nResult = pData->price_number;
				}

				l.uFakeValue[nPos] = nResult;

				nPos++;
				if (nPos >= FAKE_NUM)
				{
					return;
				}
			}
		}
	}
}

bool LoginRewardModel::IsHaveLoginAward()
{
	if (loginTimes > 0)
	{
		for (int i=0; i<loginTimes; ++i)
		{
			int nTmp = (loginAward>>i)&0x01;
			if (nTmp == 0)
			{
				return true;
			}
		}
	}

	return false;
}

void LoginRewardModel::GetLoginAwardDays(loginreward::LoginRewardDays &days)
{
	for (int i=0; i<MAX_LOGIN_REWARD_DAYS; ++i)
	{
		days[i] = false;
	}

	if (loginTimes > 0)
	{
		for (int i=0; i<loginTimes; ++i)
		{
			int nTmp = (loginAward>>i)&0x01;
			days[i] = (nTmp == 0);
		}
	}
}
