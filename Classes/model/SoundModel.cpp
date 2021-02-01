//
//  SoundModel.cpp
//  QSMY
//
//  Created by Eci on 13-12-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoundModel.h"
#include "managers/DataManager.h"
#include "utils/FileUtil.h"

#define SoundTypeMax 6

const char *SoundTypeName[SoundTypeMax] = {"unit","buff","skill","effect","unit_effect", ""};

USING_NS_CC;
using namespace sound;

SoundModel::SoundModel()
	:m_pUnitSound(NULL)
	,m_pBuffSound(NULL)
	,m_pSkillSound(NULL)
	,m_pEffectSound(NULL)
	,m_pUnitEffSound(NULL)  
	,m_pSkillKillSound(NULL)
	,m_pSpecialSkillSound(NULL)
	,m_pSkillNoSound(NULL)
	,m_pLocalSound(NULL)
	,m_pAnimationSound(NULL)
{

}

SoundModel::~SoundModel()
{
	CC_SAFE_RELEASE(m_pUnitSound);
	CC_SAFE_RELEASE(m_pBuffSound);
	CC_SAFE_RELEASE(m_pSkillSound);
	CC_SAFE_RELEASE(m_pEffectSound);
	CC_SAFE_RELEASE(m_pUnitEffSound);
	CC_SAFE_RELEASE(m_pSkillKillSound);
	CC_SAFE_RELEASE(m_pSpecialSkillSound);
	CC_SAFE_RELEASE(m_pSkillNoSound);
	CC_SAFE_RELEASE(m_pLocalSound);
	CC_SAFE_RELEASE(m_pAnimationSound);
}

bool SoundModel::init()
{
	m_pUnitSound = SoundMap::create();
	CC_SAFE_RETAIN(m_pUnitSound);
	m_pBuffSound = SoundMap::create();
	CC_SAFE_RETAIN(m_pBuffSound);
	m_pSkillSound = SoundMap::create();
	CC_SAFE_RETAIN(m_pSkillSound);
	m_pEffectSound = SoundMap::create();
	CC_SAFE_RETAIN(m_pEffectSound);
	m_pUnitEffSound = SoundMap::create();
	CC_SAFE_RETAIN(m_pUnitEffSound);
	m_pSkillKillSound = SoundMap::create();
	CC_SAFE_RETAIN(m_pSkillKillSound);
	m_pSpecialSkillSound = SoundMap::create();
	CC_SAFE_RETAIN(m_pSpecialSkillSound);
	m_pSkillNoSound = SoundMap::create();
	CC_SAFE_RETAIN(m_pSkillNoSound);
	m_pLocalSound = SoundMap::create();
	CC_SAFE_RETAIN(m_pLocalSound);
	m_pAnimationSound = SoundMap::create();
	CC_SAFE_RETAIN(m_pAnimationSound);

	parseUnitSound( CSVDecoder::create(UNITACTION_CONFIG_PATH)->getData());
	parseSound( CSVDecoder::create(SKILLEFFECT_CONFIG_PATH)->getData());
	parseSkillKillSound();
	parseSpecialSkillSound();
	parseSkillNoSound( CSVDecoder::create(SKILL_NO_SOUND_CONFIG_PATH)->getData());
	parseErrorSound( CSVDecoder::create(LOCAL_VOICE_CONFIG_PATH)->getData());
	parseAnimationSound( CSVDecoder::create(ANIMATIONSOUND_CONFIG_PATH)->getData());
	parseWarriorVoice(CSVDecoder::create(WARRIORVOICE_CONFIG_PATH)->getData());
	return Model::init();
}

void SoundModel::parseWarriorVoice(const CSVDecoder::CSV &val)
{
	SoundMap *pMap = m_pUnitSound;
	for (uint32_t i=0; i<val.size(); ++i)
	{
		Sound *pSound = Sound::create();
		//pSound->m_iType = iType;
		pSound->m_sSid = val[i][0];
		pSound->m_iIdx = atoi(val[i][1].c_str());
		pSound->m_sName = CCString::createWithFormat("warrior_talk/%s", val[i][2].c_str())->getCString();//val[i][2];
		pSound->m_fDelay = D_MILLISECOND_TO_SECOND(atoi(val[i][3].c_str()));
		pSound->m_sAction = val[i][4];
		pSound->m_iType = soundTypeUnit;
		pSound->m_fDuration = D_MILLISECOND_TO_SECOND(atoi(val[i][5].c_str()));


		CCString *pKey = CCString::createWithFormat("%s_%u_%s", pSound->m_sSid.c_str(), pSound->m_iIdx, pSound->m_sAction.c_str());

		CCArray *pArr = (CCArray *)pMap->objectForKey(pKey->getCString());
		if (NULL == pArr) {
			pArr = CCArray::create();
			pMap->setObject(pArr, pKey->getCString());
		}
		pArr->addObject(pSound);
	}	
}

