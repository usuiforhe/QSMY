//
//  SettlementViewPve.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SettlementViewPve.h"
#include "../../Game.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"


USING_NS_CC;
using namespace battle;

SettlementViewPve::SettlementViewPve()
:m_pInfo(NULL)
,m_pDelegate(NULL)
,m_pReplay(NULL)
,m_pDBClose(NULL)
,m_pExp(NULL)
,m_pGold(NULL)
,m_pLose(NULL)
,m_pWin(NULL)
,m_pEffectNode(NULL)
,m_pUpgrade(NULL)
,m_pWarriors(NULL)
,m_pMapName(NULL)
,m_pMapEffectNode(NULL)
,m_pClose(NULL)
,m_pLoseMenu1(NULL)
,m_pLoseMenu2(NULL)
,m_pLoseMenu3(NULL)
,m_pLoseMenu4(NULL)
,m_pExpFirst(NULL)
{
    for (uint8_t i=0; i<FORMATION_POS_MAX; ++i)
    {
        m_pWarriorNode[i]=NULL;
    }
    
    for (uint8_t i=0; i<BATTLE_GRADE_COUNT; ++i)
    {
        m_pStar[i]=NULL;
    }
}

SettlementViewPve::~SettlementViewPve()
{
    CC_SAFE_RELEASE(m_pInfo);
    for (uint8_t i=0; i<FORMATION_POS_MAX; ++i)
    {
        CC_SAFE_RELEASE(m_pWarriorNode[i]);
    }
    
    for (uint8_t i=0; i<BATTLE_GRADE_COUNT; ++i)
    {
        CC_SAFE_RELEASE(m_pStar[i]);
    }
    
    CC_SAFE_RELEASE(m_pReplay);
	CC_SAFE_RELEASE(m_pDBClose);
    CC_SAFE_RELEASE(m_pExp);
    CC_SAFE_RELEASE(m_pGold);
    CC_SAFE_RELEASE(m_pLose);
    CC_SAFE_RELEASE(m_pWin);
    CC_SAFE_RELEASE(m_pEffectNode);
    CC_SAFE_RELEASE(m_pUpgrade);
    CC_SAFE_RELEASE(m_pWarriors);
    CC_SAFE_RELEASE(m_pMapName);
    CC_SAFE_RELEASE(m_pMapEffectNode);
	CC_SAFE_RELEASE(m_pClose);
	CC_SAFE_RELEASE(m_pLoseMenu1);
	CC_SAFE_RELEASE(m_pLoseMenu2);
	CC_SAFE_RELEASE(m_pLoseMenu3);
	CC_SAFE_RELEASE(m_pLoseMenu4);
	CC_SAFE_RELEASE(m_pExpFirst);
}


