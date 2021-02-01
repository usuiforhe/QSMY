//
//  PvpTargetCell.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-8-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PvpTargetCell.h"
#include "Pvp.h"
#include "../../../managers/ResourceManager.h"
#include "../../../model/UserModel.h"
#include "../../../managers/GameManager.h"
#include "../../../managers/TipsManager.h"
#include "RankUser.h"

using namespace cocos2d;

#pragma mark -----------PvpTargetCell------------
PvpTargetCell::PvpTargetCell()
	:m_pDelegate(NULL),
	m_pContainer(NULL),
	m_pSelfContainer(NULL),
	m_pTargetContainer(NULL),
	m_pRank(NULL),
	m_pLevel(NULL),
	m_pName(NULL),
	m_pPointGrow(NULL),
	m_pButton(NULL),
	m_pSpriteChallenge(NULL),
	m_pSpriteRefresh(NULL),
	m_pSpriteView(NULL)
	,m_pNodeRoot(NULL)
	,m_pCCBBoss(NULL)
	,m_pTTFName(NULL)
	,m_pTTFCount(NULL)
{
	D_RESET_C_ARRAY(m_pRankUser, PVP_CELL_USER_COUNT);
};
PvpTargetCell::~PvpTargetCell()
{
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pSelfContainer);
    CC_SAFE_RELEASE(m_pTargetContainer);
    CC_SAFE_RELEASE(m_pRank);
    CC_SAFE_RELEASE(m_pLevel);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pPointGrow);
    
    CC_SAFE_RELEASE(m_pButton);
    CC_SAFE_RELEASE(m_pSpriteChallenge);
    CC_SAFE_RELEASE(m_pSpriteRefresh);
    CC_SAFE_RELEASE(m_pSpriteView);

	CC_SAFE_RELEASE(m_pNodeRoot);

	CC_SAFE_RELEASE(m_pCCBBoss);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pTTFCount);
	D_SAFE_RELEASE_C_ARRAY(m_pRankUser, PVP_CELL_USER_COUNT);
}

PvpTargetCell * PvpTargetCell::create(cocos2d::extension::CCData *pData)
{
    PvpTargetCell* pRet = new PvpTargetCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

bool PvpTargetCell::init(cocos2d::extension::CCData* pData)
{
	//    CCB_READER_DATA_AND_ADDCHILD(pData);
	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("RankUser", RankUserLoader::loader());
	//CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pNodeLoaderLibrary)
	CCB_READER_DATA_BY_CUSTOM_LOADER(pData, m_pNodeRoot, pNodeLoaderLibrary);
	CC_SAFE_RETAIN(m_pNodeRoot);
	if (m_pNodeRoot)
	{
		addChild(m_pNodeRoot);
	}
	for (int i = 0; i < PVP_CELL_USER_COUNT; i++)
	{
		m_pCellPosY[i] = m_pRankUser[i]->getPositionY();
	}
	
	return CCTableViewCell::init();
}

bool PvpTargetCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelfContainer", CCNode *, this->m_pSelfContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTargetContainer", CCNode *, this->m_pTargetContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRank", CCLabelBMFont *, this->m_pRank);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPointGrow", CCLabelBMFont *, this->m_pPointGrow);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, this->m_pButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteChallenge", CCSprite *, this->m_pSpriteChallenge);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteRefresh", CCSprite *, this->m_pSpriteRefresh);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteView", CCSprite *, this->m_pSpriteView);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCCBBoss", CCNode *, m_pCCBBoss);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFCount", CCLabelTTF *, m_pTTFCount);
	D_CCB_ASSIGN_ARRAY("m_pRank", RankUser *, m_pRankUser, PVP_CELL_USER_COUNT);
    
    return false;
}

SEL_MenuHandler PvpTargetCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackBoss", PvpTargetCell::callBackBoss);
	return NULL;
}

void PvpTargetCell::setTargetInfo(pvp::ChTargetInfo * pTargetInfo0, pvp::ChTargetInfo *pTargetInfo1, pvp::ChTargetInfo *pTargetInfo2, bool autoPos)
{
	m_pRankUser[0]->setTargetInfo(pTargetInfo0);
	m_pRankUser[1]->setTargetInfo(pTargetInfo1);
	m_pRankUser[2]->setTargetInfo(pTargetInfo2);
	for (int i = 0; i < PVP_CELL_USER_COUNT; i++)
	{
		m_pRankUser[i]->setPositionY(m_pCellPosY[i]);
	}
	if (autoPos)
	{

		if (pTargetInfo2 == NULL)
		{
			m_pRankUser[0]->setPositionY(m_pCellPosY[1]);
			m_pRankUser[1]->setPositionY(m_pCellPosY[2]);
		}
		if (pTargetInfo1 == NULL)
		{
			m_pRankUser[0]->setPositionY(m_pCellPosY[2]);
		}
	}
	

	updateView();
}

