//
//  GuildBossHealth.h
//  公会被救援
//
//  Created by Eci on 14-03-03.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossHealth.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBoss.h"

using namespace sp;

GuildBossHealth::GuildBossHealth()
	:m_pTTFName(NULL)
	,m_pDelegate(NULL)
{
}

GuildBossHealth::~GuildBossHealth()
{
	CC_SAFE_RELEASE(m_pTTFName);
}

GuildBossHealth* GuildBossHealth::create()
{
	GuildBossHealth *pRet = new GuildBossHealth();
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

bool GuildBossHealth::init()
{
	if(!DLayer::init())
	{
		return false;
	}
	CCB_READER_AND_ADDCHILD("ui/ccb/guildBoss/guildBossHealth");
	return true;
}

void GuildBossHealth::onEnter()
{
	DLayer::onEnter();
}

void GuildBossHealth::onExit()
{
	DLayer::onExit();
}

bool GuildBossHealth::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF*, this->m_pTTFName);
	return false;
}


SEL_MenuHandler GuildBossHealth::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackClose", GuildBossHealth::callBackClose);
	return NULL;
}

void GuildBossHealth::callBackClose(cocos2d::CCObject * obj)
{
	this->removeFromParent();
}

void GuildBossHealth::setNotice(guildboss::GuildBossNotice *m_pNotice)
{
	m_pTTFName->setString(m_pNotice->getTargetName().c_str());
}