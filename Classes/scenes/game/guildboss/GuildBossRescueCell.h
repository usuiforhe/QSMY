//
//  GuildBossRescueCell.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossRescueCell__
#define __QSMY__GuildBossRescueCell__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class GuildBossRescueCell;

class GuildBossRescueCellDelegate
{
public:
	virtual void GuildBossRescueUser(GuildBossRescueCell *pNode, uint32_t ) = 0;
};

class GuildBossRescueCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	GuildBossRescueCell();
	~GuildBossRescueCell();

	static GuildBossRescueCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void callBackRescue(CCObject *pSender);

	void setData(guildboss::GuildWoundedUser *pData);
protected:
	CC_SYNTHESIZE(GuildBossRescueCellDelegate *, m_pDelegate, Delegate);
private:
	CCMenuItem *m_pBtnRescue;
	
	CCLabelTTF *m_pTTFName;
	CCLabelBMFont *m_pBMFFight;
	CCLabelBMFont *m_pBMFGold;
	CCNode *m_pNodeHead;

	guildboss::GuildWoundedUser *m_pData;
	GuildBossModel *m_pModel;
};

#endif