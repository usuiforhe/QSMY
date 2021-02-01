//
//  PrizeModel.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-9-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PrizeModel.h"
#include "../managers/GameManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace prize;

void PrizeItem::loadLine(const CSVDecoder::CSVLine &line)
{    
    m_nId = atoi(line[0].c_str());
    
	m_checkerType = (prizeItemCheckerType)atoi(line[1].c_str());
    switch (m_checkerType)
    {
        case kPrizeItemCheckerLoginDays:
        {
            m_pPrizeItemChecker = PrizeItemCheckerLoginDays::create();
            break;
        }
        case kPrizeItemCheckerLevel:
        {
            m_pPrizeItemChecker = PrizeItemCheckerLevel::create();
            break;
        }
//         case kPrizeItemCheckerTotalCharge:
//         {
//             m_pPrizeItemChecker = PrizeItemCheckerTotalCharge::create();
//             break;
//         }
        case kPrizeItemCheckerNone:
        default:
            break;
    }
    CC_ASSERT(m_pPrizeItemChecker != NULL);
    m_pPrizeItemChecker->retain();
    
	m_nThreshold = atoi(line[2].c_str());
	m_nDropType = atoi(line[3].c_str());
	m_nDropId = atoi(line[4].c_str());
	m_nDropNum = atoi(line[5].c_str());
    m_sDesc = line[6].c_str();
}

bool PrizeItem::isQualified()
{
    return m_pPrizeItemChecker->isQualified(m_nThreshold);
}

bool PrizeItemCheckerLevel::isQualified(uint32_t threshold)
{
    return DM_GET_USER_MODEL->getUserInfo()->getUserLv() >= threshold;
}

bool PrizeItemCheckerLoginDays::isQualified(uint32_t threshold)
{
    return DM_GET_PRIZE_MODEL->getTotalLoginDays() >= threshold;
}

#pragma mark ----------------PrizeModel-------------------

PrizeModel::PrizeModel()
:Model()
,m_pPrizeList(NULL)
,m_nDrawnLevel(0)
,m_nDrawnLoginDays(0)
,m_nTotalLoginDays(0)
,m_bIsDone(false)
{
    
}

PrizeModel::~PrizeModel()
{
    CC_SAFE_RELEASE(m_pPrizeList);
}

bool PrizeModel::init()
{
    m_pPrizeList = CCArray::create();
    m_pPrizeList->retain();
    
    loadPrizeData();
    
    return Model::init();
}

void PrizeModel::syncFromServer()
{
    if(isSynced)
    {
        return;
    }
    
    HTTP_CLIENT->activityGetAward();
}

void PrizeModel::parsePrize(const Json::Value &data)
{
    DJSON_UINT(m_nDrawnLevel, data, "lvAward");
    DJSON_UINT(m_nDrawnLoginDays, data, "tLoginAward");
    DJSON_UINT(m_nTotalLoginDays, data, "tLoginTimes");
    
    isSynced = true;
    postNotification(D_EVENT_PRIZE_INIT);
}

bool PrizeModel::isPrizeDrawn(prize::PrizeItem * pPrizeItem)
{
    switch (pPrizeItem->getCheckerType())
    {
        case kPrizeItemCheckerLoginDays:
        {
            return ((0x00000001 << (pPrizeItem->getThreshold() - 1)) & m_nDrawnLoginDays) != 0;
            break;
        }
        case kPrizeItemCheckerLevel:
        {
            return ((0x00000001 << (pPrizeItem->getThreshold() - 1)) & m_nDrawnLevel) != 0;
            break;
        }
        case kPrizeItemCheckerNone:
        case kPrizeItemCheckerTotalCharge:
        default:
            return true;
    }
    
    return true;
}

CCArray * PrizeModel::getNotDrawnPrizeList()
{
    CCArray * pList = CCArray::create();
    CCArray * pUnQualifiedList = CCArray::create();
    CCObject * pObject(NULL);
    CCARRAY_FOREACH(m_pPrizeList, pObject)
    {
        PrizeItem * pPrizeItem = dynamic_cast<PrizeItem *>(pObject);
        if(!isPrizeDrawn(pPrizeItem))
        {
            if(pPrizeItem->isQualified())
            {
                pList->addObject(pPrizeItem);
            }
            else
            {
                pUnQualifiedList->addObject(pPrizeItem);
            }
        }
    }
    
    pList->addObjectsFromArray(pUnQualifiedList);
    
    return pList;
}

void PrizeModel::onTLoginPrizeDrawn(uint32_t days)
{
    m_nDrawnLoginDays |= (0x00000001 << (days - 1));
    postNotification(D_EVENT_PRIZE_UPDATE);
}

void PrizeModel::onLevelPrizeDrawn(uint32_t level)
{
    m_nDrawnLevel |= (0x00000001 << (level - 1));
    postNotification(D_EVENT_PRIZE_UPDATE);
}

bool PrizeModel::isDone()
{
    if(!m_bIsDone)
    {
        m_bIsDone = getNotDrawnPrizeList()->count() == 0;
    }
    
    return m_bIsDone;
}

uint32_t PrizeModel::getQualifiedNum()
{
    CCObject * pObject(NULL);
    uint32_t num(0);
    CCARRAY_FOREACH(m_pPrizeList, pObject)
    {
        PrizeItem * pPrizeItem = dynamic_cast<PrizeItem *>(pObject);
        if(!isPrizeDrawn(pPrizeItem))
        {
            if(pPrizeItem->isQualified())
            {
                ++num;
            }
        }
    }
    
    return num;
}

void PrizeModel::loadPrizeData()
{    
    m_pPrizeList->removeAllObjects();
    CSVDecoder::CSV csv =  CSVDecoder::create(PRIZE_CONFIG_PATH)->getData();
    for(CSVDecoder::CSV::iterator iter(csv.begin()); iter != csv.end(); ++iter)
    {
        PrizeItem * pPrizeItem = PrizeItem::create();
        pPrizeItem->loadLine(*iter);
        m_pPrizeList->addObject(pPrizeItem);
    }
}

bool PrizeModel::isXueNvDrawn()
{
	uint32_t xueNvID = D_CONFIG_UINT_FOR_KEY(PRIZE_XUENV);
	CCObject * pObject(NULL);
	CCARRAY_FOREACH(m_pPrizeList, pObject)
	{
		PrizeItem * pPrizeItem = dynamic_cast<PrizeItem *>(pObject);
		if(pPrizeItem->getId() == xueNvID)
		{
			return isPrizeDrawn(pPrizeItem);
		}
	}
	return true;
}