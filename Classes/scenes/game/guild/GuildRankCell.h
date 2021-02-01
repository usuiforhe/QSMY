//
//  GuildRankCell.h
//  公会排名
//
//  Created by Eci on 13-10-24.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildRankCell__
#define __QSMY__GuildRankCell__


#define  RANK_CELL_SIZE        CCSizeMake(640, 110)

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../../../managers/GameManager.h"

class GuildRankCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	GuildRankCell();
	~GuildRankCell();

	static GuildRankCell* create(int val);
	virtual bool init(int val);

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

#pragma mark setter
	void setData(ValueObjcet *pData);
	void setRank(uint32_t rank);
private:
protected:
	cocos2d::CCLabelBMFont *m_pLabelRank;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelBMFont *m_pLabelCount;
	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCLabelBMFont *m_pLabelContribution;
	cocos2d::CCNode *m_pNodeGuild;
	cocos2d::CCNode *m_pNodeUser;
	CC_SYNTHESIZE_READONLY(uint32_t, m_iRank, Rank);
	CC_SYNTHESIZE_READONLY(ValueObjcet *, m_pData, Data);
};


#endif