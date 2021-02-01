//
//  GuildTreeCell.h
//  公会天工树
//
//  Created by Eci on 13-10-31.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "GuildTreeCell.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"


GuildTreeCell::GuildTreeCell()
	:m_pNodeEnabled(NULL)
	,m_pLabelLevel(NULL)
	,m_pLabelName(NULL)
	,m_pLabelKnighthood(NULL)
	,m_pNodeActive(NULL)
	,m_pNodeRequire(NULL)
	,m_pModel(NULL)
	,m_pData(NULL)
	,bg_xiadao(NULL)
	,bg_wangdao(NULL)
	,m_pActive(NULL)
{
}

GuildTreeCell::~GuildTreeCell()
{
	CC_SAFE_RELEASE(m_pNodeEnabled);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pLabelKnighthood);
	CC_SAFE_RELEASE(m_pNodeActive);
	CC_SAFE_RELEASE(m_pNodeRequire);
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(bg_xiadao);
	CC_SAFE_RELEASE(bg_wangdao);
	CC_SAFE_RELEASE(m_pActive);
}

GuildTreeCell* GuildTreeCell::create()
{
	GuildTreeCell *pRet = new GuildTreeCell();
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

bool GuildTreeCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	//CCB_READER_AND_ADDCHILD("ui/ccb/guild/guildTreeCell");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_jianshe_xiadao");
	m_pModel = DM_GET_GUILD_MODEL;
	return true;
}

void GuildTreeCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildTreeCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildTreeCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_enabled", CCNode *, m_pNodeEnabled);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_name", CCLabelTTF *, m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_knighthood", CCLabelTTF *, m_pLabelKnighthood);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeActive", CCNode *, m_pNodeActive);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRequire", CCNode *, m_pNodeRequire);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bg_xiadao", CCNode *, bg_xiadao);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bg_wangdao", CCNode *, bg_wangdao);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pActive", CCNode *, m_pActive);
	return false;
}

void GuildTreeCell::setData(ValueObjcet *pData)
{
	guild::GuildTreeSkill *skillData = dynamic_cast<guild::GuildTreeSkill *>(pData);
	if (!skillData)
	{
		return;
	}
	CC_SAFE_RETAIN(skillData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = skillData;

	m_pLabelLevel->setString(CCString::createWithFormat("%d", skillData->level)->getCString());
	m_pLabelName->setString(skillData->text.c_str());
	m_pLabelKnighthood->setString(DM_GET_GUILD_MODEL->GetKnighthoodNameByLevel(skillData->knighthood).c_str());
}

void GuildTreeCell::setEnabled(bool isEnabled)
{
	//m_pNodeEnabled->setVisible(isEnabled);

	uint32_t lv = m_pModel->getSelfInfo()->getKnighthoodLevel();
	m_pNodeActive->setVisible(m_pData != NULL && isEnabled && (lv >= m_pData->knighthood || lv == 0));
	m_pNodeRequire->setVisible(!m_pNodeActive->isVisible());
	m_pActive->setVisible(isEnabled);
}