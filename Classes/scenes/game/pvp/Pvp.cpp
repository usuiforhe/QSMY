//
//  Pvp.cpp
//  QSMY论剑
//
//  Created by cll on 13-4-20.
//
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Pvp.h"
#include "../user/UserInfo.h"
#include "PvpView.h"
#include "PvpTopRank.h"
#include "PvpExchangeView.h"
#include "../inventory/ItemInfoView.h"
#include "managers/GameManager.h"
#include "../Game.h"
#include "model/OtherFormationModel.h"
#include "PvpTargetView.h"

#include "PvpBestEver.h"
#include "../warrior/WarriorInfo.h"
#include "../friend/FriendFormation.h"
#include "../equip/EquipInfoView.h"

USING_NS_CC;

#define PVP_RANK_TAG	0
#define PVP_EXCHANGE_TAG	1

#define  PVP_TITLE_OFFSET_Y		150

#define  PVP_TAB_RECT    CCRectMake(0, PVP_TITLE_OFFSET_Y, 640, 650)

#define ERROR_INVALID_RANK	"2206"

/************************ pvp class begin*****************************/

Pvp::Pvp()
:m_pContainer(NULL)
//,m_pItemInfoView(NULL)
,m_pDelegate(NULL)
// ,m_pPoint(NULL)
// ,m_pRemainChTimes(NULL)
// ,m_pPointGrow(NULL)
,m_pBtnRank(NULL)
,m_pBtnExchange(NULL)
,m_pPvpView(NULL)
,m_pExchangeView(NULL)
,m_pNodeBg(NULL)
,m_pBMFCoins(NULL)
,m_pBMFPoints(NULL)
,m_pBMFFight(NULL)
,m_pPBEndu(NULL)
,m_pBMFEndu(NULL)
,bFirstEnter(true)
{
    
}


Pvp::~Pvp()
{
    removeAllChildren();
	CC_SAFE_RELEASE(m_pContainer);
// 	CC_SAFE_RELEASE(m_pPoint);
// 	CC_SAFE_RELEASE(m_pRemainChTimes);
// 	CC_SAFE_RELEASE(m_pPointGrow);
	CC_SAFE_RELEASE(m_pBtnRank);
	CC_SAFE_RELEASE(m_pBtnExchange);
	CC_SAFE_RELEASE(m_pNodeBg);
	CC_SAFE_RELEASE(m_pBMFCoins);
	CC_SAFE_RELEASE(m_pBMFPoints);
	CC_SAFE_RELEASE(m_pBMFFight);
	CC_SAFE_RELEASE(m_pPBEndu);
	CC_SAFE_RELEASE(m_pBMFEndu);
	//CC_SAFE_RELEASE(m_pItemInfoView);
}


Pvp* Pvp::create(NavigationDelegate* pDelegate)
{
	Pvp *pRet = new Pvp();
	if (pRet && pRet->init(pDelegate))
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		CC_SAFE_DELETE(pRet); 
		return NULL; 
	} 
}


bool Pvp::init(NavigationDelegate* pDelegate)
{
	if(!DLayer::init())
	{
		return false;
	}
    
	DM_GET_PVP_MODEL->isSynced = false;
	m_pDelegate = pDelegate;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_lunjian/PL_lunjian");

	m_pBtnRank->setTag(PVP_RANK_TAG);
	m_pBtnRank->setTarget(this, menu_selector(Pvp::callBackTab));
	m_pBtnExchange->setTag(PVP_EXCHANGE_TAG);
	m_pBtnExchange->setTarget(this, menu_selector(Pvp::callBackTab));

// 	m_pContainer = DLayer::create();
// 	m_pContainer->retain();
// 	addChild(m_pContainer);
	return true;
}

void Pvp::createView()
{
//     DTabView* pTabView = DTabView::create(PVP_TAB_RECT,240);
//     
// 	PvpView * pvpView = PvpView::create();
// 	pvpView->setDelegate(this);
// 	pTabView->addTabWithFontImg(pvpView,"ui/common/font_sheet_lunjian.png");
//     
// 	PvpTopRankView * pvpTopRankView = PvpTopRankView::create();
// 	pvpTopRankView->setDelegate(this);
// 	pTabView->addTabWithFontImg(pvpTopRankView,"ui/common/font_sheet_qianshi.png");
//     
// 	PvpExchangeView * pvpAwardView = PvpExchangeView::create();
// 	pvpAwardView->setDelegate(this);
// 	pTabView->addTabWithFontImg(pvpAwardView,"ui/common/font_sheet_lingjiang.png");
//     
// 	m_pContainer->addChild(pTabView);
	showRankView();
}

