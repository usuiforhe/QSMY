//
//  WarriorModel.h
//  弟子
//  Created by cll on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__WarriorModel__
#define __QSMY__WarriorModel__
#include "Model.h"
#include "AvatarModel.h"
#include "utils/SystemUtil.h"
#include "managers/ResourceManager.h"
#include "FormationModel.h"

#define  D_EVENT_PICTURECOLLECT_UPDATE "update picturecollect"

#define  D_EVENT_WARRIORLIST_UPDATE	"update warriorList"
#define	 D_EVENT_WARRIORINFO_UPDATE	"update warriorInfo"

#define  D_EVENT_SOULLIST_UPDATE "update soulList"
#define  D_EVENT_SOULINFO_UPDATE "update soulInfo"

#define  D_EVENT_WARRIOR_FINISH_TRANSFORCE "finish transForce"

#define  D_EVENT_WARRIOR_FINISH_RECRUIT "finish recruit"


#define  K_WARRIOR_DEV_ONCE_MAX	 (D_CONFIG_UINT_FOR_KEY(WARRIOR_DEV_ONCE_MAX))		//弟子每次突破最大次数
#define  K_WARRIOR_DEV_COST      (D_CONFIG_UINT_FOR_KEY(WARRIOR_DEV_COST))       //精心培养需要点券

#define  WARRIOR_DEV_CANCEL		3		//放弃培养
#define  WARRIOR_DEV_CONFIRM	2		//确认培养

#define  WARRIOR_TRANSFORCE_NORMAL		1		//普通传功
#define  WARRIOR_TRANSFORCE_ADV			2		//高级传功

#define WARRIOR_GRADE_NUM 4

class WarriorModel;

namespace warrior {
    
	static inline uint32_t TransferExpNormal(uint32_t exp)
	{
		return exp * 0.8;
	}

	static inline uint32_t TransferExpGreat(uint32_t exp)
	{
		return exp;
	}

	static inline uint32_t ReturnItemNormal(uint32_t num)
	{
		return num * 0.4;
	}

	static inline uint32_t ReturnItemGreat(uint32_t num)
	{
		return num * 0.5;
	}

	//弟子品阶-颜色区分
	enum WarriorGrade
	{
        kWarGradeNone = 0,     //无品质
		kWarGradeGreen = 1,    //绿色
		kWarGradeBlue,     //蓝色
		kWarGradePurple,   //紫色
		kWarGradeRed,	   //红色
	};
	//ENUM COLOR FOR THE WARRIORHANDBOOK LIST FILTER
	enum WarriorHandBookFilter
	{
		kWarFilterNone = 0,  //NO COLOR
		kWarFilterGreen = 1, //ALL
		kWarFilterBlue,      //NEW
		kWarFilterPurple,    //JAPAN
		kWarFilterOrange,    //SHENJIE
		kWarFilterRed,       //MAJIC
		kWarFilterCount,     //counts of filter type
	};
    //弟子属性类型
    enum WarriorPropType
	{
		kPropTypeAttak,
		kPropTypeDefence,
		kPropTypeHp,
		kPropTypeSpeed,
		kPropTypeAll,
	};
    
	//英雄排序类型
    enum WarriorSortType
	{
		kSortTypeNone = 0,  
		kSortTypeLvl,      //等级
		kSortTypeGrade,    //品质
		kSortTypeZizhi,    //资质
		kSortTypeFight,    //战斗力
		kSortTypeCount,    //排序数量 
	};

	enum WarriorFateType
	{
		kFateTypeWarrior=1,		//弟子与弟子缘
		kFateTypeEquip=2,		//弟子与装备缘
	};

	//天赋， 只列出部分程序需要使用的天赋
	//天赋ID对应配置表中的ID，需手动设置
	enum WarriorTalentType
	{
		kTalentTypeHP = 1,
		kTalentTypeAttack = 4,
		kTalentTypeDefence = 5,
		kTalentTypeAttackBuff = 14,
		kTalentTypeDebuffImmunity = 15, //免疫debuff

		kTalentTypeOther = 9999,
	};

    enum WarriorLocationType
    {
        kLocationTypeT = 1,     //弟子类型 T
        kLocationTypeDPS,       //弟子类型    DPS
		kLocationTypeAssist,	 //辅助
    };
    
    enum WarriorIconType
	{
		kIconTypeWarriorList = 1, //弟子列表头像
		kIconTypeChangeFormation = 2, //更换阵容界面
		kIconTypeFull = 3,			//弟子全身像
		kIconTypeMax
	};

    enum WarriorUpgradeCriticalType
    {
        kUpgradeCriticalType_1 = 1,
        kUpgradeCriticalType_2 = 2,
        kUpgradeCriticalType_3 = 3,
    };

	enum WarriorUpErrorType
	{
		kUpErrorNone = 1,
		kUpErrorWarriorInfo,
		kUpErrorCoinNotEnough,
		kUpErrorItemNotEnough,
		kUpErrorSoulNotEnough,
		kUpErrorWarriorLevelLimit,
		kUpErrorSoulInfo,
		kUpErrorUnknow
	};

