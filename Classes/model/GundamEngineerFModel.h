///////////////////////////////////////////////////////////////////////////////////////////////////
//  
// GundamEngineerFModel.h
// QSMY
// 机师数据模型
//
// Created by jin.xia on 14-07-02.
// Copyright (c) 2013-2014 thedream. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __QSMY__EngineerFormationModel__
#define __QSMY__EngineerFormationModel__

#include "Model.h"

#define  D_EVENT_GUNDAM_E_FORMATION_JOIN     "D_EVENT_GUNDAM_E_FORMATION_JOIN"
#define  D_EVENT_GUNDAM_E_FORMATION_UPDATE   "D_EVENT_GUNDAM_E_FORMATION_UPDATE"


namespace gundam_engineer_formation
{
    enum GundamEngineerPosStatus
    {
        kGundamEngineerPosLocked = 0,
        kGundamEngineerPosUnlocked = 1,
    } ;
    



    ///////////////////////////////////////////////////////////////////////////////////////////////
    //  
    // GundamEngineerPosInfo
    // 机师位置信息
    // 
    ///////////////////////////////////////////////////////////////////////////////////////////////
    class GundamEngineerPosInfo : public ValueObjcet
    {
    public:
        //唯一编号
        uint32_t id ; 
        //单位编号           
        uint32_t warriorId ;     
        //位置编号
        uint8_t posId ; 
        //位置是否开启:0否1是       
        uint8_t posStatus ;     
        
        GundamEngineerPosInfo()
        :id( 0 )
        ,posId( 0 )
        ,warriorId( 0 )
        ,posStatus( 0 )
        {
        }
        
        CREATE_FUNC( GundamEngineerPosInfo ) ;
        D_INIT_DEFAULT( ValueObjcet ) ;
        
        bool isOpened()
        {
            return posStatus != kGundamEngineerPosLocked ;
        }
    };
    



    ///////////////////////////////////////////////////////////////////////////////////////////////
    //  
    // GundamEngineerUnlockConfig
    // 机师位置信息
    // 
    ///////////////////////////////////////////////////////////////////////////////////////////////
    class GundamEngineerUnlockConfig : public ValueObjcet
    {
    public:
        CREATE_FUNC( GundamEngineerUnlockConfig ) ;
        D_INIT_DEFAULT( ValueObjcet ) ;
        uint8_t posId;
        uint32_t level;
    } ;
}





USING_NS_CC ;
USING_NS_CC_EXT ;
using namespace Json ;
using namespace gundam_engineer_formation ;
///////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 机关兽-机师阵容数据模型
// 
///////////////////////////////////////////////////////////////////////////////////////////////////
class GundamEngineerFModel : public Model
{
public:
    GundamEngineerFModel() ;
    virtual ~GundamEngineerFModel() ;
    virtual bool init() ;
    void requestGundamData();
    void parseFomations( const Value &val ) ;
    void joinFormation( const Value &val ) ;
    void changeFormation( const Value &val ) ;
    void updateLockStatusByLv( unsigned short userLevel ) ;
    // 获取位置信息
    GundamEngineerPosInfo *getPosInfoByIndex( unsigned char index ) ;
    GundamEngineerPosInfo *getPosInfoByWid( unsigned int wid ) ;
    GundamEngineerPosInfo *getPosInfoById( unsigned int id ) ;
    // 获取解锁位置信息
    GundamEngineerPosInfo *getUnlockedPosInfoByIndex( unsigned char index ) ;
    //返回位置个数
    unsigned int getPositionsCount()
    {
        return m_Positions->count();
    }
    //返回解锁位置个数
    unsigned int getUnlockedPositionsCount()
    {
        return m_UnlockedPositions->count();
    }
    CCArray *getPositionsArray()
    {
        return m_Positions ;
    }
    /**
     *  获取位置解锁等级
     *  @param  posId  机师位置
     *  @return int 解锁等级
     */
    unsigned int getUnlockLevel( unsigned char posId ) ;
    bool isPosUnlock( uint8_t posId, uint16_t userLevel ) ;
    bool isEgineerBattle( uint32_t wid ) ;
    bool isGundamOpen(unsigned short userLevel) ;

	void calcGundamPropertyAdd(uint32_t* addAttack, uint32_t* addDefence, uint32_t* addHp);

private:
    CCArray *m_Positions ;
    CCArray *m_UnlockedPositions ;
    CCDictionary *m_UnlcokConfigs ;
    uint32_t m_UserLevel ;

    void loadData() ;
    void loadUnlockConfig( const CSVDecoder::CSV &val) ;
    void updateLockStatus( bool notifyUnlock = true) ;
    void sortPositions( CCArray *arr ) ;
    GundamEngineerUnlockConfig *getUnlockConfig( uint8_t posId ) ;
};



#endif /* defined(__QSMY__EngineerFormationModel__) */
