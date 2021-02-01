//
//  GuildBossFightCell.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__GuildBossFightCell__
#define __QSMY__GuildBossFightCell__

#define GUILDBOSS_MAX_BLOOD	5

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class GuildBossFightCell;

class GuildBossFightCellDelegate
{
public:
	virtual void GuildBossFightUser(GuildBossFightCell *pNode, guildboss::GuildBossUserBasic *pTarget) = 0;
};

class GuildBossFightCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	GuildBossFightCell();
	~GuildBossFightCell();

	static GuildBossFightCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void callBackRob(CCObject *pSender);

	void setData(guildboss::GuildBossUserBasic *pData);
	void setCountDown(float fCountDown);
protected:
	CC_SYNTHESIZE(GuildBossFightCellDelegate *, m_pDelegate, Delegate);
private:
	CCMenuItem *m_pBtnFight;
	CCLabelTTF *m_pTTFName;
	CCLabelTTF *m_pTTFGuild;
	CCLabelBMFont *m_pBMFFight;
	CCLabelBMFont *m_pBMFStar;
	CCLabelBMFont *m_pBMFGold;
	CCLabelBMFont *m_pBMFCountDown;
	CCNode *m_pNodeAttack;
	CCNode *m_pNodeGold;

	CCMenuItem *m_pBtnBlood[GUILDBOSS_MAX_BLOOD];
	CCNode *m_pNodeHealthy;
	CCNode *m_pNodeWounded;

	guildboss::GuildBossUserBasic *m_pData;
	GuildBossModel *m_pModel;
};

#endif