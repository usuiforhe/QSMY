//
//  RankUser.h
//  排行用户
//
//  Created by Eci on 14-7-7.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "RankUser.h"
#include "Pvp.h"

USING_NS_CC;
USING_NS_CC_EXT;

RankUser::RankUser()
	: m_pInfo(NULL)
	, m_pBMFRank(NULL)
	, m_pTTFName(NULL)
	, m_pTTFPoint(NULL)
	, m_pNodeHead(NULL)
	, m_pNodeTop(NULL)
	, m_pNodeBottom(NULL)
	, m_pMyStone(NULL)
	, m_pOthersStone(NULL)
{
    
}


RankUser::~RankUser()
{
	CC_SAFE_RELEASE(m_pInfo);
	CC_SAFE_RELEASE(m_pBMFRank);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pTTFPoint);
	CC_SAFE_RELEASE(m_pNodeHead);
	CC_SAFE_RELEASE(m_pNodeTop);
	CC_SAFE_RELEASE(m_pNodeBottom);
	CC_SAFE_RELEASE(m_pMyStone);
	CC_SAFE_RELEASE(m_pOthersStone);
}


bool RankUser::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRank", CCLabelBMFont *, m_pBMFRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFPoint", CCLabelTTF *, m_pTTFPoint);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeHead", CCNode *, m_pNodeHead);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeTop", CCNode *, m_pNodeTop);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeBottom", CCNode *, m_pNodeBottom);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOthersStone", CCNode *, m_pOthersStone);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMyStone", CCNode *, m_pMyStone);
    return false;
}

SEL_MenuHandler RankUser::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackTouch", RankUser::callBackTouch);
	return NULL;
}

void RankUser::setTargetInfo(pvp::ChTargetInfo * pInfo)
{
	CC_SAFE_RETAIN(pInfo);
	CC_SAFE_RELEASE(m_pInfo);
	m_pInfo = pInfo;
	updateView();
}

void RankUser::updateView()
{
	setVisible(m_pInfo != NULL);
	if (m_pInfo == NULL)
	{
		return;
	}
	
	m_pBMFRank->setString(D_CSTRING_FROM_UINT(m_pInfo->getRank()));
	CCLOG("rankuser--name, %s", m_pInfo->getName().c_str());
	m_pTTFName->setString(m_pInfo->getName().c_str());
	//m_pTTFName->setVerticalTextStyle(kCCVerticalTextStyleRightToLeft);
	m_pTTFPoint->setString(D_CSTRING_FROM_UINT(m_pInfo->getPointGrow()));
	m_pNodeHead->removeAllChildrenWithCleanup(true);
	m_pNodeHead->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon25PForPvp(m_pInfo->getAvatarId()));
	m_pNodeTop->setVisible(m_pInfo->getRank() <= D_CONFIG_UINT_FOR_KEY(PVP_TOPLIST_NUM));
	m_pNodeBottom->setVisible(!m_pNodeTop->isVisible());

	//是不是自己
	m_pMyStone->setVisible(m_pInfo->getRank() == DM_GET_PVP_MODEL->getChallengeInfo()->getRank());
	m_pOthersStone->setVisible(!m_pMyStone->isVisible());
}

void RankUser::callBackTouch(CCObject *pObj)
{
	if (m_pDelegate)
	{
		m_pDelegate->showTargetInfo(m_pInfo);
	}
// 	if (m_pDelegate)
// 	{
// 		m_pDelegate->willChallenge(m_pInfo);
// 	}
}