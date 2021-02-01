//
//  StrategyExchange.h
//  掌中宝兑换
//
//  Created by wanghejun on 13-9-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__StrategyExchange__
#define __QSMY__StrategyExchange__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/GameManager.h"

class StrategyExchange
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    StrategyExchange();
    virtual ~StrategyExchange();
    CREATE_FUNC(StrategyExchange);
    bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
private:
    void submitCallBack(cocos2d::CCObject* obj);
    void submitCallBack(const Json::Value &requestData, const Json::Value &responseData);
private:
    DButton             *m_pSubmitButton;
    cocos2d::CCNode     *m_pContainer;
    cocos2d::extension::CCEditBox *m_pEditBox;
    
};

#endif /* defined(__QSMY__StrategyExchange__) */
