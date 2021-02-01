//
//  KingTombModel.cpp
//  LibQSMY
//  皇陵model
//  Created by thedream on 14-8-1.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "KingTombModel.h"
#include "managers/DataManager.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
using namespace kingTomb;

USING_NS_CC;

#define SPEED_TIME  3600.f


namespace kingTomb
{
	kingTombRankInfo::kingTombRankInfo()
		:m_name("")
		,m_level(0)
		,m_fight(0)
		,m_count(0)
	{
	}
	kingTombRankInfo::~kingTombRankInfo()
	{
	}
	void kingTombRankInfo::parse(const Json::Value& val)
	{
		DJSON_STRING(m_name, val, "name");
		std::string str = "";
		DJSON_STRING(str, val, "lv");
		m_level = atoi(str.c_str());
		DJSON_STRING(str, val, "fight");
		m_fight = atoi(str.c_str());
		DJSON_STRING(str, val, "occupyNum");
		m_count = atoi(str.c_str());

		//DJSON_UINT(m_level, val, "lv");
		//DJSON_UINT(m_fight, val, "fight");
		//DJSON_UINT(m_count, val, "occupyNum");
	}



    kingTombItemInfo::kingTombItemInfo()
    :m_id(0)
    ,m_Itemid(0)
    ,m_Itemtype(0)
    ,m_itemNumByLv(NULL)
    {
        
    }
    bool kingTombItemInfo::init(){
        if( !ValueObjcet::init() )
		{
			return false;
		}
        m_itemNumByLv = CCDictionary::create();
        CC_SAFE_RETAIN(m_itemNumByLv);
		return true;
    }
    
    kingTombItemInfo::~kingTombItemInfo(){
        CC_SAFE_RELEASE(m_itemNumByLv);
    }
    
    kingTombVipInfo::kingTombVipInfo()
    :m_vip(0)
    ,m_tombId(0){
        
    }
    
    bool kingTombVipInfo::init(){
        if( !ValueObjcet::init() )
		{
			return false;
		}
		return true;
    }
    
    kingTombVipInfo::~kingTombVipInfo(){
        
    }
    
    kingTombTypeInfo::kingTombTypeInfo()
    :m_type(0)
    ,m_name("")
    ,m_fightNum(0)
    {
        
    }
    
    bool kingTombTypeInfo::init(){
        if(!ValueObjcet::init())
		{
			return false;
		}
		return true;
    }
    
    kingTombTypeInfo::~kingTombTypeInfo(){
        
    }
    
    userData::userData():m_name("")
    ,m_sid(0)
    ,m_lv(0)
    ,m_die(false)
    ,m_id(0)
    {
    }
    
    userData::~userData(){
    }
    
    void userData::parse(const Json::Value& val){
        DJSON_STRING(m_name, val, "name");
        DJSON_UINT(m_sid, val, "sid");
        DJSON_UINT(m_lv, val, "lv");
        DJSON_BOOL(m_die,val,"die");
        DJSON_UINT(m_id, val, "uid");
    }
    
    
    
    
    kingTombBaseInfo::kingTombBaseInfo()
    :m_type(0)
    ,m_coinOutput(0)
    ,m_occupyTs(0)
    ,m_extraOutputType(0)
    ,m_speedUpTs(0)
    {}
    
    kingTombBaseInfo::~kingTombBaseInfo(){}
    
    void kingTombBaseInfo::parse(const Json::Value &val){
        DJSON_UINT(m_type, val, "treasureType");
        DJSON_UINT(m_occupyTs, val, "occupyTs");
        DJSON_DOUBLE(m_coinOutput,val,"coinOutput");
        DJSON_UINT(m_extraOutputType, val, "extraOutputType");
        DJSON_UINT(m_speedUpTs, val, "speedUpTs");
        uint32_t time = GameManager::sharedGameManager()->getServerTime();
        if(m_speedUpTs>time)
            m_speedUpTs = time;
    }
    
