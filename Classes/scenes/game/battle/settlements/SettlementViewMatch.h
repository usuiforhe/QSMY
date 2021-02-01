//
//  SettlementViewMatch.h
//  QSMY
//
//  Created by appleos on 14-1-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__SettlementViewMatch__
#define __QSMY__SettlementViewMatch__

#include "SettlementView.h"
#include "../../common/common.h"

#define NODE_MAX 4

class SettlementViewMatch
:public SettlementView
,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    SettlementViewMatch();
    virtual ~SettlementViewMatch();
    virtual bool init(SettlementDelegate *delegate,settlement::SettlementInfo *pInfo);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual void closeView();
    
    void callBackClose(cocos2d::CCObject *sender);
protected:
    SettlementDelegate *m_pDelegate;
    settlement::SettlementInfoMatch *m_pInfo;
private:
    cocos2d::CCNode *m_pNode[NODE_MAX];
    DButton *m_pButton;
    cocos2d::CCNode *m_pContainer;
    CCLabelTTF *m_pLabelName;
    CCLabelBMFont *m_pLabelLevel;
    CCLabelBMFont *m_pLabelRank;

	CCMenuItem	*m_pCloseMenu;
};
#endif /* defined(__QSMY__SettlementViewMatch__) */
