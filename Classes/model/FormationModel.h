//
//  FormationModel.h
//  阵容信息
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__FormationModel__
#define __QSMY__FormationModel__

#include "Model.h"

#define  D_EVENT_FORMATION_UPDATE "update formation"
#define  D_EVENT_FORMATION_JOIN  "join formation"


namespace formation {
    
    enum PositionPosStatus
    {
        kPositionPosStatusLocked = 0,
        kPositionPosStatusUnLocked,
        kPositionPosStatusLeader
    };
    
    class PositionInfo:public ValueObjcet
    {
    public:
        PositionInfo()
        :iId(0)
        ,iWarriorId(0)
        ,iPosId(0)
        ,iPosStatus(0)
        {
            
        }
        
        CREATE_FUNC(PositionInfo);
        D_INIT_DEFAULT(ValueObjcet);
        
        uint32_t iId;             //唯一编号
        uint32_t iWarriorId;    //单位编号
        uint8_t  iPosId;         //位置编号
        uint8_t  iPosStatus;     //位置是否开启:0否1是 2是队长
        
        bool isOpened()
        {
            return iPosStatus != kPositionPosStatusLocked;
        }
        bool isLeader()
        {
            return iPosStatus == kPositionPosStatusLeader;
        }
        
        unsigned int getWarriorId()
        {
            return iWarriorId;
        }
        
        unsigned int getPosId()
        {
            return iPosId;
        }
    };
    
    class FormationUnLockConfig:public ValueObjcet
    {
    public:
        CREATE_FUNC(FormationUnLockConfig);
        D_INIT_DEFAULT(ValueObjcet);
        uint8_t iPosId;
        uint32_t level;
    };
    
    
}

class FormationModel : public Model
{
public:
    typedef cocos2d::CCArray PositionArray;//std::vector<formation::PositionInfo*>
    
public:
    FormationModel();
    virtual ~FormationModel();
    virtual bool init();
    void loadData();
    
public:
    
    formation::PositionInfo* getPositionInfoByIndex(unsigned char index); //获取位置信息
    formation::PositionInfo* getUnlockedPositionInfoByIndex(unsigned char index);//返回解锁位置信息
    
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
    
    
	formation::PositionInfo* getPositionInfoByWid(unsigned int wid);
    formation::PositionInfo* getPositionInfoByPosId(unsigned int posId);
	formation::PositionInfo* getPositionInfoById(unsigned int id);

	PositionArray* getPositionsArray(){return m_Positions;}
    
    //判断单位是否出战
    bool isWarriorBattle(unsigned int wid);
    
    void getBattleWarriors(cocos2d::CCArray*);
    
	//获得队长wid
	unsigned int getFormationLeader();
    
	//阵容中所有弟子增加经验
    void addWarriorsExpInFormation(unsigned int exp);
    
    void updateLockStatus(unsigned short userLevel);
    
    bool getPosIsUnlock(unsigned char iPosId,unsigned short userLevel);
    
    /**
     *	@brief	获取位置解锁等级
     *
     *	@param 	iPosId 	<#iPosId description#>
     *
     *	@return	<#return value description#>
     */
    unsigned int getUnlockLevel(unsigned char iPosId);
    
    //获取阵容总战斗值
    int  getFormationTotalForce();

public:
    void parseFomations(const Json::Value &val);
    void joinFormation(const Json::Value &val);
    void changeFormation(const Json::Value &val);
    
    
private:
     formation::FormationUnLockConfig* getUnlockConfig(uint8_t iPosId);
    
    void loadUnlockConfig(const CSVDecoder::CSV &val);
    formation::PositionInfo * parseFomation(const Json::Value &val,formation::PositionInfo * pPos=NULL);
    void sortPositions(cocos2d::CCArray*);
    void updateLockStatus(bool notifyUnlock = true);
    
private:
    PositionArray * m_Positions;
    cocos2d::CCDictionary * m_UnlcokConfigs;
    PositionArray * m_UnlockedPositions;
    uint32_t m_UserLevel;

public:
	bool isFormationFull();
	bool isEquipFull();
};



#endif /* defined(__QSMY__FormationModel__) */
