//
//  Pve.cpp
//  进战斗的模块 小模块之间切换 不优化 节省内存
//
//  Created by wanghejun on 13-3-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Tower.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "model/TowerModel.h"

#include "network/HttpClient.h"

#include "TowerHome.h"
#include "TowerChooseBuff.h"
#include "TowerDifficulty.h"
#include "TowerContinue.h"
#include "TowerReward.h"
#include "TowerFailed.h"
#include "TowerRank.h"
#include "TowerInteral.h"
#include "utils/TimerUtil.h"
#include "utils/TimeUtil.h"
#include "../friend/FriendFormation.h"

USING_NS_CC;
USING_NS_CC_EXT;

Tower::Tower()
:m_currentShow(tower::KNavUnknow)
,m_currentPop(tower::KNavUnknow)
,m_pContainer(NULL)
,m_pPopLayer(NULL)
,m_pLastLayer(NULL)
,m_pInteral(NULL)
{

}

Tower::~Tower()
{
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pPopLayer);
	CC_SAFE_RELEASE(m_pInteral);
}

bool Tower::init()
{
	if (!DLayer::init())
	{
		return false;
	}

    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_huodong/PL_tianshoufuge/pl_tsfg_main");

	m_pPopLayer = CCLayerColor::create(ccc4(0,0,0,215),DESIGN_SIZE.width,DESIGN_SIZE.height);
	m_pPopLayer->retain();

	m_pTowerModel = DM_GET_TOWER_MODEL;
    
    NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfuncO_selector(Tower::theRoundEnd), D_EVENT_TOWER_TOWER_OUT_OF_DATE);

	// 继承与GundamDelegate，用在弹出物品信息
	//setContainer(this);
    //NavigateTo(tower::KNavUnknow);
	return true;
}

void Tower::forceUpdate(float dt)
{
    m_pTowerModel->dailyReset(0.f);
	HTTP_CLIENT->towerGet();
}

void Tower::theRoundEnd(CCObject*obj)
{
    CCString* pReason = dynamic_cast<CCString*>(obj);
    if(pReason) Tips_Alert(pReason->getCString());
    NavigateTo(tower::KNavHome);
    m_pTowerModel->reset();
	HTTP_CLIENT->towerGet();
}

bool Tower::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNode",		CCNode *,			m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pInteral",		CCLabelBMFont *,	m_pInteral);

	return false;
}

SEL_MenuHandler Tower::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchTowerRank", Tower::onTouchTowerRank);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchTowerInteral", Tower::onTouchTowerInteral);

	return NULL;
}

void Tower::onTouchTowerRank(CCObject *sender)
{
	NavigateTo(tower::kNavRank);
}

void Tower::onTouchTowerInteral(CCObject *sender)
{
	NavigateTo(tower::kNavInteral);
}

void Tower::ClosePopup()
{
	if (m_currentPop<=tower::KNavUnknow || m_currentPop>=tower::kNavMax)
	{
		return;
	}

	if (m_currentPop == tower::kNavFailed)
	{
		HTTP_CLIENT->towerGet();
	}

	if (m_currentPop > 0)
	{
		m_pPopLayer->removeAllChildren();
		m_pPopLayer->removeFromParent();
	}

	m_currentPop = tower::KNavUnknow;
}