	//英雄量化信息
	class BurnHeroInfo:public ValueObjcet
	{
	public:
		BurnHeroInfo()
			:heroInfo(NULL)
			,heroType(0)
		{
			
		}
		virtual ~BurnHeroInfo()
		{
			CC_SAFE_RELEASE(heroInfo);
		}
		CREATE_FUNC(BurnHeroInfo);
		D_INIT_DEFAULT(ValueObjcet);
		ValueObjcet *heroInfo;
		uint8_t heroType;
	};
    
    class WarriorDialogInfo:public ValueObjcet
    {
    public:
        WarriorDialogInfo()
        :warriorId(0)
        ,m_pDialogList(NULL)
        {
            
        };
        
        virtual ~WarriorDialogInfo()
        {
            CC_SAFE_RELEASE(m_pDialogList);
        };
        
        CREATE_FUNC(WarriorDialogInfo);
        
        virtual bool init()
        {
            m_pDialogList = cocos2d::CCArray::create();
            m_pDialogList->retain();
            
            return ValueObjcet::init();
        }
        
        cocos2d::CCString * getDialog();
        
        void addDialog(cocos2d::CCString * m_pDialog)
        {
            m_pDialogList->addObject(m_pDialog);
        }
        
    public:
        uint32_t warriorId;
        
    private:
        cocos2d::CCArray * m_pDialogList;
    };


	class TalentInfo : public ValueObjcet
	{
	public:
		TalentInfo()
			: talent_id(0),
			name(""),
			_description(""),
			type(0),
			buff_id(0),
			warriorType(0),
			attributeType(0),
			_value(0),
			probability(0.0f)
		{

		}

		CREATE_FUNC(TalentInfo);

		uint32_t		talent_id;	//天赋Id
		std::string    name;		//名字
		std::string		_description;	//描述
		uint32_t		type;				//类型
		uint32_t		buff_id;		
		uint32_t		warriorType;
		uint32_t		attributeType;
		uint32_t		_value;
		uint32_t		probability;
	};

	//缘信息
	class FateInfo:public ValueObjcet
	{
	public:

		FateInfo():fate_id(0),type(kFateTypeWarrior),name(""),owner_id(0),
			member_ids(NULL),equip_id(0),add_defence(0),add_attack(0),add_hp(0){};

		~FateInfo()
		{
			CC_SAFE_RELEASE(member_ids);
		};

		CREATE_FUNC(FateInfo);

		bool init()
		{
            if(!ValueObjcet::init()) return false;
			member_ids = cocos2d::CCArray::create();
			CC_SAFE_RETAIN(member_ids);
			return true;
		};


		uint32_t			fate_id;		//缘ID
		WarriorFateType		type;			//类型
		std::string			name;			//名
		uint32_t			owner_id;		//拥有该缘弟子ID
		cocos2d::CCArray	*member_ids;	//有缘弟子
		uint32_t			equip_id;		//有缘装备
		uint32_t			add_attack;		//加攻
		uint32_t			add_defence;	//加防
		uint32_t			add_hp;			//加血

	};
    
    
	/*培养属性加成*/
	class PotBuff:public ValueObjcet
	{
	public:
		enum PotbuffType //绑定ui tag
		{
			kTypeNone,
			kTypeNormal,			//普通培养
			kTypeSpecial,			//精心培养
			kType10Normal,			//普通培养10次
			kType10Special,			//精心培养10次
		};
        
	public:
		PotBuff():wid(0),hp(0),def(0),att(0),cost(0),type(kTypeNormal){};
	public:
		uint32_t wid;						//对应弟子ID
		int hp;                             //生命加成
		int att;                            //攻击加成
		int def;                            //防御加成
        int cost;                           //需求潜力
		PotbuffType  type;					//培养类型
        
        CREATE_FUNC(PotBuff);
        D_INIT_DEFAULT(ValueObjcet);
	};


	/*弟子基本详细信息-本地配置*/
    class WarriorBaseInfo:public ValueObjcet
    {
	public:
		WarriorBaseInfo()
        :m_pFateArray(NULL)
        ,unit_attack_dmg_delay(0.f)
        ,warrior_id(0)
        ,real_grade(0)
        ,basic_hp(0)
        ,basic_attack_add(0.f)
        //,m_bHighQuality(false)
        ,have_resource(false)
        ,res_id(0)
		,m_uFlipRes(0)
		,m_uFlipDialogRes(0)
		{
		};

		virtual ~WarriorBaseInfo()
		{
			CC_SAFE_RELEASE(m_pFateArray);

		};
		CREATE_FUNC(WarriorBaseInfo);
        friend class ::WarriorModel;

		bool init()
		{
            if(!ValueObjcet::init()) return false;
			m_pFateArray = cocos2d::CCArray::create();
			CC_SAFE_RETAIN(m_pFateArray);

			return true;
		};


        
//        bool getHighQuality()
//        {
//            return SystemUtil::isNBDevice() || m_bHighQuality;
//        }
        