void SoundModel::parseSound(const CSVDecoder::CSV &val)
{
	SoundMap *maps[SoundTypeMax] = {m_pUnitSound, m_pBuffSound, m_pSkillSound, m_pEffectSound, m_pUnitEffSound, m_pSkillKillSound};
	SoundMap *pMap = NULL;
	for (uint32_t i=0; i<val.size(); ++i)
	{
		Sound *pSound = Sound::create();
		//pSound->m_iType = iType;
		pSound->m_sSid = val[i][0];
		pSound->m_iIdx = atoi(val[i][1].c_str());
		pSound->m_sName = CCString::createWithFormat("skill/%s", val[i][2].c_str())->getCString();//val[i][2];
		pSound->m_fDelay = D_MILLISECOND_TO_SECOND(atoi(val[i][3].c_str()));
		pSound->m_sAction = val[i][4];

		if (val[i].size() < 7)
		{
			pSound->m_iType = soundTypeUnit;
		}
		else
		{
			std::string sType = val[i][6];
			for (uint32_t j = 0; j < SoundTypeMax; j++)
			{
				if (sType == SoundTypeName[j])
				{
					pSound->m_iType = (soundType)j;
					pMap = maps[j];
					break;
				}
			}
		}
		if (val[i].size() < 6) 
		{
			pSound->m_fDuration = .0f;
		}
		else
		{
			pSound->m_fDuration = D_MILLISECOND_TO_SECOND(atoi(val[i][5].c_str()));
		}
		if (!pMap)
		{
			continue;
		}
		

		CCString *pKey = CCString::createWithFormat("%s_%u_%s", pSound->m_sSid.c_str(), pSound->m_iIdx, pSound->m_sAction.c_str());

		CCArray *pArr = (CCArray *)pMap->objectForKey(pKey->getCString());
		if (NULL == pArr) {
			pArr = CCArray::create();
			pMap->setObject(pArr, pKey->getCString());
		}
		pArr->addObject(pSound);
	}	
}

void SoundModel::parseUnitSound(const CSVDecoder::CSV &val)
{
	SoundMap *pMap = m_pUnitSound;
	for (uint32_t i=0; i<val.size(); ++i)
	{
		Sound *pSound = Sound::create();
		//pSound->m_iType = iType;
		pSound->m_sSid = val[i][0];
		pSound->m_iIdx = atoi(val[i][1].c_str());
		pSound->m_sName = CCString::createWithFormat("skill/%s", val[i][2].c_str())->getCString();//val[i][2];
		pSound->m_fDelay = D_MILLISECOND_TO_SECOND(atoi(val[i][3].c_str()));
		pSound->m_sAction = val[i][4];
		pSound->m_iType = soundTypeUnit;
		pSound->m_fDuration = .0f;


		CCString *pKey = CCString::createWithFormat("%s_%u_%s", pSound->m_sSid.c_str(), pSound->m_iIdx, pSound->m_sAction.c_str());

		CCArray *pArr = (CCArray *)pMap->objectForKey(pKey->getCString());
		if (NULL == pArr) {
			pArr = CCArray::create();
			pMap->setObject(pArr, pKey->getCString());
		}
		pArr->addObject(pSound);
	}	
}