void Tower::NavigateTo(tower::NavigationType navType)
{
	if (navType<=tower::KNavUnknow || navType>=tower::kNavMax)
	{
		navType = tower::KNavHome;
	}
    
    if(m_currentPop== navType || m_currentShow==navType) return;

	if (navType==tower::KNavHome || navType==tower::kNavDifficulty || navType==tower::kNavChooseBuff || navType==tower::kNavContinue)
	{
		m_currentShow	= navType;
		m_currentPop	= tower::KNavUnknow;
	}
	else
	{
		m_currentPop = navType;
	}
	CCNode *pFocus = NULL;
    
	m_pPopLayer->removeAllChildren();
	m_pPopLayer->removeFromParent();

	switch (navType)
	{
	case tower::KNavHome:
		{
			m_pContainer->removeAllChildren();
			pFocus = TowerHome::create();
			if (pFocus)
			{
                dynamic_cast<TowerChildDelegate*>(pFocus)->SetDelegate(this);
				m_pContainer->addChild(pFocus);
			}
		}
		break;
		case tower::kNavDifficulty:
			{
				m_pContainer->removeAllChildren();
				pFocus = TowerDifficulty::create();
				if (pFocus)
				{
                    dynamic_cast<TowerChildDelegate*>(pFocus)->SetDelegate(this);
					m_pContainer->addChild(pFocus);
				}
			}
			break;
		case tower::kNavChooseBuff:
			{
				m_pContainer->removeAllChildren();
				pFocus = TowerChooseBuff::create();
				if (pFocus)
				{
                    dynamic_cast<TowerChildDelegate*>(pFocus)->SetDelegate(this);
					m_pContainer->addChild(pFocus);
				}
			}
			break;
		case tower::kNavContinue:
			{
				m_pContainer->removeAllChildren();
				pFocus = TowerContinue::create();
				if (pFocus)
				{
                   dynamic_cast<TowerChildDelegate*>(pFocus)->SetDelegate(this);
					m_pContainer->addChild(pFocus);
				}
			}
			break;
		case tower::kNavRank:
			{
				pFocus = TowerRank::create();
				if (pFocus)
				{
                   dynamic_cast<TowerChildDelegate*>(pFocus)->SetDelegate(this);
					m_pPopLayer->addChild(pFocus);
					addChild(m_pPopLayer);
				}
			}
			break;
		case tower::kNavReward:
			{
				pFocus = TowerReward::create();
				if (pFocus)
				{
                    dynamic_cast<TowerChildDelegate*>(pFocus)->SetDelegate(this);
					m_pPopLayer->addChild(pFocus);
					addChild(m_pPopLayer);
				}
			}
			break;
		case tower::kNavFailed:
			{
				pFocus = TowerFailed::create();
				if (pFocus)
				{
                    dynamic_cast<TowerChildDelegate*>(pFocus)->SetDelegate(this);
					m_pPopLayer->addChild(pFocus);
					addChild(m_pPopLayer);
				}
			}
			break;
		case tower::kNavInteral:
			{
				pFocus = TowerInteral::create();
				if (pFocus)
				{
					dynamic_cast<TowerChildDelegate*>(pFocus)->SetDelegate(this);
					m_pPopLayer->addChild(pFocus);
					addChild(m_pPopLayer);
				}
			}
			break;
		default:
			break;
	}
}

void Tower::NotificationUpdate()
{
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
	time_t now = GameManager::sharedGameManager()->getServerTime();
	TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(Tower::forceUpdate), this,1 + TimeUtil::getDiff(now, TimeUtil::getStartTimeOfTomorrow(now)));
	tower::TowerInfo *pInfo = m_pTowerModel->GetTowerInfo();
	// 积分
	m_pInteral->setString(CCString::createWithFormat("%d", pInfo->points)->getCString());

	if (m_currentPop != tower::kNavInteral)
	{
		switch (pInfo->rstatus)
		{
		case tower::StatusTypeContinue:
			NavigateTo(tower::kNavContinue);
			break;
		case tower::StatusTypeFight:


			if (m_pTowerModel->IsNeedChooseBuf())
			{
				NavigateTo(tower::kNavChooseBuff);
			} 
			else
			{
				NavigateTo(tower::kNavDifficulty);
			}
			break;
		case tower::StatusTypeFailed:
		case tower::StatusTypeStart:
		default:
			NavigateTo(tower::KNavHome);
		}
	}
}


void Tower::onPopupClose(DPopup*)
{
    setVisible(true);
}

void Tower::showOtherFormation(otherFormation::FormationInfo* pInfo)
{
	FriendFormation* pFormation = FriendFormation::create();
	pFormation->setInfo(pInfo->getID() , pInfo->getIsRobot() , pInfo);
	addChild(pFormation);

	/*OtherFormationView* pOtherFormationView = OtherFormationView::create();
    pOtherFormationView->setPopupDelegate(this);
    setVisible(false);
    pOtherFormationView->setFormationInfo(pInfo);
    Game::sharedGame()->openPopup(pOtherFormationView);*/
}

void Tower::onEnter()
{
	if (!m_pTowerModel->isSynced)
	{
		HTTP_CLIENT->towerGet();
	}

	BINDING_EXEC(m_pTowerModel, this, Tower::NotificationUpdate, D_EVENT_TOWER_INFO_UPDATE);
	DLayer::onEnter();
}

void Tower::onExit()
{
	UNBINDING_ALL(m_pTowerModel, this);
    TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
	DLayer::onExit();
	ClosePopup();
    NavigateTo(tower::KNavUnknow);
}

void Tower::onBackKey()
{
	/*if (m_pItemInfoView && m_pItemInfoView->isRunning())
	{
	GundamDelegate::onBackKey();
	return;
	}

	if (m_pInfoView && m_pInfoView->isRunning())
	{
	GundamDelegate::onBackKey();
	return;
	}

	if (m_pPopLayer && m_pPopLayer->getParent())
	{

	ClosePopup();

	return;
	}

	BackKeyInterface::onBackKey();*/
}
