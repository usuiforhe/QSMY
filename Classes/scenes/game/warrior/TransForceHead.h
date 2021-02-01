//
//  TransForceHead.h
//  QSMY
//
//  Created by wanghejun on 13-8-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TransForceHead__
#define __QSMY__TransForceHead__

#include "AppInclude.h"
#include "model/WarriorModel.h"
#include "../common/common.h"

class TransForceHead
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pGrade, Grade);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pContainer, HeadContainer);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pLevel, Level);    
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pName, Name);
    CC_SYNTHESIZE_READONLY(WarriorTypeMark*, m_pWarriorTypeMark, WarriorTypeMark);
    
public:
    CREATE_FUNC(TransForceHead);
    TransForceHead();
    virtual ~TransForceHead();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void setWarriorInfo(warrior::WarriorBaseInfo* pWarriorInfo);
    void setWarriorInfo(warrior::WarriorCurInfo* pWarriorInfo);
    void reset();
    
private:
    void updateWarriorInfo(warrior::WarriorBaseInfo* pWarriorInfo);
};


class TransForceHeadLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(TransForceHeadLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(TransForceHead);
};

#endif /* defined(__QSMY__TransForceHead__) */
