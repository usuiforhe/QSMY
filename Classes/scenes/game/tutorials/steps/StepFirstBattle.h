//
//  StepFirstBattle.h
//  QSMY
//
//  Created by wanghejun on 14-2-12.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__StepFirstBattle__
#define __QSMY__StepFirstBattle__

#include "../Tutorial.h"
#include "../../../../components/video/VideoPlayer.h"

class Armature;

class StepFirstBattle
:public StepEnForce
,public VideoPlayerDelegate
{
public:
    StepFirstBattle();
    virtual ~StepFirstBattle();
    virtual void loadUI();
private:
    bool simTouch;
private:
    void loadBattle();
    void endBattle();
    
    virtual bool dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches = NULL);
    virtual void dTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    void playVideo();
	void playAnimation();
    
	//VideoPlayerDelegate
	virtual void playEnd();

	virtual void animationComplete(sp::Armature * target, const char* event,const char* data);
private:
	VideoPlayer* m_pVideoPlayer;
	sp::Armature*	m_pAfterBttleAnimation;
	std::vector<unsigned int> m_vAnimationStepInfo;
	unsigned int m_animationStep;
	unsigned int m_animationSubStep;
};

#endif /* defined(__QSMY__StepFirstBattle__) */
