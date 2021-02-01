//
//  WarriorModel.cpp
//  QSMY
//
//  Created by cll on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "WarriorModel.h"
#include "AppInclude.h"
#include <algorithm>
#include "../managers/GameManager.h"
#include "managers/AnalyticsManager.h"
#include "utils/StringUtil.h"

USING_NS_CC;

using namespace warrior;

warrior::WarriorHandBookInfo* WarriorModel::m_pWarriorHandBookInfo=WarriorHandBookInfo::create();

warrior::WarriorHandBookInfo* WarriorModel::getWarriorHandBookInfo()
{
	return WarriorModel::m_pWarriorHandBookInfo;
}

//int  WarriorModel::getPictureCount()
//{
	//return warrior::WarriorHandBookInfo::m_nCount;
//}
#pragma mark - WarriorCurInfo

void WarriorCurInfo::unDress()
{
    if(getDressId()==0) return;
    HTTP_CLIENT->unDress(getWid());
}

float WarriorCurInfo::getUpPercent(int8_t warriorUpLv)
{
    return 100*(1+(WARRIOR_UP_RATIO*warriorUpLv));
}

float WarriorCurInfo::getUpPercentCurrentLv()
{
    return getUpPercent(this->warriorUpLv);
}

float WarriorCurInfo::getUpPercentNextLv()
{
    return getUpPercent(this->warriorUpLv+1);
}

/* 技能是否升到最大级*/
bool WarriorCurInfo::checkSkillLimit()
{
    return slv >= SKILL_LV_MAX;
};

//获取弟子进阶需要的等级
unsigned WarriorCurInfo::getWarriorLevelForPromotion()
{
    WarriorUpSoul* upInfo = DM_GET_WARRIOR_MODEL->getWarriorUpInfo(this);
    return upInfo->getLevelLimit();
}

//弟子等级是否满足限制
bool WarriorCurInfo::checkWarriorLevelForPromotion()
{
    return warriorLv >= getWarriorLevelForPromotion();
}


/* 是否突破到最大级*/
bool WarriorCurInfo::checkPromoteLimit()
{
    
    return warriorUpLv >= WARRIOR_UP_LV_MAX;
};



bool WarriorCurInfo::checkDevelopLevel()
{
    return warriorLv >= WARRIOR_DEV_LV_MIN;
};




/* 是否等级到达限制*/
bool WarriorCurInfo::checkLevelMax()
{
    return warriorLv >= USER_WARRIOR_LEVEL_RATIO * D_CONFIG_UINT_FOR_KEY(USER_LEVEL_MAX);
}

/* 是否等级到达限制*/
bool WarriorCurInfo::checkLevelLimit()
{
    return DM_GET_WARRIOR_MODEL->checkLevelLimit(warriorLv);
}

bool WarriorModel::checkLevelLimit(uint16_t lv)
{
    return lv >= USER_WARRIOR_LEVEL_RATIO*DM_GET_USER_MODEL->getUserInfo()->getUserLv();
}

unsigned int WarriorModel::getSoulCount(unsigned int wid)
{
    warrior::SoulInfo *pInfo = getSoulInfoByWID(wid);
    if(pInfo==NULL) return 0;
	return pInfo->getNum();
}


unsigned int WarriorCurInfo::getSoulCount()
{
	return DM_GET_WARRIOR_MODEL->getSoulCount(wid);
}

void WarriorCurInfo::resetWarriorCurInfo()
{
    //属性默认值
    warriorLv = 1;
    warriorExp = 0;
    //warriorUpLv = 0;
    sexp = 0;
    slv = 1;
    hp = 0;
    attack = 0;
    defence = 0;
    potential = 0;
    costedPotential = 0;
	hp_add = 0;
	attack_add = 0;
	defence_add = 0;
    computeWarriorProps();

	postNotification(D_EVENT_WARRIORINFO_UPDATE);
}

//计算潜力
void WarriorCurInfo::computePotential()
{
    //等级潜力扣除已消耗的潜力为剩余潜力
    potential = DM_GET_WARRIOR_MODEL->getWarriorPotentialByLevel(warriorLv, warriorUpLv, baseInfo) - costedPotential;
}

unsigned int warrior::WarriorCurInfo::getWarriorNextUpLevelPotential()
{
    return DM_GET_WARRIOR_MODEL->getWarriorPotentialByLevel(warriorLv, warriorUpLv + 1, baseInfo) - costedPotential;
}

void WarriorCurInfo::computeWarriorProps()
{
    if (!this->baseInfo)
		this->baseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(this->wid);
    if(!this->baseInfo)
    {
        CCLOG("error warrior!, id:%d", this->wid);
        return;
    }  
	computeTalentProps();
	this->hp = WarriorModel::computeBasicAddProps(this->baseInfo->getWarriorId() , this->baseInfo->getRealGrade(), this->baseInfo->getBasicHp(), this->baseInfo->getBasicHpAdd(), this->warriorLv, this->warriorUpLv , warrior::kTalentTypeHP)+ this->hp_add;
	this->attack = WarriorModel::computeBasicAddProps(this->baseInfo->getWarriorId() , this->baseInfo->getRealGrade(), this->baseInfo->getBasicAttack(), this->baseInfo->getBasicAttackAdd(), this->warriorLv, this->warriorUpLv , warrior::kTalentTypeAttack)+ this->attack_add;
	this->defence = WarriorModel::computeBasicAddProps(this->baseInfo->getWarriorId() , this->baseInfo->getRealGrade(), this->baseInfo->getBasicDefence(), this->baseInfo->getBasicDefenceAdd(), this->warriorLv, this->warriorUpLv , warrior::kTalentTypeDefence)+this->defence_add;

	//this->hp = (this->baseInfo->getBasicHp() + this->baseInfo->getBasicHpAdd() * this->warriorLv + hp_talentAdd) * (1 + WARRIOR_UP_RATIO * this->warriorUpLv) + this->hp_add;
	//this->attack = (this->baseInfo->getBasicAttack() + this->baseInfo->getBasicAttackAdd() * this->warriorLv + attack_talentAdd) * (1 + WARRIOR_UP_RATIO * this->warriorUpLv) + this->attack_add;
	//this->defence = (this->baseInfo->getBasicDefence() + this->baseInfo->getBasicDefenceAdd() * this->warriorLv + defence_talentAdd) * (1 + WARRIOR_UP_RATIO * this->warriorUpLv) + this->defence_add;
}

void WarriorCurInfo::computeTalentProps()
{
	CCDictionary* talentDic = DM_GET_WARRIOR_MODEL->getWarriorTalentsDic(this->wid);
	if (talentDic == NULL)
	{
		return;
	}
	
	CCArray* pKeys = talentDic->allKeys(); 
	if (pKeys == NULL || pKeys->count() == 0)
	{
		return;
	}
	
	for (unsigned int i = 0; i < pKeys->count(); i++)
	{
		int openLevel;
		CCInteger* pIntKey = dynamic_cast<CCInteger*>(pKeys->objectAtIndex(i));
		if (pIntKey)
		{
			openLevel = pIntKey->getValue();
		}
		else
		{
			CCString* pStringKey = dynamic_cast<CCString*>(pKeys->objectAtIndex(i));
			if (pStringKey == NULL)
			{
				continue;
			}
			openLevel = atoi(pStringKey->getCString());
		}

		CCObject* pObject = talentDic->objectForKey(openLevel);
		CCInteger* talentId = dynamic_cast<CCInteger*>(pObject);
		if (talentId == NULL)
		{
			continue;
		}
		
		warrior::TalentInfo* talentInfo = DM_GET_WARRIOR_MODEL->getTalentInfoById(talentId->getValue());
		if (talentInfo == NULL)
		{
			continue;
		}
		
		if (this->warriorUpLv < openLevel)
		{
			continue;
		}

		switch(talentInfo->type)
		{
		case warrior::kTalentTypeAttack:
			 this->attack_talentAdd = talentInfo->_value;
			break;
		case warrior::kTalentTypeDefence:
			this->defence_talentAdd = talentInfo->_value;
			break;
		case warrior::kTalentTypeHP:
			this->hp_talentAdd =  talentInfo->_value;
			break;
		default:
			break;
		}
	}
}

uint32_t WarriorCurInfo::computeNextUpLevelWarriorProps(WarriorPropType propType)
{
	uint32_t addProps = 0;
	uint32_t nextUpLevelProps = 0;
	switch(propType)
	{
	case kPropTypeAttak:
		//nextUpLevelProps = (this->baseInfo->getBasicAttack() + this->baseInfo->getBasicAttackAdd() * this->warriorLv + attack_talentAdd) * (1 + WARRIOR_UP_RATIO * this->warriorUpLv) + this->attack_add;
		nextUpLevelProps = WarriorModel::computeBasicAddProps(this->baseInfo->getWarriorId() , this->baseInfo->getRealGrade(), this->baseInfo->getBasicAttack(), this->baseInfo->getBasicAttackAdd(), this->warriorLv, this->warriorUpLv + 1 , warrior::kTalentTypeAttack)+ this->attack_add;
		addProps = nextUpLevelProps - this->attack;
		break;
	case kPropTypeDefence:
		//nextUpLevelProps = (this->baseInfo->getBasicDefence() + this->baseInfo->getBasicDefenceAdd() * this->warriorLv + defence_talentAdd) * (1 + WARRIOR_UP_RATIO * this->warriorUpLv) + this->defence_add;
		nextUpLevelProps = WarriorModel::computeBasicAddProps(this->baseInfo->getWarriorId() , this->baseInfo->getRealGrade(), this->baseInfo->getBasicDefence(), this->baseInfo->getBasicDefenceAdd(), this->warriorLv, this->warriorUpLv + 1 , warrior::kTalentTypeDefence)+this->defence_add;
		addProps = nextUpLevelProps - this->defence;
		break;
	case kPropTypeHp:
		//nextUpLevelProps = (this->baseInfo->getBasicHp() + this->baseInfo->getBasicHpAdd() * this->warriorLv + hp_talentAdd) * (1 + WARRIOR_UP_RATIO * this->warriorUpLv) + this->hp_add;
		nextUpLevelProps = WarriorModel::computeBasicAddProps(this->baseInfo->getWarriorId() , this->baseInfo->getRealGrade(), this->baseInfo->getBasicHp(), this->baseInfo->getBasicHpAdd(), this->warriorLv, this->warriorUpLv + 1 , warrior::kTalentTypeHP)+ this->hp_add;
		addProps = nextUpLevelProps - this->hp;
		break;
	case kPropTypeSpeed:
		addProps = 0;
		break;
	case kPropTypeAll:
		return computeNextUpLevelWarriorForce();
		break;
	}

	return computeWarriorFinalProps(propType) + addProps;
}

