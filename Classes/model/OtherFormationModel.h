//
//  OtherFormationModel.h
//  其他人阵容
//
//  Created by cll on 13-6-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__OtherFormationModel__
#define __QSMY__OtherFormationModel__

#include "Model.h"
#include "FormationModel.h"
#include "WarriorModel.h"
#include "EquipModel.h"
#include "SkillModel.h"
#include "GundamEngineerFModel.h"

#define D_EVENT_OTHERFORMATION_UPDATE "update otherformation"


class OtherFormationModel;

namespace otherFormation {
    
    class FormationInfo;
    
	//机器人阵容信息
	class RobotFormationInfo :public ValueObjcet
	{
	public:
		RobotFormationInfo():fid(0){};

		CREATE_FUNC(RobotFormationInfo);
		~RobotFormationInfo()
		{
			CC_SAFE_RELEASE_NULL(posInfoArray);
		};

		bool init()
		{
            if(!ValueObjcet::init()) return false;
			posInfoArray = cocos2d::CCArray::create();
			CC_SAFE_RETAIN(posInfoArray);
			return true;
		};
        
        uint32_t getLeaderId()
        {
            if(posInfoArray->count() > 0)
            {
                cocos2d::CCInteger* wid = (cocos2d::CCInteger*)(posInfoArray->objectAtIndex(0));
                return wid->getValue();
            }
            return 0;
        }

	public:
		uint32_t			fid;				//阵型ID
        std::string         name;               //名字
		cocos2d::CCArray	*posInfoArray;		//位置信息
	};

	//弟子属性信息
    class WarriorPropInfo :public ValueObjcet
    {
    public:
        WarriorPropInfo()
        :wid(0)
        ,grade(0)
        ,hp(0)
        ,def(0)
		,att(0)
		,speed(0)
		,block(0)
		,crit(0)
		,dodge(0)
		,fight(0)
		,uplv(0)
		,exp(0)
		,lv(1)
		,sexp(0)
		,slv(1)
        ,dressId(0)
        ,m_pBaseInfo(NULL)
        {
            
        };
        
        CC_SYNTHESIZE_READONLY(warrior::WarriorBaseInfo*, m_pBaseInfo, BaseInfo);
        
        friend class FormationInfo;
        friend class ::OtherFormationModel;
        
        CREATE_FUNC(WarriorPropInfo);
        D_INIT_DEFAULT(ValueObjcet);
       
        uint32_t getAvatarId();

	public:
        uint32_t	wid;				//弟子id
        uint32_t	grade;				//弟子实际品阶
        uint32_t	hp;					//血
        uint32_t	def;				//防
		uint32_t	att;				//攻
		uint32_t	speed;				//速
		uint32_t	block;				//格挡
		uint32_t	crit;				//暴击
		uint32_t	dodge;				//躲闪
		uint32_t	fight;				//战斗力
       
		uint16_t	uplv;				//突破等级
		uint32_t    exp;				//经验
		uint16_t	lv;					//等级
		uint32_t    sexp;				//技能经验
		uint16_t	slv;				//技能等级
        uint32_t    dressId;            //皮肤编号
        
        /* 是否等级到达限制*/
        bool checkLevelLimit();
        
        /*获取弟子的下一等级*/
        uint16_t getNextLv()
        {
            return checkLevelLimit()?lv:lv+1;
        }

    };
    
    class FormationInfo:public ValueObjcet
    {
    public:
        FormationInfo()
        :m_positions(NULL)
        ,m_equipInfoMap(NULL)
        ,m_warPropInfoMap(NULL)
        ,m_nUpdateTime(0)
        ,m_nRobotLevel(0)
        ,m_bIsRobot(false)
        ,m_nId(0)
		,m_pGundamPositions(NULL)
        {
            
        }
        virtual ~FormationInfo()
        {
            CC_SAFE_RELEASE(m_positions);
            CC_SAFE_RELEASE(m_equipInfoMap);
            CC_SAFE_RELEASE(m_warPropInfoMap);
			CC_SAFE_RELEASE(m_pGundamPositions);
        }
        
        CREATE_FUNC(FormationInfo);
        
