//
//  EquipModel.h
//  装备信息
//
//  Created by cll on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__EquipModel__
#define __QSMY__EquipModel__

#include "Model.h"
#include "components/components.h"


#define  D_EVENT_EQUIPLIST_UPDATE "update equipList"
#define  D_EVENT_EQUIPEDLIST_UPDATE "update equipedList" //已经装备的装备信息列表变化
#define  D_EVENT_EQUIPINFO_UPDATE "update equipInfo"
#define  D_EVENT_EQUIP_SELL_EQUIP "sell equip"
#define  D_EVENT_EQUIP_REFINE "refine equip"

#define  EQUIP_AUTOENHANCE_VIP_LEVEL	2			//自动强化需要VIP等级

#define EQUIP_TYPE_COUNT 4

#define SELL_PRICE_RATE 0.1 //售價計算因子(12.1修改from0.8to0.1)
//精炼与强化等级之间的系数
#define	EQUIP_REFINE_RATE 10

class EquipModel;


namespace equip {

    class EquipCurInfo;
    
	//类型定义
    enum EquipType
    {
        kEquipTypeALL,
		kEquipTypeWeapon,		//武器
		kEquipTypeArmor,		//防具
		kEquipTypeOrnament,		//饰品
		kEquipTypeTreasure,		//典籍
        kEquipTypeCount         //类型总数
    };
  
	//品阶定义
	enum EquipGrade
	{
		kEquipGradeWhite,		//白色
		kEquipGradeGreen,		//绿色
		kEquipGradeBlue,		//蓝色
		kEquipGradePurple,		//紫色
		kEquipGradeRed,			//红色
		kEquipGradeTotal		//
	};
	
	//属性定义(对应UI)
	enum EquipPropType
	{
		kEquipPropAttack = 0,
		kEquipPropDefence,
		kEquipPropHp,
		kEquipPropSpeed,
		kEquipPropDodge,
		kEquipPropCritical,
		kEquipPropBlock,
		kEquipPropHit,
        kEquipPropTotal
	};

	//宝物进阶加成属性
	enum TreasurePropType
	{
		kTreasurePropAttack = 0,
		kTreasurePropDefence,
		kTreasurePropHp,
		kTreasurePropSpeed,
		kTreasurePropDodge,
		kTreasurePropCritical,
		kTreasurePropBlock,
		kTreasurePropHit,
		kTreasurePropResist,//抗性
		kTreasurePropAntiCritical,//抗暴击率
		kTreasurePropPercentAttack,//攻击百分比
		kTreasurePropPercentDefence,//防御百分比
		kTreasurePropPercentHp,//生命值百分比
	};
    
	/*宝物炼化返还-本地数据表配置*/
	class EquipBurnItemInfo : public ValueObjcet
	{
	public:
		CREATE_FUNC(EquipBurnItemInfo);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::EquipModel;
		CC_SYNTHESIZE_READONLY(equip::EquipType, equipType, EquipType);				// 装备类型
		CC_SYNTHESIZE_READONLY(equip::EquipGrade, equipGrade, EquipGrade);				// 装备颜色
		CC_SYNTHESIZE_READONLY(unsigned int, coinNum, CoinNum);				// 返还铜币数
		CC_SYNTHESIZE_READONLY(unsigned int, itemNum, ItemNum);		// 返还强化石数
	};

	/*装备详细信息-本地数据表配置*/
    class EquipBaseInfo:public ValueObjcet
    {
    public:
        CREATE_FUNC(EquipBaseInfo);
        D_INIT_DEFAULT(ValueObjcet);
        friend class ::EquipModel;
        friend class EquipCurInfo;
        
        CC_SYNTHESIZE_READONLY(unsigned int,m_uBurnItemNum,BurnItemNum);	//炼化返回物品数量