void SoundModel::parseSound(soundType iType)
{
	SoundMap *pMap = NULL;
    CSVDecoder *pDecoder = NULL;
	switch (iType)
	{
	case soundTypeUnit:
		pMap = m_pUnitSound;
            pDecoder = CSVDecoder::create(UNITACTION_CONFIG_PATH);
		break;
	case soundTypeBuff:
            pMap = m_pBuffSound;
            pDecoder = CSVDecoder::create(SKILLEFFECT_CONFIG_PATH);
		break;
	case soundTypeSkill:
            pMap = m_pSkillSound;
            pDecoder = CSVDecoder::create(SKILLEFFECT_CONFIG_PATH);
		break;
	case soundTypeEffect:
            pMap = m_pEffectSound;
            pDecoder = CSVDecoder::create(SKILLEFFECT_CONFIG_PATH);
		break;
	case soundTypeUnitEff:
            pMap = m_pUnitEffSound;
            pDecoder = CSVDecoder::create(SKILLEFFECT_CONFIG_PATH);
		break;
	case soundTypeSkillKill:
		parseSkillKillSound();
		return;
		break;
	case soundTypeNone:
	default:
		return;
		break;
	}

	if (NULL == pMap)
	{
		return;
	}
    if (NULL == pDecoder) {
        return;
    }
    const CSVDecoder::CSV val = pDecoder->getData();
	for (uint32_t i=0; i<val.size(); ++i)
	{
		Sound *pSound = Sound::create();
		pSound->m_iType = iType;
		pSound->m_sSid = val[i][0];
		pSound->m_iIdx = atoi(val[i][1].c_str());
		pSound->m_sName = CCString::createWithFormat("skill/%s", val[i][2].c_str())->getCString();//val[i][2];
		pSound->m_fDelay = D_MILLISECOND_TO_SECOND(atoi(val[i][3].c_str()));
		pSound->m_sAction = val[i][4];
        if (iType != soundTypeUnit) {
            pSound->m_fDuration = D_MILLISECOND_TO_SECOND(atoi(val[i][5].c_str()));
        }

		CCString *pKey = CCString::createWithFormat("%s_%u_%s", pSound->m_sSid.c_str(), pSound->m_iIdx, pSound->m_sAction.c_str());
        
        CCArray *pArr = (CCArray *)pMap->objectForKey(pKey->getCString());
        if (NULL == pArr) {
            pArr = CCArray::create();
            pMap->setObject(pArr, pKey->getCString());
        }
        pArr->addObject(pSound);
	}	
}

void SoundModel::parseSkillKillSound()
{
	SoundMap *pMap = m_pSkillKillSound;
	const CSVDecoder::CSV val =  CSVDecoder::create(SKILL_KILL_CONFIG_PATH)->getData();
	for (uint32_t i=0; i<val.size(); ++i)
	{
		Sound *pSound = Sound::create();
		pSound->m_iType = soundTypeSkillKill;
		pSound->m_sSid = val[i][0];
		pSound->m_iIdx = atoi(val[i][1].c_str());
		pSound->m_sName = CCString::createWithFormat("unit/%s_%u.mp3", pSound->m_sSid.c_str(), pSound->m_iIdx)->getCString();
		pSound->m_fDelay = 0.f;
		pSound->m_sAction = "";
		pSound->m_fDuration = 0.f;


		CCArray *pArr = (CCArray *)pMap->objectForKey(pSound->m_sSid.c_str());
		if (NULL == pArr)
		{
			pArr = CCArray::create();
			pMap->setObject(pArr, pSound->m_sSid.c_str());
		}
		pArr->addObject(pSound);

		//CCString *pKey = CCString::createWithFormat("%s_%u", pSound->m_sSid.c_str(), pSound->m_iIdx);

		//pMap->setObject(pSound, pKey->getCString());
	}	
}

void SoundModel::parseSpecialSkillSound()
{
	CCArray *pSkillArray = CCArray::create();
	DM_GET_SKILL_MODEL->getSpecialSkills(pSkillArray);
	for ( uint32_t i = 0; i < pSkillArray->count(); i++)
	{
		skill::Skill *pSkill = dynamic_cast<skill::Skill *>(pSkillArray->objectAtIndex(i));
		getSpecialSkillSoundByWid(pSkill->wid1);
		getSpecialSkillSoundByWid(pSkill->wid2);
		getSpecialSkillSoundByWid(pSkill->wid3);
	}
}

void SoundModel::getSpecialSkillSoundByWid(uint32_t wid)
{
	if (!wid)
	{
		return;
	}
	
	const char *sName = CCString::createWithFormat("sounds/sfx/unit/mix/%u.mp3", wid)->getCString();
	std::string sFull = CCFileUtils::sharedFileUtils()->fullPathForFilename(sName);
	if (sFull == sName)
	{
		return;
	}
	CCArray *pArr = (CCArray *)m_pSpecialSkillSound->objectForKey(wid);
	if (NULL == pArr)
	{
		pArr = CCArray::create();
		m_pSpecialSkillSound->setObject(pArr, wid);
	}
	Sound *pSound = Sound::create();
	pSound->m_iType = soundTypeSpecialSkill;
	pSound->m_sSid = CCString::createWithFormat("%u", wid)->getCString();
	pSound->m_iIdx = 0;
	pSound->m_sName = CCString::createWithFormat("unit/mix/%u.mp3", wid)->getCString();
	pSound->m_fDelay = 0.f;
	pSound->m_sAction = "";
	pSound->m_fDuration = 0.f;
	pArr->addObject(pSound);
}

