//
//  EquipModel.cpp
//  QSMY
//
//  Created by cll on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "EquipModel.h"
#include "BurnModel.h"
#include "managers/GameManager.h"
#include "managers/AnalyticsManager.h"
#include <algorithm>

using namespace equip;

static int sortEquipList(const CCObject* p1, const CCObject* p2)
{
	EquipCurInfo* e1 = (EquipCurInfo*)p1;
	EquipCurInfo* e2 = (EquipCurInfo*)p2;
	int i1 = (int)e1->isEquiped();
	int i2 = (int)e2->isEquiped();
	if(i1 == i2)
	{
		return ((int)(e1->getBaseInfo()->getEquipGrade()) > (int)(e2->getBaseInfo()->getEquipGrade()));
	}
	return i1 > i2;
};

#pragma mark - EquipBaseInfo

EquipPropType EquipBaseInfo::getPropertyType()
{
    EquipPropType type = kEquipPropTotal;
    switch(equip_type)
    {
        case equip::kEquipTypeWeapon:	//武器
            type = equip::kEquipPropAttack;
            break;
        case equip::kEquipTypeArmor:	//防具
            type = equip::kEquipPropDefence;
            break;
        case equip::kEquipTypeOrnament:	//饰品
            type = equip::kEquipPropHp;
            break;
        case equip::kEquipTypeTreasure:		//典籍 + 速度/躲闪/暴击/格挡
        {
            if(speed > 0)
            {
                type = equip::kEquipPropSpeed;
            }
            else if(dodge_rate > 0)
            {
                type = equip::kEquipPropDodge;
            }
            else if(critical_rate>0)
            {
                type = equip::kEquipPropCritical;
            }
            else if(block_rate>0)
            {
                type = equip::kEquipPropBlock;
            }
            else if(hit_rate>0)
            {
                type = equip::kEquipPropHit;
            }
            else
            {
                CCLOG("error prop");
            }
            break;
        }
        default:
            CCLOG("error type in getPropertyType.");
            break;
    }
    
    return type;
}

#pragma mark - EquipCurInfo

bool EquipCurInfo::checkLevelLimit()
{
    return level >= USER_EQUIP_LEVEL_RATIO*DM_GET_USER_MODEL->getUserInfo()->getUserLv();
}

bool EquipCurInfo::isCanEnhance()
{
    //return baseInfo->equip_type!= equip::kEquipTypeTreasure
    //&&!checkLevelLimit();
	bool rtn = !checkLevelLimit();
	if(baseInfo->getEquipType() == equip::kEquipTypeTreasure && level >= TREASURE_MAX_LEVEL)rtn = false;
	return rtn;
}

bool EquipCurInfo::isCanAutoEnhance()
{
    return isCanEnhance() && DM_GET_USER_MODEL->getUserInfo()->getVipLevel() >= EQUIP_AUTOENHANCE_VIP_LEVEL;
}

bool equip::EquipCurInfo::isCanRefine()
{
	bool rtn = DM_GET_EQUIP_MODEL->getEquipRefineIsCanUpLevel(this);
	return rtn;
	//return baseInfo->equip_grade >= kEquipGradeBlue && 
	//	(baseInfo->equip_type == kEquipTypeWeapon || baseInfo->equip_type == kEquipTypeArmor || baseInfo->equip_type == kEquipTypeOrnament);
}

EquipCurInfo* EquipCurInfo::create()
{
    equip::EquipCurInfo* pEquip = new equip::EquipCurInfo();
    if(pEquip && pEquip->init())
    {
        pEquip->autorelease();
        return pEquip;
    }
    CC_SAFE_DELETE(pEquip);
    return NULL;
}

EquipCurInfo* EquipCurInfo::create(unsigned int equipId)
{
    equip::EquipBaseInfo* pBase = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(equipId);
    return EquipCurInfo::create(pBase);
}

EquipCurInfo* EquipCurInfo::create(EquipBaseInfo* pInfo)
{
    if(pInfo==NULL) return NULL;
    equip::EquipCurInfo* pEquip = equip::EquipCurInfo::create();
    if(pEquip)
    {
        pEquip->equipId = pInfo->equip_id;
        pEquip->baseInfo = pInfo;
        return pEquip;
    }
    return NULL;
}


#pragma mark - EquipModel

EquipModel::EquipModel()
:m_EquipColorMap(NULL)
,m_equipBaseInfoMap(NULL)
,m_equipEnhanceMap(NULL)
,m_equipCurInfoVec(NULL)
,m_equipRefineItemInfoVec(NULL)
,m_equipRefineExpInfoVec(NULL)
,m_equipedList(NULL)
,m_equipBurnItemInfoVec(NULL)
{
	
}

EquipModel::~EquipModel()
{
    CC_SAFE_RELEASE_NULL(m_equipBaseInfoMap);
    CC_SAFE_RELEASE_NULL(m_equipEnhanceMap);
    CC_SAFE_RELEASE_NULL(m_equipCurInfoVec);
	CC_SAFE_RELEASE_NULL(m_equipRefineItemInfoVec);
	CC_SAFE_RELEASE_NULL(m_equipBurnItemInfoVec);
	CC_SAFE_RELEASE_NULL(m_equipRefineExpInfoVec);
    CC_SAFE_RELEASE_NULL(m_equipedList);
    CC_SAFE_RELEASE_NULL(m_EquipColorMap);
}


bool EquipModel::init()
{
    m_equipBaseInfoMap = cocos2d::CCDictionary::create();
	m_equipEnhanceMap = cocos2d::CCDictionary::create();
    m_EquipColorMap = cocos2d::CCDictionary::create();
	m_equipCurInfoVec = EquipCurInfoVec::create();
	m_equipRefineItemInfoVec = cocos2d::CCArray::create();
	m_equipRefineExpInfoVec = cocos2d::CCArray::create();
    m_equipedList = cocos2d::CCDictionary::create();
	m_equipBurnItemInfoVec = cocos2d::CCArray::create();
    
    CC_SAFE_RETAIN(m_equipBaseInfoMap);
    CC_SAFE_RETAIN(m_equipEnhanceMap);
    CC_SAFE_RETAIN(m_equipCurInfoVec);
	CC_SAFE_RETAIN(m_equipRefineItemInfoVec);
	CC_SAFE_RETAIN(m_equipRefineExpInfoVec);
    CC_SAFE_RETAIN(m_equipedList);
    CC_SAFE_RETAIN(m_EquipColorMap);
	CC_SAFE_RETAIN(m_equipBurnItemInfoVec);
    
	loadEquipsData();
    
	//loadEquipUpExpData();
    
    return Model::init();
}

void EquipModel::reset()
{
    if(m_equipCurInfoVec) m_equipCurInfoVec->removeAllObjects();
    if(m_equipedList) m_equipedList->removeAllObjects();
    Model::reset();
}

