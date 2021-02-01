//
//  TransForceHead.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TransForceHead.h"
#include "../../../managers/DataManager.h"
#include "../../../managers/ResourceManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

TransForceHead::TransForceHead()
:m_pContainer(NULL)
,m_pGrade(NULL)
,m_pLevel(NULL)
,m_pName(NULL)
,m_pWarriorTypeMark(NULL)
{
    
}


TransForceHead::~TransForceHead()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pLevel);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
}


bool TransForceHead::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade",  CCSprite*, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
    return false;
}

void TransForceHead::setWarriorInfo(warrior::WarriorCurInfo* pWarriorInfo)
{
    if(pWarriorInfo==NULL) {
        reset();
        return;
    }
    updateWarriorInfo(pWarriorInfo->getBaseInfo());
    if(m_pLevel) m_pLevel->setString(CCString::createWithFormat("%d",pWarriorInfo->getWarriorLv())->getCString());
    if(m_pContainer)
    {
        m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForDetail(pWarriorInfo->getAvatarId()));
    }
}


void TransForceHead::setWarriorInfo(warrior::WarriorBaseInfo* pWarriorInfo)
{
    updateWarriorInfo(pWarriorInfo);
    if(m_pContainer && pWarriorInfo)
    {
        m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForDetail(pWarriorInfo->getWarriorId()));
    }
}

void TransForceHead::reset()
{
    if(m_pLevel) m_pLevel->setString(CCString::createWithFormat("%d",0)->getCString());
    if(m_pGrade) m_pGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByGrade(warrior::kWarGradeGreen));
    if(m_pContainer)
    {
        m_pContainer->removeAllChildren();
    }
    if(m_pName) m_pName->setString("");
    if(m_pWarriorTypeMark) m_pWarriorTypeMark->setWarriorInfo(NULL);
}

void TransForceHead::updateWarriorInfo(warrior::WarriorBaseInfo* pWarriorInfo)
{
    if(pWarriorInfo==NULL) {
        reset();
        return;
    }
    
    if(m_pGrade) m_pGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(pWarriorInfo->getWarriorId()));
    if(m_pContainer)
    {
        m_pContainer->removeAllChildren();
    }
    
    if(m_pName) m_pName->setString(pWarriorInfo->getWarriorName().c_str());
    
    if(m_pWarriorTypeMark) m_pWarriorTypeMark->setWarriorInfo(pWarriorInfo);
}


