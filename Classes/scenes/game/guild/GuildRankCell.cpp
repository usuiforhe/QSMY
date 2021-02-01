//
//  GuildRankCell.h
//  公会排名
//
//  Created by Eci on 13-10-24.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildRankCell.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"


GuildRankCell::GuildRankCell()
	:m_pLabelRank(NULL)
	,m_pLabelCount(NULL)
	,m_pLabelLevel(NULL)
	,m_pLabelName(NULL)
	,m_pLabelContribution(NULL)
	,m_pNodeGuild(NULL)
	,m_pNodeUser(NULL)
	,m_pData(NULL)
    ,m_iRank(0)
{
}

GuildRankCell::~GuildRankCell()
{
	CC_SAFE_RELEASE(m_pLabelRank);
	CC_SAFE_RELEASE(m_pLabelCount);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pLabelContribution);
	CC_SAFE_RELEASE(m_pNodeGuild);
	CC_SAFE_RELEASE(m_pNodeUser);
	CC_SAFE_RELEASE(m_pData);
}

GuildRankCell* GuildRankCell::create(int val)
{
	GuildRankCell *pRet = new GuildRankCell();
	if (pRet && pRet->init(val))
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

bool GuildRankCell::init(int val)
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	//m_pModel = DM_GET_GUILD_MODEL;
	char * path1 = "PL_ui/ccb/pl_banghui/pl_banghui_paihang_bh";
	char * path2 = "PL_ui/ccb/pl_banghui/pl_banghui_paihang_gx";
	if(val == 0)
	{
		CCB_READER_AND_ADDCHILD(path1);
	}
	else
	{
		CCB_READER_AND_ADDCHILD(path2);
	}
	return true;
}

void GuildRankCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildRankCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildRankCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_rank_cell_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_rank_cell_rank", CCLabelBMFont *, this->m_pLabelRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_count", CCLabelBMFont *, this->m_pLabelCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_contribution", CCLabelBMFont *, this->m_pLabelContribution);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_user", CCNode *, this->m_pNodeUser);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_rank", CCNode *, this->m_pNodeGuild);
	return false;
}

void GuildRankCell::setData(ValueObjcet *pData)
{
	if (m_pData == pData)
	{
		return;
	}
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
	guild::GuildBasics *guildData = dynamic_cast<guild::GuildBasics *>(pData);
	if (NULL != guildData)
	{
		m_pLabelName->setString(guildData->getName().c_str());
		m_pLabelLevel->setString(CCString::createWithFormat("%d", guildData->getLevel())->getCString());
		m_pLabelCount->setString(CCString::createWithFormat("%d", guildData->getUserCount())->getCString());
		//m_pNodeUser->setVisible(false);
		//m_pNodeGuild->setVisible(true);
	}
	else
	{
		guild::GuildUserBasics *userData = dynamic_cast<guild::GuildUserBasics *>(pData);
		m_pLabelName->setString(userData->getName().c_str());
		m_pLabelContribution->setString(CCString::createWithFormat("%d", userData->getContribution())->getCString());
		//m_pNodeUser->setVisible(true);
		//m_pNodeGuild->setVisible(false);
	}
}

void GuildRankCell::setRank(uint32_t rank)
{
	if (rank == m_iRank)
	{
		return;
	}
	m_iRank = rank;
	m_pLabelRank->setString(CCString::createWithFormat("%d", m_iRank)->getCString());
}