    uint32_t kingTombBaseInfo::getOutputNum(){
        uint32_t time = GameManager::sharedGameManager()->getServerTime();
        if(m_endTs==0)
            m_endTs = time;
        if(time>m_endTs)
            time = m_endTs;
        uint32_t output = 0 ;
        //如果开始运行时间 大于 系统时间 大于结束时间就是错误的 返回0
        if(m_occupyTs>time){
            return 0;
        }
        if(m_speedUpTs!=0){
            output = (m_speedUpTs- m_occupyTs)*m_coinOutput + (time-m_speedUpTs)*m_coinOutput *2;
        }else{
            output = (time-m_occupyTs)*m_coinOutput;
        }
        return output;
    }
    
    uint32_t kingTombBaseInfo::getSpeedNum(){
        return ceil(m_coinOutput*SPEED_TIME);
    }
    
    uint32_t kingTombBaseInfo::getOutputItemNum(){
        uint32_t time = GameManager::sharedGameManager()->getServerTime();
        if(m_endTs==0)
            m_endTs = time;
        kingTomb::kingTombItemInfo *itemInfo = DM_GET_KINGTOMB_MODEL->getTombItemInfo(m_extraOutputType);
        if(!itemInfo)
            return 0;
        CCInteger *num = (CCInteger*)itemInfo->getItemNumByLv()->objectForKey(getType());
        int speed = num->getValue();
        if(time>m_endTs)
            time = m_endTs;
        //如果开始运行时间 大于 系统时间 大于结束时间就是错误的 返回0
        if(m_occupyTs>time){
            return 0;
        }
        uint32_t leftHour ;
        if(m_speedUpTs==0){
            leftHour =  time - m_occupyTs;
        }else{
            leftHour =  (m_speedUpTs- m_occupyTs) +  (time-m_speedUpTs) *2;
        }
        return (uint32_t)floor(leftHour/SPEED_TIME) * speed;
    }
    
    
    kingTombInfo::kingTombInfo()
    :m_id(0)
    ,m_searchTs(0)
    ,m_endTs(0)
    ,m_isRob(false)
    ,m_occupyId(0)
    ,m_isFight(false)
    ,m_defendUserData(NULL)
    ,m_attackUserData(NULL)
    ,m_pMyInfo(NULL)
    ,m_pOtherInfo(NULL)
    ,m_bonus(NULL)
    {
        
    }
    kingTombInfo::~kingTombInfo(){
        CC_SAFE_RELEASE(m_defendUserData);
        CC_SAFE_RELEASE(m_attackUserData);
        CC_SAFE_RELEASE(m_pMyInfo);
        CC_SAFE_RELEASE(m_pOtherInfo);
        CC_SAFE_RELEASE(m_bonus);
    }
    
    
    
    
	void kingTombInfo::parse(const Json::Value &val){
        DJSON_UINT(m_id, val, "treasureId");
        
        DJSON_UINT(m_searchTs, val, "searchTs");
        DJSON_UINT(m_endTs, val, "endTs");
        DJSON_BOOL(m_isRob, val, "isRob");
        DJSON_UINT(m_occupyId, val, "occupyId");
        
        if(m_pMyInfo==NULL){
            m_pMyInfo = kingTombBaseInfo::create();
            CC_SAFE_RETAIN(m_pMyInfo);
        }
        m_pMyInfo->setEndTs(m_endTs);
        m_pMyInfo->parse(val);
        
        if(m_pOtherInfo==NULL){
            m_pOtherInfo = kingTombBaseInfo::create();
            CC_SAFE_RETAIN(m_pOtherInfo);
        }
        
        m_pOtherInfo->setEndTs(m_endTs);
        m_pOtherInfo->parse(val["occupyInfo"]);
        
        if(m_bonus==NULL){
            m_bonus = CCArray::create();
            CC_SAFE_RETAIN(m_bonus);
        }else{
            m_bonus->removeAllObjects();
        }
        if (val.isMember("bonus"))
		{
            
			const Json::Value& bonusInfo = val["bonus"];
			for (uint32_t i = 0; i < bonusInfo.size(); i++)
			{
				drop::Drop* pDrop = DM_GET_DROP_MODEL->parseDrop(bonusInfo[i]);
				if (pDrop)
				{
					m_bonus->addObject(pDrop);
				}
			}
		}
        
        
        if(m_attackUserData == NULL){
            m_attackUserData = CCArray::create();
            CC_SAFE_RETAIN(m_attackUserData);
        }else{
            m_attackUserData->removeAllObjects();
        }
        if(m_defendUserData == NULL){
            m_defendUserData = CCArray::create();
            CC_SAFE_RETAIN(m_defendUserData);
        }else{
            m_defendUserData->removeAllObjects();
        }
        
        m_isFight = false;
        
        for (Json::ValueIterator i = val["defendUserData"].begin(); i != val["defendUserData"].end(); i++)
        {
//            Json::Value v = *i;
            userData *pInfo = userData::create();
            pInfo->parse(*i);
            if(pInfo->getDie())
                m_isFight = true;
            m_defendUserData->addObject(pInfo);
        }
        for (Json::ValueIterator i = val["attackUserData"].begin(); i != val["attackUserData"].end(); i++)
        {
//            Json::Value v = *i;
            userData *pInfo = userData::create();
            pInfo->parse(*i);
            if(pInfo->getDie())
                m_isFight = true;
            m_attackUserData->addObject(pInfo);
        }
    }
    
