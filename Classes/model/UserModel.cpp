//
//  UserModel.cpp
//  QSMY
//
//  Created by cll on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "UserModel.h"
#include "managers/GameManager.h"
#include "utils/StringUtil.h"
#include "utils/TimerUtil.h"

#define USERINFO_REQUEST_INTERVAL 200

using namespace user;
USING_NS_CC;

UserModel::UserModel()
:m_userInfo(NULL)
,m_userBaseMap(NULL)
,m_userUpAwardMap(NULL)
,m_vipInfos(NULL)
,m_firstPunchInfos(NULL)
,m_bLogin(false)
,m_nMaxVipLv(0)
,m_pDictPrivileges(NULL)
,m_bPrivilegesSynced(false)
,m_nNearestEndTs(UINT32_MAX)
,m_ExploreActivityInfo(NULL)
{
	
}



UserModel::~UserModel()
{
	UNBINDING(m_userInfo, this, D_EVENT_RESET_SCHEDULE);
	CC_SAFE_RELEASE(m_userUpAwardMap);
    CC_SAFE_RELEASE(m_userBaseMap);
	CC_SAFE_RELEASE(m_userInfo);
	CC_SAFE_RELEASE(m_vipInfos);
	CC_SAFE_RELEASE(m_firstPunchInfos);
	CC_SAFE_RELEASE(m_pDictPrivileges);
	CC_SAFE_RELEASE(m_ExploreActivityInfo);
    TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(UserModel::updateUserInfo), this);
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(UserModel::refreshPrivileges), this);
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

bool UserModel::init()
{
    m_userInfo = UserInfo::create();
    m_userBaseMap = UserBaseMap::create();
	m_userUpAwardMap = UserUpAwardMap::create();
    m_vipInfos = CCArray::create();
	m_firstPunchInfos = CCArray::create();
	m_pDictPrivileges = CCDictionary::create();
	m_ExploreActivityInfo = ExporeActivityInfo::create();
    CC_SAFE_RETAIN(m_userBaseMap);
	CC_SAFE_RETAIN(m_userUpAwardMap);
	CC_SAFE_RETAIN(m_userInfo);
	CC_SAFE_RETAIN(m_vipInfos);
	CC_SAFE_RETAIN(m_firstPunchInfos);
	CC_SAFE_RETAIN(m_pDictPrivileges);
	CC_SAFE_RETAIN(m_ExploreActivityInfo);
    
	loadUserLvExpData( CSVDecoder::create(USER_LV_EXP_CONFIG_PATH)->getData());
	loadUserUpAwardData( CSVDecoder::create(USER_UP_AWARD_CONFIG_PATH)->getData());
	loadVipInfoData( CSVDecoder::create(VIPS_CONFIG_PATH)->getData());
	loadFirstPunchInfoData( CSVDecoder::create(FIRST_PUNCH_CONFIG_PATH)->getData());
	loadPrivileges( CSVDecoder::create(PRIVILEGES_CONFIG_PATH)->getData());
    TimerUtil::sharedTimerUtil()->schedule(schedule_selector(UserModel::updateUserInfo), this, USERINFO_REQUEST_INTERVAL); //FIXME: 临时加下
    //loadCookie();

	BINDING_EXEC(m_userInfo,this,UserModel::resetSchedule,D_EVENT_RESET_SCHEDULE);
    
    NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(UserModel::onBuyMonthCard), D_EVENT_BUY_MONTHCARD);
    return Model::init();
}

void UserModel::parseExploreActivity(const Json::Value &val)
{
	//进游戏初始化副本活动数据
	//{"id":267,"type":43,"startTs":1425139200,"endTs":1427212800,"exploreActivityId":1,"activityMid":20001}
	
	DJSON_UINT(m_ExploreActivityInfo->m_ExploreActivityId, val, "exploreActivityId");
	DJSON_UINT(m_ExploreActivityInfo->m_StartTs, val, "startTs");
	DJSON_UINT(m_ExploreActivityInfo->m_EndTs, val, "endTs");
	DJSON_UINT(m_ExploreActivityInfo->m_AwardState,val,"isGetAward");
	DJSON_UINT(m_ExploreActivityInfo->m_ActivityMid,val,"activityMid");

	/*Json::Value::Members members(val.getMemberNames());
	for (Json::Value::Members::reverse_iterator it = members.rbegin(); it!=members.rend(); ++it) {
		const std::string &key =*it;
		CCLOG("m_ExploreActivityInfo====:%s:%d",key.c_str(),val[key.c_str()]);
	}*/
}

