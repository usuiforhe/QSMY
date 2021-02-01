//
//  Team.h
//  组队
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__Team__
#define __QSMY__Team__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"



#define FORMATION_MAX	7
#define ITEM_MAX	3

enum TeamNav
{
	kTeamNavNoPop = -1,
	kTeamNavHome = 0,
	kTeamNavMake = 1,
	kTeamNavFriends = 2,
	kTeamNavFormation = 3,
	kTeamNavTreasure = 4,
	kTeamNavRule = 5,
};

class TeamViewDelegate
{
public:
	virtual void TeamNavigateTo(TeamNav nav) = 0;
protected:
private:
};

class TeamChapter;

class Team 
	:public DLayer
	,public BackKeyInterface
	,public TeamViewDelegate
{
public:
	Team();
	~Team();

	static Team* create();
	virtual bool init();

	void updateView();

	virtual void onEnter();
	virtual void onExit();

	virtual void TeamNavigateTo(TeamNav nav);

private:
	cocos2d::CCNode	*m_pFocusLayer;
	DPopup	*m_pPopupLayer;
	TeamChapter *m_pTeamView;

	FriendsModel *m_pModel;
protected:
	TeamNav m_eShowingNav;
	TeamNav m_ePopingNav;
public:
	virtual void onBackKey();
};

#endif