//
//  SoulHunterWarriorView.cpp
//  狩魂单个弟子视图
//
//  Created by liuxiaogang on 13-10-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoulHunterWarriorView.h"
#include "managers/DataManager.h"
#include "utils/TimeUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;

SoulHunterWarriorView::SoulHunterWarriorView()
:m_pBgGrade(NULL)
,m_pSoulNumBg(NULL)
,m_pAvatorContainer(NULL)
,m_pName(NULL)
,m_pSoulNum(NULL)
,m_pProtectLeftTime(NULL)
,m_pProtectOn(NULL)
,m_pSnatchMenuItem(NULL)
,m_pWarriorInfo(NULL)
,m_pEffectNode(NULL)
{
    
}


SoulHunterWarriorView::~SoulHunterWarriorView()
{
	CC_SAFE_RELEASE(m_pBgGrade);
	CC_SAFE_RELEASE(m_pSoulNumBg);
	CC_SAFE_RELEASE(m_pAvatorContainer);
    CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pSoulNum);
    CC_SAFE_RELEASE(m_pProtectLeftTime);
    CC_SAFE_RELEASE(m_pProtectOn);
	CC_SAFE_RELEASE(m_pSnatchMenuItem);
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
	CC_SAFE_RELEASE(m_pEffectNode);
}


bool SoulHunterWarriorView::init()
{
	if(!CCNode::init())
	{
		return false;
	}

    return true;
}


bool SoulHunterWarriorView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgGrade",  cocos2d::CCSprite*, this->m_pBgGrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoulNumBg",  cocos2d::CCSprite*, this->m_pSoulNumBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvatorContainer",  cocos2d::CCNode*, this->m_pAvatorContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", cocos2d::CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoulNum", cocos2d::CCLabelBMFont *, this->m_pSoulNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProtectLeftTime", CCLabelBMFont *, this->m_pProtectLeftTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProtectOn", CCNode *, this->m_pProtectOn); 
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSnatchMenuItem", CCMenuItem *, this->m_pSnatchMenuItem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode *, this->m_pEffectNode);
    return false;
}



void SoulHunterWarriorView::setData(warrior::WarriorCurInfo * pWarrior)
{
    if(m_pAvatorContainer) m_pAvatorContainer->removeAllChildren();
    CC_SAFE_RETAIN(pWarrior);
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
    m_pWarriorInfo = pWarrior;
    
    if(m_pWarriorInfo==NULL)
    {
        //setVisible(false);
        m_pSnatchMenuItem->setVisible(false);
		m_pBgGrade->setVisible(false);
		m_pAvatorContainer->setVisible(false);
		m_pProtectOn->setVisible(false);
		m_pSoulNum->setVisible(false);
		m_pSoulNumBg->setVisible(false);
        m_pName->setString("? ? ?");
        return;
    }
	else
	{
		m_pProtectOn->setVisible(true);
		m_pSoulNum->setVisible(true);
		m_pSoulNumBg->setVisible(true);
        m_pSnatchMenuItem->setVisible(true);
		m_pBgGrade->setVisible(true);
		m_pAvatorContainer->setVisible(true);
		BINDING_EXEC(m_pWarriorInfo, this, SoulHunterWarriorView::updateView, D_EVENT_WARRIORINFO_UPDATE);
		m_pBgGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(pWarrior->getWid()));
		m_pAvatorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50ForSoulHunter(pWarrior->getAvatarId()));
		m_pName->setString(CCString::createWithFormat("%s",pWarrior->getBaseInfo()->getWarriorName().c_str())->getCString());
		setVisible(true); 
	}
}

void SoulHunterWarriorView::updateView()
{
	warrior::SoulInfo* pSoulInfo = DM_GET_WARRIOR_MODEL->getSoulInfoByWID(m_pWarriorInfo->getWid());
    
	if(pSoulInfo==NULL)
	{
		m_pSoulNum->setString("0");
		m_pProtectOn->setVisible(false);
	}else
	{
		m_pSoulNum->setVisible(true);
		m_pSoulNumBg->setVisible(true);
		m_pSoulNum->setString(CCString::createWithFormat("%d",pSoulInfo->getNum())->getCString());
		m_pProtectLeftTime->setString(TimeUtil::getSeconds2String(pSoulInfo->getHpts(),"%H:%M"));
		m_pProtectOn->setVisible(pSoulInfo->getHpts()>0);
	}	
}

void SoulHunterWarriorView::onEnter()
{
	m_pEffectNode->removeAllChildren();
	sp::Armature *pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("soul");
	m_pEffectNode->addChild(pArmature);
	CCNode::onEnter();
}

void SoulHunterWarriorView::onExit()
{
    m_pEffectNode->removeAllChildren();
    CCNode::onExit();
}