void EquipModel::loadEquipsData()
{
    loadColorConfig( CSVDecoder::create(EQUIP_COLOR_CONFIG_PATH)->getData());
    parseEquipsData( CSVDecoder::create(EQUIPS_CONFIG_PATH)->getData());
	parseEquipEnhanceData( CSVDecoder::create(EQUIP_ENHANCE_COST_CONFIG_PATH)->getData());
	parseEquipRefineItemData( CSVDecoder::create(REFINE_ITEM_CONFIG_PATH)->getData());
	parseEquipRefineExpData( CSVDecoder::create(REFINE_EXP_CONFIG_PATH)->getData());
	loadEquipBurnItemData( CSVDecoder::create(BURN_EXP_CONFIG_PATH)->getData());
}

void EquipModel::loadColorConfig(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
        EquipColorConfig *pEquipColorConfig = EquipColorConfig::create();
        EquipGrade grade = (EquipGrade)atoi(val[i][0].c_str());
		uint32_t color = 0;
        sscanf(val[i][1].c_str(), "%i",&color);
        pEquipColorConfig->color = cocos2d::ccc3(color>>16&0xFF, color>>8&0xFF, color>>0&0xFF);
        m_EquipColorMap->setObject(pEquipColorConfig, grade);
	}
}

void EquipModel::parseEquipsData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		EquipBaseInfo *pEquip = EquipBaseInfo::create();
		pEquip->equip_id = atoi(val[i][0].c_str());
		pEquip->equip_type = (EquipType)atoi(val[i][1].c_str());
		pEquip->equip_grade = (EquipGrade)atoi(val[i][2].c_str());
		pEquip->equip_name = val[i][3].c_str();
		pEquip->equip_desc = val[i][4].c_str();
		pEquip->hp = atoi(val[i][5].c_str());
		pEquip->attack = atoi(val[i][6].c_str());
		pEquip->defence = atoi(val[i][7].c_str());
		pEquip->speed = atoi(val[i][8].c_str());
        pEquip->hit_rate = atoi(val[i][15].c_str());
		pEquip->dodge_rate = atoi(val[i][9].c_str());
		pEquip->critical_rate = atoi(val[i][10].c_str());
		pEquip->block_rate = atoi(val[i][11].c_str());
		pEquip->grow = atof(val[i][14].c_str());
		pEquip->exp_grow = atoi(val[i][13].c_str());
		pEquip->quality  = atoi(val[i][16].c_str());

		//pEquip->resist_rate = atof(val[i][17].c_str());
		//pEquip->resist_grow = atof(val[i][18].c_str());
		pEquip->level_type5  = atoi(val[i][17].c_str());
		pEquip->level_rate5  = atoi(val[i][18].c_str());
		pEquip->level_type10  = atoi(val[i][19].c_str());
		pEquip->level_rate10  = atoi(val[i][20].c_str());

		pEquip->refine_type1  = atoi(val[i][22].c_str());
		pEquip->refine_rate1  = atoi(val[i][23].c_str());
		pEquip->refine_type2  = atoi(val[i][24].c_str());
		pEquip->refine_rate2  = atof(val[i][25].c_str());//float

		//这里给好初始值
		pEquip->m_uBurnItemNum = atoi(val[i][21].c_str());

        m_equipBaseInfoMap->setObject(pEquip, pEquip->equip_id);
	}
}

void EquipModel::parseEquipEnhanceData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		EquipEhanceInfo *pEquip = EquipEhanceInfo::create();
		pEquip->equip_lv = atoi(val[i][0].c_str());
		pEquip->green_cost = atoi(val[i][1].c_str());
		pEquip->green_total = atoi(val[i][2].c_str());
		pEquip->blue_cost = atoi(val[i][7].c_str());
		pEquip->blue_total = atoi(val[i][8].c_str());
		pEquip->purple_cost = atoi(val[i][13].c_str());
		pEquip->purple_total = atoi(val[i][14].c_str());
		pEquip->red_cost = atoi(val[i][19].c_str());
		pEquip->red_total = atoi(val[i][20].c_str());

		//j从1开始，0是白色，根本就没有白色的装备
		for(int j = 1 ; j < equip::kEquipGradeTotal ; j++)
		{
			pEquip->m_pTreasureCostCoin[j] = atoi(val[i][j*6 - 3].c_str());
			pEquip->m_pTreasureCostCoinTotal[j] = atoi(val[i][j*6 - 2].c_str());			
			pEquip->m_pTreasureCostItem[j] = atoi(val[i][j*6 - 1].c_str());
			pEquip->m_pTreasureCostItemTotal[j] = atoi(val[i][j*6].c_str());
		}
        
        m_equipEnhanceMap->setObject(pEquip, pEquip->equip_lv);
	}
}

void EquipModel::loadEquipBurnItemData(const CSVDecoder::CSV& val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		EquipBurnItemInfo* pInfo = EquipBurnItemInfo::create();
		pInfo->itemNum = atoi(val[i][3].c_str());
		pInfo->coinNum = atoi(val[i][4].c_str());
		int colorgrade = atoi(val[i][1].c_str());
		pInfo->equipGrade = (equip::EquipGrade)colorgrade;

		m_equipBurnItemInfoVec->addObject(pInfo);
	}
}

void EquipModel::parseEquipRefineItemData(const CSVDecoder::CSV& val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		EquipRefineItemInfo* pInfo = EquipRefineItemInfo::create();
		pInfo->beginLevel = atoi(val[i][1].c_str());
		pInfo->endLevel = atoi(val[i][2].c_str());
		pInfo->itemId = atoi(val[i][3].c_str());
		pInfo->exp = atoi(val[i][4].c_str());
		pInfo->expDouble = atoi(val[i][5].c_str());
		pInfo->itemNum = 0;//默认
		m_equipRefineItemInfoVec->addObject(pInfo);
	}
}

void EquipModel::parseEquipRefineExpData(const CSVDecoder::CSV& val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		EquipRefineExpInfo* pInfo = EquipRefineExpInfo::create();
		pInfo->level = atoi(val[i][0].c_str());
		pInfo->blueExp = atoi(val[i][1].c_str());
		pInfo->purpleExp = atoi(val[i][2].c_str());
		pInfo->redExp = atoi(val[i][3].c_str());

		m_equipRefineExpInfoVec->addObject(pInfo);
	}
}

CCArray * EquipModel::getEquipCurInfosByEquipID(unsigned int equip_Id)
{
    CCArray* pRet = CCArray::create();
    CCObject *obj = NULL;
    EquipCurInfo* pEquipCurInfo = NULL;
    CCARRAY_FOREACH(m_equipCurInfoVec, obj)
    {
        pEquipCurInfo = (EquipCurInfo*)obj;
        if(pEquipCurInfo->equipId == equip_Id)
        {
            pRet->addObject(pEquipCurInfo);
        }
    }
	return pRet;
}


