//
//  ActivityModel.cpp
//  活动
//
//  created by Eci on 14-3-24.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "ActivityModel.h"
#include "managers/GameManager.h"

USING_NS_CC;

using namespace activity;

namespace activity
{
	OnlineAwardInfo::OnlineAwardInfo()
		: m_uTime(0)
		, m_uID(0)
		, m_pDropList(NULL)
	{
	}
	OnlineAwardInfo::~OnlineAwardInfo()
	{
		CC_SAFE_RELEASE(m_pDropList);
	}
	bool OnlineAwardInfo::init()
	{
		m_pDropList = CCArray::create();
		CC_SAFE_RETAIN(m_pDropList);
		return ValueObjcet::init();  
	}

	void OnlineAwardInfo::loadData(const CSVDecoder::CSV &val)
	{
		//int type = atoi(val[2].c_str());
		//int id = atoi(val[3].c_str());
		//int num = atoi(val[4].c_str());

		//drop::DropType droptype = (drop::DropType)type;
		//drop::Drop* dropinfo = drop::Drop::create(droptype , id , num);

		//if(dropinfo)
		//{
		//	m_DropList.addObject(dropinfo);
		//}
		//else{
		//	//数据解析错误
		//	CCLOG("OnlineAwardInfo::loadData error!");
		//}
	}




	SoulPointInfo::SoulPointInfo()
		: m_nEndTs(0)
		, m_vSoulPointFromCard()
		, m_vWarriorID()
		, m_uSoulPoint(0)
		, m_uSoulPointAdded(0)
	{

	}
	bool SoulPointInfo::isOpen()
	{
		uint32_t serverTime = GameManager::sharedGameManager()->getServerTime();
		return serverTime < m_nEndTs;
	}

	bool SoulPointInfo::init()
	{
		if( !ValueObjcet::init() )
		{
			return false;
		}
		loadSoulPointFromCard(  CSVDecoder::create(SOUL_POINT_CONFIG_PATH)->getData());
		return true;  
	}

	void SoulPointInfo::loadSoulPointFromCard(const CSVDecoder::CSV &val)
	{
		for (uint32_t i = 0; i < val.size(); ++i)
		{
			int point = atoi(val[i][1].c_str());
			m_vSoulPointFromCard.push_back(point);
		}
	}
	uint32_t SoulPointInfo::getSoulPointFromCardAtIndex(int idx)
	{
		return m_vSoulPointFromCard[idx];
	}
}

ActivityModel::ActivityModel()
	: m_pSoulPointInfo(NULL)
	,m_pOnlineAwardList(NULL)
	,m_uOnlineAwardTimes(0)
	,m_uOnlineTime(0)
	,isOnlineAwardSynced(false)
{

}

ActivityModel::~ActivityModel()
{
	CC_SAFE_RELEASE(m_pSoulPointInfo);
	CC_SAFE_RELEASE(m_pOnlineAwardList);
}

bool ActivityModel::init()
{
	m_pSoulPointInfo = activity::SoulPointInfo::create();
	CC_SAFE_RETAIN(m_pSoulPointInfo);

	m_pOnlineAwardList = CCDictionary::create();
	CC_SAFE_RETAIN(m_pOnlineAwardList);
	loadOnlineAwardData(CSVDecoder::create(ONLINEAWARD_CONFIG_PATH)->getData());
	
	return Model::init();
}

bool ActivityModel::showAward()
{
	if(m_pOnlineAwardList && m_pOnlineAwardList->count()!= 0)
	{
		//当已领取的已经是最后一个的时候，说明都灵过了，不必再显示在线奖励标志
		return (m_uOnlineAwardTimes != m_pOnlineAwardList->count());
	}
	else{
		//load data不成功 不显示在线奖励标志
		return false;
	}
}

unsigned int ActivityModel::getOnlineAwardCountdown()
{
	unsigned int countdown = 0;

	OnlineAwardInfo* pInfo = NULL;
	pInfo = dynamic_cast<OnlineAwardInfo*>(m_pOnlineAwardList->objectForKey(m_uOnlineAwardTimes + 1));
	if(pInfo)
	{
		//防止溢出
		int maxtime = pInfo->getTime();
		int elapsedtime = GameManager::sharedGameManager()->getServerTime() - m_uOnlineTime;
		//返回剩余时间，或0
		countdown = (maxtime > elapsedtime) ? (maxtime - elapsedtime) : 0;
	}
	
	return countdown;	
}

