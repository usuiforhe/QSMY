//
//  WarriorTypeMark.h
//  QSMY
//
//  Created by wanghejun on 13-8-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__WarriorTypeMark__
#define __QSMY__WarriorTypeMark__

#include "AppInclude.h"
#include "model/WarriorModel.h"

class WarriorTypeMark
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
	friend class PictureCollect;

    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*,m_pT,TypeT);
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*,m_pDPS,TypeDPS);
	CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*,m_pAssist,TypeAssist);
    
public:
    CREATE_FUNC(WarriorTypeMark);
    WarriorTypeMark();
    virtual ~WarriorTypeMark();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    void setWarriorInfo(warrior::WarriorBaseInfo* pInfo);
	void setWarriorInfobyHandBook(warrior::WarriorHandBookInfo* pInfo);
	void reset();
};


class WarriorTypeMarkLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(WarriorTypeMarkLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(WarriorTypeMark);
};

#endif /* defined(__QSMY__WarriorTypeMark__) */
