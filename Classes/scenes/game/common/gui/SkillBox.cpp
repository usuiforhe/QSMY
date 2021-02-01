//
//  SkillBox.cpp
//  
//
//  Created by cll on 13-7-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SkillBox.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"


USING_NS_CC;
USING_NS_CC_EXT;

SkillBox::SkillBox()
:m_pSkillName(NULL)
,m_pSkillGrade(NULL)
,m_pSkillSprite(NULL)
,m_pTouchMenu(NULL)
,m_pSkillUpLevel(NULL)
,m_pSkillLevel(NULL)
,m_pUnForce(NULL)
,m_pNameBg(NULL)
,m_pSkillInfo(NULL)
,m_pCanEnhance(NULL)
,m_pNodeLevel(NULL)
,m_bShowCanEnhance(true)
,m_pNoneSkill(NULL)
,m_pLevel(NULL)
,m_pLevelNode(NULL)
{
    
}


SkillBox::~SkillBox()
{
	CC_SAFE_RELEASE(m_pSkillSprite);
    CC_SAFE_RELEASE(m_pSkillName);
    CC_SAFE_RELEASE(m_pSkillGrade);
    CC_SAFE_RELEASE(m_pSkillUpLevel);
	CC_SAFE_RELEASE(m_pTouchMenu);
    CC_SAFE_RELEASE(m_pSkillLevel);
    CC_SAFE_RELEASE(m_pUnForce);
    CC_SAFE_RELEASE(m_pNameBg);
	CC_SAFE_RELEASE(m_pSkillInfo);
	CC_SAFE_RELEASE(m_pCanEnhance);
	CC_SAFE_RELEASE(m_pNodeLevel);
	CC_SAFE_RELEASE(m_pNoneSkill);
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pLevelNode);
}


bool SkillBox::init()
{
	if(!CCNode::init())
	{
		return false;
	}
	return true;
}

bool SkillBox::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillGrade", cocos2d::CCNodeRGBA *, this->m_pSkillGrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillSprite", CCNode *, this->m_pSkillSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchMenu", CCMenuItem *, this->m_pTouchMenu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillName", CCLabelBMFont *, this->m_pSkillName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillUpLevel", CCNode *, this->m_pSkillUpLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillLevel", CCLabelBMFont *, this->m_pSkillLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUnForce", CCSprite *, this->m_pUnForce);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameBg", CCNode *, this->m_pNameBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCanEnhance", CCNode *, this->m_pCanEnhance);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeLevel", CCNode *, this->m_pNodeLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoneSkill", CCNode *, this->m_pNoneSkill);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelNode", CCNode *, this->m_pLevelNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    
    return false;
}

void SkillBox::reset()
{
    CC_SAFE_RELEASE_NULL(m_pSkillInfo);
    if(m_pNameBg) m_pNameBg->setVisible(false);
    if(m_pSkillSprite)
    {
       m_pSkillSprite->setVisible(false);
    }
    if(m_pSkillName)   m_pSkillName->setVisible(false);
    if(m_pSkillGrade) m_pSkillGrade->setVisible(false);
    if(m_pSkillUpLevel) m_pSkillUpLevel->setVisible(false);
    if(m_pSkillLevel) m_pSkillLevel->setVisible(false);
	if(m_pUnForce)m_pUnForce->setVisible(false);
	if(m_pCanEnhance) m_pCanEnhance->setVisible(false);
	if(m_pNodeLevel) m_pNodeLevel->setVisible(false);
	if(m_pLevelNode) m_pLevelNode->setVisible(false);

}

void SkillBox::updateView(skill::Skill *pSkill,int8_t level)
{
    if(m_pSkillSprite) m_pSkillSprite->removeAllChildren();
    if(m_pSkillUpLevel) m_pSkillUpLevel->removeAllChildren();
	SkillModel *  pSkillModel = DM_GET_SKILL_MODEL;
    CC_SAFE_RETAIN(pSkill);
    CC_SAFE_RELEASE(m_pSkillInfo);
    m_pSkillInfo = pSkill;
    
    if(!pSkill)
    {
      reset();
	  if(m_pNoneSkill)m_pNoneSkill->setVisible(true);
      return;  
    }
    
    if(m_pNameBg) m_pNameBg->setVisible(true);
        
    if(m_pSkillSprite)
    {
        m_pSkillSprite->setVisible(true);
        
        if(pSkill->getSid() > 0)
        {
            CCSprite *pIcon = ResourceManager::sharedResourceManager()->getSkillIcon(pSkill->getSid());
			if (level < 0)
			{
				pIcon->setShaderProgram(D_SHADER_MASK_GRAY);
			}
			
            pIcon->setScale(0.9f);
            if (pIcon)	m_pSkillSprite->addChild(pIcon);
        }
    }

	if(m_pSkillName)
    {
        m_pSkillName->setVisible(true);
        m_pSkillName->setString(pSkill->name.c_str());
    }
    
    if(m_pSkillGrade)
    {
        m_pSkillGrade->setVisible(true);
        m_pSkillGrade->setColor(pSkillModel->getSkillColor(pSkill->grade));
    }


	if(m_pCanEnhance) 
	{
		m_pCanEnhance->setVisible(false);
	}
    if(level<0)
    {
        if(m_pSkillUpLevel) m_pSkillUpLevel->setVisible(false);
        if(m_pSkillLevel) m_pSkillLevel->setVisible(false);
        if(m_pUnForce)m_pUnForce->setVisible(pSkill->getSid() > 0);
        if(m_pNodeLevel) m_pNodeLevel->setVisible(false);
		if(m_pLevelNode) m_pLevelNode->setVisible(false);
        return;
    }else
    {
        if(m_pUnForce) m_pUnForce->setVisible(false);
        if(m_pNodeLevel) m_pNodeLevel->setVisible(true);
    }
    if(pSkill->isSpecial())
    {
        if(m_pSkillUpLevel) m_pSkillUpLevel->setVisible(true);
        if(m_pSkillLevel) m_pSkillLevel->setVisible(false);
        if(m_pNodeLevel) m_pNodeLevel->setVisible(false);
    }else
    {
        if(m_pSkillUpLevel) m_pSkillUpLevel->setVisible(false);
        if(m_pSkillLevel) m_pSkillLevel->setVisible(true);
    }
    
	if(m_pSkillUpLevel)
    {
       m_pSkillUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(level));
    }
    
    if(m_pSkillLevel)
    {
        m_pSkillLevel->setString(CCString::createWithFormat("%d",level)->getCString());
    }

	if(m_pLevel && m_pLevelNode && level){
		//主界面技能等级显示
		m_pLevelNode->setVisible(true);
		m_pLevel->setString(CCString::createWithFormat("%d",level)->getCString());
	}
}


void SkillBox::updateView(skill::Skill *pSkill,warrior::WarriorCurInfo *pWarrior)
{
	updateView(pSkill, pWarrior->getSlv());
	if (m_pCanEnhance && pSkill)
	{
		uint32_t itemNum = DM_GET_ITEM_MODEL->getItemNum(ITEM_NEILIDAN_ID);
		uint32_t needNum = DM_GET_SKILL_MODEL->getSkillUpExp(pSkill->grade, pWarrior->getSlv()) - pWarrior->getSexp();
		m_pCanEnhance->setVisible(pWarrior->getSlv() < SKILL_LV_MAX && needNum <= itemNum && m_bShowCanEnhance);
	}

}