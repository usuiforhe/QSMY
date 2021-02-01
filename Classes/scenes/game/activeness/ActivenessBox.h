//
//  ActivenessBox.h
//  QSMY
//
//  Created by 汪燕军 on 13-8-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ActivenessBox__
#define __QSMY__ActivenessBox__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/ActivenessModel.h"
using namespace activeness;

struct ActivenessDelegate;


#pragma mark - ActivenessBox -

class ActivenessBox:
public cocos2d::CCNode,
public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    ActivenessBox();
    
    ~ActivenessBox();
    
    CREATE_FUNC(ActivenessBox);
    
    D_INIT_DEFAULT(CCNode);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    void updateView();
    
    inline void setAward(AwardInfo * pAward)
    {
        if(pAward == m_pAward)
        {
            return;
        }
        CC_SAFE_RELEASE(m_pAward);
        m_pAward = pAward;
        CC_SAFE_RETAIN(m_pAward);
    }
    
    inline void setDelegate(ActivenessDelegate * pDelegate)
    {
        m_pDelegate = pDelegate;
    }
    
private:
    void onViewClick(cocos2d::CCObject * sender);
    
private:
    cocos2d::CCSprite * m_pBoxUnqualified;
    cocos2d::CCSprite * m_pBoxAvailable;
    cocos2d::CCSprite * m_pBoxOpened;
    cocos2d::CCSprite * m_pPointDark;
    cocos2d::CCSprite * m_pPointLight;
    cocos2d::CCMenuItem * m_pButton;
    cocos2d::CCNode     *m_pBoxAvailableEffect;
    AwardInfo * m_pAward;
    ActivenessDelegate * m_pDelegate;
};

class ActivenessBoxLoader:
public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ActivenessBoxLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ActivenessBox);
};

#endif /* defined(__QSMY__ActivenessBox__) */
