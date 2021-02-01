//
//  StoryTalk.h
//  QSMY
//
//  Created by wanghejun on 13-10-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__StoryTalk__
#define __QSMY__StoryTalk__

#include "AppInclude.h"
#include "model/BattleModel.h"
#include "components/components.h"

class StoryTalk
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pDialogRight, DialogRight);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pDialogLeft, DialogLeft);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pDialogNoHead, DialogNoHead);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pContainerRight, ContainerRight);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pContainerLeft, ContainerLeft);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pDialogLabelRight, DialogLabelRight);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pDialogLabelLeft, DialogLabelLeft);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pDialogLabelNoHead, DialogLabelNoHead);
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pNextDialogButton, NextDialogButton);
    
public:
    CREATE_FUNC(StoryTalk);
    StoryTalk();
    virtual ~StoryTalk();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void showInfo(battle::BattleDialogInfo* pInfo);
private:
    bool                     first;
    cocos2d::CCRichLabelTTF  *m_pDialogRichLabelLeft;
    cocos2d::CCRichLabelTTF  *m_pDialogRichLabelRight;
	cocos2d::CCRichLabelTTF	 *m_pDialogRichLabelNoHead;
};


class StoryTalkLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(StoryTalkLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(StoryTalk);
};

#endif /* defined(__QSMY__StoryTalk__) */
