//
//  ActivenessModel.cpp
//  QSMY
//
//  created by cll on 13-6-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ActivenessModel.h"
#include "../managers/GameManager.h"
#include "../model/DropModel.h"

USING_NS_CC;
using namespace activeness;

#pragma mark - ActivenessInfo 活跃度配置信息 -

void ActivenessInfo::loadLine(const CSVDecoder::CSVLine &line)
{
    m_nId = atoi(line[0].c_str());
	m_nTarget = atoi(line[1].c_str());
	m_nPoint = atoi(line[3].c_str());
	m_sName = line[4].c_str();
	m_sDesc = line[5].c_str();
	m_eNavType = (navigation::NavigationType)atoi(line[6].c_str());
	m_eNavTag = atoi(line[7].c_str());
}

bool ActivenessInfo::isUnlocked()
{
    navigation::NavigationNode * pNode = DM_GET_NAVIGATION_MODEL->getNavigationNodeByNavType(m_eNavType);
    return pNode == NULL || pNode->isUnlocked();
};

#pragma mark - UserActiveness 玩家活跃度达成情况 -

void UserActiveness::loadData(const Json::Value &val)
{    
    uint32_t id(0);
	DJSON_UINT(id, val, "id");
	DJSON_UINT(m_nTimes,val,"times");
    CC_SAFE_RELEASE_NULL(m_pInfo);
	m_pInfo = DM_GET_ACTIVENESS_MODEL->getActivenessInfo(id);
    CC_SAFE_RETAIN(m_pInfo);
    postNotification(D_EVENT_ACTIVENESS_USER_UPDATE);
}

#pragma mark - AwardItemInfo 奖励项信息 -

drop::Drop * AwardItemInfo::toDrop(AwardInfo * pAward)
{
    drop::Drop * pDrop(NULL);
    switch (m_nType) {
        case kAwardTypeItem:
        {
            item::ItemInfo * pItem = item::ItemInfo::create(m_nId,m_nNum);
            pDrop = drop::Drop::create(pItem);
            
            break;
        }
        case kAwardTypeEquip:
        {
            equip::EquipCurInfo * pEquip = equip::EquipCurInfo::create(m_nId);
            pDrop = drop::Drop::create(pEquip);
            break;
        }
        case kAwardTypeCoin:
        {
            uint32_t userLevel = DM_GET_USER_MODEL->getUserInfo()->getUserLv();
            uint32_t coin = m_nNum * (uint32_t)(pow(pAward->getCoinFactor(), (3.0f * userLevel -1) / 3));
            pDrop = drop::Drop::createCoin(coin);
            break;
        }
        case kAwardTypePoint:
        {
            pDrop = drop::Drop::createPoint(m_nNum);
            break;
        }
        default:
            break;
    }
    return pDrop;
}

#pragma mark - AwardInfo 活跃度奖励配置 -

void AwardInfo::loadLine(const CSVDecoder::CSVLine &line, AwardInfo* pInfo)
{
    m_nRequirePoint = atoi(line[1].c_str());
	drop::Drop *pDrop = drop::Drop::create((drop::DropType(atoi(line[2].c_str()))), atoi(line[3].c_str()), atoi(line[4].c_str()));
	m_pAwardItems->addObject(pDrop);
    //m_fCoinFactor = atof(line[1].c_str());
    
	/*uint32_t column = line.size();

	for (uint32_t i = 2; i < column - 2; i += 3)
	{
	if(atoi(line[i].c_str()) > 0)
	{
	AwardItemInfo *pAwardItem = AwardItemInfo::create();
	pAwardItem->setType((AwardItemInfo::AwardItemType)(atoi(line[i].c_str())));
	pAwardItem->setId(atoi(line[i + 1].c_str()));
	pAwardItem->setNum(atoi(line[i + 2].c_str()));

	m_pAwardItems->addObject(pAwardItem);
	}
	}*/
}

#pragma mark - ActivenessModel 活跃度模块数据 -

ActivenessModel::ActivenessModel():
m_nPoint(0),
m_pDicAward(NULL),
m_pDicActivenessInfo(NULL),
m_pDicActiveness(NULL),
m_pArrDrawnAward(NULL)
{
    
}

ActivenessModel::~ActivenessModel()
{
    CC_SAFE_RELEASE(m_pDicAward);
    CC_SAFE_RELEASE(m_pDicActivenessInfo);
    CC_SAFE_RELEASE(m_pDicActiveness);
    CC_SAFE_RELEASE(m_pArrDrawnAward);
}

