//
//  GuildBossSettleCell.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossSettleCell__
#define __QSMY__GuildBossSettleCell__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class GuildBossSettleCell;

class GuildBossSettleCellDelegate
{
public:
	virtual void GuildBossTargetSelect(GuildBossSettleCell *pNode) = 0;
};

class GuildBossSettleCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	GuildBossSettleCell();
	~GuildBossSettleCell();

	static GuildBossSettleCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	void setCCBData(cocos2d::extension::CCData *pCCBData);
	void setData(guildboss::GuildBossBasic *pData);

	void setRank(uint32_t iRank);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	/************************************************************************/
	/*	Rank                                                                  
		Name
		Star
		Fight	C
		Change	
		Head	R
	************************************************************************/
    
private:
	cocos2d::CCNode *m_pNodeChange[guildboss::RankChangeTypeMax];
	CCLabelBMFont *m_pBMFFight;
	CCLabelBMFont *m_pBMFStar;
	CCLabelBMFont *m_pBMFRank;
	CCLabelTTF *m_pTTFName;
	cocos2d::extension::CCData *m_pCCBData;

	guildboss::GuildBossBasic *m_pData;
};

#endif