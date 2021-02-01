//
//  ServerStatus.h
//  QSMY
//
//  Created by wanghejun on 13-9-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ServerStatus__
#define __QSMY__ServerStatus__

#include "AppInclude.h"
#include "model/PlatformModel.h"

class ServerStatus
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pFireStatus, FireStatus);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pNewStatus, NewStatus);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pMaintainStatus, MaintainStatus);
    
public:
    CREATE_FUNC(ServerStatus);
    ServerStatus();
    virtual ~ServerStatus();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void setStatus(platform::ServerStatus);
};


class ServerStatusLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ServerStatusLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ServerStatus);
};

#endif /* defined(__QSMY__ServerStatus__) */
