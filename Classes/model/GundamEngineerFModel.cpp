///////////////////////////////////////////////////////////////////////////////////////////////////
//  
// GundamEngineerFModel.ccp
// QSMY
// 机师数据模型
//
// Created by jin.xia on 14-07-02.
// Copyright (c) 2013-2014 thedream. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "../managers/GameManager.h"
#include <algorithm>
#include "WarriorModel.h"

#include "GundamEngineerFModel.h"

USING_NS_CC ;
USING_NS_CC_EXT ;
using namespace Json ;
using namespace gundam_engineer_formation ;


#define FORMATION_ENGINEER_POS_MAX		8                            //机量阵型一方最大位置数

GundamEngineerFModel::GundamEngineerFModel()
:m_Positions( NULL )
,m_UnlcokConfigs( NULL )
,m_UnlockedPositions( NULL )
,m_UserLevel( 0 )
{
    
}


GundamEngineerFModel::~GundamEngineerFModel()
{
    CC_SAFE_RELEASE_NULL( m_Positions ) ;
    CC_SAFE_RELEASE_NULL( m_UnlcokConfigs ) ;
    CC_SAFE_RELEASE_NULL( m_UnlockedPositions ) ;
}


bool GundamEngineerFModel::init()
{
    m_Positions = CCArray::create() ;
    CC_SAFE_RETAIN( m_Positions ) ;

    m_UnlcokConfigs = CCDictionary::create() ;
    CC_SAFE_RETAIN( m_UnlcokConfigs ) ;

    m_UnlockedPositions = CCArray::create() ;
    CC_SAFE_RETAIN( m_UnlockedPositions ) ;
    
    loadData() ;

    return Model::init() ;
}


void GundamEngineerFModel::loadData()
{
    loadUnlockConfig( CSVDecoder::create( "data/guys/guys" )->getData() ) ;
}


void GundamEngineerFModel::loadUnlockConfig( const CSVDecoder::CSV &val )
{
    for ( uint32_t i=0; i<val.size(); ++i )
	{
        GundamEngineerUnlockConfig *m_UnlockConfig = GundamEngineerUnlockConfig::create() ;
		m_UnlockConfig->posId = atoi( val[i][0].c_str() ) ;
        m_UnlockConfig->level = atoi( val[i][1].c_str() ) ;
        m_UnlcokConfigs->setObject( m_UnlockConfig, m_UnlockConfig->posId ) ;
	}
}


void GundamEngineerFModel::updateLockStatusByLv( unsigned short userLevel )
{
    
    m_UserLevel = userLevel ;
    updateLockStatus(false) ;
}

void GundamEngineerFModel::requestGundamData()
{
	if(!isSynced)    HTTP_CLIENT->getGundam();
}

void GundamEngineerFModel::parseFomations( const Value &val )
{
    // "gundamFormation":{"gundamId":0,"masters":{"1":0,"2":0,"3":0,"4":0,"5":0,"6":0,"7":0}}
    m_Positions->removeAllObjects() ;
    for ( unsigned int i=1; i <= FORMATION_ENGINEER_POS_MAX; ++i )
    {
        GundamEngineerPosInfo *posInfo = GundamEngineerPosInfo::create() ;
        posInfo->id = i ;
        posInfo->posId = i ;
        posInfo->warriorId = val["masters"][CCString::createWithFormat( "%d", i )->getCString()].asUInt() ;
        m_Positions->addObject( posInfo ) ;
    }
    isSynced = true ;
    sortPositions( m_Positions ) ;
    updateLockStatusByLv(DM_GET_USER_MODEL->getUserInfo()->getUserLv());
    postNotification( D_EVENT_GUNDAM_E_FORMATION_UPDATE ) ;
}


void GundamEngineerFModel::joinFormation( const Value &val )
{
    uint32_t wid = val["wid"].asUInt() ;
    uint32_t id = val["pos"].asUInt() ;
    getPosInfoById( id )->warriorId = wid ;
    postNotification( D_EVENT_GUNDAM_E_FORMATION_UPDATE ) ;
    //postNotification( D_EVENT_GUNDAM_E_FORMATION_JOIN ) ;
}


void GundamEngineerFModel::changeFormation( const Value &val )
{
	CCLOG( "already give gundamFormation" ) ;
  /*  uint32_t id1 = val["pos1"].asUInt() ;
    uint32_t id2 = val["pos2"].asUInt() ;
    uint8_t tempPos = getPosInfoById( id1 )->warriorId ;
    getPosInfoById( id1 )->warriorId = getPosInfoById( id2 )->warriorId ;
    getPosInfoById( id2 )->warriorId = tempPos ;
    sortPositions( m_Positions ) ;
    sortPositions( m_UnlockedPositions ) ;
	postNotification( D_EVENT_GUNDAM_E_FORMATION_UPDATE ) ;*/
}



uint32_t GundamEngineerFModel::getUnlockLevel( uint8_t posId )
{
    GundamEngineerUnlockConfig *m_UnlockConfig = getUnlockConfig( posId ) ;
    return m_UnlockConfig ? m_UnlockConfig->level : 0 ;
}


bool GundamEngineerFModel::isPosUnlock( uint8_t posId, uint16_t userLevel )
{
    uint32_t unlockedLevel = getUnlockLevel( posId ) ;
    return userLevel >= unlockedLevel ;
}


bool GundamEngineerFModel::isEgineerBattle( uint32_t wid )
{
    return getPosInfoByWid( wid ) != NULL ;
}