EquipCurInfo* EquipModel::getEquipCurInfoByID(unsigned int id)
{
    CCObject *obj = NULL;
    EquipCurInfo* pEquipCurInfo = NULL;
    CCARRAY_FOREACH(m_equipCurInfoVec, obj)
    {
        pEquipCurInfo = (EquipCurInfo*)obj;
        if(pEquipCurInfo->id == id)
        {
            return pEquipCurInfo;
        }
    }
    
	return NULL;
}

unsigned int EquipModel::getEquipBurnCount(equip::EquipCurInfo *pEquipInfo , int val)
{
	unsigned int coincount = 0;
	unsigned int itemcount = 0;

	coincount += getPriceForSell(pEquipInfo);
	itemcount += pEquipInfo->getBaseInfo()->m_uBurnItemNum;

	//返回结果
	if(val == 1)return coincount;
	if(val == 2)return itemcount;
	return 0;
}

unsigned int EquipModel::getTreasureEnhanceCostItem(equip::EquipCurInfo *pEquipInfo)
{
	if(pEquipInfo->getBaseInfo()->getEquipType() != equip::kEquipTypeTreasure)
	{
		CCLOG("ERROR : EquipModel::getTreasureEnhanceCostItem wrong type");
		return 0;
	}
	EquipEhanceInfo* pEquipEnhanceInfo = (EquipEhanceInfo *)(m_equipEnhanceMap->objectForKey(pEquipInfo->level+1));
	if(!pEquipEnhanceInfo){
		//可能已经达到了强化上限
		CCLOG("ERROR : EquipModel::getTreasureEnhanceCostItem");
		return 0;
	}

	//下面终于到了正常情况
	unsigned int cost = 0;
	int equipcolor = (int)pEquipInfo->getBaseInfo()->getEquipGrade();
	cost = pEquipEnhanceInfo->m_pTreasureCostItem[equipcolor];
	return cost;
}

unsigned int EquipModel::getEquipEnhanceCost(equip::EquipCurInfo *pEquipInfo)
{
	EquipEhanceInfo* pEquipEnhanceInfo = (EquipEhanceInfo *)(m_equipEnhanceMap->objectForKey(pEquipInfo->level+1));
	if(!pEquipEnhanceInfo){
		//可能已经达到了强化上限
		CCLOG("ERROR : EquipModel::getEquipEnhanceCost");
		return 0;
	}

	//以下是正常情况，区分宝物类型的装备和其他类型的装备

	unsigned int cost = 0;
    unsigned int add = 0;
	if (pEquipInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure)
	{
		//宝物
		int equipcolor = (int)pEquipInfo->getBaseInfo()->getEquipGrade();
		cost = pEquipEnhanceInfo->m_pTreasureCostCoin[equipcolor];
	}
	else
	{
		//品质不同
		switch(pEquipInfo->baseInfo->equip_grade)
		{
            case equip::kEquipGradeRed:
                cost = pEquipEnhanceInfo->red_cost;
                break;
            case equip::kEquipGradePurple:
                cost = pEquipEnhanceInfo->purple_cost;
                break;
            case equip::kEquipGradeBlue:
                cost = pEquipEnhanceInfo->blue_cost;
                break;
            case equip::kEquipGradeGreen:
                cost = pEquipEnhanceInfo->green_cost;
                break;
            case equip::kEquipGradeWhite:
            default:
                break;
		}
	}
    
    return cost;
}

int EquipModel::getPriceForSell(equip::EquipCurInfo *pEquipInfo)
{
	EquipEhanceInfo* pEquipEnhanceInfo = (EquipEhanceInfo *)(m_equipEnhanceMap->objectForKey(pEquipInfo->level));
	int cost = 0;
	float add = 0.5f;//四捨五入
	if (pEquipEnhanceInfo)
	{
		//品质不同
		switch(pEquipInfo->baseInfo->equip_grade)
		{
		case equip::kEquipGradeRed:cost = (int)(add + pEquipEnhanceInfo->red_total * SELL_PRICE_RATE);break;
		case equip::kEquipGradePurple:cost = (int)(add + pEquipEnhanceInfo->purple_total * SELL_PRICE_RATE);break;
		case equip::kEquipGradeBlue:cost = (int)(add + pEquipEnhanceInfo->blue_total * SELL_PRICE_RATE);break;
		case equip::kEquipGradeGreen:cost = (int)(add + pEquipEnhanceInfo->green_total * SELL_PRICE_RATE);break;
		default:
			break;
		}
	}
	return cost;
}


EquipBaseInfo * EquipModel::getEquipBaseInfoByID(unsigned int equip_id)
{
    return (EquipBaseInfo*)m_equipBaseInfoMap->objectForKey(equip_id);
}

equip::EquipRefineExpInfo* EquipModel::getRefineExpInfo(unsigned int level)
{
	CCObject* obj = NULL;
	EquipRefineExpInfo* pRtnInfo = NULL;
	EquipRefineExpInfo* pInfo = NULL;
	CCARRAY_FOREACH(m_equipRefineExpInfoVec, obj)
	{
		pInfo = (EquipRefineExpInfo*)obj;
		if (pInfo->level == level)
		{
			pRtnInfo = pInfo;
			break;
		}
	}
	return pRtnInfo;
}

equip::EquipRefineItemInfo* EquipModel::getRefineItemInfo(unsigned int itemId)
{
	CCObject* obj = NULL;
	EquipRefineItemInfo* pRtnInfo = NULL;
	EquipRefineItemInfo* pInfo = NULL;
	CCARRAY_FOREACH(m_equipRefineItemInfoVec, obj)
	{
		pInfo = (EquipRefineItemInfo*)obj;
		if (pInfo->itemId == itemId)
		{
			pRtnInfo = pInfo;
			break;
		}
	}
	return pRtnInfo;
}

unsigned int EquipModel::getRefineValueById(unsigned int itemId)
{
	CCObject* obj = NULL;
	EquipRefineItemInfo* pEquipRefineInfo = NULL;
	CCARRAY_FOREACH(m_equipRefineItemInfoVec, obj)
	{
		pEquipRefineInfo = (EquipRefineItemInfo*)obj;
		if (pEquipRefineInfo->itemId == itemId)
		{
			return pEquipRefineInfo->exp;
		}
	}

	return 0;
}

unsigned short EquipModel::getRefineMaxLevel()
{
	return m_equipRefineExpInfoVec->count();
}

