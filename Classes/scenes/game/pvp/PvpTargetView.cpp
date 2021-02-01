//
//  PvpTargetView.cpp
//  查看对象
//
//  Created by Eci on 14-8-1.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "PvpTargetView.h"
#include "Pvp.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace drop;


/************************ pvp exchange select class begin*******************/



PvpTargetView::PvpTargetView()
	: m_pDBtnClose(NULL)
	, m_pDBtnChallenge(NULL)
	, m_pDBtnView(NULL)
	, m_pTTFName(NULL)
	, m_pBMFFight(NULL)
	, m_pBMFWin(NULL)
	, m_pBMFLose(NULL)
	, m_pBMFRate(NULL)
	, m_pLevel(NULL)
	, m_pInfo(NULL)
	, m_pFormationInfo(NULL)
	, m_pDelegate(NULL)
{
    D_RESET_C_ARRAY(m_pWarrior, FORMATION_POS_MAX);
}

PvpTargetView::~PvpTargetView()
{
	CC_SAFE_RELEASE(m_pDBtnClose);
	CC_SAFE_RELEASE(m_pDBtnChallenge);
	CC_SAFE_RELEASE(m_pDBtnView);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pBMFFight);
	CC_SAFE_RELEASE(m_pInfo);
	D_SAFE_RELEASE_C_ARRAY(m_pWarrior, FORMATION_POS_MAX);
	SAFE_RELEASE_UNBINDING_ALL(m_pFormationInfo, this);
}


bool PvpTargetView::init()
{
	if(!DPopup::init())
	{
		return false;
	}

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorHead", WarriorHeadLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_lunjian/PL_lunjian_chakan", pNodeLoaderLibrary);

	m_pDBtnClose->getButton()->setTarget(this, menu_selector(PvpTargetView::callBackClose));
	m_pDBtnChallenge->getButton()->setTarget(this, menu_selector(PvpTargetView::callBackChallenge));
	m_pDBtnView->getButton()->setTarget(this, menu_selector(PvpTargetView::callBackView));
	//m_pDBtnView->setVisible(false);//fixme
	return true;

}

void PvpTargetView::onEnter()
{
	if(!m_pFormationInfo->getIsRobot())
	{
		if(!m_pFormationInfo->isSynced || m_pFormationInfo->isTimeOut())
		{
			BINDING_ONLY(m_pFormationInfo, this, PvpTargetView::updateFormation, D_EVENT_OTHERFORMATION_UPDATE);
			HTTP_CLIENT->getOtherFormation(m_pFormationInfo->getID());
		}
	}
	updateFormation();
    DPopup::onEnter();
}

void PvpTargetView::onExit()
{
	SAFE_RELEASE_UNBINDING_ALL(m_pFormationInfo, this);
	m_pFormationInfo = NULL;
	DPopup::onExit();
}

bool PvpTargetView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnClose", DButton *, m_pDBtnClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnChallenge", DButton *, m_pDBtnChallenge);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnView", DButton *, m_pDBtnView);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFFight", CCLabelBMFont *, m_pBMFFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFWin", CCLabelBMFont *, m_pBMFWin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFLose", CCLabelBMFont *, m_pBMFLose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRate", CCLabelBMFont *, m_pBMFRate);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, m_pLevel);
	D_CCB_ASSIGN_ARRAY("m_pWarrior", WarriorHead *, m_pWarrior, FORMATION_POS_MAX);
	return false;
}

void PvpTargetView::callBackClose(CCObject *pObj)
{
	closePopup();
}

