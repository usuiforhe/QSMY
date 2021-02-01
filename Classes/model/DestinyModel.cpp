//
//  DestinyModel.cpp
//  QSMY
//
//  Created by OnePiece on 13-6-5.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DestinyModel.h"
#include "../managers/GameManager.h"

using namespace destiny;
USING_NS_CC;

#define UpGradeRate (DM_GET_CONFIG_MODEL->getConfigByKey(DESTINY_UPGRADE_RATE)->intValue())


void DestinySlot::setCurrentNum(uint32_t val)
{
    m_nCurrentNum = val;
}

uint32_t DestinySlot::getCurrentNum()
{
    CCAssert(m_pDestinyInfo, "");
    if(m_pDestinyInfo->isFullLevel() || (m_pDestinyInfo->isCanReward() && m_pDestinyInfo->getRewardType()==kDestinyRewardOnce)) return getUpgradeNum();
    return m_nCurrentNum;
}

bool DestinySlot::isSlotUnlock()
{
    CCAssert(m_pDestinyInfo, "");
    if(m_pDestinyInfo->isFullLevel()) return true;
    return getCurrentNum()>=m_nUpgradeNum;
}

bool DestinySlot::isCanCollect()
{
    CCAssert(m_pDestinyInfo, "");
    if(m_pDestinyInfo->isFullLevel()) return false; //如果满级 不能收集
    if(m_pDestinyInfo->isCanReward() && m_pDestinyInfo->getRewardType()==kDestinyRewardOnce) return false;
    if(isSlotUnlock()) return false;                //如果曹伟收集满 不能收集
    
    WarriorModel* pWarriorModel = DM_GET_WARRIOR_MODEL;
    FormationModel* pFormationModel = DM_GET_FORMATION_MODEL;
    return pWarriorModel->getSoulInfoByWID(getWarriorID())!=NULL || (pWarriorModel->getWarriorCurInfoByWID(getWarriorID())!=NULL && !pFormationModel->isWarriorBattle(getWarriorID()));
}

bool DestinySlotAny::isCanCollect()
{
    CCAssert(m_pDestinyInfo, "");
    if(m_pDestinyInfo->isFullLevel() || m_pDestinyInfo->isEqUserLevel()) return false; //如果满级 不能收集
    if(isSlotUnlock()) return false;                //如果曹伟收集满 不能收集
    
    WarriorModel* pWarriorModel = DM_GET_WARRIOR_MODEL;
    FormationModel* pFormationModel = DM_GET_FORMATION_MODEL;
    if(pWarriorModel->getSoulVec()->count()>0) return true;
    if(pWarriorModel->getWarriorVec()->count()<=0) return false;
    
    cocos2d::CCArray* pWarriors = pWarriorModel->getWarriorVec();
    warrior::WarriorCurInfo *pWarriorCurInfo = NULL;
    CCObject* obj = NULL;
    bool canCollect = false;
    CCARRAY_FOREACH(pWarriors, obj)
    {
        pWarriorCurInfo = (warrior::WarriorCurInfo *)obj;
        if(!pFormationModel->isWarriorBattle(pWarriorCurInfo->getWid()))
        {
            canCollect = true;
            break;
        }
    }
    return canCollect;
}

uint32_t DestinySlotAny::getLvUpgradeNum()
{
    CCAssert(m_pDestinyInfo, "");
    if(m_pDestinyInfo->isFullLevel())  return UpGradeRate*m_pDestinyInfo->getLevel();
    uint16_t nextLevel = m_pDestinyInfo->getLevel()+1;
    return UpGradeRate*nextLevel;
}

uint32_t DestinySlotAny::getLvCurrentNum()
{
    CCAssert(m_pDestinyInfo, "");
    if(m_pDestinyInfo->isFullLevel() || m_pDestinyInfo->isEqUserLevel()) return getLvUpgradeNum();
    uint16_t cLevel = m_pDestinyInfo->getLevel();
    return m_nCurrentNum - UpGradeRate*(1+cLevel)*(float)cLevel/2;
}

