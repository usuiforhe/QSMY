//
//  ServerCell.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ServerCell.h"

USING_NS_CC;
USING_NS_CC_EXT;

ServerCell::ServerCell()
:m_pServerId(NULL)
,m_pTouchMenu(NULL)
,m_pStatus(NULL)
,m_pServerName(NULL)
{
    
}

ServerCell::~ServerCell()
{
    CC_SAFE_RELEASE(m_pServerId);
    CC_SAFE_RELEASE(m_pTouchMenu);
    CC_SAFE_RELEASE(m_pStatus);
    CC_SAFE_RELEASE(m_pServerName);
}


bool ServerCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pServerId",  CCLabelTTF*, this->m_pServerId);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchMenu",  CCMenuItem*, this->m_pTouchMenu);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStatus",  ServerStatus*, this->m_pStatus);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pServerName",  CCLabelTTF*, this->m_pServerName);
    
    return false;
}

void ServerCell::setServerInfo(platform::ServerInfo* pInfo)
{
    m_pServerId->setString(D_LOCAL_STRING("Server%d",pInfo->getSID()).c_str());
    m_pServerName->setString(pInfo->getName().c_str());
    m_pStatus->setStatus(pInfo->getStatus());
}

