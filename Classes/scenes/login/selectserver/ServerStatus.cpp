//
//  ServerStatus.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ServerStatus.h"

USING_NS_CC;
USING_NS_CC_EXT;

ServerStatus::ServerStatus()
:m_pFireStatus(NULL)
,m_pNewStatus(NULL)
,m_pMaintainStatus(NULL)
{
    
}

ServerStatus::~ServerStatus()
{
    CC_SAFE_RELEASE(m_pFireStatus);
    CC_SAFE_RELEASE(m_pNewStatus);
	CC_SAFE_RELEASE(m_pMaintainStatus);
}


bool ServerStatus::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFireStatus",  CCNode*, this->m_pFireStatus);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNewStatus",  CCNode*, this->m_pNewStatus);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMaintainStatus",  CCNode*, this->m_pMaintainStatus);
    
    return false;
}

void ServerStatus::setStatus(platform::ServerStatus val)
{
    m_pNewStatus->setVisible(val==platform::kServerStatusNew);
    m_pFireStatus->setVisible(val==platform::kServerStatusFire);
	m_pMaintainStatus->setVisible(val==platform::kServerStatusMaintain);
}

