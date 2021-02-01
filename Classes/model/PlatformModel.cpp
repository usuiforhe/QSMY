//
//  PlatformModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#define SAVED_LOGINED_USER_COUNT 3

#include "PlatformModel.h"
#include "managers/GameManager.h"
#include "managers/PlatformManager.h"
#include "managers/AnalyticsManager.h"

USING_NS_CC;
using namespace platform;

PlatformModel::PlatformModel()
:m_pServerList(NULL)
,m_pMyServerList(NULL)
,m_pRechargeList(NULL)
,m_pRechargeInfo(NULL)
,m_mParameter()
,m_pDictForAnalytics(NULL)
,m_pDictForAds(NULL)
,isMyServerListSynced(false)
,m_pLoginedUsers(NULL)
{
    
}

PlatformModel::~PlatformModel()
{
    CC_SAFE_RELEASE(m_pMyServerList);
    CC_SAFE_RELEASE(m_pServerList);
    CC_SAFE_RELEASE(m_pRechargeList);
	CC_SAFE_RELEASE(m_pDictForAnalytics);
	CC_SAFE_RELEASE(m_pDictForAds);
	CC_SAFE_RELEASE(m_pLoginedUsers);
}

bool PlatformModel::init()
{
    m_pServerList = CCArray::create();
    CC_SAFE_RETAIN(m_pServerList);
    
    m_pMyServerList = CCArray::create();
    CC_SAFE_RETAIN(m_pMyServerList);
    
    m_pRechargeList = CCArray::create();
    CC_SAFE_RETAIN(m_pRechargeList);
    
	m_pDictForAnalytics = CCDictionary::create();
	CC_SAFE_RETAIN(m_pDictForAnalytics);

	m_pDictForAds = CCDictionary::create();
	CC_SAFE_RETAIN(m_pDictForAds);
    
	m_pLoginedUsers = CCArray::create();
	CC_SAFE_RETAIN(m_pLoginedUsers);

    loadData();
    return Model::init();
}

void PlatformModel::loadData()
{
    //parseServerList( CSVDecoder::create(SERVERS_CONFIG_PATH)->getData());
    parseRechargeList( CSVDecoder::create(RECHARGE_CONFIG_PATH)->getData());
    parseConfig( CSVDecoder::create(CONFIG_CONFIG_PATH)->getData());

	parseDeveloperInfo( CSVDecoder::create(PARAMETER_CONFIG_PATH)->getData(), m_mParameter);

	m_mParameter["cookie"] = COOKIE_PATH;

	loadAnalyticsData( CSVDecoder::create(ANALYTICS_CONFIG_PATH)->getData());
	loadAdsData( CSVDecoder::create(ADS_CONFIG_PATH)->getData());

	loadLoginedUsers(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_lOGINED_USERS));
}

static CCString* getValue(CCDictionary* map,const char* key)
{
    CCString* value = static_cast<CCString*>(map->objectForKey(key));
    if(value) return value;
    return CCString::create("");
}