unsigned int EquipModel::getEquipRefineCurExp(unsigned int exp, equip::EquipGrade grade)
{
	CCObject* obj = NULL;
	EquipRefineExpInfo* pEquipRefineInfo = NULL;
	unsigned int tempExp = 0;
	CCARRAY_FOREACH(m_equipRefineExpInfoVec, obj)
	{
		pEquipRefineInfo = (EquipRefineExpInfo*)obj;
		switch (grade)
		{
		case kEquipGradeBlue:
			{
				if (exp < pEquipRefineInfo->blueExp)
				{
					return exp - tempExp;
				}
				else if (pEquipRefineInfo->level == getRefineMaxLevel())
				{
					return pEquipRefineInfo->blueExp - tempExp;
				}
				
				tempExp = pEquipRefineInfo->blueExp;
				break;
			}
		case kEquipGradePurple:
			{
				if (exp < pEquipRefineInfo->purpleExp)
				{
					return exp - tempExp;
				}
				else if (pEquipRefineInfo->level == getRefineMaxLevel())
				{
					return pEquipRefineInfo->purpleExp - tempExp;
				}

				tempExp = pEquipRefineInfo->purpleExp;
				break;
			}
		case kEquipGradeRed:
			{
				if (exp < pEquipRefineInfo->redExp)
				{
					return exp - tempExp;
				}
				else if (pEquipRefineInfo->level == getRefineMaxLevel())
				{
					return pEquipRefineInfo->redExp - tempExp;
				}

				tempExp = pEquipRefineInfo->redExp;
				break;
			}
		default:
			break;
		}
	}

	return 0;
}

unsigned short EquipModel::getRefineLevelByExpGrade(unsigned int exp, equip::EquipGrade grade)
{
	CCObject* obj = NULL;
	EquipRefineExpInfo* pEquipRefineInfo = NULL;
	unsigned int tempExp = 0;
	CCARRAY_FOREACH(m_equipRefineExpInfoVec, obj)
	{
		pEquipRefineInfo = (EquipRefineExpInfo*)obj;
		switch (grade)
		{
		case kEquipGradeBlue:
			tempExp = pEquipRefineInfo->blueExp;
			break;
		case kEquipGradePurple:
			tempExp = pEquipRefineInfo->purpleExp;
			break;
		case kEquipGradeRed:
			tempExp = pEquipRefineInfo->redExp;
			break;
		default:
			break;
		}

		if (tempExp > exp)
		{
			return pEquipRefineInfo->level - 1;
		}
		else if (tempExp == exp)
		{
			return pEquipRefineInfo->level;
		}
		else if (pEquipRefineInfo->level == getRefineMaxLevel())
		{
			return getRefineMaxLevel();
		}
	}

	return 0;
}

float EquipModel::getRefineAdditionByLevel(unsigned short level)
{
	return (level * 0.025f + 1) * 100.0f;
}

float EquipModel::getEquipRefineExpPercent(equip::EquipCurInfo* pInfo)
{
	float rtn = 0.0f;
	unsigned int refineLevel = pInfo->getRefineLevel();
	unsigned int refineLevelMax = getRefineMaxLevel();

	if(refineLevel >= refineLevelMax)
	{
		//顶级
		rtn = 1.0f;
	}
	else
	{		
		int levelExp = 0;
		int levelExpNext = 0;
		int exp = pInfo->getRefineExp();
		EquipRefineExpInfo* pRefineInfo = getRefineExpInfo(refineLevel);
		EquipRefineExpInfo* pRefineInfoNext = getRefineExpInfo(refineLevel + 1);
		equip::EquipGrade equipgrade = pInfo->getBaseInfo()->getEquipGrade();

		if(equipgrade == equip::kEquipGradeBlue)
		{
			levelExp = refineLevel == 0 ? 0 : pRefineInfo->getBlueExp();
			levelExpNext = pRefineInfoNext->getBlueExp();
		}
		else if(equipgrade == equip::kEquipGradePurple)
		{
			levelExp = refineLevel == 0 ? 0 : pRefineInfo->getPurpleExp();
			levelExpNext = pRefineInfoNext->getPurpleExp();
		}
		else if(equipgrade == equip::kEquipGradeRed)
		{
			levelExp = refineLevel == 0 ? 0 : pRefineInfo->getRedExp();
			levelExpNext = pRefineInfoNext->getRedExp();
		}

		float total = (float)levelExpNext - levelExp;
		float now = (float)exp - levelExp;
		rtn = now/total;
	}
	if(rtn < 0.0f || rtn > 1.0f)
	{
		//异常结果
		rtn = 0.0f;
	}
	return rtn;
}

bool EquipModel::getEquipRefineIsCanUpLevel(equip::EquipCurInfo* pInfo , bool bShowTips)
{
	bool rtn = true;

	unsigned int refineLevel = pInfo->getRefineLevel();
	unsigned int equipLevel = pInfo->getLevel();
	unsigned int refineLevelMax = getRefineMaxLevel();

	if(pInfo->getBaseInfo()->getEquipGrade() < equip::kEquipGradeBlue)
	{
		//颜色不对
		rtn = false;
		if(bShowTips) Tips_Alert(D_LOCAL_STRING("EquipCantRefine").c_str());
	}
	else if(pInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure)
	{
		//类型不对
		rtn = false;
		if(bShowTips) Tips_Alert(D_LOCAL_STRING("EquipCantRefine").c_str());
	}
	else if(refineLevel >= refineLevelMax)
	{
		//顶级
		rtn = false;
		if(bShowTips) Tips_Alert(D_LOCAL_STRING("EquipRefineMax").c_str());
	}
	else if(refineLevel+1 > equipLevel/EQUIP_REFINE_RATE)
	{
		//强化等级不够
		rtn = false;
		if(bShowTips) Tips_Alert(D_LOCAL_STRING("EquipRefineMax").c_str());
	}

	return rtn;
	/*if (!pInfo)
	{
		return false;
	}

	cocos2d::CCArray* pItems = getEquipRefineItemInfoByType(pInfo->getBaseInfo()->getEquipType());
	unsigned int exp = 0;
	cocos2d::CCObject* obj = NULL;
	equip::EquipRefineItemInfo* pEquipRefineInfo = NULL;
	CCARRAY_FOREACH(pItems, obj)
	{
		pEquipRefineInfo = (equip::EquipRefineItemInfo*)obj;
		if (pEquipRefineInfo)
		{
			exp += DM_GET_ITEM_MODEL->getItemNum(pEquipRefineInfo->getItemId()) * pEquipRefineInfo->getRefineValue();
		}
	}

	if (exp == 0)
	{
		return false;
	}

	unsigned short curLevel = getRefineLevelByExpGrade(pInfo->getRefineExp(), pInfo->getBaseInfo()->getEquipGrade());
	unsigned short tmpLevel = getRefineLevelByExpGrade(pInfo->getRefineExp() + exp, pInfo->getBaseInfo()->getEquipGrade());

	return tmpLevel > curLevel;*/
}

bool EquipModel::compareProperty(equip::EquipCurInfo* pEquip1, equip::EquipCurInfo* pEquip2)
{
	return  computeEquipInfo(pEquip1) > computeEquipInfo(pEquip2);
}