/*计算弟子属性装备加成*/
uint32_t WarriorCurInfo::computeWarriorFinalProps(WarriorPropType propType, formation::PositionInfo * _pPosInfo/* = NULL*/)
{
    float mix_hp = this->hp;
	float mix_defence = this->defence;
	float mix_attack = this->attack;
	float mix_speed = this->baseInfo->getBasicSpeed();
    uint32_t return_value = 0;		//返回值
    float mix_skill = 0.f;
    
    float dodge = 0.f;
    float critical = 0.f;
    float block = 0.f;
    
    if(DM_GET_WARRIOR_MODEL->isMyWarrior(this))
	{
        float fate_total_attack = 0;
        float fate_total_defence = 0;
        float fate_total_hp = 0;
        //缘属性加成 增加自身攻防血，不含装备

        FateInfo* pFateInfo = NULL;
        CCObject *obj = NULL;

        CCARRAY_FOREACH(baseInfo->getFateArray(), obj)
        {
            pFateInfo = (FateInfo*)obj;
        
            if(DM_GET_WARRIOR_MODEL->checkFateActivated(pFateInfo))
            {
                fate_total_attack += ((float)pFateInfo->add_attack * this->attack*0.01f);
                fate_total_defence += ((float)pFateInfo->add_defence * this->defence*0.01f);
                fate_total_hp += ((float)pFateInfo->add_hp * this->hp*0.01f);
            }
        }

		mix_attack += fate_total_attack;
		mix_defence += fate_total_defence;
		mix_hp += fate_total_hp;


		formation::PositionInfo * pPosInfo = _pPosInfo;
		if(pPosInfo == NULL)
		{
			pPosInfo = DM_GET_FORMATION_MODEL->getPositionInfoByWid(this->wid);
		}

		//宝物加成
		float treasure_attack = 0;
		float treasure_defence = 0;
		float treasure_hp = 0;
		float treasure_speed = 0;
		float treasure_dodge = 0;
		float treasure_critical = 0;
		float treasure_block = 0;
		
		if (pPosInfo)
		{
			cocos2d::CCDictElement* pElement = NULL;
			equip::EquipCurInfo * pEquipInfo = NULL;
			cocos2d::CCDictionary* pDic = DM_GET_EQUIP_MODEL->getEquipedsById(pPosInfo->iId);
			CCDICT_FOREACH(pDic, pElement)
			{
				pEquipInfo = (equip::EquipCurInfo *)pElement->getObject();
				switch (pEquipInfo->getBaseInfo()->getEquipType()) {
				case equip::kEquipTypeTreasure: //典籍 + 速度/躲闪/暴击/格挡
					{
						if(pEquipInfo->getLevel() >= 5)
						{
							if(pEquipInfo->getBaseInfo()->getLevelType5() == equip::kTreasurePropAttack)treasure_attack += pEquipInfo->getBaseInfo()->getLevelRate5();
							if(pEquipInfo->getBaseInfo()->getLevelType5() == equip::kTreasurePropDefence)treasure_defence += pEquipInfo->getBaseInfo()->getLevelRate5();
							if(pEquipInfo->getBaseInfo()->getLevelType5() == equip::kTreasurePropHp)treasure_hp += pEquipInfo->getBaseInfo()->getLevelRate5();
							if(pEquipInfo->getBaseInfo()->getLevelType5() == equip::kTreasurePropSpeed)treasure_speed += pEquipInfo->getBaseInfo()->getLevelRate5();
							if(pEquipInfo->getBaseInfo()->getLevelType5() == equip::kTreasurePropDodge)treasure_dodge += pEquipInfo->getBaseInfo()->getLevelRate5();
							if(pEquipInfo->getBaseInfo()->getLevelType5() == equip::kTreasurePropCritical)treasure_critical += pEquipInfo->getBaseInfo()->getLevelRate5();
							if(pEquipInfo->getBaseInfo()->getLevelType5() == equip::kTreasurePropBlock)treasure_block += pEquipInfo->getBaseInfo()->getLevelRate5();
							if(pEquipInfo->getBaseInfo()->getLevelType5() == equip::kTreasurePropPercentAttack)treasure_attack += pEquipInfo->getBaseInfo()->getLevelRate5() * 0.01f * this->attack;
							if(pEquipInfo->getBaseInfo()->getLevelType5() == equip::kTreasurePropPercentDefence)treasure_defence += pEquipInfo->getBaseInfo()->getLevelRate5() * 0.01f * this->defence;
							if(pEquipInfo->getBaseInfo()->getLevelType5() == equip::kTreasurePropPercentHp)treasure_hp += pEquipInfo->getBaseInfo()->getLevelRate5() * 0.01f * this->hp;
						}
						if(pEquipInfo->getLevel() >= 10)
						{
							if(pEquipInfo->getBaseInfo()->getLevelType10() == equip::kTreasurePropAttack)treasure_attack += pEquipInfo->getBaseInfo()->getLevelRate10();
							if(pEquipInfo->getBaseInfo()->getLevelType10() == equip::kTreasurePropDefence)treasure_defence += pEquipInfo->getBaseInfo()->getLevelRate10();
							if(pEquipInfo->getBaseInfo()->getLevelType10() == equip::kTreasurePropHp)treasure_hp += pEquipInfo->getBaseInfo()->getLevelRate10();
							if(pEquipInfo->getBaseInfo()->getLevelType10() == equip::kTreasurePropSpeed)treasure_speed += pEquipInfo->getBaseInfo()->getLevelRate10();
							if(pEquipInfo->getBaseInfo()->getLevelType10() == equip::kTreasurePropDodge)treasure_dodge += pEquipInfo->getBaseInfo()->getLevelRate10();
							if(pEquipInfo->getBaseInfo()->getLevelType10() == equip::kTreasurePropCritical)treasure_critical += pEquipInfo->getBaseInfo()->getLevelRate10();
							if(pEquipInfo->getBaseInfo()->getLevelType10() == equip::kTreasurePropBlock)treasure_block += pEquipInfo->getBaseInfo()->getLevelRate10();
							if(pEquipInfo->getBaseInfo()->getLevelType10() == equip::kTreasurePropPercentAttack)treasure_attack += pEquipInfo->getBaseInfo()->getLevelRate10() * 0.01f * this->attack;
							if(pEquipInfo->getBaseInfo()->getLevelType10() == equip::kTreasurePropPercentDefence)treasure_defence += pEquipInfo->getBaseInfo()->getLevelRate10() * 0.01f * this->defence;
							if(pEquipInfo->getBaseInfo()->getLevelType10() == equip::kTreasurePropPercentHp)treasure_hp += pEquipInfo->getBaseInfo()->getLevelRate10() * 0.01f * this->hp;
						}
						break;
					}

				default:
					break;
				}
			}
		}
		//宝物加成结算
		mix_attack += treasure_attack;
		mix_defence += treasure_defence;
		mix_hp += treasure_hp;
		mix_speed += treasure_speed;
		dodge += treasure_dodge;
		critical += treasure_critical;
		block += treasure_block;
        
        
        if(getDressId()!=0)
        {
            avatar::AvatarInfo* pAvatarInfo = DM_GET_AVATAR_MODEL->getAvatarInfoById(getDressId());
            if(pAvatarInfo)
            {
				//TODO
                //mix_attack += this->attack*pAvatarInfo->getBaseInfo()->getAttack();
                //mix_defence += this->defence*pAvatarInfo->getBaseInfo()->getDefence();
                //mix_hp += this->hp*pAvatarInfo->getBaseInfo()->getHp();
                //mix_speed += pAvatarInfo->getBaseInfo()->getSpeed();
                //dodge += pAvatarInfo->getBaseInfo()->getDodgeRate()*0.01f;
                //critical += pAvatarInfo->getBaseInfo()->getCriticalRate()*0.01f;
                //block += pAvatarInfo->getBaseInfo()->getBlockRate()*0.01f;
            }
        }
        
        
        //计算天命加成
        mix_attack += DM_GET_DESTINY_MODEL->destinyAddProps(kPropTypeAttak);
        mix_defence += DM_GET_DESTINY_MODEL->destinyAddProps(kPropTypeDefence);
        mix_hp += DM_GET_DESTINY_MODEL->destinyAddProps(kPropTypeHp);
        //只有阵容上的英雄计算小伙伴加成
		if(DM_GET_FORMATION_MODEL->isWarriorBattle(this->wid))
		{
			//计算小伙伴加成
			uint32_t gundam_addAttack,gundam_addDefence,gundam_addHp;
			gundam_addAttack=gundam_addDefence=gundam_addHp=0;
			DM_GET_GUNDAM_EF_MODEL->calcGundamPropertyAdd(&gundam_addAttack, &gundam_addDefence, &gundam_addHp);
			//CCLOG("FormationWarriror Gundam property add ---------gundam_addAttack:%d, gundam_addDefence:%d, gundam_addHp:%d", gundam_addAttack, gundam_addDefence, gundam_addHp);
			mix_attack += gundam_addAttack;
			mix_defence += gundam_addDefence;
			mix_hp += gundam_addHp;
		}
        //装备属性加成
        if (pPosInfo)
        {
            cocos2d::CCDictElement* pElement = NULL;
            equip::EquipCurInfo * pEquipInfo = NULL;
            cocos2d::CCDictionary* pDic = DM_GET_EQUIP_MODEL->getEquipedsById(pPosInfo->iId);
            CCDICT_FOREACH(pDic, pElement)
            {
                pEquipInfo = (equip::EquipCurInfo *)pElement->getObject();
                switch (pEquipInfo->getBaseInfo()->getEquipType()) {
                    case equip::kEquipTypeWeapon://加功
                        mix_attack += DM_GET_EQUIP_MODEL->computeEquipInfo(pEquipInfo, equip::kEquipPropAttack);
                        break;
                    case equip::kEquipTypeArmor://加防
                        mix_defence += DM_GET_EQUIP_MODEL->computeEquipInfo(pEquipInfo, equip::kEquipPropDefence);
                        break;
                    case equip::kEquipTypeOrnament://饰品加血
                        mix_hp += DM_GET_EQUIP_MODEL->computeEquipInfo(pEquipInfo, equip::kEquipPropHp);
                        break;
                    case equip::kEquipTypeTreasure: //典籍 + 速度/躲闪/暴击/格挡
                    {
                        if(pEquipInfo->getBaseInfo()->getSpeed()>0)
                        {
                            mix_speed += DM_GET_EQUIP_MODEL->computeEquipInfo(pEquipInfo,equip::kEquipPropSpeed);
                        }
                        else if(pEquipInfo->getBaseInfo()->getDodgeRate()>0)
                        {
                            dodge += DM_GET_EQUIP_MODEL->computeEquipInfo(pEquipInfo,equip::kEquipPropDodge)/100.f;
                            
                        }
                        else if(pEquipInfo->getBaseInfo()->getCriticalRate()>0)
                        {
                            critical = DM_GET_EQUIP_MODEL->computeEquipInfo(pEquipInfo,equip::kEquipPropCritical)/100.f;
                            
                        }
                        else if(pEquipInfo->getBaseInfo()->getBlockRate()>0)
                        {
                            block = DM_GET_EQUIP_MODEL->computeEquipInfo(pEquipInfo,equip::kEquipPropBlock)/100.f;
                        }
                        break;
                    }
                        
                    default:
                        break;
                }
            }
        }
    }
	switch(propType)
	{
        case kPropTypeAttak:
            return_value = mix_attack;
            break;
        case kPropTypeDefence:
            return_value = mix_defence;
            break;
        case kPropTypeHp:
            return_value = mix_hp;
            break;
        case kPropTypeSpeed:
            return_value = mix_speed;
            break;
        case kPropTypeAll:
        default:
            skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(this->getBaseInfo()->getSkillId());
            if(pSkill)
            {
                mix_skill = pSkill->getForceRatio(this->slv);
            }
            return_value = WarriorModel::computeWarriorForce(mix_hp,mix_attack,mix_defence,mix_speed,dodge,block,critical,mix_skill);
	}
    
    
	return return_value;
	
}