        unsigned int getResId()
        {
            return have_resource?res_id:DEFAULT_UNIT;
        }
        
       
        
        
		CC_SYNTHESIZE_READONLY(unsigned int, warrior_id,WarriorId);                         //本地配置弟子id
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, warrior_name, WarriorName);			//弟子名
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string,	warrior_desc, WarriorDesc);			//弟子描述	界面暂时未用到
		CC_SYNTHESIZE_READONLY(WarriorGrade, color_grade,ColorGrade);                       //显示品质	-- 颜色
		CC_SYNTHESIZE_READONLY(unsigned char, real_grade,RealGrade);						//实际品质
		CC_SYNTHESIZE_READONLY(unsigned int, basic_hp, BasicHp);                            //血量
		CC_SYNTHESIZE_READONLY(float, basic_hp_add,BasicHpAdd);                             //血量成长
		CC_SYNTHESIZE_READONLY(unsigned int, basic_attack, BasicAttack);					//攻击
		CC_SYNTHESIZE_READONLY(float, basic_attack_add, BasicAttackAdd);					//攻击成长
		CC_SYNTHESIZE_READONLY(unsigned int, basic_defence, BasicDefence);					//防御
		CC_SYNTHESIZE_READONLY(float, basic_defence_add,BasicDefenceAdd);                   //防御成长
		CC_SYNTHESIZE_READONLY(unsigned int, basic_speed, BasicSpeed);                      //速度
		CC_SYNTHESIZE_READONLY(WarriorLocationType,location,Location);                      //弟子位置
		CC_SYNTHESIZE_READONLY(unsigned int, skill_id, SkillId);                            //技能ID
		CC_SYNTHESIZE_READONLY(unsigned int, sp_skill_id,SpSkillId);                        //特殊技能ID

        CC_SYNTHESIZE_READONLY(bool , showInEquipFate,ShowInEquipFate);                     //是否在装备有缘列表显示
		CC_SYNTHESIZE_READONLY(cocos2d::CCArray	*,m_pFateArray,FateArray);                  //弟子拥有的缘
        CC_SYNTHESIZE_READONLY(unsigned int, quality,Quality);                              //资质
        CC_SYNTHESIZE_READONLY(float,unit_attack_dmg_delay,UnitAttackDmgDelay);    //普攻伤害delay(s)
		CC_SYNTHESIZE_READONLY(uint32_t,m_uFlipRes,FlipRes);    //需要翻转的时候用的资源
		CC_SYNTHESIZE_READONLY(uint32_t,m_uFlipDialogRes,FlipDialogRes);    //战斗对话需要翻转的时候用的资源
        
    private:
        bool        have_resource;
        uint32_t    res_id;
        //bool        m_bHighQuality;
        
    };

	/*
	TU JIAN 本地配置
	*/
	class WarriorHandBookInfo:public ValueObjcet
	{
	public:
		static int m_nCount;
		friend class WarriorModel;
		//friend class PictureCollect;
		WarriorHandBookInfo():warrior_id(0),
		warrior_name(),
		color_grade(),
		quality(0),
		locationg(0),
		New(0),
		area(0),
		num_new(0),
		isLighted(false)
		{

		};
		virtual ~WarriorHandBookInfo()
		{
		};
		CREATE_FUNC(WarriorHandBookInfo);
	public:
		/*unsigned int warrior_id;                        //本地配置弟子id
		std::string warrior_name;		   //弟子名
		WarriorGrade color_grade;                     //显示品质-- 颜色
		unsigned int quality;                              //资质
		unsigned int locationg;                          //locationg 
        unsigned int New;                                      //new                
		unsigned int area;   */
        friend class ::WarriorModel;
		CC_SYNTHESIZE(unsigned int,warrior_id,Warrior_id);
		CC_SYNTHESIZE(std::string,warrior_name,Warrior_name);
		CC_SYNTHESIZE(WarriorGrade,color_grade,Color_grade);
		CC_SYNTHESIZE(unsigned int,quality,Quality);
		CC_SYNTHESIZE(unsigned int,locationg,Locationg);
		CC_SYNTHESIZE(unsigned int,New,New);
		CC_SYNTHESIZE(unsigned int,area,Area);
		CC_SYNTHESIZE(unsigned int,num_new,Num_new);
		CC_SYNTHESIZE(bool,isLighted,IsLighted);
	};

	/* 弟子突破所需魂魄表-本地配置*/
	class WarriorUpSoul:public ValueObjcet
	{
	public:
        CREATE_FUNC(WarriorUpSoul);
        D_INIT_DEFAULT(ValueObjcet);
        
        friend class ::WarriorModel;
        
		CC_SYNTHESIZE_READONLY(unsigned short, upLevel, UpLevel);				//突破等级
		CC_SYNTHESIZE_READONLY(unsigned short, bGradeSoul, GradeSoul);			//蓝色需要魂魄
		CC_SYNTHESIZE_READONLY(unsigned short, pGradeSoul, PGradeSoul);			//紫色需要魂魄
		CC_SYNTHESIZE_READONLY(unsigned short, rGradeSoul, RGradeSoul);			//红色所需魂魄

		CC_SYNTHESIZE_READONLY(unsigned short, bGradeItem, GradeItem);			//消耗进阶丹
		CC_SYNTHESIZE_READONLY(unsigned short, pGradeItem, PGradeItem);			
		CC_SYNTHESIZE_READONLY(unsigned short, rGradeItem, RGradeItem);			

		CC_SYNTHESIZE_READONLY(unsigned int, bGradeCoin, GradeCoin);			//消耗金币
		CC_SYNTHESIZE_READONLY(unsigned int, pGradeCoin, PGradeCoin);			
		CC_SYNTHESIZE_READONLY(unsigned int, rGradeCoin, RGradeCoin);			

		CC_SYNTHESIZE_READONLY(unsigned int , levelLimit, LevelLimit);//等级限制

		int getUpItemCount(WarriorGrade grade)
		{
			switch (grade)
			{
			case kWarGradeBlue:
				return bGradeItem;
			case kWarGradePurple:
				return pGradeItem;
			case kWarGradeRed:
				return rGradeItem;
			}
			
			return 0;
		}

		int getUpCoinCount(WarriorGrade grade)
		{
			switch (grade)
			{
			case kWarGradeBlue:
				return bGradeCoin;
			case kWarGradePurple:
				return pGradeCoin;
			case kWarGradeRed:
				return rGradeCoin;
			}

			return 0;
		}
	};


	/* 弟子品阶成长配置表 -本地配置*/
	class WarriorGrowInfo:public ValueObjcet
	{
	public:
        CREATE_FUNC(WarriorGrowInfo);
        D_INIT_DEFAULT(ValueObjcet);
        
        friend class ::WarriorModel;
        
		CC_SYNTHESIZE_READONLY(unsigned short, grade, Grade);                                   //弟子品质
		CC_SYNTHESIZE_READONLY(unsigned short, color, Color);                                   //弟子等级
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(cocos2d::ccColor3B, colorValue, ColorValue);         //品质对应的颜色值
		CC_SYNTHESIZE_READONLY(unsigned short, rctSoul, RctSoul);                               //招募所需魂魄
		CC_SYNTHESIZE_READONLY(float,growRate,GrowRate);                                        //成长率
		CC_SYNTHESIZE_READONLY(unsigned short, soulValue, SoulValue);                           //魂魄价值

	};



	/*弟子当前信息*/
	class WarriorCurInfo:public ValueObjcet
	{
    private:
        static WarriorCurInfo* create();
        
	public:
		WarriorCurInfo()
        :wid(0)
        ,warriorLv(1)
        ,warriorExp(0)
        ,warriorUpLv(0)
        ,sexp(0)
        ,slv(1)
        ,hp(0)
        ,attack(0)
        ,defence(0)
        ,hp_add(0)
        ,attack_add(0)
        ,defence_add(0)
		,hp_talentAdd(0)
		,attack_talentAdd(0)
		,defence_talentAdd(0)
        ,potential(0)
        ,costedPotential(0)
        ,baseInfo(NULL)
		,m_iDevelopItemNum(0)
        ,m_nDressId(0)
        ,skeletonName("")
		{

		}
        
        unsigned int getAvatarId()
        {
            return m_nDressId!=0?D_FIX_DRESS_ID(m_nDressId)/* x1000 fix跟弟子id的冲突*/:getBaseInfo()->getResId();
        }
        
        static WarriorCurInfo* create(unsigned int wid);
        static WarriorCurInfo* create(WarriorBaseInfo*);
        
        D_INIT_DEFAULT(ValueObjcet);
        friend class ::WarriorModel;

		unsigned int getSoulCount();

		/* 重置弟子信息 -传功后初始化*/
		void resetWarriorCurInfo();
        //计算潜力
        void computePotential();
        /*计算攻防血*/
        void computeWarriorProps();
		/* 计算天赋增加的属性 */
		void computeTalentProps();
        /*计算弟子加成属性,含装备和缘属性，返回对应结果*/
        uint32_t computeWarriorFinalProps(WarriorPropType propType, formation::PositionInfo * _pPosInfo = NULL);
		uint32_t computeNextUpLevelWarriorProps(WarriorPropType propType);
        /*计算战斗力,默认按阵容位置计算装备的战斗力*/
        uint32_t computeWarriorForce(formation::PositionInfo * pPosInfo = NULL);
		uint32_t computeNextUpLevelWarriorForce();
        
        //计算英雄下一等级的潜力
        unsigned int getWarriorNextUpLevelPotential();

		/* 提升突破等级*/
		void addUpLevel();
		/* 增加经验*/
		void addExp(int exp, float fDelayTime=0.f,bool* isLevelUp=NULL);
        /* 判断弟子是否可以突破*/
        bool isPromoteable()
        {
            return baseInfo->getColorGrade() >= warrior::kWarGradeBlue;
        };
        
        //获取弟子进阶需要的等级
        unsigned int getWarriorLevelForPromotion();
        //弟子等级是否满足限制
        bool checkWarriorLevelForPromotion();
        
        /* 技能是否升到最大级*/
		bool checkSkillLimit();
		/* 是否突破到最大级*/
		bool checkPromoteLimit();
        /* 是否等级到达限制*/
        bool checkLevelLimit();
        /* 是否等级到达最大值 */
        bool checkLevelMax();        
        /*是否到达可培养等级*/
        bool checkDevelopLevel();
        float getUpPercentCurrentLv();
        float getUpPercentNextLv();
        static float getUpPercent(signed char warriorUpLv);
        
        /*获取弟子的下一等级*/
        unsigned short getNextLv()
        {
            return checkLevelLimit()?warriorLv:warriorLv+1;
        }
        
        const char* getSkeletonName()
        {
            if(skeletonName.empty()) skeletonName = UNIT_SK_NAME(getAvatarId());
            return skeletonName.c_str();
        }
        
        /*脱下时装*/
        void unDress();

	public:
		CC_SYNTHESIZE_READONLY(unsigned short, warriorLv, WarriorLv);				//等级
		CC_SYNTHESIZE_READONLY(unsigned int, warriorExp, WarriorExp);			//经验
		CC_SYNTHESIZE_READONLY(unsigned char, warriorUpLv, WarriorUpLv);             //突破等级
		CC_SYNTHESIZE_READONLY(unsigned int, sexp, Sexp);					//天赋技能经验值
		CC_SYNTHESIZE_READONLY(unsigned char, slv, Slv);                     //天赋技能等级

		CC_SYNTHESIZE_READONLY(unsigned int, hp, Hp);					//当前生命
		CC_SYNTHESIZE_READONLY(unsigned int, attack, Attack);				//当前攻击
		CC_SYNTHESIZE_READONLY(unsigned int, defence, Defence);				//当前防御
		CC_SYNTHESIZE_READONLY(unsigned int, hp_add, HpAdd);				//生命培养加成
		CC_SYNTHESIZE_READONLY(unsigned int, attack_add, AttackAdd);			//攻击培养加成
		CC_SYNTHESIZE_READONLY(unsigned int, defence_add, DefenceAdd);			//防御培养加成

		CC_SYNTHESIZE_READONLY(unsigned int, hp_talentAdd, HpTalentAdd);			//生命天赋加成
		CC_SYNTHESIZE_READONLY(unsigned int, attack_talentAdd, AttackTalentAdd);			//攻击天赋加成
		CC_SYNTHESIZE_READONLY(unsigned int, defence_talentAdd, DefenceTalentAdd);			//防御天赋加成

		CC_SYNTHESIZE_READONLY(unsigned int, potential, Potential);				//潜力点
        
        CC_SYNTHESIZE_READONLY(unsigned int, costedPotential, CostedPotential);       //已消耗的潜力点

        CC_SYNTHESIZE_READONLY(unsigned int, wid, Wid);//本地配置ID
        CC_SYNTHESIZE_READONLY(WarriorBaseInfo*, baseInfo, BaseInfo);//base info

		CC_SYNTHESIZE_READONLY(uint32_t, m_iDevelopItemNum, DevelopItemNum);
        
        CC_SYNTHESIZE_READONLY(unsigned int, m_nDressId, DressId);  //皮肤
    
    private:
        std::string skeletonName;
	
    };


	/*魂魄信息*/
	class SoulInfo:public ValueObjcet
	{
	public:
		SoulInfo()
        :wid(0)
        ,num(0)
        ,hpts(0)
        ,baseInfo(NULL)
		{

		}
        
        static SoulInfo* create(unsigned int wid,unsigned int num);

        CREATE_FUNC(SoulInfo);
        D_INIT_DEFAULT(ValueObjcet);
        friend class ::WarriorModel;
	public:
		
		CC_SYNTHESIZE_READONLY(unsigned int,wid,Wid);						//对应弟子ID
		CC_SYNTHESIZE_READONLY(unsigned int,num,Num);						//数量
		CC_SYNTHESIZE_READONLY(unsigned int,hpts,Hpts);						//该魂魄的保护结束时间

		CC_SYNTHESIZE_READONLY(WarriorBaseInfo*, baseInfo, BaseInfo);//base info
	};
    
	/*弟子头像信息 */
	class IconsInfo : public ValueObjcet
	{
	public:
		IconsInfo()
			: origin(cocos2d::CCPointZero)
			, scale(1.0f)
			,maskId(0)
		{

		}

		static IconsInfo* create( std::string infoString );

		CREATE_FUNC(IconsInfo);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::WarriorModel;
		CC_SYNTHESIZE_READONLY(cocos2d::CCPoint, origin, _Origin); //在人物大图上的起点
		CC_SYNTHESIZE_READONLY(float, scale, _Scale);//使用时的缩放值
		CC_SYNTHESIZE_READONLY(unsigned int, maskId, MaskId);
	};
    
    class UpgradeItemInfo : public ValueObjcet
    {
    public:
        UpgradeItemInfo()
        :itemId(0)
        ,coinConsum(0)
        ,criticalTriggerValue(0)
		,experience(0)
        {};
        
        static UpgradeItemInfo* create(unsigned int _itemId, unsigned int _coinConsum, unsigned int _criticalTriggerValue, unsigned int _experience);
        
        CREATE_FUNC(UpgradeItemInfo);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::WarriorModel;
        
        CC_SYNTHESIZE_READONLY(unsigned int, itemId, ItemId);
        CC_SYNTHESIZE_READONLY(unsigned int, coinConsum, CoinConsum);
        CC_SYNTHESIZE_READONLY(unsigned int, criticalTriggerValue, CriticalTriggerValue);
		CC_SYNTHESIZE_READONLY(unsigned int, experience, Experience);
    };
    
    class UpgradeCriticalData : public ValueObjcet
    {
    public:
        UpgradeCriticalData()
        : type(kUpgradeCriticalType_1)
        ,factor(0.0f)
        ,probability(0.0f)
        {};
        
        static UpgradeCriticalData* create(unsigned int _type, float _factor, float _probability);
        
        CREATE_FUNC(UpgradeCriticalData);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::WarriorModel;
        
        CC_SYNTHESIZE_READONLY(unsigned int, type, Type);
        CC_SYNTHESIZE_READONLY(float, factor, Factor);
        CC_SYNTHESIZE(float, probability, Probability);
    };
}

