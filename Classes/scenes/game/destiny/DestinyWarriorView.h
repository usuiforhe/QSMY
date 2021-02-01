//
//  DestinyWarriorView.h
//  QSMY
//
//  Created by wanghejun on 13-7-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DestinyWarriorView__
#define __QSMY__DestinyWarriorView__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/DataManager.h"
#include "Destiny.h"
#include "DestinyWarriorHead.h"
#include "DestinyWarriorIcon.h"

class DestinyWarriorView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    CC_SYNTHESIZE(DestinyDelegate*, m_DestinyDelegate, Delegate);
    CC_SYNTHESIZE_READONLY(destiny::DestinyInfo*, m_pDestinyInfo, DestinyInfo);
    
public:
    DestinyWarriorView();
    virtual ~DestinyWarriorView();
    CREATE_FUNC(DestinyWarriorView);
    virtual bool init();
    
    void setDestinyInfo(destiny::DestinyInfo* pDestinyInfo,uint8_t slotIndex);
    
    virtual void onEnter();
    virtual void onExit();
public:
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
private:
    void closeCallBack(cocos2d::CCObject* sender);
    void updateCanCollectStatus();
    void updateDestinyInfo();
    void updateSlotInfo();
    void updateHighLightStatus();
    void submitCallBack(cocos2d::CCObject* sender);
    void improveCallBack(const Json::Value &requestData, const Json::Value &responseData);
    void updateView();
private:
    destiny::DestinySlot        *m_pSlot;
private:
    cocos2d::CCMenuItemImage    *m_pGiveup;
    cocos2d::CCMenuItemImage    *m_pSubmit;
    DestinyWarriorIcon          *m_pWarriorIcon;
    DestinyWarriorHead          *m_pWarriorHead;
    DestinyWarriorHead          *m_pSoulHead;
    cocos2d::CCLabelBMFont      *m_pCompleteCount;
    cocos2d::CCLabelBMFont      *m_pNeedCount;
private:
    DestinyModel                *m_pDestinyModel;
    WarriorModel                *m_pWarriorModel;
    FormationModel              *m_pFormationModel;
    bool                        m_bIsFull;
};


#endif /* defined(__QSMY__DestinyWarriorView__) */