uint32_t WarriorModel::computeWarriorForce(float hp,float attack,float defence,float speed,float dodge,float block,float critical,float skillRatio)
{
    //return (uint32_t)(hp/((1-dodge)*(1-(1-dodge)*block*0.7f))*(attack*(1+0.5f*critical)*(1+skillRatio)*speed/500+attack*(1-dodge)*block*0.6f+defence)/1000);
	uint32_t force = (uint32_t)(hp * 0.15f + attack * 0.51f + defence * 1.71f + speed * 0.1f);
	return force;
}

uint32_t WarriorCurInfo::computeWarriorForce(formation::PositionInfo * pPosInfo/* = NULL*/)
{
	return computeWarriorFinalProps(kPropTypeAll, pPosInfo);
}

uint32_t warrior::WarriorCurInfo::computeNextUpLevelWarriorForce()
{
	float addPercent = getUpPercentNextLv() - getUpPercentCurrentLv();
	uint32_t nextFight = (uint32_t)(computeWarriorForce() * (1 + addPercent * 0.01));
	return nextFight;
}

//增加经验
void WarriorCurInfo::addExp(int exp, float fDelayTime,bool* isLevelUp)
{
    if(checkLevelMax()) return;
    if(isLevelUp) *isLevelUp = false;
	if (exp > 0)
	{
		warriorExp += exp;
		/************************************************************************/
		/* 逻辑全部完成后，再post								                                        */
		/************************************************************************/
		//postNotification(D_EVENT_WARRIORINFO_UPDATE, fDelayTime);
        uint16_t currLevel = warriorLv;
        warriorLv = DM_GET_WARRIOR_MODEL->getWarriorLevelByExp(warriorExp, baseInfo->getRealGrade(), currLevel);
        if(checkLevelMax()) warriorExp = DM_GET_WARRIOR_MODEL->getWarriorAddExp(warriorLv,baseInfo->getRealGrade());
		if (warriorLv > currLevel)
		{
			if(isLevelUp) *isLevelUp = true;
            computePotential();     //重新计算潜力
			computeWarriorProps();	//重新计算攻防血
		}
		/************************************************************************/
		/* 修改了post的位置，不能更新了一半就post！@fengkerong.billy            */
		/************************************************************************/
		postNotification(D_EVENT_WARRIORINFO_UPDATE, fDelayTime);
	}

}

//突破等级加1
void  WarriorCurInfo::addUpLevel()
{
	warriorUpLv++;		
	computeWarriorProps();	//重新计算攻防血
	computePotential();
	postNotification(D_EVENT_WARRIORINFO_UPDATE);
}

WarriorCurInfo* WarriorCurInfo::create(unsigned int wid)
{
    warrior::WarriorBaseInfo* pBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wid);
    return WarriorCurInfo::create(pBase);
}

WarriorCurInfo* WarriorCurInfo::create()
{
    warrior::WarriorCurInfo * pRet = new warrior::WarriorCurInfo();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE(pRet);
    return NULL;
}

WarriorCurInfo* WarriorCurInfo::create(WarriorBaseInfo* pInfo)
{
    if(pInfo==NULL) return NULL;
    
    warrior::WarriorCurInfo * pRet = warrior::WarriorCurInfo::create();
    if(pRet)
    {
        pRet->wid = pInfo->getWarriorId();
        pRet->baseInfo = pInfo;
        pRet->computeWarriorProps();
    }
    return pRet;
}


#pragma mark - WarriorDialogInfo -

cocos2d::CCString * WarriorDialogInfo::getDialog()
{
    if(m_pDialogList->count() == 0)
    {
        return NULL;
    }
    
    return (cocos2d::CCString *)m_pDialogList->objectAtIndex(rand() % m_pDialogList->count());
}


#pragma mark - WarriorModel -


WarriorModel::WarriorModel()
:m_warFateInfoMap(NULL)
,m_warHandBookMap(NULL)
,m_warBaseInfoMap(NULL)
,m_warDialogMap(NULL)
,m_warGrowInfoMap(NULL)
,m_warPotBuffMap(NULL)
,m_warCurInfoVec(NULL)
,m_warUpSoulVec(NULL)
,m_soulInfoVec(NULL)
,m_warIconInfoMap(NULL)
,m_warriorUpgadeCriticalMap(NULL)
,m_warriorUpgradeItemMap(NULL)
,m_warriorUpgradeAccumulationMap(NULL)
,m_pWarriorLevelExpArr(NULL)
,m_talentInfoMap(NULL)
,m_warriorTalentMap(NULL)
,m_warriorPotentialFactorVec(NULL)
{
	//init();
}

WarriorModel::~WarriorModel()
{
	CC_SAFE_RELEASE(m_warFateInfoMap);
    CC_SAFE_RELEASE(m_warBaseInfoMap);
	CC_SAFE_RELEASE(m_warHandBookMap);
    CC_SAFE_RELEASE(m_warDialogMap);
	CC_SAFE_RELEASE(m_warGrowInfoMap);
	CC_SAFE_RELEASE(m_warPotBuffMap);
	CC_SAFE_RELEASE(m_warCurInfoVec);
    CC_SAFE_RELEASE(m_warUpSoulVec);
    CC_SAFE_RELEASE(m_soulInfoVec);
	CC_SAFE_RELEASE(m_warIconInfoMap);
	CC_SAFE_RELEASE(m_warriorUpgadeCriticalMap);
    CC_SAFE_RELEASE(m_warriorUpgradeItemMap);
    CC_SAFE_RELEASE(m_warriorUpgradeAccumulationMap);
	CC_SAFE_RELEASE(m_pWarriorLevelExpArr);
	CC_SAFE_RELEASE(m_talentInfoMap);
	CC_SAFE_RELEASE(m_warriorTalentMap);
    CC_SAFE_RELEASE(m_warriorPotentialFactorVec);
}



bool WarriorModel::init()
{
	m_warFateInfoMap =  WarriorFateMap::create();
	m_warHandBookMap =  WarriorHandBookMap::create();
    m_warBaseInfoMap =  WarriorBaseMap::create();
    m_warGrowInfoMap =  WarriorGrowMap::create();
    m_warDialogMap   =  WarriorDialogMap::create();
    m_warPotBuffMap  =  PotBuffMap::create();
    m_warCurInfoVec  =  WarriorCurInfoVec::create();
    m_warUpSoulVec   =  WarriorUpSoulVec::create();
    m_soulInfoVec    =  SoulInfoVec::create();
    m_pWarriorLevelExpArr = CCArray::create();
	m_warIconInfoMap = WarriorIconInfoMap::create();
    m_warriorUpgradeItemMap = WarriorUpgradeItemMap::create();
    m_warriorUpgradeAccumulationMap = WarriorUpgradeAccumulationMap::create();
    m_warriorUpgadeCriticalMap = WarriorUpgadeCriticalMap::create();
	m_talentInfoMap = TalentInfoMap::create();
	m_warriorTalentMap = WarriorTalentMap::create();
    m_warriorPotentialFactorVec = WarriorPotentialFactorVec::create();
    
	CC_SAFE_RETAIN(m_warFateInfoMap);
    CC_SAFE_RETAIN(m_warBaseInfoMap);
	CC_SAFE_RETAIN(m_warHandBookMap);
	CC_SAFE_RETAIN(m_warGrowInfoMap);
    CC_SAFE_RETAIN(m_warDialogMap);
	CC_SAFE_RETAIN(m_warPotBuffMap);
	CC_SAFE_RETAIN(m_warCurInfoVec);
    CC_SAFE_RETAIN(m_warUpSoulVec);
    CC_SAFE_RETAIN(m_soulInfoVec);
    CC_SAFE_RETAIN(m_pWarriorLevelExpArr);
    CC_SAFE_RETAIN(m_warIconInfoMap);
    CC_SAFE_RETAIN(m_warriorUpgradeItemMap);
    CC_SAFE_RETAIN(m_warriorUpgradeAccumulationMap);
    CC_SAFE_RETAIN(m_warriorUpgadeCriticalMap);
	CC_SAFE_RETAIN(m_talentInfoMap);
	CC_SAFE_RETAIN(m_warriorTalentMap);
    CC_SAFE_RETAIN(m_warriorPotentialFactorVec);
	loadWarriorLocalData();
    return Model::init();
}

void WarriorModel::loadWarriorLocalData()
{
	parseWarriorFateInfoData( CSVDecoder::create(WARRIOR_FATE_CONFIG_PATH)->getData());
    parseWarriorBaseData( CSVDecoder::create(WARRIOR_BASE_INFO_CONFIG_PATH)->getData());
	//parseWarriorUpExpData( CSVDecoder::create(WARRIOR_LV_EXP_CONFIG_PATH)->getData());
	parseWarriorGrowData( CSVDecoder::create(WARRIOR_GRADE_INFO_CONFIG_PATH)->getData());
	parseWarriorUpSoulData( CSVDecoder::create(WARRIOR_UP_SOUL_CONFIG_PATH)->getData());
	parseWarriorLevelExpData( CSVDecoder::create(WARRIOR_LV_EXP_CONFIG_PATH)->getData());
    parseWarriorDialogInfoData( CSVDecoder::create(WARRIOR_DIALOG_CONFIG_PATH)->getData());
	parseWarriorIconInfoData(CSVDecoder::create(WARRIOR_ICONS_CONFIG_PATH)->getData());
    parseWarriorUpgradeItemsData(CSVDecoder::create(WARRIOR_UPGRADE_ITEM_CONFIG_PATH)->getData());
    parseWarriorUpgradeCriticalData(CSVDecoder::create(WARRIOR_UPGRADE_CRITICAL_CONFIG_PATH)->getData());
    parseWarriorUpgradeItemAccumulation(CSVDecoder::create(WARRIOR_UPGRADE_ACCUMULATIION_CONFIG_PATH)->getData());
	parseTalentInfoData(CSVDecoder::create(TALENT_CONFIG_PATH)->getData());
	parseWarriorTalentData(CSVDecoder::create(WARRIOR_TALENT_CONFIG_PATH)->getData());
    parseWarriorPotentialByUpLevel(CSVDecoder::create(WARRIOR_POTENTIAL_FACTOR_CONFIG_PATH)->getData());
	parseWarriorHandBookData(CSVDecoder::create(WARRIOR_HANDBOOK_CONFIG_PATH)->getData());
}


void WarriorModel::parseWarriorFateInfoData(const CSVDecoder::CSV &val)
{
	
	for (uint32_t i=0; i<val.size(); ++i)
	{
		FateInfo *pFateInfo= FateInfo::create();
		pFateInfo->fate_id = atoi(val[i][0].c_str());
		pFateInfo->type = (WarriorFateType)atoi(val[i][1].c_str());
		pFateInfo->name = val[i][2].c_str();
		pFateInfo->owner_id = atoi(val[i][3].c_str());

		uint32_t  member_id = 0;
		
		for (uint32_t j=4; j<8; j++)	
		{
			member_id = atoi(val[i][j].c_str());
			if(member_id>0)
			{
				pFateInfo->member_ids->addObject(CCInteger::create(member_id));
			}
		}

		pFateInfo->equip_id = atoi(val[i][8].c_str());
		pFateInfo->add_attack = atoi(val[i][9].c_str());
		pFateInfo->add_defence = atoi(val[i][10].c_str());
		pFateInfo->add_hp = atoi(val[i][11].c_str());
		
		m_warFateInfoMap->setObject(pFateInfo, pFateInfo->fate_id);
	}
}

