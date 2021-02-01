//
//  GuildBoss.h
//  苍龙现视图
//
//  Created by Eci on 14-01-26.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#include "GuildBoss.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "GuildBossView.h"
#include "GuildBossDragon.h"
#include "GuildBossTarget.h"
#include "GuildBossRank.h"
#include "GuildBossRankMember.h"
#include "GuildBossRescue.h"
#include "GuildBossSettle.h"
#include "GuildBossFight.h"
#include "GuildBossFightSettle.h"
#include "GuildBossRule.h"
#include "GuildBossHealth.h"

GuildBoss::GuildBoss()
	: m_pFocusLayer(NULL)
	, m_pPopupLayer(NULL)
	, m_eShowingNav(GuildBossNavHome)
	, m_ePopingNav(GuildBossNavHome)
{
}

GuildBoss::~GuildBoss()
{
	NotificationCenter::sharedNotificationCenter()->removeObserver(this, D_EVENT_END_BATTLE);
}

GuildBoss* GuildBoss::create()
{
	GuildBoss *pRet = new GuildBoss();
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

bool GuildBoss::init()
{
	if(!DLayer::init())
	{
		return false;
	}
	m_pFocusLayer = CCNode::create();
	addChild(m_pFocusLayer, 1);
	m_pPopupLayer = CCNode::create();
	addChild(m_pPopupLayer, 1);

	m_pModel = DM_GET_GUILDBOSS_MODEL;

	GuildBossView *pView = GuildBossView::create();
	pView->setDelegate(this);
	addChild(pView);


	NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(GuildBoss::afterBattle),D_EVENT_END_BATTLE);

	return true;
}

void GuildBoss::onEnter()
{
	BINDING_ONLY(m_pModel, this, GuildBoss::showNoGuildError, D_EVENT_GUILDBOSS_ERROR_NO_GUILD);
	BINDING_ONLY(DM_GET_GUILD_MODEL, this, GuildBoss::updateGuild, D_EVENT_GUILD_GET);

	DM_GET_GUILD_MODEL->syncFromServer();
	DLayer::onEnter();
}

void GuildBoss::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
	UNBINDING_ALL(DM_GET_GUILD_MODEL, this);
	GuildBossNavigateTo(GuildBossNavHome);
}

void GuildBoss::onBackKey()
{
	if (m_pPopupLayer && m_pPopupLayer->getChildrenCount())
	{
		m_pPopupLayer->removeAllChildren();
		return;
	}
	if (m_pFocusLayer && m_pFocusLayer->getChildrenCount())
	{
		m_pFocusLayer->removeAllChildren();
		return;
	}
	BackKeyInterface::onBackKey();
}

void GuildBoss::GuildBossTargetSubmit(GuildBossTarget *pNode, guildboss::GuildBossBasic *pTarget)
{
	if(m_pModel->getBoss()->getGuildID() == pTarget->getGuildID())
	{
		//打自己的帮会
		Tips_Alert(D_LOCAL_STRING("CannotDeclare").c_str());
	}
	else
	{
		HTTP_CLIENT->guildBossDeclareWar(pTarget->getGuildID(), this, callfuncJson_selector(GuildBoss::GuildBossTargetSubmit));
	}
}

void GuildBoss::GuildBossBattleShow(GuildBossDragon *pNode)
{
	if (!m_pModel->getBossSelf()->getAvailTimes() && !m_pModel->getBossSelf()->getPayedTimes())
	{
		Tips_Alert(D_LOCAL_STRING("GuildBossNotEnoughTimes"));
		return;
	}
	int times = 1;
	if (m_pModel->getBossSelf()->getAvailTimes() > 0)
	{
		times = 0;
	}
	
	int needPoints = times * m_pModel->getConfig()->getAttackCost();
	if (!checkPoints(needPoints))
	{
		return;
	}
	HTTP_CLIENT->guildBossAttackBoss(!m_pModel->getBossSelf()->getAvailTimes());
}

void GuildBoss::GuildBossAutoAttack(GuildBossDragon *pNode)
{
	HTTP_CLIENT->guildBossAutoAttackBoss();
}

void GuildBoss::GuildBossRescueUser(GuildBossRescueCell *pNode, uint32_t userID)
{
	if (!checkPoints(m_pModel->getConfig()->getRescueCost()))
	{
		return;
	}
	CCLOG("%u", userID);
	HTTP_CLIENT->guildBossRescue(userID, false);
}

void GuildBoss::GuildBossRescueAll(GuildBossRescue *pNode, uint32_t uNeedGold)
{
	if (!uNeedGold)
	{
		return;
	}
	if (!checkPoints(uNeedGold))
	{
		return;
	}
	HTTP_CLIENT->guildBossRescue(0, true);
}

