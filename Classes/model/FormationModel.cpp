//
//  FormationModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "FormationModel.h"
#include "../managers/GameManager.h"
#include <algorithm>
#include "WarriorModel.h"

using namespace formation;
USING_NS_CC;


FormationModel::FormationModel()
:m_Positions(NULL)
,m_UnlcokConfigs(NULL)
,m_UnlockedPositions(NULL)
,m_UserLevel(0)
{
    
}

FormationModel::~FormationModel()
{
    CC_SAFE_RELEASE_NULL(m_Positions);
    CC_SAFE_RELEASE_NULL(m_UnlcokConfigs);
    CC_SAFE_RELEASE_NULL(m_UnlockedPositions);
}

bool FormationModel::init()
{
    m_Positions = PositionArray::create();
    m_UnlcokConfigs = cocos2d::CCDictionary::create();
    m_UnlockedPositions = PositionArray::create();
    
    CC_SAFE_RETAIN(m_Positions);
    CC_SAFE_RETAIN(m_UnlcokConfigs);
    CC_SAFE_RETAIN(m_UnlockedPositions);
    
    loadData();
    return Model::init();
}

void FormationModel::loadData()
{
    loadUnlockConfig( CSVDecoder::create(FORMATION_UNLOCK_CONFIG_PATH)->getData());
}

void FormationModel::loadUnlockConfig(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
        FormationUnLockConfig *pFormationUnLockConfig = FormationUnLockConfig::create();
        pFormationUnLockConfig->level = atoi(val[i][0].c_str());
		pFormationUnLockConfig->iPosId = atoi(val[i][1].c_str());
        m_UnlcokConfigs->setObject(pFormationUnLockConfig, pFormationUnLockConfig->iPosId);
	}
}

void FormationModel::updateLockStatus(uint16_t userLevel)
{
    m_UserLevel = userLevel;
    updateLockStatus();
}

uint32_t FormationModel::getUnlockLevel(uint8_t iPosId)
{
    FormationUnLockConfig * pConfig = getUnlockConfig(iPosId);
    return pConfig?pConfig->level:0;
}

FormationUnLockConfig* FormationModel::getUnlockConfig(uint8_t iPosId)
{
    return static_cast<FormationUnLockConfig *>(m_UnlcokConfigs->objectForKey(iPosId));
}

bool FormationModel::getPosIsUnlock(uint8_t iPosId,uint16_t userLevel)
{
    FormationUnLockConfig * pConfig = getUnlockConfig(iPosId);
    return pConfig?userLevel>=pConfig->level:false;
}

void FormationModel::updateLockStatus(bool notifyUnlock)
{
    if(!isSynced || m_UserLevel==0) return;
    
    CCObject* obj =NULL;
    PositionInfo* pPos = NULL;
    
    m_UnlockedPositions->removeAllObjects();
    
    CCARRAY_FOREACH(m_Positions, obj)
    {
        pPos = (PositionInfo*)obj;
        if(pPos->isLeader())
        {
            m_UnlockedPositions->addObject(pPos);
            continue;
        }
        bool unLock = getPosIsUnlock(pPos->iPosId,m_UserLevel);
        if(unLock)
        {
            pPos->iPosStatus = kPositionPosStatusUnLocked;
            m_UnlockedPositions->addObject(pPos);
            if(notifyUnlock)
            {
                postNotification(D_EVENT_FORMATION_UPDATE);
            }
        }else
        {
            pPos->iPosStatus = kPositionPosStatusLocked;
        }
    }
    //sortPositions(m_UnlockedPositions); //遍历的列表排过序了 m_UnlockedPositions不需要再排序了
}

void FormationModel::parseFomations(const Json::Value &val)
{
    m_Positions->removeAllObjects();
    
    for (unsigned int i=0; i<val.size(); ++i){
        PositionInfo *pPos = parseFomation(val[i]);
        m_Positions->addObject(pPos);
    }
    isSynced = true;
    sortPositions(m_Positions);
    updateLockStatus(false);
    postNotification(D_EVENT_FORMATION_UPDATE);
}

