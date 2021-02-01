//
//  NavSoul.h
//  魂魄模块
//
//  Created by liuxiaogang on 13-10-18.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__SoulHunter__
#define __QSMY__SoulHunter__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "../common/OtherFormationView.h"
#include "../user/UserInfo.h"

#include "managers/GameManager.h"

#include "SoulHunterSuccess.h"
#include "SoulHunterFail.h"
#include "SoulHunterDefeate.h"


class SoulHunterView;
class SoulHunterPageView;
class SoulHunterRivalView;


#pragma mark -SoulHunterDelegate-
class SoulHunterDelegate
{
public:
	virtual void sendRivalRequst(warrior::WarriorCurInfo * pWarrior) = 0;
	virtual void changeRinalViewClosed() = 0;
};


class SoulHunter 
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public SoulHunterDelegate
,public BackKeyInterface
{
public:
	SoulHunter();
	~SoulHunter();

	static SoulHunter* create(NavigationDelegate* pDelegate);
	virtual bool init(NavigationDelegate* pDelegate);

	void updateView();
    
    void onEnter();
    void onExit();
    void update(float dt);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
private:
    virtual void showChooseRivalView();
	virtual void sendRivalRequst(warrior::WarriorCurInfo * pWarrior);
	virtual void changeRinalViewClosed();
private:

    SoulHunterModel					*m_pModel;
private:
    cocos2d::CCLabelBMFont			 *m_pRemainChTimes;
private:
	cocos2d::CCNode					 *m_pPopupLayer;
	cocos2d::CCNode					 *m_pContainer;
private:
	SoulHunterView					 *m_pHunter;
	warrior::WarriorCurInfo			 *m_pTargetWarrior;
	bool							  m_pChooseRivalViewIsShowing;
private:
	void showHunterView();
	void showHunterFinish();
	void showHunterSuccess();
	void showHunterFail();
	void showHunterDefeate();
	void removeAllPopupLayer();
public:
	virtual void onBackKey();
};
#endif	/*#define __QSMY__SoulHunter__*/
