//
//  GuildApplyCell.h
//  公会申请
//
//  Created by Eci on 13-10-25.

//  Copyright (c) 2013年 thedream. All rights reserved.
//



#include "GuildApplyCell.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"


GuildApplyCell::GuildApplyCell()
	:m_pContainer(NULL)
	//,m_pSelectButton(NULL)
	//,m_pNodeExpand(NULL)
	,m_pLabelName(NULL)
	,m_pAdmin(NULL)
	,m_pLabelFight(NULL)
	,m_pLabelLevel(NULL)
	,m_pButtonAgree(NULL)
	,m_pButtonRefuse(NULL)
	,m_pDelegate(NULL)
	,m_pData(NULL)
{
}

GuildApplyCell::~GuildApplyCell()
{
	CC_SAFE_RELEASE(m_pContainer);
	//CC_SAFE_RELEASE(m_pSelectButton);
	//CC_SAFE_RELEASE(m_pNodeExpand);
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pAdmin);
	CC_SAFE_RELEASE(m_pLabelFight);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pButtonAgree);
	CC_SAFE_RELEASE(m_pButtonRefuse);
	CC_SAFE_RELEASE(m_pData);
}

GuildApplyCell* GuildApplyCell::create()
{
	GuildApplyCell *pRet = new GuildApplyCell();
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

bool GuildApplyCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	//m_pModel = DM_GET_GUILD_MODEL;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_guanli_shenqing");

	m_pButtonAgree->getButton()->setTarget(this, menu_selector(GuildApplyCell::callBackAgree));
	m_pButtonRefuse->getButton()->setTarget(this, menu_selector(GuildApplyCell::callBackRefuse));

	guild::GuildPosition iPosition = DM_GET_GUILD_MODEL->getSelfInfo()->getPosition();
	if (guild::GuildPositionAssistant != iPosition && guild::GuildPositionPresident != iPosition)
	{
		m_pAdmin->setVisible(false);
	}
	return true;
}

void GuildApplyCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildApplyCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildApplyCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_expand", CCNode *, this->m_pNodeExpand);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_fight", CCLabelBMFont *, this->m_pLabelFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_agree", DButton *, this->m_pButtonAgree);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_refuse", DButton *, this->m_pButtonRefuse);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAdmin", CCNode *, this->m_pAdmin);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectButton", CCMenuItem *, this->m_pSelectButton);

	return false;
}

void GuildApplyCell::setData(ValueObjcet *pData)
{
	if (pData == m_pData)
	{
		return;
	}
	CC_SAFE_RELEASE(m_pData);
	m_pData = dynamic_cast<guild::GuildApplication *>(pData);
	CC_SAFE_RETAIN(m_pData);
	m_pLabelName->setString(m_pData->getName().c_str());
	m_pLabelLevel->setString(CCString::createWithFormat("%u" ,m_pData->getLevel())->getCString());
	m_pLabelFight->setString(CCString::createWithFormat("%u" ,m_pData->getFight())->getCString());
}

void GuildApplyCell::setExpand(bool isExpanded)
{
	//m_pNodeExpand->setVisible(isExpanded);
	if (isExpanded)
	{
		m_pContainer->setPositionY(APPLY_CELL_SIZE_EXPAND.height - APPLY_CELL_SIZE.height);
	}
	else
	{
		m_pContainer->setPositionY(0);
	}
}

void GuildApplyCell::callBackAgree(cocos2d::CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->willDealApply(m_pData->getApplicationID(), 1);
	}
}

void GuildApplyCell::callBackRefuse(cocos2d::CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->willDealApply(m_pData->getApplicationID(), 2);
	}
}