void FormationModel::joinFormation(const Json::Value &val)
{
    uint32_t wid = val["wid"].asUInt();
    uint32_t id = val["id"].asUInt();
    getPositionInfoById(id)->iWarriorId = wid;
    postNotification(D_EVENT_FORMATION_UPDATE);
    postNotification(D_EVENT_FORMATION_JOIN);
}

void FormationModel::changeFormation(const Json::Value &val)
{
    uint32_t id1 = val["id1"].asUInt();
    uint32_t id2 = val["id2"].asUInt();
    uint8_t tempPos = getPositionInfoById(id1)->iPosId;
	formation::PositionInfo *pInfo1 = getPositionInfoById(id1);
	formation::PositionInfo *pInfo2 = getPositionInfoById(id2);
	uint8_t eStatus = pInfo1->iPosStatus;
	pInfo1->iPosStatus = pInfo2->iPosStatus;
	pInfo2->iPosStatus = eStatus;
    getPositionInfoById(id1)->iPosId = getPositionInfoById(id2)->iPosId;
    getPositionInfoById(id2)->iPosId = tempPos;
    sortPositions(m_Positions);
    sortPositions(m_UnlockedPositions);
    postNotification(D_EVENT_FORMATION_UPDATE);
}

void FormationModel::sortPositions(cocos2d::CCArray* pPositions)
{
    int i,j,length = pPositions->data->num;
    PositionInfo ** x = (PositionInfo**)pPositions->data->arr;
    PositionInfo *tempItem;
    
    for(i=1; i<length; i++)
    {
        tempItem = x[i];
        j = i-1;
        
        
        while(j>=0 && ( tempItem->iPosId < x[j]->iPosId) )
        {
            x[j+1] = x[j];
            j = j-1;
        }
        x[j+1] = tempItem;
    }
}

//获取阵容总战斗值
int  FormationModel::getFormationTotalForce()
{
    int nTotalForce = 0;
    
    CCObject* obj = NULL;
	PositionInfo* pPositionInfo = NULL;
    
	CCARRAY_FOREACH(m_Positions, obj)
	{
		pPositionInfo = (PositionInfo *)obj;
        if(pPositionInfo && pPositionInfo->isOpened())
        {
            int nWarriorID = pPositionInfo->iWarriorId;
            warrior::WarriorCurInfo *pWarriorCurInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(nWarriorID);
            if(pWarriorCurInfo)
            {
                nTotalForce += pWarriorCurInfo->computeWarriorForce();
            }
        }
	}
    
    return nTotalForce;
}


PositionInfo *FormationModel::parseFomation(const Json::Value &val,PositionInfo * pPos)
{
    
    if (pPos==NULL)
    {
        pPos = PositionInfo::create();
    }
    
    pPos->iId = val["id"].asUInt();
    pPos->iWarriorId = val["wid"].asUInt();
    pPos->iPosId = val["pid"].asUInt();
    pPos->iPosStatus = val["pstatus"].asUInt();
    
    return pPos;
}

formation::PositionInfo* FormationModel::getPositionInfoByIndex(uint8_t index)
{
    if(index<0 || index>=m_Positions->count()) return NULL;
    return (formation::PositionInfo*)(m_Positions->objectAtIndex(index));
}

formation::PositionInfo* FormationModel::getUnlockedPositionInfoByIndex(uint8_t index)
{
    if(index<0 || index>=m_UnlockedPositions->count()) return NULL;
    return (formation::PositionInfo*)(m_UnlockedPositions->objectAtIndex(index));
}

void FormationModel::getBattleWarriors(cocos2d::CCArray* pArray)
{
    if(pArray==NULL) return;
    pArray->removeAllObjects();
    CCObject* obj = NULL;
    PositionInfo* pPositionInfo = NULL;
    
    CCARRAY_FOREACH(m_Positions, obj)
    {
        pPositionInfo = (PositionInfo*)obj;
        if(!pPositionInfo || !pPositionInfo->isOpened()) continue;
        if(pPositionInfo->iWarriorId!=0)
        {
            pArray->addObject(CCInteger::create(pPositionInfo->iWarriorId));
        }
    }
}

