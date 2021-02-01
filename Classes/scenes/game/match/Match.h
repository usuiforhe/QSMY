//
//  Match.h
//  杯赛
//
//  Created by cll on 13-4-16.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Match__
#define __QSMY__Match__

#include "AppInclude.h"
#include "model/MatchModel.h"
#include "components/components.h"
#include "../Game.h"

class MatchRuleView;
class MatchLotteryView;
class MatchBetView;
class MatchPrizeView;


class MatchDelegate {

public:
	virtual void showMatchBetView(uint32_t finalNum,uint32_t mid) = 0;
    virtual void signUpCup() = 0;
};


class Match
:public DLayer
,public MatchDelegate
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCBMemberVariableAssigner
,public BackKeyInterface
{

public:
	Match();
	~Match();
	virtual bool init();
	CREATE_FUNC(Match);

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;}
    
    
    virtual void onEnter();
    virtual void onExit();

    
private:
    
    void updateView();
    void checkUpdate();
	virtual void showMatchBetView(uint32_t finalNum,uint32_t mid);	//传入比赛id
    virtual void signUpCup();
	void signUpCup(cocos2d::CCObject * sender)
	{
		signUpCup();
	}
    void doSignUpCup(MessageType,DMessageBox*);

	void showMatchRule();
	void menuCallBack(cocos2d::CCObject * sender);
	void intervalCallBack(float dt);
	void refreshCallBack(float dt);
    
private:
    cocos2d::CCNode  *m_pTabContainer;
	cocos2d::CCNode  *m_pContainerSize;
	DTabView *m_pTabView;
	MatchModel *m_pMatchModel;
    
    cocos2d::CCNode* m_pPopupLayer;
    
    bool    isFirstEnter;

public:
	virtual void onBackKey();
};

#endif /* defined(__QSMY__Match__) */