    bool kingTombInfo::isTimeLimit(){
        uint32_t time = GameManager::sharedGameManager()->getServerTime();
        if(m_pMyInfo->getOccupyTs()>0)
            return false;
        else if(m_searchTs>time){
            return false;
        }
        else if((time-m_searchTs)>D_CONFIG_UINT_FOR_KEY(TOMBS_TIMELIMIT)){
            return true;
        }
        return false;
    }
    
    kingTombBaseInfo * kingTombInfo::getMyInfo(){
        return m_pMyInfo;
    }
    kingTombBaseInfo * kingTombInfo::getOtherInfo(){
        return m_pOtherInfo;
    }
    
    bool kingTombInfo::theKingTombIsUser(){
        return getOccupyId()!=0;
    }
    
    bool kingTombInfo::theKingTombIsWork(){
        return m_pMyInfo->getOccupyTs()>0;
    }
    
    
    uint32_t kingTombInfo::getState(){
        if(m_isRob){
            return kKingTombStateIsRob;
        }
        uint32_t uNow = GameManager::sharedGameManager()->getServerTime();
        if(m_endTs!=0  && m_endTs<=uNow){
            return kKingTombStateIsComplete;
        }
        if(m_pMyInfo->getOccupyTs()>0){
            return kKingTombStateIsWork;
        }else{
            return kKingTombStateIsReady;
        }
        return kKingTombStateNone;
    }
    
    
    
    
    
    uint32_t kingTombInfo::getSpeedUpYuan(){
        unsigned int time = GameManager::sharedGameManager()->getServerTime();
        return ceil((m_endTs-time)/SPEED_TIME)*D_CONFIG_UINT_FOR_KEY(TOMBS_SPEED_YUAN);
    }
    
    
    
    
    
	AssistBonusInfo::AssistBonusInfo()
    : assistLv(0)
    , assistName("")
    , treasureType(0)
    , addTs(0)
    , id(0)
    , bonusData(NULL)
	{
        
	}
    
	AssistBonusInfo::~AssistBonusInfo()
	{
		CC_SAFE_RELEASE(bonusData);
	}
    