class WarriorModel : public Model
{
	friend class PictureCollect;
public:
	static warrior::WarriorHandBookInfo* m_pWarriorHandBookInfo;
	static warrior::WarriorHandBookInfo* getWarriorHandBookInfo();
	//static int getPictureCount();
	typedef cocos2d::CCDictionary WarriorHandBookMap;//std::map<uint32_t, warrior::WarriorHandBookInfo*>
    typedef cocos2d::CCDictionary WarriorBaseMap;   //std::map<uint32_t, warrior::WarriorBaseInfo*>
	typedef cocos2d::CCDictionary PotBuffMap;       //std::map<uint32_t, warrior::PotBuff*>
	typedef cocos2d::CCDictionary WarriorGrowMap;   //std::map<uint16_t, warrior::WarriorGrowInfo*>
    typedef cocos2d::CCArray WarriorCurInfoVec;     //std::vector<warrior::WarriorCurInfo*>
	typedef cocos2d::CCArray WarriorUpInfoVec;      //std::vector<warrior::WarriorUpInfo*>
	typedef cocos2d::CCArray WarriorUpSoulVec;      //std::vector<warrior::WarriorUpSoul*>
	typedef cocos2d::CCArray SoulInfoVec;           //std::vector<warrior::SoulInfo*>
	typedef cocos2d::CCDictionary WarriorFateMap;
	typedef cocos2d::CCDictionary WarriorDialogMap;
	typedef cocos2d::CCDictionary WarriorIconInfoMap; //std::map< uint32_t, std::map<warrior::IconsInfo*, WarriorIconType> >
    typedef cocos2d::CCDictionary WarriorUpgradeItemMap;//std::map<uint_32, UpgradeItemInfo>
    typedef cocos2d::CCDictionary WarriorUpgradeAccumulationMap;//std::map<uint_32, uint_32>
    typedef cocos2d::CCDictionary WarriorUpgadeCriticalMap;//std::map<uint_32, uint_32>
    typedef cocos2d::CCDictionary TalentInfoMap;
	typedef cocos2d::CCDictionary	WarriorTalentMap; //std::map< uint_32, std::map<uint_32, CCInteger*> >英雄id，进阶等级，天赋id
    typedef cocos2d::CCArray    WarriorPotentialFactorVec;
public:
	WarriorModel();
    virtual ~WarriorModel();
    virtual bool init();
    
