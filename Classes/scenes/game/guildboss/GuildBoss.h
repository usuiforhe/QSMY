//
//  GuildBoss.h
//  苍龙现视图
//
//  Created by Eci on 14-01-26.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBoss__
#define __QSMY__GuildBoss__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"


enum GuildBossNav
{
	GuildBossNavNoPop = -1,
	GuildBossNavHome = 0,
	GuildBossNavDragon = 1,
	GuildBossNavFight = 2,
	GuildBossNavDragonSettle = 3,
	GuildBossNavFightSettle = 4,
	GuildBossNavRule = 5,
	GuildBossNavSettle = 6,
	GuildBossNavTarget = 7,
	GuildBossNavRankGuild = 8,
	GuildBossNavRankHatred = 9,
	GuildBossNavRankContribution = 10,
	GuildBossNavRankRescue = 11,
	GuildBossNavRescue = 12,
	//GuildBossNavHealth = 13,
};


class GuildBossDragon;
class GuildBossView;
class GuildBossTarget;
class GuildBossRescueCell;
class GuildBossFightCell;
class GuildBossFight;
class GuildBossRescue;
//class GuildBossSettle;
class GuildBossView;

class GuildBossViewDelegate
{
public:
	virtual void GuildBossTargetSubmit(GuildBossTarget *pNode, guildboss::GuildBossBasic *pTarget) = 0;
	virtual void GuildBossBattleShow(GuildBossDragon *pNode) = 0;
	virtual void GuildBossAutoAttack(GuildBossDragon *pNode) = 0;
	virtual void GuildBossRescueUser(GuildBossRescueCell *pNode, uint32_t userID) = 0;
	virtual void GuildBossRescueAll(GuildBossRescue *pNode, uint32_t uNeedGold) = 0;
	virtual void GuildBossFightUser(GuildBossFightCell *pNode, guildboss::GuildBossUserBasic *pTarget) = 0;
	virtual void GuildBossShowHealthView(guildboss::GuildBossNotice *pNotice) = 0;

	virtual void GuildBossNavigateTo(GuildBossNav nav) = 0;
protected:
private:
};

class GuildBoss 
	:public DLayer
	,public BackKeyInterface
	,public GuildBossViewDelegate
{
public:
	GuildBoss();
	~GuildBoss();

	static GuildBoss* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual void GuildBossTargetSubmit(GuildBossTarget *pNode, guildboss::GuildBossBasic *pTarget);
	virtual void GuildBossBattleShow(GuildBossDragon *pNode);
	virtual void GuildBossAutoAttack(GuildBossDragon *pNode);
	virtual void GuildBossRescueUser(GuildBossRescueCell *pNode, uint32_t userID);
	virtual void GuildBossRescueAll(GuildBossRescue *pNode, uint32_t uNeedGold);
	virtual void GuildBossFightUser(GuildBossFightCell *pNode, guildboss::GuildBossUserBasic *pTarget);
	virtual void GuildBossShowHealthView(guildboss::GuildBossNotice *pNotice);

	virtual void GuildBossNavigateTo(GuildBossNav nav);

	void GuildBossGetInformation(const Json::Value &,const Json::Value &responseData);
	void GuildBossTargetSubmit(const Json::Value &,const Json::Value &responseData);
	void GuildBossFightUser(const Json::Value &,const Json::Value &responseData);

private:
	GuildBossModel *m_pModel;
	cocos2d::CCNode	*m_pFocusLayer;
	cocos2d::CCNode	*m_pPopupLayer;

	void showNoGuildError();

	bool checkPoints(uint32_t needPoints);

	void callBackMessageBox(MessageType eType,DMessageBox* pBox);

	void updateGuild();

	void afterBattle();
protected:
	GuildBossNav m_eShowingNav;
	GuildBossNav m_ePopingNav;
public:
	virtual void onBackKey();
};

#endif