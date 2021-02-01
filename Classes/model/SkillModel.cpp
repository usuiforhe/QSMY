//
//  SkillModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SkillModel.h"
#include "WarriorModel.h"
#include "../managers/GameManager.h"

USING_NS_CC;
using namespace skill;


bool Skill::isSpecial()
{
    return !(wid1==0 && wid2==0 && wid3==0);
}

float Skill::getAttackRatio(uint32_t slv)
{
	/************************************************************************/
	/* 1级：基本值 ； 1级以上计算                                                               */
	/************************************************************************/
    if(isSpecial())
	{
		//return attackRatio + (1 + grow*slv);

		//组合技从0开始
		return attackRatio + grow * ( slv );
	}
	else
	{
		//return attackRatio + (1 + grow * slv);
		return attackRatio + grow * ( slv - 1 );
	}

    ////不同品阶的技能
    //switch(grade)
    //{
    //    case skill::kSkillGradeRed:
    //        return attackRatio * (1+0.05f*(slv-1));
    //    case skill::kSkillGradePurple:
    //        return attackRatio * (1+0.0375f*(slv-1));
    //    default:
    //        return attackRatio * (1+0.0325f*(slv-1));
    //}
}

float Skill::getForceRatio(uint32_t slv)
{
    switch(grade)
    {
        case skill::kSkillGradeRed: // fix me
            return (2*(1+0.05f*(slv-1))-1)*0.4f;
        case skill::kSkillGradePurple:
            return (1.78*(1+0.0375f*(slv-1))-1)*0.4f;
		case skill::kSkillGradeBlue:
			return (1.59*(1+0.0375f*(slv-1))-1)*0.4f;
        default:
            return (1.42*(1+0.0325f*(slv-1))-1)*0.4f;
    }
}


SkillModel::SkillModel()
:m_Skills(NULL)
,m_SkillUpInfoVec(NULL)
,m_SkillColorMap(NULL)
,m_SkillZoneMap(NULL)
{
    
}

SkillModel::~SkillModel()
{
    CC_SAFE_RELEASE(m_Skills);
    CC_SAFE_RELEASE(m_SkillUpInfoVec);
    CC_SAFE_RELEASE(m_SkillColorMap);
    CC_SAFE_RELEASE(m_SkillZoneMap);
}

bool SkillModel::init()
{
    m_Skills = SkillMap::create();
	m_SkillUpInfoVec = SkillUpInfoVec::create();
    m_SkillColorMap = SkillColorMap::create();
	m_SkillZoneMap = SkillZoneMap::create();
    
    CC_SAFE_RETAIN(m_Skills);
    CC_SAFE_RETAIN(m_SkillUpInfoVec);
    CC_SAFE_RETAIN(m_SkillColorMap);
    CC_SAFE_RETAIN(m_SkillZoneMap);
    
    loadData();
    return Model::init();
}

void SkillModel::loadData()
{
    parseInfo( CSVDecoder::create(SKILL_CONFIG_PATH)->getData());
	loadUpExpData( CSVDecoder::create(SKILL_LV_EXP_CONFIG_PATH)->getData());
    loadColorConfig( CSVDecoder::create(SKILL_COLOR_CONFIG_PATH)->getData());
	loadZoneConfig( CSVDecoder::create(SKILL_ZONE_CONFIG_PATH)->getData());
}

void SkillModel::parseInfo(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
    {
        Skill *pSkill = Skill::create();
        pSkill->sid = atoi(val[i][0].c_str());
		pSkill->name = val[i][1].c_str();
		pSkill->grade = (SkillGrade)atoi(val[i][2].c_str());
		pSkill->type = (SkillType)atoi(val[i][4].c_str());
		pSkill->desc = val[i][5].c_str();
		pSkill->wid1 = atoi(val[i][6].c_str());
		pSkill->wid2 = atoi(val[i][7].c_str());
		pSkill->wid3 = atoi(val[i][8].c_str());
        pSkill->trigger_probability = val[i][9].c_str();
		pSkill->zone = atoi(val[i][10].c_str());
		pSkill->attackRatio = atof(val[i][13].c_str());
        pSkill->have_resource = CCString::create(val[i][15].c_str())->boolValue();
        pSkill->story = val[i][16].c_str();
        pSkill->m_bHighQuality = atoi(val[i][17].c_str())==1;
        pSkill->res_id = atoi(val[i][18].c_str());
		pSkill->grow = atof(val[i][19].c_str());
		pSkill->m_uSkillFlip = atoi(val[i][20].c_str());
		pSkill->m_uHitFlip = atoi(val[i][21].c_str());
        m_Skills->setObject(pSkill, pSkill->sid);
    }
}