float EquipModel::computeEquipInfo(equip::EquipCurInfo *pEquipInfo)
{
	//std::string pecent;
    std::string valueTypeString;
	equip::EquipPropType type = getEquipPropType(pEquipInfo, valueTypeString);

	return computeEquipInfo(pEquipInfo,type);
}

float  EquipModel::computeEquipInfo(equip::EquipCurInfo *pEquipInfo, equip::EquipPropType type, bool upLevel , unsigned int toLevel)
{
    
	float prop_value = 0;
	
	unsigned int level = pEquipInfo->level + (upLevel?1:0) ;
    if(toLevel>0)level = toLevel;

	//float grow_value = (pow(1.05f, ((float)(level-1)/3))-1) * DM_GET_CONFIG_MODEL->getConfigByKey(EQUIP_GROW_RATIO)->floatValue() * pEquipInfo->baseInfo->grow;
	float grow_value = (level-1) * DM_GET_CONFIG_MODEL->getConfigByKey(EQUIP_GROW_RATIO)->floatValue() * pEquipInfo->baseInfo->grow;
	//1.精炼加成(下面加在grow_value中)
	float refine_value = pEquipInfo->getRefineLevel() * pEquipInfo->getBaseInfo()->getRefineRate1();
	grow_value += refine_value;
	//2.过去的精炼(不在使用)
	float refineAdd = 1.0f;//getRefineAdditionByLevel(getRefineLevelByExpGrade(pEquipInfo->getRefineExp(), pEquipInfo->getBaseInfo()->getEquipGrade())) / 100.0f;

	switch(type)
	{
        case equip::kEquipPropAttack:
            prop_value = (pEquipInfo->baseInfo->attack + grow_value) * refineAdd;
            break;
        case equip::kEquipPropDefence:
            prop_value = (pEquipInfo->baseInfo->defence + grow_value) * refineAdd;
            break;
        case equip::kEquipPropHp:
            prop_value = (pEquipInfo->baseInfo->hp + grow_value) * refineAdd;
            break;
        case equip::kEquipPropSpeed:
            prop_value = pEquipInfo->baseInfo->speed + grow_value;//以下的加成按百分比计算，不用乘以100
            break;
        case equip::kEquipPropDodge:
            prop_value = pEquipInfo->baseInfo->dodge_rate + grow_value;
            break;
        case equip::kEquipPropCritical:
            prop_value = pEquipInfo->baseInfo->critical_rate + grow_value;
            break;
        case equip::kEquipPropBlock:
            prop_value = pEquipInfo->baseInfo->block_rate + grow_value;
            break;
		case equip::kEquipPropHit:
			prop_value = pEquipInfo->baseInfo->hit_rate + grow_value;
			break;
        case equip::kEquipPropTotal:
        default:
            break;
	}
    
	return prop_value;
}

bool EquipModel::isEquipExist(unsigned int id)
{
    return getEquipCurInfoByID(id)!=NULL;
}

/*************************************************************************
 * desc: 有条件性获得装备列表											 *
 * param:                                                            *
 *		equipType 装备类型											 *
 *		ignoreID 不包括的装备id，0表示不限ID								 *
 *	    vec  列表存放位置												 *
 ************************************************************************/

void EquipModel::getEquipListByTypeIgnoreID(EquipType equipType, uint32_t ignoreID,EquipCurInfoVec &vec)
{
    vec.removeAllObjects();
    
    CCObject* obj = NULL;
    EquipCurInfo* pEquipCurInfo = NULL;
    
    CCARRAY_FOREACH(m_equipCurInfoVec, obj)
    {
        pEquipCurInfo = (EquipCurInfo*)obj;
		if (!pEquipCurInfo || !(pEquipCurInfo->baseInfo))
		{
			continue;
		}
        if((pEquipCurInfo->baseInfo->equip_type == equipType  || kEquipTypeALL == equipType)
           &&(pEquipCurInfo->id != ignoreID || 0 == ignoreID))
        {
            vec.addObject(pEquipCurInfo);
        }
        
    }

	//排序
	std::sort(vec.data->arr, vec.data->arr + vec.data->num, sortEquipList);

}
/*************************************************************************
 * desc: 有条件性获得装备列表，没有被装备的						*
 * param:                                                            *
 *		equipType 装备类型											 *
 *		ignoreID 不包括的装备id，0表示不限ID								 *
 *	    vec  列表存放位置												 *
 ************************************************************************/
void EquipModel::getEquipNotEquipedListByType(equip::EquipType equipType, EquipCurInfoVec &vec)
{
	vec.removeAllObjects();

	CCObject* obj = NULL;
	EquipCurInfo* pEquipCurInfo = NULL;

	CCARRAY_FOREACH(m_equipCurInfoVec, obj)
	{
		pEquipCurInfo = (EquipCurInfo*)obj;
		if (!pEquipCurInfo || !(pEquipCurInfo->baseInfo))
		{
			continue;
		}
		if((pEquipCurInfo->baseInfo->equip_type == equipType  || kEquipTypeALL == equipType)
			&&(!pEquipCurInfo->isEquiped()))
		{
			vec.addObject(pEquipCurInfo);
		}

	}
}

/*************************************************************************
 * desc: 有条件性获得装备列表											 *
 * param:                                                            *
 *		equipGrade 装备品质											 *
 *		ignoreID 不包括的装备id，0表示不限ID								 *
 *	    vec  列表存放位置												 *
 ************************************************************************/
void EquipModel::getEquipListByGradeIgnoreID(equip::EquipGrade equipGrade, uint32_t ignoreID, EquipCurInfoVec& vec)
{
	vec.removeAllObjects();

	CCObject* obj = NULL;
	EquipCurInfo* pEquipCurInfo = NULL;

	CCARRAY_FOREACH(m_equipCurInfoVec, obj)
	{
		pEquipCurInfo = (EquipCurInfo*)obj;
		if((pEquipCurInfo->baseInfo->equip_grade == equipGrade)
			&&(pEquipCurInfo->id != ignoreID || 0 == ignoreID))
		{
			vec.addObject(pEquipCurInfo);
		}
	}
}

void EquipModel::getEquipRefineListByType(equip::EquipType equipType, EquipCurInfoVec& vec)
{
	vec.removeAllObjects();

	CCObject* obj = NULL;
	EquipCurInfo* pEquipCurInfo = NULL;

	CCARRAY_FOREACH(m_equipCurInfoVec, obj)
	{
		pEquipCurInfo = (EquipCurInfo*)obj;
		if((pEquipCurInfo->baseInfo->equip_type == equipType  || kEquipTypeALL == equipType)
			&&(pEquipCurInfo->isCanRefine()))
		{
			vec.addObject(pEquipCurInfo);
		}
	}
}

void EquipModel::parseEquips(const Json::Value &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		parseOneEquip(val[i],false);
	}
	isSynced = true;
	postNotification(D_EVENT_EQUIPLIST_UPDATE);
}


