//
//  SettlementViewPvp.h
//  QSMY
//
//  Created by wanghejun on 13-8-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SettlementViewPvp__
#define __QSMY__SettlementViewPvp__

#include "SettlementView.h"
#include "../../common/common.h"

class SettlementViewPvp
:public SettlementView
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    SettlementViewPvp();
    virtual ~SettlementViewPvp();    
    virtual bool init(SettlementDelegate *delegate,settlement::SettlementInfo *pInfo);
    virtual void closeView();
private:
    void quitBattle(cocos2d::CCObject*);
	void replayBattle(cocos2d::CCObject*);
	void gotoEnhanceEquip(cocos2d::CCObject*);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler					onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler	onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName) {return NULL;}

	void numberAction1(float num);
	void numberAction2(float num);
private:
    SettlementDelegate * m_pDelegate;
    settlement::SettlementInfoPvp *m_pInfo;
    
private:
    DButton                 *m_pReplay;
    
    cocos2d::CCLabelBMFont	*m_pExp;
    cocos2d::CCLabelBMFont	*m_pGold;
    cocos2d::CCNode				*m_pLose;
    cocos2d::CCNode				*m_pWin;

	cocos2d::CCLabelTTF		*m_pNameLeft;
    cocos2d::CCLabelTTF		*m_pNameRight;
	cocos2d::CCLabelBMFont	*m_pFightLeft;
    cocos2d::CCLabelBMFont	*m_pFightRight;
	cocos2d::CCLabelBMFont	*m_pCoin;
	cocos2d::CCLabelBMFont	*m_pScore;
	cocos2d::CCLabelBMFont	*m_pEndu;
	DButton							*m_pButtonLeft1;
	DButton							*m_pButtonLeft2;
	DButton							*m_pButtonRight1;
	DButton							*m_pButtonRight2;

    cocos2d::CCNode         *m_pEffectNode;
    
    cocos2d::CCNode         *m_pUpgrade;
    cocos2d::CCNode         *m_pWarriors;
    
    cocos2d::CCLabelTTF     *m_pMapName;
    
    cocos2d::CCNode         *m_pMapEffectNode;

	cocos2d::CCMenuItem     *m_pClose;

	void callBackUpgrade(CCObject *sender);
	void goToFormationView(CCObject *sender);
	void goToMallView(CCObject *sender);
	void viewOtherFormation(CCObject *sender);
	void cardCallBack(CCObject *sender);
};


#endif /* defined(__QSMY__SettlementViewPvp__) */
