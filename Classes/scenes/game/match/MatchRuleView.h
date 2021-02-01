//
//  MatchRuleView.h
//  比赛规则
//
//  Created by cll on 13-4-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MatchRuleView__
#define __QSMY__MatchRuleView__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../managers/DataManager.h"

class MatchDelegate;

class MatchRuleView
:public DLayer
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCBMemberVariableAssigner
{
    
public:
	MatchRuleView();
	virtual ~MatchRuleView();
    
    CREATE_FUNC(MatchRuleView);

    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void setDelegate(MatchDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
    
    
    virtual void onEnter();
    virtual void onExit();

    virtual void update(float dt);
    
private:
    
    void updateView();
	void closeCallBack(cocos2d::CCObject * obj);
    void signCallBack(cocos2d::CCObject*obj);

private:
    
	MatchDelegate                       *m_pDelegate;
    MatchModel                          *m_pMatchModel;
    cocos2d::CCMenuItem                 *m_pSignMenu;
    cocos2d::extension::CCScrollView    *m_pPage;
    cocos2d::CCLabelBMFont              *m_pTimeLabel;
    cocos2d::CCNode                     *m_pTime;
	cocos2d::CCNode						*m_pVipLabel;

	DButton									*m_pCloseButton;

	cocos2d::CCSprite						*m_pCompete;
	cocos2d::CCSprite						*m_pCompeted;
	cocos2d::CCMenuItemImage	*m_pCompeteMenu;
    
private:
    float                            m_fTimer;
};



#endif /* defined(__QSMY__MatchRuleView__) */
