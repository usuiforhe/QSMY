//
//  StepFirstBattle.cpp
//  QSMY
//
//  Created by wanghejun on 14-2-12.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "StepFirstBattle.h"


#include "StepPve.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"
#include "managers/SoundManager.h"
#include "../../Game.h"
#ifdef CRYPT
#include "../../../../utils/aes.h"
#endif


USING_NS_CC;
using namespace sp;

static const char* g_battleFileName = "data/TFB.txt";

StepFirstBattle::StepFirstBattle()
	:simTouch(false)
	,m_pVideoPlayer(NULL)
	,m_pAfterBttleAnimation(NULL)
	,m_animationStep(0)
	,m_animationSubStep(0)
{
	m_vAnimationStepInfo.insert(m_vAnimationStepInfo.begin(), 4, 1);
	m_vAnimationStepInfo[2] = 2;
	m_vAnimationStepInfo[3] = 2;
}

StepFirstBattle::~StepFirstBattle()
{
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);

	if (m_pVideoPlayer)
	{
		m_pVideoPlayer->setDelegate(NULL);
	}
	
	CC_SAFE_RELEASE(m_pVideoPlayer);
}

void StepFirstBattle::loadUI()
{
    SoundManager::sharedSoundManager()->stopBackgroundMusic();
    CCLayerColor* pLayer = CCLayerColor::create(ccc4(0, 0, 0, 255),D_DESIGN_SIZE_WIDTH,D_DESIGN_SIZE_HEIGHT);
    addChild(pLayer);
	
	NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(StepFirstBattle::endBattle),D_EVENT_END_BATTLE);
	simTouch = true;

    playVideo();
}

void StepFirstBattle::playVideo()
{
    std::string videoPath;
     videoPath = "video/tutorial.mp4";

    if (videoPath.length() == 0) {
        playEnd();
    }
    
    m_pVideoPlayer = VideoPlayer::create();
	if (m_pVideoPlayer)
	{
		m_pVideoPlayer->setDelegate(this);
		CC_SAFE_RETAIN(m_pVideoPlayer);
		m_pVideoPlayer->playVideo(videoPath.c_str());
	}
	else
	{
		playEnd();
	}
}

void StepFirstBattle::loadBattle()
{
    std::string fileName(g_battleFileName);
    fileName += SUFFIX;
	 if(CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName.c_str())))
    {
		unsigned long size(0);
       unsigned char* pContent = CCFileUtils::sharedFileUtils()->getFileData(fileName.c_str(), "rb", &size);
		std::string jsonStr((char*)pContent,size);
		CC_SAFE_DELETE_ARRAY(pContent);
#ifdef CRYPT
		jsonStr = DecodeAES(DATA_PASSWORD, jsonStr);
#endif

		Json::Value root;
        Json::Reader reader;
        reader.parse(jsonStr, root);
		
        DM_GET_BATTLE_MODEL->parseInfo(root);
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE,cocos2d::CCInteger::create(battle::kSkipBattleTypeSkip));
		DM_GET_BATTLE_MODEL->setIsShowResult(false);
    }
	 else
    {
        endBattle();
    }

}

void StepFirstBattle::endBattle()
{
    /*防止穿帮*/
    CCLayerColor* pLayer = CCLayerColor::create(ccc4(0, 0, 0, 255),D_DESIGN_SIZE_WIDTH,D_DESIGN_SIZE_HEIGHT);
    addChild(pLayer);
  
	playAnimation();
}

bool StepFirstBattle::dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches)
{
	if (m_pVideoPlayer)
	{
        playEnd();
		return true;
	}

	if (m_pAfterBttleAnimation)
	{
		animationComplete(m_pAfterBttleAnimation, COMPLETE,NULL);
		return true;
	}
	
    if(simTouch && StepEnForce::dTouchBegan(pTouch, pEvent,bSwallowsTouches))
    {
        Game::sharedGame()->dTouchBegan(pTouch, NULL);
        return true;
    }
    return true;
}

void StepFirstBattle::dTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    StepEnForce::dTouchEnded(pTouch, pEvent);
    Game::sharedGame()->dTouchEnded(pTouch, NULL);
}

void StepFirstBattle::playEnd()
{
	//播放完手动释放播放器，否则引发战斗时触屏bug
    if(m_pVideoPlayer)
    {
        m_pVideoPlayer->setDelegate(NULL);
		CC_SAFE_RELEASE_NULL(m_pVideoPlayer);
        removeAllChildren();
        loadBattle();
    }
	else
	{
		CCLog("Illegal Invoke playend!!!!");
        /*防止穿帮*/
        CCLayerColor* pLayer = CCLayerColor::create(ccc4(0, 0, 0, 255),D_DESIGN_SIZE_WIDTH,D_DESIGN_SIZE_HEIGHT);
        addChild(pLayer);
        finishStep();
	}
}

void StepFirstBattle::playAnimation()
{
	if (m_animationStep >= m_vAnimationStepInfo.size())
	{
		/*防止穿帮*/
		CCLayerColor* pLayer = CCLayerColor::create(ccc4(0, 0, 0, 255),D_DESIGN_SIZE_WIDTH,D_DESIGN_SIZE_HEIGHT);
		addChild(pLayer);
		finishStep();
		return;
	}
	
	if (m_animationSubStep < m_vAnimationStepInfo[m_animationStep] - 1 && m_pAfterBttleAnimation)
	{
		m_animationSubStep++;
		m_pAfterBttleAnimation->getAnimation()->playByIndex(m_animationSubStep);
	}
	else
	{
		if (m_pAfterBttleAnimation)
		{
			m_pAfterBttleAnimation->removeFromParentAndCleanup(true);
			m_pAfterBttleAnimation = NULL;
		}
		
		m_animationSubStep = 0;
		CCString* animationName = CCString::createWithFormat("tutorial_%d", m_animationStep + 1);
		CCLog("%s", animationName->getCString());
		m_pAfterBttleAnimation =ResourceManager::sharedResourceManager()->getUIAnimate(animationName->getCString());
		m_animationStep++;
		if (m_pAfterBttleAnimation)
		{
			addChild(m_pAfterBttleAnimation);
			m_pAfterBttleAnimation->setPosition(D_DESIGN_POINT_CENTER);
			m_pAfterBttleAnimation->getAnimation()->addObserver(this, SPNotification_callfunc_selector(StepFirstBattle::animationComplete), COMPLETE);
		}
		else
		{
			playAnimation();
		}
	}//else end

}

void StepFirstBattle::animationComplete(sp::Armature * target, const char* event,const char* data)
{
	playAnimation();
}