bool SettlementViewPve::init(SettlementDelegate *delegate,settlement::SettlementInfo *pInfo)
{
    CCAssert(delegate!=NULL && dynamic_cast<settlement::SettlementInfoPve*>(pInfo)!=NULL, "");
    if(!DLayer::init()) return false;
    CC_SAFE_RELEASE(m_pInfo);
    CC_SAFE_RETAIN(pInfo);
    m_pInfo = dynamic_cast<settlement::SettlementInfoPve*>(pInfo);
    m_pDelegate =delegate;

	
    
    //CCB_READER_AND_ADDCHILD("ui/ccb/pve/pveSettle");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_battle/PL_battleover");

    pve::MapInfo* pMapInfo = DM_GET_PVE_MODEL->getMapInfo(m_pInfo->mapId);
    m_pMapName->setString(pMapInfo->map_name.c_str());
    
    for (uint8_t i=0; i<FORMATION_POS_MAX; ++i)
    {
        formation::PositionInfo *pPosInfo = DM_GET_FORMATION_MODEL->getPositionInfoByIndex(i);
        
        warrior::WarriorCurInfo * pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(pPosInfo->iWarriorId);
        if(pWarrior)
        {
            m_pWarriorNode[i]->updateView(pWarrior,m_pInfo->wexp,m_pInfo->isWarriorLevelup(pPosInfo->iWarriorId));
        }else
        {
            m_pWarriorNode[i]->updateView(NULL,0,false,pPosInfo->iPosId);
        }
        
    }
	m_pExpFirst->setVisible(DM_GET_PVE_MODEL->getFightType() != pve::PveFightElite);
    m_pWin->setVisible(m_pInfo->win);
    m_pLose->setVisible(!m_pWin->isVisible());
    m_pEffectNode->removeAllChildren();
    m_pEffectNode->setVisible(m_pWin->isVisible());
    m_pWarriors->setVisible(m_pWin->isVisible());
    m_pUpgrade->setVisible(!m_pWin->isVisible());
    if(m_pEffectNode->isVisible())
    {
        m_pEffectNode->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("starsflash"));
    }
    
    for (uint8_t i=0; i<BATTLE_GRADE_COUNT; ++i)
    {
        if(m_pInfo->win)
        {
            m_pStar[i]->setVisible(true);
        }else
        {
            //m_pStar[i]->setVisible(false);
            continue;
        }
        if(m_pInfo->grade>i)
        {
            m_pStar[i]->selected();
        }else
        {
            m_pStar[i]->unselected();
        }
    }
    
    m_pGold->setString(CCString::createWithFormat("%d",m_pInfo->coin)->getCString());
    m_pExp->setString(CCString::createWithFormat("%d",m_pInfo->exp)->getCString());
    
    m_pReplay->getButton()->setTarget(this, menu_selector(SettlementViewPve::replayBattle));
	m_pDBClose->getButton()->setTarget(this, menu_selector(SettlementViewPve::quitBattle));
	m_pClose->setTarget(this, menu_selector(SettlementViewPve::quitBattle));
    //m_pEnhance->getButton()->setTarget(this, menu_selector(SettlementViewPve::gotoEnhanceEquip));
    //m_pQuit->getButton()->setTarget(this, menu_selector(SettlementViewPve::quitBattle));
    
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHOW_PVE_SETTLEMENT);
    
    /*CCSprite* pTitleBg =  ResourceManager::sharedResourceManager()->getSettlementTitle(pMapInfo->chapter_id);
    pTitleBg->setOpacity(0);
    pTitleBg->runAction(CCFadeIn::create(1.0f));
    m_pMapEffectNode->addChild(pTitleBg);
    pTitleBg->setAnchorPoint(D_ANCHOR_POINT_CENTER);*/

    if(!m_pInfo->win)
	{
		//pve::PveFailureInfo *pFailureInfo = DM_GET_PVE_MODEL->getFailureInfo();
		//pve::PveFailureInfo *pFailureInfoHigher = DM_GET_PVE_MODEL->getFailureInfoHigher();
  //      m_pNodeNormal->removeAllChildren();
  //      m_pNodeGreat->removeAllChildren();
		//if (pFailureInfo)
		//{
		//	m_pLabelNormal->setString(pFailureInfo->sText.c_str());
		//	m_pButtonNormal->setTarget(this, menu_selector(SettlementViewPve::callBackUpgrade));
		//	m_pButtonNormal->setUserObject(pFailureInfo);
  //          m_pNodeNormal->addChild(CCSprite::create(pFailureInfo->sImage.c_str()));
		//}
		//else
		//{
		//	m_pLabelNormal->setString("");
		//}
		//
		//if (pFailureInfoHigher)
		//{
		//	m_pLabelGreat->setString(pFailureInfoHigher->sText.c_str());
		//	m_pButtonGreat->setTarget(this, menu_selector(SettlementViewPve::callBackUpgrade));
		//	m_pButtonGreat->setUserObject(pFailureInfoHigher);
  //          m_pNodeGreat->addChild(CCSprite::create(pFailureInfoHigher->sImage.c_str()));
		//}
		//else
		//{
		//	m_pLabelGreat->setString("");
		//}

		m_pLoseMenu1->setTarget(this, menu_selector(SettlementViewPve::gotoEnhanceEquip));
		m_pLoseMenu2->setTarget(this, menu_selector(SettlementViewPve::gotoEnhanceEquip));
		m_pLoseMenu3->setTarget(this, menu_selector(SettlementViewPve::gotoEnhanceEquip));
		m_pLoseMenu4->setTarget(this, menu_selector(SettlementViewPve::goToMallView));
	}

	//HTTP_CLIENT->getBusyLayer()->hideBusy();
    return true;
}