void PvpTargetCell::setBossVisible(bool bBossVisible)
{
	m_pCCBBoss->setVisible(bBossVisible);
}

void PvpTargetCell::updateView()
{    

	pvp::ChallengeInfo * info = DM_GET_PVP_MODEL->getChallengeInfo();
	if(info->bossChallengable())
	{
		m_pTTFCount->setString(CCString::createWithFormat("%d/%d", info->getRemainBossTimes(), info->getMaxBoxTimes())->getCString());
	}
	else
	{
		m_pTTFCount->setString("0/0");
	}
//     m_pContainer->removeAllChildren();
//     m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(m_pTargetInfo->getAvatarId()));
//     
//     m_pRank->setString(CCString::createWithFormat("%d",m_pTargetInfo->getRank())->getCString());
//     m_pLevel->setString(CCString::createWithFormat("%d",m_pTargetInfo->getLevel())->getCString());
//     m_pName->setString(m_pTargetInfo->getName().c_str());    
//     m_pPointGrow->setString(CCString::createWithFormat("%d",m_pTargetInfo->getPointGrow())->getCString());
//     
//     if(DM_GET_USER_MODEL->checkCurrentUser(m_pTargetInfo->getUserId()))
//     {
//         m_pSelfContainer->setVisible(true);
//         m_pTargetContainer->setVisible(false);
//     }
//     else
//     {
//         m_pSelfContainer->setVisible(false);
//         m_pTargetContainer->setVisible(true);
//     }
}

void PvpTargetCell::callBackBoss(CCObject *pObj)
{
	if (m_pDelegate)
	{
		m_pDelegate->willChallenge(NULL, true);
	}
}

#pragma mark -----------PvpTopRankCell------------

PvpTopRankCell * PvpTopRankCell::create(cocos2d::extension::CCData *pData)
{
    PvpTopRankCell* pRet = new PvpTopRankCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

bool PvpTopRankCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    PvpTargetCell::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
    
    return false;
}

void PvpTopRankCell::updateView()
{
//     PvpTargetCell::updateView();
//     m_pSpriteRefresh->setVisible(false);
//     m_pSpriteChallenge->setVisible(false);
//     
//     if(DM_GET_USER_MODEL->checkCurrentUser(m_pTargetInfo->getUserId()))
//     {
//         m_pSpriteView->setVisible(false);
//         m_pButton->setVisible(false);
//     }
//     else
//     {
//         m_pButton->getButton()->setTarget(this, menu_selector(PvpTopRankCell::onViewClick));
//         m_pSpriteView->setVisible(true);
//         m_pButton->setVisible(true);
//     }
}

void PvpTopRankCell::onViewClick(cocos2d::CCObject *sender)
{
// 	if(m_pDelegate)
// 	{
// 		if(!m_pTargetInfo->isRobot())
// 		{
// 			m_pDelegate->showOtherFormation(DM_GET_OTHERFORMATION_MODEL->getForamtionInfoByUid(m_pTargetInfo->getUserId()));
// 		}
// 		else
// 		{
// 			m_pDelegate->showOtherFormation(DM_GET_OTHERFORMATION_MODEL->getRobotFormationInfoByFid(m_pTargetInfo->getRobotFormation()->fid,m_pTargetInfo->getLevel()));
// 		}
// 	}
}


#pragma mark -----------PvpViewCell------------

PvpViewCell * PvpViewCell::create(cocos2d::extension::CCData *pData)
{
    PvpViewCell* pRet = new PvpViewCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

bool PvpViewCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    PvpTargetCell::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
    
    return false;
}

void PvpViewCell::updateView()
{
    PvpTargetCell::updateView();
//    m_pSpriteView->setVisible(false);
    
//     if(DM_GET_USER_MODEL->checkCurrentUser(m_pTargetInfo->getUserId()))
//     {
//         m_pSpriteChallenge->setVisible(false);
//         m_pSpriteRefresh->setVisible(true);
//         m_pButton->setVisible(true);
//         m_pButton->getButton()->setTarget(this, menu_selector(PvpViewCell::onRefreshClick));
//     }
//     else
//     {
//         m_pSpriteRefresh->setVisible(false);
//         
//         if(m_pTargetInfo->getRank() < DM_GET_PVP_MODEL->getChallengeInfo()->getRank())
//         {
//             m_pButton->getButton()->setTarget(this, menu_selector(PvpViewCell::onChallengeClick));
//             m_pButton->setVisible(true);
//             m_pSpriteChallenge->setVisible(true);
//         }
//         else
//         {
//             m_pButton->setVisible(false);
//             m_pSpriteChallenge->setVisible(false);
//         }
//     }
}

void PvpViewCell::onRefreshClick(cocos2d::CCObject *sender)
{    
    DM_GET_PVP_MODEL->getInitInfo();
}