void Pvp::onPopupClose(DPopup*)
{
    setVisible(true);
}

void Pvp::showOtherFormation(otherFormation::FormationInfo* pInfo)
{
	FriendFormation* pFormation = FriendFormation::create();
	pFormation->setInfo(pInfo->getID() , pInfo->getIsRobot() , pInfo);
	addChild(pFormation);
}
void Pvp::showOtherFormation(unsigned int tid)
{
	//机器人，不处理
	if(tid == 0)return;

	FriendFormation* pFormation = FriendFormation::create();
	pFormation->setInfo(tid);
	addChild(pFormation);
}

void Pvp::willChallenge(pvp::ChTargetInfo * m_pTargetInfo, bool isBoss /* = false */, bool isRobot /* = false */)
{
	if (m_pTargetInfo)
	{

		CCArray *pTargets = CCArray::create();
		DM_GET_PVP_MODEL->getTarges(pTargets);
		bool valid = false;
		for (int i = 0; i < pTargets->count(); i++)
		{

			pvp::ChTargetInfo * target = dynamic_cast<pvp::ChTargetInfo *>(pTargets->objectAtIndex(i));
			if (target->getRank() == m_pTargetInfo->getRank())
			{
				valid = true;
			}

		}
		if (valid == false)
		{
			DM_GET_ERRORTIP_MODEL->showErrorTips(ERROR_INVALID_RANK);
			return;
		}
	}
	
	
	pvp::ChallengeInfo * info = DM_GET_PVP_MODEL->getChallengeInfo();
	if(info->getRemainChallengeTimes() <= 0)
	{
		int nVipLv = DM_GET_USER_MODEL->getUserInfo()->getVipLevel();
		if (nVipLv == 0)
		{
			nVipLv = 1;
		}

		user::VipInfo *pCurInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(nVipLv);
		int nMaxLv = DM_GET_USER_MODEL->getMaxVipLevel();
		for (int i = nVipLv + 1; i <= nMaxLv; i++)
		{
			user::VipInfo *pInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(i);
			if (pInfo->getPvpMax() > pCurInfo->getPvpMax())
			{
				NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV,CCString::create(D_LOCAL_STRING("PVP_NEED_VIP%d",i)));
				return;
			}

		}
		TIPSMANAGER->warning(D_LOCAL_STRING("PvpNotEnoughChTimes"));
		return;
	}
	if(isBoss && info->getRemainBossTimes() <= 0)
	{
		TIPSMANAGER->warning(D_LOCAL_STRING("PvpNotEnoughBossTimes"));
		return;
	}
	if (isBoss)
	{
		HTTP_CLIENT->chPvpBoss(info->getRank(), DM_GET_PVP_MODEL, callfuncJson_selector(PvpModel::handleError));
		return;
	}
	else if (isRobot)
	{
		HTTP_CLIENT->chPvpNPC(info->getRank(), DM_GET_PVP_MODEL, callfuncJson_selector(PvpModel::handleError));
		return;
	}
	
	HTTP_CLIENT->chPvpTarget(m_pTargetInfo->getUserId(), m_pTargetInfo->getRank(), info->getRank(), DM_GET_PVP_MODEL, callfuncJson_selector(PvpModel::handleError));
}

void Pvp::showTargetInfo(pvp::ChTargetInfo *pInfo)
{
	PvpTargetView *pView = PvpTargetView::create();
	pView->setInfo(pInfo);
	pView->setDelegate(this);
	Game::sharedGame()->openPopup(pView);
}

//void Pvp::closeInfoView(InventoryType type)
//{
//    if(m_pItemInfoView != NULL)
//    {
//        item::ItemInfo* item = NULL;
//        m_pItemInfoView->setItemInfo(item);
//        m_pItemInfoView->removeFromParent();
//    }
//}

