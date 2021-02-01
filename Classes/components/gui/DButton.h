//
//  DButton.h
//  QSMY
//
//  Created by wanghejun on 13-7-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DButton__
#define __QSMY__DButton__

#include "AppInclude.h"

class DButton
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItemImage*, m_pButton, Button);
    
public:
    CREATE_FUNC(DButton);
    DButton();
    virtual ~DButton();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
};


class DButtonLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(DButtonLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(DButton);
};

#endif /* defined(__QSMY__DButton__) */