void GuildBoss::GuildBossFightUser(GuildBossFightCell *pNode, guildboss::GuildBossUserBasic *pTarget)
{
	int needPoints = 0;
	if (m_pModel->getBossSelf()->getAvailTimes() == 0)
	{
		needPoints = m_pModel->getConfig()->getFightCost();
	}

	if (!checkPoints(needPoints))
	{
		return;
	}

	if (!m_pModel->getBossSelf()->getAvailTimes() && !m_pModel->getBossSelf()->getPayedTimes())
	{
		Tips_Alert(D_LOCAL_STRING("GuildRobNotEnoughTimes"));
		return;
	}
	HTTP_CLIENT->guildBossRob(!m_pModel->getBossSelf()->getAvailTimes(), pTarget->getUserID(), pTarget->getDamageForRob(), this, callfuncJson_selector(GuildBoss::GuildBossFightUser));
}

void GuildBoss::GuildBossShowHealthView(guildboss::GuildBossNotice *pNotice)
{
	/************************************************************************/
	/* TODO                                                                     */
	/************************************************************************/
	//GuildBossHealth *pView = GuildBossHealth::create();
	//pView->setNotice(pNotice);
	//pView->setDelegate(this);
	//m_pPopupLayer->addChild(pView);
}

void GuildBoss::GuildBossGetInformation(const Json::Value &,const Json::Value &responseData)
{
	if (responseData["code"].asString().compare(GUILDBOSS_NEW_JOIN)==0)
	{
		DMessageBox::showMessageBox(D_LOCAL_STRING("GuildBossNewJoin").c_str(), this, messageBox_callfunc_selector(GuildBoss::callBackMessageBox));
		return;
	}
	m_pModel->setIsNewJoin(false);
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	DM_GET_GUILD_MODEL->syncFromServer();
}

void GuildBoss::GuildBossTargetSubmit(const Json::Value &,const Json::Value &responseData)
{
	if (responseData["code"].asString().compare(GUILDBOSS_DECLARED)==0
		|| responseData["code"].asString().compare(GUILDBOSS_DECLARE_WRONG_TIME)==0)
	{
		HTTP_CLIENT->guildBossGet();
		m_pFocusLayer->removeAllChildren();
		return;
	}
	if (responseData["code"].asString().compare(GUILD_PERMISSION_DENIED) == 0)
	{
		m_pFocusLayer->removeAllChildren();
		return;
	}
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	//哈哈哈宣战成功啦！
	Tips_Alert(D_LOCAL_STRING("$GuildXuanzhan"));
	m_pFocusLayer->removeAllChildren();
}

void GuildBoss::GuildBossFightUser(const Json::Value &,const Json::Value &responseData)
{
	if (responseData["code"].asString().compare(GUILDBOSS_ROB_TARGET_OLD)==0)
	{
		HTTP_CLIENT->guildBossGetTargetList(m_pModel->getBoss()->getGuildID());
		return;
	}
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
}

bool GuildBoss::checkPoints(uint32_t needPoints)
{
	UserModel *pUserModel = DM_GET_USER_MODEL;
	if (pUserModel->getUserInfo()->getPoints() < needPoints)
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
		return false;
	}
	return true;
}
void GuildBoss::showNoGuildError()
{
	m_pFocusLayer->removeAllChildren();
	m_pPopupLayer->removeAllChildren();
	
	DMessageBox::showMessageBox(D_LOCAL_STRING("GuildErrorNoGuild").c_str(), this, messageBox_callfunc_selector(GuildBoss::callBackMessageBox));
}

void GuildBoss::callBackMessageBox(MessageType eType,DMessageBox* pBox)
{
	if (eType == kMessageTypeOk)
	{
		//Game::sharedGame()->NavigateTo(navigation::kNavGuild);
		//留在原处
		//暂不处理
	}
	else
	{
		Game::sharedGame()->NavigateTo(navigation::kNavHome);
	}
}

void GuildBoss::updateGuild()
{
	uint32_t guildID = DM_GET_GUILD_MODEL->getSelfInfo()->getGuildID();
	if (!guildID)
	{
		showNoGuildError();
		return;
	}
	bool isCreatedToday = DM_GET_GUILD_MODEL->getGuildInfo()->getIsCreatedToday();

	//新创建的公会不能参加，不用弹筐显示
	//if (isCreatedToday)
	//{
	//	DMessageBox::showMessageBox(D_LOCAL_STRING("GuildErrorNewGuild").c_str(), this, messageBox_callfunc_selector(GuildBoss::callBackMessageBox));
	//	return;
	//}

	if (m_pModel->getIsNewJoin())
	{
		DMessageBox::showMessageBox(D_LOCAL_STRING("GuildBossNewJoin").c_str(), this, messageBox_callfunc_selector(GuildBoss::callBackMessageBox));
		return;
	}

	if (!m_pModel->getJoined())
	{
		HTTP_CLIENT->guildBossStart(this, callfuncJson_selector(GuildBoss::GuildBossGetInformation));
	}
	else
	{
		m_pModel->syncSelfInfo();
	}
}

