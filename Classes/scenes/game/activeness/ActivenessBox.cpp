//
//  ActivenessBox.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-8-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ActivenessBox.h"
#include "Activeness.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

#pragma mark - ActivenessBox -

ActivenessBox::ActivenessBox():
m_pBoxUnqualified(NULL),
m_pBoxAvailable(NULL),
m_pBoxOpened(NULL),

m_pDelegate(NULL),
m_pAward(NULL),
m_pButton(NULL)
,m_pBoxAvailableEffect(NULL)
{
};

ActivenessBox::~ActivenessBox()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pBoxUnqualified);
    CC_SAFE_RELEASE(m_pBoxAvailable);
    CC_SAFE_RELEASE(m_pBoxOpened);

    CC_SAFE_RELEASE(m_pAward);
    CC_SAFE_RELEASE(m_pButton);
    CC_SAFE_RELEASE(m_pBoxAvailableEffect);
};


void ActivenessBox::onEnter()
{
    CCNode::onEnter();
    BINDING_ONLY(DM_GET_ACTIVENESS_MODEL, this, ActivenessBox::updateView,D_EVENT_ACTIVENESS_AWARD_DRAWN);
    BINDING_EXEC(DM_GET_ACTIVENESS_MODEL, this, ActivenessBox::updateView,D_EVENT_ACTIVENESS_UPDATE);
}

void ActivenessBox::onExit()
{
    UNBINDING_ALL(DM_GET_ACTIVENESS_MODEL, this);
    CCNode::onExit();
}

bool ActivenessBox::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxUnqualified", cocos2d::CCSprite *, this->m_pBoxUnqualified);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxAvailable", cocos2d::CCSprite *, this->m_pBoxAvailable);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxAvailableEffect", cocos2d::CCNode *, this->m_pBoxAvailableEffect);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxOpened", cocos2d::CCSprite *, this->m_pBoxOpened);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", cocos2d::CCMenuItem *, this->m_pButton);
    
    return false;
}

void ActivenessBox::updateView()
{
    if(m_pAward==NULL)
    {
        //
        return;
    }
    
    bool available = DM_GET_ACTIVENESS_MODEL->isAwardAvailable(m_pAward);
    bool drawn = DM_GET_ACTIVENESS_MODEL->isAwardDrawn(m_pAward);
    
    m_pBoxUnqualified->setVisible(!available);
    m_pBoxAvailable->setVisible(available && !drawn);
    m_pBoxAvailableEffect->setVisible(m_pBoxAvailable->isVisible());
    m_pBoxAvailableEffect->removeAllChildren();
    if(m_pBoxAvailableEffect->isVisible())
    {
        m_pBoxAvailableEffect->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("activeness_get"));
    }
    m_pBoxOpened->setVisible(drawn);
    

    
    m_pButton->setTarget(this, menu_selector(ActivenessBox::onViewClick));
}

void ActivenessBox::onViewClick(cocos2d::CCObject *sender)
{
    if(m_pDelegate)
    {
        m_pDelegate->showAwardView(m_pAward);
    }
}
