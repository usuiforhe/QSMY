//
//  WarriorTransForceSettle.h
//  传功结算界面
//  Created by cll on 13-5-22.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__WARRIORTRANSFORCESETTLE__
#define __QSMY__WARRIORTRANSFORCESETTLE__

#include "AppInclude.h"
#include "../common/common.h"
#include "../../../model/WarriorModel.h"

class WarriorDelegate;


class WarriorTransForceSettleView
:public DLayerModal
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
public:
    
	WarriorTransForceSettleView();
	~WarriorTransForceSettleView();

	CREATE_FUNC(WarriorTransForceSettleView);

	virtual bool init();

	void setWarrior(warrior::WarriorCurInfo * pWarrior)
	{
		m_pWarriorInfo = pWarrior;
	}

	void setDelegate(WarriorDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
	
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
	virtual void onEnter(); 

	virtual void onExit();

private:

	void updateWarriorSettle();

	void menuCallBack(cocos2d::CCObject * obj);

private:
    
	WarriorDelegate * m_pDelegate;
    
	warrior::WarriorCurInfo * m_pWarriorInfo;

	WarriorModel * m_pWarriorModel;
    
private:
    
    cocos2d::CCLabelTTF*                m_pName;
    cocos2d::CCLabelBMFont*             m_pLevel;
    cocos2d::CCLabelBMFont*             m_pSpeed;
    cocos2d::CCLabelBMFont*             m_pHp;
    cocos2d::CCLabelBMFont*             m_pDefence;
    cocos2d::CCLabelBMFont*             m_pAttack;
    cocos2d::CCNode*                    m_pContainer;
    cocos2d::CCNode*                    m_pEffectNode;
    cocos2d::CCSprite*                  m_pGrade;
    WarriorTypeMark*                    m_pWarriorTypeMark;
    
};



#endif /* defined(__QSMY__WARRIORTRANSFORCESETTLE__) */