void EquipModel::parseOneEquip(const Json::Value &val,bool notifyEquipListUpdate,float delay)
{
	EquipCurInfo *pEquip = getEquipCurInfoByID(val["id"].asUInt());
/************************************************************************/
/* for analytics begin                                                  */
/************************************************************************/
	bool dropped = false;
	if (NULL == pEquip)
	{
		dropped = true;
	}
/************************************************************************/
/*  for analytics end                                                   */
/************************************************************************/	   
    bool equipedListChanged(false);
    if(pEquip)
    {
        if(val.isMember("pid") && pEquip->pid != val["pid"].asUInt())
        {
            if(pEquip->isEquiped())
            {
                removeEquiped(pEquip);
            }
            equipedListChanged = true;
        }
        parseEquip(val, pEquip);
        if(pEquip->isEquiped())
        {
            addEquiped(pEquip);
        }
        pEquip->postNotification(D_EVENT_EQUIPINFO_UPDATE,delay);
        checkEquipOrder(pEquip,notifyEquipListUpdate);
        
    }else
    {
        pEquip = parseEquip(val, pEquip);
        if(pEquip->isEquiped())
        {
            addEquiped(pEquip);
            equipedListChanged = true;
        }
        pEquip->isSynced = true;
        addEquip(pEquip,notifyEquipListUpdate);
    }
    if(equipedListChanged) postNotification(D_EVENT_EQUIPEDLIST_UPDATE,delay);
/************************************************************************/
/* for analytics begin                                                  */
/************************************************************************/

	if (!pEquip || !dropped || !pEquip->getBaseInfo())
	{
		return;
	}
	else
	{
		AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
		pAnalytics->dropItem(pEquip->getBaseInfo()->getEquipName().c_str(), 1);
	}
/************************************************************************/
/*  for analytics end                                                   */
/************************************************************************/	 
}

void EquipModel::addEquiped(equip::EquipCurInfo *pEquip)
{
	if (!pEquip || !(pEquip->baseInfo))
	{
		return;
	}
    if(!m_equipedList->objectForKey(pEquip->pid))
    {
        m_equipedList->setObject(cocos2d::CCDictionary::create(), pEquip->pid);
    }
    cocos2d::CCDictionary *pDic = (cocos2d::CCDictionary*)m_equipedList->objectForKey(pEquip->pid);
    pDic->setObject(pEquip, pEquip->baseInfo->equip_type);
    
}

void EquipModel::removeEquiped(equip::EquipCurInfo *pEquip)
{
    if(!m_equipedList->objectForKey(pEquip->pid))
    {
        m_equipedList->setObject(cocos2d::CCDictionary::create(), pEquip->pid);
    }
    cocos2d::CCDictionary *pDic = (cocos2d::CCDictionary*)m_equipedList->objectForKey(pEquip->pid);
    pDic->removeObjectForKey(pEquip->baseInfo->equip_type);
}

cocos2d::CCDictionary* EquipModel::getEquipedsById(uint32_t id)
{
    if(!m_equipedList->objectForKey(id))
    {
        m_equipedList->setObject(cocos2d::CCDictionary::create(), id);
    }
    return (cocos2d::CCDictionary*)m_equipedList->objectForKey(id);
}

EquipCurInfo* EquipModel::getEquipedEquipByIdAndType(uint32_t id,equip::EquipType type)
{
    cocos2d::CCDictionary *pDic = getEquipedsById(id);
    EquipCurInfo* pEquipCurInfo = (EquipCurInfo*)pDic->objectForKey(type);
    return pEquipCurInfo;
}


void EquipModel::checkEquipOrder(EquipCurInfo *pEquip,bool notifyEquipListUpdate)
{
    int index = m_equipCurInfoVec->indexOfObject(pEquip); //防止强化升级 重新调整顺序
    m_equipCurInfoVec->removeObjectAtIndex(index,false);
    addEquipInorder(pEquip);
    if(index!=m_equipCurInfoVec->indexOfObject(pEquip))
    {
        if(notifyEquipListUpdate) postNotification(D_EVENT_EQUIPLIST_UPDATE); //如果排序顺序变化 通知刷新列表
    }
}



/* 添加一个全新的装备*/
void EquipModel::addEquip(EquipCurInfo *pEquip,bool notifyEquipListUpdate)
{
    if(!pEquip)
    {
        CCLOG("%s","添加非法的装备");
        return;
    }
    pEquip->baseInfo = getEquipBaseInfoByID(pEquip->equipId);
	addEquipInorder(pEquip);
    if(notifyEquipListUpdate) postNotification(D_EVENT_EQUIPLIST_UPDATE);
}


void  EquipModel::removeEquip(unsigned int sid)
{
    EquipCurInfo* pEquipCurInfo = getEquipCurInfoByID(sid);
    if(pEquipCurInfo)
    {
        if(pEquipCurInfo->isEquiped())
        {
            removeEquiped(pEquipCurInfo);
            postNotification(D_EVENT_EQUIPEDLIST_UPDATE);
        }
        m_equipCurInfoVec->removeObject(pEquipCurInfo);
        postNotification(D_EVENT_EQUIPLIST_UPDATE);
        postNotification(D_EVENT_EQUIP_SELL_EQUIP);
    }else
    {
        CCLOG("%s:%d","找不到装备信息",sid);
    }
	//postNotification(D_EVENT_SELL_EQUIP);
}

void EquipModel::addEquipInorder(equip::EquipCurInfo *pEquip)
{
    //有序插入
    equip::EquipCurInfo * pEquipCurInfo = NULL;
    uint32_t i = 0;
    for (; i<m_equipCurInfoVec->count(); ++i)
    {
        pEquipCurInfo = (equip::EquipCurInfo *)m_equipCurInfoVec->objectAtIndex(i);
		if (!pEquipCurInfo || !( pEquip->baseInfo))
		{
			break;
		}
        if (pEquipCurInfo->baseInfo->equip_grade < pEquip->baseInfo->equip_grade )
        {
            break;
        }else if( pEquipCurInfo->baseInfo->equip_grade > pEquip->baseInfo->equip_grade )
        {
            continue;
        }//相等比下一项
        
        if(pEquip->level > pEquipCurInfo->level)
        {
            break;
        }else if(pEquip->level < pEquipCurInfo->level)
        {
            continue;
        }//相等比下一项
        
        if(pEquip->baseInfo->equip_type < pEquipCurInfo->baseInfo->equip_type)
        {
            break;
        }else if(pEquip->baseInfo->equip_type > pEquipCurInfo->baseInfo->equip_type)
        {
            continue;
        }//相等比下一项
        
        if(pEquipCurInfo->baseInfo->equip_name==pEquip->baseInfo->equip_name)
        {
            break;
        }
    }
    //m_equipCurInfoVec.insert(itr, pEquip);
    m_equipCurInfoVec->insertObject(pEquip,i);
}

