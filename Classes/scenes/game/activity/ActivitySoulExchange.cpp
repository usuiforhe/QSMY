//
//  ActivitySoulExchange.cpp
//  活动
//
//  Created by Eci on 14-3-24.
//
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#define K_SOUL_EXCHANGE_INDEX 16

#include "ActivitySoulExchange.h"
#include "managers/ResourceManager.h"
#include "../warrior/WarriorInfo.h"
USING_NS_CC;
USING_NS_CC_EXT;

ActivitySoulExchange::ActivitySoulExchange()
	: m_pBMFPoint(NULL)
	, m_pMyPoint(NULL)
	, m_pBtnEnter(NULL)
	, m_pNode0Container(NULL)
	, m_pAnimateSoulPoint(NULL)
	, m_pModel(NULL)
{
    D_RESET_C_ARRAY(m_pNode1Container, K_NODE_CONTAINER_MAX);
	D_RESET_C_ARRAY(m_pBMFSoul, K_BMF_SOUL_MAX);
	D_RESET_C_ARRAY(m_pNode, K_NODE_SOUL_MAX);
}

ActivitySoulExchange::~ActivitySoulExchange()
{
	CC_SAFE_RELEASE(m_pBMFPoint);
	CC_SAFE_RELEASE(m_pMyPoint);
	CC_SAFE_RELEASE(m_pBtnEnter);
	CC_SAFE_RELEASE(m_pNode0Container);
	CC_SAFE_RELEASE(m_pAnimateSoulPoint);
	D_SAFE_RELEASE_C_ARRAY(m_pNode1Container, K_NODE_CONTAINER_MAX);
	D_SAFE_RELEASE_C_ARRAY(m_pBMFSoul, K_BMF_SOUL_MAX);
	D_SAFE_RELEASE_C_ARRAY(m_pNode, K_NODE_SOUL_MAX);
}

bool ActivitySoulExchange::init()
{

	if(!DLayer::init())
	{
		return false;
	}


	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorHeadSmall", WarriorHeadSmallLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_huodong/PL_hunpoduihuan/pl_hunpodiuhuan_chouka", pNodeLoaderLibrary);

	m_pModel = DM_GET_ACTIVITY_MODEL;

	for (int i = 0; i < K_BMF_SOUL_MAX; i++)
	{
		//m_pBMFSoul[i]->setString(D_CSTRING_FROM_UINT(m_pModel->getSoulPointInfo()->getSoulPointFromCardAtIndex(i)));
		m_pBMFSoul[i]->setString(CCString::createWithFormat("+%d" , m_pModel->getSoulPointInfo()->getSoulPointFromCardAtIndex(i))->getCString());
	}

	for (int i = 0; i < K_NODE_CONTAINER_MAX; i++)
	{
		m_pNode1Container[i]->getMenuItem()->setTarget(this , menu_selector(ActivitySoulExchange::onHeroClick));
	}
	
	m_pBtnEnter->setTarget(this, menu_selector(ActivitySoulExchange::callBackEnter));

	this->setVisible(false);

	return true;
}

void ActivitySoulExchange::onHeroClick(CCObject* target)
{
	CCNode* pMenuItem = NULL;
	pMenuItem = dynamic_cast<CCNode*>(target);
	if(pMenuItem && pMenuItem->getTag() > 0)
	{
		int wid = pMenuItem->getTag();
		warrior::WarriorBaseInfo *pInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wid);
		if(pInfo)
		{
			WarriorInfoView *m_pWarriorInfoView = WarriorInfoView::create(WarriorInfoView::kTypeCloseOnly);
			m_pWarriorInfoView->setWarrior(pInfo);
			PL_MOVE_NODE_BELOW_CENTER_Y(m_pWarriorInfoView);
			addChild(m_pWarriorInfoView);
		}		
	}
}

void ActivitySoulExchange::onEnter()
{
	m_pModel->syncFromServer();
	
	BINDING_EXEC(m_pModel, this, ActivitySoulExchange::updateView, D_EVENT_ACTIVITY_SOUL_EXCHANGE_REFRESH);
	BINDING_EXEC(m_pModel, this, ActivitySoulExchange::updateVisible, D_EVENT_ACTIVITY_REFRESH_CPP);
	DLayer::onEnter();
}

void ActivitySoulExchange::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	DLayer::onExit();
}

void ActivitySoulExchange::updateView()
{
	updateVisible();
	if (!this->isVisible())
	{
		return;
	}

	m_pMyPoint->setString(D_CSTRING_FROM_UINT(DM_GET_USER_MODEL->getUserInfo()->getPoints()));
	m_pBMFPoint->setString(D_CSTRING_FROM_UINT(m_pModel->getSoulPointInfo()->getSoulPoint()));
	bool bVisible = m_pModel->getSoulPointInfo()->getWarriorID().size() == 1;
	m_pNode[0]->setVisible(bVisible);
	if (bVisible)
	{
		m_pNode0Container->setWarriorInfo(DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pModel->getSoulPointInfo()->getWarriorID()[0]));
	}

	bVisible = m_pModel->getSoulPointInfo()->getWarriorID().size() == K_NODE_CONTAINER_MAX;
	m_pNode[1]->setVisible(bVisible);
	for (int i = 0; i < K_NODE_CONTAINER_MAX; i++)
	{
		if (bVisible)
		{
			m_pNode1Container[i]->setWarriorInfo(DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pModel->getSoulPointInfo()->getWarriorID()[i]));
			//set TAG with wid
			m_pNode1Container[i]->getMenuItem()->setTag(m_pModel->getSoulPointInfo()->getWarriorID()[i]);
		}
		
	}
	if (m_pModel->getSoulPointInfo()->getSoulPointAdded() > 0)
	{
		//m_pAnimateSoulPoint->playAtIndex(0);
		m_pModel->getSoulPointInfo()->setSoulPointAdded(0);
	}
	
}

void ActivitySoulExchange::updateVisible()
{
	this->setVisible(m_pModel->getSoulPointInfo()->isOpen());
	int userlevel = DM_GET_USER_MODEL->getUserInfo()->getUserLv();
	if(userlevel < 2) this->setVisible(false);
}

bool ActivitySoulExchange::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMyPoint", CCLabelBMFont *, m_pMyPoint);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFPoint", CCLabelBMFont *, m_pBMFPoint);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnEnter", CCMenuItem *, m_pBtnEnter);
	D_CCB_ASSIGN_ARRAY("m_pBMFSoul", CCLabelBMFont *, m_pBMFSoul, K_BMF_SOUL_MAX);
	D_CCB_ASSIGN_ARRAY("m_pNode1Container", WarriorHeadSmall *, m_pNode1Container, K_NODE_CONTAINER_MAX);
	D_CCB_ASSIGN_ARRAY("m_pNode", CCNode *, m_pNode, K_NODE_SOUL_MAX);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode0Container0", WarriorHeadSmall *, m_pNode0Container);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimationSoulPoint", UIAnimate *, m_pAnimateSoulPoint);
	return false;
}

void ActivitySoulExchange::callBackEnter(CCObject *pSender)
{
	Game::sharedGame()->NavigateTo(navigation::kNavActivity, true, CCInteger::create(K_SOUL_EXCHANGE_INDEX));
}