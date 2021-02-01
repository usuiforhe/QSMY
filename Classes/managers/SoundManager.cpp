//
//  SoundManager.cpp
//  QSMY
//
//  Created by wanghejun on 13-6-1.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoundManager.h"
#include "DataManager.h"
#include "utils/TimerUtil.h"

using namespace CocosDenshion;

// singleton stuff
static SoundManager *s_SharedSoundManager = NULL;

SoundManager* SoundManager::sharedSoundManager(void)
{
    if (!s_SharedSoundManager)
    {
        s_SharedSoundManager = new SoundManager();
        s_SharedSoundManager->init();
    }
    
    return s_SharedSoundManager;
}


void SoundManager::purgeSoundManager(void)
{
    
    
    CC_SAFE_RELEASE_NULL(s_SharedSoundManager);
}


SoundManager::SoundManager()
:music_name("")
,playing_music_name("")
,m_pSettingModel(NULL)
,m_pHelperNode(NULL)
{
    
}

SoundManager::~SoundManager()
{
    SAFE_RELEASE_UNBINDING_ALL(m_pSettingModel, this);
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CC_SAFE_RELEASE(m_pHelperNode);
}

bool SoundManager::init(void)
{
    SAFE_RELEASE_UNBINDING_ALL(m_pSettingModel, this);
    m_pSettingModel = DM_GET_SETTING_MODEL;
    CC_SAFE_RETAIN(m_pSettingModel);
    BINDING_ONLY(m_pSettingModel, this, SoundManager::updateMusicStatus, D_EVENT_SETTING_MUSIC_STATUS_UPDATED);
    BINDING_ONLY(m_pSettingModel, this, SoundManager::updateSfxStatus, D_EVENT_SETTING_SFX_STATUS_UPDATED);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SoundManager::playSFX), PLAY_SFX, NULL);
    if(m_pHelperNode)
    {
        m_pHelperNode->cleanup();
        m_pHelperNode->onExit();
        CC_SAFE_RELEASE(m_pHelperNode);
    }
    m_pHelperNode = CCNode::create();
    CC_SAFE_RETAIN(m_pHelperNode);
    m_pHelperNode->onEnter();
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(D_CONFIG_FLOAT_FOR_KEY(VOLUME_BGM));
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(D_CONFIG_FLOAT_FOR_KEY(VOLUME_SFX));
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfuncO_selector(SoundManager::playLangSound), FIND_LOCAL_STRING, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SoundManager::playAnimationSound), PLAY_ANIMATION, NULL);
    return true;
}


void SoundManager::updateMusicStatus()
{
   if(!m_pSettingModel->getMusicStatus())
   {
       SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
   }else
   {
       playing_music_name="";
       playMainUISound();
   }
}
void SoundManager::updateSfxStatus()
{
    if(!m_pSettingModel->getSfxStatus())
    {
        SimpleAudioEngine::sharedEngine()->stopAllEffects();
    }
}

void SoundManager::playBattleSound()
{
    #define BATTLE_MUSIC_NUM 3
    static uint8_t battleMusicId(0);
    music_name = cocos2d::CCString::createWithFormat("sounds/music/battle_0%d.mp3", (1 + (battleMusicId++) % BATTLE_MUSIC_NUM))->getCString();
    playMusic();
}

void SoundManager::playLoginSound()
{
	const std::string loginSound ="sounds/music/login.mp3";
	if(music_name != loginSound)
	{
		music_name = loginSound;
		playMusic();
	}
}

void SoundManager::playMainUISound()
{
    #define MAIN_UI_MUSIC_NUM 3
    static uint8_t mainUiMusicId(0);
    if(playing_music_name.find("mainUI")!=std::string::npos) return;
    music_name = cocos2d::CCString::createWithFormat("sounds/music/mainUI_0%d.mp3", (1 + (mainUiMusicId++) % MAIN_UI_MUSIC_NUM))->getCString();
    
    playMusic();
}

void SoundManager::playChallengeSound()
{
    const std::string challengeSound ="sounds/music/challenge.mp3";
    if(music_name != challengeSound)
    {
        music_name = challengeSound;
        playMusic();
    }
}

void SoundManager::playBossSound()
{
#define BOSS_MUSIC_NUM 2
    static uint8_t bossMusicId(0);
    music_name = cocos2d::CCString::createWithFormat("sounds/music/BOSS_0%d.mp3", (1 + (bossMusicId++) % BOSS_MUSIC_NUM))->getCString();
    
    playMusic();
}