uint32_t DestinyAnyUpgradeInfo::getRewardItemNum()
{
    CCAssert(m_pDestinyInfo, "");
    return  (uint32_t)(m_nRewardItemNum*((powf(1.05f, m_pDestinyInfo->getLevel()) - 1.f) / 0.05f));
}

float DestinyAnyUpgradeInfo::getRewardItemNum() const
{
    CCAssert(m_pDestinyInfo, "");
    return  m_nRewardItemNum*((powf(1.05f, m_pDestinyInfo->getLevel()) - 1.f) / 0.05f);
}

void DestinyInfo::setMaxLevel(uint16_t lv)
{
    m_nMaxLevel = lv;
}

uint16_t DestinyInfo::getMaxLevel()
{
    return m_nMaxLevel;
}

bool DestinyInfo::isCanCollect()
{
    DestinySlot* pSlot = NULL;
    for (uint32_t i= 0;i<getDestinySlotCount(); ++i) {
        pSlot = getDestinySlotByIndex(i);
        if(pSlot->isCanCollect()) return true;
    }
    return false;
}

bool DestinyInfo::isFullLevel()
{
    return m_nLevel == getMaxLevel();
}

//任意插的级别不能高过玩家级别 是否等于玩家等级
bool DestinyInfo::isEqUserLevel()
{
    uint16_t lv = DM_GET_USER_MODEL->getUserInfo()->getUserLv();
    return m_nLevel == lv; //任意插的级别不能高过玩家级别
}


//是否可以领取奖励
bool DestinyInfo::isCanReward()
{
    if(m_nLevel==0) return false;
    switch (m_nRewardType) {
        case kDestinyRewardOnce:
            return m_nRewardTime==0;
            break;
        case kDestinyRewardDaily:
            return m_nRewardTime<TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime());
        case kDestinyRewardProperty:
        default:
            return false;
    }
    return false;
}

bool DestinyInfo::isReWard()
{
    switch (m_nRewardType) {
        case kDestinyRewardOnce:
            return m_nRewardTime!=0;
            break;
        case kDestinyRewardDaily:
            return m_nRewardTime>TimeUtil::getTimeToday(GameManager::sharedGameManager()->getServerTime());
        case kDestinyRewardProperty:
        default:
            return false;
    }
    return false;
}

bool DestinyInfo::isNeedShowInterval()
{
    return m_nRewardType==kDestinyRewardDaily && isReWard();
}

float DestinyInfo::getPercent()
{
    destiny::DestinySlot *pSlotInfo = NULL;
    int n=0;
    int m=0;
    for (uint32_t i = 0 ; i<getDestinySlotCount(); ++i)
    {
        pSlotInfo = getDestinySlotByIndex(i);
        n+=pSlotInfo->getCurrentNum();
        m+=pSlotInfo->getUpgradeNum();
    }
    if(m==0) return 0.f;
    return (float)n/m;
}


DestinyModel::DestinyModel()
:m_pDestinyDic(NULL)
,m_DestinyColorMap(NULL)
{
    
}

DestinyModel::~DestinyModel()
{
    CC_SAFE_RELEASE(m_pDestinyDic);
    CC_SAFE_RELEASE(m_DestinyColorMap);
}
bool DestinyModel::init()
{
    m_pDestinyDic = CCDictionary::create();
    CC_SAFE_RETAIN(m_pDestinyDic);
    
    m_DestinyColorMap = CCDictionary::create();
    CC_SAFE_RETAIN(m_DestinyColorMap);
    
    loadData();
    return Model::init();
}

void DestinyModel::loadData()
{
    parseDestinys(  CSVDecoder::create(DESTINY_CONFIG_PATH)->getData());
    loadColorConfig(  CSVDecoder::create(DESTINY_COLOR_CONFIG_PATH)->getData());
}

