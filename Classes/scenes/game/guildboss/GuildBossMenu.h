//
//  GuildBossMenu.h
//  苍龙菜单
//
//  Created by Eci on 14-01-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__GuildBossMenu__
#define __QSMY__GuildBossMenu__

#include "AppInclude.h"
#include "components/DProgressBar.h"
#include "managers/DataManager.h"

class GuildBossMenu
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    
public:
    CREATE_FUNC(GuildBossMenu);
    GuildBossMenu();
    virtual ~GuildBossMenu();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

};


class GuildBossMenuLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildBossMenuLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildBossMenu);
};

#endif /* defined(__QSMY__GuildBossMenu__) */
