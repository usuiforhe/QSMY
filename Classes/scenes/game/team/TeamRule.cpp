//
//  TeamRule.h
//  组队规则
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "TeamRule.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "Team.h"
using namespace sp;

TeamRule::TeamRule()
	: m_pDelegate(NULL)
{
}

TeamRule::~TeamRule()
{
}

TeamRule* TeamRule::create()
{
	TeamRule *pRet = new TeamRule();
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

bool TeamRule::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}
	CCB_READER_AND_ADDCHILD("ui/ccb/team/teamRule");
	return true;
}

void TeamRule::onEnter()
{
	CCTableViewCell::onEnter();
}

void TeamRule::onExit()
{
	CCTableViewCell::onExit();
}

bool TeamRule::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	return false;
}

SEL_MenuHandler TeamRule::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", TeamRule::callBackClose);
	return NULL;
}

void TeamRule::callBackClose(CCObject *pObj)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamNavigateTo(kTeamNavNoPop);
	}
}