		CC_SYNTHESIZE_READONLY(unsigned int,equip_id,EquipId);				//装备id
		CC_SYNTHESIZE_READONLY(unsigned int,hp,Hp);					//生命
		CC_SYNTHESIZE_READONLY(unsigned int,attack,Attack);				//攻击
		CC_SYNTHESIZE_READONLY(unsigned int,defence,Defence);				//防御
		CC_SYNTHESIZE_READONLY(unsigned int,speed,Speed);					//速度
		CC_SYNTHESIZE_READONLY(unsigned int,dodge_rate,DodgeRate);            //躲闪
		CC_SYNTHESIZE_READONLY(unsigned int,critical_rate,CriticalRate);			//暴击
		CC_SYNTHESIZE_READONLY(unsigned int,block_rate,BlockRate);				//格挡
		CC_SYNTHESIZE_READONLY(unsigned int,hit_rate,HitRate);				//命中
					
        CC_SYNTHESIZE_READONLY(float, grow, Grow);//成长
        CC_SYNTHESIZE_READONLY(unsigned int, exp_grow, ExpGrow);//成长经验
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, equip_name, EquipName);//装备名
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, equip_desc, EquipDesc);//装备描述

        CC_SYNTHESIZE_READONLY(EquipGrade, equip_grade, EquipGrade);//装备品质
        CC_SYNTHESIZE_READONLY(EquipType, equip_type, EquipType);//装备类型
        
		CC_SYNTHESIZE_READONLY(unsigned int , quality,Quality);

		unsigned int level_type5;//5阶可以获得的加成类型
		CC_SYNTHESIZE_READONLY(unsigned int, level_rate5, LevelRate5);//5阶可以获得的加成
		unsigned int level_type10;//10阶可以获得的加成类型
		CC_SYNTHESIZE_READONLY(unsigned int, level_rate10, LevelRate10);//10阶可以获得的加成
		equip::TreasurePropType getLevelType5()
		{
			return (equip::TreasurePropType)(level_type5 - 1);
		}
		equip::TreasurePropType getLevelType10()
		{
			return (equip::TreasurePropType)(level_type10 - 1);
		}

		unsigned int refine_type1;//精炼属性1
		CC_SYNTHESIZE_READONLY(unsigned int, refine_rate1, RefineRate1);//精炼加成1(整数 攻防血)
		unsigned int refine_type2;//精炼属性2
		CC_SYNTHESIZE_READONLY(float, refine_rate2, RefineRate2);//精炼加成2(小数)
		equip::TreasurePropType getRefineType1()
		{
			return (equip::TreasurePropType)(refine_type1 - 1);
		}
		equip::TreasurePropType getRefineType2()
		{
			return (equip::TreasurePropType)(refine_type2 - 1);
		}

        bool isNeedShowGradeColor()
        {
            return equip_grade != kEquipGradeWhite;
        }
        
        equip::EquipPropType getPropertyType();//装备属性类型
    };

	/*装备强化信息-本地数据表配置*/
	class EquipEhanceInfo:public ValueObjcet
	{
	public:
		EquipEhanceInfo()
		{
			for(int i = 0 ; i<equip::kEquipGradeTotal ; i++)
			{
				m_pTreasureCostCoin[i] = 0;
				m_pTreasureCostItem[i] = 0;
				m_pTreasureCostCoinTotal[i] = 0;
				m_pTreasureCostItemTotal[i] = 0;
			}
		};

        CREATE_FUNC(EquipEhanceInfo);
        D_INIT_DEFAULT(ValueObjcet);
        friend class ::EquipModel;
        friend class EquipCurInfo;
        

        CC_SYNTHESIZE_READONLY(unsigned short, equip_lv, EquipLv); //装备等级
        CC_SYNTHESIZE_READONLY(unsigned int, green_cost, GreenCost);//强化消耗金币数
		CC_SYNTHESIZE_READONLY(unsigned int, green_total, GreenTotal);
		CC_SYNTHESIZE_READONLY(unsigned int, blue_cost, BlueCost);
		CC_SYNTHESIZE_READONLY(unsigned int, blue_total, BlueTotal);
		CC_SYNTHESIZE_READONLY(unsigned int, purple_cost, PurpleCost);
		CC_SYNTHESIZE_READONLY(unsigned int, purple_total, PurpleTotal);
		CC_SYNTHESIZE_READONLY(unsigned int, red_cost, RedCost);
		CC_SYNTHESIZE_READONLY(unsigned int, red_total, RedTotal);
		//宝物强化消耗
		unsigned int m_pTreasureCostCoin[equip::kEquipGradeTotal];
		unsigned int m_pTreasureCostItem[equip::kEquipGradeTotal];
		unsigned int m_pTreasureCostCoinTotal[equip::kEquipGradeTotal];
		unsigned int m_pTreasureCostItemTotal[equip::kEquipGradeTotal];

	};

	/*装备精炼材料信息-本地数据表配置*/
	class EquipRefineItemInfo : public ValueObjcet
	{
	public:
		CREATE_FUNC(EquipRefineItemInfo);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::EquipModel;

		CC_SYNTHESIZE_READONLY(unsigned int, beginLevel, BeginLevel);	//双倍增益区间
		CC_SYNTHESIZE_READONLY(unsigned int, endLevel, EndLevel);		//双倍增益区间
		CC_SYNTHESIZE_READONLY(unsigned int, itemId, ItemId);			// 材料ID
		CC_SYNTHESIZE_READONLY(unsigned int, exp, Exp);					// 精炼值
		CC_SYNTHESIZE_READONLY(unsigned int, expDouble, ExpDouble);		// 精炼值双倍增益
		CC_SYNTHESIZE(unsigned int, itemNum, ItemNum);					//数量,与物品数量相一致
	};
	
	/*装备精炼经验信息-本地数据表配置*/
	class EquipRefineExpInfo : public ValueObjcet
	{
	public:
		CREATE_FUNC(EquipRefineExpInfo);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::EquipModel;

		CC_SYNTHESIZE_READONLY(unsigned short, level, Level);		// 精炼等级
		CC_SYNTHESIZE_READONLY(unsigned int, blueExp, BlueExp);		// 蓝色品阶装备需要的精炼值
		CC_SYNTHESIZE_READONLY(unsigned int, purpleExp, PurpleExp);	// 紫色品阶装备需要的精炼值
		CC_SYNTHESIZE_READONLY(unsigned int, redExp, RedExp);		// 红色品阶装备需要的精炼值
	};
	

	/*装备列表中基本信息*/
	class EquipCurInfo:public ValueObjcet
	{
        
	public:
		EquipCurInfo()
        :id(0)
        ,equipId(0)
        ,exp(0)
        ,level(1)
		,refineLevel(0)
		,refineExp(0)
        ,gem(0)
        ,pid(0)
        ,baseInfo(NULL)
		,m_pInFate(false)
        {};

        static EquipCurInfo* create(unsigned int equipId);
        static EquipCurInfo* create(EquipBaseInfo*);
        
        
        D_INIT_DEFAULT(ValueObjcet);
        friend class ::EquipModel;
        
        
        /* 判断装备是否被穿戴 */
        bool isEquiped()
        {
            return pid > 0;
        }
        //是否能强化
        bool isCanEnhance();
        //是否能自动强化
        bool isCanAutoEnhance();
        /* 是否等级到达限制*/
        bool checkLevelLimit();
		//是否能精炼
		bool isCanRefine();
        
        
        CC_SYNTHESIZE_READONLY(unsigned int, id, Id);//数据库中唯一标识
        CC_SYNTHESIZE_READONLY(unsigned int, equipId, EquipId);//本地配置ID
		CC_SYNTHESIZE_READONLY(unsigned int, exp, Exp);	//宝物吞噬总经验
		CC_SYNTHESIZE_READONLY(unsigned int, pid, Pid);//绑定阵容唯一编号
        CC_SYNTHESIZE_READONLY(unsigned short, level, Level);//级别
		CC_SYNTHESIZE_READONLY(unsigned short, refineLevel, RefineLevel);//精炼级别
		CC_SYNTHESIZE_READONLY(unsigned short, refineExp, RefineExp);//精炼级别
        CC_SYNTHESIZE_READONLY(unsigned char, gem, Gem);//镶嵌宝石
        CC_SYNTHESIZE_READONLY(EquipBaseInfo*, baseInfo, BaseInfo);

		CC_SYNTHESIZE(bool, m_pInFate, InFate);//是否有缘 
        
    private:
        static EquipCurInfo * create();

	};
    
    
    class EquipColorConfig:public ValueObjcet
    {
    public:
        CREATE_FUNC(EquipColorConfig);
        D_INIT_DEFAULT(ValueObjcet);
        friend class ::EquipModel;
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(cocos2d::ccColor3B, color, Color);
    };
}