void ActivityModel::parseOnlineAward(const Json::Value &requestData, const Json::Value &responseData)
{
	unsigned int onlineGift = responseData["onlineGift"].asUInt();
	unsigned int onlineGiftTs = responseData["onlineGiftTs"].asUInt();
	m_uOnlineAwardTimes = onlineGift;
	m_uOnlineTime = onlineGiftTs;
	//通知界面更新
	isOnlineAwardSynced = true;
	postNotification(D_EVENT_ACTIVITY_ONLINE_AWARD);	
}


void ActivityModel::loadOnlineAwardData(const CSVDecoder::CSV &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{//ID	TaskID LastTaskID	TaskCategory	TaskType	TaskColor	CompleteID	TaskNum	TaskRewardType	TaskRewardID	RewardNum	TaskScore	TaskDescription
		
		unsigned int id = atoi(val[i][0].c_str());

		if(!m_pOnlineAwardList->objectForKey(id)){
			OnlineAwardInfo* pNewInfo = OnlineAwardInfo::create();
			pNewInfo->m_uID = id;
			pNewInfo->m_uTime = (atoi(val[i][1].c_str())) * 60;//配表里是分钟，程序里用秒
			m_pOnlineAwardList->setObject(pNewInfo , id);
		}

		OnlineAwardInfo* pInfo = NULL;
		pInfo = dynamic_cast<OnlineAwardInfo*>(m_pOnlineAwardList->objectForKey(id));
		//pInfo->loadData(val[i]);

		//读取信息
		unsigned int type = atoi(val[i][2].c_str());
		unsigned int awardid = atoi(val[i][3].c_str());
		unsigned int num = atoi(val[i][4].c_str());

		drop::DropType droptype = (drop::DropType)type;
		drop::Drop* dropinfo = drop::Drop::create(droptype , awardid , num);

		if(dropinfo)
		{
			pInfo->m_pDropList->addObject(dropinfo);
		}
		else{
			//数据解析错误
			CCLOG("OnlineAwardInfo::loadData error!");
		}
	}
}

void ActivityModel::parseInfo(const Json::Value& val)
{
	for (uint32_t i=0; i<val.size(); ++i){
		int type = val[i]["type"].asInt();
		switch (type) {
		case activity::ActivityTypeSoulExchange:
			parseSoulExchange(val[i]);
			break;

		default:
			break;
		}
	}
	isSynced = true;
	postNotification(D_EVENT_ACTIVITY_REFRESH_CPP);
}

void ActivityModel::parseSoulExchange(const Json::Value& val)
{
	uint32_t oldSp = m_pSoulPointInfo->m_uSoulPoint;
	DJSON_UINT(m_pSoulPointInfo->m_uSoulPoint, val, "point");
	m_pSoulPointInfo->m_uSoulPointAdded = 0;
	if (this->isSynced)
	{
		m_pSoulPointInfo->m_uSoulPointAdded = m_pSoulPointInfo->m_uSoulPoint - oldSp;
	}
	
	DJSON_UINT(m_pSoulPointInfo->m_nEndTs, val, "endTs");
	if (val.isMember("recommend"))
	{
		m_pSoulPointInfo->m_vWarriorID.clear();
		for (uint32_t i = 0; i < val["recommend"].size(); i++)
		{
			uint32_t wid = val["recommend"][i].asUInt();
			m_pSoulPointInfo->m_vWarriorID.push_back(wid);
		}
	}
	postNotification(D_EVENT_ACTIVITY_SOUL_EXCHANGE_REFRESH);
	CCLOG("SOULPOINT:%u", m_pSoulPointInfo->m_uSoulPoint);
}

void ActivityModel::syncFromServer()
{
	if (!isSynced)
	{
		HTTP_CLIENT->activityGet();
	}
}

void ActivityModel::reset()
{
	// 不接受零点重置
}

void ActivityModel::setSpecialActivityType(unsigned int type)
{
    if(m_iSpecialActivityType != type)
    {
        m_iSpecialActivityType = type;
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SPECIAL_ACTIVI_CHANGED);
    }
}