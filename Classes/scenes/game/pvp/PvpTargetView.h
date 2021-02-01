//
//  PvpTargetView.h
//  查看对象
//
//  Created by Eci on 14-8-1.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__PvpTargetView__
#define __QSMY__PvpTargetView__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/PvpModel.h"
#include "../../../model/OtherFormationModel.h"

#include "../common/gui/WarriorHead.h"

struct PvpDelegate;

#pragma mark -------------PvpTargetView-------------
class PvpTargetView
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE(PvpDelegate *, m_pDelegate, Delegate);
public:
    PvpTargetView();

	virtual ~PvpTargetView();
    
	CREATE_FUNC(PvpTargetView);

    
    virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

	void setInfo(pvp::ChTargetInfo *pInfo);
private:
	pvp::ChTargetInfo *m_pInfo;
	otherFormation::FormationInfo* m_pFormationInfo;

	DButton *m_pDBtnClose;

	void callBackClose(CCObject *pObj);

	DButton *m_pDBtnChallenge;

	void callBackChallenge(CCObject *pObj);

	DButton *m_pDBtnView;

	void callBackView(CCObject *pObj);

	cocos2d::CCLabelTTF *m_pTTFName;
	cocos2d::CCLabelBMFont *m_pBMFFight;
	cocos2d::CCLabelBMFont *m_pBMFWin;
	cocos2d::CCLabelBMFont *m_pBMFLose;
	cocos2d::CCLabelBMFont *m_pBMFRate;
	cocos2d::CCLabelBMFont *m_pLevel;

	WarriorHead *m_pWarrior[FORMATION_POS_MAX];

	void updateFormation();
	void callBackDelay();
};

#endif /* defined(__QSMY__PvpTargetView__) */
