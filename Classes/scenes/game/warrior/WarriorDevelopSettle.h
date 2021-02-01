//
//  DevelopSettle.h
//  培养结算界面
//  Created by cll on 13-5-22.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__WARRIORDEVELOPSETTLE__
#define __QSMY__WARRIORDEVELOPSETTLE__

#include "AppInclude.h"
#include "../common/common.h"
#include "model/WarriorModel.h"
#include "WarriorDelegate.h"


class WarriorDevelopSettleView
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
{
    
    CC_SYNTHESIZE(WarriorDelegate*, m_pDelegate, Delegate);
public:
	WarriorDevelopSettleView();
	~WarriorDevelopSettleView();

	CREATE_FUNC(WarriorDevelopSettleView);

	virtual bool init();

	void setWarrior(warrior::WarriorCurInfo * pWarrior)
	{
		m_pWarriorInfo = pWarrior;
	}
    
	virtual void onEnter(); 
	virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual void onBackKey();
private:
	void updateWarriorSettle();
	void submitCallBack(cocos2d::CCObject * obj);
    void closeCallBack(cocos2d::CCObject * obj);
    void close(const Json::Value &requestData, const Json::Value &responseData);
    
    void showAtt(cocos2d::CCNode* pAniNode,cocos2d::CCLabelBMFont* pLabel,int val);
private:
	warrior::WarriorCurInfo * m_pWarriorInfo;
	WarriorModel * m_pWarriorModel;
    
private:
    cocos2d::CCNode			*m_pContainer;      //弟子头像
    
	cocos2d::CCLabelBMFont	*m_pAttack;
	cocos2d::CCLabelBMFont	*m_pDefence;
	//cocos2d::CCLabelBMFont	*m_pSpeed;
	cocos2d::CCLabelBMFont	*m_pHp;
    
    
    cocos2d::CCLabelBMFont	*m_pAttackAdd;
	cocos2d::CCLabelBMFont	*m_pDefenceAdd;
	cocos2d::CCLabelBMFont	*m_pHpAdd;
	//cocos2d::CCLabelBMFont	*m_pSpeedAdd;
    
    cocos2d::CCNode         *m_pAttackAni;
	cocos2d::CCNode         *m_pDefenceAni;
	cocos2d::CCNode         *m_pHpAni;
	//cocos2d::CCNode         *m_pSpeedAni;
    
	cocos2d::CCSprite		*m_pGrade;			//品阶
	cocos2d::CCLabelBMFont	*m_pLevel;
	cocos2d::CCLabelTTF		*m_pName;
    
    
    DButton                 *m_pGiveup;
    DButton                 *m_pSubmit;
    
    cocos2d::CCLabelBMFont	*m_pCurPot;
    cocos2d::CCLabelBMFont	*m_pPotCost;
    cocos2d::CCLabelBMFont	*m_pItemNum;
    cocos2d::CCLabelBMFont	*m_pItemCost;
    
    WarriorTypeMark         *m_pWarriorTypeMark;
    
    cocos2d::CCNode         *m_pEffectNode;
};



#endif /* defined(__QSMY__WARRIORDEVELOPSETTLE__) */