bool GundamEngineerFModel::isGundamOpen(unsigned short userLevel)
{
    return ( userLevel >= getUnlockLevel( 1 ) ) ;
}


GundamEngineerPosInfo *GundamEngineerFModel::getPosInfoByIndex( uint8_t index )
{
    if ( index < 0 || index >= m_Positions->count() )
    {
        return NULL ;
    }
    return (GundamEngineerPosInfo *)( m_Positions->objectAtIndex( index ) ) ;
}
  

GundamEngineerPosInfo* GundamEngineerFModel::getPosInfoByWid( uint32_t wid)
{
    CCObject *obj = NULL;
    GundamEngineerPosInfo *posInfo = NULL;
    CCARRAY_FOREACH( m_Positions, obj )
    {
        posInfo = (GundamEngineerPosInfo*)obj ;
        if ( posInfo->warriorId == wid ) 
        {
            return posInfo ;
        }
    }
    return NULL ;
}


GundamEngineerPosInfo* GundamEngineerFModel::getPosInfoById( uint32_t id )
{
    CCObject *obj = NULL;
    GundamEngineerPosInfo *posInfo = NULL;
    CCARRAY_FOREACH( m_Positions, obj )
    {
        posInfo = (GundamEngineerPosInfo*)obj ;
        if ( posInfo->id == id ) 
        {
            return posInfo ;
        }
    }
    return NULL ;
}


GundamEngineerPosInfo *GundamEngineerFModel::getUnlockedPosInfoByIndex( uint8_t index )
{
    if ( index < 0 || index >= m_UnlockedPositions->count() )
    {
        return NULL ;
    }
    return (GundamEngineerPosInfo *)( m_UnlockedPositions->objectAtIndex( index ) ) ;
}


GundamEngineerUnlockConfig* GundamEngineerFModel::getUnlockConfig( uint8_t posId )
{
    return static_cast<GundamEngineerUnlockConfig *>( m_UnlcokConfigs->objectForKey( posId ) ) ;
}


void GundamEngineerFModel::updateLockStatus( bool notifyUnlock )
{
    if( !isSynced || m_UserLevel == 0 ) return ;

    m_UnlockedPositions->removeAllObjects();
    CCObject *obj = NULL ;
    GundamEngineerPosInfo *posInfo = NULL;
	//是否需要抛出更新通知
	bool pNeedUpdate = false ;
    CCARRAY_FOREACH( m_Positions, obj )
    {
        posInfo = (GundamEngineerPosInfo *)obj ;
        bool unLock = isPosUnlock( posInfo->posId, m_UserLevel ) ;
        if ( unLock )
        {
            posInfo->posStatus = kGundamEngineerPosUnlocked ;
            m_UnlockedPositions->addObject( posInfo ) ;
            if ( notifyUnlock )
            {
				pNeedUpdate = true ;
            }
        }
        else
        {
            posInfo->posStatus = kGundamEngineerPosLocked;
        }
    }
	if ( pNeedUpdate )
	{
		postNotification( D_EVENT_GUNDAM_E_FORMATION_UPDATE ) ;
	}
}


void GundamEngineerFModel::sortPositions( CCArray *posInfoArr )
{
    int length = posInfoArr->data->num ;
    GundamEngineerPosInfo **x = (GundamEngineerPosInfo**)posInfoArr->data->arr ;
    GundamEngineerPosInfo *tempItem ;
    for( int i = 1; i < length; i++ )
    {
        tempItem = x[i];
        int j = i - 1 ;
        while ( j >= 0 && ( tempItem->posId < x[j]->posId) )
        {
            x[j+1] = x[j];
            j = j-1;
        }
        x[j+1] = tempItem;
    }
}

void GundamEngineerFModel::calcGundamPropertyAdd(uint32_t* addAttack, uint32_t* addDefence, uint32_t* addHp)
{
	updateLockStatusByLv(DM_GET_USER_MODEL->getUserInfo()->getUserLv());
	//计算属性加成
	uint32_t allAttack = 0;
	uint32_t allDefence = 0;
	uint32_t allHp = 0;
	CCObject *obj = NULL;
	GundamEngineerPosInfo *posInfo = NULL;
	WarriorModel* pWarriorModel = DM_GET_WARRIOR_MODEL;
	warrior::WarriorCurInfo* warriorCurrInfo = NULL;
	CCARRAY_FOREACH( m_Positions, obj)
	{
		posInfo = (GundamEngineerPosInfo*)obj ;
		if (!posInfo || !posInfo->isOpened() || !posInfo->warriorId) continue;
		warriorCurrInfo = pWarriorModel->getWarriorCurInfoByWID(posInfo->warriorId);
		//warriorCurrInfo->computeWarriorProps();

		allAttack += warriorCurrInfo->getAttack();
		allDefence += warriorCurrInfo->getDefence();
		allHp += warriorCurrInfo->getHp();
	}
	//CCLOG("Gundam property add---------allAttack:%d, allDefence:%d, allHp:%d", allAttack, allDefence, allHp);
	//CCLOG("Gundam property add---------allAttack:%d, allDefence:%d, allHp:%d", allAttack/10, allDefence/10, allHp/10);
	//CCLOG("Gundam property add---------allAttack:%f, allDefence:%f, allHp:%f", allAttack/10.0, allDefence/10.0, allHp/10.0);
	*addAttack = (int)(allAttack / 10.0);
	*addDefence = (int)(allDefence / 10.0);
	*addHp = (int)(allHp / 10.0);
}