void PlatformModel::parseConfig(const CSVDecoder::CSV &val)
{
    CCDictionary* pTempMap = CCDictionary::create();
    for (uint32_t i=0; i<val.size(); ++i)
	{
		pTempMap->setObject(ccs(val[i][1]), val[i][0]);
        //CCLOG("key:%s, value:%s", val[i][1].c_str(), val[i][0].c_str());
	}
    
    m_PlatformConfig.m_user             = getValue(pTempMap,"user")->getCString();
    m_PlatformConfig.m_iap              = getValue(pTempMap,"iap")->getCString();
    m_PlatformConfig.m_order            = getValue(pTempMap,"order")->getCString();
    m_PlatformConfig.m_platform         = getValue(pTempMap,"platformPath")->getCString();
    //m_PlatformConfig.m_uPlatformId      = getValue(pTempMap,"platformId")->intValue();
    m_PlatformConfig.m_fourmURL         = getValue(pTempMap,"fourmUrl")->getCString();
    m_PlatformConfig.m_serverURL        = getValue(pTempMap,"serverUrl")->getCString();
    m_PlatformConfig.m_announceURL      = getValue(pTempMap, "announceUrl")->getCString();
    m_PlatformConfig.m_checkVersionURL  = getValue(pTempMap,"checkVersionUrl")->getCString();
    if(CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename("test")))
    {
        //m_PlatformConfig.m_serverURL = "http://test.qsmobile.haohaowan.com/game/port";
        //m_PlatformConfig.m_loginSetTokken = CCString::createWithFormat("http://test.qsmobile.haohaowan.com/platform/port_public/%s/login_cb.php",m_PlatformConfig.m_platform.c_str())->getCString();
    }
    
    
    m_PlatformConfig.m_versionURL       = getValue(pTempMap,"versionUrl")->getCString();
    m_PlatformConfig.m_syzzb            = getValue(pTempMap,"syzzb")->getCString();
    m_PlatformConfig.m_ipaURL           = getValue(pTempMap,"ipaUrl")->getCString();
    m_PlatformConfig.m_channelId        = getValue(pTempMap,"channelId")->getCString();
    
   // m_PlatformConfig.m_maintainUrl      = CCString::createWithFormat(const char *format, ...);
   // m_PlatformConfig.m_loginSetTokken   = CCString::createWithFormat(val[7][1].c_str(),m_PlatformConfig.m_platform.c_str())->getCString();
    
    
    m_PlatformConfig.m_platformURL      = CCString::createWithFormat("%s/%s",m_PlatformConfig.m_serverURL.c_str(),m_PlatformConfig.m_platform.c_str())->getCString();
	m_PlatformConfig.m_bHideService = getValue(pTempMap, "hideService")->boolValue();

	m_PlatformConfig.m_iSettingConfig = getValue(pTempMap, "setting_config")->intValue();
	m_PlatformConfig.m_bStrategyHidden = getValue(pTempMap, "strategy_hidden")->boolValue();
    
	m_PlatformConfig.m_pAuthUrl = getValue(pTempMap, "authUrl")->getCString();
    
    CCLOG("----m_PlatformConfig.m_platformURL--%s", m_PlatformConfig.m_platformURL.c_str());
    CCLOG("----m_PlatformConfig.m_pAuthUrl--%s", m_PlatformConfig.m_pAuthUrl.c_str());

    m_PlatformConfig.m_sServerPlatformId = getValue(pTempMap, "serverPlatformId")->getCString();
	}

//void PlatformModel::parseServerList(const CSVDecoder::CSV &val)
//{
//    for (uint32_t i=0; i<val.size(); ++i)
//	{
//        ServerInfo* pItem= ServerInfo::create();
//       
//		pItem->m_nId = atoi(val[i][0].c_str());
//        pItem->m_Name = val[i][1];
//        pItem->m_Status = static_cast<ServerStatus>(atoi(val[i][2].c_str()));
//        m_pServerList->addObject(pItem);
//	}
//}

void PlatformModel::parseRechargeList(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
		RechargeInfo *pItem = RechargeInfo::create();
        pItem->m_sIdentifier = val[i][0];
        pItem->m_nPrice = val[i][1].c_str();
        pItem->m_nPoint = atoi(val[i][2].c_str());
        pItem->m_nExtRate = atof(val[i][3].c_str());
        pItem->m_nMonthCardId = atoi(val[i][4].c_str());
        pItem->m_nResId  = atoi(val[i][5].c_str());
		pItem->m_nShowExt  = atoi(val[i][6].c_str());

		pItem->m_nMCPointFirst  = atoi(val[i][9].c_str());
		pItem->m_nMCPoint  = atoi(val[i][10].c_str());
		pItem->m_nMCTimes  = atoi(val[i][11].c_str());
		pItem->m_nMCDay  = atoi(val[i][12].c_str());
		
		pItem->m_nShowPoint  = pItem->m_nPoint - pItem->m_nShowExt;
        
		if (val[i].size() > 7) {
			 pItem->m_des = val[i][7];
		}

        if (val[i].size() > 8) {
			pItem->m_nShowExtFirst  = atoi(val[i][8].c_str());
        }

        m_pRechargeList->addObject(pItem);
	}
}

void PlatformModel::parseDeveloperInfo(const CSVDecoder::CSV &val, DeveloperInfo &mmap)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		mmap[val[i][0]] = val[i][1];
	}
}


