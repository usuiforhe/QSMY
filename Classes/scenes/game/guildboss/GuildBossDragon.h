//
//  GuildBossDragon.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossDragon__
#define __QSMY__GuildBossDragon__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class GuildBossViewDelegate;

class GuildBossDragon 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public UIAnimateDelegate
{

public:
	GuildBossDragon();
	~GuildBossDragon();

	static GuildBossDragon* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();
	void updateStar();
	void updateRescue();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	virtual void animationComplete(UIAnimate *pAnimate);

	void callBackBack(CCObject *pSender);
	void callBackRescue(CCObject *pSender);
	void callBackBattle(CCObject *pSender);

	void callBackRank(CCObject *pSender);
	void callBackContribution(CCObject *pSender);

	void callBackAutoAttack(CCObject *pSender);

	void playAutoAttackAnimation();
	void playAttackAnimation();

	void armatureComplete(sp::Armature * target, const char* event,const char* data);
	void otherArmatureComplete(sp::Armature * target, const char* event,const char* data);

	void playAutoAttackPart(CCObject *pObj);
protected:
	CC_SYNTHESIZE(GuildBossViewDelegate *, m_pDelegate, Delegate);
private:
	cocos2d::extension::CCBReader *m_pReader;

	CCMenuItem *m_pBtnBack;
	DButton *m_pBtnRescue;
	CCMenuItem *m_pBtnBattle;

	CCLabelBMFont *m_pBMFCountFree;
	CCLabelBMFont *m_pBMFCountPayed;
	CCLabelBMFont *m_pBMFGold;
	CCLabelBMFont *m_pBMFTime;
	CCLabelBMFont *m_pBMFFight;
	CCLabelBMFont *m_pBMFSelfStar;
	CCLabelBMFont *m_pBMFRescue;
	CCLabelTTF *m_pTTFName;
	UIAnimate *m_pAnimateAttack;
	UIAnimate *m_pAnimateAttack10;
	UIAnimate *m_pAnimateDragon;
	UIAnimate *m_pAnimateAttackOther;
	UIAnimate *m_pAnimateAttackName;
	UIAnimate *m_pAnimateDamage;
	UIAnimate *m_pAnimateRescue;

	CCNode *m_pNodeTen;
	CCNode *m_pNodeFree;
	CCNode *m_pNodePay;

	CCLabelBMFont *m_pBMFCountDown;
	CCNode *m_pTxtZhan;
	CCNode *m_pNodeAttack;
	CCNode *m_pNodeHealthy;
	CCNode *m_pNodeWounded;
	CCNode *m_pNodeRescue;

	GuildBossModel *m_pModel;

	float m_fWaitTime;
	float m_fColdDelay;

	CCLabelBMFont *m_pBMFOtherDamage;
	CCLabelTTF *m_pTTFOtherName;
	CCLabelBMFont *m_pBMFDamage;
	CCLabelTTF *m_pTTFGBMessage;

	CCNode *m_pTouchFlashNode;
	CCNode *m_pNodeNormalAttack;
	CCNode *m_pNodeAutoAttack;
	CCMenuItem *m_pBtnAutoAttack;
	DProgressBar *m_pPBLeft;
	DProgressBar *m_pPBRight;

	DButton *m_pBtnGuildInfo;
	DButton *m_pBtnPersonInfo;
	CCNode *m_pCriticalNode;

	void update(float dt);

	void playNoticeAnimation();
	void playOtherAnimation();

	void playRescueAnimation();

	void playTouchAnimation();

	bool m_bRunningOtherAnimation;

	bool m_bProgressActionShown;

	float m_fLastPercent;
};

#endif