void DestinyModel::loadColorConfig(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
        DestinyColorConfig *pDestinyColorConfig = DestinyColorConfig::create();
        DestinyGrade grade = (DestinyGrade)atoi(val[i][0].c_str());
		uint32_t color = 0;
        sscanf(val[i][1].c_str(), "%i",&color);
        pDestinyColorConfig->color = cocos2d::ccc3(color>>16&0xFF, color>>8&0xFF, color>>0&0xFF);
        m_DestinyColorMap->setObject(pDestinyColorConfig, grade);
	}
}

void DestinyModel::parseDestinys(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
        uint32_t dID = atoi(val[i][0].c_str());
        
        m_RequireTypeMap[dID] = (DestinyRequireType)atoi(val[i][4].c_str());
        
        /**
         *  初始化天命信息
         */
        DestinyInfo *pDestinyInfo = getDestinyInfoByID(dID);
        
        
        
        if(!pDestinyInfo)
        {
            //新的天命
            pDestinyInfo = DestinyInfo::create();
            
            pDestinyInfo->setID(dID);
            pDestinyInfo->setName(val[i][1].c_str());
            pDestinyInfo->setLevel(0);
            pDestinyInfo->setMaxLevel(atoi(val[i][3].c_str()));
            pDestinyInfo->setRequireType(m_RequireTypeMap[dID]);
            pDestinyInfo->setRewardType((DestinyRewardType)atoi(val[i][5].c_str()));
            pDestinyInfo->setDestinyGrade((DestinyGrade)atoi(val[i][21].c_str()));
            pDestinyInfo->setLimitLevel(atoi(val[i][22].c_str()));
            if(pDestinyInfo->isRequireAny())
            {
                pDestinyInfo->setMaxLevel(kMaxLevelForAnySoulDestiny);
            }
            
            //将天命信息加入天命库
            getDestinyListByRequireType(pDestinyInfo->getRequireType())->addObject(pDestinyInfo);
        }
        
        /**
         *  初始化等级信息
         */
        DestinyUpgradeInfo *pDestinyUpgradeInfo =NULL;
        
        if(pDestinyInfo->isRequireAny())
        {
            pDestinyUpgradeInfo = DestinyAnyUpgradeInfo::create();
            DestinySlotAny*pDestinySlot = DestinySlotAny::create();
            pDestinySlot->m_pDestinyInfo = pDestinyInfo;
            pDestinyUpgradeInfo->addSlot(pDestinySlot);
        }else
        {
            pDestinyUpgradeInfo = DestinyUpgradeInfo::create();
            //初始化6个槽
            for(int j = 0 ; j < MAX_SLOTS ; j ++)
            {
                int nWarriorID =  atoi(val[i][6 + j*2 + 0].c_str());
                int nUpgradeNum = atoi(val[i][6 + j*2 + 1].c_str());
                
                //warriorID=0时  不再需要添加
                if(nWarriorID<=0) continue;
                
                DestinySlot *pDestinySlot = DestinySlot::create();
                pDestinySlot->setWarriorID(nWarriorID);
                pDestinySlot->setCurrentNum(0);
                pDestinySlot->setUpgradeNum(nUpgradeNum);
                pDestinySlot->m_pDestinyInfo = pDestinyInfo;
                pDestinyUpgradeInfo->addSlot(pDestinySlot);
            }
        }
        //天命奖励内容类型
        //天命奖励内容ID
        //天命奖励内容num
        pDestinyUpgradeInfo->setRewardItemType((DestinyRewardItemType)atoi(val[i][18].c_str()));
        pDestinyUpgradeInfo->setRewardItemID(atoi(val[i][19].c_str()));
        pDestinyUpgradeInfo->setRewardItemNum(atof(val[i][20].c_str()));
        pDestinyUpgradeInfo->m_pDestinyInfo = pDestinyInfo;
        /**
         *  将等级信息加入天命信息s
         */
        pDestinyInfo->addDestinyUpgradeInfo(pDestinyUpgradeInfo);
	}
    
}

void DestinyModel::updateDestinys(const Json::Value &val)
{
    for(uint32_t i = 0 ; i < val.size() ; i++)
    {
        updateDestiny(val[i]);
    }
    
    //postNotification(D_EVENT_DESTINYS_UPDATED);
}

