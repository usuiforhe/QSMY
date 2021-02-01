//
//  Team.h
//  组队
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#include "Team.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "TeamChapter.h"
#include "../user/UserInfo.h"
#include "TeamFriends.h"
#include "TeamMake.h"
#include "TeamFormation.h"
#include "TeamTreasure.h"
#include "TeamRule.h"

Team::Team()
	: m_pFocusLayer(NULL)
	, m_pPopupLayer(NULL)
	, m_pTeamView(NULL)
	, m_eShowingNav(kTeamNavHome)
	, m_ePopingNav(kTeamNavNoPop)
	, m_pModel(NULL)
{
}

Team::~Team()
{
	CC_SAFE_RELEASE(m_pPopupLayer);
	m_pModel->setIsInTeam(false);
}

Team* Team::create()
{
	Team *pRet = new Team();
	if (pRet && pRet->init())
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		CC_SAFE_DELETE(pRet); 
		return NULL; 
	} 
}

bool Team::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView(),1);

	m_pFocusLayer = CCNode::create();
	addChild(m_pFocusLayer, 1);
	m_pPopupLayer = DPopup::create();
	//addChild(m_pPopupLayer, 1);
	CC_SAFE_RETAIN(m_pPopupLayer);

 	m_pTeamView = TeamChapter::create();
 	m_pTeamView->setDelegate(this);
 	addChild(m_pTeamView);

	m_pModel = DM_GET_FRIENDS_MODEL;

	m_pModel->setIsInTeam(true);

	return true;
}

void Team::onEnter()
{
	BINDING_EXEC(m_pModel, this, Team::updateView, D_EVENT_FRIENDS_DUNGEON);
	DLayer::onEnter();
}

void Team::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	DLayer::onExit();
	TeamNavigateTo(kTeamNavHome);
}

void Team::updateView()
{
	if (m_pModel->getDungeon()->getOngoingProgress() > 0)
	{
		TeamNavigateTo(kTeamNavFormation);
	}
	else
	{
		//TeamNavigateTo(kTeamNavHome);
	}
}

void Team::onBackKey()
{
	if (m_eShowingNav == kTeamNavFriends)
	{
		TeamNavigateTo(kTeamNavMake);
		return;
	}
	if (m_ePopingNav != kTeamNavNoPop)
	{
		TeamNavigateTo(kTeamNavNoPop);
		return;
	}
	if (m_eShowingNav != kTeamNavHome)
	{
		TeamNavigateTo(kTeamNavHome);
		return;
	}
	BackKeyInterface::onBackKey();
}

void Team::TeamNavigateTo(TeamNav nav)
{
	if (m_ePopingNav == nav || (m_ePopingNav == kTeamNavNoPop && m_eShowingNav == nav))
	{
		return;
	}

	switch(nav)
	{
	case kTeamNavHome:
	case kTeamNavMake:
	case kTeamNavFriends:
		m_pFocusLayer->removeAllChildren();
		m_pPopupLayer->removeAllChildren();
		m_pPopupLayer->closePopup();
		m_ePopingNav = kTeamNavNoPop;
		m_eShowingNav = nav;
		break;
	case kTeamNavFormation:
	case kTeamNavTreasure:
	case kTeamNavRule:
	case kTeamNavNoPop:
		m_ePopingNav = nav;
		m_pPopupLayer->removeAllChildren();
		m_pPopupLayer->closePopup();
		break;
	}
	switch(nav)
	{
	case kTeamNavMake:
		{
			TeamMake *pView = TeamMake::create();
			pView->setDelegate(this);
			m_pFocusLayer->addChild(pView);
			break;
		}
	case kTeamNavFriends:
		{
			TeamFriends *pView = TeamFriends::create();
			pView->setDelegate(this);
			m_pFocusLayer->addChild(pView);
			break;
		}
	case kTeamNavFormation:
		{
			TeamFormation *pView = TeamFormation::create();
			pView->setDelegate(this);
			m_pPopupLayer->addChild(pView);
			Game::sharedGame()->openPopup(m_pPopupLayer);
			break;
		}
	case kTeamNavTreasure:
		{
			TeamTreasure *pView = TeamTreasure::create();
			pView->setDelegate(this);
			m_pPopupLayer->addChild(pView);
			Game::sharedGame()->openPopup(m_pPopupLayer);
			break;
		}
	case kTeamNavRule:
		{
			TeamRule *pView = TeamRule::create();
			pView->setDelegate(this);
			m_pPopupLayer->addChild(pView);
			Game::sharedGame()->openPopup(m_pPopupLayer);
			break;
		}
	case kTeamNavHome:
	case kTeamNavNoPop:
	default:
		break;
	}
}