void SoundManager::playMusic()
{
    if(!m_pSettingModel->getMusicStatus()) return;
    if(music_name.compare("")==0) return;
    if(playing_music_name==music_name) return;
    playing_music_name = music_name;
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(music_name.c_str(), true);
}

void SoundManager::playSFX(CCObject *sender)
{
	float delay = 0.f;
    
	if (dynamic_cast<CCString *>(sender))
	{
        playSFX(static_cast<CCString*>(sender));
	}
	else if (dynamic_cast<sound::Sound *>(sender))
	{
		sound::Sound *pSound = (sound::Sound *)sender;
		delay = pSound->getDelay();
        
        if (!D_FLOAT_EQUALS(delay, 0.f))
        {
            CCAction* pAction = CCSequence::create(CCDelayTime::create(delay),
                                                   CCCallFuncO::create(this, callfuncO_selector(SoundManager::playSound), pSound),
                                                   NULL
                                                   );
            m_pHelperNode->runAction(pAction);
            return;
        }
        //playSFX(ccs(pSound->getName()));
		playSound(pSound);
	}
	else
    {
      playSFX(DM_GET_CONFIG_MODEL->getConfigByKey(DEFAULT_SFX));
    }
}

unsigned int SoundManager::playSFX(CCString* pName)
{	
	if (!m_pSettingModel->getSfxStatus())
	{
		return 0;
	}
    std::string sfx_name = pName->getCString();
    if (sfx_name.compare("") == 0)
	{
		return 0;
	}
    if (sfx_name.substr(sfx_name.length() - 4, 4) == ".mp3" || sfx_name.substr(sfx_name.length() - 4, 4) == ".MP3") {
        sfx_name = CCString::createWithFormat("sounds/sfx/%s", sfx_name.c_str())->getCString();
    }
    else
    {
        sfx_name = CCString::createWithFormat("sounds/sfx/%s.mp3", sfx_name.c_str())->getCString();
    }
	return SimpleAudioEngine::sharedEngine()->playEffect(sfx_name.c_str(), false);
}

void SoundManager::playSound(sound::Sound *pSound)
{
	unsigned int soundID = playSFX(ccs(pSound->getName()));
	if (kCCActionTagInvalid != soundID)
	{
		m_pHelperNode->stopActionByTag(soundID);
	}
	
	if (!D_FLOAT_EQUALS(pSound->getDuration(), 0.f))
	{	
		CCAction* pAction = CCSequence::create(CCDelayTime::create(pSound->getDuration()),
			CCCallFuncO::create(this, callfuncO_selector(SoundManager::stopSound), CCString::createWithFormat("%u", soundID)),
			NULL
			);
		pAction->setTag(soundID);
		m_pHelperNode->runAction(pAction);
	}
	
}

void SoundManager::stopSound(CCString *pID)
{
	//SimpleAudioEngine::sharedEngine()->stopEffect(pID->uintValue());
	this->stopSFX(pID->uintValue());
}

void SoundManager::stopSFX(uint32_t nID)
{
	SimpleAudioEngine::sharedEngine()->stopEffect(nID);
}

void SoundManager::stopAllEffects()
{
    m_pHelperNode->stopAllActions();
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
}


void SoundManager::stopBackgroundMusic()
{
    playing_music_name="";
    music_name="";
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void SoundManager::pauseBackgroundMusic()
{
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void SoundManager::pauseAllEffects()
{
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

void SoundManager::resumeBackgroundMusic()
{
    if(!m_pSettingModel->getMusicStatus()) return;
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void SoundManager::resumeAllEffects()
{
    
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void SoundManager::playLangSound(cocos2d::CCObject *sender)
{
	CCString *key = static_cast<CCString *>(sender);
	CCString* pSound =  DM_GET_SOUND_MODEL->getLocalSound(key);
	if(pSound)
    {
        static uint32_t lastId = 0;
        if(lastId) stopSFX(lastId);
        lastId = playSFX(pSound);
    }
}

void SoundManager::playAnimationSound(cocos2d::CCObject *sender)
{
	CCString *key = dynamic_cast<CCString *>(sender);
	if (key)
	{
		DM_GET_SOUND_MODEL->playAnimationSound(key);
	}

}

void SoundManager::preloadEffect(cocos2d::CCString *pName)
{
    if(pName == NULL)
    {
        return;
    }
    
    SimpleAudioEngine::sharedEngine()->preloadEffect(pName->getCString());
}

void SoundManager::unloadEffect(cocos2d::CCString *pName)
{
    if (pName == NULL)
    {
        return;
    }
    
    SimpleAudioEngine::sharedEngine()->unloadEffect(pName->getCString());
}