void GuildBoss::afterBattle()
{
	if (guildboss::GuildBossStateRob == m_pModel->getStateOfLivingPoint())
	{
		GuildBossNavigateTo(GuildBossNavFight);
	}
	else
	{
		GuildBossNavigateTo(GuildBossNavHome);
	}
}

void GuildBoss::GuildBossNavigateTo(GuildBossNav nav)
{
	if (m_ePopingNav == nav || (m_ePopingNav == GuildBossNavNoPop && m_eShowingNav == nav))
	{
		return;
	}
	
	switch(nav)
	{
	case GuildBossNavHome:
	case GuildBossNavDragon:
	case GuildBossNavFight:
	case GuildBossNavDragonSettle:
	case GuildBossNavFightSettle:
	case GuildBossNavRule:
	case GuildBossNavSettle:
	case GuildBossNavTarget:
		m_pFocusLayer->removeAllChildren();
		m_pPopupLayer->removeAllChildren();
		m_ePopingNav = GuildBossNavNoPop;
		m_eShowingNav = nav;
		break;
	case GuildBossNavRankGuild:
	case GuildBossNavRankHatred:
	case GuildBossNavRankContribution:
	case GuildBossNavRankRescue:
	case GuildBossNavRescue:
	case GuildBossNavNoPop:
		m_ePopingNav = nav;
		m_pPopupLayer->removeAllChildren();
		break;
	}
	switch(nav)
	{
	case GuildBossNavDragon:
		{
			GuildBossDragon *pView = GuildBossDragon::create();
			pView->setDelegate(this);
			m_pFocusLayer->addChild(pView);
			break;
		}
	case GuildBossNavFight:
		{
			GuildBossFight *pView = GuildBossFight::create();
			pView->setDelegate(this);
			m_pFocusLayer->addChild(pView);
			break;
		}
	case GuildBossNavDragonSettle:
		{
			GuildBossFightSettle *pView = GuildBossFightSettle::create();
			pView->setState(guildboss::GuildBossStateBoss);
			pView->setDelegate(this);
			//m_pFocusLayer->addChild(pView);
			Game::sharedGame()->openPopup(pView);
			break;
		}
	case GuildBossNavFightSettle:
		{
			GuildBossFightSettle *pView = GuildBossFightSettle::create();
			pView->setState(guildboss::GuildBossStateRob);
			pView->setDelegate(this);
			//m_pFocusLayer->addChild(pView);
			Game::sharedGame()->openPopup(pView);
			break;
		}
	case GuildBossNavRule:
		{
			GuildBossRule *pView = GuildBossRule::create();
			pView->setDelegate(this);
			//m_pFocusLayer->addChild(pView);
			Game::sharedGame()->openPopup(pView);
			break;
		}
	case GuildBossNavSettle:
		{
			GuildBossSettle *pView = GuildBossSettle::create();
			pView->setDelegate(this);
			//m_pFocusLayer->addChild(pView);
			Game::sharedGame()->openPopup(pView);
			break;
		}
	case GuildBossNavTarget:
		{
			GuildBossTarget *pView = GuildBossTarget::create();
			pView->setDelegate(this);
			//m_pFocusLayer->addChild(pView);
			Game::sharedGame()->openPopup(pView);
			break;
		}
	case GuildBossNavRankGuild:
	case GuildBossNavRankHatred:
		{
			//帮会排行和帮会仇恨列表
			//若未得到数据，不做请求
			if(m_pModel->getBoss()->getGuildID())
			{
				GuildBossRank *pView = GuildBossRank::create();
				pView->setDelegate(this);
				pView->setType(guildboss::GuildBossRankTableTypeGuild);
				//m_pPopupLayer->addChild(pView);
				Game::sharedGame()->openPopup(pView);
			}
			else
			{
				Tips_Alert(D_LOCAL_STRING("GuildNotStart").c_str());
			}
			break;
		}
	case GuildBossNavRankContribution:
	case GuildBossNavRankRescue:
		{
			//帮会排行和帮会仇恨列表
			//若未得到数据，不做请求
			if(m_pModel->getBoss()->getGuildID())
			{
				GuildBossRankMember *pView = GuildBossRankMember::create();
				pView->setDelegate(this);
				pView->setType(guildboss::GuildBossRankMemberTableTypeContribution);
				//m_pPopupLayer->addChild(pView);
				Game::sharedGame()->openPopup(pView);
			}
			else
			{
				Tips_Alert(D_LOCAL_STRING("GuildNotStart").c_str());
			}
			break;
		}
	case GuildBossNavRescue:
		{
			GuildBossRescue *pView = GuildBossRescue::create();
			pView->setDelegate(this);
			//m_pPopupLayer->addChild(pView);
			Game::sharedGame()->openPopup(pView);
			break;
		}
	case GuildBossNavHome:
	case GuildBossNavNoPop:
	default:
		break;
	}
}