void WarriorModel::parseWarriorIconInfoData( const CSVDecoder::CSV &val )
{
	for (unsigned int i = 0; i < val.size(); i++)
	{
		unsigned int warriorId = atoi(val[i][0].c_str());
		cocos2d::CCDictionary* pIconsMap = cocos2d::CCDictionary::create();
		IconsInfo* warriorListIconInfo = IconsInfo::create(val[i][1].c_str());
		pIconsMap->setObject(warriorListIconInfo, kIconTypeWarriorList);

		IconsInfo* changeFormationIconInfo = IconsInfo::create(val[i][2].c_str());
		pIconsMap->setObject(changeFormationIconInfo, kIconTypeChangeFormation);

		IconsInfo* fullIconInfo = IconsInfo::create(val[i][3].c_str());
		pIconsMap->setObject(fullIconInfo, kIconTypeFull);

		m_warIconInfoMap->setObject(pIconsMap, warriorId);
	}
	
}

void WarriorModel::parseWarriorDialogInfoData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
        uint32_t warriorId = atoi(val[i][0].c_str());
        
		WarriorDialogInfo *pInfo = (WarriorDialogInfo *)m_warDialogMap->objectForKey(warriorId);
        if(pInfo == NULL)
        {
            pInfo = WarriorDialogInfo::create();
            pInfo->warriorId = warriorId;
            m_warDialogMap->setObject(pInfo, pInfo->warriorId);
        }
        pInfo->addDialog(cocos2d::CCString::create(val[i][1]));		
	}
}

void WarriorModel::parseWarriorUpgradeItemsData(const CSVDecoder::CSV &val)
{
    for (uint32_t i = 0; i < val.size(); ++i) {
        uint32_t itemId = atoi(val[i][1].c_str());
        uint32_t coinConsum = atoi(val[i][2].c_str());
        uint32_t criticalTriggerValue = atoi(val[i][3].c_str());
		uint32_t experience = atoi(val[i][4].c_str());
        
        UpgradeItemInfo *pInfo = UpgradeItemInfo::create(itemId, coinConsum, criticalTriggerValue, experience);
        m_warriorUpgradeItemMap->setObject(pInfo, val[i][1].c_str());
    }
}

void WarriorModel::parseWarriorUpgradeItemAccumulation(const CSVDecoder::CSV &val)
{
    for (uint32_t i = 0; i < val.size(); ++i) {
        uint32_t _value = atoi(val[i][1].c_str());
        
        //value, level
        m_warriorUpgradeAccumulationMap->setObject(CCInteger::create(_value), val[i][0].c_str());
    }
}

void WarriorModel::parseWarriorUpgradeCriticalData(const CSVDecoder::CSV &val)
{
    for (uint32_t i = 0; i < val.size(); ++i) {
        uint32_t type = atoi(val[i][0].c_str());
        uint32_t critical = atoi(val[i][1].c_str());
        uint32_t probability = atoi(val[i][2].c_str());
        
        UpgradeCriticalData* _data = UpgradeCriticalData::create(type, critical, probability);
        m_warriorUpgadeCriticalMap->setObject(_data, val[i][0].c_str());
    }
}

void WarriorModel::parseTalentInfoData( const CSVDecoder::CSV &val )
{ 
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		TalentInfo* info = TalentInfo::create();
		if (info)
		{
			info->talent_id = atoi(val[i][1].c_str());
			info->name = val[i][2];
			info->type = atoi(val[i][4].c_str());
			info->buff_id = atoi(val[i][5].c_str());
			info->warriorType = atoi(val[i][6].c_str());
			info->attributeType = atoi(val[i][7].c_str());
			info->_value = atoi(val[i][8].c_str());
			info->probability = atoi(val[i][9].c_str());
			m_talentInfoMap->setObject(info, info->talent_id);

			CCString* description = NULL;
			std::string originalString(val[i][3]);
			switch (info->type)
			{
			case kTalentTypeAttackBuff:
				{
					std::string buffName = D_LOCAL_STRING(CCString::createWithFormat("$buffname%d", info->buff_id)->getCString());
					description = CCString::createWithFormat(originalString.c_str(), info->probability, buffName.c_str());
				}
				break;
			case kTalentTypeDebuffImmunity:
				{
					std::string buffName = D_LOCAL_STRING(CCString::createWithFormat("$buffname%d", info->buff_id)->getCString());
					description = CCString::createWithFormat(originalString.c_str(), buffName.c_str());
				}
				break;
			default:
				{
					description = CCString::createWithFormat(originalString.c_str(), info->_value);
				}
				break;
			}
			
			if (description)
			{
				info->_description.assign(description->getCString());
			}
		}
	}
}

void WarriorModel::parseWarriorTalentData( const CSVDecoder::CSV &val )
{
	uint32_t i = 0;
	while( i < val.size() )
	{
		uint32_t warriorId = atoi(val[i][0].c_str());
		CCDictionary* warriorTalents = CCDictionary::create();
		bool infoAdded = false;

		while( i < val.size() && atoi(val[i][0].c_str()) == warriorId)
		{
			warriorTalents->setObject(CCInteger::create(atoi(val[i][2].c_str())), atoi(val[i][1].c_str()));
			//CCLOG("warriorId:%d, uplevel:%d, talentId:%d", warriorId, atoi(val[i][1].c_str()), atoi(val[i][2].c_str()) );
			++i;
			infoAdded = true;
		}

		if (infoAdded == false)
		{
			 ++i;
		}
		m_warriorTalentMap->setObject(warriorTalents, warriorId);
	}
}

void WarriorModel::parseWarriorPotentialByUpLevel(const CSVDecoder::CSV &val)
{
    for (uint32_t i = 0; i < val.size(); ++i) {
        float factor = atof(val[i][1].c_str());
        CCFloat* cc_factor = CCFloat::create(factor);
        
        m_warriorPotentialFactorVec->addObject(cc_factor);
    }
}

void WarriorModel::getWarriorUpgradeItemsInfo(CCArray* itemArray)
{
    if(itemArray)
    {
        itemArray->removeAllObjects();
        
        CCDictElement* pElement = NULL;
        CCDICT_FOREACH(m_warriorUpgradeItemMap, pElement)
        {
            UpgradeItemInfo* itemInfo = (UpgradeItemInfo*)pElement->getObject();
            itemArray->addObject(itemInfo);
        }
    }
}


UpgradeItemInfo* WarriorModel::getWarriorUpgradeItemInfoById( uint32_t _id )
{
	CCDictElement* pElement = NULL;
	CCDICT_FOREACH(m_warriorUpgradeItemMap, pElement)
	{
		UpgradeItemInfo* itemInfo = (UpgradeItemInfo*)pElement->getObject();
		if (itemInfo->getItemId() == _id)
		{
			return itemInfo;
		}
	}

	return NULL;
}

void WarriorModel::getWarriorFatesByWIDAndEquipId(uint32_t wid,uint32_t eid,cocos2d::CCArray * fateArray)
{
    if (fateArray)
	{
        fateArray->removeAllObjects();
        
		FateInfo *pFateInfo = NULL;
		CCDictElement* pElement = NULL;
        
		CCDICT_FOREACH(m_warFateInfoMap, pElement)
		{
			pFateInfo = (FateInfo*)pElement->getObject();
			if (pFateInfo->owner_id == wid && pFateInfo->equip_id == eid)
			{
				fateArray->addObject(pFateInfo);
			}
		}
	}
}

void WarriorModel::getWarriorFatesByEquipId(uint32_t eid,cocos2d::CCArray * fateArray)
{
    if (fateArray)
	{
        fateArray->removeAllObjects();
        
		FateInfo *pFateInfo = NULL;
		CCDictElement* pElement = NULL;
        
		CCDICT_FOREACH(m_warFateInfoMap, pElement)
		{
			pFateInfo = (FateInfo*)pElement->getObject();
			if (pFateInfo->equip_id == eid)
			{
				warrior::WarriorBaseInfo* pWarrior = getWarriorBaseByWID(pFateInfo->owner_id);
				//if(pWarrior->showInEquipFate) fateArray->addObject(pFateInfo);
				fateArray->addObject(pFateInfo);
			}
		}
	}
}

void WarriorModel::getWarriorFatesByWID(uint32_t wid, cocos2d::CCArray * fateArray)
{
	if (fateArray)
	{
        fateArray->removeAllObjects();
        
		FateInfo *pFateInfo = NULL;
		CCDictElement* pElement = NULL;

		CCDICT_FOREACH(m_warFateInfoMap, pElement)
		{
			pFateInfo = (FateInfo*)pElement->getObject(); 
			if (pFateInfo->owner_id == wid)
			{
				fateArray->addObject(pFateInfo);
			}
		}
	}
}

bool WarriorModel::checkFateActivated(warrior::FateInfo *pFateInfo)
{
	if (pFateInfo)
	{
        //弟子是否在阵容中  //弟子是否小伙伴中
        if(!DM_GET_FORMATION_MODEL->isWarriorBattle(pFateInfo->owner_id) && !DM_GET_GUNDAM_EF_MODEL->isEgineerBattle(pFateInfo->owner_id)) return false;
		//弟子缘，判断有关弟子是否都上阵
		if (pFateInfo->type == kFateTypeWarrior)
		{
			CCInteger *member_id = NULL;
			CCObject *obj = NULL;

			CCARRAY_FOREACH(pFateInfo->member_ids, obj)
			{
				member_id = (CCInteger*)obj; 
				if (!DM_GET_FORMATION_MODEL->isWarriorBattle(member_id->getValue()) && !DM_GET_GUNDAM_EF_MODEL->isEgineerBattle(member_id->getValue()))
				{
					return false;
				}
			}
			return true;
		}
		//装备缘，判断弟子是否已装备某装备
		else if (pFateInfo->type == kFateTypeEquip)
		{
			//弟子在阵容中的位置
			formation::PositionInfo * pPosInfo = DM_GET_FORMATION_MODEL->getPositionInfoByWid(pFateInfo->owner_id);

			if (pPosInfo)
			{
				cocos2d::CCDictElement* pElement = NULL;
				equip::EquipCurInfo * pEquipInfo = NULL;
				cocos2d::CCDictionary* pDic = DM_GET_EQUIP_MODEL->getEquipedsById(pPosInfo->iId);
				CCDICT_FOREACH(pDic, pElement)
				{
					pEquipInfo = (equip::EquipCurInfo *)pElement->getObject();
					if (pEquipInfo->getEquipId() == pFateInfo->equip_id)
						return true;
				}
			}		
		}
	}

	return false;	
}