EquipCurInfo* EquipModel::parseEquip(const Json::Value &val, EquipCurInfo *pEquip)
{
	if(pEquip == NULL)
	{
		pEquip = EquipCurInfo::create();
	}
    
	DJSON_UINT(pEquip->id,val,"id");
	DJSON_UINT(pEquip->equipId,val,"eid");
	DJSON_UINT(pEquip->exp,val,"exp");
	DJSON_UINT(pEquip->level,val,"lv");
	DJSON_UINT(pEquip->gem,val,"gem");
	DJSON_UINT(pEquip->pid,val,"pid");
	DJSON_UINT(pEquip->refineLevel,val,"refining");
	DJSON_UINT(pEquip->refineExp,val,"refineExp");
    pEquip->baseInfo = getEquipBaseInfoByID(pEquip->equipId);
	return pEquip;
}

bool EquipModel::isMyEquip(equip::EquipCurInfo* pEquip)
{
    if(pEquip==NULL) return false;
    return m_equipCurInfoVec->indexOfObject(pEquip) != CC_INVALID_INDEX;
}



//void EquipModel::sellEquip(equip::EquipCurInfo* pEquipInfo)
//{
//    if(pEquipInfo==NULL) return;
//    if(pEquipInfo->baseInfo->getEquipGrade()>=equip::kEquipGradePurple)
//    {
//        DMessageBox* pPBox = DMessageBox::showMessageBox(D_LOCAL_STRING("ConfirmSellEquip").c_str(), this, messageBox_callfunc_selector(EquipModel::confirmSellEquip));
//        pPBox->setUserObject(pEquipInfo);
//        return;
//    }
//    doSellEquip(pEquipInfo->id);
//}

void EquipModel::sellEquip(CCArray* pEquipInfoVec)
{
	if (pEquipInfoVec == NULL)
	{
		return;
	}

	if (pEquipInfoVec->count() == 0)
	{
		Tips_Alert(D_LOCAL_STRING("EquipDecomposeNoSelect").c_str());
		return;
	}
	/************************************************************************/
	/* 无需验证颜色(装备炼化已验证)                                         */
	/************************************************************************/
	//CCObject* obj = NULL;
	//EquipCurInfo* pEquipCurInfo = NULL;
	//CCARRAY_FOREACH(pEquipInfoVec, obj)
	//{
	//	pEquipCurInfo = (EquipCurInfo*)obj;
	//	if (pEquipCurInfo && pEquipCurInfo->getBaseInfo()->getEquipGrade() >= equip::kEquipGradePurple)
	//	{
	//		DMessageBox* pPBox = DMessageBox::showMessageBox(D_LOCAL_STRING("ConfirmSellEquip").c_str(), this, messageBox_callfunc_selector(EquipModel::confirmSellEquip));
	//		pPBox->setUserObject(pEquipInfoVec);
	//		return;
	//	}
	//}

	doSellEquip(pEquipInfoVec);
}

void EquipModel::decomposeEquip(CCArray* pEquipInfoVec)
{
	if (pEquipInfoVec == NULL)
	{
		return;
	}

	if (pEquipInfoVec->count() == 0)
	{
		Tips_Alert(D_LOCAL_STRING("EquipDecomposeNoSelect").c_str());
		return;
	}
	/************************************************************************/
	/* 先把提示去掉！                                                                     */
	/************************************************************************/
	/*CCObject* obj = NULL;
	EquipCurInfo* pEquipCurInfo = NULL;
	CCARRAY_FOREACH(pEquipInfoVec, obj)
	{
		pEquipCurInfo = (EquipCurInfo*)obj;
		if (pEquipCurInfo && pEquipCurInfo->getBaseInfo()->getEquipGrade() >= equip::kEquipGradeBlue)
		{
			DMessageBox* pPBox = DMessageBox::showMessageBox(D_LOCAL_STRING("ConfirmDecomposeEquip").c_str(), this, messageBox_callfunc_selector(EquipModel::confirmDecomposeEquip));
			pPBox->setUserObject(pEquipInfoVec);
			return;
		}

	}*/

	doDecomposeEquip(pEquipInfoVec);
}

//void EquipModel::confirmSellEquip(MessageType type,DMessageBox* target)
//{
//    if(type == kMessageTypeCancel) return;
//    equip::EquipCurInfo* pEquipInfo = dynamic_cast<equip::EquipCurInfo*>(target->getUserObject());
//    if(pEquipInfo) doSellEquip(pEquipInfo->id);
//}
void EquipModel::confirmSellEquip(MessageType type,DMessageBox* target)
{
	if (type == kMessageTypeCancel)
	{
		return;
	}

	CCArray* pEquipInfoVec = dynamic_cast<CCArray*>(target->getUserObject());
	if(pEquipInfoVec) 
	{
		doSellEquip(pEquipInfoVec);
	}

	target->setUserObject(NULL);
}
void EquipModel::confirmDecomposeEquip(MessageType type, DMessageBox* target)
{
	if (type == kMessageTypeCancel)
	{
		return;
	}

	CCArray* pEquipInfoVec = dynamic_cast<CCArray*>(target->getUserObject());
	if(pEquipInfoVec) 
	{
		doDecomposeEquip(pEquipInfoVec);
	}

	target->setUserObject(NULL);
}

//void EquipModel::doSellEquip(uint32_t id)
//{
//    HTTP_CLIENT->sellEquips(id,this,callfuncJson_selector(EquipModel::sellEquipCallBack));
//}
void EquipModel::doSellEquip(CCArray* pEquipInfoVec)
{
	if (!pEquipInfoVec)
	{
		return;
	}

	Json::Value equips(Json::arrayValue);
	CCObject* obj = NULL;
	equip::EquipCurInfo* pEquipCurInfo = NULL;
	CCARRAY_FOREACH(pEquipInfoVec, obj)
	{
		pEquipCurInfo = (equip::EquipCurInfo*)obj;
		if (pEquipCurInfo)
		{
			equips.append(pEquipCurInfo->getId());
		}
	}

	if (equips.empty())
	{
		return;
	}

	//HTTP_CLIENT->decomposeEquip(equips);
	HTTP_CLIENT->sellEquips(equips,this,callfuncJson_selector(EquipModel::sellEquipCallBack));
}
void EquipModel::doDecomposeEquip(CCArray* pEquipInfoVec)
{
	if (!pEquipInfoVec)
	{
		return;
	}

	Json::Value equips(Json::arrayValue);
	CCObject* obj = NULL;
	equip::EquipCurInfo* pEquipCurInfo = NULL;
	CCARRAY_FOREACH(pEquipInfoVec, obj)
	{
		pEquipCurInfo = (equip::EquipCurInfo*)obj;
		if (pEquipCurInfo)
		{
			equips.append(pEquipCurInfo->getId());
		}
	}

	if (equips.empty())
	{
		return;
	}

	HTTP_CLIENT->decomposeEquip(equips);
}

