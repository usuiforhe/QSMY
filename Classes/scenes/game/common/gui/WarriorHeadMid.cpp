//
//  WarriorHeadMid.cpp
//  QSMY
//
//  Created by cll on 13-7-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "WarriorHeadMid.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "utils/ActionUtil.h"
#include "model/FormationModel.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace warrior;

WarriorHeadMid::WarriorHeadMid()
:m_pBgColor1Sprite(NULL)
,m_pBgColor2Sprite(NULL)
,m_pBgColor3Sprite(NULL)
,m_pBgColor4Sprite(NULL)
,m_pHeadBgSprite(NULL)
,m_pWarriorHead(NULL)
,m_pWarriorName(NULL)
,m_pUpLevel(NULL)
,m_pGrade(NULL)
,m_pLevel(NULL)
,m_pExpAdd(NULL)
,m_pLevelupEffect(NULL)
,m_pWarriorContainer(NULL)
,m_pExpAddNode(NULL)
,m_pLock(NULL)
,m_pUnlockLevel(NULL)
,m_pUnlock(NULL)
,m_pCanChange(NULL)
,m_bFlash(NULL)
{
    
}


WarriorHeadMid::~WarriorHeadMid()
{
	CC_SAFE_RELEASE(m_pBgColor1Sprite);
	CC_SAFE_RELEASE(m_pBgColor2Sprite);
	CC_SAFE_RELEASE(m_pBgColor3Sprite);
	CC_SAFE_RELEASE(m_pBgColor4Sprite);
    CC_SAFE_RELEASE(m_pHeadBgSprite);
	CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pWarriorHead);
    CC_SAFE_RELEASE(m_pWarriorName);
    CC_SAFE_RELEASE(m_pUpLevel);
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pExpAdd);
	CC_SAFE_RELEASE(m_pLevelupEffect);
	CC_SAFE_RELEASE(m_pWarriorContainer);
    CC_SAFE_RELEASE(m_pExpAddNode);
    
    CC_SAFE_RELEASE(m_pLock);
    CC_SAFE_RELEASE(m_pUnlockLevel);
    CC_SAFE_RELEASE(m_pUnlock);
    CC_SAFE_RELEASE(m_pCanChange);
    CC_SAFE_RELEASE(m_bFlash);
}


bool WarriorHeadMid::init()
{
	if(!CCNode::init())
	{
		return false;
	}

	return true;

}

bool WarriorHeadMid::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgColor1Sprite",  cocos2d::CCNode*, this->m_pBgColor1Sprite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgColor2Sprite",  cocos2d::CCNode*, this->m_pBgColor2Sprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgColor3Sprite",  cocos2d::CCNode*, this->m_pBgColor3Sprite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgColor4Sprite",  cocos2d::CCNode*, this->m_pBgColor4Sprite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHeadBgSprite", cocos2d::CCNodeRGBA *, this->m_pHeadBgSprite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", cocos2d::CCNodeRGBA *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorHead", CCNode *, this->m_pWarriorHead);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorName", CCLabelTTF *, this->m_pWarriorName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCNode *, this->m_pUpLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpAdd", CCLabelBMFont *, this->m_pExpAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelupEffect", CCNode *, this->m_pLevelupEffect);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorContainer", CCNode *, this->m_pWarriorContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpAddNode", CCNode *, this->m_pExpAddNode);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUnlockLevel", CCLabelBMFont *, this->m_pUnlockLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUnlockLevel", CCLabelTTF *, this->m_pUnlockLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLock", CCNode *, this->m_pLock);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUnlock", CCNode *, this->m_pUnlock);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCanChange", CCNode *, this->m_pCanChange);
    
    return false;
}

bool WarriorHeadMid::onAssignCCBCustomProperty(cocos2d::CCObject *pTarget, const char* pMemberVariableName, CCBValue* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "flash", CCBValue *, this->m_bFlash);
    return false;
}

