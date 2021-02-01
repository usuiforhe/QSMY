//
//  GuildBossPoint.h
//  苍龙星宿
//
//  Created by Eci on 14-01-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "GuildBossPoint.h"
#include "GuildBoss.h"

USING_NS_CC;
USING_NS_CC_EXT;

GuildBossPoint::GuildBossPoint()
	: m_pBtnPoint(NULL)
	, m_pBMFTime(NULL)
	, m_pNodeTime(NULL)
	, m_pDelegate(NULL)
	, m_pNextPoint(NULL)
{
    
}


GuildBossPoint::~GuildBossPoint()
{
	CC_SAFE_RELEASE(m_pBtnPoint);
	CC_SAFE_RELEASE(m_pBMFTime);
	CC_SAFE_RELEASE(m_pNodeTime);
}


bool GuildBossPoint::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnPoint", CCMenuItem *, m_pBtnPoint);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFTime", CCLabelBMFont *, m_pBMFTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeTime", CCNode *, m_pNodeTime);
    return false;
}

SEL_MenuHandler GuildBossPoint::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackPoint", GuildBossPoint::callBackPoint);
	return NULL;
}

void GuildBossPoint::callBackPoint(CCObject *pSender)
{
	if (m_pDelegate)
	{
		//m_pDelegate->GuildBossPointSelected(this);
	}
	
}

void GuildBossPoint::setCleared(bool cleared)
{
	m_pBtnPoint->setEnabled(!cleared);
	m_pNodeTime->setVisible(!cleared);
}

void GuildBossPoint::setCountingDown(int countingDown)
{
	if (countingDown <= 0)
	{
		m_pNodeTime->setVisible(false);
	}
	else
	{
		if (countingDown >= 3600)
		{
			m_pBMFTime->setString(TimeUtil::getSeconds2String(countingDown, "%H:%M:%S"));
		}
		else
		{
			m_pBMFTime->setString(TimeUtil::getSeconds2String(countingDown, "%M:%S"));
		}
	}
}

void GuildBossPoint::setDayCountingDown(int dayCountingDown)
{
	m_pBMFTime->setString(D_CSTRING_FROM_INT(dayCountingDown));
}

bool GuildBossPoint::getIsOpen()
{
	return !m_pNodeTime->isVisible() && m_pBtnPoint->isEnabled();
}