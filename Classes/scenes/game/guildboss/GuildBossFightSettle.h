//
//  GuildBossFightSettle.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossFightSettle__
#define __QSMY__GuildBossFightSettle__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

#define PROGRESS_BAR_COUNT	4

class WarriorGradeBg;
class GuildBossViewDelegate;

class GuildBossFightSettle 
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	GuildBossFightSettle();
	~GuildBossFightSettle();

	static GuildBossFightSettle* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	void updateStatus();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void callBackSubmit(CCObject *pSender);

	void update(float dt);
	void updateRefresh(float dt);

	void setState(guildboss::GuildBossState eState);

	void closeCallBack(CCObject*);
protected:
	CC_SYNTHESIZE(GuildBossViewDelegate *, m_pDelegate, Delegate);
private:
	DButton *m_pConfirm;
	DButton *m_pCloseButton;
	CCNode *m_pNodeHead;
	CCLabelTTF *m_pTTFName;
	CCLabelBMFont *m_pBMFLevel;
	CCLabelBMFont *m_pBMFDamageAdd;
	CCLabelBMFont *m_pBMFDamageAll;
	CCLabelBMFont *m_pBMFDamageRob;

	CCNode *m_pNodeHealthy;
	CCNode *m_pNodeHurtBoss;
	CCNode *m_pNodeHurtRob;

	CCNode *m_pNodeBoss;
	CCNode *m_pNodeRob;
	CCNode *m_pNodeWin;
	CCNode *m_pNodeLose;
	CCNode *m_pNodePercent80;
	CCNode *m_pNodePercent50;
	DProgressBar		*m_pGrade[PROGRESS_BAR_COUNT];
	WarriorGradeBg	*m_pWarriorGradeBg;

	GuildBossModel *m_pModel;

	float m_fWaitTime;
	guildboss::GuildBossState m_eState;
	bool m_bWounded;
};

#endif