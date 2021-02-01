//
//  SettlementViewPve.h
//  QSMY
//
//  Created by wanghejun on 13-8-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SettlementViewPve__
#define __QSMY__SettlementViewPve__

#include "SettlementView.h"
#include "../../common/common.h"

class SettlementViewPve
:public SettlementView
,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    SettlementViewPve();
    virtual ~SettlementViewPve();    
    virtual bool init(SettlementDelegate *delegate,settlement::SettlementInfo *pInfo);
    virtual void closeView();
private:
    void quitBattle(cocos2d::CCObject*);
	void replayBattle(cocos2d::CCObject*);
	void gotoEnhanceEquip(cocos2d::CCObject*);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
private:
    SettlementDelegate * m_pDelegate;
    settlement::SettlementInfoPve *m_pInfo;
    
private:
    WarriorHeadMid          *m_pWarriorNode[FORMATION_POS_MAX];
    DButton                 *m_pReplay;
	DButton                 *m_pDBClose;
    
    cocos2d::CCLabelBMFont  *m_pExp;
    cocos2d::CCLabelBMFont  *m_pGold;
    cocos2d::CCSprite       *m_pLose;
    cocos2d::CCSprite       *m_pWin;
    
    cocos2d::CCMenuItem     *m_pStar[BATTLE_GRADE_COUNT];
    
    cocos2d::CCNode         *m_pEffectNode;
    
    cocos2d::CCNode         *m_pUpgrade;
    cocos2d::CCNode         *m_pWarriors;
    
    cocos2d::CCLabelBMFont     *m_pMapName;
    
    cocos2d::CCNode         *m_pMapEffectNode;
	cocos2d::CCNode         *m_pExpFirst;

	cocos2d::CCMenuItem     *m_pClose;
	cocos2d::CCMenuItemImage     *m_pLoseMenu1;
	cocos2d::CCMenuItemImage     *m_pLoseMenu2;
	cocos2d::CCMenuItemImage     *m_pLoseMenu3;
	cocos2d::CCMenuItemImage     *m_pLoseMenu4;

	void callBackUpgrade(CCObject *sender);
	void goToFormationView(CCObject *sender);
	void goToMallView(CCObject *sender);
};


#endif /* defined(__QSMY__SettlementViewPve__) */
