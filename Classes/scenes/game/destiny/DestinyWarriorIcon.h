//
//  DestinyWarriorIcon.h
//  天命显示弟子
//
//  Created by wanghejun on 13-7-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DestinyWarriorIcon__
#define __QSMY__DestinyWarriorIcon__

#include "AppInclude.h"
class DestinyWarriorIcon
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pGrade, Grade);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pContainer, HeadContainer);
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItemImage*, m_pMenuItem, MenuItem);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pHighLight, HighLight);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pFullEffect, FullEffect);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pName, Name);
    
public:
    CREATE_FUNC(DestinyWarriorIcon);
    DestinyWarriorIcon();
    virtual ~DestinyWarriorIcon();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
};


class DestinyWarriorIconLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(DestinyWarriorIconLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(DestinyWarriorIcon);
};



class DestinyWarriorIconItem
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pCount, Count);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pCanBeCollected, CanBeCollected);
    CC_SYNTHESIZE_READONLY(DestinyWarriorIcon*, m_pIcon, Icon);
    
public:
    CREATE_FUNC(DestinyWarriorIconItem);
    DestinyWarriorIconItem();
    virtual ~DestinyWarriorIconItem();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void setCanBeCollected(bool val);
};


class DestinyWarriorIconItemLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(DestinyWarriorIconItemLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(DestinyWarriorIconItem);
};




#endif /* defined(__QSMY__DestinyWarriorIcon__) */
