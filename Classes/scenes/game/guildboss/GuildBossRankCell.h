//
//  GuildBossRankCell.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossRankCell__
#define __QSMY__GuildBossRankCell__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

namespace guildboss
{
	enum GuildBossRankType
	{
		GuildBossRankTypeContribution = 0,
		GuildBossRankTypeHatred = 1,
		GuildBossRankTypeGuild = 2,
		GuildBossRankTypeRescue = 3,
	};
}

class GuildBossRankCell;

class GuildBossRankCell 
	:public cocos2d::extension::CCTableViewCell
{

public:
	GuildBossRankCell();
	~GuildBossRankCell();

	static GuildBossRankCell* create(
		guildboss::GuildBossRankType eType, 
		cocos2d::extension::CCData *pCCBData);
	virtual bool init(cocos2d::extension::CCData *pCCBData);
	virtual void setData(CCObject *pData, uint32_t iRank, bool bChangeVisible, bool bLastWeek) = 0;
};

#endif