void WarriorHeadMid::updateView(warrior::WarriorCurInfo* pWarriorInfo,int expAdd /* = 0 */,bool isLevelUp /* = false */,int iPosId /* = -1 */)
{
    //m_pCanChange->setVisible(m_bFlash->getBoolValue());
    if(m_pWarriorHead) m_pWarriorHead->removeAllChildren();
    
    if(pWarriorInfo==NULL)
    {
        m_pWarriorContainer->setVisible(false);
        if(iPosId>0)
        {
            uint32_t unLockLevel = DM_GET_FORMATION_MODEL->getUnlockLevel(iPosId);
            m_pLock->setVisible(DM_GET_USER_MODEL->getUserInfo()->getUserLv()<unLockLevel);
            m_pUnlockLevel->setString(CCString::createWithFormat("%d",unLockLevel)->getCString());
        }else
        {
            m_pLock->setVisible(false);
            
        }
        m_pUnlock->setVisible(!m_pLock->isVisible());
        return;
    }
    
    m_pLock->setVisible(false);
    
    m_pWarriorContainer->setVisible(true);
    
    m_pLevelupEffect->removeAllChildren();
	if (isLevelUp)
    {
        m_pLevelupEffect->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("warriorlevelup"));
    }
    
    m_pBgColor1Sprite->setVisible(pWarriorInfo->getBaseInfo()->getColorGrade() >= warrior::kWarGradeRed);
	m_pBgColor2Sprite->setVisible(pWarriorInfo->getBaseInfo()->getColorGrade() == warrior::kWarGradePurple);
    m_pBgColor3Sprite->setVisible(pWarriorInfo->getBaseInfo()->getColorGrade() == warrior::kWarGradeBlue);
	m_pBgColor4Sprite->setVisible(pWarriorInfo->getBaseInfo()->getColorGrade() <= warrior::kWarGradeGreen);
	
	//m_pUpLevel->removeAllChildren();
	//m_pUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(pWarriorInfo->getWarriorUpLv()));
    
    cocos2d::ccColor3B color = DM_GET_WARRIOR_MODEL->getWarriorColorByWid(pWarriorInfo->getWid());
    
	//m_pHeadBgSprite->setColor(color);
	//m_pGrade->setColor(color);
    
    if(expAdd>0)
    {
        m_pExpAddNode->setVisible(true);
        CCAction* pAction = CCNumberTo::create(1.f, 0.f, expAdd, tween_num_selector(WarriorHeadMid::updateAddExp));
        runAction(pAction);
    }else
    {
        m_pExpAddNode->setVisible(false);
        updateAddExp(0.f);
    }
    
    
	m_pLevel->setString(CCString::createWithFormat("%d",pWarriorInfo->getWarriorLv())->getCString());
	m_pWarriorName->setString(pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
	
	//更新头像
	
    
	if(m_pWarriorHead) 
	{
		m_pWarriorHead->removeAllChildren();
		//m_pWarriorHead->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForFormation(pWarriorInfo->getAvatarId()));
		m_pWarriorHead->addChild(ResourceManager::sharedResourceManager()->getWarriorIconForChangeFormation(pWarriorInfo->getAvatarId()));
	}
}

void WarriorHeadMid::updateView(friends::FriendWarrior * friendWarrior, int level)
{
	if (!friendWarrior)
	{
		updateView(NULL);
		m_pLock->setVisible(false);
		m_pUnlock->setVisible(false);
	}
	else
	{
		updateView(warrior::WarriorCurInfo::create(friendWarrior->getWID()));
		//m_pUpLevel->removeAllChildren();
		//m_pUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(friendWarrior->getUpLevel()));

		if(m_pWarriorHead) 
		{
			m_pWarriorHead->removeAllChildren();
			m_pWarriorHead->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForFormation(friendWarrior->getAvatarID()));
		}

		m_pLevel->setString(CCString::createWithFormat("%d",level)->getCString());
	}
}

void WarriorHeadMid::updateAddExp(float num)
{
   if(m_pExpAdd) m_pExpAdd->setString(CCString::createWithFormat("+%d",(int)num)->getCString());
}

void WarriorHeadMid::setFlash(bool isFlash)
{
    if (isFlash == m_bFlash->getBoolValue()) {
        return;
    }
    CC_SAFE_RELEASE(m_bFlash);
    m_bFlash = CCBValue::create(isFlash);
    CC_SAFE_RETAIN(m_bFlash);
    
    //m_pCanChange->setVisible(isFlash);
}

