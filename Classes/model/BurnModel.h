//
//  BurnModel.h
//  公会
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__BURNMODEL__
#define __QSMY__BURNMODEL__

#include "Model.h"
#include "UserModel.h"
#include "DropModel.h"
#include "WarriorModel.h"
#include "components/components.h"

//英雄炼化奖励数(将来改成7)
#define HERO_BURN_PREVIEW_COUNT	7
//宝物炼化奖励数
#define EQUIP_BURN_PREVIEW_COUNT	2
//炼化可放英雄数
#define TOTAL_BURN_NUM 5
//物品更新
#define D_EVENT_BURN_UPDATE	"burn_update"
//头像更新
#define D_EVENT_BURN_ICON_UPDATE	"burn_icon_update"
//自动炼化返回列表更新
#define D_EVENT_BURN_AUTOLIST	"burn_autolist"
//很奇怪的rate
#define BURN_RATE 0.8f
#define BURN_EXP_RATE 0.6f
//低级经验丹
#define EXP_ITEM_ID 10001
#define EQUIP_BURN_ITEM_ID	11005

//宝物吞噬相关
#define	TREASURE_BURN_MAX	4
#define TREASURE_RATE	0.7
#define	TREASURE_MAX_LEVEL	10

#define BURN_ITEM1	"BURN_ITEM1"	//炼化物品1
#define BURN_ITEM2	"BURN_ITEM2"	//炼化物品2
#define BURN_ITEM3	"BURN_ITEM3"	//炼化物品3
#define BURN_ITEM4	"BURN_ITEM4"	//炼化物品4
#define BURN_ITEM5	"BURN_ITEM5"	//炼化物品5
#define BURN_ITEM6	"BURN_ITEM6"	//炼化物品6

class BurnModel : public Model
{
	//飞翔的值
	CC_SYNTHESIZE(unsigned int, m_uCoinShowNum, CoinShowNum);
	CC_SYNTHESIZE(unsigned int, m_uItemShowNum, ItemShowNum);

	CC_SYNTHESIZE_READONLY(unsigned int, m_uCoin, Coin);
	CC_SYNTHESIZE_READONLY(unsigned int, m_uExp, Exp);
	CC_SYNTHESIZE_READONLY(unsigned int, m_uPro, Pro);

	CC_SYNTHESIZE_READONLY(unsigned int, m_uBigExp, BigExp);
	CC_SYNTHESIZE_READONLY(unsigned int, m_uSuperExp, SuperExp);
	CC_SYNTHESIZE_READONLY(unsigned int, m_uSkillItem, SkillItem);

	CC_SYNTHESIZE_READONLY(unsigned int, m_uUid, Uuid);
	//英雄表，键值对应
	CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *, m_pBurnDict, BurnDict);
	//魂魄
	CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *, m_pSoulDict,SoulDict);
	//里面是装备ID（唯一标识，不是equipid）
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pTreasureList, TreasureList);
	//里面是装备ID（唯一标识，不是equipid）
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pEquipList, EquipList);
	//装备炼化物品数
	CC_SYNTHESIZE(unsigned int, m_uEquipRefineryItemNum, EquipRefineryItemNum);

	//宝物吞噬
	CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *, m_pTreasureDict, TreasureDict);
	CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *, m_pTreasureLevel, TreasureLevel);
	CC_SYNTHESIZE_READONLY(cocos2d::CCDictionary *, m_pTreasureResistForLevel, TreasureResistForLevel);
	
	int getBurnNum();
	
