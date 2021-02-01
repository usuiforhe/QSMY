//
//  RushSettle.h
//  扫荡结算
//
//  Created by wanghejun on 13-9-5.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__RushSettle__
#define __QSMY__RushSettle__

#include "AppInclude.h"
#include "../common/common.h"
#include "model/SettlementModel.h"

class RushSettle
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
{
    
    CC_SYNTHESIZE_READONLY(settlement::SettlementInfoPve*, m_pInfo, Info);
    
public:
    RushSettle();
    virtual ~RushSettle();
    CREATE_FUNC(RushSettle);
    virtual bool init();
    
    void setInfo(settlement::SettlementInfoPve* pInfo);
    
    void onEnter();
    void onExit();
    D_POPUP_PRIORITY(DPOPUP_RUSH_SETTLE_PRIORITY);
private:
    
    void updateView();
    void closeCallBack(cocos2d::CCObject*);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
private:
    WarriorHeadMid          *m_pWarriorNode[FORMATION_POS_MAX];
    DButton                 *m_pQuit;
    
    cocos2d::CCLabelBMFont  *m_pExp;
    cocos2d::CCLabelBMFont  *m_pGold;

	cocos2d::CCMenuItem     *m_pClose;
	cocos2d::CCSprite       *m_pLose;
	cocos2d::CCSprite       *m_pWin;
	cocos2d::CCSprite       *m_pSaodang;
	cocos2d::CCNode         *m_pUpgrade;
	cocos2d::CCNode         *m_pWarriors;
	cocos2d::CCNode         *m_pReplayNode;
	cocos2d::CCMenu        *m_pStarMenu;
	cocos2d::CCLabelBMFont	*m_pMapName;
};

#endif /* defined(__QSMY__RushSettle__) */
