//
//  GuildNoneView.h
//  无工会
//
//  Created by Eci on 13-10-23.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildNoneView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"


GuildNoneView::GuildNoneView()
	: m_pButtonJoin(NULL)
	, m_pButtonCreate(NULL)
	, m_pCloseButton(NULL)
	, m_pDelegate(NULL)
{
}

GuildNoneView::~GuildNoneView()
{
	CC_SAFE_RELEASE(m_pButtonJoin);
	CC_SAFE_RELEASE(m_pButtonCreate);
	CC_SAFE_RELEASE(m_pCloseButton);
}

GuildNoneView* GuildNoneView::create()
{
	GuildNoneView *pRet = new GuildNoneView();
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

bool GuildNoneView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	//m_pModel = DM_GET_GUILD_MODEL;

	//CCB_READER_AND_ADDCHILD("ui/ccb/guild/guildFirst");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_chushi");
	
	m_pButtonJoin->getButton()->setTarget(this, menu_selector(GuildNoneView::callBackJoin));
	m_pButtonCreate->getButton()->setTarget(this, menu_selector(GuildNoneView::callBackCreate));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(GuildNoneView::callBackClose));
	return true;
}

void GuildNoneView::onEnter()
{
	DLayer::onEnter();

//	BINDING_ONLY(m_pModel, this, GuildView::updateNoticeView, D_EVENT_GUILD_GET_NOTICE);
}

void GuildNoneView::onExit()
{
	DLayer::onExit();
//	UNBINDING_ALL(m_pModel, this);
}

bool GuildNoneView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_join1", DButton *, this->m_pButtonJoin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_create1", DButton *, this->m_pButtonCreate);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	return false;
}

void GuildNoneView::callBackJoin(CCObject *sender)
{
	m_pDelegate->willJoinGuild();
}

void GuildNoneView::callBackCreate(CCObject *sender)
{
	m_pDelegate->willCreateGuild();
}

void GuildNoneView::callBackClose(CCObject *sender)
{
	m_pDelegate->willClose();
}

