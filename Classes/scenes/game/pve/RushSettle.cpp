//
//  RushSettle.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-5.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "RushSettle.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"


USING_NS_CC;
using namespace battle;

RushSettle::RushSettle()
:m_pInfo(NULL)
,m_pQuit(NULL)
,m_pExp(NULL)
,m_pGold(NULL)
,m_pClose(NULL)
,m_pSaodang(NULL)
,m_pLose(NULL)
,m_pWin(NULL)
,m_pUpgrade(NULL)
,m_pWarriors(NULL)
,m_pReplayNode(NULL)
,m_pStarMenu(NULL)
,m_pMapName(NULL)
{
    
    D_RESET_C_ARRAY(m_pWarriorNode, FORMATION_POS_MAX);
    
}

RushSettle::~RushSettle()
{
    CC_SAFE_RELEASE(m_pInfo);
    
    D_SAFE_RELEASE_C_ARRAY(m_pWarriorNode, FORMATION_POS_MAX);
    
    CC_SAFE_RELEASE(m_pQuit);
    CC_SAFE_RELEASE(m_pExp);
    CC_SAFE_RELEASE(m_pGold);
	CC_SAFE_RELEASE(m_pClose);
	CC_SAFE_RELEASE(m_pSaodang);
	CC_SAFE_RELEASE(m_pLose);
	CC_SAFE_RELEASE(m_pWin);
	CC_SAFE_RELEASE(m_pUpgrade);
	CC_SAFE_RELEASE(m_pWarriors);
	CC_SAFE_RELEASE(m_pReplayNode);
	CC_SAFE_RELEASE(m_pStarMenu);
	CC_SAFE_RELEASE(m_pMapName);
}


bool RushSettle::init()
{
    if(!DPopup::init()) return false;
    //CCB_READER_AND_ADDCHILD("ui/ccb/pve/pveRushSettle");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_battle/PL_battleover");
    
    //m_pQuit->getButton()->setTarget(this, menu_selector(RushSettle::closeCallBack));
	//m_pReplay->getButton()->setTarget(this, menu_selector(SettlementViewPve::replayBattle));
	m_pClose->setTarget(this, menu_selector(RushSettle::closeCallBack));

	m_pSaodang->setVisible(true);
	m_pWin->setVisible(false);
	m_pLose->setVisible(false);
	m_pWarriors->setVisible(true);
	m_pUpgrade->setVisible(false);
	m_pStarMenu->setVisible(false);
	m_pReplayNode->setVisible(false);
    return true;
}

void RushSettle::closeCallBack(cocos2d::CCObject*)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(m_pInfo->dropId));
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVNET_PVE_CAN_GET_REPUTATION);
    closePopup();
}

bool RushSettle::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior1Node", WarriorHeadMid*, this->m_pWarriorNode[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior2Node", WarriorHeadMid*, this->m_pWarriorNode[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior3Node", WarriorHeadMid*, this->m_pWarriorNode[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior4Node", WarriorHeadMid*, this->m_pWarriorNode[3]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior5Node", WarriorHeadMid*, this->m_pWarriorNode[4]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior6Node", WarriorHeadMid*, this->m_pWarriorNode[5]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior7Node", WarriorHeadMid*, this->m_pWarriorNode[6]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pQuit", DButton*, this->m_pQuit);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGold", CCLabelBMFont*, this->m_pGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExp", CCLabelBMFont*, this->m_pExp);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pClose", CCMenuItem*, this->m_pClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSaodang", CCSprite*, this->m_pSaodang);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWin", CCSprite*, this->m_pWin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLose", CCSprite*, this->m_pLose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpgrade", CCNode*, this->m_pUpgrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriors", CCNode*, this->m_pWarriors);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReplayNode", CCNode*, this->m_pReplayNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStarMenu", CCMenu*, this->m_pStarMenu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMapName", CCLabelBMFont *, m_pMapName);
	
    return false;
}

void RushSettle::onEnter()
{
    CCAssert(m_pInfo, "RushSettle m_pInfo is null!!");
    updateView();
    DPopup::onEnter();
}

void RushSettle::onExit()
{
    CC_SAFE_RELEASE_NULL(m_pInfo);
    DPopup::onExit();
}

void RushSettle::updateView()
{
	m_pReplayNode->setVisible(false);
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
    
    m_pGold->setString(CCString::createWithFormat("%d",m_pInfo->coin)->getCString());
    m_pExp->setString(CCString::createWithFormat("%d",m_pInfo->exp)->getCString());
	pve::MapInfo *pMap = DM_GET_PVE_MODEL->getMapInfo(m_pInfo->mapId);
	m_pMapName->setString(pMap->map_name.c_str());
}

void RushSettle::setInfo(settlement::SettlementInfoPve* pInfo)
{
    CC_SAFE_RELEASE(m_pInfo);
    CC_SAFE_RETAIN(pInfo);
    m_pInfo = pInfo;
}