uint32_t WarriorModel::computeBasicAddProps(uint8_t wid , uint8_t real_grade,uint32_t basic, float basic_add,uint32_t level,uint8_t upLevel , WarriorTalentType type)
{
    //WarriorGrowInfo * pGrowInfo = DM_GET_WARRIOR_MODEL->getWarriorGrowInfoByGrade(real_grade);
	//策划要求
    //float ratio =  (pow(1.05f,(level-1)/3.f)-1) * WARRIOR_GROW_RATIO * pGrowInfo->growRate;
	//CCLOG("computeBasicAddProps begin");
	//CCLOG("computeBasicAddProps -------------------- wid %d" , wid);
	//CCLOG("computeBasicAddProps -------------------- basic %d" , basic);
	//CCLOG("computeBasicAddProps -------------------- basic_add %f" , basic_add);
	//CCLOG("computeBasicAddProps -------------------- level %d" , level);
	//CCLOG("computeBasicAddProps -------------------- upLevel %d" , upLevel);
	//CCLOG("computeBasicAddProps -------------------- type %d" , (int)type);
	//
	float ratio = level - 1 >= 0 ? level - 1 : 0;
	unsigned int attack_talentAdd = 0;
	unsigned int defence_talentAdd = 0;
	unsigned int hp_talentAdd = 0;
	unsigned int talentAdd = 0;

	CCDictionary* pTalentDic = DM_GET_WARRIOR_MODEL->getWarriorTalentsDic(wid);
	if(pTalentDic)
	{
		CCArray* pKeys = pTalentDic->allKeys(); 
		if (pKeys && pKeys->count() > 0)
		{
			//遍历天赋表
			for (unsigned int i = 0; i < pKeys->count(); i++)
			{
				int openLevel;
				CCInteger* pIntKey = dynamic_cast<CCInteger*>(pKeys->objectAtIndex(i));
				if (pIntKey)
				{
					openLevel = pIntKey->getValue();
				}
				else
				{
					CCString* pStringKey = dynamic_cast<CCString*>(pKeys->objectAtIndex(i));
					if (pStringKey == NULL)
					{
						continue;
					}
					openLevel = atoi(pStringKey->getCString());
				}

				if(openLevel <= upLevel)
				{
					//天赋已经开启
					CCObject* pObject = pTalentDic->objectForKey(openLevel);
					CCInteger* talentId = dynamic_cast<CCInteger*>(pObject);
					if (talentId == NULL)
					{
						continue;
					}

					warrior::TalentInfo* talentInfo = DM_GET_WARRIOR_MODEL->getTalentInfoById(talentId->getValue());
					if (talentInfo == NULL)
					{
						continue;
					}

					switch(talentInfo->type)
					{
					case warrior::kTalentTypeAttack:
						attack_talentAdd += talentInfo->_value;
						break;
					case warrior::kTalentTypeDefence:
						defence_talentAdd += talentInfo->_value;
						break;
					case warrior::kTalentTypeHP:
						hp_talentAdd +=  talentInfo->_value;
						break;
					default:
						break;
					}
				}
			}
		}		
	}

	switch(type)
	{
	case warrior::kTalentTypeAttack:
		talentAdd = attack_talentAdd;
		break;
	case warrior::kTalentTypeDefence:
		talentAdd = defence_talentAdd;
		break;
	case warrior::kTalentTypeHP:
		talentAdd = hp_talentAdd;
		break;
	default:
		break;
	}

    float mix = (basic + basic_add * ratio + talentAdd) * (1 + WARRIOR_UP_RATIO * upLevel);

	//CCLOG("computeBasicAddProps -------------------- return %f" , mix);
	//CCLOG("computeBasicAddProps end");
	return (uint32_t)mix;
}

WarriorDialogInfo * WarriorModel::getWarriorDialogInfo(uint32_t warriorId)
{
    return dynamic_cast<WarriorDialogInfo *>(m_warDialogMap->objectForKey(warriorId));
}

void WarriorModel::parseWarriorBaseData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		WarriorBaseInfo *pWarrior = WarriorBaseInfo::create();
		pWarrior->warrior_id = atoi(val[i][0].c_str());
		pWarrior->warrior_name = val[i][1].c_str();
		pWarrior->warrior_desc = val[i][2].c_str();
		pWarrior->color_grade = (WarriorGrade)atoi(val[i][3].c_str());
		pWarrior->real_grade = atoi(val[i][4].c_str());
		//pWarrior->warrior_limit_lv = atoi(val[i][4].c_str());
		pWarrior->basic_hp = atoi(val[i][5].c_str());
		pWarrior->basic_hp_add = atof(val[i][6].c_str());
		pWarrior->basic_attack = atoi(val[i][7].c_str());
		pWarrior->basic_attack_add = atof(val[i][8].c_str());
		pWarrior->basic_defence = atoi(val[i][9].c_str());
		pWarrior->basic_defence_add = atof(val[i][10].c_str());
		pWarrior->basic_speed = atoi(val[i][11].c_str());
		pWarrior->location =  (WarriorLocationType)atoi(val[i][12].c_str());
		pWarrior->skill_id = atoi(val[i][13].c_str());
		pWarrior->sp_skill_id = atoi(val[i][14].c_str());
        pWarrior->showInEquipFate = atoi(val[i][15].c_str())==1;
		getWarriorFatesByWID(pWarrior->warrior_id,pWarrior->m_pFateArray);
        
        pWarrior->res_id = atoi(val[i][16].c_str());
        pWarrior->unit_attack_dmg_delay = D_MILLISECOND_TO_SECOND(atoi(val[i][17].c_str()));
        //pWarrior->m_bHighQuality = atoi(val[i][18].c_str())==1;
        pWarrior->have_resource = atoi(val[i][19].c_str())==1;
		pWarrior->quality  = atoi(val[i][20].c_str());
		pWarrior->m_uFlipRes  = atoi(val[i][21].c_str());
		pWarrior->m_uFlipDialogRes  = atoi(val[i][22].c_str());
        m_warBaseInfoMap->setObject(pWarrior, pWarrior->warrior_id);
        
	}
}

int WarriorModel::getNewCount()
{
	int i = 0;
	int count_t = 0;
	for(i=0;i<this->getWarriorHandBookMap()->count();i++)
	{
		if(getWarriorHandBookInfoByWID(i+1)->num_new)
		{
			count_t++;
		}
	}
	return count_t;
}

void WarriorModel::parseWarriorHandBookData(const CSVDecoder::CSV &val)
{ 
	int i=0;
	for (i=0;i<val.size();i++)
	{
		WarriorHandBookInfo* pWarrior = WarriorHandBookInfo::create();
		pWarrior->warrior_id = atoi(val[i][1].c_str());
		CCArray * pWarriors = this->getWarriorVec();
		CCObject * obj(NULL);
		CCARRAY_FOREACH(pWarriors, obj)
		{
			warrior::WarriorCurInfo * pWarrior_t = (warrior::WarriorCurInfo*)obj;
			if(pWarrior->warrior_id==(pWarrior_t->getWid()))
			{
				if(pWarrior->isLighted==false)
				{
					pWarrior->isLighted=true;
				}
				break;
			}
		}
		if(pWarrior->isLighted==false)
		{
			pWarrior->isLighted=false;
		}
		pWarrior->warrior_name = val[i][2].c_str();
		pWarrior->color_grade = (WarriorGrade)atoi(val[i][4].c_str());
		pWarrior->quality = atoi(val[i][5].c_str());
		pWarrior->locationg = (WarriorLocationType)atoi(val[i][6].c_str());
		pWarrior->New = atoi(val[i][7].c_str());
		pWarrior->num_new = (atoi(val[i][7].c_str()))?pWarrior->num_new+=1:pWarrior->num_new;
		pWarrior->area = atoi(val[i][8].c_str());
		m_warHandBookMap->setObject(pWarrior, pWarrior->warrior_id);
	}
}

void WarriorModel::refreshWarriorHandBookData()
{
	WarriorHandBookInfo* pWarrior = NULL;
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(m_warHandBookMap,pEle)
	{
		pWarrior = static_cast<WarriorHandBookInfo*>(pEle->getObject());
		pWarrior->isLighted=false;
	}
	CCArray * pWarriors = this->getWarriorVec();
	CCObject * obj(NULL);
	CCARRAY_FOREACH(pWarriors, obj)
	{
		warrior::WarriorCurInfo * pWarrior_t = (warrior::WarriorCurInfo*)obj;
		pWarrior = static_cast<WarriorHandBookInfo*>(m_warHandBookMap->objectForKey(pWarrior_t->getWid()));
		if(pWarrior)
		{
			pWarrior->isLighted=true;
		}
	}
}

void WarriorModel::parseWarriorUpSoulData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		WarriorUpSoul *pWarUpSoul = WarriorUpSoul::create();
		pWarUpSoul->upLevel = atoi(val[i][0].c_str());
		pWarUpSoul->bGradeSoul = atoi(val[i][1].c_str());
		pWarUpSoul->pGradeSoul = atoi(val[i][2].c_str());
		pWarUpSoul->rGradeSoul = atoi(val[i][3].c_str());

		pWarUpSoul->bGradeItem = atoi(val[i][4].c_str());
		pWarUpSoul->pGradeItem = atoi(val[i][5].c_str());
		pWarUpSoul->rGradeItem = atoi(val[i][6].c_str());

		pWarUpSoul->bGradeCoin = atoi(val[i][7].c_str());
		pWarUpSoul->pGradeCoin = atoi(val[i][8].c_str());
		pWarUpSoul->rGradeCoin = atoi(val[i][9].c_str());

		pWarUpSoul->levelLimit = atoi(val[i][10].c_str());
        m_warUpSoulVec->addObject(pWarUpSoul);
	}
}

void WarriorModel::parseWarriorGrowData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		WarriorGrowInfo *pWarGrow = WarriorGrowInfo::create();
		pWarGrow->grade = atoi(val[i][0].c_str());
		pWarGrow->color = atoi(val[i][1].c_str());
		pWarGrow->rctSoul = atoi(val[i][2].c_str());
		//pWarGrow->growIndex = atof(val[i][3].c_str());
		pWarGrow->growRate = atof(val[i][3].c_str());
		pWarGrow->soulValue = atoi(val[i][4].c_str());
        
        uint32_t color = 0;
        sscanf(val[i][5].c_str(), "%i",&color);
        pWarGrow->colorValue = cocos2d::ccc3(color>>16&0xFF, color>>8&0xFF, color>>0&0xFF);
        

        m_warGrowInfoMap->setObject(pWarGrow, pWarGrow->grade);
	}
}

void WarriorModel::parseWarriorLevelExpData(const CSVDecoder::CSV &val)
{
    for(unsigned int i = 0 ; i < (int)val.size() ; i++)
    {
        CCArray *pWarriorLevelExpArr = CCArray::create();
        
        for(unsigned int j = 1 ; j <= D_CONFIG_UINT_FOR_KEY(WARRIOR_REAL_GRADE_MAX) * 2 ; j++)
        {
            CCString *pExp = CCString::createWithFormat("%d",atoi(val[i][j].c_str()));
            //CCLOG("%d",pExp->intValue());
            pWarriorLevelExpArr->addObject(pExp);
        }
        m_pWarriorLevelExpArr->addObject(pWarriorLevelExpArr);
    }
}


WarriorBaseInfo* WarriorModel::getWarriorBaseByWID(unsigned int wid)
{
	return (WarriorBaseInfo*)m_warBaseInfoMap->objectForKey(wid);
}

WarriorHandBookInfo* WarriorModel::getWarriorHandBookInfoByWID(unsigned int wid)
{
	return (WarriorHandBookInfo*)m_warHandBookMap->objectForKey(wid);
}

/* 获取弟子品阶对应的颜色值 */
cocos2d::ccColor3B WarriorModel::getWarriorColorByWid(unsigned int wid)
{
    WarriorBaseInfo * pInfo = getWarriorBaseByWID(wid);
    if(pInfo==NULL) return cocos2d::ccc3(0,0,0);
    return getWarriorColorByGrade(pInfo->getRealGrade());
}

