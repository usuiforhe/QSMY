//
//  Strategy.h
//  掌中宝
//
//  Created by wanghejun on 13-9-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Strategy__
#define __QSMY__Strategy__

#include "AppInclude.h"
#include "components/components.h"
#include "StrategyExchange.h"

class StrategyView
:public DLayer
{
    CC_SYNTHESIZE_RETAIN(cocos2d::CCNode*, m_pContainer, Container);
public:
    StrategyView();
    virtual ~StrategyView();
    CREATE_FUNC(StrategyView);
    bool init();
    void onEnter();
    void onExit();
private:
    CCWebView       *m_pWebView;
};

class Strategy
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
public:
    Strategy();
    virtual ~Strategy();
    CREATE_FUNC(Strategy);
    bool init();
	virtual void onEnter();
	virtual void onExit();

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
private:
    void navCallBack(cocos2d::CCObject* obj);
    
    void showStrategyView();
    void showStrategyExchange();
    
private:
    cocos2d::CCNode         *m_pContainer;
    cocos2d::CCNode         *m_pWebContainer;
    StrategyView            *m_pStrategyView;
    StrategyExchange        *m_pStrategyExchange;
    cocos2d::CCMenu         *m_pMenu;
    
private:
    cocos2d::CCMenuItem     *m_pSelected;
};

#endif /* defined(__QSMY__Strategy__) */
