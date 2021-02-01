//
//  MatchBetMenu.h
//  杯赛中的竞猜按钮
//
//  Created by wanghejun on 13-7-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MatchBetMenu__
#define __QSMY__MatchBetMenu__

#include "AppInclude.h"
#include "model/MatchModel.h"

class MatchBetMenu
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItemImage*, m_pRewardMenuItem, RewardMenuItem);
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItemImage*, m_pBgMenuItem, BgMenuItem);
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItemImage*, m_pBetMenuItem, BetMenuItem);
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItemImage*, m_pViewMenuItem, ViewMenuItem);
    
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pFailed, Failed);
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pBeted, Beted);
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pRewarded, Rewarded);
    
    
    CC_SYNTHESIZE_READONLY(match::VSInfo*, m_pVSInfo, VSInfo);
    CC_SYNTHESIZE_READONLY(match::BetInfo*, m_pBetInfo, BetInfo);
    
public:
    
    void setVSInfo(match::VSInfo *pVSInfo);
    
    CREATE_FUNC(MatchBetMenu);
    MatchBetMenu();
    virtual ~MatchBetMenu();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
private:
    void updateView();
    
private:
    MatchModel *m_pModel;
};


class MatchBetMenuLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MatchBetMenuLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MatchBetMenu);
};

#endif /* defined(__QSMY__MatchBetMenu__) */