	void AssistBonusInfo::parse(const Json::Value& val)
	{
		DJSON_UINT(assistLv, val, "assistLv");
		DJSON_STRING(assistName, val, "assistName");
		DJSON_UINT(treasureType, val, "treasureType");
		DJSON_UINT(addTs, val, "addTs");
		DJSON_UINT(id, val, "id");
        
		// 助战奖励物品信息
		if (val.isMember("bonusData"))
		{
			CC_SAFE_RELEASE(bonusData);
			bonusData = cocos2d::CCArray::create();
			CC_SAFE_RETAIN(bonusData);
            
			const Json::Value& bonusInfo = val["bonusData"];
			for (unsigned int i = 0; i < bonusInfo.size(); i++)
			{
				drop::Drop* pDrop = DM_GET_DROP_MODEL->parseDrop(bonusInfo[i]);
				if (pDrop)
				{
					bonusData->addObject(pDrop);
				}
			}
		}
	}
    
    
	LogInfo::LogInfo()
    : treasureType(0)
    , name("")
    , lv(0)
    , bonus(NULL)
    , lose(NULL)
    , addTs(0)
	{
        
	}
    
	LogInfo::~LogInfo()
	{
		CC_SAFE_RELEASE(bonus);
		CC_SAFE_RELEASE(lose);
	}
    
	void LogInfo::parse(const Json::Value& val)
	{
		DJSON_UINT(treasureType, val, "treasureType");
		DJSON_STRING(name, val, "name");
		DJSON_UINT(lv, val, "lv");
		DJSON_UINT(addTs, val, "addTs");
        
        if(bonus ==NULL){
            bonus = cocos2d::CCArray::create();
            CC_SAFE_RETAIN(bonus);
        }else{
            bonus->removeAllObjects();
        }
		// 获得、失去
		if (val.isMember("bonus"))
		{
			const Json::Value& bonusInfo = val["bonus"];
			for (unsigned int i = 0; i < bonusInfo.size(); i++)
			{
				drop::Drop* pDrop = DM_GET_DROP_MODEL->parseDrop(bonusInfo[i]);
				if (pDrop)
				{
					bonus->addObject(pDrop);
				}
			}
		}
        
        
        if(lose==NULL){
            lose = cocos2d::CCArray::create();
            CC_SAFE_RETAIN(lose);
        }else{
            lose->removeAllObjects();
        }
        
		if (val.isMember("lose"))
		{
			const Json::Value& loseInfo = val["lose"];
			for (unsigned int i = 0; i < loseInfo.size(); i++)
			{
				drop::Drop* pDrop = DM_GET_DROP_MODEL->parseDrop(loseInfo[i]);
				if (pDrop)
				{
					lose->addObject(pDrop);
				}
			}
		}
	}
    
}


KingTombModel::KingTombModel()
:m_pOccupyList(NULL)
,m_pVipInfos(NULL)
,m_pSeleteUser(NULL)
,m_pTypeInfos(NULL)
,m_pAllAssistUserIds(NULL)
,m_pAssistBonusList(NULL)
,m_pLogList(NULL)
,m_nLastAssistBonusListTime(0)
,m_nLastKingTomeListTime(0)
,m_nLastLogTime(0)
,m_pKingTombItems(NULL)
,m_bHaveNewAssistBonus(false)
,m_bHaveNewLog(false)
,m_dragonWin(false)
,m_dragonEnd(false)
,m_pStopAin(false)
,m_pGuildRankList(NULL)
,m_pUserRankList(NULL)
,m_uUserRankTs(0)
,m_uGuildRankTs(0)
,m_uTodayGuildName("")
,m_uTodayUserName("")
,m_uTodayType(0)
,m_bTodayFound(false)
{
    
}

KingTombModel::~KingTombModel()
{
    CC_SAFE_RELEASE(m_pOccupyList);
    CC_SAFE_RELEASE(m_pVipInfos);
    CC_SAFE_RELEASE(m_pSeleteUser);
    CC_SAFE_RELEASE(m_pTypeInfos);
    CC_SAFE_RELEASE(m_pAllAssistUserIds);
	CC_SAFE_RELEASE(m_pAssistBonusList);
	CC_SAFE_RELEASE(m_pLogList);
    CC_SAFE_RELEASE(m_pKingTombItems);
	CC_SAFE_RELEASE(m_pGuildRankList);
	CC_SAFE_RELEASE(m_pUserRankList);
}