void PvpTargetView::setInfo(pvp::ChTargetInfo *pInfo)
{
	CC_SAFE_RETAIN(pInfo);
	CC_SAFE_RELEASE(m_pInfo);
	m_pInfo = pInfo;
	m_pTTFName->setString(pInfo->getName().c_str());

	if(!m_pInfo->isRobot())
	{
		m_pFormationInfo = DM_GET_OTHERFORMATION_MODEL->getForamtionInfoByUid(m_pInfo->getUserId());
	}
	else
	{
		m_pDBtnView->getButton()->setEnabled(false);
		m_pFormationInfo = DM_GET_OTHERFORMATION_MODEL->getRobotFormationInfoByFid(m_pInfo->getRobotFormation()->fid,m_pInfo->getLevel() , m_pInfo->getRank());
	}
	CC_SAFE_RETAIN(m_pFormationInfo);
	m_pFormationInfo->getFormationTotalForce();
	m_pBMFWin->setString(D_CSTRING_FROM_UINT(pInfo->getWin()));
	m_pBMFLose->setString(D_CSTRING_FROM_UINT(pInfo->getLose()));
	m_pLevel->setString(D_CSTRING_FROM_UINT(pInfo->getVipLevel()));
	float rate = 0.f;
	if (pInfo->getWin() != 0 || pInfo->getLose() != 0)
	{
		rate = (float)pInfo->getWin() / (pInfo->getWin() + pInfo->getLose());
	}
	m_pBMFRate->setString(CCString::createWithFormat("%.1f%%", rate * 100)->getCString());

	//m_pDBtnChallenge->getButton()->setEnabled(m_pInfo->getRank() < DM_GET_PVP_MODEL->getChallengeInfo()->getRank());

	//等级相差过大 不可挑战
	CCArray *pTargets = CCArray::create();
	DM_GET_PVP_MODEL->getTarges(pTargets);
	bool valid = false;
	for (int i = 0; i < pTargets->count(); i++)
	{
		pvp::ChTargetInfo * target = dynamic_cast<pvp::ChTargetInfo *>(pTargets->objectAtIndex(i));
		if (target->getRank() == m_pInfo->getRank())
		{
			valid = true;
			break;
		}
	}
	m_pDBtnChallenge->getButton()->setEnabled(valid);

	//排名比自己靠后，包括自己 也不可挑战，前10名可以任意挑战
	if(DM_GET_PVP_MODEL->getChallengeInfo()->getRank() <= 10 && m_pInfo->getRank() <= 10){
		if(m_pInfo->getRank() == DM_GET_PVP_MODEL->getChallengeInfo()->getRank())
		{
			m_pDBtnChallenge->getButton()->setEnabled(false);
		}
	} else {
		if(m_pInfo->getRank() >= DM_GET_PVP_MODEL->getChallengeInfo()->getRank())
		{
			m_pDBtnChallenge->getButton()->setEnabled(false);
		}
	}
	updateFormation();
}

void PvpTargetView::callBackDelay()
{
	m_pDBtnChallenge->getButton()->setEnabled(true);
}

void PvpTargetView::callBackChallenge(CCObject *pObj)
{
	//点击后禁用
	m_pDBtnChallenge->getButton()->setEnabled(false);
	this->runAction(CCSequence::create(CCDelayTime::create(2.5f),CCCallFunc::create(this, callfunc_selector(PvpTargetView::callBackDelay)),NULL));

	DM_GET_BATTLE_MODEL->setBattleType(battle::kBattleTypePvp);

	if (m_pInfo && m_pDelegate)
	{
		DM_GET_OTHERFORMATION_MODEL->setCurrentWarriorId(m_pFormationInfo->getID());
		m_pDelegate->willChallenge(m_pInfo);
	}
	closePopup();
}

void PvpTargetView::callBackView(CCObject *pObj)
{
	//return;//fixme
	if (m_pFormationInfo && m_pDelegate)
	{
		m_pDelegate->showOtherFormation(m_pFormationInfo);
	}
	closePopup();
}

void PvpTargetView::updateFormation()
{
	if(!m_pFormationInfo->isSynced || m_pFormationInfo->isTimeOut()) return;

	uint32_t count = m_pFormationInfo->getPositionsCount();

	for(uint32_t i = 0; i < count; ++i)
	{
		formation::PositionInfo* pPos = m_pFormationInfo->getPositionInfoByIndex(i);
		m_pWarrior[i]->setWarriorBaseInfo(warrior::WarriorCurInfo::create(pPos->getWarriorId())->getBaseInfo());
		m_pWarrior[i]->getIconSprite()->removeAllChildren();
		CCNode *pHead = ResourceManager::sharedResourceManager()->getWarriorHeadIcon(pPos->getWarriorId(), CCSizeMake(90, 90));
		pHead->setAnchorPoint(CCPointZero);
		m_pWarrior[i]->getIconSprite()->addChild(pHead);
	}
	m_pBMFFight->setString(D_CSTRING_FROM_UINT(m_pFormationInfo->getFormationTotalForce()));
}