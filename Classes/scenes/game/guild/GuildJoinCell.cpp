//
//  GuildJoinCell.h
//  加入公会
//
//  Created by Eci on 13-10-23.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildJoinCell.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"


GuildJoinCell::GuildJoinCell()
	:m_pContainer(NULL)
	,m_pNodeExpand(NULL)
	,m_pNodeApplied(NULL)
	,m_pNodeApply(NULL)
	,m_pNodeCancel(NULL)
	,m_pLabelRank(NULL)
	,m_pLabelCount(NULL)
	,m_pLabelLevel(NULL)
	,m_pLabelName(NULL)
	,m_pSelectButton(NULL)
	,m_pDelegate(NULL)
	,m_pData(NULL)
	,m_pButtonApply(NULL)
	,m_pButtonCancel(NULL)
	,m_pButtonDetails(NULL)
	,m_iRank(0)
{
}

GuildJoinCell::~GuildJoinCell()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pNodeExpand);
	CC_SAFE_RELEASE(m_pNodeApplied);
	CC_SAFE_RELEASE(m_pNodeApply);
	CC_SAFE_RELEASE(m_pNodeCancel);
	CC_SAFE_RELEASE(m_pLabelRank);
	CC_SAFE_RELEASE(m_pLabelCount);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pSelectButton);
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pButtonApply);
	CC_SAFE_RELEASE(m_pButtonCancel);
	CC_SAFE_RELEASE(m_pButtonDetails);
}

GuildJoinCell* GuildJoinCell::create()
{
	GuildJoinCell *pRet = new GuildJoinCell();
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

bool GuildJoinCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	//m_pModel = DM_GET_GUILD_MODEL;

	//CCB_READER_AND_ADDCHILD("ui/ccb/guild/guildJoinCell");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_liebiao_cell");

	m_pButtonDetails->setTarget(this, menu_selector(GuildJoinCell::callBackDetails));
	m_pButtonApply->getButton()->setTarget(this, menu_selector(GuildJoinCell::callBackApply));
	m_pButtonCancel->getButton()->setTarget(this, menu_selector(GuildJoinCell::callBackApply));
	return true;
}

void GuildJoinCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildJoinCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildJoinCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_join_cell_expand", CCNode *, this->m_pNodeExpand);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_join_cell_applied", CCNode *, this->m_pNodeApplied);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_join_cell_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_join_cell_rank", CCLabelBMFont *, this->m_pLabelRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_join_cell_count", CCLabelBMFont *, this->m_pLabelCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_join_cell_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonDetails", CCMenuItem *, this->m_pButtonDetails);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_join_cell_apply", DButton *, this->m_pButtonApply);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_join_cell_cancel", DButton *, this->m_pButtonCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_font_apply", CCNode *, this->m_pNodeApply);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_font_cancel", CCNode *, this->m_pNodeCancel);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectButton", CCMenuItem *, this->m_pSelectButton);
	return false;
}

void GuildJoinCell::setExpanded(bool isExpanded)
{
	//m_pNodeExpand->setVisible(isExpanded);
	if (isExpanded)
	{
		m_pContainer->setPositionY(JOIN_CELL_SIZE_EXPANDED.height - JOIN_CELL_SIZE.height);
	}
	else
	{
		m_pContainer->setPositionY(0);
	}
}

void GuildJoinCell::setData(guild::GuildBasics *pData)
{
	if (m_pData == pData && m_pData->isSynced == false)
	{
		return;
	}
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
	m_pData->isSynced = false;
	CC_SAFE_RETAIN(m_pData);
	m_pLabelName->setString(pData->getName().c_str());
	m_pLabelLevel->setString(CCString::createWithFormat("%d", pData->getLevel())->getCString());
	m_pLabelCount->setString(CCString::createWithFormat("%d", pData->getUserCount())->getCString());
	setApplied(pData->getIsApplied());
}

void GuildJoinCell::setRank(uint32_t rank)
{
	m_pLabelRank->setString(CCString::createWithFormat("%d", rank)->getCString());
	m_iRank = rank;
}

void GuildJoinCell::setApplied(bool isApplied)
{
	m_pNodeApplied->setVisible(isApplied);
	m_pNodeApply->setVisible(!isApplied);
	m_pNodeCancel->setVisible(isApplied);
}

void GuildJoinCell::callBackDetails(CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->willShowGuildDetails(m_pData->getGuildID(), m_iRank);
	}
}

void GuildJoinCell::callBackApply(CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->willApplyForTheGuild(m_pData->getGuildID(), m_pNodeApplied->isVisible() ? 2 : 1);
		//m_pDelegate->willApplyForTheGuild(m_pData->getGuildID(), 1);//1是申请，2是取消申请
	}
}