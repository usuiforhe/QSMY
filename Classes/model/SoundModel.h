//
//  SoundModel.h
//  音效信息
//
//  Created by Eci on 13-12-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SoundModel__
#define __QSMY__SoundModel__

#include "Model.h"
#include "../AppMacros.h"

//Global.ITEMNAME = ["unit","buff","skill","effect","unit_effect"];

class SoundModel;

namespace sound
{
	enum soundType
	{
		soundTypeNone = 0,
		soundTypeUnit,
		soundTypeBuff,
		soundTypeSkill,
		soundTypeEffect,
		soundTypeUnitEff,
		soundTypeSkillKill,
		soundTypeSpecialSkill,
		soundTypeAnimation,
	};

	class Sound:public ValueObjcet
	{
	public:
		Sound()
			:m_iType(soundTypeNone)
			,m_sSid("")
			,m_iIdx(0)
			,m_sName("")
			,m_fDelay(0.f)
			,m_sAction("")
			,m_fDuration(0.f)
		{

		}
		~Sound()
		{

		}
		CREATE_FUNC(Sound);
		D_INIT_DEFAULT(ValueObjcet);
	protected:
		friend class ::SoundModel;

		CC_SYNTHESIZE_READONLY(soundType, m_iType, Type);
		CC_SYNTHESIZE_READONLY(std::string, m_sSid, Sid);
		CC_SYNTHESIZE_READONLY(uint32_t, m_iIdx, Idx);
		CC_SYNTHESIZE_READONLY(std::string, m_sName, Name);
		CC_SYNTHESIZE_READONLY(float, m_fDelay, Delay);
		CC_SYNTHESIZE_READONLY(std::string, m_sAction, Action);
		CC_SYNTHESIZE_READONLY(float, m_fDuration, Duration);
	};
}

class SoundModel : public Model
{
	typedef cocos2d::CCDictionary SoundMap;
public:
	SoundModel();
	~SoundModel();

	virtual bool init();

	void playUnitSound(uint32_t sid, uint32_t idx = 0, const char *action = "none");
	void playBuffSound(uint32_t sid, uint32_t idx = 0, const char *action = "none");
	void playSkillSound(uint32_t sid, uint32_t idx = 0, const char *action = "none");
	void playEffectSound(std::string& sid, uint32_t idx = 0, const char *action = "none");
	void playEffectSound(uint32_t sid, uint32_t idx = 0, const char *action = "none");
	void playCommonEffectSound(const char *action = "none", uint32_t idx = 0);
	void playUnitEffSound(uint32_t sid, uint32_t idx = 0, const char *action = "none");
	void playSkillKillSound(uint32_t sid, uint32_t idx = -1);
	void playSpecialSkillSound(uint32_t sid, uint32_t idx = -1);
	sound::Sound* getUnitSound(uint32_t sid, uint32_t idx = 0, const char *action = "none");
protected:
private:
	void parseSound(sound::soundType iType);
	void parseSound(const CSVDecoder::CSV &val);
	void parseUnitSound(const CSVDecoder::CSV &val);
	void parseWarriorVoice(const CSVDecoder::CSV &val);

	void parseSkillKillSound();
	void parseSpecialSkillSound();
	void getSpecialSkillSoundByWid(uint32_t wid);
    
    void sendNotificationToPlay(cocos2d::CCArray *pArray);

	SoundMap *m_pUnitSound;
	SoundMap *m_pBuffSound;
	SoundMap *m_pSkillSound;
	SoundMap *m_pEffectSound;
	SoundMap *m_pUnitEffSound;
	SoundMap *m_pSkillKillSound;
	SoundMap *m_pSpecialSkillSound;

	void playUnitSound(std::string& sid, uint32_t idx = 0, const char *action = "none");
	void playBuffSound(std::string& sid, uint32_t idx = 0, const char *action = "none");
	void playSkillSound(std::string& sid, uint32_t idx = 0, const char *action = "none");
	void playUnitEffSound(std::string& sid, uint32_t idx = 0, const char *action = "none");
//	void playSkillKillSound(std::string& sid, uint32_t idx = -1);
	

public:
	bool isSkillNoSound(uint32_t sid);
private:
	void parseSkillNoSound(const CSVDecoder::CSV &val);

	cocos2d::CCDictionary *m_pSkillNoSound;

#pragma mark error voice
public:
	cocos2d::CCString* getLocalSound(cocos2d::CCString *obj);
private:
	SoundMap *m_pLocalSound;
	void parseErrorSound(const CSVDecoder::CSV &val);

#pragma mark animation sound
public:
	void playAnimationSound(cocos2d::CCString *pKey);
private:
	SoundMap *m_pAnimationSound;
	void parseAnimationSound(const CSVDecoder::CSV &val);
};


#endif /* defined(__QSMY__SoundModel__) */