bool KingTombModel::init()
{
    m_pOccupyList = CCArray::create();
    CC_SAFE_RETAIN(m_pOccupyList);
    
    m_pVipInfos = CCDictionary::create();
    CC_SAFE_RETAIN(m_pVipInfos);
    
    m_pSeleteUser = CCDictionary::create();
    CC_SAFE_RETAIN(m_pSeleteUser);
    
    m_pKingTombItems = CCDictionary::create();
    CC_SAFE_RETAIN(m_pKingTombItems);
    
	m_pAssistBonusList = CCArray::create();
	CC_SAFE_RETAIN(m_pAssistBonusList);
    
	m_pLogList = CCArray::create();
	CC_SAFE_RETAIN(m_pLogList);

	m_pGuildRankList = CCArray::create();
	CC_SAFE_RETAIN(m_pGuildRankList);
    
	m_pUserRankList = CCArray::create();
	CC_SAFE_RETAIN(m_pUserRankList);
    
    m_pTypeInfos = CCDictionary::create();
    CC_SAFE_RETAIN(m_pTypeInfos);
    
    m_pAllAssistUserIds = CCDictionary::create();
    CC_SAFE_RETAIN(m_pAllAssistUserIds);
    
    
    
    parseKingTombVipInfo(CSVDecoder::create("data/KingTomb/vip_KingTomb")->getData());
    parseKingTombTypeInfo(CSVDecoder::create("data/KingTomb/KingTombType")->getData());
    parseKingTombItemInfo(CSVDecoder::create("data/KingTomb/KingTombItem")->getData());
    
    
	return Model::init();
}

void KingTombModel::parseKingTombVipInfo(const CSVDecoder::CSV &val){
    for (uint32_t i = 0; i < val.size(); ++i){
        kingTombVipInfo *vipInfo = kingTombVipInfo::create();
        vipInfo->setTombId(atoi(val[i][0].c_str()));
        vipInfo->setVip(atoi(val[i][1].c_str()));
        m_pVipInfos->setObject(vipInfo,vipInfo->getTombId());
    }
}
void KingTombModel::parseKingTombTypeInfo(const CSVDecoder::CSV &val){
    for (uint32_t i = 0; i < val.size(); ++i){
        kingTombTypeInfo *typeInfo = kingTombTypeInfo::create();
        typeInfo->setType(atoi(val[i][0].c_str()));
        typeInfo->setName(val[i][1].c_str());
        typeInfo->setFightNum(atoi(val[i][2].c_str()));
        m_pTypeInfos->setObject(typeInfo,typeInfo->getType());
    }
}
void KingTombModel::parseKingTombItemInfo(const CSVDecoder::CSV &val){
    for (uint32_t i = 0; i < val.size(); ++i){
        kingTombItemInfo *itemInfo = kingTombItemInfo::create();
        itemInfo->setID(atoi(val[i][0].c_str()));
        itemInfo->setItemid(atoi(val[i][1].c_str()));
        itemInfo->getItemNumByLv()->setObject(CCInteger::create(atoi(val[i][2].c_str())),1);
        itemInfo->getItemNumByLv()->setObject(CCInteger::create(atoi(val[i][3].c_str())),2);
        itemInfo->getItemNumByLv()->setObject(CCInteger::create(atoi(val[i][4].c_str())),3);
        itemInfo->getItemNumByLv()->setObject(CCInteger::create(atoi(val[i][5].c_str())),4);
        itemInfo->getItemNumByLv()->setObject(CCInteger::create(atoi(val[i][6].c_str())),5);
		itemInfo->getItemNumByLv()->setObject(CCInteger::create(atoi(val[i][7].c_str())),6);
		itemInfo->getItemNumByLv()->setObject(CCInteger::create(atoi(val[i][8].c_str())),7);
        itemInfo->setItemtype(atoi(val[i][9].c_str()));
        m_pKingTombItems->setObject(itemInfo,itemInfo->getID());
    }
}