void PvpViewCell::onChallengeClick(cocos2d::CCObject *sender)
{
// 	if (m_pDelegate)
// 	{
// 		m_pDelegate->willChallenge(m_pTargetInfo);
// 	}
// 	return;
// 	pvp::ChallengeInfo * info = DM_GET_PVP_MODEL->getChallengeInfo();
//     if(info->getRemainChallengeTimes() <= 0)
// 	{
// 		int nVipLv = DM_GET_USER_MODEL->getUserInfo()->getVipLevel();
// 		if (nVipLv == 0)
// 		{
// 			nVipLv = 1;
// 		}
// 		
// 		user::VipInfo *pCurInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(nVipLv);
// 		int nMaxLv = DM_GET_USER_MODEL->getMaxVipLevel();
// 		for (int i = nVipLv + 1; i <= nMaxLv; i++)
// 		{
// 			user::VipInfo *pInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(i);
// 			if (pInfo->getPvpMax() > pCurInfo->getPvpMax())
// 			{
// 				NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV,CCString::create(D_LOCAL_STRING("NEED_VIP%d%d",DM_GET_USER_MODEL->getToVipLevelCost(i),i)));
// 				return;
// 			}
// 			
// 		}
//         TIPSMANAGER->warning(D_LOCAL_STRING("PvpNotEnoughChTimes"));
//         return;
//     }
//     
//     HTTP_CLIENT->chPvpTarget(m_pTargetInfo->getUserId(), m_pTargetInfo->getRank(), info->getRank(), DM_GET_PVP_MODEL, callfuncJson_selector(PvpModel::handleError));
}



#pragma mark ------------PvpBossView--------------

PvpBossView::~PvpBossView()
{
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pDesc);
    CC_SAFE_RELEASE(m_pChContainer);
    CC_SAFE_RELEASE(m_pBossTimes);
    CC_SAFE_RELEASE(m_pButton);
}

bool PvpBossView::init()
{
    if(!cocos2d::CCNode::init())
    {
        return false;
    }
    
    return true;
}

void PvpBossView::updateView()
{
    m_pName->setString(D_LOCAL_STRING("PvpBoss").c_str());
    m_pDesc->setString(D_LOCAL_STRING("PvpWinBoss").c_str());
    
    pvp::ChallengeInfo * info = DM_GET_PVP_MODEL->getChallengeInfo();
    
    m_pChContainer->setVisible(info->bossChallengable());
    if(info->bossChallengable())
    {
        m_pBossTimes->setString(CCString::createWithFormat("%d/%d", info->getRemainBossTimes(), info->getMaxBoxTimes())->getCString());
        m_pButton->getButton()->setTarget(this, menu_selector(PvpBossView::onChallengeClick));
    }
}

bool PvpBossView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF *, this->m_pDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChContainer", CCNode *, this->m_pChContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBossTimes", CCLabelBMFont *, this->m_pBossTimes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, this->m_pButton);
    
    return false;
}

void PvpBossView::onChallengeClick(cocos2d::CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->willChallenge(NULL, true);
	}
	return;
    pvp::ChallengeInfo * info = DM_GET_PVP_MODEL->getChallengeInfo();
    if(info->getRemainChallengeTimes() <= 0)
    {
        TIPSMANAGER->warning(D_LOCAL_STRING("PvpNotEnoughChTimes"));
        return;
    }
    if(info->getRemainBossTimes() <= 0)
    {
        TIPSMANAGER->warning(D_LOCAL_STRING("PvpNotEnoughBossTimes"));
        return;
    }
    HTTP_CLIENT->chPvpBoss(info->getRank(), DM_GET_PVP_MODEL, callfuncJson_selector(PvpModel::handleError));
}

#pragma mark ------------PvpRobotView--------------

PvpRobotView::~PvpRobotView(){    
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pDesc);
    CC_SAFE_RELEASE(m_pButton);
}

bool PvpRobotView::init()
{
    if(!cocos2d::CCNode::init())
    {
        return false;
    }
    
    return true;
}

void PvpRobotView::updateView()
{
    m_pName->setString(D_LOCAL_STRING("PvpTrainRobot").c_str());
    m_pDesc->setString(D_LOCAL_STRING("PvpChallengeRobot").c_str());
    
    m_pButton->setVisible(true);
    m_pButton->getButton()->setTarget(this, menu_selector(PvpRobotView::onChallengeClick));
}

bool PvpRobotView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF *, this->m_pDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, this->m_pButton);
    
    return false;
}

void PvpRobotView::onChallengeClick(cocos2d::CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->willChallenge(NULL, false, true);
	}
	return;
    pvp::ChallengeInfo * info = DM_GET_PVP_MODEL->getChallengeInfo();
    if(info->getRemainChallengeTimes() <= 0)
    {
        TIPSMANAGER->warning(D_LOCAL_STRING("PvpNotEnoughChTimes"));
        return;
    }
    HTTP_CLIENT->chPvpNPC(info->getRank(), DM_GET_PVP_MODEL, callfuncJson_selector(PvpModel::handleError));
}