bool ActivenessModel::init()
{
    m_pDicAward = cocos2d::CCDictionary::create();
    CC_SAFE_RETAIN(m_pDicAward);
    
    m_pDicActivenessInfo = cocos2d::CCDictionary::create();
    CC_SAFE_RETAIN(m_pDicActivenessInfo);
    
    m_pDicActiveness = cocos2d::CCDictionary::create();
    CC_SAFE_RETAIN(m_pDicActiveness);
    
    m_pArrDrawnAward = cocos2d::CCArray::create();
    CC_SAFE_RETAIN(m_pArrDrawnAward);
    
    loadActivenessData();
    loadAwardData();
    
    return Model::init();
}

void ActivenessModel::parseActiveness(const Json::Value &val)
{
    if(val.isNull()) return;
    uint32_t changeOnly(0);
    DJSON_UINT(changeOnly, val, "changeOnly");
    DJSON_UINT(m_nPoint, val, "point");
    const Json::Value & rewards = val["rewards"];
    const Json::Value & list = val["list"];
    
    if(!changeOnly)
    {
        m_pDicActiveness->removeAllObjects();
    }
    else if(!isSynced)
    {
        return;
    }
    
    m_pArrDrawnAward->removeAllObjects();
    for(Json::Value::iterator iter(rewards.begin()); iter != rewards.end(); ++iter)
    {
        m_pArrDrawnAward->addObject(cocos2d::CCInteger::create((*iter).asUInt()));
    }
    
    for(Json::Value::iterator iter(list.begin()); iter != list.end(); ++iter)
    {
        UserActiveness * activeness = UserActiveness::create();
        activeness->loadData(*iter);
        if(activeness->getInfo() != NULL)
        {
            m_pDicActiveness->setObject(activeness, activeness->getInfo()->getId());
        }
        
        isSynced = true;
    }
    postNotification(D_EVENT_ACTIVENESS_UPDATE);
    //m_nPoint = 70;
}

void ActivenessModel::onGetAwardComplete(uint32_t point)
{
    m_pArrDrawnAward->addObject(cocos2d::CCInteger::create(point));
    postNotification(D_EVENT_ACTIVENESS_AWARD_DRAWN);
}

cocos2d::CCArray * ActivenessModel::getAwardInfoList()
{
    cocos2d::CCArray * pList = cocos2d::CCArray::create();
    cocos2d::CCDictElement * pElement(NULL);
    CCDICT_FOREACH(m_pDicAward, pElement)
    {
        AwardInfo * award = dynamic_cast<AwardInfo *>(pElement->getObject());
        if(award != NULL)
        {
            pList->addObject(award);
        }
    }
    return pList;
}

cocos2d::CCArray * ActivenessModel::getActivenessInfoList()
{
    cocos2d::CCArray * pList = cocos2d::CCArray::create();
    cocos2d::CCDictElement * pElement(NULL);
    CCDICT_FOREACH(m_pDicActivenessInfo, pElement)
    {
        ActivenessInfo * info = dynamic_cast<ActivenessInfo *>(pElement->getObject());
        if(info != NULL)
        {
            pList->addObject(info);
        }
    }
    return pList;
}

void ActivenessModel::loadActivenessData()
{
    m_pDicActivenessInfo->removeAllObjects();
    CSVDecoder::CSV csv =  CSVDecoder::create(ACTIVENESS_CONFIG_PATH)->getData();
    for(CSVDecoder::CSV::iterator iter(csv.begin()); iter != csv.end(); ++iter)
    {
        ActivenessInfo * info = ActivenessInfo::create();
        info->loadLine(*iter);
        m_pDicActivenessInfo->setObject(info, info->getId());
    }
}


void ActivenessModel::loadAwardData()
{
    m_pDicAward->removeAllObjects();
    CSVDecoder::CSV csv =   CSVDecoder::create(ACTIVENESS_AWARD_CONFIG_PATH)->getData();
	AwardInfo *info = NULL;
	uint32_t requirePoint = 0;  
	CSVDecoder::CSVLine line;
    for(CSVDecoder::CSV::iterator iter(csv.begin()); iter != csv.end(); ++iter)
    {
		line = *iter;
		requirePoint = atoi(line[1].c_str());
		info = (AwardInfo*)m_pDicAward->objectForKey(requirePoint);
		if(!info)
		{
			info = AwardInfo::create();
		}
        info->loadLine(*iter, info);
        m_pDicAward->setObject(info, info->getRequirePoint());
	}
}

bool ActivenessModel::hasAwardDrawable()
{
    cocos2d::CCDictElement * pElement(NULL);
    CCDICT_FOREACH(m_pDicAward, pElement)
    {
        AwardInfo * award = dynamic_cast<AwardInfo *>(pElement->getObject());
        if(award != NULL)
        {
            if(isAwardAvailable(award) && !isAwardDrawn(award)) return true;
        }
    }
    return false;
}