void DestinyModel::updateDestiny(const Json::Value &val)
{
    // {"destiny":{"nums":[31],"destinyId":2,"level":3,"rewardTs":0}}
    
    uint32_t destinyID = val["destinyId"].asUInt();
    destiny::DestinyInfo *pDestinyInfo = getDestinyInfoByID(destinyID);
    
    if(pDestinyInfo)
    {
        int nLevel = val["level"].asInt();
        
        //更新等级
        pDestinyInfo->setLevel(nLevel);
        
        //更新上次奖励时间
        pDestinyInfo->setRewardTime(val["rewardTs"].asUInt());
        
        //更新天命槽
        for(uint32_t i = 0 ; i < pDestinyInfo->getDestinySlotCount() ; i++)
        {
            DestinySlot *pDestinySlot = pDestinyInfo->getDestinySlotByIndex(i);
            pDestinySlot->setCurrentNum(val["nums"][i].asUInt());
        }
        pDestinyInfo->postNotification(D_EVENT_DESTINY_UPDATED);
    }
}

destiny::DestinyInfo * DestinyModel::getDestinyInfoByID(uint32_t did)
{
    DestinyRequireType type = m_RequireTypeMap[did];
    CCArray* pDestinyArray = getDestinyListByRequireType(type);
    CCObject *pObj = NULL;
    destiny::DestinyInfo *pDestinyInfo = NULL;
    CCARRAY_FOREACH(pDestinyArray, pObj)
    {
        pDestinyInfo = (destiny::DestinyInfo *)pObj;
        if(pDestinyInfo && pDestinyInfo->getID() == did)
        {
            return pDestinyInfo;
        }
    }
    
    return NULL;
}

uint32_t DestinyModel::getDestinyIndexByDid(uint32_t did)
{
    DestinyRequireType type = m_RequireTypeMap[did];
    CCArray* pDestinyArray = getDestinyListByRequireType(type);
    
    destiny::DestinyInfo *pDestinyInfo = NULL;
    for (uint32_t i =0; i<pDestinyArray->count(); ++i) {
        pDestinyInfo = (destiny::DestinyInfo *)pDestinyArray->objectAtIndex(i);
        if(pDestinyInfo && pDestinyInfo->getID() == did)
        {
            return i;
        }
    }
    return 0;
}


CCArray* DestinyModel::getDestinyListByRequireType(destiny::DestinyRequireType type)
{
    CCArray* pArray = (CCArray*)m_pDestinyDic->objectForKey(type);
    if(pArray) return pArray;
    pArray = CCArray::create();
    m_pDestinyDic->setObject(pArray, type);
    return pArray;
}



void DestinyModel::rewardDestiny(uint32_t did,uint32_t ts)
{
    destiny::DestinyInfo *pDestinyInfo = getDestinyInfoByID(did);
    pDestinyInfo->setRewardTime(ts);
    pDestinyInfo->postNotification(D_EVENT_DESTINY_UPDATED);
}

uint32_t DestinyModel::destinyAddProps(warrior::WarriorPropType type)
{
    destiny::DestinyInfo* pInfo = NULL;
    switch (type)
    {
        case warrior::kPropTypeAttak:
            pInfo = getDestinyInfoByID(3);
            if(pInfo==NULL) return 0;
            return pInfo->getCurrentDestinyUpgradeInfo()->getRewardItemNum();
        case warrior::kPropTypeDefence:
            pInfo = getDestinyInfoByID(2);
            if(pInfo==NULL) return 0;
            return pInfo->getCurrentDestinyUpgradeInfo()->getRewardItemNum();
        case warrior::kPropTypeHp:
            pInfo = getDestinyInfoByID(1);
            if(pInfo==NULL) return 0;
            return pInfo->getCurrentDestinyUpgradeInfo()->getRewardItemNum();
        case warrior::kPropTypeSpeed:
        case warrior::kPropTypeAll:
        default:
            return 0;
    }
    return 0;
}