kingTombVipInfo * KingTombModel::getTombVipInfo(uint32_t pos)
{
    return (kingTombVipInfo*)m_pVipInfos->objectForKey(pos);
}


bool KingTombModel::isOpenTombByPos(int pos){
    kingTomb::kingTombVipInfo *vipInfo=  getTombVipInfo(pos);
    return  DM_GET_USER_MODEL->getUserInfo()->getVipLevel()>=vipInfo->getVip();
}


kingTombTypeInfo * KingTombModel::getTombTypeInfo(uint32_t type)
{
    return (kingTombTypeInfo*)m_pTypeInfos->objectForKey(type);
}


kingTomb::kingTombItemInfo * KingTombModel::getTombItemInfo(uint32_t m_id){
    return (kingTombItemInfo*)m_pKingTombItems->objectForKey(m_id);
}


//已经助战过的人的列表
void KingTombModel::parseAssistUserIds(const Json::Value& val){
    m_pAllAssistUserIds->removeAllObjects();
    for (uint32_t i=0; i<val.size(); ++i)
    {
        m_pAllAssistUserIds->setObject(CCInteger::create(val[i].asUInt()),val[i].asUInt());
    }
}

void KingTombModel::addAssistUserId(){
    for(int i = 1; i< FIGHT_PLAY_MAX_NUM;i++){
        guild::GuildUserBasics* guildUser= getSeleteUseByPos(i);
        if(guildUser)
            m_pAllAssistUserIds->setObject(CCInteger::create(guildUser->getUserId()), guildUser->getUserId());
    }
}

void KingTombModel::removeAllAssistUserId(){
    
}


void KingTombModel::parseOccupyList(const Json::Value& val){
    m_nLastKingTomeListTime = GameManager::sharedGameManager()->getServerTime();
    m_pOccupyList->removeAllObjects();
    for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		kingTombInfo *pInfo = kingTombInfo::create();
		pInfo->parse(*i);
		m_pOccupyList->addObject(pInfo);
	}
    
	postNotification(D_EVENT_KINGTOMBLIST_GET);
}



void KingTombModel::addSeleteUse(guild::GuildUserBasics *pdata,uint32_t pos){
    m_pSeleteUser->setObject(pdata,pos);
}

void KingTombModel::removeSeleteUse(){
    m_pSeleteUser->removeAllObjects();
}

guild::GuildUserBasics* KingTombModel::getSeleteUseByPos(uint32_t pos){
    return (guild::GuildUserBasics*)m_pSeleteUser->objectForKey(pos);
}



kingTombInfo* KingTombModel::getKingTombInfoByPos(unsigned int pos){
    CCObject *pObj = NULL;
    CCARRAY_FOREACH(m_pOccupyList, pObj)
    {
        kingTombInfo *info = static_cast<kingTombInfo *>(pObj);
        if (info->m_id == pos)
        {
            return info;
        }
    }
    return NULL;
}

void KingTombModel::changeKingTombInfo(const Json::Value& val,bool delay){
    kingTombInfo *pInfo = kingTombInfo::create();
    pInfo->parse(val);
    kingTombInfo *pInfoOld = getKingTombInfoByPos(pInfo->getId());
    if(pInfoOld){
        //如果战斗是失败的要把 可以选择的人列表中 去掉这个矿 助战的人员
        if(m_dragonEnd && !m_dragonWin){
            removemAssistUser(pInfoOld);
        }
        m_pOccupyList->removeObject(pInfoOld);
    }
    m_pOccupyList->addObject(pInfo);
    if(!delay)
        postNotification(D_EVENT_KINGTOMBLIST_GET);
}

void KingTombModel::removemAssistUser(kingTombInfo *pInfo ){
    for(int i =0 ;i<pInfo->getattackUserData()->count();i++){
        kingTomb::userData *data = static_cast< kingTomb::userData *>(pInfo->getattackUserData()->objectAtIndex(i));
        if(DM_GET_USER_MODEL->getUserInfo()->getUserId()!=data->getID())
            m_pAllAssistUserIds->removeObjectForKey(data->getID());
    }
}


