//
//  GuildBossTargetCell.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossTargetCell.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBoss.h"

using namespace sp;

GuildBossTargetCell::GuildBossTargetCell()
	: m_pDBtnCheck(NULL)
	, m_pMenuWar(NULL)
	, m_pTTFName(NULL)
	, m_pBMFFight(NULL)
	, m_pBMFStar(NULL)
	, m_pBMFRank(NULL)
	, m_pNodeSelf(NULL)
	, m_pNodeTarget(NULL)
	, m_pNodeLastWeek(NULL)
	, m_pNodeThisWeek(NULL)
	, m_pDelegate(NULL)
	, m_pData(NULL)
	, m_pModel(NULL)
{
}

GuildBossTargetCell::~GuildBossTargetCell()
{
	CC_SAFE_RELEASE(m_pDBtnCheck);
	CC_SAFE_RELEASE(m_pMenuWar);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pBMFFight);
	CC_SAFE_RELEASE(m_pBMFStar);
	CC_SAFE_RELEASE(m_pBMFRank);
	CC_SAFE_RELEASE(m_pNodeSelf);
	CC_SAFE_RELEASE(m_pNodeTarget);
	CC_SAFE_RELEASE(m_pNodeLastWeek);
	CC_SAFE_RELEASE(m_pNodeThisWeek);
}

GuildBossTargetCell* GuildBossTargetCell::create()
{
	GuildBossTargetCell *pRet = new GuildBossTargetCell();
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

bool GuildBossTargetCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_xz_cell");

	//m_pDBtnCheck->getButton()->setTarget(this, menu_selector(GuildBossTargetCell::callBackCheck));
	m_pMenuWar->setTarget(this, menu_selector(GuildBossTargetCell::callBackCheck));

	m_pModel = DM_GET_GUILDBOSS_MODEL;
	return true;
}

void GuildBossTargetCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildBossTargetCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildBossTargetCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnCheck", DButton *, m_pDBtnCheck);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuWar", CCMenuItemImage *, m_pMenuWar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFFight", CCLabelBMFont *, m_pBMFFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFStar", CCLabelBMFont *, m_pBMFStar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRank", CCLabelBMFont *, m_pBMFRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeSelf", CCNode *, m_pNodeSelf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeTarget", CCNode *, m_pNodeTarget);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeLastWeek", CCNode *, m_pNodeLastWeek);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeThisWeek", CCNode *, m_pNodeThisWeek);
	return false;
}

void GuildBossTargetCell::callBackCheck(CCObject *pSender)
{
	//m_pDBtnCheck->getButton()->selected();
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossTargetSelect(this);
		m_pDelegate->callBackSubmit(NULL);
	}
}

void GuildBossTargetCell::setSelected(bool selected)
{
	//if (selected)
	//{
	//	m_pDBtnCheck->getButton()->selected();
	//}
	//else
	//{
	//	m_pDBtnCheck->getButton()->unselected();
	//}
}

void GuildBossTargetCell::setData(guildboss::GuildBossBasic *pData)
{
	if (NULL ==	pData)
	{
		return;
	}
	m_pData = pData;
	m_pTTFName->setString(pData->getGuildName().c_str());
	m_pBMFFight->setString(stringForUIntWith10K(pData->getGuildFight()));
	m_pBMFStar->setString(stringForUIntWith10K(pData->getDamage()));
	//m_pDBtnCheck->setVisible(m_pModel->getBoss()->getGuildID() != m_pData->getGuildID());
	//m_pNodeSelf->setVisible(!m_pDBtnCheck->isVisible());
	//m_pNodeTarget->setVisible(m_pDBtnCheck->isVisible());
	m_pNodeTarget->setVisible(true);
}

void GuildBossTargetCell::setRank(uint32_t iRank)
{
	m_pBMFRank->setString(D_CSTRING_FROM_UINT(iRank));
}

void GuildBossTargetCell::setLastWeek(bool bLastWeek)
{
	m_pNodeLastWeek->setVisible(bLastWeek);
	m_pNodeThisWeek->setVisible(!bLastWeek);
}