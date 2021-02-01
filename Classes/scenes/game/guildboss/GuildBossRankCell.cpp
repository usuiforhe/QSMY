//
//  GuildBossRankCell.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossRankCell.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossRankCellContribution.h"
#include "GuildBossRankCellGuild.h"
#include "GuildBossRankCellHatred.h"
#include "GuildBossRankCellRescue.h"

using namespace sp;

GuildBossRankCell::GuildBossRankCell()
{
}

GuildBossRankCell::~GuildBossRankCell()
{
}

GuildBossRankCell* GuildBossRankCell::create(guildboss::GuildBossRankType eType, cocos2d::extension::CCData *pCCBData)
{
	if(!pCCBData) return NULL;
	GuildBossRankCell * pCell = NULL;
	switch (eType) {
	case guildboss::GuildBossRankTypeContribution:
		pCell = new GuildBossRankCellContribution();
		break;
	case guildboss::GuildBossRankTypeGuild:
		pCell = new GuildBossRankCellGuild();
		break;
	case guildboss::GuildBossRankTypeHatred:
		pCell = new GuildBossRankCellHatred();
		break;
	case guildboss::GuildBossRankTypeRescue:
		pCell = new GuildBossRankCellRescue();
		break;
	default:
		break;
	}
	if( pCell && pCell->init(pCCBData))
	{
		pCell->autorelease();
		return pCell;
	}
	CC_SAFE_DELETE(pCell);
	return NULL;
}

bool GuildBossRankCell::init(cocos2d::extension::CCData *pCCBData)
{
	if(!CCTableViewCell::init())
	{
		return false;
	}
	return true;
}