class EquipModel : public Model
{
public:
    typedef cocos2d::CCArray EquipCurInfoVec;
    
public:
    
	EquipModel();
    virtual ~EquipModel();
    virtual bool init();

	/* 从 EquipCurInfoVec 获取装备基本信息*/
    CCArray* getEquipCurInfosByEquipID(unsigned int equip_Id);
	equip::EquipCurInfo * getEquipCurInfoByID(unsigned int id);
	equip::EquipBaseInfo * getEquipBaseInfoByID(unsigned int equip_Id);

	// 获取装备精炼经验信息
	equip::EquipRefineExpInfo* getRefineExpInfo(unsigned int level);
	// 获取装备精炼材料信息
	equip::EquipRefineItemInfo* getRefineItemInfo(unsigned int itemId);
	// 获得精炼最大级别
	unsigned short getRefineMaxLevel();
	// 判断装备是否可以升阶(with tips)
	bool getEquipRefineIsCanUpLevel(equip::EquipCurInfo* pInfo , bool bShowTips = false);
	// 精炼经验值
	float getEquipRefineExpPercent(equip::EquipCurInfo* pInfo);

	// 获得材料精炼值
	unsigned int getRefineValueById(unsigned int itemId);
	// 获取装备精炼当前等级经验
	unsigned int getEquipRefineCurExp(unsigned int exp, equip::EquipGrade grade);
	// 根据总经验值获取精炼级别
	unsigned short getRefineLevelByExpGrade(unsigned int exp, equip::EquipGrade grade);
	// 获取精炼加成
	float getRefineAdditionByLevel(unsigned short level);
	

