//
//  SkillBox.h
//  技能框
//
//  Created by cll on 13-7-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SkillBox__
#define __QSMY__SkillBox__

#include "AppInclude.h"
#include "model/SkillModel.h"
#include "model/WarriorModel.h"

class SkillBox
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE_READONLY(cocos2d::CCNodeRGBA*, m_pSkillGrade, SkillGrade);		//品阶
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pSkillSprite, SkillSprite);						//图标
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pSkillName, SkillName);						//名
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pSkillUpLevel, SkillUpLevel);						//突破等级
	CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pTouchMenu, TouchMenu);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pSkillLevel, SkillLevel);                  //技能级别
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pUnForce, UnForce);                             //未激活标记
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pNameBg, NameBg);
    
    CC_SYNTHESIZE_READONLY(skill::Skill*, m_pSkillInfo, SkillInfo);
	
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pNoneSkill, NoneSkill);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pCanEnhance, CanEnhance);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pNodeLevel, NodeLevel);      //tolua
	CC_SYNTHESIZE(bool, m_bShowCanEnhance, ShowCanEnhance);		//tolua

	//阵容界面用 技能框显示技能等级
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pLevel, Level);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pLevelNode, LevelNode);
public:
    CREATE_FUNC(SkillBox);
    SkillBox();
    virtual ~SkillBox();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void updateView(skill::Skill *pSkill,int8_t level);
	void updateView(skill::Skill *pSkill,warrior::WarriorCurInfo *pWarrior);
    void reset();
};


class SkillBoxLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SkillBoxLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SkillBox);
};

#endif /* defined(__QSMY__SkillBox__) */
