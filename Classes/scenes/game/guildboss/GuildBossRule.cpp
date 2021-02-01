//
//  GuildBossRule.h
//  公会主页
//
//  Created by Eci on 14-02-21.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossRule.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBoss.h"

using namespace sp;

GuildBossRule::GuildBossRule()
	: m_pDelegate(NULL)
	, m_pCloseButton(NULL)
{
	D_RESET_C_ARRAY(m_pNode, RULE_NODE_MAX);
	D_RESET_C_ARRAY(m_pBtnSwitch, RULE_NODE_MAX);
	D_RESET_C_ARRAY(m_pPage, RULE_NODE_MAX);
}

GuildBossRule::~GuildBossRule()
{
	CC_SAFE_RELEASE(m_pCloseButton);
	D_SAFE_RELEASE_C_ARRAY(m_pNode, RULE_NODE_MAX);
	D_SAFE_RELEASE_C_ARRAY(m_pBtnSwitch, RULE_NODE_MAX);
	D_SAFE_RELEASE_C_ARRAY(m_pPage, RULE_NODE_MAX);
}

GuildBossRule* GuildBossRule::create()
{
	GuildBossRule *pRet = new GuildBossRule();
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

bool GuildBossRule::init()
{
	if(!DPopup::init())
	{
		return false;
	}
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_jianghufenzheng_shuoming");
	for (int i = 0; i < RULE_NODE_MAX; i++)
	{
		m_pBtnSwitch[i]->getButton()->setTarget(this, menu_selector(GuildBossRule::callBackSwitch));
	}
	callBackSwitch(m_pBtnSwitch[0]->getButton());

	m_pCloseButton->getButton()->setTarget(this , menu_selector(GuildBossRule::closeCallBack));

	return true;
}

void GuildBossRule::onEnter()
{
	for (int i = 0; i < RULE_NODE_MAX; i++)
	{
		m_pPage[i]->setContentOffset(m_pPage[i]->minContainerOffset());
	}
	
	DPopup::onEnter();
}

void GuildBossRule::onExit()
{
	DPopup::onExit();
}

bool GuildBossRule::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);
	D_CCB_ASSIGN_ARRAY("m_pNode", CCNode *, m_pNode, RULE_NODE_MAX);
	D_CCB_ASSIGN_ARRAY("m_pBtnSwitch", DButton *, m_pBtnSwitch, RULE_NODE_MAX);
	D_CCB_ASSIGN_ARRAY("m_pPage", CCScrollView *, m_pPage, RULE_NODE_MAX);
	return false;
}


SEL_MenuHandler GuildBossRule::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", GuildBossRule::closeCallBack);
	return NULL;
}

void GuildBossRule::closeCallBack(cocos2d::CCObject * obj)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavHome);
	}
	closePopup();
}

void GuildBossRule::callBackSwitch(cocos2d::CCObject *pSender)
{
	CCMenuItem *pItem = (CCMenuItem *)pSender;
	if (pItem->isSelected())
	{
		return;
	}
	for (int i = 0; i < RULE_NODE_MAX; i++)
	{
		if (pItem == m_pBtnSwitch[i]->getButton())
		{
			m_pBtnSwitch[i]->getButton()->selected();
		}
		else
		{
			m_pBtnSwitch[i]->getButton()->unselected();
		}
		m_pNode[i]->setVisible(m_pBtnSwitch[i]->getButton()->isSelected());
	}
}