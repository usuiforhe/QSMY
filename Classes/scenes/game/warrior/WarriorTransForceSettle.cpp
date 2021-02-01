//  WarriorTransForceSettle.cpp
//  传功结算界面
//  Created by cll on 13-3-29
//  Copyright (c) 2013, thedream. All rights reserved.
//

#include "WarriorTransForceSettle.h"
#include "../../../managers/ResourceManager.h"
#include "../../../managers/GameManager.h"
#include "../Game.h"
#include "Warrior.h"
#include "../../../managers/TipsManager.h"

USING_NS_CC;

WarriorTransForceSettleView::WarriorTransForceSettleView()
:m_pDelegate(NULL)
,m_pWarriorInfo(NULL)
,m_pWarriorModel(NULL)
,m_pContainer(NULL)
,m_pName(NULL)
,m_pLevel(NULL)
,m_pSpeed(NULL)
,m_pHp(NULL)
,m_pDefence(NULL)
,m_pAttack(NULL)
,m_pGrade(NULL)
,m_pEffectNode(NULL)
,m_pWarriorTypeMark(NULL)
{
    
}

WarriorTransForceSettleView::~WarriorTransForceSettleView()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pLevel);
    CC_SAFE_RELEASE(m_pSpeed);
    CC_SAFE_RELEASE(m_pHp);
    CC_SAFE_RELEASE(m_pDefence);
    CC_SAFE_RELEASE(m_pAttack);
    CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pEffectNode);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
}


bool WarriorTransForceSettleView::init()
{
    
	if(!DLayerModal::init())
	{
		return false;
	}
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    CCB_READER_AND_ADDCHILD("ui/ccb/warrior/transForceSettle");
    
	return true;
}



void WarriorTransForceSettleView::onEnter()
{
	Game::sharedGame()->setNavigatorVisable(false);
	updateWarriorSettle();
	DLayerModal::onEnter();
}


void WarriorTransForceSettleView::onExit()
{
	Game::sharedGame()->setNavigatorVisable(true);
	DLayerModal::onExit();
}


void WarriorTransForceSettleView::updateWarriorSettle()
{
	
	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("WarriorInfoError").c_str());
		return ;
	}
    
	m_pContainer->removeAllChildren();
    m_pEffectNode->removeAllChildren();
    
	CCNode *pAvator =  ResourceManager::sharedResourceManager()->getUnitAnimate2X(m_pWarriorInfo->getAvatarId());
    CCNode* pAni = ResourceManager::sharedResourceManager()->getUIAnimate("trans");
    
    m_pEffectNode->addChild(pAni);
	m_pContainer->addChild(pAvator);
    
    m_pGrade->setColor(m_pWarriorModel->getWarriorColorByWid(m_pWarriorInfo->getWid()));
    
    m_pName->setString(m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
    
	m_pLevel->setString(CCString::createWithFormat("%d",m_pWarriorInfo->getWarriorLv())->getCString());
    
    m_pSpeed->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeSpeed))->getCString());
    
    m_pHp->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeHp))->getCString());
    
    m_pDefence->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeDefence))->getCString());
    
    m_pAttack->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeAttak))->getCString());
    
    m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
}

bool WarriorTransForceSettleView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeed", CCLabelBMFont *, this->m_pSpeed);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHp", CCLabelBMFont *, this->m_pHp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefence", CCLabelBMFont *, this->m_pDefence);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttack", CCLabelBMFont *, this->m_pAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode *, this->m_pEffectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
    return false;
}

SEL_MenuHandler WarriorTransForceSettleView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallBack", WarriorTransForceSettleView::menuCallBack);
    return NULL;
}

void WarriorTransForceSettleView::menuCallBack(CCObject * obj)
{
    
    
	if (m_pDelegate)
	{
		m_pDelegate->showInfoView(kTransForceView,m_pWarriorInfo);
	}
    
    
	//removeAllChildren();
	//removeFromParent();
    
}	

/////////////////////////////////////////////////////