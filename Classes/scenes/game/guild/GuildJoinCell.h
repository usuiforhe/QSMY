//
//  GuildJoinCell.h
//  加入公会
//
//  Created by Eci on 13-10-23.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildJoinCell__
#define __QSMY__GuildJoinCell__

#define  JOIN_CELL_SIZE        CCSizeMake(640, 100)
#define  JOIN_CELL_SIZE_EXPANDED CCSizeMake(640, 174)

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../../../managers/GameManager.h"

class GuildJoinCellDelegate
{
public:
	virtual void willApplyForTheGuild(uint32_t guildID, int action) = 0;
	virtual void willShowGuildDetails(uint32_t guildID, uint32_t rank) = 0;
};

class GuildJoinCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	GuildJoinCell();
	~GuildJoinCell();

	static GuildJoinCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

#pragma mark setter

	void setExpanded(bool isExpanded);

	void setData(guild::GuildBasics *pData);
	void setRank(uint32_t rank);
	void setApplied(bool isApplied);
private:
	cocos2d::CCNode *m_pContainer;
	cocos2d::CCNode *m_pNodeExpand;
	cocos2d::CCNode *m_pNodeApplied;
	cocos2d::CCNode *m_pNodeApply;
	cocos2d::CCNode *m_pNodeCancel;
	cocos2d::CCLabelBMFont *m_pLabelRank;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelBMFont *m_pLabelCount;
	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCMenuItem *m_pButtonDetails;
	DButton *m_pButtonApply;
	DButton *m_pButtonCancel;
#pragma mark callback function

	void callBackDetails(CCObject *sender);
	void callBackApply(CCObject *sender);
protected:
	CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pSelectButton, SelectButton);
	CC_SYNTHESIZE(GuildJoinCellDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(uint32_t, m_iRank, Rank);
	CC_SYNTHESIZE_READONLY(guild::GuildBasics *, m_pData, Data);
// 	uint32_t m_iRank;
// 	guild::GuildBasics *m_pData;
};


#endif