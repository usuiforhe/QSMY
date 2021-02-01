//  RecruitSettle.cpp
//  招募结算界面
//  Created by cll on 13-5-22
//  Copyright (c) 2013, thedream. All rights reserved.
//

#include "RecruitSettle.h"
#include "../../../managers/ResourceManager.h"
#include "../../../managers/GameManager.h"
#include "../Game.h"
#include "Warrior.h"
#include "../../../managers/TipsManager.h"
#include "WarriorInfo.h"

USING_NS_CC;

RecruitSettleView::RecruitSettleView()
:m_pDelegate(NULL)
,m_pWarriorInfo(NULL)
,m_pContainer(NULL)
,m_pName(NULL)
,m_pLevel(NULL)
,m_pSpeed(NULL)
,m_pHp(NULL)
,m_pDefence(NULL)
,m_pAttack(NULL)
,m_pGrade(NULL)
,m_pEffectNode(NULL)
//,m_pDBtnShare(NULL)
,m_pDBtnView(NULL)
,m_pWarriorTypeMark(NULL)
,m_pPopupNode(NULL)
,m_pWarriorGradeBg(NULL)
{
    
}

RecruitSettleView::~RecruitSettleView()
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
	//CC_SAFE_RELEASE(m_pDBtnShare);
	CC_SAFE_RELEASE(m_pDBtnView);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
	CC_SAFE_RELEASE(m_pWarriorInfo);
	CC_SAFE_RELEASE(m_pWarriorGradeBg);
}


bool RecruitSettleView::init()
{    
	if(!DLayerModal::init())
	{
		return false;
	}
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zhaomu_cg/pl_zhaomu");

	m_pDBtnView->setTarget(this, menu_selector(RecruitSettleView::viewCallback));
	//m_pDBtnShare->getButton()->setTarget(this, menu_selector(RecruitSettleView::shareCallback));

	m_pPopupNode = CCNode::create();
	this->addChild(m_pPopupNode, 1);
    
	return true;
}

void RecruitSettleView::onEnter()
{
	Game::sharedGame()->setNavigatorVisable(true);
	updateWarriorSettle();
	DLayerModal::onEnter();
}

void RecruitSettleView::onExit()
{
	Game::sharedGame()->setNavigatorVisable(true);
	DLayerModal::onExit();
}

void RecruitSettleView::onBackKey()
{
	if (m_pPopupNode->getChildrenCount() > 0)
	{
		m_pPopupNode->removeAllChildren();
		return;
	}
	
	removeFromParent();
}

void RecruitSettleView::updateWarriorSettle()
{
	
	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("WarriorInfoError").c_str());
		return ;
	}
    
	m_pContainer->removeAllChildren();
    m_pEffectNode->removeAllChildren();
    
	CCNode *pAvator =  ResourceManager::sharedResourceManager()->getUnitAnimate2X(m_pWarriorInfo->getAvatarId());
    CCNode* pAni = ResourceManager::sharedResourceManager()->getUIAnimate("zhaomu_success");
    
    m_pEffectNode->addChild(pAni);
	m_pContainer->addChild(pAvator);
    
    m_pGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(m_pWarriorInfo->getWid()));
    m_pWarriorGradeBg->setWarriorGrade(m_pWarriorInfo->getBaseInfo()->getColorGrade());
    m_pName->setString(m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
    
	m_pLevel->setString(CCString::createWithFormat("%d",m_pWarriorInfo->getWarriorLv())->getCString());
    
    m_pSpeed->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeSpeed))->getCString());
    
    m_pHp->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeHp))->getCString());
    
    m_pDefence->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeDefence))->getCString());
    
    m_pAttack->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeAttak))->getCString());
    
    m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
}

bool RecruitSettleView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
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
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnShare", DButton *, this->m_pDBtnShare);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnView", CCMenuItemImage *, this->m_pDBtnView);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg* ,m_pWarriorGradeBg);
    return false;
}

SEL_MenuHandler RecruitSettleView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallBack", RecruitSettleView::menuCallBack);
    return NULL;
}

void RecruitSettleView::menuCallBack(CCObject * obj)
{
    if(m_pDelegate)
    {
        m_pDelegate->closeInfoView(kRecruitSettleView);
    }else
    {
        removeFromParent();
    }
}

void RecruitSettleView::viewCallback(cocos2d::CCObject* obj)
{
	if (!m_pWarriorInfo)
	{
		return;
	}

	WarriorInfoView* m_pWarriorInfoView = WarriorInfoView::create(WarriorInfoView::kTypeCloseOnly);
	m_pWarriorInfoView->setWarrior(m_pWarriorInfo);

	m_pPopupNode->addChild(m_pWarriorInfoView);
}

void RecruitSettleView::shareCallback(cocos2d::CCObject* obj)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHARE, CCStringMake(D_LOCAL_STRING("ShareMessage")));
    menuCallBack(obj);
}