    /*穿上衣服*/
    void dressUp(avatar::AvatarInfo* pAvatar,bool showBusy = true);
    void dressUp(avatar::AvatarBaseInfo* pAvatar,bool showBusy = true);
    unsigned int getSoulCount(unsigned int wid);

	/* 从 WarriorMap 获取本地弟子详细基本信息*/
	warrior::WarriorBaseInfo* getWarriorBaseByWID(unsigned int wid);

	warrior::WarriorHandBookInfo* getWarriorHandBookInfoByWID(unsigned int wid);
    
    /* 获取弟子品阶对应的成长信息 */
    warrior::WarriorGrowInfo* getWarriorGrowInfoByGrade(unsigned char real_grade);
    /* 获取弟子品阶对应的成长信息 */
    warrior::WarriorGrowInfo* getWarriorGrowInfoByGrade(unsigned int wid);

	/* 获取弟子品阶对应的颜色 */
	unsigned short getWarriorGradeColorByWid(unsigned int wid);
    
    /* 获取弟子品阶对应的颜色值 */
    cocos2d::ccColor3B getWarriorColorByWid(unsigned int wid);
    
    /* 获取弟子品阶对应的颜色值 */
    cocos2d::ccColor3B getWarriorColorByGrade(unsigned char real_grade);
    /* 根据弟子颜色获得颜色值 */
    cocos2d::ccColor3B getWarriorColorByColorGrade(warrior::WarriorGrade grade);