void UserModel::onBuyMonthCard()
{
    getUserInfo()->monthcard = true;
}

void UserModel::updateUserInfo(float dt)
{
    HTTP_CLIENT->getUserInfo(NULL,NULL,true,false);
}



void UserModel::loadUserUpAwardData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		UserUpAward *pAward = UserUpAward::create();
		pAward->userLv = atoi(val[i][0].c_str());
		pAward->coins = atoi(val[i][1].c_str());
		pAward->points = atoi(val[i][2].c_str());
		pAward->unlockPosNum = atoi(val[i][3].c_str());
        m_userUpAwardMap->setObject(pAward, pAward->userLv);
	}
}

void UserModel::loadUserLvExpData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		UserBase *pUser = UserBase::create();
		pUser->userLv = atoi(val[i][0].c_str());
		pUser->maxExp = atoi(val[i][1].c_str());
		pUser->totalExp = atoi(val[i][2].c_str());
		pUser->maxPower = atoi(val[i][3].c_str());//USER_MAX_POWER;
		pUser->maxEndu = atoi(val[i][4].c_str());
        
		//pUser->maxVitality = atoi(val[i][3].c_str());
        
		m_userBaseMap->setObject(pUser, pUser->userLv);
	}
}



void UserModel::loadVipInfoData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		VipInfo *pVipInfo = VipInfo::create();
        
		pVipInfo->vip_lv = atoi(val[i][0].c_str());
        pVipInfo->rmb = 0.1f*atoi(val[i][1].c_str());
		pVipInfo->point = atoi(val[i][1].c_str());
		pVipInfo->vip_desc = val[i][2].c_str();
		//pVipInfo->vip_id = atoi(val[i][3].c_str());
        pVipInfo->addPowerCountMax  = atoi(val[i][3].c_str());
        pVipInfo->prestige_ratio  = atof(val[i][4].c_str())+0.0000001f;
        pVipInfo->vip_item_id = atoi(val[i][5].c_str());
        pVipInfo->vip_box_id = atoi(val[i][6].c_str());
		pVipInfo->m_nPvpMax = atoi(val[i][7].c_str());
		pVipInfo->m_nPractiseMax = atoi(val[i][8].c_str());
		pVipInfo->m_nFriendsMax = atoi(val[i][9].c_str());;
		pVipInfo->m_nTeamResetMax = atoi(val[i][10].c_str());;
        
        //台湾版本增加了一个字段表示vip累计需要充值的数量其他版本没有这个字段
        if(val[i].size()==9){
            pVipInfo->rmb = atoi(val[i][8].c_str());
        }
        replace(pVipInfo->vip_desc,"\\n","\n");
        m_nMaxVipLv = pVipInfo->vip_lv>m_nMaxVipLv?pVipInfo->vip_lv:m_nMaxVipLv;
		m_vipInfos->addObject(pVipInfo);
	}
}

void UserModel::loadFirstPunchInfoData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		FirstPunchInfo *pInfo = FirstPunchInfo::create();

		pInfo->m_nIndex = atoi(val[i][0].c_str());
		pInfo->m_nType = drop::DropType(atoi(val[i][1].c_str()));
		pInfo->m_nID = atoi(val[i][2].c_str());
		pInfo->m_nNum = atoi(val[i][3].c_str());

		m_firstPunchInfos->addObject(pInfo);
	}
}


void UserModel::parseUserInfo(const Json::Value &val,bool* isLevelUp)
{
    uint32_t lv = m_userInfo->iUserLv;
    uint8_t viplv = m_userInfo->iVipLevel;
	parseUser(val,m_userInfo);
    bool levelUp = lv!=0 && m_userInfo->iUserLv>lv;
    bool vipLvelUp = m_userInfo->iVipLevel>viplv;
    
    if(isLevelUp) *isLevelUp = levelUp;
    
	//m_userInfo->baseInfo = getUserBaseByLv(m_userInfo->iUserLv);
    m_userInfo->postNotification(D_EVENT_USERINFO_UPDATE);
    if(levelUp)
	{
		m_userInfo->getUpAward();
        postNotification(D_EVENT_USER_LEVELUP);
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_USER_LEVELUP);
	}
    
    if(vipLvelUp)
    {
        postNotification(D_EVENT_USER_VIP_LEVELUP);
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_USER_VIP_LEVELUP);
    }
}