/* 获取弟子品阶对应的颜色值 */
cocos2d::ccColor3B WarriorModel::getWarriorColorByGrade(unsigned char real_grade)
{
    warrior::WarriorGrowInfo* pWarriorGrowInfo = getWarriorGrowInfoByGrade(real_grade);
    return pWarriorGrowInfo?pWarriorGrowInfo->colorValue:cocos2d::ccc3(0,0,0);
}

bool WarriorModel::isWarriorExist(uint32_t wid)
{
    return getWarriorCurInfoByWID(wid) == NULL;
}

WarriorCurInfo* WarriorModel::getWarriorCurInfoByWID(uint32_t wid)
{
    CCObject *obj = NULL;
    WarriorCurInfo* pWarriorCurInfo = NULL;
    
    CCARRAY_FOREACH(m_warCurInfoVec, obj)
    {
        pWarriorCurInfo = (WarriorCurInfo*)obj;
        if(pWarriorCurInfo->wid == wid)
        {
            return pWarriorCurInfo;
        }
    }
	return NULL;
}

bool WarriorModel::isHasWarriorsUnBattle()
{
    CCObject *obj = NULL;
    WarriorCurInfo* pWarriorCurInfo = NULL;
    CCARRAY_FOREACH(m_warCurInfoVec, obj)
    {
        pWarriorCurInfo = (WarriorCurInfo*)obj;
        if(pWarriorCurInfo && !DM_GET_FORMATION_MODEL->isWarriorBattle(pWarriorCurInfo->wid))
        {
            return true;
        }
    }
    return false;
}


cocos2d::CCArray * WarriorModel::getWarriorsUnBattle(cocos2d::CCArray * pWarriors)
{
    if(pWarriors)
    {
        pWarriors->removeAllObjects();
    }else
    {
        pWarriors = WarriorCurInfoVec::create();
    }
        
    CCObject *obj = NULL;
    WarriorCurInfo* pWarriorCurInfo = NULL;
    
    CCARRAY_FOREACH(m_warCurInfoVec, obj)
    {
        pWarriorCurInfo = (WarriorCurInfo*)obj;
		//英雄既不能在阵型中，也不能在小伙伴中
        if(pWarriorCurInfo && !DM_GET_FORMATION_MODEL->isWarriorBattle(pWarriorCurInfo->getWid()) && !DM_GET_GUNDAM_EF_MODEL->isEgineerBattle(pWarriorCurInfo->getWid()))
        {
            pWarriors->addObject(pWarriorCurInfo);
        }
    }
    
    return pWarriors;
}


/*************************************************************************
 * desc: 获得弟子列表											 *
 * param:																 *
 *		grade 弟子显示品阶											 *
 *		wid 不包括的弟子id，0表示不限ID								 *
 *	    vec  列表存放位置												 *
 ************************************************************************/

void WarriorModel::getWarriorsIgnoreWID(uint32_t wid, WarriorCurInfoVec &vec)
{
    vec.removeAllObjects();

	if (wid == 0)
	{
		vec.addObjectsFromArray(m_warCurInfoVec);
		return;
	}
	
    CCObject *obj = NULL;
    WarriorCurInfo* pWarriorCurInfo = NULL;
    
    CCARRAY_FOREACH(m_warCurInfoVec, obj)
    {
        pWarriorCurInfo = (WarriorCurInfo*)obj;
        if(pWarriorCurInfo->getWid() != wid )
        {
            vec.addObject(pWarriorCurInfo);
        }
    }
}

uint32_t WarriorModel::getWarriorUpSoul(WarriorCurInfo * pWarrior)
{
    CCObject *obj = NULL;
    WarriorUpSoul* pWarriorUpSoul = NULL;
    
    CCARRAY_FOREACH(m_warUpSoulVec, obj)
    {
        pWarriorUpSoul = (WarriorUpSoul*)obj;

		//取下一级的突破魂魄数 突破等级从0开始
        if(pWarriorUpSoul->upLevel == pWarrior->warriorUpLv+1)	
        {
			if( pWarrior->baseInfo->color_grade == kWarGradeBlue)
				return pWarriorUpSoul->bGradeSoul;
            else if( pWarrior->baseInfo->color_grade == kWarGradePurple)
                return pWarriorUpSoul->pGradeSoul;
            else if( pWarrior->baseInfo->color_grade == kWarGradeRed)
                return pWarriorUpSoul->rGradeSoul;
        }
    }
	return 0;
}

unsigned int WarriorModel::getWarriorUpTotalAmount(warrior::WarriorCurInfo * pWarrior , int type)
{
	//若英雄根本没有进阶 直接返回0！
	unsigned int uplevel = pWarrior->getWarriorUpLv();
	if(uplevel == 0) return 0;

	//英雄有进阶的情况 开始计算
	//铜币总数
	unsigned int totalcoin = 0;
	//进阶丹总数
	unsigned int totalitem = 0;
	//魂魄总数
	unsigned int totalSoul = 0;

	for(int i = 0 ; i<uplevel ; i++)
	{
		WarriorUpSoul* pInfo = dynamic_cast<WarriorUpSoul*>(m_warUpSoulVec->objectAtIndex(i));
		if(pInfo)
		{
			switch(pWarrior->getBaseInfo()->getColorGrade())
			{
			case warrior::kWarGradeRed:
				totalcoin += pInfo->getRGradeCoin();
				totalitem += pInfo->getRGradeItem();
				totalSoul += pInfo->getRGradeSoul();
				break;
			case warrior::kWarGradePurple:
				totalcoin += pInfo->getPGradeCoin();
				totalitem += pInfo->getPGradeItem();
				totalSoul += pInfo->getPGradeSoul();
				break;
			case warrior::kWarGradeBlue:
				totalcoin += pInfo->getGradeCoin();
				totalitem += pInfo->getGradeItem();
				totalSoul += pInfo->getGradeSoul();
				break;
				//缺一个绿色！暂时没有
			}
		}
		else{
			CCLOG("ERROR:WarriorModel::getWarriorUpTotalAmount");
		}
	}

	//返回结果
	unsigned int rtn = 0;
	if(type == 1) rtn = totalcoin;
	if(type == 2) rtn = totalitem;
	if(type == 3) rtn = totalSoul;
	return rtn;
}

WarriorUpSoul* WarriorModel::getWarriorUpInfo(WarriorCurInfo * pWarrior)
{
	CCObject *obj = NULL;
	WarriorUpSoul* pWarriorUpSoul = NULL;
	CCARRAY_FOREACH(m_warUpSoulVec, obj)
	{
		pWarriorUpSoul = (WarriorUpSoul*)obj;
		//取下一级的突破魂魄数 突破等级从0开始
		if(pWarriorUpSoul->upLevel == pWarrior->warriorUpLv+1)	
			break;
	}
	return pWarriorUpSoul;
}

uint32_t WarriorModel::getItemExp(uint32_t itemid)
{
	const char* key = cocos2d::CCString::createWithFormat("%d" , itemid)->getCString();
	if(m_warriorUpgradeItemMap->objectForKey( key )){
		warrior::UpgradeItemInfo* upInfo = (warrior::UpgradeItemInfo*)m_warriorUpgradeItemMap->objectForKey(key);
		return upInfo->getExperience();
	}
	return 0;
}
/**
 *	@brief	判断弟子是否可以立即突破
 *
 *	@param 	pWarrior 	弟子
 *
 *	@return	是否可以立即突破
 */
WarriorUpErrorType WarriorModel::canWarriorUpNow(warrior::WarriorCurInfo *pWarrior)
{
	if (pWarrior == NULL)
	{
		return kUpErrorWarriorInfo;
	}
	
    if(!pWarrior->checkWarriorLevelForPromotion())
    {
        return kUpErrorWarriorLevelLimit;
    }
    
	warrior::WarriorUpSoul* upinfo = getWarriorUpInfo(pWarrior);
	if (upinfo == NULL)
	{
		return kUpErrorSoulInfo;
	}
	
	uint32_t itemid = D_CONFIG_UINT_FOR_KEY(WARRIORUP_ITEMID);
	item::ItemBase* itembase = DM_GET_ITEM_MODEL->getItemBaseByID(itemid);
	item::ItemInfo* iteminfo = DM_GET_ITEM_MODEL->getItemInfoByID(itemid);
	uint32_t currentItemCount = iteminfo?iteminfo->getNum():0;

	uint32_t needItemNum = upinfo->getUpItemCount(pWarrior->getBaseInfo()->getColorGrade());
	int needCoinNum = upinfo->getUpCoinCount(pWarrior->getBaseInfo()->getColorGrade());

	 SoulInfo * pSoul = getSoulInfoByWID(pWarrior->getWid());

	if (pSoul != NULL && pSoul->num < getWarriorUpSoul(pWarrior))
	{
		return kUpErrorSoulNotEnough;
	}

	if (currentItemCount < needItemNum)
	{
		return kUpErrorItemNotEnough;
	}
	
	if (DM_GET_USER_MODEL->getUserInfo()->getCoins() < needCoinNum)
	{
		return kUpErrorCoinNotEnough;
	}
	
	return kUpErrorNone;
}


//根据经验获得弟子等级
int WarriorModel::getWarriorLevelByExp(const int &exp, const int &grade, const int & currLevel)
{
    if(currLevel >= WARRIOR_LV_MAX)
    {
        return currLevel;
    }
    
    int level = currLevel;
    uint32_t nextLevel = level + 1;
    uint32_t gradeIndex = (grade - 1) * 2  + 1;
    CCArray *pWarriorLevelExpArr = (CCArray *)m_pWarriorLevelExpArr->objectAtIndex(nextLevel - 1);
    int requireExp = ((CCString *)pWarriorLevelExpArr->objectAtIndex(gradeIndex))->intValue();
    
    while(exp >= requireExp)
    {
        level = nextLevel;
        ++nextLevel;
		if(nextLevel-1>=m_pWarriorLevelExpArr->count()) break;
        pWarriorLevelExpArr = (CCArray *)m_pWarriorLevelExpArr->objectAtIndex(nextLevel - 1);
        requireExp = ((CCString *)pWarriorLevelExpArr->objectAtIndex(gradeIndex))->intValue();
    }
    
    return level;
}


//获取弟子升级需要的经验值
int WarriorModel::getWarriorUpExp(const int &level , const int &grade)
{
    CCAssert(level>0 && level<=(int)m_pWarriorLevelExpArr->count(), "弟子等级下标越界");
    
    CCArray *pWarriorLevelExpArr = (CCArray *)m_pWarriorLevelExpArr->objectAtIndex(level-1);
    CCString *pWarriorLevel = (CCString *)pWarriorLevelExpArr->objectAtIndex((grade-1)*2);
    
    return pWarriorLevel->intValue();
}

//获取弟子升级需要的累积经验值
int WarriorModel::getWarriorAddExp(const int &level , const int &grade)
{
    CCAssert(level>0 && level<=(int)m_pWarriorLevelExpArr->count(), "弟子等级下标越界");
    
    CCArray *pWarriorLevelExpArr = (CCArray *)m_pWarriorLevelExpArr->objectAtIndex(level-1);
    CCString *pWarriorLevel = (CCString *)pWarriorLevelExpArr->objectAtIndex(grade*2-1);
    
    return pWarriorLevel->intValue();
}

//根据弟子当前获得的经验总值 计算弟子当前等级获得的经验
int WarriorModel::getWarriorCurExpByGetExp(const int &getExp , const int &level , const int &grade)
{
    int nAddExp = getWarriorAddExp(level, grade);
    return getExp - nAddExp;
}