bool FormationModel::isWarriorBattle(uint32_t wid)
{
    return getPositionInfoByWid(wid)!=NULL;
}

uint32_t FormationModel::getFormationLeader()
{
	CCObject* obj = NULL;
	PositionInfo* pPositionInfo = NULL;
    
	CCARRAY_FOREACH(m_Positions, obj)
	{
		pPositionInfo = (PositionInfo*)obj;
		if(pPositionInfo->isLeader())
			return pPositionInfo->iWarriorId;
	}
	return 0;
    
}


formation::PositionInfo* FormationModel::getPositionInfoByWid(uint32_t wid)
{
	CCObject* obj = NULL;
    PositionInfo* pPositionInfo = NULL;
    
    CCARRAY_FOREACH(m_Positions, obj)
    {
        pPositionInfo = (PositionInfo*)obj;
        if(pPositionInfo->iWarriorId==wid) return pPositionInfo;
    }
	return NULL;
}

formation::PositionInfo* FormationModel::getPositionInfoByPosId( unsigned int posId )
{
	CCObject* obj = NULL;
	PositionInfo* pPositionInfo = NULL;

	CCARRAY_FOREACH(m_Positions, obj)
	{
		pPositionInfo = (PositionInfo*)obj;
		if(pPositionInfo->iPosId==posId) return pPositionInfo;
	}
	return NULL;
}

formation::PositionInfo* FormationModel::getPositionInfoById(uint32_t id)
{
    CCObject* obj = NULL;
    PositionInfo* pPositionInfo = NULL;
    
    CCARRAY_FOREACH(m_Positions, obj)
    {
        pPositionInfo = (PositionInfo*)obj;
        if(pPositionInfo->iId==id) return pPositionInfo;
    }
    
	return NULL;
}


void FormationModel::addWarriorsExpInFormation(unsigned int exp)
{
	CCObject* obj = NULL;
	PositionInfo* pPositionInfo = NULL;

	CCARRAY_FOREACH(m_Positions, obj)
	{
		pPositionInfo = (PositionInfo*)obj;
		if(pPositionInfo && pPositionInfo->isOpened())
		{
			int nWarriorID = pPositionInfo->iWarriorId;
			warrior::WarriorCurInfo *pWarriorCurInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(nWarriorID);
			if(pWarriorCurInfo)
			{
				pWarriorCurInfo->addExp(exp);
			}
		}
	}
}

bool FormationModel::isFormationFull()
{
	int warriorNum = 0;
	CCObject* obj = NULL;
	PositionInfo* pPositionInfo = NULL;

	CCARRAY_FOREACH(m_Positions, obj)
	{
		pPositionInfo = (PositionInfo*)obj;
		if(!pPositionInfo || !pPositionInfo->isOpened()) continue;
		if(pPositionInfo->iWarriorId!=0)
		{
			warriorNum++;
		}
	}

	if (warriorNum < m_UnlockedPositions->count())
	{
		return false;
	}
	return true;
}

bool FormationModel::isEquipFull()
{
	CCObject* obj = NULL;
	PositionInfo* pPositionInfo = NULL;

	equip::EquipType eType = equip::kEquipTypeALL;

	CCARRAY_FOREACH(m_Positions, obj)
	{
		pPositionInfo = (PositionInfo*)obj;
		if(!pPositionInfo || !pPositionInfo->isOpened()) continue;
		if(pPositionInfo->iWarriorId!=0)
		{

			for (uint8_t i = equip::kEquipTypeALL + 1; i< equip::kEquipTypeCount; ++i)
			{
				equip::EquipCurInfo* pEquip = DM_GET_EQUIP_MODEL->getEquipedEquipByIdAndType(pPositionInfo->iId, (equip::EquipType)i);
				if (!pEquip)
				{
					eType = (equip::EquipType)i;
					break;
				}
				
			}
		}
	}
	if (eType == equip::kEquipTypeALL)
	{
		return true;
	}

	return !DM_GET_EQUIP_MODEL->hasUnequipedByType(eType);
}

