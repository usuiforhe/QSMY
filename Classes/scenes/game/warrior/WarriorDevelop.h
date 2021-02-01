//
//  WarriorDevelop.h
//  弟子培养界面
//  Created by cll on 13-5-22.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__WARRIORDEVELOP__
#define __QSMY__WARRIORDEVELOP__

#include "AppInclude.h"
#include "../common/common.h"
#include "../../../model/WarriorModel.h"


class WarriorDelegate;
class WarriorGradeBg;

class WarriorDevelopView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{

public:
	WarriorDevelopView();
	~WarriorDevelopView();

	CREATE_FUNC(WarriorDevelopView);
    
	virtual bool init();

	virtual void onEnter(); 

	virtual void onExit();

	void setWarrior(warrior::WarriorCurInfo * pWarrior);

	void setDelegate(WarriorDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

private:
	void binding();

	void changeDevelopButtonState(bool isReset);

	void submitCallback(const Json::Value &requestData,const Json::Value &responseData);
	void confirmDevelopCallback( const Json::Value &requestData, const Json::Value &responseData );
	void updateWarriorDevelop();
	
	void BottomButtonsCallBack(cocos2d::CCObject * obj);
    void developOneButtonCallBack(cocos2d::CCObject * obj);
	void developTenButtonCallBack(cocos2d::CCObject * obj);
    void closeCallBack(cocos2d::CCObject * obj);
	void giveUpCallBack(cocos2d::CCObject * obj);
	void confirmButtonCallBack(cocos2d::CCObject* obj);
    void selectCallBack(cocos2d::CCObject* obj);
    void developWarrior();
	void setDevelopResultInfoVisible(bool visible);
	void updateDevelopResult();

    void updateSelectButtons();
private:
	WarriorDelegate *m_pDelegate;
	WarriorModel    *m_pWarriorModel;
private:
    cocos2d::CCNode			*m_pContainer;      //弟子头像
	cocos2d::CCLabelBMFont	*m_pUpLevel;		//突破等级
    WarriorGradeBg          *m_pWarriorGradeBg;
	cocos2d::CCLabelTTF	*m_pAttack;
	cocos2d::CCLabelTTF	*m_pDefence;
	cocos2d::CCLabelTTF	*m_pSpeed;
	cocos2d::CCLabelTTF	*m_pHp;
	cocos2d::CCLabelTTF	*m_pAddedAttack;
	cocos2d::CCLabelTTF	*m_pAddedDefence;
	cocos2d::CCLabelTTF	*m_pAddedSpeed;
	cocos2d::CCLabelTTF	*m_pAddedHp;

	cocos2d::CCSprite				*m_pAttackUpArrow;			//培养后属性变化箭头
	cocos2d::CCSprite				*m_pAttackDownArrow;
	cocos2d::CCSprite				*m_pDefenceUpArrow;
	cocos2d::CCSprite				*m_pDefenceDownArrow;
	cocos2d::CCSprite				*m_pSpeedUpArrow;
	cocos2d::CCSprite				*m_pSpeedDownArrow;
	cocos2d::CCSprite				*m_pHpUpArrow;
	cocos2d::CCSprite				*m_pHpDownArrow;

	cocos2d::CCSprite				*m_pGrade;			//品阶
	cocos2d::CCLabelBMFont	*m_pLevel;
	cocos2d::CCLabelTTF		*m_pName;
	cocos2d::CCLabelBMFont *m_pFight;
    
    
	cocos2d::CCLabelTTF  *m_pPot;            //潜力
	cocos2d::CCLabelTTF  *m_pPill;           //培养丹
	cocos2d::CCLabelTTF  *m_pExpendPot;
	cocos2d::CCLabelTTF  *m_pExpendPill;
	cocos2d::CCLabelBMFont  *m_pPointsCost;		//消耗点券
    
    
    DButton                 *m_pSelectButtons[warrior::PotBuff::kTypeSpecial];
	DButton				  *m_pCloseButton;
	DButton                 *m_pLeftButton;
	DButton                 *m_pMiddleButton;
	DButton                 *m_pRightButton;

	std::map<unsigned int, cocos2d::CCSprite*> m_buttonTextMap;

    WarriorTypeMark         *m_pWarriorTypeMark;
	
	cocos2d::CCNode         *m_pEffectNode;

	cocos2d::CCNode	*m_pColorGrade[WARRIOR_GRADE_NUM];

private:
	warrior::WarriorCurInfo * m_pWarriorInfo;
	warrior::PotBuff::PotbuffType  m_DevType;		//培养类型

	//todo
	bool		m_isWiattingForConfirm;
};

#endif /* defined(__QSMY__WARRIORDEVELOP__) */
