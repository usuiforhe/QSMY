//
//  SkillModel.h
//  技能信息
//
//  Created by wanghejun on 13-3-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SkillModel__
#define __QSMY__SkillModel__

#include "Model.h"
#include "../AppMacros.h"
#include "utils/SystemUtil.h"
#include "../managers/ResourceManager.h"


class SkillModel;


namespace skill
{
    
    enum SkillGrade //技能品质
    {
		kSkillGradeWhite,
		kSkillGradeGreen,
		kSkillGradeBlue,
        kSkillGradePurple,
        kSkillGradeRed,
        
    };
    
    
    //绑定UI
    enum SkillType
    {
        kSkillDmg = 0,      //skill作用对象，0伤害，1治疗
        kSkillHeal,
        kSkillTypes         //总数
    };
    
    
	/*技能升级经验表-本地配置*/
	class SkillUpInfo:public ValueObjcet
	{
	public:
        CREATE_FUNC(SkillUpInfo);
        D_INIT_DEFAULT(ValueObjcet);
		SkillGrade grade;					//品质
		uint16_t lv;					//等级
		uint32_t exp;					//升级所需经验
	};
    
	/* 技能攻击范围配置表*/
	class SkillZoneInfo:public ValueObjcet
	{
	public:
        CREATE_FUNC(SkillZoneInfo);
        D_INIT_DEFAULT(ValueObjcet);
		uint8_t zid;			//id
		std::string desc;		//技能攻击范围描述
	};
    
    
    class Skill:public ValueObjcet
    {
        //CC_SYNTHESIZE_READONLY(uint32_t, sid, Sid);
        
    public:
        
        CREATE_FUNC(Skill);
        D_INIT_DEFAULT(ValueObjcet);
        
        uint32_t sid;			//技能ID
        std::string name;		//技能名
		SkillGrade	grade;		//品阶 红紫蓝绿
		SkillType	type;			//类型 攻击型 治疗型 吸血型
		std::string desc;		//技能描述
        std::string story;      //技能故事描述
		uint32_t wid1;			//弟子ID1
		uint32_t wid2;			//弟子ID2
		uint32_t wid3;			//弟子ID3
		uint8_t	 zone;			//攻击范围ID
		float	 attackRatio;	//攻击系数
        bool have_resource;     //是否有战场资源（没有使用默认资源）
        std::string trigger_probability; //触发概率
        float grow;				//技能每升一级增加的伤害系数

        bool    m_bHighQuality;
		CC_SYNTHESIZE_READONLY(uint8_t, m_uSkillFlip, SkillFlip);
		CC_SYNTHESIZE_READONLY(uint8_t, m_uHitFlip, HitFlip);
		
        
        
        bool getHighQuality()
        {
            return SystemUtil::isNBDevice() || m_bHighQuality;
        }
        
        const uint32_t getSid()
        {
            return sid;
        }
        
        Skill()
        :name("")
        ,sid(0)
        ,grade(kSkillGradeWhite)
        ,type(kSkillDmg)
        ,desc("")
        ,story("")
        ,wid1(0)
        ,wid2(0)
        ,wid3(0)
        ,zone(0)
		,attackRatio(0.0f)
        ,have_resource(false)
        ,m_bHighQuality(false)
        ,res_id(0)
        ,skeletonName("")
		,m_uSkillFlip(0)
		,m_uHitFlip(0)
        {
            
        }
        
        bool isSpecial();
        
        virtual ~Skill()
        {
        }
        friend class ::SkillModel;
        
        const char* getSkillName()
        {
            return name.c_str();
        }
        
        const char* getSkeletonName()
        {
            if(skeletonName.compare("")==0) skeletonName = cocos2d::CCString::createWithFormat("SKILL_%d",getResId())->getCString();
            return skeletonName.c_str();
        }
        
        uint32_t getResId()
        {
            return have_resource?res_id:DEFAULT_SKILL;
        }
        
        /**
         *	@brief	技能效果
         *
         *	@param 	slv 	技能等级
         *
         *	@return
         */
        float getAttackRatio(uint32_t slv);
        
        
        float getForceRatio(uint32_t slv);

    private:
        uint32_t    res_id;
        std::string skeletonName;

};


class SkillColorConfig:public ValueObjcet
{
public:
    CREATE_FUNC(SkillColorConfig);
    D_INIT_DEFAULT(ValueObjcet);
    //uint32_t top;
    //uint32_t bottom;
    cocos2d::ccColor3B top;             //渐变色上
    cocos2d::ccColor3B bottom;          //渐变色下
    cocos2d::ccColor3B color;           //纯色
};

}


class SkillModel : public Model
{
    typedef cocos2d::CCDictionary SkillMap;//skill::Skill*
	typedef cocos2d::CCDictionary SkillZoneMap;//skill::SkillZoneInfo*
    typedef cocos2d::CCArray SkillUpInfoVec;//skill::SkillUpInfo*
    typedef cocos2d::CCDictionary SkillColorMap;//skill::SkillGrade,skill::SkillColorConfig*
    
public:
    
    SkillModel();
    virtual ~SkillModel();
    virtual bool init();
    
    skill::Skill* getSkill(uint32_t sid)
	{
		return (skill::Skill*)m_Skills->objectForKey(sid);
	}
    
	/**
	 *	@brief	获取组合技能等级
	 *
	 *	@param 	pSkillInfo 	技能对象
	 *
	 *	@return	技能等级  -1 未激活
	 */
	int8_t getSpecialSkillLv(skill::Skill* pSkillInfo);
    
    
    skill::SkillColorConfig* getSkillColorConfig(skill::SkillGrade grade)
    {
        return (skill::SkillColorConfig*)m_SkillColorMap->objectForKey(grade);
    }
    
    cocos2d::ccColor3B getSkillColor(skill::SkillGrade grade)
    {
        skill::SkillColorConfig* pConfig = getSkillColorConfig(grade);
        return pConfig?pConfig->color:cocos2d::ccc3(0,0,0);
    }
    
	skill::SkillZoneInfo* getSkillZoneInfo(uint8_t zid)
	{
        return (skill::SkillZoneInfo*)m_SkillZoneMap->objectForKey(zid);
	}
    
	//获得技能升级所需经验 grade-技能品阶  lv-当前等级
	uint32_t getSkillUpExp(uint16_t grade, uint16_t lv);
	uint16_t getSkillLevelByExp(uint32_t skillID, uint32_t exp);
    //获得技能升级所消耗的总和
	uint32_t getSkillUpTotalAmount(uint16_t grade, uint16_t lv);
private:
    
    void loadData();
    void parseInfo(const CSVDecoder::CSV &val);
	void loadUpExpData(const CSVDecoder::CSV &val);
    void loadColorConfig(const CSVDecoder::CSV &val);
	void loadZoneConfig(const CSVDecoder::CSV &val);
    
private:
    SkillMap *m_Skills;
	SkillUpInfoVec *m_SkillUpInfoVec;
    SkillColorMap *m_SkillColorMap;
	SkillZoneMap *m_SkillZoneMap;
  
public:
	void getSpecialSkills(cocos2d::CCArray *skillArray);
};

#endif /* defined(__QSMY__SkillModel__) */