//根据等级获得弟子潜力
int WarriorModel::getWarriorPotentialByLevel(const int &level, const int &upLevel, WarriorBaseInfo * pBaseInfo)
{
    if(pBaseInfo==NULL)
    {
        CCLOG("%s", "error warrior");
        return 0;
    }
    
    if (m_warriorPotentialFactorVec->count() == 0) {
        CCLOG("%s", "error m_warriorPotentialFactorVec");
        return 0;
    }
    
    int _upLevel = upLevel;
    if (upLevel < 0) {
        _upLevel = 0;
    }
    
    if (upLevel >= m_warriorPotentialFactorVec->count()) {
        _upLevel  = m_warriorPotentialFactorVec->count() -1;
    }
    
    WarriorGrowInfo * pGrow = getWarriorGrowInfoByGrade(pBaseInfo->real_grade);
    CCFloat* uplevelRate = dynamic_cast<CCFloat*>(m_warriorPotentialFactorVec->objectAtIndex(_upLevel));
    if (uplevelRate == NULL) {
        CCLOG("%s", "error uplevelRate");
        return 0;
    }

    float rate = (pow(1.05f,  (level -1) / 3.0f) - 1) * WARRIOR_GROW_RATIO * (1 - pGrow->growRate) * uplevelRate->getValue();
    int pot = rate * (pBaseInfo->basic_attack_add + pBaseInfo->basic_defence_add + pBaseInfo->basic_hp_add / 8);
    CCLOG("potential of warrior %d is:%d", pBaseInfo->warrior_id, pot);
    return pot;
}



void WarriorModel::parseWarriors(const Json::Value &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		parseOneWarrior(val[i],false);
	}
    isSynced = true;
    postNotification(D_EVENT_WARRIORLIST_UPDATE);
}


void WarriorModel::parseOneWarrior(const Json::Value &val,bool notifyListChanged)
{
	
    WarriorCurInfo *pWarriorInfo = getWarriorCurInfoByWID(val["wid"].asUInt());
/************************************************************************/
/* for analytics begin                                                  */
/************************************************************************/
	bool dropped = false;
	if (NULL == pWarriorInfo)
	{
		dropped = true;
	}
/************************************************************************/
/*  for analytics end                                                   */
/************************************************************************/	 
	if(pWarriorInfo)
    {
        uint32_t lv = pWarriorInfo->getWarriorLv();
        parseWarrior(val,pWarriorInfo);
        pWarriorInfo->postNotification(D_EVENT_WARRIORINFO_UPDATE);
        if(lv !=pWarriorInfo->getWarriorLv()) checkWarriorOrder(pWarriorInfo,notifyListChanged); //如果等级变化则重新排序
    }else
    {
        pWarriorInfo = parseWarrior(val,pWarriorInfo);
        pWarriorInfo->isSynced = true;
        addWarrior(pWarriorInfo,notifyListChanged);
    }
    pWarriorInfo->computeWarriorProps();

/************************************************************************/
/* for analytics begin                                                  */
/************************************************************************/

	if (!pWarriorInfo || !dropped)
	{
		return;
	}
	else
	{
		WarriorBaseInfo *pBase = getWarriorBaseByWID(pWarriorInfo->getWid());
		if (NULL == pBase)
		{
			return;
		}

		AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
		pAnalytics->dropItem(pBase->getWarriorName().c_str(), 1);
	}
/************************************************************************/
/*  for analytics end                                                   */
/************************************************************************/	 
}

void WarriorModel::addWarrior(WarriorCurInfo * pWarrior,bool notifyListChanged)
{
	WarriorCurInfo *pWarriorInfo = getWarriorCurInfoByWID(pWarrior->wid);
    
	if (!pWarriorInfo)
	{
		pWarrior->baseInfo = getWarriorBaseByWID(pWarrior->wid);
		//pWarrior->growInfo = getWarriorGrowInfoByGrade(pWarrior->baseInfo->real_grade);
		addWarriorInorder(pWarrior);
		postNotification(D_EVENT_WARRIORLIST_UPDATE);
	}
	else
		CCLOG("错误，已经有该弟子！");
}

void WarriorModel::removeWarriors(cocos2d::CCArray *pWarriors)
{
    if(!pWarriors || pWarriors->count()==0) return;
    
    m_warCurInfoVec->removeObjectsInArray(pWarriors);
    
    postNotification(D_EVENT_WARRIORLIST_UPDATE);
}

void WarriorModel::checkWarriorOrder(WarriorCurInfo * pWarriorInfo,bool notifyListChanged)
{
    int index = m_warCurInfoVec->indexOfObject(pWarriorInfo); //防止弟子升级 重新调整顺序
    m_warCurInfoVec->removeObjectAtIndex(index,false);
    addWarriorInorder(pWarriorInfo);
    if(index!=m_warCurInfoVec->indexOfObject(pWarriorInfo))
    {
        if(notifyListChanged) postNotification(D_EVENT_WARRIORLIST_UPDATE); //如果排序顺序变化 通知刷新列表
    }
}

WarriorGrowInfo* WarriorModel::getWarriorGrowInfoByGrade(unsigned int wid)
{
    WarriorBaseInfo * pInfo = getWarriorBaseByWID(wid);
    if(pInfo==NULL) return NULL;
    return getWarriorGrowInfoByGrade(pInfo->getRealGrade());
}

WarriorGrowInfo* WarriorModel::getWarriorGrowInfoByGrade(uint8_t real_grade)
{
    return (WarriorGrowInfo*)m_warGrowInfoMap->objectForKey(real_grade);
}

unsigned short WarriorModel::getWarriorGradeColorByWid(unsigned int wid)
{
	WarriorGrowInfo* growInfo = getWarriorGrowInfoByGrade(wid);
	return growInfo->getColor();
}

/* 获取弟子颜色对应的颜色值 */
cocos2d::ccColor3B WarriorModel::getWarriorColorByColorGrade(WarriorGrade grade)
{
    CCDictElement * pElement(NULL);
    CCDICT_FOREACH(m_warGrowInfoMap, pElement)
    {
        WarriorGrowInfo * pInfo = (WarriorGrowInfo*)pElement->getObject();
        if(pInfo->color == grade)
        {
            return pInfo->colorValue;
        }
    }
    return ccColor3B();
}

void WarriorModel::addWarriorInorder(WarriorCurInfo * pWarrior)
{
    if(!pWarrior->baseInfo)
    {
        CCLOG("error warrior!, id:%d", pWarrior->wid);
        return;
    }
    WarriorCurInfo * pWarriorInfo = NULL;
    uint32_t i = 0;
    for (; i<m_warCurInfoVec->count(); ++i)
    {
        pWarriorInfo = (WarriorCurInfo *)m_warCurInfoVec->objectAtIndex(i);

        if (pWarriorInfo->baseInfo->color_grade < pWarrior->baseInfo->color_grade)
        {
            break;
        }else if(pWarriorInfo->baseInfo->color_grade > pWarrior->baseInfo->color_grade)
        {
            continue;
        }//相等比下一项
        
        if(pWarriorInfo->warriorLv < pWarrior->warriorLv)
        {
            break;
        }/*else if(pWarriorInfo->warriorLv < pWarrior->warriorLv)
          {
          continue;
          }//相等比下一项*/
        
    }
    m_warCurInfoVec->insertObject(pWarrior,i);
}


WarriorCurInfo* WarriorModel::parseWarrior(const Json::Value &val, WarriorCurInfo *pWarrior)
{
	if(pWarrior == NULL)
	{
		pWarrior =  WarriorCurInfo::create();
	}
    
	DJSON_UINT(pWarrior->sexp,val,"sexp");
	DJSON_UINT(pWarrior->slv,val,"slv");
	DJSON_UINT(pWarrior->warriorExp,val,"exp");
	DJSON_UINT(pWarrior->wid,val,"wid");
	DJSON_UINT(pWarrior->warriorLv,val,"lv");
	DJSON_UINT(pWarrior->warriorUpLv,val,"uplv");
	DJSON_UINT(pWarrior->potential,val,"potential");
    
	DJSON_UINT(pWarrior->hp_add,val,"hpadd");		//这里读取的是属性buff
	DJSON_UINT(pWarrior->attack_add,val,"attadd");
	DJSON_UINT(pWarrior->defence_add,val,"defadd");
    DJSON_UINT(pWarrior->m_nDressId, val, "dressId");
	pWarrior->baseInfo = getWarriorBaseByWID(pWarrior->wid);
    
    //等级潜力-当前潜力=已消耗潜力
    uint32_t potential = getWarriorPotentialByLevel(pWarrior->warriorLv, pWarrior->warriorUpLv, pWarrior->baseInfo);
    pWarrior->costedPotential = potential >= pWarrior->potential ? (potential - pWarrior->potential) : potential;

	DJSON_UINT(pWarrior->m_iDevelopItemNum, val, "developItemNum");
	return pWarrior;
}



#pragma mark PotBuff


PotBuff* WarriorModel::getWarriorPotBuff(WarriorCurInfo * pWarriorInfo)
{
	return (PotBuff*)m_warPotBuffMap->objectForKey(pWarriorInfo->getWid());
}

void WarriorModel::dealWithPotBuff(WarriorCurInfo * pWarriorInfo, const bool &accepted)
{

	PotBuff* potBuff = getWarriorPotBuff(pWarriorInfo);

	/*通过培养更新弟子攻防血属性*/
	if (potBuff )
	{
		if(accepted)
		{
			pWarriorInfo->hp_add += potBuff->hp;
			pWarriorInfo->attack_add += potBuff->att;
			pWarriorInfo->defence_add += potBuff->def;
//			pWarriorInfo->potential -= potBuff->cost;
			pWarriorInfo->computeWarriorProps();
		}
		m_warPotBuffMap->removeObjectForKey(pWarriorInfo->getWid());

		pWarriorInfo->postNotification(D_EVENT_WARRIORINFO_UPDATE);	//等清除potbuff后再刷新培养界面
	}


}


void WarriorModel::parsePotBuffs(const Json::Value &val)
{
    for (uint32_t i=0; i<val.size(); i++)
    {
		PotBuff *m_warPotBuff = parsePotBuff(val[i]);
		CCLOG("Warrior with pot buffs:%u", m_warPotBuff->wid);
		m_warPotBuffMap->setObject(m_warPotBuff,m_warPotBuff->wid);
    }
	
}


void WarriorModel::parseOnePotBuff(const Json::Value &val)
{
	PotBuff *m_warPotBuff = (PotBuff*)m_warPotBuffMap->objectForKey(val["wid"].asUInt());

	if (m_warPotBuff)
	{
		parsePotBuff(val,m_warPotBuff);
	}
	else
	{
		m_warPotBuff = parsePotBuff(val);
		m_warPotBuffMap->setObject(m_warPotBuff,m_warPotBuff->wid);
	}	
}


PotBuff * WarriorModel::parsePotBuff(const Json::Value &val, PotBuff * pPotBuff)
{
	if (!pPotBuff)
	{
		pPotBuff = PotBuff::create();
	}
    
	DJSON_UINT(pPotBuff->wid,val,"wid");
	DJSON_INT(pPotBuff->hp,val,"hp");
	DJSON_INT(pPotBuff->att,val,"att");
	DJSON_INT(pPotBuff->def,val,"def");
    DJSON_INT(pPotBuff->cost,val,"cost");
	uint32_t type = 0;
	DJSON_UINT(type,val,"type");
	pPotBuff->type = (PotBuff::PotbuffType)type;
    
	return pPotBuff;
}




