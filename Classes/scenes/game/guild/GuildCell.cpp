//
//  GuildCell.h
//  公会主页
//
//  Created by Eci on 13-10-28.

//  Copyright (c) 2013年 thedream. All rights reserved.
//



#include "GuildCell.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"


GuildCell::GuildCell()
	: m_pSpriteEnabled(NULL)
	, m_pSpriteDisabled(NULL)
	, m_pLabelTime(NULL)
	, m_sFileName("")
	, m_isEnabled(false)
{
}

GuildCell::~GuildCell()
{
	CC_SAFE_RELEASE(m_pSpriteEnabled);
	CC_SAFE_RELEASE(m_pSpriteDisabled);
	CC_SAFE_RELEASE(m_pLabelTime);
}

GuildCell *GuildCell::create()
{
	GuildCell *pRet = new GuildCell();
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

bool GuildCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	//m_pModel = DM_GET_GUILD_MODEL;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/guildCell");

	m_pLabelTime->setVisible(false);
	return true;
}

void GuildCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_page_enabled", CCLayer *, this->m_pSpriteEnabled);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_page_disabled", CCLayer *, this->m_pSpriteDisabled);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_time", CCLabelBMFont *, this->m_pLabelTime);
	return false;
}

void GuildCell::setFileName(const char *pFileName)
{
	if (m_sFileName != pFileName)
	{
		m_sFileName = pFileName;
        
        {
            m_pSpriteEnabled->removeAllChildren();
            DSprite *pNewSprite = DSprite::create(pFileName);
            if (pNewSprite)
            {
                pNewSprite->setAnchorPoint(CCPointZero);
                //pNewSprite->setHsvColorModify(ccc3(255, 120, 255));
                m_pSpriteEnabled->addChild(pNewSprite);
            }
        }
        {
            m_pSpriteDisabled->removeAllChildren();
            DSprite *pNewSprite = DSprite::create(pFileName);
            if (pNewSprite)
            {
                pNewSprite->setAnchorPoint(CCPointZero);
				/************************************************************************/
				/* 11.11编译不通过                                                                     */
				/************************************************************************/
                //pNewSprite->setHsvColorModify(ccc3(255, 120, 255));
                m_pSpriteDisabled->addChild(pNewSprite);
            }
        }
	}
}

void GuildCell::setEnabled(bool isEnabled)
{
	m_isEnabled = isEnabled;
	if (isEnabled)
	{
		m_pSpriteEnabled->setVisible(true);
		m_pSpriteDisabled->setVisible(false);
	}
	else
	{
		m_pSpriteEnabled->setVisible(false);
		m_pSpriteDisabled->setVisible(true);
	}
}

void GuildCell::setTime(uint32_t iTime)
{
	if (iTime > 0)
	{
		m_pLabelTime->setVisible(true);
		m_pLabelTime->setString(TimeUtil::getSeconds2String(iTime, "%H:%M:%S"));
		setEnabled(m_isEnabled);
	}
	else
	{
		m_pLabelTime->setVisible(false);
		setEnabled(m_isEnabled);
	}
}