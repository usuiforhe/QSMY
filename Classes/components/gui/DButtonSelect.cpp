//
//  DButtonSelect.cpp
//  QSMY
//
//  Created by wanghejun on 13-12-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DButtonSelect.h"

USING_NS_CC;
USING_NS_CC_EXT;

DButtonSelect::DButtonSelect()
:m_pButton(NULL)
,m_pMark(NULL)
{
    
}


DButtonSelect::~DButtonSelect()
{
    CC_SAFE_RELEASE(m_pButton);
    
}

bool DButtonSelect::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton",  CCMenuItemImage*, this->m_pButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMark",  CCMenuItemImage*, this->m_pMark);
    return false;
}


void DButtonSelect::setSelected(bool b)
{
    if(b) m_pMark->selected();
    else m_pMark->unselected();
}