void SoundModel::sendNotificationToPlay(CCArray *pArray)
{
    if (NULL == pArray) {
        return;
	}
    for (uint32_t i = 0; i < pArray->count(); i++) {
        Sound *pSound = (Sound *)pArray->objectAtIndex(i);
        if (NULL == pSound) {
            continue;
		}
		CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, pSound);
    }
}

void SoundModel::playUnitSound(uint32_t sid, uint32_t idx /* = 0 */, const char *action /* = "" */)
{
	warrior::WarriorBaseInfo *pWarrior = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(sid);
	if (!pWarrior)
	{
		return;
	}
	sid = pWarrior->getResId();

	const char *pKey = CCString::createWithFormat("%u_%u_%s", sid, idx, action)->getCString();
    sendNotificationToPlay((CCArray *)m_pUnitSound->objectForKey(pKey));
}

Sound* SoundModel::getUnitSound(uint32_t sid, uint32_t idx /* = 0 */, const char *action /* = "" */)
{
	warrior::WarriorBaseInfo *pWarrior = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(sid);
	if (!pWarrior)
	{
		return NULL;
	}
	sid = pWarrior->getResId();

	const char *pKey = CCString::createWithFormat("%u_%u_%s", sid, idx, action)->getCString();
	CCArray* pArray = (CCArray *)m_pUnitSound->objectForKey(pKey);

	if (NULL == pArray) {
		return NULL;
	}
	Sound *pSound = NULL;
	for (uint32_t i = 0; i < pArray->count(); i++) {
		pSound = (Sound *)pArray->objectAtIndex(i);
		if (NULL == pSound) {
			continue;
		}
		
	}
	return pSound;
}

void SoundModel::playUnitSound(std::string& sid, uint32_t idx /* = -1 */, const char *action /* = "" */)
{
	playUnitSound(atoi(sid.c_str()), idx, action);
}
void SoundModel::playBuffSound(uint32_t sid, uint32_t idx /* = 0 */, const char *action /* = "" */)
{
	skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(sid);
	if (!pSkill)
	{
		return;
	}
	sid = pSkill->getResId();

	const char *pKey = CCString::createWithFormat("%u_%u_%s", sid, idx, action)->getCString();
    sendNotificationToPlay((CCArray *)m_pBuffSound->objectForKey(pKey));
}
void SoundModel::playBuffSound(std::string& sid, uint32_t idx /* = -1 */, const char *action /* = "" */)
{
	playBuffSound(atoi(sid.c_str()), idx, action);
}
void SoundModel::playSkillSound(uint32_t sid, uint32_t idx /* = 0 */, const char *action /* = "" */)
{
	skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(sid);
	if (!pSkill)
	{
		return;
	}
	sid = pSkill->getResId();

	const char *pKey = CCString::createWithFormat("%u_%u_%s", sid, idx, action)->getCString();
    sendNotificationToPlay((CCArray *)m_pSkillSound->objectForKey(pKey));
}
void SoundModel::playSkillSound(std::string& sid, uint32_t idx /* = -1 */, const char *action /* = "" */)
{
	playSkillSound(atoi(sid.c_str()), idx, action);
}
void SoundModel::playEffectSound(uint32_t sid, uint32_t idx /* = 0 */, const char *action /* = "" */)
{
	std::string strID = CCString::createWithFormat("%u", sid)->getCString();
	playEffectSound(strID, idx, action);
}
void SoundModel::playEffectSound(std::string& sid, uint32_t idx /* = -1 */, const char *action /* = "" */)
{
	const char *pKey = CCString::createWithFormat("%s_%u_%s", sid.c_str(), idx, action)->getCString();
    sendNotificationToPlay((CCArray *)m_pEffectSound->objectForKey(pKey));
}
void SoundModel::playCommonEffectSound(const char *action /* = "" */, uint32_t idx /* = -1 */)
{
	const char *pKey = CCString::createWithFormat("%s_%u_%s", "effect", idx, action)->getCString();
	sendNotificationToPlay((CCArray *)m_pEffectSound->objectForKey(pKey));
}
void SoundModel::playUnitEffSound(uint32_t sid, uint32_t idx /* = 0 */, const char *action /* = "" */)
{
	skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(sid);
	if (!pSkill)
	{
		return;
	}
	sid = pSkill->getResId();

	const char *pKey = CCString::createWithFormat("%u_%u_%s", sid, idx, action)->getCString();
    sendNotificationToPlay((CCArray *)m_pUnitEffSound->objectForKey(pKey));
}
void SoundModel::playUnitEffSound(std::string& sid, uint32_t idx /* = -1 */, const char *action /* = "" */)
{
	playUnitEffSound(atoi(sid.c_str()), idx, action);
}
void SoundModel::playSkillKillSound(uint32_t sid, uint32_t idx /* = 0 */)
{
	warrior::WarriorBaseInfo *pWarrior = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(sid);
	if (!pWarrior)
	{
		return;
	}
	sid = pWarrior->getResId();

    CCObject *pSound = NULL;
	CCArray *pArr = (CCArray *)m_pSkillKillSound->objectForKey(CCString::createWithFormat("%u", sid)->getCString());
    if (NULL == pArr) {
        return;
    }
	if (-1 == idx || idx >= pArr->count())
	{
        pSound = pArr->randomObject();
	}
	else
	{
        pSound = pArr->objectAtIndex(idx);
	}
    if (NULL == pSound) {
        return;
    }
    sendNotificationToPlay(CCArray::createWithObject(pSound));
}

