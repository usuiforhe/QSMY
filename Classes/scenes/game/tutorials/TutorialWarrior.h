//
//  TutorialWarrior.h
//  新手弟子选择
//
//  Created by wanghejun on 13-8-22.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TutorialWarrior__
#define __QSMY__TutorialWarrior__

#include "AppInclude.h"
#include "../../../model/TutorialModel.h"

class TutorialWarrior
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pContainer, Container);              //头像
	CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pSelectStatus, SelectStatus);	//选择状态
    CC_SYNTHESIZE_RETAIN(tutorial::WarriorInfo*, m_pWarriorInfo, WarriorInfo);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pEffectNode, EffectNode);            //效果
	CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pWarriorImage, WarriorImage);
    
public:
    CREATE_FUNC(TutorialWarrior);
    TutorialWarrior();
    virtual ~TutorialWarrior();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void setWarriorImage(cocos2d::CCSprite *pImage);
};


class TutorialWarriorLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(TutorialWarriorLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(TutorialWarrior);
};


#endif /* defined(__QSMY__TutorialWarrior__) */
