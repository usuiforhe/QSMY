//
//  WeeklyModel.cpp
//  活动
//
//  created by Eci on 14-5-28.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "WeeklyModel.h"
#include "managers/GameManager.h"

USING_NS_CC;

using namespace weekly;

namespace weekly
{
	WeeklyTypeInfo::WeeklyTypeInfo()
		: m_uType(0)
		, m_uNav(0)
		, m_bResident(false)
		, m_bActive(false)
	{
		
	}
	WeeklyTypeInfo::~WeeklyTypeInfo()
	{

	}

	bool WeeklyTypeInfo::isActive()
	{
		return m_bResident || m_bActive;
	}
}

WeeklyModel::WeeklyModel()
	: m_pWeeklyList(NULL)
	, m_pWeatherEffect(NULL)
	, m_uOpenTs(0)
{

}

WeeklyModel::~WeeklyModel()
{
	CC_SAFE_RELEASE(m_pWeeklyList);
	CC_SAFE_RELEASE(m_pWeatherEffect);
}

bool WeeklyModel::init()
{
	loadWeeklyTypeInfo(CSVDecoder::create("data/weekly/WeeklyTypeInfo")->getData());
	loadWeeklyWeatherEffect(CSVDecoder::create("data/weekly/weather_effect")->getData());
	return Model::init();
}


void WeeklyModel::loadWeeklyTypeInfo(const CSVDecoder::CSV &val)
{
	if (m_pWeeklyList == NULL)
	{
		m_pWeeklyList = CCDictionary::create();
		CC_SAFE_RETAIN(m_pWeeklyList);
	}
	else
	{
		m_pWeeklyList->removeAllObjects();
	}
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		WeeklyTypeInfo *pInfo = WeeklyTypeInfo::create();
		pInfo->m_uType = atoi(val[i][0].c_str());
		pInfo->m_uNav = atoi(val[i][1].c_str());
		pInfo->m_bResident = atoi(val[i][2].c_str());
		m_pWeeklyList->setObject(pInfo, pInfo->m_uType);
	}
}

void WeeklyModel::loadWeeklyWeatherEffect(const CSVDecoder::CSV &val)
{
	if (m_pWeatherEffect == NULL)
	{
		m_pWeatherEffect = CCDictionary::create();
		CC_SAFE_RETAIN(m_pWeatherEffect);
	}
	else
	{
		m_pWeatherEffect->removeAllObjects();
	}
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		uint32_t weather = atoi(val[i][0].c_str());
		uint32_t effect = atoi(val[i][1].c_str());
		m_pWeatherEffect->setObject(CCInteger::create(effect), weather);
	}
}

bool WeeklyModel::isWeekly(navigation::NavigationType eNav)
{
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(m_pWeeklyList, pEle)
	{
		WeeklyTypeInfo *pInfo = dynamic_cast<WeeklyTypeInfo *>(pEle->getObject());
		if (!pInfo)
		{
			continue;
		}
		if (pInfo->m_uNav == eNav)
		{
			return true;
		}

	}
	return false;
}


bool WeeklyModel::isWeeklyActive(navigation::NavigationType eNav)
{
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(m_pWeeklyList, pEle)
	{
		WeeklyTypeInfo *pInfo = dynamic_cast<WeeklyTypeInfo *>(pEle->getObject());
		if (!pInfo)
		{
			continue;
		}
		if (pInfo->m_uNav == eNav)
		{
			return pInfo->isActive();
		}
		
	}
	return false;
}

void WeeklyModel::parseInfo(const Json::Value& val)
{
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
		unsigned int uType = (*i).asUInt();
		WeeklyTypeInfo *pInfo = dynamic_cast<WeeklyTypeInfo *>(m_pWeeklyList->objectForKey(uType));
		if (pInfo)
		{
			pInfo->m_bActive = true;
		}
	}
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_WEEKLY_UPDATE);
}

void WeeklyModel::parseWeekly(const Json::Value& val)
{
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(m_pWeeklyList, pEle)
	{
		WeeklyTypeInfo *pInfo = dynamic_cast<WeeklyTypeInfo *>(pEle->getObject());
		if (pInfo)
		{
			pInfo->m_bActive = false;
		}

	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
		unsigned int uType = (*i)["type"].asUInt();
		WeeklyTypeInfo *pInfo = dynamic_cast<WeeklyTypeInfo *>(m_pWeeklyList->objectForKey(uType));
		if (pInfo)
		{
			pInfo->m_bActive = true;

			pInfo->m_uWeather = (*i)["weather"].asUInt();
		}
	}
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_WEEKLY_UPDATE);
}

void WeeklyModel::parseOneWeekly(const Json::Value& val)
{
	unsigned int uType = val["type"].asUInt();
	WeeklyTypeInfo *pInfo = dynamic_cast<WeeklyTypeInfo *>(m_pWeeklyList->objectForKey(uType));
	if (pInfo)
	{
		pInfo->m_uWeather = val["weather"].asUInt();
	}
}

void WeeklyModel::reset()
{
	// 不接受零点重置
}

uint32_t WeeklyModel::getWeatherEffect(unsigned int weeklyType)
{
	WeeklyTypeInfo *pInfo = dynamic_cast<WeeklyTypeInfo *>(m_pWeeklyList->objectForKey(weeklyType));
	if (pInfo)
	{
		CCInteger *pEff = dynamic_cast<CCInteger *>(m_pWeatherEffect->objectForKey(pInfo->m_uWeather));
		if (pEff)
		{
			return pEff->getValue();
		}
	}
	
	return 0;
}

bool WeeklyModel::isOpen()
{
	return GameManager::sharedGameManager()->getServerTime() > m_uOpenTs;
}

unsigned int WeeklyModel::getDaysToUnlock()
{
	return (m_uOpenTs - GameManager::sharedGameManager()->getServerTime()) / 86400 + 1;
}