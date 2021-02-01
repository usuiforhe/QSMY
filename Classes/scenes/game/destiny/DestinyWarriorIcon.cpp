//
//  DestinyWarriorIcon.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DestinyWarriorIcon.h"
#include "../../../managers/ResourceManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

DestinyWarriorIcon::DestinyWarriorIcon()
:m_pGrade(NULL)
,m_pContainer(NULL)
,m_pMenuItem(NULL)
,m_pHighLight(NULL)
,m_pFullEffect(NULL)
,m_pName(NULL)
{
    
}


DestinyWarriorIcon::~DestinyWarriorIcon()
{
    CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pMenuItem);
    CC_SAFE_RELEASE(m_pHighLight);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pFullEffect);
}

bool DestinyWarriorIcon::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade",  CCSprite*, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuItem", CCMenuItemImage *, this->m_pMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHighLight", CCNode*, this->m_pHighLight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFullEffect", CCNode*, this->m_pFullEffect);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF*, this->m_pName);
    return false;
}


#pragma mark -- DestinyWarriorIconItem

DestinyWarriorIconItem::DestinyWarriorIconItem()
:m_pCount(NULL)
,m_pCanBeCollected(NULL)
,m_pIcon(NULL)
{
    
}

DestinyWarriorIconItem::~DestinyWarriorIconItem()
{
    CC_SAFE_RELEASE(m_pCount);
    CC_SAFE_RELEASE(m_pCanBeCollected);
    CC_SAFE_RELEASE(m_pIcon);
}


bool DestinyWarriorIconItem::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCount",  CCLabelBMFont*, this->m_pCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCanBeCollected", CCNode *, this->m_pCanBeCollected);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIcon", DestinyWarriorIcon *, this->m_pIcon);
    return false;
}

void DestinyWarriorIconItem::setCanBeCollected(bool val)
{
    m_pCanBeCollected->removeAllChildren();
    m_pCanBeCollected->setVisible(val);
    if(m_pCanBeCollected->isVisible())
    {
        m_pCanBeCollected->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("collect"));
    }
    
}



