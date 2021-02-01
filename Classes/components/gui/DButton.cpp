//
//  DButton.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DButton.h"

USING_NS_CC;
USING_NS_CC_EXT;

DButton::DButton()
:m_pButton(NULL)
{
    
}


DButton::~DButton()
{
    CC_SAFE_RELEASE(m_pButton);
    
}


bool DButton::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton",  CCMenuItemImage*, this->m_pButton);
    return false;
}