public:

	//目前只有英雄炼化
	enum BurnType
	{
		kBurnTypeHero = 1,
		kBurnTypeEquip = 2,
	};

	//英雄量化类型：英雄（魂魄）
	enum BurnHeroType
	{
		kBurnHero =1, //英雄类型
		kBurnSoul =2, //魂魄类型
	};

	BurnModel()
		:m_pBurnDict(NULL)
		,m_pSoulDict(NULL)
		,m_pTreasureList(NULL)
		,m_pEquipList(NULL)
		,m_pTreasureDict(NULL)
		,m_pTreasureLevel(NULL)
		,m_pTreasureResistForLevel(NULL)
		,m_uCoin(0)
		,m_uExp(0)
		,m_uPro(0)
		,m_uUid(0)
		,m_uBigExp(0)
		,m_uSuperExp(0)
		,m_uSkillItem(0)
		,m_uEquipRefineryItemNum(0)
		,m_uCoinShowNum(0)
		,m_uItemShowNum(0)
	{
		for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++)
		{
			m_initCoinNum[i] = 0;
			m_initExpNum[i] = 0;
			m_initProNum[i] = 0;
			m_initTreasureNum[i] = 0;
		}

		for(uint8_t i = 0 ; i < HERO_BURN_PREVIEW_COUNT ; i++)
		{
			m_HeroBurnPreview[i] = 0;
		}

		for(uint8_t i = 0 ; i < EQUIP_BURN_PREVIEW_COUNT ; i++)
		{
			m_EquipBurnPreview[i] = 0;
		}	
	}
	~BurnModel();
	virtual bool init();

	//添加一个到炼化
	void addToList(uint32_t wid , BurnModel::BurnType burnType = BurnModel::kBurnTypeHero);
	void addToSoul(uint32_t wid);
	//移除一个  heroType1英雄 2魂魄
	void removeFromList(uint32_t wid , BurnModel::BurnType burnType = BurnModel::kBurnTypeHero,uint8_t heroType=0);
	//计算回报 post natification
	void changeItem(BurnModel::BurnType burnType = BurnModel::kBurnTypeHero);
	//heroType1英雄2魂魄 所得物品计算
	void setBurnGain(int wid,int heroType=1);
	//是否已选
	bool isSelected(uint32_t wid , BurnModel::BurnType burnType = BurnModel::kBurnTypeHero);
	bool isSoulSelected(uint32_t wid);
	//宝物自动炼化，客户端生成list
	void makeTreasureAutoList(unsigned int m_uCurrentID);
	//装备自动炼化
	void makeEquipAutoList();
	unsigned int getTreasureExpAdd(equip::EquipCurInfo*);

	//炼化确认(宝物吞噬)
	void confirmBurn(MessageType type, DMessageBox* target);

	//NETWORK
	void doTreasureBurn(unsigned int id);
	void parseTreasureBurn(const Json::Value &requestData,const Json::Value &responseData);

	//自动添加 传入个数 返回wid数组
	cocos2d::CCArray* autoAdd(uint32_t howmany);
	//request 发送要烧的数组
	void doBurn(BurnModel::BurnType burnType = BurnModel::kBurnTypeHero);
	//橙色英雄炼化二次确认
	bool willShowConfirm(BurnModel::BurnType burnType = BurnModel::kBurnTypeHero);
	//response
	void parseBurnHero(const Json::Value &requestData,const Json::Value &responseData);
	void parseBurnTreasure(const Json::Value &requestData,const Json::Value &responseData);
	//请求自动炼化数组
	void getAutoList();
	//处理返回
	void parseAutoList(const Json::Value &requestData,const Json::Value &responseData);

	virtual void reset();

private:
	void LoadBurnData(const CSVDecoder::CSV &val);
	uint32_t m_initCoinNum[WARRIOR_GRADE_NUM];
	uint32_t m_initExpNum[WARRIOR_GRADE_NUM];
	uint32_t m_initProNum[WARRIOR_GRADE_NUM];
	uint32_t m_initSoulNum[WARRIOR_GRADE_NUM]; //魂魄量化系数
	uint32_t m_initTreasureNum[WARRIOR_GRADE_NUM];//宝物的初始经验值
	void LoadTreasureInitData(const CSVDecoder::CSV &val);
	void LoadTreasureLevelData(const CSVDecoder::CSV &val);
	void LoadTreasureResistData(const CSVDecoder::CSV &val);
public:
	//英雄炼化预览数据
	uint32_t m_HeroBurnPreview[HERO_BURN_PREVIEW_COUNT];
	//宝物炼化预览数据
	uint32_t m_EquipBurnPreview[EQUIP_BURN_PREVIEW_COUNT];
};
#endif