#pragma mark soul

SoulInfo* WarriorModel::getSoulInfoByWID(uint32_t wid)
{
    SoulInfo* pSoulInfo = NULL;
    CCObject *obj = NULL;
    
    CCARRAY_FOREACH(m_soulInfoVec, obj)
    {
        pSoulInfo = (SoulInfo*)obj;
        if(pSoulInfo->wid == wid)
        {
            return pSoulInfo;
        }
    }
	return NULL;
}

bool hasSoul(const Json::Value &val,uint32_t wid)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
		if(val[i]["wid"].asUInt()==wid) return true;
	}
    return false;
}

void WarriorModel::updateSouls(const Json::Value &val)
{
    SoulInfo* pSoulInfo = NULL;
    CCObject *obj = NULL;
    CCArray* pTempList = CCArray::create();
	CCARRAY_FOREACH(m_soulInfoVec, obj)
    {
        pSoulInfo = (SoulInfo*)obj;
        for (uint32_t i=0; i<val.size(); ++i)
        {
            if(!hasSoul(val,pSoulInfo->wid)) pTempList->addObject(pSoulInfo);
        }
    }
    
    CCARRAY_FOREACH(pTempList, obj)
    {
        pSoulInfo = (SoulInfo*)obj;
        removeSoul(pSoulInfo);
    }
    pTempList->removeAllObjects();
    
    parseSouls(val);
}


void WarriorModel::parseSouls(const Json::Value &val)
{
	
    for (uint32_t i=0; i<val.size(); ++i)
	{
		parseOneSoul(val[i],false);
	}
    
    postNotification(D_EVENT_SOULLIST_UPDATE);
	isSynced = true;
}

void WarriorModel::parseOneSoul(const Json::Value &val,bool notifyListChanged)
{
	SoulInfo *pSoulInfo = getSoulInfoByWID(val["wid"].asUInt());
/************************************************************************/
/* for analytics begin                                                  */
/************************************************************************/
	int oldNum = 0;
	if (NULL != pSoulInfo)
	{
		oldNum = pSoulInfo->getNum();
	}
/************************************************************************/
/*  for analytics end                                                   */
/************************************************************************/	
	if(pSoulInfo)
    {
        parseSoul(val,pSoulInfo);
        
        notifySoulInfoUpdate(pSoulInfo);
        
        //魂魄没有级别 品阶不会变化不需要重新排序
    }else
    {
        pSoulInfo = parseSoul(val,pSoulInfo);
        pSoulInfo->isSynced = true;
        addSoul(pSoulInfo,notifyListChanged);
    }
/************************************************************************/
/* for analytics begin                                                  */
/************************************************************************/
	int newNum = 0;
	if (NULL != pSoulInfo)
	{
		newNum = pSoulInfo->getNum();
	}
	else
	{
		return;
	}
	WarriorBaseInfo *pBase = getWarriorBaseByWID(pSoulInfo->getWid());
	if (NULL == pBase)
	{
		return;
	}
	
	AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
	if (newNum > oldNum)
	{
		pAnalytics->dropItem(CCString::createWithFormat("soul_%s", pBase->getWarriorName().c_str())->getCString(), newNum - oldNum);
	}
	else if (newNum < oldNum)
	{
		pAnalytics->useItem(CCString::createWithFormat("soul_%s", pBase->getWarriorName().c_str())->getCString(), oldNum - newNum);
	}

/************************************************************************/
/*  for analytics end                                                   */
/************************************************************************/	
}


/* 掉落魂魄*/
void WarriorModel::addSoul(SoulInfo * pSoul,bool notifyListChanged)
{
	if ( pSoul->wid == 0)
	{
		CCLOG("error pSoul！");
		return;
	}

	SoulInfo *pSoulInfo = getSoulInfoByWID(pSoul->wid);
    pSoul->baseInfo = getWarriorBaseByWID(pSoul->wid);


    if(pSoulInfo)
    {
        pSoulInfo->num += pSoul->num;
        notifySoulInfoUpdate(pSoulInfo);
        return;
    }
	addSoulInorder(pSoul);
    if(notifyListChanged) postNotification(D_EVENT_SOULLIST_UPDATE);
}


SoulInfo* WarriorModel::parseSoul(const Json::Value &val, SoulInfo *pSoul)
{
	if(pSoul == NULL)
	{
		pSoul = SoulInfo::create();
	}
    
	//DJSON_UINT(pSoul->id,val,"id");
	DJSON_UINT(pSoul->wid,val,"wid");
	DJSON_UINT(pSoul->num,val,"num");
	DJSON_UINT(pSoul->hpts,val,"HPTS");
	pSoul->baseInfo = getWarriorBaseByWID(pSoul->wid);
	return pSoul;
}



void WarriorModel::addSoulInorder(SoulInfo * pSoul)
{
    if(!pSoul->baseInfo)
    {
        CCLOG("error pSoul!");
        return;
    }
    SoulInfo * pSoulInfo = NULL;
    uint32_t i = 0;
	
    for (; i<m_soulInfoVec->count(); ++i)
    {
        
        pSoulInfo = (SoulInfo *)m_soulInfoVec->objectAtIndex(i);
        if (pSoulInfo->baseInfo->color_grade < pSoul->baseInfo->color_grade)
        {
            break;
        }/*else if(pSoulInfo->baseInfo->color_grade < pWarrior->baseInfo->color_grade)
          {
          continue;
          }//相等比下一项
          */
    }
    m_soulInfoVec->insertObject(pSoul,i);
}


void WarriorModel::removeSoul(SoulInfo * pSoul , const bool &notifyListChanged)
{
    if(!pSoul) return;
    
	SoulInfo * pSoulInfo = NULL;
	uint32_t i = 0;
	for (; i<m_soulInfoVec->count(); ++i)
	{
		pSoulInfo = (SoulInfo *)m_soulInfoVec->objectAtIndex(i);
		if (pSoulInfo->wid == pSoul->wid)
		{
			pSoulInfo->num -= pSoul->num;
            
            notifySoulInfoUpdate(pSoulInfo);
			if (pSoulInfo->num ==0 )
			{
				m_soulInfoVec->removeObjectAtIndex(i);
                if(notifyListChanged)
                {
                    postNotification(D_EVENT_SOULLIST_UPDATE);
                }
			}
			break;
		}
	}
}

void WarriorModel::removeSouls(cocos2d::CCArray *pSouls)
{
    if(!pSouls || pSouls->count()==0) return;
    
    bool bNeedNotify = false;
    
    CCObject *pObj = NULL;
	SoulInfo * pSoulInfo = NULL;
    
    CCARRAY_FOREACH(pSouls, pObj)
	{
		pSoulInfo = (SoulInfo *)pObj;
		if(pSoulInfo)
		{
			removeSoul(pSoulInfo , false);

			bNeedNotify = true;
        }
    }
    
    if(bNeedNotify)
    {
        postNotification(D_EVENT_SOULLIST_UPDATE);
    }
}

bool WarriorModel::isMyWarrior(warrior::WarriorCurInfo* pWarrior)
{
    if(pWarrior==NULL) return false;
    return m_warCurInfoVec->indexOfObject(pWarrior) != CC_INVALID_INDEX;
}

void WarriorModel::notifySoulInfoUpdate(warrior::SoulInfo* pSoulInfo)
{
    pSoulInfo->postNotification(D_EVENT_SOULINFO_UPDATE);
    warrior::WarriorCurInfo* pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(pSoulInfo->wid);
    if(pWarrior) pWarrior->postNotification(D_EVENT_WARRIORINFO_UPDATE); //如果魂魄变化 通知弟子信息变化 这样处理方便些
}

void WarriorModel::dressUp(avatar::AvatarInfo* pAvatar,bool showBusy)
{
    if(pAvatar==NULL) return;
    WarriorCurInfo* pWarrior = getWarriorCurInfoByWID(pAvatar->getBaseInfo()->getWarriorId());
    if(pWarrior==NULL || pWarrior->getDressId()==pAvatar->getId()) return;
    HTTP_CLIENT->setDress(pWarrior->getWid(), pAvatar->getId(),NULL,NULL,true,showBusy);
}
void WarriorModel::dressUp(avatar::AvatarBaseInfo* pAvatar,bool showBusy)
{
	if(pAvatar==NULL) return;
	WarriorCurInfo* pWarrior = getWarriorCurInfoByWID(pAvatar->getWarriorId());
	if(pWarrior==NULL || pWarrior->getDressId()==pAvatar->getId()) return;
	HTTP_CLIENT->setDress(pWarrior->getWid(), pAvatar->getId(),NULL,NULL,true,showBusy);
}

const IconsInfo* WarriorModel::getWarriorIconInfoByType( unsigned int wid, WarriorIconType type )
{
	IconsInfo* info = NULL;
	CCDictionary* pIconsMap =  dynamic_cast<CCDictionary*>(m_warIconInfoMap->objectForKey(wid));
	if (pIconsMap)
	{
		info = dynamic_cast<IconsInfo*>(pIconsMap->objectForKey(type));
	}
	
	return info;
}

warrior::TalentInfo* WarriorModel::getTalentInfoById( uint32_t talentId )
{
	if (m_talentInfoMap == NULL)
	{
		return NULL;
	}
	
	return dynamic_cast<TalentInfo*>(m_talentInfoMap->objectForKey(talentId));
}

CCDictionary* WarriorModel::getWarriorTalentsDic( uint32_t warriorId )
{
	if (m_warriorTalentMap == NULL)
	{
		return NULL;
	}
	
	return dynamic_cast<CCDictionary*>(m_warriorTalentMap->objectForKey(warriorId));
}

SoulInfo* SoulInfo::create(unsigned int wid,unsigned int num)
{
    SoulInfo* pRet = SoulInfo::create();
    if(pRet)
    {
        pRet->wid = wid;
		pRet->num = num;
        pRet->baseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wid);
    }
    return pRet;
}

IconsInfo* warrior::IconsInfo::create( std::string infoString )
{
	IconsInfo* pRet = IconsInfo::create();
	if (pRet)
	{
		std::vector<std::string> elems;
		split(infoString, ",", elems);
		if (elems.size() >=3 )
		{
			pRet->origin.x = atoi(elems[0].c_str());
			pRet->origin.y = atoi(elems[1].c_str());
			pRet->scale = atof(elems[2].c_str());
		}
		else
		{
			CCLog("error config in warriorIconsInfo.");
		}
	}
	
	return pRet;
}

UpgradeItemInfo* warrior::UpgradeItemInfo::create(unsigned int _itemId, unsigned int _coinConsum, unsigned int _criticalTriggerValue, unsigned int _experience)
{
    UpgradeItemInfo* pRet = UpgradeItemInfo::create();
    if (pRet) {
        pRet->itemId = _itemId;
        pRet->coinConsum = _coinConsum;
        pRet->criticalTriggerValue = _criticalTriggerValue;
		pRet->experience = _experience;
    }
    
    return pRet;
}

UpgradeCriticalData* warrior::UpgradeCriticalData::create(unsigned int _type, float _factor, float _probability)
{
    UpgradeCriticalData* pRet = UpgradeCriticalData::create();
    if (pRet) {
        pRet->type = _type;
        pRet->factor = _factor;
        pRet->probability = _probability;
    }
    
    return pRet;
}