	/* 从 WarriorCurInfoVec 获取弟子wid当前信息 */
	warrior::WarriorCurInfo * getWarriorCurInfoByWID(unsigned int wid);
    
    bool isWarriorExist(unsigned int wid);

    WarriorCurInfoVec* getWarriorVec()
    {
        return m_warCurInfoVec;
    };

	WarriorHandBookMap* getWarriorHandBookMap()
	{
		return m_warHandBookMap;
	}

	int getNewCount();

    /**
     *	@brief	获取没有出战的弟子
     *
     */
    cocos2d::CCArray* getWarriorsUnBattle(cocos2d::CCArray* pWarriors = NULL);
    
    bool isHasWarriorsUnBattle();

	void getWarriorsIgnoreWID(uint32_t wid, WarriorCurInfoVec &vec);

	/* 获取弟子突破所需魂魄 旧方法不再使用！！！*/
	unsigned int getWarriorUpSoul(warrior::WarriorCurInfo * pWarrior);

	/* 获取弟子突破所需物品信息（魂魄，金币，进阶丹） */
    warrior::WarriorUpSoul* getWarriorUpInfo(warrior::WarriorCurInfo * pWarrior);

	/* 获取英雄进阶消耗的总和（type1金币，type2进阶丹 , type3魂魄） 炼化收益预览使用*/
	unsigned int getWarriorUpTotalAmount(warrior::WarriorCurInfo * pWarrior , int type);

