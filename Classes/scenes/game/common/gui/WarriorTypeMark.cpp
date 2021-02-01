//
//  WarriorTypeMark.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "WarriorTypeMark.h"

USING_NS_CC;
USING_NS_CC_EXT;

WarriorTypeMark::WarriorTypeMark()
:m_pT(NULL)
,m_pDPS(NULL)
,m_pAssist(NULL)
{
    
}

WarriorTypeMark::~WarriorTypeMark()
{
    CC_SAFE_RELEASE(m_pT);
    CC_SAFE_RELEASE(m_pDPS);
	CC_SAFE_RELEASE(m_pAssist);
}


bool WarriorTypeMark::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pT",  CCSprite*, this->m_pT);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDPS",  CCSprite*, this->m_pDPS);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAssist", CCSprite*, this->m_pAssist);
    return false;
}

void WarriorTypeMark::reset()
{
	if(m_pT)m_pT->setVisible(false);
	if(m_pDPS)m_pDPS->setVisible(false);
	if(m_pAssist)m_pAssist->setVisible(false);
}
void WarriorTypeMark::setWarriorInfo(warrior::WarriorBaseInfo* pInfo)
{
	if (this == NULL)
	{
		CCLog("error: WarriorTypeMark::setWarriorInfo, NULL pointer");
		return;
	}
	
	if (pInfo == NULL)
	{
		reset();
	}
	
    if(m_pT)m_pT->setVisible(pInfo && pInfo->getLocation()==warrior::kLocationTypeT);
    if(m_pDPS)m_pDPS->setVisible(pInfo && pInfo->getLocation()==warrior::kLocationTypeDPS);
	if(m_pAssist)m_pAssist->setVisible(pInfo && pInfo->getLocation()==warrior::kLocationTypeAssist);
}

void WarriorTypeMark::setWarriorInfobyHandBook(warrior::WarriorHandBookInfo* pInfo)
{
	if(getTypeT())
	{
		getTypeT()->setVisible(pInfo && pInfo->getLocationg()==warrior::kLocationTypeT);
	}
	if(getTypeDPS())
	{
		getTypeDPS()->setVisible(pInfo && pInfo->getLocationg()==warrior::kLocationTypeDPS);
	}
	if(getTypeAssist())
	{
		getTypeAssist()->setVisible(pInfo && pInfo->getLocationg()==warrior::kLocationTypeAssist);
	}
}