void PlatformModel::loadAnalyticsData(const CSVDecoder::CSV &val)
{
	m_pDictForAnalytics->removeAllObjects();
	for (uint32_t i=0; i<val.size(); ++i)
	{
		m_pDictForAnalytics->setObject(ccs(val[i][1]), val[i][0]);
	}
}

void PlatformModel::loadAdsData(const CSVDecoder::CSV &val)
{
	m_pDictForAds->removeAllObjects();
	for (uint32_t i = 0; i < val.size(); i++)
	{
		m_pDictForAds->setObject(ccs(val[i][1]), val[i][0]);
	}
	
}



platform::ServerInfo* PlatformModel::getServerInfoById(uint32_t id)
{
    ServerInfo* pInfo = NULL;
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pServerList, obj)
    {
        pInfo = static_cast<ServerInfo*>(obj);
        if(pInfo && pInfo->getID()==id) return pInfo;
    }
    return NULL;
}

void PlatformModel::setCurrentServer(platform::ServerInfo* pInfo)
{
    if(!PlatformManager::sharedPlatformManager()->isLogined())
    {
        return;
    }
    CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_SERVERID, pInfo->getID());
    CCUserDefault::sharedUserDefault()->flush();
    postNotification(D_EVENT_PLATFORM_SERVER_CHANGED);
}

platform::ServerInfo* PlatformModel::getCurrentServer()
{
    if(m_pServerList->count() <=0) this->syncServerList();
    CCLOG("getcurrentserver---enter");
    if(!PlatformManager::sharedPlatformManager()->isLogined())
    {
        CCLOG("getcurrentserver---not login");
        if(m_pServerList->count()<=0) return NULL;
        return static_cast<ServerInfo*>(m_pServerList->objectAtIndex(0));
    }
    CCLOG("getcurrentserver---1");
    int id = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_SERVERID,-1);
    CCLOG("getcurrentserver---2--%d", id);
    CCLOG("getcurrentserver---3--%d", m_pServerList->count());
    if(id>=0)
    {
        platform::ServerInfo* pItem = getServerInfoById(id);
        if(pItem){
            
            CCLOG("getcurrentserver---4--%d,%d,%s,%d ", pItem->m_nId, pItem->m_nSId, pItem->m_Name.c_str(), pItem->m_Status);
            return pItem;
        }
    }
    
    if(m_pServerList->count()<=0) return NULL;
    return static_cast<ServerInfo*>(m_pServerList->objectAtIndex(0));
}

void PlatformModel::getChargeOrder()
{
    if(m_pRechargeInfo==NULL) return;
    if(!m_PlatformConfig.getOrderMethod().empty())
    {
        HTTP_CLIENT->getChargeOrder(m_PlatformConfig.getOrderMethod().c_str(),m_pRechargeInfo->getMonthCardId(),m_pRechargeInfo->getPrice().c_str(),this,callfuncJson_selector(PlatformModel::onGetChargeOrder));
        return;
    }
    buy(0,"");
}


void PlatformModel::onGetChargeOrder(const Json::Value &requestData, const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	buy(responseData["data"]["billno"].asUInt(),responseData["data"]["token"].asString());
}

void PlatformModel::buy(uint32_t billno,const std::string &token)
{
    if(m_pRechargeInfo==NULL)
    {
        Tips_Alert("error");
    }

	AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
	pAnalytics->chargeRequst(CCString::createWithFormat("%u", billno)->getCString(),
		"",
		atof(m_pRechargeInfo->getPrice().c_str()),
		"CNY",
		m_pRechargeInfo->getPoint(),
		getConfig().getPlatform().c_str());

    PlatformManager::sharedPlatformManager()->buy(m_pRechargeInfo,billno,token);
}

void PlatformModel::saveMyCurrentServer()
{
    if(!isMyServerListSynced) //如果没有同步当前服务器列表则注册当前服
    {
        HTTP_CLIENT->registerToPlat(this,callfuncJson_selector(PlatformModel::onSaveMyCurrentServer),false,false);
        return;
    }
    platform::ServerInfo* pItem = getCurrentServer();
    if(pItem == NULL) return;
    if(m_pMyServerList->indexOfObject(pItem)!=CC_INVALID_INDEX) return;
    HTTP_CLIENT->registerToPlat(this,callfuncJson_selector(PlatformModel::onSaveMyCurrentServer),false,false);
}

