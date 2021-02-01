//
//  SkillUpgrade.h
//  弟子武功修炼
//  Created by cll on 13-5-29.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__SKILLUPGRADE__
#define __QSMY__SKILLUPGRADE__

#include "AppInclude.h"
#include "components/components.h"
#include "model/SkillModel.h"
#include "model/WarriorModel.h"
#include "model/ItemModel.h"
#include "../common/common.h"
#include "DragonBone.h"
#include "../warrior/WarriorDelegate.h"



class FormationDelegate;

class SkillUpgradeView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    
public:
	SkillUpgradeView();
	virtual ~SkillUpgradeView();

	CREATE_FUNC(SkillUpgradeView);
	virtual bool init();
	virtual void onEnter(); 
	virtual void onExit();

	void setDelegate(WarriorDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

	void setWarriorInfo(warrior::WarriorCurInfo *);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    

private:
	
    void binding();
	void updateSkillView();
	void menuCallBack(cocos2d::CCObject * obj);
    void closeCallBack(cocos2d::CCObject* obj);
    void upgradeCallBack(const Json::Value &requestData,const Json::Value &responseData);
    void animateEnd(sp::Armature * target, const char* event,const char* data);
private:
	WarriorDelegate       *m_pDelegate;
    
	SkillModel              *m_pSkillModel;
	WarriorModel            *m_pWarriorModel;
	warrior::WarriorCurInfo *m_pWarriorInfo;
    sp::Armature            *m_pEffect;
    
private:

	cocos2d::CCLabelBMFont			*m_pCurUpLevel;		//突破等级
	cocos2d::CCLabelBMFont			*m_pToUpLevel;		//突破等级
	cocos2d::CCLabelBMFont			*m_pBigSkillLevel; //大技能框上显示的等级
    
    cocos2d::CCLabelBMFont  *m_pCurProps;       //当前属性
    cocos2d::CCLabelBMFont  *m_pToProps;        //提升后属性
    
    //cocos2d::CCLabelBMFont  *m_pCurItems;
    DButton                 *m_pSubmitButton;   //提交按钮
	DButton				  *m_pReturnButton;
	DButton				  *m_pCloseButton;
    
	//Item info
	cocos2d::CCNode*					m_pIconContainer;
	cocos2d::CCLabelTTF*			m_pItemName;
	cocos2d::CCLabelBMFont*	m_pItemNeedCount;
	cocos2d::CCLabelBMFont*	m_pItemCurrentCount;
   // ItemBox                 *m_pItemBox;
    SkillBox                *m_pSkillBox;
    
    cocos2d::CCNode         *m_pEffectNode;

	cocos2d::CCNode         *m_pSkillType[skill::kSkillTypes];
};



#endif /* defined(__QSMY__SKILLUPGRADE__) */