user::UserBase* UserModel::getUserBaseByLv(uint32_t lv)
{
    return (UserBase*)m_userBaseMap->objectForKey(lv);
}

user::UserUpAward* UserModel::getUserUpAwardByLv(uint32_t lv)
{
	return (UserUpAward*)m_userUpAwardMap->objectForKey(lv);
}

user::VipInfo* UserModel::getVipInfoByVipLevel(uint8_t level)
{
    if(level==0) return VipInfo::create();
    user::VipInfo* pInfo = NULL;
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_vipInfos, obj)
    {
        pInfo = static_cast<user::VipInfo*>(obj);
        if(pInfo->vip_lv==level)
        {
            return pInfo;
        }
    }
    return NULL;
}


uint32_t UserModel::getToVipLevelCost(uint8_t level)
{
	/************************************************************************/
	/* @fengkerong.billy
		返回單位從RMB改爲元寶
		策劃沒配表，元寶/RMB兌換率寫死10...
	*/
	/************************************************************************/
    //return getVipInfoByVipLevel(level)->rmb-getUserInfo()->amount;
	uint32_t nextVipPoint = getVipInfoByVipLevel(level)->point;
	uint32_t currentPoint = getUserInfo()->amount;
	return nextVipPoint - currentPoint;
}

UserInfo * UserModel::parseUser(const Json::Value &val, UserInfo *pUserInfo)
{
	if(pUserInfo == NULL)
	{
		pUserInfo = UserInfo::create();
	}
    
	//int32_t sp = 0;
    
	DJSON_UINT(pUserInfo->iUserId,		val, "uid");
	DJSON_STRING(pUserInfo->sPlatId,	val, "pid");
	DJSON_STRING(pUserInfo->sUserName,	val, "name");
	DJSON_UINT(pUserInfo->iUserLv,		val, "lv");
	DJSON_UINT(pUserInfo->iUserExp,		val, "exp");
	DJSON_UINT(pUserInfo->iPoints,		val, "point");
	DJSON_UINT(pUserInfo->iCoins,		val, "coin");
	DJSON_INT(pUserInfo->iPower,		val, "pow");
	uint32_t updateTime = 0;
	//if(val.isMember("pow"))
	//{
	//	updateTime = GameManager::sharedGameManager()->getServerTime();
	//}
	DJSON_UINT(updateTime, val, "up_ts");
	if (updateTime)
	{
		pUserInfo->m_nUpdatePowerTs = updateTime;
		pUserInfo->postNotification(D_EVENT_RESET_SCHEDULE);
	}
	//耐力值
	DJSON_INT(pUserInfo->iEndurance,		val, "endurance");
	uint32_t enduUpdateTime = 0;
	//if(val.isMember("endurance"))
	//{
	//	enduUpdateTime = GameManager::sharedGameManager()->getServerTime();
	//}
	DJSON_UINT(enduUpdateTime, val, "endu_up_ts");
	if (enduUpdateTime)
	{
		pUserInfo->m_nUpdateEnduTs = enduUpdateTime;
	}

	DJSON_UINT(pUserInfo->iVipLevel,	val, "vip");
	DJSON_UINT(pUserInfo->mid,			val, "mid");
    DJSON_BOOL(pUserInfo->charged,      val, "charged");
    DJSON_UINT(pUserInfo->amount,      val, "tamount");
    DJSON_BOOL(pUserInfo->monthcard,    val, "monthcard");
	uint32_t nTmpRush = pUserInfo->lastRush;
	DJSON_UINT(pUserInfo->lastRush,		val, "lastRush");
	if (nTmpRush != pUserInfo->lastRush)
	{
		postNotification(D_EVENT_PVE_RUSH_TIME);
	}
    
	DJSON_UINT(pUserInfo->vip_bag,val,"vip_bag");
    DJSON_UINT(pUserInfo->pow_times,val,"pow_times");
    DJSON_UINT(pUserInfo->reg_ts, val, "reg_ts");
	//DJSON_UINT(pUserInfo->iActiveness,val,"act");
    
	return pUserInfo;
    
}

