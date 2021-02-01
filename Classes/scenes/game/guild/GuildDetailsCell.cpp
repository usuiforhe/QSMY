//
//  GuildDetailsCell.h
//  公会详情
//
//  Created by Eci on 13-10-28.

//  Copyright (c) 2013年 thedream. All rights reserved.
//



#include "GuildDetailsCell.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"


GuildDetailsCell::GuildDetailsCell()
	:m_pLabelName(NULL)
	,m_pLabelLevel(NULL)
	,m_pLabelKnighthood(NULL)
	,m_pBossMark(NULL)
	,m_pViceMark(NULL)
{
}

GuildDetailsCell::~GuildDetailsCell()
{
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelKnighthood);
	CC_SAFE_RELEASE(m_pBossMark);
	CC_SAFE_RELEASE(m_pViceMark);
}

GuildDetailsCell* GuildDetailsCell::create()
{
	GuildDetailsCell *pRet = new GuildDetailsCell();
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

bool GuildDetailsCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	//m_pModel = DM_GET_GUILD_MODEL;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_paihang_xinxi_cell");
	return true;
}

void GuildDetailsCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildDetailsCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildDetailsCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_knighthood", CCLabelTTF *, this->m_pLabelKnighthood);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBossMark", CCNode *, this->m_pBossMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pViceMark", CCNode *, this->m_pViceMark);
	return false;
}

void GuildDetailsCell::setData(ValueObjcet *pData)
{
	guild::GuildUserBasics *userData = dynamic_cast<guild::GuildUserBasics *>(pData);
	m_pLabelName->setString(userData->getName().c_str());
	m_pLabelLevel->setString(CCString::createWithFormat("%u", userData->getLevel())->getCString());
	m_pLabelKnighthood->setString(userData->getKnighthoodName().c_str());

	m_pBossMark->setVisible(userData->getKnighthoodLevel() == 0);
	//m_pViceMark->setVisible(userData->getKnighthoodLevel() == 1);
	m_pLabelKnighthood->setVisible(!(m_pBossMark->isVisible() || m_pViceMark->isVisible()));
}
