//
//  GuildBossRankCellGuild.h
//  公会主页
//
//  Created by Eci on 14-03-22.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossRankCellGuild__
#define __QSMY__GuildBossRankCellGuild__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

#include "GuildBossRankCell.h"

class GuildBossRankCellGuild 
	:public GuildBossRankCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	GuildBossRankCellGuild();
	~GuildBossRankCellGuild();

	virtual bool init(cocos2d::extension::CCData *pCCBData);
	virtual void setData(CCObject *pData, uint32_t iRank, bool bChangeVisible, bool bLastWeek);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
private:
	cocos2d::CCNode *m_pRankNode;
	cocos2d::CCNode *m_pHateNode;
	cocos2d::CCLabelBMFont *m_pBMFFight;
	cocos2d::CCLabelBMFont *m_pBMFStar;
	cocos2d::CCLabelBMFont *m_pBMFRank;
	cocos2d::CCLabelTTF *m_pTTFName;
	cocos2d::CCNode *m_pNodeLastWeek;
	cocos2d::CCNode *m_pNodeThisWeek;
	cocos2d::CCNode *m_pNodeSelf;
	cocos2d::CCNode *m_pNodeOther;
	cocos2d::CCNode *m_pCCBChange;
	cocos2d::CCNode *m_pNodeChange[guildboss::RankChangeTypeMax];

	cocos2d::CCObject *m_pData;
	GuildBossModel *m_pModel;
};

#endif