void PlatformModel::onSaveMyCurrentServer(const Json::Value &requestData,const Json::Value &responseData)
{
    platform::ServerInfo* pItem = getCurrentServer();
    if(pItem == NULL) return;
    m_pMyServerList->addObject(pItem);
}

void PlatformModel::syncMyServerList()
{
    if(isMyServerListSynced) return;
    HTTP_CLIENT->getMyServerList(PlatformManager::sharedPlatformManager()->getServersParam().c_str(),this, callfuncJson_selector(PlatformModel::parseMyServerList));
}

void PlatformModel::parseMyServerList(const Json::Value &requestData,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    for (uint32_t i=0; i<responseData["data"]["servers"].size(); ++i)
	{
        int sId = responseData["data"]["servers"][i].asUInt();
        platform::ServerInfo* pItem = getServerInfoById(sId);
        if(pItem) m_pMyServerList->addObject(pItem);
	}
    isMyServerListSynced = true;
    postNotification(D_EVENT_PLATFORM_ON_GET_MY_SERVER_LIST);
}

void PlatformModel::syncServerList()
{
    if(isSynced) return;
    HTTP_CLIENT->getServerList(this, callfuncJson_selector(PlatformModel::parseServerList));
}


void PlatformModel::parseServerList(const Json::Value &requestData,const Json::Value &responseData)
{
    CCLOG("serverlist---");
    for (uint32_t i=0; i<responseData["data"].size(); ++i)
	{
        
        ServerInfo* pItem= ServerInfo::create();
        
        pItem->m_nId = responseData["data"][i]["id"].asUInt();
		pItem->m_nSId = responseData["data"][i]["sid"].asUInt();
        pItem->m_Name = responseData["data"][i]["name"].asCString();
        pItem->m_Status = static_cast<ServerStatus>(responseData["data"][i]["s"].asUInt());
        CCLOG("serverlist---%d,%d,%s,%d ", pItem->m_nId, pItem->m_nSId, pItem->m_Name.c_str(), pItem->m_Status);
        m_pServerList->addObject(pItem);
	}
    //if(m_pServerList->count() <=0) this->syncServerList();
    isSynced = true;
    postNotification(D_EVENT_PLATFORM_ON_GET_SERVER_LIST);
}

std::string PlatformModel::getServerURL()
{
    CCLOG("getServerURL---");
    return CCString::createWithFormat(m_PlatformConfig.m_serverURL.c_str(),getCurrentServer()->getID())->getCString();
}

std::string PlatformModel::getAnnounceURL()
{
    return m_PlatformConfig.m_announceURL;
}


std::string PlatformModel::getPlatformURL()
{
    return CCString::createWithFormat(m_PlatformConfig.m_platformURL.c_str(),getCurrentServer()->getID())->getCString();
}

std::string PlatformModel::getAuthURL()
{
	return CCString::createWithFormat(m_PlatformConfig.m_pAuthUrl.c_str())->getCString();
}

std::string PlatformModel::getMaintainURL()
{
    return CCString::createWithFormat("%s/%s",getPlatformURL().c_str(),API_MAINT)->getCString();
}

bool PlatformModel::getSettingVisibleByIndex(int idx)
{
	return !(1 << idx & m_PlatformConfig.m_iSettingConfig);
}



void PlatformModel::loadLoginedUsers(const std::string usersStr)
{
	Json::Reader reader;
	Json::Value users;
	reader.parse(usersStr, users);
	uint32_t nSize = users["users"].size();
	Json::Value userVal; 
	for (uint32_t i=0; i<nSize; ++i)
	{
		userVal = users["users"][i];
		UserLoginInfo* pUser = UserLoginInfo::create();
		DJSON_INT(pUser->m_nLoginType, userVal, "lt");
		DJSON_CSTRING(pUser->m_pLoginName, userVal, "u");
		DJSON_CSTRING(pUser->m_pPassword, userVal, "p");
		DJSON_CSTRING(pUser->m_pEmail, userVal, "e");
		DJSON_CSTRING(pUser->m_facebookName, userVal, "fn");
		DJSON_CSTRING(pUser->m_sUniqueId, userVal, "unid");
		DJSON_CSTRING(pUser->m_sPlatId, userVal, "pid");
		m_pLoginedUsers->addObject(pUser);
	}
}

