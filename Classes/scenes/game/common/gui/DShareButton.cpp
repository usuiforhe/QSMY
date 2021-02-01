//
//  DShareButton.cpp
//  QSMY
//
//  Created by heven on 14-4-29.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "DShareButton.h"
#include "managers/DataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

DShareButton::DShareButton()
:m_pAward(NULL)
,m_pModel(NULL)
{
    
}

DShareButton::~DShareButton()
{
    CC_SAFE_RELEASE(m_pAward);
    
}


bool DShareButton::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAward",  CCNode*, this->m_pAward);
    return DButton::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

void DShareButton::onEnter()
{
    DButton::onEnter();
    m_pModel = DM_GET_SHARE_MODEL;
    BINDING_EXEC(m_pModel, this, DShareButton::updateView, D_EVENT_SHARE_TIMES_UPDATE);
}

void DShareButton::onExit()
{
    DButton::onExit();
    UNBINDING_ALL(m_pModel, this);
    m_pModel = NULL;
}

void DShareButton::updateView()
{
    m_pAward->setVisible(m_pModel!=NULL &&m_pModel->isCanReward());
}