bool SettlementViewPve::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior1Node", WarriorHeadMid*, this->m_pWarriorNode[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior2Node", WarriorHeadMid*, this->m_pWarriorNode[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior3Node", WarriorHeadMid*, this->m_pWarriorNode[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior4Node", WarriorHeadMid*, this->m_pWarriorNode[3]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior5Node", WarriorHeadMid*, this->m_pWarriorNode[4]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior6Node", WarriorHeadMid*, this->m_pWarriorNode[5]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior7Node", WarriorHeadMid*, this->m_pWarriorNode[6]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStar1", CCMenuItem*, this->m_pStar[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStar2", CCMenuItem*, this->m_pStar[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStar3", CCMenuItem*, this->m_pStar[2]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReplay", DButton*, this->m_pReplay);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBClose", DButton*, this->m_pDBClose);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLose", CCSprite*, this->m_pLose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWin", CCSprite*, this->m_pWin);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGold", CCLabelBMFont*, this->m_pGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExp", CCLabelBMFont*, this->m_pExp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode*, this->m_pEffectNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpgrade", CCNode*, this->m_pUpgrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriors", CCNode*, this->m_pWarriors);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMapName", CCLabelBMFont*, this->m_pMapName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMapEffectNode", CCNode*, this->m_pMapEffectNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pClose", CCMenuItem*, this->m_pClose);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLoseMenu1", CCMenuItemImage*, this->m_pLoseMenu1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLoseMenu2", CCMenuItemImage*, this->m_pLoseMenu2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLoseMenu3", CCMenuItemImage*, this->m_pLoseMenu3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLoseMenu4", CCMenuItemImage*, this->m_pLoseMenu4);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpFirst" , CCNode*,   m_pExpFirst);

    return false;
}

void SettlementViewPve::replayBattle(CCObject*)
{
    if(m_pDelegate) m_pDelegate->replayBattle();
}

void SettlementViewPve::gotoEnhanceEquip(CCObject*)
{
    //if(m_pDelegate) m_pDelegate->quitBattle();
    //Game::sharedGame()->NavigateTo(navigation::kNavEquip);
	Game::sharedGame()->NavigateTo(navigation::kNavFormation);
	this->quitBattle(NULL);
}

void SettlementViewPve::goToFormationView(CCObject*)
{
	Game::sharedGame()->NavigateTo(navigation::kNavChangeFormation);
	this->quitBattle(NULL);
}

void SettlementViewPve::goToMallView(CCObject*)
{
	Game::sharedGame()->NavigateTo(navigation::kNavMallLottery);
	this->quitBattle(NULL);
}


void SettlementViewPve::quitBattle(CCObject*)
{
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(m_pInfo->dropId));
    //HTTP_CLIENT->getUserInfo(); //同步用户信息
	if(m_pDelegate) m_pDelegate->quitBattle();
}

void SettlementViewPve::callBackUpgrade(CCObject *sender)
{
	CCMenuItem *pItem = (CCMenuItem *)sender;
	pve::PveFailureInfo *pInfo = (pve::PveFailureInfo *)pItem->getUserObject();

	Game::sharedGame()->NavigateTo((navigation::NavigationType)pInfo->uScene,false,pInfo->uIndex>0?CCInteger::create(pInfo->uIndex):NULL);
	this->quitBattle(NULL);
}

void SettlementViewPve::closeView()
{
	this->quitBattle(NULL);
}