void Pvp::showAwardInfo(pvp::ChMallItem * pAwardData)
{
	if(!pAwardData) return;

	//显示奖品详情
	switch (pAwardData->getItemType())
	{
	case drop::kDropTypeItem:
	case drop::kDropTypePkg:
		{
			item::ItemInfo* pInfo = item::ItemInfo::create(pAwardData->getItemID() , 1);
			if(pInfo)
			{
				if(pInfo->getBaseInfo()->getItemType() == item::kItemTypeFragment)
				{

					EquipInfoView *m_pEquipInfoView = EquipInfoView::create(EquipInfoView::kTypeViewOnly);
					PL_MOVE_NODE_ABOVE_CENTER_Y(m_pEquipInfoView);			
					equip::EquipCurInfo* pEquip = equip::EquipCurInfo::create(pInfo->getBaseInfo()->getResID());
					m_pEquipInfoView->setEquip(pEquip);
					addChild(m_pEquipInfoView);
				}
				else
				{
					ItemInfoView * m_pItemInfoView = ItemInfoView::create();
					m_pItemInfoView->setItemInfo(pInfo);
					addChild(m_pItemInfoView);
				}
			}			
			break;
		}
	case drop::kDropTypeWarrior:
	case drop::kDropTypeSoul:
		{
			warrior::WarriorCurInfo* pWarrior = warrior::WarriorCurInfo::create(pAwardData->getItemID());
			if(pWarrior)
			{
				WarriorInfoView* m_pSoulInfoView = WarriorInfoView::create(WarriorInfoView::kTypeCloseOnly);			
				m_pSoulInfoView->setWarrior(pWarrior);
				addChild(m_pSoulInfoView);
				PL_MOVE_NODE_BELOW_CENTER_Y(m_pSoulInfoView);
			}			
			break;
		}
	case drop::kDropTypeEquip:
		{
			EquipInfoView *m_pEquipInfoView = EquipInfoView::create(EquipInfoView::kTypeViewOnly);
			PL_MOVE_NODE_ABOVE_CENTER_Y(m_pEquipInfoView);			
			equip::EquipCurInfo* pEquip = equip::EquipCurInfo::create(pAwardData->getItemID());
			m_pEquipInfoView->setEquip(pEquip);
			addChild(m_pEquipInfoView);	
			break;
		}
	case drop::kDropTypeNone:
	default:;
	}
}

//void Pvp::showInfoView(InventoryType type,item::ItemInfo* pItemInfo,cocos2d::CCObject* param)
//{
//    if(m_pItemInfoView == NULL)
//    {
//        m_pItemInfoView = ItemInfoView::create();
//        m_pItemInfoView->setDelegate(this);
//        m_pItemInfoView->retain();
//    }
//    if(m_pItemInfoView->getParent())
//    {
//        m_pItemInfoView->removeFromParent();
//    }
//    m_pItemInfoView->setItemInfo(pItemInfo);
//    addChild(m_pItemInfoView);
//}

void Pvp::onEnter()
{
    createView();
    DM_GET_PVP_MODEL->getInitInfo();
	BINDING_EXEC(DM_GET_PVP_MODEL, this, Pvp::updateView, D_EVENT_PVP_INIT);
	BINDING_ONLY(DM_GET_PVP_MODEL, this, Pvp::updateView, D_EVENT_PVPINFO_UPDATE);
	BINDING_ONLY(DM_GET_USER_MODEL->getUserInfo(), this, Pvp::updateView, D_EVENT_USERINFO_UPDATE);
	
	DLayer::onEnter();

	if(DM_GET_PVP_MODEL->getBestEver() == true)
	{
		//置位
		DM_GET_PVP_MODEL->setBestEver(false);
		Game::sharedGame()->openPopup(PvpBestEver::create());
		HTTP_CLIENT->getNewMails(NULL,NULL,true,false); //悄悄的
	}

	if(DM_GET_PVP_MODEL->getTargetID() > 0)
	{
		unsigned int tid = DM_GET_PVP_MODEL->getTargetID();
		//置位
		DM_GET_PVP_MODEL->setTargetID(0);
		showOtherFormation(tid);
	}
}

void Pvp::onExit()
{
    UNBINDING_ALL(DM_GET_PVP_MODEL, this);
	UNBINDING_ALL(DM_GET_USER_MODEL->getUserInfo(), this);
	DLayer::onExit();
    m_pContainer->removeAllChildren();
	m_pPvpView = NULL;
	m_pExchangeView = NULL;
}

