//
//  SoundManager.h
//  QSMY
//
//  Created by wanghejun on 13-6-1.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SoundManager__
#define __QSMY__SoundManager__

#include "AppInclude.h"
#include "model/SettingModel.h"
#include "model/SoundModel.h"

class SoundManager :public cocos2d::CCObject
{
    
public:
    /** returns a shared instance of the SoundManager */
    static SoundManager* sharedSoundManager(void);
    static void purgeSoundManager(void);

public:
    void playBattleSound();
    void playMainUISound();
	void playLoginSound();
    void playChallengeSound();
    void playBossSound();
    void playMusic();
    void playSFX(cocos2d::CCObject *sender);
	unsigned int playSFX(cocos2d::CCString* pName);
    void playSound(sound::Sound *pSound);
	void stopSound(cocos2d::CCString *pID);
	void stopSFX(uint32_t nID);
    void stopAllEffects();
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void pauseAllEffects();
    void resumeBackgroundMusic();
    void resumeAllEffects();
    
    void preloadEffect(cocos2d::CCString* pName);
    void unloadEffect(cocos2d::CCString* pName);
    
private:
    
	void playLangSound(cocos2d::CCObject *sender);
	void playAnimationSound(cocos2d::CCObject *sender);
    
private:
    SoundManager(void);
    ~SoundManager(void);
    bool init(void);
    
    void updateMusicStatus();
    void updateSfxStatus();
    
private:
    SettingModel * m_pSettingModel;
    std::string music_name;
    std::string playing_music_name;
    cocos2d::CCNode *m_pHelperNode;
};


#endif /* defined(__QSMY__SoundManager__) */