UserLoginInfo* PlatformModel::getLoginedUserByName(const char* platId, int iLoginType)
{
	if(!platId) return NULL;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(m_pLoginedUsers, pObj)
	{
		UserLoginInfo* pUser = (UserLoginInfo*)pObj; 
		if(strcmp(pUser->getPlatId().c_str(), platId) == 0 && iLoginType == pUser->getLogType())
		{
			return pUser;
		}
	}
	return NULL;
}

void PlatformModel::saveLoginedUser(const char* userName, const char* password, int iLoginType, const char* email, const char* facebookUserName/* = NULL */, const char* uniqueId/* = NULL*/, const char* platId/*=NULL*/)
{
	if(platId && getLoginedUserByName(platId, iLoginType) != NULL) return ;
	
	UserLoginInfo* pUser = new UserLoginInfo();
	pUser->m_nLoginType = iLoginType;
	pUser->m_pLoginName = userName;
	pUser->m_pPassword = password;
	if(email)
	{
		pUser->m_pEmail = email;
	} else 
	{
		pUser->m_pEmail = "";
	}
    
    if (facebookUserName) {
        pUser->m_facebookName = facebookUserName;
    }
    else
    {
        pUser->m_facebookName = "";
    }

	if(uniqueId){
		pUser->m_sUniqueId = uniqueId;
	} else {
		pUser->m_sUniqueId = "";
	}

	if(platId){
		pUser->m_sPlatId = platId;
	} else {
		pUser->m_sPlatId = "";
	}
	
	m_pLoginedUsers->insertObject(pUser, 0);

	if(m_pLoginedUsers->data->num > SAVED_LOGINED_USER_COUNT)
	{
		m_pLoginedUsers->removeLastObject();
	}

	saveToClient();
}

void PlatformModel::removeLoginedUser(platform::UserLoginInfo* pReUser)
{
	removeLoginedUser(pReUser->getLoginName().c_str(), pReUser->getLogType());
}


void PlatformModel::removeLoginedUser(const char* userName, int iLoginType, bool triggerNofi/*=true*/)
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(m_pLoginedUsers, pObj)
	{
		UserLoginInfo* pUser = (UserLoginInfo*)pObj; 
		if(strcmp(pUser->getLoginName().c_str(), userName) == 0 && iLoginType == pUser->getLogType())
		{
			m_pLoginedUsers->removeObject(pUser);
		}
	}

	std::string pDeUserName = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME,"");
	int iLoginTypeOld = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE, kLogin);
	//清除缓存的登录用户密码
	if(strcmp(userName, pDeUserName.c_str()) == 0 &&  iLoginType == iLoginTypeOld)
	{
		UserLoginInfo* pUser = NULL;
		if(DM_GET_PLATFORM_MODEL->getLoginedUsers()->data->num > 0)
		{
			pUser = (UserLoginInfo*)DM_GET_PLATFORM_MODEL->getLoginedUsers()->objectAtIndex(0);
		} else 
		{
			pUser = UserLoginInfo::create();
		}
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, pUser->getLoginName());
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, pUser->getPassword());
		CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE,pUser->getLogType());
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_PLATID, pUser->getPlatId());
	}
	saveToClient();

	if(triggerNofi) postNotification(D_EVENT_LOGINED_USER_CHANGED);
}

void PlatformModel::saveToClient()
{
	Json::Value users(Json::arrayValue);
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(m_pLoginedUsers, pObj)
	{
		UserLoginInfo* pUser = (UserLoginInfo*)pObj; 
		Json::Value val(Json::objectValue);
		val["u"]=pUser->getLoginName();
		val["p"]=pUser->getPassword();
		val["e"]=pUser->getEmail();
		val["lt"]=pUser->getLogType();
		val["fn"]=pUser->getFacebookName();
		val["unid"]=pUser->getUniqueId();
		val["pid"]=pUser->getPlatId();
		users.append(val);
	}

	json_serializer s;
	s<<"users"<<users;
	CCUserDefault::sharedUserDefault()->setStringForKey(KEY_lOGINED_USERS, s.toString());
	CCLOG("all logined users: %s", s.toString().c_str());
	CCUserDefault::sharedUserDefault()->flush();
}