	/* 获取装备列表 */
	void getEquipListByTypeIgnoreID(equip::EquipType equipType, uint32_t ignoreID, EquipCurInfoVec& vec);
	// 获取装备列表(品质)
	void getEquipListByGradeIgnoreID(equip::EquipGrade equipGrade, uint32_t ignoreID, EquipCurInfoVec& vec);
	// 获取可以精炼的装备列表
	void getEquipRefineListByType(equip::EquipType equipType, EquipCurInfoVec &vec);
	// 获取没有被装备的装备列表
	void getEquipNotEquipedListByType(equip::EquipType equipType, EquipCurInfoVec &vec);

	/* 解析网络下发装备信息列表*/
	void parseEquips(const Json::Value &val);
	
	void parseOneEquip(const Json::Value &val,bool notifyEquipListUpdate = true,float delay = 0.f);

	void refineEquipCallBack(const Json::Value&,const Json::Value&);

	equip::EquipCurInfo * parseEquip(const Json::Value &val, equip::EquipCurInfo * pEquip = NULL);
    
	void addEquip(equip::EquipCurInfo *pEquip,bool notifyEquipListUpdate = true);
	void removeEquip(unsigned int id);

    //装备是否存在
    bool isEquipExist(unsigned int id);
    
	/*计算装备属性 显示下一级 显示下n级*/
	float computeEquipInfo(equip::EquipCurInfo *pEquipInfo, equip::EquipPropType type, bool upLevel = false , unsigned int toLevel = 0);
	/*计算装备属性*/
	float computeEquipInfo(equip::EquipCurInfo *pEquipInfo);
    
    /* 获取强化消耗 */
    unsigned int getEquipEnhanceCost(equip::EquipCurInfo *pEquipInfo);