void SkillModel::loadColorConfig(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
        SkillColorConfig *pSkillColorConfig = SkillColorConfig::create();
        SkillGrade grade = (SkillGrade)atoi(val[i][0].c_str());
		uint32_t top = 0;
        sscanf(val[i][1].c_str(), "%i",&top);
        uint32_t bottom = 0;
        sscanf(val[i][2].c_str(), "%i",&bottom);
        
        uint32_t color = 0;
        sscanf(val[i][3].c_str(), "%i",&color);
        
        pSkillColorConfig->top = ccc3(top>>16&0xFF, top>>8&0xFF, top>>0&0xFF);
        pSkillColorConfig->bottom = ccc3(bottom>>16&0xFF, bottom>>8&0xFF, bottom>>0&0xFF);
		pSkillColorConfig->color = ccc3(color>>16&0xFF, color>>8&0xFF, color>>0&0xFF);
        m_SkillColorMap->setObject(pSkillColorConfig, grade);
	}
}

void SkillModel::loadZoneConfig(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		SkillZoneInfo *pSkillZoneInfo = SkillZoneInfo::create();
		pSkillZoneInfo->zid = atoi(val[i][0].c_str());
		pSkillZoneInfo->desc = val[i][1].c_str();
		
        m_SkillZoneMap->setObject(pSkillZoneInfo, pSkillZoneInfo->zid);
	}
}


void SkillModel::loadUpExpData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		SkillUpInfo *pSkillUpInfo = SkillUpInfo::create();
		pSkillUpInfo->grade = (SkillGrade)atoi(val[i][0].c_str());
		pSkillUpInfo->lv = atoi(val[i][1].c_str());
		pSkillUpInfo->exp = atoi(val[i][2].c_str());
        m_SkillUpInfoVec->addObject(pSkillUpInfo);
	}
}

uint32_t SkillModel::getSkillUpTotalAmount(uint16_t grade, uint16_t lv)
{
	if(lv <= 1) return 0;

	//下面开始计算 技能升级所消耗的物品数总和
	uint32_t totalamount = 0;
	CCObject *obj = NULL;
	SkillUpInfo *pSkillUpInfo  = NULL;
	CCARRAY_FOREACH(m_SkillUpInfoVec, obj)
	{
		pSkillUpInfo = (SkillUpInfo *)obj;
		if(pSkillUpInfo->lv == lv && pSkillUpInfo->grade == grade)
			totalamount = pSkillUpInfo->exp;
	}
	return totalamount;
}

uint32_t SkillModel::getSkillUpExp(uint16_t grade, uint16_t lv)
{
    CCObject *obj = NULL;
    SkillUpInfo *pSkillUpInfo  = NULL;
    CCARRAY_FOREACH(m_SkillUpInfoVec, obj)
    {
        pSkillUpInfo = (SkillUpInfo *)obj;
        if(pSkillUpInfo->lv == lv+1 && pSkillUpInfo->grade == grade)
            return pSkillUpInfo->exp;
    }
    
	return 0;
}

uint16_t SkillModel::getSkillLevelByExp(uint32_t skillID, uint32_t exp)
{
	Skill *pSkill = (skill::Skill*)m_Skills->objectForKey(skillID);
	if (!pSkill)
	{
		return 0;
	}
	
	uint16_t grade = pSkill->grade;
	for (uint16_t i = 1; i < SKILL_LV_MAX; i++)
	{
		if (exp < getSkillUpExp(grade, i))
		{
			return i;
		}
	}
	return SKILL_LV_MAX;
}

int8_t SkillModel::getSpecialSkillLv(skill::Skill* pSkill)
{
    if(!pSkill) return -1;
    warrior::WarriorCurInfo *pWarrior = NULL;
    uint8_t upLevel  = -1; //max
    
    uint8_t warrior_num = 0;	//技能需要合体弟子个数
    uint8_t inpos_num = 0;		//在阵容中个数
    
    uint32_t wids[] = {pSkill->wid1,pSkill->wid2,pSkill->wid3};
    
    for (uint8_t i=0;i<D_CONFIG_UINT_FOR_KEY(SKILL_SPECIAL_MAXWARRIOR);i++)
    {
        pWarrior = NULL;
        
        if(wids[i]<=0) continue;
        
        if(DM_GET_FORMATION_MODEL->isWarriorBattle(wids[i]) || DM_GET_GUNDAM_EF_MODEL->isEgineerBattle(wids[i]))
        {
            inpos_num++;
            pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wids[i]);
            if(upLevel>pWarrior->getWarriorUpLv()) upLevel=pWarrior->getWarriorUpLv();
        }
        warrior_num++;
    }
    if (warrior_num == inpos_num) return upLevel;
    return -1;
}

void SkillModel::getSpecialSkills(CCArray *skillArray)
{
	if (!skillArray)
	{
		return;
	}
	skillArray->removeAllObjects();
	skill::Skill *pSkill = NULL;
	CCDictElement *pElement = NULL;
	CCDICT_FOREACH(m_Skills, pElement){
		pSkill = static_cast<skill::Skill *>(pElement->getObject());
		if (pSkill->isSpecial())
		{
			skillArray->addObject(pSkill);
		}
	}
}