void KingTombModel::removeKingTombInfoByPos(unsigned int pos , bool backUser){
    CCObject *pObj = NULL;
    CCARRAY_FOREACH(m_pOccupyList, pObj)
    {
        kingTombInfo *info = static_cast<kingTombInfo *>(pObj);
        if (info->m_id == pos)
        {
            if(backUser){
                removemAssistUser(info);
            }
            m_pOccupyList->removeObject(info);
            postNotification(D_EVENT_KINGTOMBLIST_GET);
        }
    }
}

bool KingTombModel::isAssistUser(uint32_t uid){
    return m_pAllAssistUserIds->objectForKey(uid)!=NULL;
}

bool KingTombModel::isSeleteUser(uint32_t uid){
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_pSeleteUser, pElement)
    {
        uint32_t key = pElement->getIntKey();
        guild::GuildUserBasics* user =  (guild::GuildUserBasics*)m_pSeleteUser->objectForKey(key);
        if(uid==user->getUserId()){
            return true;
        }
    }
    return false;
}

unsigned int KingTombModel::getFreeFindNum(){
    unsigned int findNum = D_CONFIG_UINT_FOR_KEY(TOMBS_FREE_FIND_NUM)>m_pSearchTimes ? D_CONFIG_UINT_FOR_KEY(TOMBS_FREE_FIND_NUM) - m_pSearchTimes:0;
    return findNum;
}

unsigned int KingTombModel::getFindYuan(){
    unsigned int yuan =  (m_pSearchTimes - D_CONFIG_UINT_FOR_KEY(TOMBS_FREE_FIND_NUM) + 1) * D_CONFIG_UINT_FOR_KEY(TOMBS_FIND_YUAN);
    yuan = yuan>D_CONFIG_UINT_FOR_KEY(TOMBS_MAX_FIND_YUAN)?D_CONFIG_UINT_FOR_KEY(TOMBS_MAX_FIND_YUAN):yuan;
    return yuan;
}



void KingTombModel::getReword(unsigned int pos){
    m_RewordPos = pos;
    HTTP_CLIENT->occupyFinishDragonValley(pos);
}

void KingTombModel::speedUp(unsigned int pos){
    HTTP_CLIENT->speedUpDragonValley(pos);
}

void KingTombModel::search(unsigned int pos){
    
    HTTP_CLIENT->searchDragonValley(pos);
}



void KingTombModel::battleStartDragonValley(unsigned int pos,Json::Value& assist){
    HTTP_CLIENT->battleStartDragonValley(pos,assist);
}



void KingTombModel::getModeData(){
    if (GameManager::sharedGameManager()->getServerTime() - m_nLastKingTomeListTime < D_CONFIG_FLOAT_FOR_KEY(KINGTOMB_LIST_UPDATE_TIME))
	{
		return;
	}
    HTTP_CLIENT->getOccupyListDragonValley();
}

void KingTombModel::syncAssistBonusList()
{
	if (GameManager::sharedGameManager()->getServerTime() - m_nLastAssistBonusListTime < D_CONFIG_FLOAT_FOR_KEY(KINGTOMB_HISTORY_UPDATE_TIME))
	{
		return;
	}
	HTTP_CLIENT->getAssistBonusListDragonValley();
}

void KingTombModel::syncLog()
{
	if (GameManager::sharedGameManager()->getServerTime() - m_nLastLogTime < D_CONFIG_FLOAT_FOR_KEY(KINGTOMB_HISTORY_UPDATE_TIME))
	{
		return;
	}
	HTTP_CLIENT->getLogDragonValley();
}

