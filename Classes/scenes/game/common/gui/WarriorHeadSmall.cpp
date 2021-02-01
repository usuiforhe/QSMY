//
//  WarriorHeadSmall.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "WarriorHeadSmall.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

WarriorHeadSmall::WarriorHeadSmall()
:m_pContainer(NULL)
,m_pGrade(NULL)
,m_pBorder(NULL)
,m_pStars(NULL)
,m_pTfLevel(NULL)
,m_pLevelBg(NULL)
,m_pLevel(NULL)
,m_pHead(NULL)
,m_pDefault(NULL)
,m_pName(NULL)
,m_pUpLevel(NULL)
,m_pHas(NULL)
,m_pNotInBattle(NULL)
,m_pTouchToChange(NULL)
,m_pMenuItem(NULL)
,m_pAddBurn(NULL)
{
    
}


WarriorHeadSmall::~WarriorHeadSmall()
{
    CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pBorder);
    CC_SAFE_RELEASE(m_pStars);
    
    CC_SAFE_RELEASE(m_pTfLevel);
    CC_SAFE_RELEASE(m_pLevelBg);
    CC_SAFE_RELEASE(m_pLevel);
    
    CC_SAFE_RELEASE(m_pHead);
    CC_SAFE_RELEASE(m_pDefault);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pUpLevel);
    CC_SAFE_RELEASE(m_pHas);
	CC_SAFE_RELEASE(m_pNotInBattle);
	CC_SAFE_RELEASE(m_pTouchToChange);

	CC_SAFE_RELEASE(m_pMenuItem);
	CC_SAFE_RELEASE(m_pAddBurn);
}


bool WarriorHeadSmall::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBorderHighLight", CCNode *, this->m_pBorderHighLight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBorder", CCNode *, this->m_pBorder);
	
    /*CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStars", CCNode *, this->m_pStars);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfLevel", CCLabelBMFont *, this->m_pTfLevel);    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelBg", CCSprite *, this->m_pLevelBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCNode *, this->m_pLevel);*/
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefault", CCSprite *, this->m_pDefault);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHead", CCNode *, this->m_pHead);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNotInBattle", CCSprite*, this->m_pNotInBattle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchToChange", CCSprite*, this->m_pTouchToChange);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    /*CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCNode *, this->m_pUpLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHas", CCSprite *, this->m_pHas);*/
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuItem", CCMenuItem *, this->m_pMenuItem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddBurn", CCNode *, this->m_pAddBurn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade" ,  WarriorHeadGradeBg* ,m_pGrade);

    if(m_pBorder) m_pBorder->setVisible(true);
	//if(m_pNotInBattle) m_pNotInBattle->setVisible(true);
    return false;
}

void WarriorHeadSmall::resetWarriorInfo()
{
    if(m_pContainer) m_pContainer->removeAllChildren();
    if(m_pDefault) m_pDefault->setVisible(true);
    if(m_pHead) m_pHead->setVisible(false);
	if(m_pNotInBattle) m_pNotInBattle->setVisible(true);
	if(m_pGrade) m_pGrade->setVisible(false);
}

void WarriorHeadSmall::setWarriorInfoOnBurn(warrior::WarriorCurInfo* pWarriorInfo)
{
	setWarriorInfo(pWarriorInfo);
	m_pAddBurn->setVisible(pWarriorInfo==NULL);
	m_pTouchToChange->setVisible(false);
	m_pDefault->setScale(1.1f);
	m_pDefault->setVisible(true);
}

void WarriorHeadSmall::setWarriorInfo(warrior::WarriorCurInfo* pWarriorInfo)
{
    if(pWarriorInfo==NULL)
    {
        resetWarriorInfo();
        return;
    }
    updateWarriorInfo(pWarriorInfo->getBaseInfo());
    if(m_pContainer) m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorHeadIcon(pWarriorInfo->getAvatarId()));
}

void WarriorHeadSmall::setWarriorInfo(warrior::WarriorBaseInfo* pWarriorInfo)
{
    updateWarriorInfo(pWarriorInfo);
    if(m_pContainer && pWarriorInfo) m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorHeadIcon(pWarriorInfo->getWarriorId()));
}

void WarriorHeadSmall::setWarriorInfo(warrior::WarriorBaseInfo* pWarriorInfo,uint32_t avtarId)
{
    updateWarriorInfo(pWarriorInfo);
    if(m_pContainer) m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorHeadIcon(avtarId));
}

void WarriorHeadSmall::updateWarriorInfo(warrior::WarriorBaseInfo* pWarriorInfo)
{
    if(m_pContainer) m_pContainer->removeAllChildren();
    if(pWarriorInfo==NULL)
    {
        resetWarriorInfo();
        return;
    }
    if(m_pNotInBattle) m_pNotInBattle->setVisible(false);
	if(m_pTouchToChange) m_pTouchToChange->setVisible(false);
    if(m_pDefault) m_pDefault->setVisible(false);
    if(m_pHead) m_pHead->setVisible(true);
    
    
    //if(m_pGrade) m_pGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(pWarriorInfo->getWarriorId()));
	if(m_pGrade)
	{
		m_pGrade->setVisible(true);
		m_pGrade->setWarriorGrade(pWarriorInfo->getColorGrade());
	}
    
    //if(m_pBorderHighLight) m_pBorderHighLight->setVisible(pWarriorInfo->getColorGrade() > warrior::kWarGradeBlue);
    //if(m_pBorder) m_pBorder->setVisible(pWarriorInfo->getColorGrade() <= warrior::kWarGradeBlue);
    if(m_pStars) m_pStars->setVisible(pWarriorInfo->getColorGrade() > warrior::kWarGradePurple);
    if(m_pLevelBg) m_pLevelBg->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(pWarriorInfo->getWarriorId()));
    if(m_pName) m_pName->setString(pWarriorInfo->getWarriorName().c_str());
}

