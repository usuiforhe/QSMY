//
//  ServerCell.h
//  QSMY
//
//  Created by wanghejun on 13-9-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ServerCell__
#define __QSMY__ServerCell__

#include "AppInclude.h"
#include "ServerStatus.h"
#include "model/PlatformModel.h"

class ServerCell
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pServerId, ServerId);
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pTouchMenu, TouchMenu);
    CC_SYNTHESIZE_READONLY(ServerStatus*, m_pStatus, Status);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pServerName, ServerName);
    
public:
    CREATE_FUNC(ServerCell);
    ServerCell();
    virtual ~ServerCell();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void setServerInfo(platform::ServerInfo* );
};


class ServerCellLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ServerCellLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ServerCell);
};

#endif /* defined(__QSMY__ServerCell__) */