void KingTombModel::syncRankList(bool isUserRank)
{
	unsigned int now = GameManager::sharedGameManager()->getServerTime();
	if(isUserRank)
	{
		if(now > m_uUserRankTs + KINGTOMB_RANK_REFRESH_INTERVAL) HTTP_CLIENT->getUserRank();
		else postNotification(D_EVENT_KINGTOMB_GET_RANK);
	}
	else
	{
		if(now > m_uGuildRankTs + KINGTOMB_RANK_REFRESH_INTERVAL) HTTP_CLIENT->getGuildRank();
		else postNotification(D_EVENT_KINGTOMB_GET_RANK);
	}
}

void KingTombModel::parseMeetRare(const Json::Value& val)
{
	DJSON_STRING(m_uTodayUserName, val, "name");
	DJSON_STRING(m_uTodayGuildName, val, "guildName");
	DJSON_UINT(m_uTodayType, val, "treasureType");
	CCLOG("%d" , m_uTodayType);
	m_bTodayFound = m_uTodayType!=0;
}

void KingTombModel::parseRank(const Json::Value& val , bool isUserRank)
{
	//处理旧数据
	CCArray* pList = NULL;
	pList = isUserRank ? m_pUserRankList : m_pGuildRankList;
	pList->removeAllObjects();
	//更新时间
	if(isUserRank)m_uUserRankTs = GameManager::sharedGameManager()->getServerTime();
	else m_uGuildRankTs = GameManager::sharedGameManager()->getServerTime();
	//读取新数据
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
		kingTombRankInfo* pInfo = kingTombRankInfo::create();
		pInfo->parse(*i);
		pList->addObject(pInfo);
	}
	//更新
	postNotification(D_EVENT_KINGTOMB_GET_RANK);
}

void KingTombModel::parseAssistBonusList(const Json::Value& val)
{
	m_nLastAssistBonusListTime = GameManager::sharedGameManager()->getServerTime();
	unsigned int oldNum = m_pAssistBonusList->count();
	m_pAssistBonusList->removeAllObjects();
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		AssistBonusInfo* pInfo = AssistBonusInfo::create();
		pInfo->parse(*i);
		m_pAssistBonusList->addObject(pInfo);
	}
    
	m_bHaveNewAssistBonus = m_pAssistBonusList->count() > oldNum;
	postNotification(D_EVENT_KINGTOMB_GET_ASSIST_BONUS_LIST);
}

void KingTombModel::parseLog(const Json::Value& val)
{
	m_nLastLogTime = GameManager::sharedGameManager()->getServerTime();
	unsigned int oldNum = m_pLogList->count();
	m_pLogList->removeAllObjects();
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
//		Json::Value v = *i;
		LogInfo* pInfo = LogInfo::create();
		pInfo->parse(*i);
		m_pLogList->addObject(pInfo);
	}
	m_bHaveNewLog = m_pLogList->count() > oldNum;
	postNotification(D_EVENT_KINGTOMB_GET_LOG);
}

void KingTombModel::removeAssistBonusById(unsigned int id)
{
	cocos2d::CCObject* obj = NULL;
	CCARRAY_FOREACH(m_pAssistBonusList, obj)
	{
		AssistBonusInfo* pAssistBonusInfo = (AssistBonusInfo*)obj;
		if (pAssistBonusInfo && pAssistBonusInfo->getId() == id)
		{
			m_pAssistBonusList->removeObject(obj);
			postNotification(D_EVENT_KINGTOMB_GET_ASSIST_BONUS_LIST);
			break;
		}
	}
}

void KingTombModel::removeAllAssistBonus()
{
	m_pAssistBonusList->removeAllObjects();
	postNotification(D_EVENT_KINGTOMB_GET_ASSIST_BONUS_LIST);
}

void KingTombModel::getAssistBonusList(cocos2d::CCArray* pArr) const
{
	if (NULL == pArr)
	{
		return;
	}
	pArr->removeAllObjects();
	pArr->addObjectsFromArray(m_pAssistBonusList);
}

void KingTombModel::getLogList(cocos2d::CCArray* pArr) const
{
	if (NULL == pArr)
	{
		return;
	}
	pArr->removeAllObjects();
	pArr->addObjectsFromArray(m_pLogList);
}