	/* 获取宝物强化消耗的物品 */
	unsigned int getTreasureEnhanceCostItem(equip::EquipCurInfo *pEquipInfo);

	/* 获取宝物炼化返还的物品数 val = 1铜币，val = 2强化石*/
	unsigned int getEquipBurnCount(equip::EquipCurInfo *pEquipInfo , int val);
    
	/* 获取售賣價格 */
	int getPriceForSell(equip::EquipCurInfo *pEquipInfo);
    
    /* 检查调整装备在列表中的顺序*/
    void checkEquipOrder(equip::EquipCurInfo *pEquip,bool notify = true);
    
    /* 获取对应品阶的颜色值 */
	cocos2d::ccColor3B getEquipColorByGrade(equip::EquipGrade grade)
	{
		equip::EquipColorConfig* pEquipColorConfig = (equip::EquipColorConfig*)m_EquipColorMap->objectForKey(grade);
		return pEquipColorConfig?pEquipColorConfig->color:cocos2d::ccc3(0,0,0);
	}
    
    void reset();
    
    bool isMyEquip(equip::EquipCurInfo* pEquip);

	equip::EquipPropType getEquipPropType(equip::EquipCurInfo* pEquip, std::string& valueTypeLabel);

	bool compareProperty(equip::EquipCurInfo* pEquip1, equip::EquipCurInfo* pEquip2);
	
    
     /*装备了的装备信息*/
public:
    void addEquiped(equip::EquipCurInfo *pEquip);
    void removeEquiped(equip::EquipCurInfo *pEquip);
    equip::EquipCurInfo* getEquipedEquipByIdAndType(unsigned int id,equip::EquipType type);
    cocos2d::CCDictionary* getEquipedsById(unsigned int id);
	//计算装备战斗力
	unsigned int computeEquipFight(equip::EquipCurInfo *pEquip);
    
public:
    //void sellEquip(equip::EquipCurInfo*);
	void sellEquip(CCArray*);
	void decomposeEquip(CCArray*);
private:
    void confirmSellEquip(MessageType,DMessageBox* target);
	void confirmDecomposeEquip(MessageType,DMessageBox* target);
    
    //void doSellEquip(uint32_t);
	void doSellEquip(CCArray*);
	void doDecomposeEquip(CCArray*);
    void sellEquipCallBack(const Json::Value&,const Json::Value&);
	void decomposeEquipCallBack(const Json::Value&,const Json::Value&);
    
    
private:
    
    void loadColorConfig(const CSVDecoder::CSV &val);
	void loadEquipsData();
	void parseEquipsData(const CSVDecoder::CSV &val);
	void parseEquipEnhanceData(const CSVDecoder::CSV &val);
	void parseEquipRefineItemData(const CSVDecoder::CSV& val);
	void parseEquipRefineExpData(const CSVDecoder::CSV& val);
	void addEquipInorder(equip::EquipCurInfo *pEquipInfo);
	void loadEquipBurnItemData(const CSVDecoder::CSV& val);
private:
    
    cocos2d::CCDictionary *m_equipBaseInfoMap;
	cocos2d::CCDictionary *m_equipEnhanceMap;
	EquipCurInfoVec *m_equipCurInfoVec;
    cocos2d::CCArray* m_equipBurnItemInfoVec;
    /*装备了的装备信息*/
    cocos2d::CCDictionary *m_equipedList;    
    cocos2d::CCDictionary *m_EquipColorMap;

public:
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_equipRefineItemInfoVec, EquipRefineItemInfoVec);
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_equipRefineExpInfoVec, EquipRefineExpInfoVec);

	void getEquiped(CCArray *equiped);

	bool needToEnhance();

	bool hasUnequipedByType(equip::EquipType equipType);
	//强化/自动强化之前的客户端检验与提示
	bool doEnhanceCheck(equip::EquipCurInfo* pInfo , bool isAutoEnhance);
};





#endif /* defined(__QSMY__EquipModel__) */