        bool init()
        {
            if(!ValueObjcet::init()) return false;
            m_positions = cocos2d::CCArray::create();
            m_equipInfoMap = cocos2d::CCDictionary::create();
            m_warPropInfoMap = cocos2d::CCDictionary::create();
			m_pGundamPositions = cocos2d::CCArray::create();
            CC_SAFE_RETAIN(m_positions);
            CC_SAFE_RETAIN(m_equipInfoMap);
            CC_SAFE_RETAIN(m_warPropInfoMap);
			CC_SAFE_RETAIN(m_pGundamPositions);
            return true;
        }
        
        int8_t getSpecialSkillLv(skill::Skill* pSkill);
        
        bool checkFateActivated(warrior::FateInfo *pFateInfo);
        
        friend class ::OtherFormationModel;
        
        //获得阵容单位数
        uint32_t getPositionsCount()
        {
            return m_positions->count();
        };
        
        //获取位置信息
        formation::PositionInfo * getPositionInfoByIndex(uint8_t index);
        
        
        //根据装备ID获得装备信息
        equip::EquipCurInfo * getEquipInfoById(uint32_t id);
        
        //根据位置ID以及类型获得装备信息
        equip::EquipCurInfo * getEquipInfoByPidAndType(uint32_t pid,equip::EquipType eType);
        
        
        //获取弟子属性
        WarriorPropInfo* getWarriorPropInfoByWid(uint32_t wid);
        
        //获得队长wid
        uint32_t getFormationLeader();
        //获取阵容总战斗值
        int  getFormationTotalForce();
        
        bool isTimeOut(); //是否过期
        
        CC_SYNTHESIZE_READONLY(uint32_t, m_nId, ID);
        CC_SYNTHESIZE_READONLY(bool, m_bIsRobot, IsRobot);
        
    private:
        void parseFomations(const Json::Value &val);
        void parseWarriors(const Json::Value &val);
        void parseEquips(const Json::Value &val);
        void parseWarriorProps(const Json::Value &val);
		void parseGundamFomations(const Json::Value &val);
        gundam_engineer_formation::GundamEngineerPosInfo* getGundamPosInfoByWid( uint32_t wid);
        
        WarriorPropInfo * parseWarriorProp(const Json::Value &val, WarriorPropInfo * pProp = NULL);
        
        equip::EquipCurInfo * parseEquip(const Json::Value &val, equip::EquipCurInfo * pEquip = NULL);
        WarriorPropInfo * parseWarrior(const Json::Value &val, WarriorPropInfo * pWarrior = NULL);
        formation::PositionInfo * parseFomation(const Json::Value &val,formation::PositionInfo * pPos=NULL);
        void sortPositions();
        
    private:
        cocos2d::CCArray			*m_positions;				//位置信息
        cocos2d::CCDictionary		*m_equipInfoMap;			//装备信息
        cocos2d::CCDictionary		*m_warPropInfoMap;			//弟子属性
        time_t                      m_nUpdateTime;              //更新时间
        uint32_t                    m_nRobotLevel;              //如果是机器人振兴 保存等级信息
		cocos2d::CCArray *			m_pGundamPositions ;		//小伙伴信息
    };
    
}


class OtherFormationModel : public Model
{
	CC_SYNTHESIZE(uint32_t, m_iCurrentWarriorId, CurrentWarriorId);
public:
    OtherFormationModel();
    virtual ~OtherFormationModel();
    virtual bool init();

    void parseOtherFomations(const Json::Value &val,const time_t &updatedTime);

    
    //根据阵容ID获取机器人阵容信息
    otherFormation::RobotFormationInfo* getRobotFormationByFid(uint32_t fid);
    
    otherFormation::FormationInfo* getForamtionInfoByUid(uint32_t id);
	//修改方法，rank200以内的机器人，阵容中英雄等级的算法和一般的不一样！
    otherFormation::FormationInfo* getRobotFormationInfoByFid(uint32_t id,uint32_t level , unsigned int m_rank);
    otherFormation::FormationInfo* getBattleOtherForamtionInfo();
private:

	//读取pvp机器人阵容信息
	void loadPvpFormationData(const CSVDecoder::CSV &val);
    
private:
	cocos2d::CCDictionary		*m_pRobotsMap;      //pvp机器人阵容基本信息
    cocos2d::CCDictionary		*m_pRobotFormationInfoMap;	//pvp机器人阵容详细信息
    cocos2d::CCDictionary       *m_pUserFormationMap;       //其他玩家阵容信息

};



#endif /* defined(__QSMY__OtherFormationModel__) */
