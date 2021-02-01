//
//  SkillInfo.h
//	弟子武功信息
//  Created by cll on 13-5-29.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__SKILLINFO__
#define __QSMY__SKILLINFO__

#include "AppInclude.h"
#include "components/components.h"
#include "model/SkillModel.h"
#include "model/WarriorModel.h"
#include "../common/common.h"
#include "../warrior/WarriorDelegate.h"


class FormationDelegate;
class SkillInfo;

class SkillInfoView :public DLayer
{
public:
	SkillInfoView();
	virtual ~SkillInfoView();
	CREATE_FUNC(SkillInfoView);
	bool init();
	
	void setDelegate(WarriorDelegate* pDelegate);
	void setSkillInfo(warrior::WarriorCurInfo* pWarrior, const uint32_t &sid);
	//查看技能介绍，不能修炼
	void setSkillViewOnly(bool bViewOnly = true);
private:
    SkillInfo*          m_pSkillInfo;
    SkillInfo*          m_pSkillInfoSP;
};



class SkillInfo
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
public:
	SkillInfo();
	virtual ~SkillInfo();
    
	bool init();
	virtual void onEnter();
	virtual void onExit();
    
	//查看技能介绍，不能修炼
	void setViewOnly(bool bViewOnly);

	void setDelegate(WarriorDelegate* pDelegate);
	void setWarrior(warrior::WarriorCurInfo* pWarrior);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
private:
    void _updateSkillInfo();
protected:
	virtual void updateSkillInfo();
	virtual void menuCallBack(cocos2d::CCObject * obj);
    void closeCallBack(cocos2d::CCObject * obj);
    virtual bool loadUI();
    void binding();
    
protected:
	WarriorDelegate           *m_pDelegate;
	SkillModel                  *m_pSkillModel;
	WarriorModel                *m_pWarriorModel;
    warrior::WarriorCurInfo     *m_pWarriorInfo;
    
    
protected:
    DButton                 *m_pSubmitButton;
	DButton				  *m_pCloseButton;
    
    SkillBox                *m_pSkillBox;
    cocos2d::CCLabelTTF     *m_pDesc;
    cocos2d::CCLabelTTF     *m_pStory;
    cocos2d::CCLabelTTF     *m_pTriggerProbability;
    cocos2d::CCLabelTTF     *m_pZone;
	cocos2d::CCLabelBMFont* m_pBigSkillLevel; //大技能框上显示的等级
    cocos2d::CCNode*				  m_pNodeLevel;
    cocos2d::CCLabelBMFont  *m_pAttackRatio;
    
    /*cocos2d::CCNode         *m_pCloseLabel;
    cocos2d::CCNode         *m_pSubmitLabel;*/
    
    cocos2d::CCSprite       *m_pSkillType[skill::kSkillTypes];
};

class SkillInfoSP :public SkillInfo
{
public:
    SkillInfoSP();
    virtual ~SkillInfoSP();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
protected:
    virtual bool loadUI();
    virtual void menuCallBack(cocos2d::CCObject * obj);
    virtual void updateSkillInfo();
protected:
    
    cocos2d::extension::CCData  *m_pSPData;
    cocos2d::CCLabelTTF  *m_pWarNum;
    cocos2d::CCNode         *m_pWarContainer;
    
};


#endif /* defined(__QSMY__SKILLINFO__) */