void SoundModel::playSpecialSkillSound(uint32_t sid, uint32_t idx /* = 0 */)
{
	warrior::WarriorBaseInfo *pWarrior = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(sid);
	if (!pWarrior)
	{
		return;
	}
	sid = pWarrior->getResId();

	CCObject *pSound = NULL;
	CCArray *pArr = (CCArray *)m_pSpecialSkillSound->objectForKey(sid);
	if (NULL == pArr) {
		return;
	}
	if (-1 == idx || idx >= pArr->count())
	{
		pSound = pArr->randomObject();
	}
	else
	{
		pSound = pArr->objectAtIndex(idx);
	}
	if (NULL == pSound) {
		return;
	}
	sendNotificationToPlay(CCArray::createWithObject(pSound));
}

// void SoundModel::playSkillKillSound(std::string& sid, uint32_t idx /* = -1 */)
// {
// 	playSkillKillSound(atoi(sid.c_str()), idx);
// }

void SoundModel::parseSkillNoSound(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		uint32_t skillID = atoi(val[i][0].c_str());
		m_pSkillNoSound->setObject(CCInteger::create(skillID), skillID);
	}
}

bool SoundModel::isSkillNoSound(uint32_t sid)
{
	skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(sid);
	if (!pSkill)
	{
		return false;
	}
	sid = pSkill->getResId();

	return m_pSkillNoSound->objectForKey(sid) != NULL;
}

void SoundModel::parseErrorSound(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		CCString *pSound = CCString::createWithFormat("local/%s", val[i][1].c_str());
		m_pLocalSound->setObject(pSound, val[i][0]);
	}	
}

CCString* SoundModel::getLocalSound(CCString *key)
{
	if (key)
	{
		CCString *pSound = static_cast<CCString *>(m_pLocalSound->objectForKey(key->getCString()));
        if(pSound)
        {
            pSound->retain();
            pSound->autorelease();
        }
		return pSound;
	}
    return NULL;
}


void SoundModel::parseAnimationSound(const CSVDecoder::CSV &val)
{
	SoundMap *pMap = m_pAnimationSound;
	for (uint32_t i=0; i<val.size(); ++i)
	{
		Sound *pSound = Sound::create();
		pSound->m_iType = soundTypeAnimation;
		pSound->m_sSid = val[i][0];
		pSound->m_iIdx = atoi(val[i][1].c_str());
		pSound->m_sName = CCString::createWithFormat("animation/%s", val[i][2].c_str())->getCString();
		pSound->m_fDelay = D_MILLISECOND_TO_SECOND(atoi(val[i][3].c_str()));
		pSound->m_sAction = val[i][4];
		pSound->m_fDuration = D_MILLISECOND_TO_SECOND(atoi(val[i][5].c_str()));


		CCString *pKey = CCString::createWithFormat("%s_%s", pSound->m_sSid.c_str(), pSound->m_sAction.c_str());
		CCLOG("%s", pKey->getCString());
		CCArray *pArr = (CCArray *)pMap->objectForKey(pKey->getCString());
		if (NULL == pArr)
		{
			pArr = CCArray::create();
			pMap->setObject(pArr, pKey->getCString());
		}
		pArr->addObject(pSound);
	}
}


void SoundModel::playAnimationSound(CCString *pKey)
{
	//CCLOG("%s", pKey->getCString());
	CCArray *pArr = dynamic_cast<CCArray *>(m_pAnimationSound->objectForKey(pKey->getCString()));
	if (pArr)
	{
		sendNotificationToPlay(pArr);
	}
}

