//
//  RecruitSettle.h
//  招募结算界面
//  Created by cll on 13-5-22.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__RECRUITSETTLE__
#define __QSMY__RECRUITSETTLE__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/WarriorModel.h"
#include "../common/common.h"
#include "../Game.h"

class WarriorDelegate;

class RecruitSettleView :public DLayerModal
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public BackKeyInterface
{
public:
	RecruitSettleView();
	~RecruitSettleView();

	CREATE_FUNC(RecruitSettleView);

	virtual bool init();
    
	void setWarrior(warrior::WarriorCurInfo * pWarrior)
	{
		if (m_pWarriorInfo == pWarrior)
		{
			return;
		}
		
		CC_SAFE_RELEASE(m_pWarriorInfo);
		CC_SAFE_RETAIN(pWarrior);
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

	virtual void onBackKey();
    
private:
    
	void updateWarriorSettle();
    
	void menuCallBack(cocos2d::CCObject * obj);
	void viewCallback(cocos2d::CCObject* obj);
	void shareCallback(cocos2d::CCObject* obj);
    
private:    
	WarriorDelegate * m_pDelegate;
    
	warrior::WarriorCurInfo * m_pWarriorInfo;
    
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
	//DButton*							m_pDBtnShare;
	cocos2d::CCMenuItemImage*							m_pDBtnView;
    WarriorTypeMark*                    m_pWarriorTypeMark;
	cocos2d::CCNode*					m_pPopupNode;
	WarriorGradeBg*                     m_pWarriorGradeBg;
};



#endif /* defined(__QSMY__RECRUITSETTLE__) */