void UserModel::resetSchedule()
{
	uint32_t time = K_POWER_REFRESH_INTERVAL;
	if (m_userInfo)
	{
		uint32_t recTime = m_userInfo->getPowerRecTime(false);
		if (recTime)
		{
			time = recTime;
		}
		
	}
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(UserModel::updateUserInfo), this);
	TimerUtil::sharedTimerUtil()->schedule(schedule_selector(UserModel::updateUserInfo), this, USERINFO_REQUEST_INTERVAL);
}


void UserModel::parsePrivileges(const Json::Value &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		Json::Value v = val[i];
		uint32_t nID = 0;
		DJSON_UINT(nID, v, "id");
		Privilege *pPrivilege = (Privilege *)m_pDictPrivileges->objectForKey(nID);
		if (!pPrivilege)
		{
			continue;
		}
		pPrivilege->m_bEnabled = true;
		DJSON_UINT(pPrivilege->m_nEndTs, v, "endTs");
		if (pPrivilege->m_nEndTs < m_nNearestEndTs)
		{
			m_nNearestEndTs = pPrivilege->m_nEndTs;
		}
	}
	m_bPrivilegesSynced = true;
	uint32_t serverTime = GameManager::sharedGameManager()->getServerTime();
	if (m_nNearestEndTs < UINT32_MAX)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(UserModel::refreshPrivileges), this);
		if (m_nNearestEndTs > serverTime)
		{
			TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(UserModel::refreshPrivileges), this, m_nNearestEndTs - serverTime + 1);
		}
		else
		{
			TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(UserModel::refreshPrivileges), this, 1);
		}
	}
	postNotification(D_EVENT_GET_PRIVILEGES);
}

void UserModel::refreshPrivileges(float dt)
{
	resetPrivileges();
	HTTP_CLIENT->userPrivileges();
	m_nNearestEndTs = UINT32_MAX;
}

void UserModel::getPrivileges(cocos2d::CCArray *pArr)
{
	if (!pArr)
	{
		return;
	}
	pArr->removeAllObjects();

	CCDictElement *pElement = NULL;
	Privilege *pPrivilege = NULL;
	CCDICT_FOREACH(m_pDictPrivileges, pElement)
	{
		pPrivilege = dynamic_cast<Privilege *>(pElement->getObject());
		if (pPrivilege->m_bEnabled)
		{
			pArr->addObject(pPrivilege);
		}
		
	}
}

void UserModel::loadPrivileges(const CSVDecoder::CSV &val)
{

	for (uint32_t i = 0; i < val.size(); ++i)
	{
		Privilege *pPrivilege = Privilege::create();
		pPrivilege->m_nID = atoi(val[i][0].c_str());
		pPrivilege->m_sContent = val[i][1];
		pPrivilege->m_bEnabled = false;
		pPrivilege->m_nEndTs = 0;
		pPrivilege->m_ePrivilegeType = PrivilegeTypeNone;
		if (atoi(val[i][2].c_str()))
		{
			pPrivilege->m_ePrivilegeType |= PrivilegeTypeAutoEnhance;
		}
		if (atoi(val[i][3].c_str()))
		{
			pPrivilege->m_ePrivilegeType |= PrivilegeTypeRush;
		}
		if (atoi(val[i][4].c_str()))
		{
			pPrivilege->m_ePrivilegeType |= PrivilegeTypeBuyExplore;
		}
		if (atoi(val[i][5].c_str()))
		{
			pPrivilege->m_ePrivilegeType |= PrivilegeTypeSpeed;
		}
		m_pDictPrivileges->setObject(pPrivilege, pPrivilege->m_nID);
	}
}

void UserModel::resetPrivileges()
{
	CCDictElement *pElement = NULL;
	Privilege *pPrivilege = NULL;
	CCDICT_FOREACH(m_pDictPrivileges, pElement)
	{
		pPrivilege = dynamic_cast<Privilege *>(pElement->getObject());
		pPrivilege->m_bEnabled = false;
	}
	m_bPrivilegesSynced = false;
}