	/* 获取强化信息*/
	uint32_t getItemExp(uint32_t itemid);

    /**
     *	@brief	判断弟子是否可以立即突破
     *
     *	@param 	pWarrior 	弟子
     *
     *	@return	是否可以立即突破
     */
    warrior::WarriorUpErrorType canWarriorUpNow(warrior::WarriorCurInfo * pWarrior);

    //获取弟子升级需要的经验值
    int getWarriorUpExp(const int &level , const int &grade);
    //根据弟子经验获得等级
    int getWarriorLevelByExp(const int &exp , const int &grade, const int & currLevel = 1);
    //获取弟子升级需要的累积经验值
    int getWarriorAddExp(const int &level , const int &grade);
    //根据弟子当前获得的经验总值 计算弟子当前等级获得的经验
    int getWarriorCurExpByGetExp(const int &getExp , const int &level , const int &grade);
    //根据弟子等级获得潜力
    int getWarriorPotentialByLevel(const int &level , const int &upLevel, warrior::WarriorBaseInfo * pBaseInfo);

	const warrior::IconsInfo* getWarriorIconInfoByType(unsigned int wid, warrior::WarriorIconType type); 
    
	/* 解析网络下发弟子信息列表*/
	void parseWarriors(const Json::Value &val);
    
	/* 解析网络下发弟子信息*/
	void parseOneWarrior(const Json::Value &val,bool notifyListChanged = true);
    void parseWarriorHandBookData( const CSVDecoder::CSV &val);
	void refreshWarriorHandBookData();
    void addWarrior(warrior::WarriorCurInfo * pWarrior,bool notifyListChanged = true);
    void removeWarriors(cocos2d::CCArray *pWarriors);

	/*缘是否激活*/
    bool checkFateActivated(warrior::FateInfo *pFateInfo);


	SoulInfoVec *getSoulVec()
	{
		return m_soulInfoVec;
	};
	
	warrior::SoulInfo* getSoulInfoByWID(uint32_t wid);

	/* 解析网络下发弟子魂魄列表*/
	void parseSouls(const Json::Value &val);
    
    /* 更新魂魄列表 */
    void updateSouls(const Json::Value &val);
    

	warrior::WarriorCurInfo * parseWarrior(const Json::Value &val, warrior::WarriorCurInfo * pWarrior = NULL);	

	warrior::SoulInfo * parseSoul(const Json::Value &val, warrior::SoulInfo * pSoul = NULL);	

	/* 解析pot buff*/
	void parsePotBuffs(const Json::Value &val);
	void parseOnePotBuff(const Json::Value &val);
	
	/*接受或放弃培养*/
	void dealWithPotBuff(warrior::WarriorCurInfo * pWarriorInfo, const bool &accepted = true);

	warrior::PotBuff* getWarriorPotBuff(warrior::WarriorCurInfo * pWarriorInfo);

    /* 解析网络下发魂魄信息*/
	void parseOneSoul(const Json::Value &val,bool notifyListChanged = true);
    
	/* 掉落弟子 魂魄*/
    void addSoul(warrior::SoulInfo * pSoul,bool notifyListChanged = true);
	void removeSoul(warrior::SoulInfo * pSoul , const bool &notifyListChanged=true);
	void removeSouls(cocos2d::CCArray *pSouls);
    
