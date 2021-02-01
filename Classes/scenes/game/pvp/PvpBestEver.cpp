//
//  PvpBestEver.cpp
//  账户余额不足
//
//  Created by cll on 13-6-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PvpBestEver.h"
#include "managers/GameManager.h"

USING_NS_CC;


/************************ PvpBestEver class begin*******************/


PvpBestEver::PvpBestEver()
	:m_pCloseButton(NULL)
	,m_pConfirmButton(NULL)
	,m_pHigh(NULL)
	,m_pNow(NULL)
	,m_pGrow(NULL)
	,m_pAward(NULL)
	,m_pNode123(NULL)
	,m_pFirst(NULL)
	,m_pSecond(NULL)
	,m_pThird(NULL)
{
}

PvpBestEver::~PvpBestEver()
{
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pHigh);
	CC_SAFE_RELEASE(m_pNow);
	CC_SAFE_RELEASE(m_pGrow);
	CC_SAFE_RELEASE(m_pAward);
	CC_SAFE_RELEASE(m_pNode123);
	CC_SAFE_RELEASE(m_pFirst);
	CC_SAFE_RELEASE(m_pSecond);
	CC_SAFE_RELEASE(m_pThird);
}


bool PvpBestEver::init()
{
	if(!DPopup::init(true))
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_lunjian/PL_lunjian_zuigao");

	m_pCloseButton->getButton()->setTarget(this, menu_selector(PvpBestEver::closeCallBack));
	m_pConfirmButton->getButton()->setTarget(this, menu_selector(PvpBestEver::closeCallBack));
	
	//通常情况
	m_pNow->setVisible(true);
	m_pNode123->setVisible(false);

	m_pHigh->setString(CCString::createWithFormat("%d" , DM_GET_PVP_MODEL->getOldBest())->getCString());
	m_pNow->setString(CCString::createWithFormat("%d" , DM_GET_PVP_MODEL->getNewBest())->getCString());
	m_pGrow->setString(CCString::createWithFormat("%d" , DM_GET_PVP_MODEL->getOldBest() - DM_GET_PVP_MODEL->getNewBest())->getCString());
	m_pAward->setString(CCString::createWithFormat("x%d" , DM_GET_PVP_MODEL->getBestRewardPoint())->getCString());
	
	//1，2，3名的情况
	if(DM_GET_PVP_MODEL->getNewBest() <= 3){
		m_pNow->setVisible(false);
		m_pNode123->setVisible(true);

		m_pFirst->setVisible(DM_GET_PVP_MODEL->getNewBest() == 1);
		m_pSecond->setVisible(DM_GET_PVP_MODEL->getNewBest() == 2);
		m_pThird->setVisible(DM_GET_PVP_MODEL->getNewBest() == 3);
	}

	return true;

}

bool PvpBestEver::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, this->m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHigh", CCLabelBMFont *, this->m_pHigh);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNow", CCLabelBMFont *, this->m_pNow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrow", CCLabelBMFont *, this->m_pGrow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAward", CCLabelBMFont *, this->m_pAward);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode123", CCNode *, this->m_pNode123);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFirst", CCNode *, this->m_pFirst);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSecond", CCNode *, this->m_pSecond);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pThird", CCNode *, this->m_pThird);
	return false;
}



void PvpBestEver::closeCallBack(cocos2d::CCObject *obj)
{
	closePopup();
}