bool UserModel::checkPrivilegeType(user::PrivilegeType eType)
{
	CCDictElement *pElement = NULL;
	Privilege *pPrivilege = NULL;
	CCDICT_FOREACH(m_pDictPrivileges, pElement)
	{
		pPrivilege = dynamic_cast<Privilege *>(pElement->getObject());
		if (pPrivilege->m_bEnabled && pPrivilege->m_ePrivilegeType & eType)
		{
			return true;
		}
	}
	return false;
}

uint32_t UserInfo::getRemainBuyPowTimes()
{
    return DM_GET_USER_MODEL->getVipInfoByVipLevel(iVipLevel)->getAddPowerCountMax()-pow_times;
}

//增加经验
void UserInfo::addExp(uint32_t exp, float fDelayTime)
{
    if(checkLevelLimit()) return;
	if (exp > 0)
	{
		UserBase * pUserBase = DM_GET_USER_MODEL->getUserBaseByLv(iUserLv+1);
		iUserExp += exp;
		postNotification(D_EVENT_USERINFO_UPDATE, fDelayTime);
		if (iUserExp >= pUserBase->getTotalExp())
		{
			iUserLv++;	//升级
			getUpAward();
            postNotification(D_EVENT_USER_LEVELUP, fDelayTime);
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_USER_LEVELUP, fDelayTime);
		}
	}
}

bool UserInfo:: checkLevelLimit()
{
    return iUserLv >= D_CONFIG_UINT_FOR_KEY(USER_LEVEL_MAX);
}

void UserInfo::updatePoints(unsigned int point, float fDelayTime)
{
	if (point == iPoints) return;
	
    iPoints = point;
    postNotification(D_EVENT_USERINFO_UPDATE, fDelayTime);
	
}


void UserInfo::updateCoins(unsigned int coin, float fDelayTime)
{
	if (coin == iCoins) return;
	
    iCoins = coin;
    postNotification(D_EVENT_USERINFO_UPDATE, fDelayTime);
	
}

void UserInfo::changeCoins(int coin, float fDelayTime)
{
	//不能小于0
	int result = MAX(iCoins+coin , 0);
	updateCoins(result,fDelayTime);
}


void UserInfo::changePoints(int point, float fDelayTime)
{
	updatePoints(iPoints+point,fDelayTime);
}

void UserInfo::changeEnduranceBy(int delta)
{
	int lastEndurance = iEndurance;
	lastEndurance += delta;
	iEndurance = lastEndurance>0? lastEndurance : 0;
	//这里应该不需要立即post
	postNotification(D_EVENT_USERINFO_UPDATE);
}

void UserInfo::changePower(int power, bool bAdd, float fDelayTime)
{
	//CCLOG("UserInfo::changePower power:%d , iPower:%d , bAdd%d" , power , iPower , (int)bAdd);
	int nTmp = (int)iPower;
	if (bAdd)
	{
		nTmp += power;
	}
	else
	{
		nTmp -= power;
	}
    
	if (nTmp < 0)
	{
		nTmp = 0;
	}

	//不再修改体力回复倒计时
	//if (iPower != nTmp)
	//{
	//	m_nUpdatePowerTs = GameManager::sharedGameManager()->getServerTime();
	//	postNotification(D_EVENT_RESET_SCHEDULE);
	//}

	iPower = nTmp;
    
	postNotification(D_EVENT_USERINFO_UPDATE, fDelayTime);
}

void UserInfo::setPower(int power, float fDelayTime)
{
	//不再修改体力回复倒计时
	//if (iPower != power)
	//{
	//	m_nUpdatePowerTs = GameManager::sharedGameManager()->getServerTime();
	//	postNotification(D_EVENT_RESET_SCHEDULE);
	//}
	//CCLOG("UserInfo::setPower power:%d , iPower:%d" , power , iPower);
	iPower = power;
	postNotification(D_EVENT_USERINFO_UPDATE, fDelayTime);
}

