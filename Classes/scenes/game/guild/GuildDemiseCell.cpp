//
//  GuildDemiseCell.h
//  公会让位
//
//  Created by Eci on 13-10-26.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildDemiseCell.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"


GuildDemiseCell::GuildDemiseCell()
	:m_pLabelLevel(NULL)
	,m_pLabelKngihthood(NULL)
	,m_pLabelName(NULL)
	,m_pButtonSelected(NULL)
	,m_pData(NULL)
{
}

GuildDemiseCell::~GuildDemiseCell()
{
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelKngihthood);
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pButtonSelected);
	CC_SAFE_RELEASE(m_pData);
}

GuildDemiseCell* GuildDemiseCell::create()
{
	GuildDemiseCell *pRet = new GuildDemiseCell();
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

bool GuildDemiseCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	//m_pModel = DM_GET_GUILD_MODEL;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_guanli_cw_cell");
	return true;
}

void GuildDemiseCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildDemiseCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildDemiseCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_knighthood", CCLabelTTF *, this->m_pLabelKngihthood);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_selected", DButton *, this->m_pButtonSelected);
	return false;
}

void GuildDemiseCell::setData(guild::GuildUserBasics *pData)
{
	if (m_pData == pData && m_pData->isSynced == false)
	{
		return;
	}
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
	m_pData->isSynced = false;
	CC_SAFE_RETAIN(m_pData);
	m_pLabelKngihthood->setString(m_pData->getKnighthoodName().c_str());
	m_pLabelName->setString(m_pData->getName().c_str());
	m_pLabelLevel->setString(CCString::createWithFormat("%u", m_pData->getLevel())->getCString());
	guild::GuildUser *selfInfo = DM_GET_GUILD_MODEL->getSelfInfo();
	if (selfInfo->getUserId() == m_pData->getUserId())
	{
		m_pButtonSelected->getButton()->setEnabled(false);
	}
}

void GuildDemiseCell::setSelected(bool isSelected)
{
	if (m_pButtonSelected->getButton()->isEnabled() == false)
	{
		return;
	}
	if (isSelected)
	{
		m_pButtonSelected->getButton()->selected();
	}
	else
	{
		m_pButtonSelected->getButton()->unselected();
	}
}