void EquipModel::sellEquipCallBack(const Json::Value& requestData,const Json::Value& responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	int num = responseData["data"]["coin_change"].asInt();
	//获得物品的数量
	int itemnum = responseData["data"]["refinery_cost_change"].asInt();
	//Tips_Alert(D_LOCAL_STRING("YouGot%dCions",num).c_str());
	//Tips_Alert(D_LOCAL_STRING("EquipRefineTip6",itemnum).c_str());
	DM_GET_BURN_MODEL->setCoinShowNum(num);
	DM_GET_BURN_MODEL->setItemShowNum(itemnum);

	//最后加
	itemnum += DM_GET_BURN_MODEL->getEquipRefineryItemNum();
	DM_GET_BURN_MODEL->setEquipRefineryItemNum(itemnum);

	DM_GET_BURN_MODEL->postNotification(D_EVENT_BURN_ICON_UPDATE);
}

void EquipModel::decomposeEquipCallBack(const Json::Value& requestData,const Json::Value& responseData)
{
	
}

void EquipModel::refineEquipCallBack(const Json::Value& requestData,const Json::Value& responseData)
{

}

void EquipModel::getEquiped(CCArray *equiped)
{
	if (!equiped)
	{
		return;
	}

	equiped->removeAllObjects();
	
	CCObject *obj = NULL;
	CCARRAY_FOREACH(m_equipCurInfoVec, obj)
	{
		equip::EquipCurInfo *pInfo = static_cast<EquipCurInfo *>(obj);
		if (pInfo && pInfo->isEquiped())
		{
			equiped->addObject(pInfo);
		}
	}
}

bool EquipModel::needToEnhance()
{
	CCObject *obj = NULL;
	CCARRAY_FOREACH(m_equipCurInfoVec, obj)
	{
		equip::EquipCurInfo *pInfo = static_cast<EquipCurInfo *>(obj);
		if (pInfo && pInfo->isEquiped() && pInfo->isCanEnhance())
		{
			return true;
		}
	}
	return false;
}

bool EquipModel::hasUnequipedByType(EquipType equipType)
{
	CCObject* obj = NULL;
	EquipCurInfo* pEquipCurInfo = NULL;

	CCARRAY_FOREACH(m_equipCurInfoVec, obj)
	{
		pEquipCurInfo = (EquipCurInfo*)obj;
		if(!pEquipCurInfo->isEquiped() && (pEquipCurInfo->baseInfo->equip_type == equipType  || kEquipTypeALL == equipType))
		{
			return true;
		}

	}
	return false;
}

equip::EquipPropType EquipModel::getEquipPropType(equip::EquipCurInfo* pEquip, std::string& valueTypeLabel)
{
	equip::EquipPropType type = equip::kEquipPropAttack;
	std::string pecent;

	switch(pEquip->getBaseInfo()->getEquipType())
	{
	case equip::kEquipTypeWeapon:	//武器
		type = equip::kEquipPropAttack;
		break;
	case equip::kEquipTypeArmor:	//防具
		type = equip::kEquipPropDefence;
		break;
	case equip::kEquipTypeOrnament:	//饰品
		type = equip::kEquipPropHp;
		break;
	case equip::kEquipTypeTreasure:		//典籍 + 速度/躲闪/暴击/格挡
		{
			if(pEquip->getBaseInfo()->getSpeed()>0)
			{
				type = equip::kEquipPropSpeed;
			}
			else if(pEquip->getBaseInfo()->getDodgeRate()>0)
			{
				type = equip::kEquipPropDodge;
				pecent = "%";
			}
			else if(pEquip->getBaseInfo()->getCriticalRate()>0)
			{
				type = equip::kEquipPropCritical;
				pecent = "%";
			}
			else if(pEquip->getBaseInfo()->getBlockRate()>0)
			{
				type = equip::kEquipPropBlock;
				pecent = "%";
			}
			else if(pEquip->getBaseInfo()->getHitRate()>0)
			{
				type = equip::kEquipPropHit;
				pecent = "%";
			}
			else
			{
				CCLOG("error prop");
			}
			break;
		}
	default:
		CCLOG("error type");
		break;
	}
	if(!valueTypeLabel.empty())
	     valueTypeLabel = pecent;
	return type;
}

unsigned int EquipModel::computeEquipFight(equip::EquipCurInfo *pEquip)
{	
	unsigned int equipfight = 0;			//战斗力	
	unsigned int equipvalue = 0;		//属性值
	float ratio = 0.0f;							//比率

	//属性类型
	std::string valueTypeString;
	equip::EquipPropType type = getEquipPropType(pEquip , valueTypeString);
	equipvalue = computeEquipInfo(pEquip , type);

	switch(type)
	{
	case equip::kEquipPropAttack:ratio = 0.51f;break;		//攻
	case equip::kEquipPropDefence:ratio = 1.71f;break;	//防	
	case equip::kEquipPropHp:ratio = 0.15f;break;			//血	
	case equip::kEquipPropSpeed:ratio = 0.1f;break;		//速	
	default:break;		
	}

	equipfight = (int)(ratio*equipvalue);
	return equipfight;
}

bool EquipModel::doEnhanceCheck(equip::EquipCurInfo* pInfo , bool isAutoEnhance)
{

	if(isAutoEnhance)
	{
		//自动强化

		if(DM_GET_USER_MODEL->getUserInfo()->getVipLevel()< EQUIP_AUTOENHANCE_VIP_LEVEL && !DM_GET_USER_MODEL->checkPrivilegeType(user::PrivilegeTypeAutoEnhance))
		{
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV,CCString::create(D_LOCAL_STRING("NEED_VIP%d%d",DM_GET_USER_MODEL->getToVipLevelCost(EQUIP_AUTOENHANCE_VIP_LEVEL),EQUIP_AUTOENHANCE_VIP_LEVEL)));
			return false;
		}
	}
	//普通强化和自动强化

	//最大等级了？
	if(pInfo->checkLevelLimit())
	{
		Tips_Alert(D_LOCAL_STRING("EquipMaxLevel").c_str());
		return false;
	}

	//钱不够？
	if(DM_GET_USER_MODEL->getUserInfo()->getCoins()< getEquipEnhanceCost(pInfo))
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_COIN);
		return false;
	}

	//如果是宝物 强化石不够？
	if(pInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure)
	{
		unsigned int stonenum = DM_GET_ITEM_MODEL->getItemNum(TREASURE_ENHANCE_ITEM_ID);
		unsigned int stonecost = getTreasureEnhanceCostItem(pInfo);
		if(stonenum < stonecost)
		{
			//强化石的数量连强化一次都不够
			Tips_Alert(D_LOCAL_STRING("$NotEnoughQianghuashi").c_str());
			return false;
		}
	}

	//走到这里 终于可以强化了
	return true;
}