void Pvp::updateView()
{

	user::UserInfo *pInfo = DM_GET_USER_MODEL->getUserInfo();

	m_pBMFCoins->setString(D_CSTRING_FROM_UINT(pInfo->getCoins()));
	m_pBMFPoints->setString(D_CSTRING_FROM_UINT(pInfo->getPoints()));
	m_pBMFFight->setString(D_CSTRING_FROM_UINT(DM_GET_FORMATION_MODEL->getFormationTotalForce()));

	user::UserBase * pUserBase = DM_GET_USER_MODEL->getUserBaseByLv(pInfo->getUserLv());
	CCString * str_endu = CCString::createWithFormat("%d/%d", pInfo->getEndurance(), pUserBase->getMaxEndu());
	m_pBMFEndu->setString(str_endu->getCString());
	float percent = (float)pInfo->getEndurance() / pUserBase->getMaxEndu();
	if(percent > 1.0f) percent = 1.0f;
	m_pPBEndu->setPercent(percent, 0.0f);
//     if(!DM_GET_PVP_MODEL->isSynced)
//     {
//         return;
//     }

//    pvp::ChallengeInfo * info = DM_GET_PVP_MODEL->getChallengeInfo();
//     m_pPoint->setString(CCString::createWithFormat("%d", info->getPoint())->getCString());
//     m_pRemainChTimes->setString(CCString::createWithFormat("%d", info->getRemainChallengeTimes())->getCString());
//     m_pPointGrow->setString(CCString::createWithFormat("%d", info->getPointGrow())->getCString());    
}

bool Pvp::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPoint", CCLabelBMFont *, this->m_pPoint);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRemainChTimes", CCLabelBMFont *, this->m_pRemainChTimes);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPointGrow", CCLabelBMFont *, this->m_pPointGrow);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRank", CCMenuItem *, this->m_pBtnRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnExchange", CCMenuItem *, this->m_pBtnExchange);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeBg", CCNode *, this->m_pNodeBg);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCoins",CCLabelBMFont *, this->m_pBMFCoins);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFPoints", CCLabelBMFont *, this->m_pBMFPoints);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFFight", CCLabelBMFont *, this->m_pBMFFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPBEndu", DProgressBar *, this->m_pPBEndu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFEndu", CCLabelBMFont *, this->m_pBMFEndu);

    return false;
}

void Pvp::menuCallback(CCObject * obj)
{
	CCMenuItemImage * pMenuItem = (CCMenuItemImage*) obj;
	int tag = pMenuItem->getTag();

	if (m_pDelegate)
	{
		m_pDelegate->NavigateTo((navigation::NavigationType)tag);
	}
	
}


void Pvp::callBackTab(cocos2d::CCObject *pObj)
{
	CCMenuItem *pBtn = static_cast<CCMenuItem *>(pObj);
	if (pBtn->getTag() == PVP_RANK_TAG)
	{
		showRankView();
	}
	else
	{
		showExchangeView();
	}
}

void Pvp::showRankView()
{
	if (m_pPvpView == NULL)
	{
		PvpView *pvpView = PvpView::create();
		pvpView->setSlideEnabled(bFirstEnter);
		bFirstEnter = false;
		pvpView->setDelegate(this);
		m_pContainer->addChild(pvpView);
		m_pPvpView = pvpView;
	}
	else
	{
		m_pPvpView->setVisible(true);
	}
	if (m_pExchangeView != NULL)
	{
		m_pExchangeView->setVisible(false);
	}
	m_pBtnRank->setEnabled(false);
	m_pBtnRank->selected();
	m_pBtnExchange->setEnabled(true);
	m_pBtnExchange->unselected();
	m_pNodeBg->setVisible(true);
}

void Pvp::slideBackGround(float offset)
{
	m_pNodeBg->setPositionY(offset);
}

void Pvp::showExchangeView()
{
	if (m_pExchangeView == NULL)
	{
		PvpExchangeView *ExchangeView = PvpExchangeView::create();
		ExchangeView->setDelegate(this);
		m_pContainer->addChild(ExchangeView);
		m_pExchangeView = ExchangeView;
	}
	else
	{
		m_pExchangeView->setVisible(true);
	}
	if (m_pPvpView != NULL)
	{
		m_pPvpView->setVisible(false);
	}
	m_pBtnRank->setEnabled(true);
	m_pBtnRank->unselected();
	m_pBtnExchange->setEnabled(false);
	m_pBtnExchange->selected();
	m_pNodeBg->setVisible(false);
}

/************************ pvp class end*****************************/