void UserInfo::getUpAward()
{
	UserUpAward * pAward = DM_GET_USER_MODEL->getUserUpAwardByLv(iUserLv);
	UserBase * pUserBase = DM_GET_USER_MODEL->getUserBaseByLv(iUserLv);
	
	if(pAward && pUserBase)
	{
		if (iPower < pUserBase->getMaxPower())
            iPower = pUserBase->getMaxPower();
		if (iEndurance < pUserBase->getMaxEndu())
			iEndurance = pUserBase->getMaxEndu();
		
		iPoints += pAward->getPoints();
		iCoins += pAward->getCoins();
	}
}

int UserInfo::getRushCD()
{
    return D_CONFIG_UINT_FOR_KEY(PVE_RUSH_CD_TIME) - MAX(0, TimeUtil::getDiff(lastRush, GameManager::sharedGameManager()->getServerTime()));
}

int UserInfo::getRushCDCost()
{
    return ceilf(getRushCD()/60.f);
}


uint32_t UserInfo::getPowerRecTime(bool bChange)
{
	if (!m_nUpdatePowerTs)
	{
		return 0;
	}
	UserBase *pBase = DM_GET_USER_MODEL->getUserBaseByLv(iUserLv);
	if (iPower >= pBase->getMaxPower())
	{
		return 0;
	}
	uint32_t nServerTime = GameManager::sharedGameManager()->getServerTime();
	uint32_t restTime = nServerTime - m_nUpdatePowerTs;
	if (nServerTime < m_nUpdatePowerTs)
	{
		return 0;
	}
	while (restTime > K_POWER_REFRESH_INTERVAL)
	{
		restTime -= K_POWER_REFRESH_INTERVAL;
	}
	//CCLOG("UserInfo::getPowerRecTime nServerTime:%d , m_nUpdatePowerTs:%d , restTime%d" , nServerTime , m_nUpdatePowerTs , restTime);
	if(K_POWER_REFRESH_INTERVAL - restTime == 0)
	{
		//计时相等 该增加体力了！
		if(bChange) changePower(1);
	}
	return K_POWER_REFRESH_INTERVAL - restTime;
}

uint32_t UserInfo::getPowerFullTime()
{
	uint32_t recTime = getPowerRecTime(false);
	UserBase *pBase = DM_GET_USER_MODEL->getUserBaseByLv(iUserLv);
	if (iPower >= pBase->getMaxPower())
	{
		return 0;
	}
	return K_POWER_REFRESH_INTERVAL * (pBase->getMaxPower() - iPower - 1) + recTime;
}




uint32_t UserInfo::getEnduRecTime(bool bChange)
{
	if (!m_nUpdateEnduTs)
	{
		return 0;
	}
	UserBase *pBase = DM_GET_USER_MODEL->getUserBaseByLv(iUserLv);
	if (iEndurance >= pBase->getMaxEndu())
	{
		return 0;
	}
	uint32_t nServerTime = GameManager::sharedGameManager()->getServerTime();
	uint32_t restTime = nServerTime - m_nUpdateEnduTs;
	if (nServerTime < m_nUpdateEnduTs)
	{
		return 0;
	}
	while (restTime > K_ENDURANCE_REFRESH_INTERVAL)
	{
		restTime -= K_ENDURANCE_REFRESH_INTERVAL;
	}
	if(K_ENDURANCE_REFRESH_INTERVAL - restTime == 0)
	{
		//计时相等 该增加体力了！
		if(bChange) changeEnduranceBy(1);
	}
	return K_ENDURANCE_REFRESH_INTERVAL - restTime;
}

uint32_t UserInfo::getEnduFullTime()
{
	uint32_t recTime = getEnduRecTime(false);
	UserBase *pBase = DM_GET_USER_MODEL->getUserBaseByLv(iUserLv);
	if (iEndurance >= pBase->getMaxEndu())
	{
		return 0;
	}
	return K_ENDURANCE_REFRESH_INTERVAL * (pBase->getMaxEndu() - iEndurance - 1) + recTime;
}

void UserInfo::setLocked(bool locked, float fDelay /* = 0.f */)
{
	m_bLocked = locked;
	if (!m_bLocked)
	{
		postNotification(D_EVENT_USERINFO_UPDATE, fDelay);
	}
	
}