    void checkWarriorOrder(warrior::WarriorCurInfo * pWarrior,bool notify = true);
    
    
    
    //获取缘信息
    void getWarriorFatesByWIDAndEquipId(uint32_t wid,uint32_t eid,cocos2d::CCArray * fateArray);
	void getWarriorFatesByWID(uint32_t wid,cocos2d::CCArray * fateArray);
    void getWarriorFatesByEquipId(uint32_t eid,cocos2d::CCArray * fateArray);
    
    //获取英雄强化信息
    void getWarriorUpgradeItemsInfo(cocos2d::CCArray* itemArray);

	warrior::UpgradeItemInfo* getWarriorUpgradeItemInfoById(uint32_t _id);

	//获取天赋信息
	warrior::TalentInfo* getTalentInfoById(uint32_t talentId);
	cocos2d::CCDictionary* getWarriorTalentsDic(uint32_t warriorId);
    
    //获取弟子对话
    warrior::WarriorDialogInfo * getWarriorDialogInfo(uint32_t warriorId);
    
    /**
     *	@brief	计算弟子成长属性
     *
     *	@param 	real_grade 	真实品质
     *	@param 	basic 	基础属性
     *	@param 	basic_add 	基础成长
     *	@param 	level 	级别
     *	@param 	upLevel 	强化等级
	 *	@wid 英雄id（new）
     *
     *	@return	成长之后的属性
     */
    static uint32_t computeBasicAddProps(uint8_t wid , uint8_t real_grade,uint32_t basic, float basic_add,uint32_t level,uint8_t upLevel , warrior::WarriorTalentType type);
    
    
    static uint32_t computeWarriorForce(float hp,float attack,float defence,float speed,float dodge,float block,float critical,float skillRatio);
    
    /*判断该弟子等级是否到达最大值*/
    bool checkLevelLimit(unsigned short lv);
    
    bool isMyWarrior(warrior::WarriorCurInfo* pWarrior);

private:

	void loadWarriorLocalData();

	void parseWarriorBaseData(const CSVDecoder::CSV &val);

	void parseWarriorUpSoulData(const CSVDecoder::CSV &val);
    
	void parseWarriorGrowData(const CSVDecoder::CSV &val);
    
	void parseWarriorLevelExpData(const CSVDecoder::CSV &val);

	void parseWarriorFateInfoData(const CSVDecoder::CSV &val);
    
	void parseWarriorDialogInfoData(const CSVDecoder::CSV &val);

	void parseWarriorIconInfoData(const CSVDecoder::CSV &val);
    
    void parseWarriorUpgradeItemsData(const CSVDecoder::CSV &val);
    
    void parseWarriorUpgradeCriticalData(const CSVDecoder::CSV &val);
    
    void parseWarriorUpgradeItemAccumulation(const CSVDecoder::CSV &val);
	
	void parseTalentInfoData(const CSVDecoder::CSV &val);

	void parseWarriorTalentData(const CSVDecoder::CSV &val);
    
    void parseWarriorPotentialByUpLevel(const CSVDecoder::CSV &val);

	warrior::PotBuff * parsePotBuff(const Json::Value &val, warrior::PotBuff * pPotBuff= NULL);	

	void addWarriorInorder(warrior::WarriorCurInfo * pWarrior);
	void addSoulInorder(warrior::SoulInfo * pSoul);

    
    void notifySoulInfoUpdate(warrior::SoulInfo* pInfo);

private:

	WarriorHandBookMap *m_warHandBookMap; //TU JIAN HANDBOOK MAP
	WarriorFateMap	*m_warFateInfoMap;		//缘信息
    WarriorBaseMap *m_warBaseInfoMap;		//弟子基础信息
	WarriorGrowMap *m_warGrowInfoMap;		//弟子成长信息
    WarriorDialogMap * m_warDialogMap;      //弟子对话信息
	PotBuffMap *m_warPotBuffMap;			//弟子培养buff信息
	WarriorCurInfoVec *m_warCurInfoVec;		//当前拥有弟子信息
	//WarriorHandBookVec *
	WarriorUpSoulVec *m_warUpSoulVec;		//弟子突破需要魂魄信息
	SoulInfoVec *m_soulInfoVec;				//当前拥有魂魄信息
    cocos2d::CCArray *m_pWarriorLevelExpArr; //弟子经验对照表
	WarriorIconInfoMap  *m_warIconInfoMap;//弟子头像信息表
    WarriorUpgradeItemMap *m_warriorUpgradeItemMap; //弟子强化物品表
    WarriorUpgradeAccumulationMap *m_warriorUpgradeAccumulationMap;//弟子强化累加表
	WarriorUpgadeCriticalMap      *m_warriorUpgadeCriticalMap;//弟子强化暴击表
	TalentInfoMap							*m_talentInfoMap;
    WarriorTalentMap					 *m_warriorTalentMap;//弟子天赋表
    WarriorPotentialFactorVec            *m_warriorPotentialFactorVec;//弟子潜力点系数表
	
};





#endif /* defined(__QSMY__WarriorModel__) */
