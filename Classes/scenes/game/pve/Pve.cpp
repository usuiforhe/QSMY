//
//  Pve.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//


#define SFX_PVE_EXPLORE CCString::create("pve_walk")
#define SFX_PVE_BATTLE CCString::create("pve_fight")

#include "Pve.h"
#include "AppEventType.h"
#include "../user/UserInfo.h"
#include "../Game.h"
#include "model/UserModel.h"
#include "managers/TipsManager.h"

#include "ChapterView.h"
#include "MapView.h"
#include "MapInfoView.h"
#include "PveReward.h"
#include "RecoverPower.h"
//#include "RecoverByItem.h"
#include "utils/TimeUtil.h"
#include "RushSettle.h"

#include "./effect/ExploreResultEffect.h"
#include "./effect/ExploreBattleEffect.h"
#include "./effect/ExploreEffect.h"
#include "./effect/DropWonderEffect.h"
#include "utils/TimerUtil.h"
#include "WonderDrop.h"
#include "../common/LoadingLayer.h"

#include "../inventory/BuyPopup.h"
#include "PveElite.h"
#include "TianqiChapterView.h"

USING_NS_CC;
USING_NS_CC_EXT;

Pve::Pve()
:m_pPveModel(NULL)
,m_pUserModel(NULL)
,m_pPopup(NULL)
,m_pRecoverPower(NULL)
,m_pRushSettle(NULL)
,m_pDialogs(NULL)
,currentDialogIndex(0)
,m_pMapInfoWithDialog(NULL)
,m_nLastShowMapID(0)
,m_nLastShowChapterID(0)
,m_bAfterBattle(false)
,m_bReplayDialog(false)
,m_bFirstPlay(false)
,m_bRushResult(false)
,m_pMapInfo(NULL)
,m_uBuyMid(0)
//,m_pRecoverByItem(NULL)
{}

Pve::~Pve()
{
    removeAllChildren();
	CC_SAFE_RELEASE(m_pPopup);
    CC_SAFE_RELEASE(m_pRecoverPower);
	//CC_SAFE_RELEASE(m_pRecoverByItem);
    CC_SAFE_RELEASE(m_pRushSettle);
	CC_SAFE_RELEASE(m_pDialogs);
}

bool Pve::init(pve::PveFightType type)
{
    if(!DLayer::init())
    {
        return false;
    }
    
    m_pPveModel     = DM_GET_PVE_MODEL;
    m_pUserModel    = DM_GET_USER_MODEL;
    m_pPveModel->setFightType(type);
	//CCB_READER_AND_ADDCHILD("ui/ccb/pve/Pve");
    
    m_pContainer = CCNode::create();
    addChild(m_pContainer);
    
    
	m_pPopup = CCLayerColor::create(ccc4(0,0,0,180));
	PL_MOVE_NODE_BELOW_CENTER_Y(m_pPopup);
	CC_SAFE_RETAIN(m_pPopup);
    
	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView());	//添加基本信息title
    
	m_pPveModel->getChapters();
	m_pPveModel->setShowChapter(true);
	m_pPveModel->getElites();
	if (m_pPveModel->isSynced)
	{
		user::UserInfo *pUserInfo = DM_GET_USER_MODEL->getUserInfo();
		uint32_t mid = pUserInfo->getMid();
		pve::MapInfo *pMapInfo = m_pPveModel->getMapInfo(mid);
		uint32_t chapter_id = pMapInfo->chapter_id;
		if (pMapInfo->getMapId() == pMapInfo->chapterInfo->lastMid)
		{
			pve::ChapterInfo *pNewChapter = DM_GET_PVE_MODEL->getChapterInfo(pMapInfo->chapter_id+1);
			if (pNewChapter)
			{
				chapter_id = pNewChapter->getCid();
				//pNewChapter->chapter_unlock = true; 解锁状态由下面更新地图信息设置
			}
		}
		if(pMapInfo->chapterInfo->chapterType == m_pPveModel->getActivityID())
		{
			m_pPveModel->setLastShowActivityChapterID(chapter_id);
		}
		else
		{
			m_pPveModel->setLastShowChapterID(chapter_id);
		}
	}
	
    return true;
}

Pve* Pve::create(pve::PveFightType type)
{
	Pve* pRet = new Pve();
	if(pRet && pRet->init(type))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}

	return pRet;
}

void Pve::onEnter()
{
    DLayer::onEnter();
    updateView();
}

void Pve::onExit()
{
    RemoveChild();
	DLayer::onExit();
}

void Pve::updateView()
{
	if(m_pPveModel->getFightType() ==pve::PveFightElite)
	{
		//精英赛
		showEliteView();
		return;
	}
	if(m_pPveModel->getFightType() ==pve::PveFightActivity)
	{
		//副本活动
		updateActivityView();
		return;
	}

	m_pPveModel->setFightType(pve::PveFightNormal);
	if (m_bReplayDialog || (m_bAfterBattle && m_pPveModel->getShowChapter()))
	{
		m_bFirstPlay = m_bAfterBattle && m_pPveModel->getShowChapter();
		currentDialogIndex = 0;
		showDialogs(m_nLastShowMapID);
	}
	else if (m_pPveModel->getShowChapter())
	{
        int cid = m_pPveModel->getLastShowChapterID();
		showChapterView(cid);
	}
	else if (m_pPveModel->getShowMap())
	{
        int cid = m_pPveModel->getLastShowChapterID();
		showMapView(cid);
	}
	else
	{
        pve::MapInfo *pMap = m_pPveModel->getMapInfo(m_pPveModel->getLastShowMapID());
        if (pMap)
        {
			if(pMap->todayTimes>=pMap->challenge_limit)
			{
				int cid = m_pPveModel->getLastShowChapterID();
				showMapView(cid);
				return;
			}

			pve::ChapterInfo *pChapter = pMap->chapterInfo;
			if(pChapter->lastMid == pMap->getMapId())
			{
				//某章节最后关卡，完成后跳回章节
				if((pMap->iTimes-1)%pMap->fight_open == pMap->fight_open-1
					&& pChapter->getCid() != m_pPveModel->getFirstChapterId())
				{
					int cid = m_pPveModel->getLastShowChapterID();
					showChapterView(cid);
				}
				else
				{
					showMapInfoView(pMap->getMapId());
				}
			}
			else
			{
				/*if((pMap->iTimes-1)%pMap->fight_open == pMap->fight_open-1)
				{
					int cid = m_pPveModel->getLastShowChapterID();
					showMapView(cid);
				}*/
				showMapInfoView(pMap->getMapId());
			}
			
        }
	}
}

void Pve::updateActivityView()
{
	if (m_pPveModel->getShowActivityChapter())
	{
        int cid = m_pPveModel->getLastShowActivityChapterID();
		showTianqiChapterView(cid);
	}
	else if (m_pPveModel->getShowActivityMap())
	{
        int cid = m_pPveModel->getLastShowActivityChapterID();
		showMapView(cid);
	}
	else
	{
        pve::MapInfo *pMap = m_pPveModel->getMapInfo(m_pPveModel->getLastShowActivityMapID());
        if (pMap)
        {
			if(pMap->todayTimes>=pMap->challenge_limit)
			{
				int cid = m_pPveModel->getLastShowActivityChapterID();
				showMapView(cid);
				return;
			}

			pve::ChapterInfo *pChapter = pMap->chapterInfo;
			if(pChapter->lastMid == pMap->getMapId())
			{
				//某章节最后关卡，完成后跳回章节
				if((pMap->iTimes-1)%pMap->fight_open == pMap->fight_open-1
					&& pChapter->getCid() != m_pPveModel->getFirstChapterId())
				{
					int cid = m_pPveModel->getLastShowActivityChapterID();
					showTianqiChapterView(cid);
				}
				else
				{
					showMapInfoView(pMap->getMapId());
				}
			}
			else
			{
				showMapInfoView(pMap->getMapId());
			}
			
        }
	}
}

void Pve::showDialogs(int mapID)
{
	m_pDialogs = m_pPveModel->getDialog(m_nLastShowChapterID);
	CC_SAFE_RETAIN(m_pDialogs);
	m_nLastShowChapterID = m_pPveModel->getLastShowChapterID();
	RemoveChild();
	/*m_pMapInfoWithDialog = MapInfoView::create();
	m_pMapInfoWithDialog->setisShowingDialog(true);
	m_pMapInfoWithDialog->setDelegate(this);

	m_pMapInfoWithDialog->setMapInfo(m_pPveModel->getMapInfo(mapID));
	m_pContainer->addChild(m_pMapInfoWithDialog);*/

	showChapterView(m_nLastShowChapterID);
	//showMapView(m_nLastShowChapterID);
	if (m_bAfterBattle)
	{
		m_bAfterBattle = false;
		m_pPveModel->setShowChapter(true);
	}
	if (m_bReplayDialog)
	{
		m_bReplayDialog = false;
		m_pPveModel->setShowMap(true);
	}
	
	m_pPveModel->setLastShowChapterID(m_nLastShowChapterID);
	playNextDialog();
}

void Pve::playNextDialog()
{
	if(!m_pDialogs || currentDialogIndex>=m_pDialogs->count())
	{
		RemoveChild();
		m_pMapInfoWithDialog = NULL;
		updateView();
		if (m_bFirstPlay)
		{
			int cid = m_pPveModel->getLastShowChapterID() - 1;
			CCNotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_AFTER_DIALOG, CCInteger::create(cid));
			CCLOG("%s:%d", D_EVENT_AFTER_DIALOG, cid);
		}
		
	}else
	{
		m_pMapInfoWithDialog->playDialog((pve::MapDialogInfo*)m_pDialogs->objectAtIndex(currentDialogIndex++));
	}
}

void Pve::endDialog()
{
	if (m_pDialogs)
	{
		currentDialogIndex = m_pDialogs->count();
	}
	
	playNextDialog();
}

bool Pve::isLastDialog()
{
	return !m_pDialogs || currentDialogIndex>=m_pDialogs->count();
}

void Pve::RemoveChild()
{
    m_pContainer->removeAllChildren();
	m_pMapInfo = NULL;
}


void Pve::getGradeAward(uint32_t cid)
{
    HTTP_CLIENT->getGradeAward(cid , this , callfuncJson_selector(Pve::onGradeAward));
}

void Pve::onGradeAward(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ITEM_MODEL->addPow(NULL , true);
}

void Pve::clearRushCD()
{
    user::UserInfo* pInfo = m_pUserModel->getUserInfo();
    DMessageBox::showMessageBox(D_LOCAL_STRING("ClearRushCDNeed%d",pInfo->getRushCDCost()).c_str(),this,messageBox_callfunc_selector(Pve::confirmDoRush));
}

void Pve::confirmDoRush(MessageType type,DMessageBox* target)
{
    if(type==kMessageTypeCancel) return;
    user::UserInfo* pInfo = m_pUserModel->getUserInfo();
    if(pInfo->getRushCD()>1)//如果cd 仍然大于1秒
    {
        HTTP_CLIENT->pve_rush_clearcd();
    }
}

void Pve::showPowerNotEnough()
{
	//直接现场买体力
	m_pPopup->removeAllChildren();
	if(m_pRecoverPower==NULL)
	{
		m_pRecoverPower = RecoverPower::create();
		CC_SAFE_RETAIN(m_pRecoverPower);
		m_pRecoverPower->setDelegate(this);
	}
	m_pPopup->addChild(m_pRecoverPower);
	m_pPopup->setVisible(true);
	addChild(m_pPopup);

	//item::ItemInfo* m_pPower = DM_GET_ITEM_MODEL->getItemInfoByID(D_CONFIG_UINT_FOR_KEY(ITEM_CHICKEN));
	//if(m_pPower){
	//	//有体力丹 直接吃
	//	//TODO
	//	m_pPopup->removeAllChildren();
	//	if(m_pRecoverPower==NULL)
	//	{
	//		m_pRecoverPower = RecoverPower::create();
	//		CC_SAFE_RETAIN(m_pRecoverPower);
	//		m_pRecoverPower->setDelegate(this);
	//	}
	//	m_pPopup->addChild(m_pRecoverPower);
	//	m_pPopup->setVisible(true);
	//	addChild(m_pPopup);
	//}
	//else{
	//	//没有体力丹，现场购买
	//	DM_GET_MALL_MODEL->setUnenoughItemID(D_CONFIG_UINT_FOR_KEY(ITEM_CHICKEN));
	//	BuyPopup* popup = BuyPopup::create();
	//	Game::sharedGame()->openPopup(popup);
	//}
    
}

void Pve::rushMap(uint32_t mid)
{
    if(!m_pUserModel->getUserInfo()->hasPower())
    {
        showPowerNotEnough();
        return;
    }
    pve::MapInfo* pMapInfo = m_pPveModel->getMapInfo(mid);
    if(pMapInfo->remainTimes()==0)
    {
        Tips_Warning(D_LOCAL_STRING("NotEnoughTimes").c_str());
        return;
    }
    HTTP_CLIENT->pve_rush(mid,this,callfuncJson_selector(Pve::rushMapEnd));
}

void Pve::startExploreRequest(uint32_t mid)
{
	DM_GET_SOUND_MODEL->playCommonEffectSound(SFX_FIGHT, 0);
	m_nLastShowMapID = m_pPveModel->getLastShowMapID();
	m_nLastShowChapterID = m_pPveModel->getLastShowChapterID();
	HTTP_CLIENT->enterMap(mid,this,callfuncJson_selector(Pve::startExploreHandler), false);
}

bool Pve::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	return false;
}

SEL_MenuHandler Pve::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	return NULL;
}

void Pve::showChapterView()
{
	showChapterView(m_pPveModel->getLastShowChapterID());
}

void Pve::showChapterView(uint32_t id, bool bFromMapList)
{
	m_pPveModel->setFightType(pve::PveFightNormal);
    RemoveChild();
    m_pPveModel->setShowChapter(true);
    ChapterView* pChapterView = ChapterView::create();
	pChapterView->setIsFromMapList(bFromMapList);
	pChapterView->setDelegate(this);
	pChapterView->rotateTo(id);
	m_pContainer->addChild(pChapterView);
}


void Pve::showMapView(uint32_t id, bool bFromChapter, bool bFromMapInfo)
{
    RemoveChild();
    m_pPveModel->setShowChapter(false);
    m_pPveModel->setShowMap(true);
    MapView* pMapView = MapView::create();
	pMapView->setIsFromChapter(bFromChapter);
	pMapView->setIsFromMapInfo(bFromMapInfo);
    pMapView->setDelegate(this);
	pve::ChapterInfo *pChapterInfo = m_pPveModel->getChapterInfo(id);
    pMapView->setChapterInfo(pChapterInfo);
    m_pContainer->addChild(pMapView);
    
}

void Pve::showTianqiChapterView()
{
	showTianqiChapterView(m_pPveModel->getLastShowActivityChapterID());
}

void Pve::showTianqiChapterView(uint32_t id, bool bFromMapList)
{
	m_pPveModel->setFightType(pve::PveFightActivity);
	RemoveChild();
	m_pPveModel->setShowActivityChapter(true);
	TianqiChapterView* pChapterView = TianqiChapterView::create();
	pChapterView->setIsFromActivityMapList(bFromMapList);
	pChapterView->setDelegate(this);
	pChapterView->rotateTo(id);
	m_pContainer->addChild(pChapterView);
}

void Pve::showMapInfoView(uint32_t id)
{
	RemoveChild();
    m_pMapInfo = MapInfoView::create();
    m_pMapInfo->setDelegate(this);
    
    m_pMapInfo->setMapInfo(m_pPveModel->getMapInfo(id));
    m_pContainer->addChild(m_pMapInfo);
}

void Pve::showEliteView()
{
	if(DM_GET_USER_MODEL->getUserInfo()->getUserLv()<7)
	{
		Tips_Alert(D_LOCAL_STRING("eliteInfoError"));
		return;
	}
	m_pPveModel->setFightType(pve::PveFightElite);
	RemoveChild();
	m_pPveModel->setShowChapter(false);
	m_pPveModel->setShowMap(false);
	PveElite* pEliteView = PveElite::create();
	pEliteView->setDelegate(this);
	m_pContainer->addChild(pEliteView);
}

void Pve::hideMapInfoView()
{
	RemoveChild();
	int cid = 0;
	if(m_pPveModel->getFightType() ==pve::PveFightActivity)
	{
		cid = m_pPveModel->getLastShowActivityChapterID();
		//showTianqiMapView(cid,false,true);
	}
	else
	{
		cid = m_pPveModel->getLastShowChapterID();
	}
	showMapView(cid,false,true);
}

void Pve::rushMapEnd(const Json::Value & requestData,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
	m_bRushResult = true;
    //displayRushResult();
	displayWonders();
//     if(m_pRushSettle==NULL)
//     {
//         m_pRushSettle = RushSettle::create();
//         m_pRushSettle->setPopupDelegate(this);
//         CC_SAFE_RETAIN(m_pRushSettle);
//     }
//     m_pRushSettle->setInfo(DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve());
//     Game::sharedGame()->openPopup(m_pRushSettle);
}

void Pve::startExploreHandler(const Json::Value & requestData,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
    
	settlement::SettlementInfoPve *pSettlementInfo = DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve();
	if (pSettlementInfo)
	{
		if (pSettlementInfo->win)
		{
			DM_GET_USER_MODEL->getUserInfo()->setMid(requestData["mid"].asUInt());
		}
		
		if (pSettlementInfo->inwar)
		{
			displayExploreBattle(m_pPveModel->getMapInfo(requestData["mid"].asUInt()));
			m_pPveModel->getElites();
		}
		else
		{
			if (m_pMapInfo)
			{
				m_pMapInfo->showResult();
			}
			else
			{
				CCLOG("map info is empty!");
			}
			
			displayExploreResult();
		}
	}
}

void Pve::playOnceMore(uint32_t mid)
{
	int nVipLev = m_pUserModel->getUserInfo()->getVipLevel();
	if (nVipLev < D_CONFIG_UINT_FOR_KEY(PVE_EXPLORE_ONCE_MORE_VIPLEV) && !DM_GET_USER_MODEL->checkPrivilegeType(user::PrivilegeTypeBuyExplore))
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV,CCString::create(D_LOCAL_STRING("NEED_VIP%d%d",m_pUserModel->getToVipLevelCost(D_CONFIG_UINT_FOR_KEY(PVE_EXPLORE_ONCE_MORE_VIPLEV)),D_CONFIG_UINT_FOR_KEY(PVE_EXPLORE_ONCE_MORE_VIPLEV))));
		return;
	}
    
	if (m_pUserModel->getUserInfo()->getPoints() < DM_GET_MALL_MODEL->getProductPriceById(D_CONFIG_UINT_FOR_KEY(PRODUCT_PVE_EXEC_TIMES)))
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
		return;
	}
    
	m_uBuyMid = mid;
	DMessageBox::showMessageBox(D_LOCAL_STRING("EnterToBuyPve").c_str(), this, messageBox_callfunc_selector(Pve::confirmDoBuy));
}

void Pve::confirmDoBuy(MessageType type,DMessageBox* targe)
{
	if(type == kMessageTypeCancel) return;

	HTTP_CLIENT->pve_onceMore(m_uBuyMid, this, callfuncJson_selector(Pve::onceMore));
}

void Pve::onceMore(const Json::Value &requestData, const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
	showMapInfoView(requestData["mid"].asUInt());
}


void Pve::showTouchItem(uint32_t chapterId)
{
	m_pPopup->removeAllChildren();
	pve::ChapterInfo* pChapterInfo = DM_GET_PVE_MODEL->getChapterInfo(chapterId);
	if (pChapterInfo)
	{
		PveReward *pReward = PveReward::create();
		pReward->UpdateInfo(pChapterInfo, this);
		m_pPopup->addChild(pReward);
		m_pPopup->setVisible(true);
		addChild(m_pPopup);
	}
}

void Pve::showDropItemInfo(cocos2d::CCNode* obj,bool setY)
{
	if(setY)
	{
		PL_MOVE_NODE_ABOVE_CENTER_Y(obj);
	}
	m_pPopup->removeAllChildren();
	m_pPopup->addChild(obj);
	m_pPopup->setVisible(true);
	addChild(m_pPopup);
}

void Pve::onTouchPveRewardOk(uint32_t chapterId,uint8_t vip)
{
	HTTP_CLIENT->collectChapterBox(chapterId,vip);
}

void Pve::onTouchPveRewardCancel(uint32_t chapterId)
{
    closePopup();
}

void Pve::onPopupOpen(DPopup* pPopup)
{
    if(pPopup == m_pRushSettle)
    {
        setVisible(false);
    }
}

void Pve::onPopupClose(DPopup* pPopup)
{
    if(pPopup == m_pRushSettle)
    {
        setVisible(true);
    }
}

void Pve::closePopup()
{
    m_pPopup->removeAllChildren();
	m_pPopup->removeFromParent();
    m_pPopup->setVisible(false);
}


void Pve::displayRushResult()
{
    //settlement::SettlementInfoPve *pSettlementInfo = DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve();
//     if (!pSettlementInfo->isHasWonder())
//     {
//         TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(Pve::showRushResultEnd),this, 0.f);
//     }
    displayWonders();
}

void Pve::displayExploreResult()
{
    settlement::SettlementInfoPve *pSettlementInfo = DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve();
    ExploreResultEffect* pEffect = ExploreResultEffect::create(pSettlementInfo);
//    if (!pSettlementInfo->isHasWonder()) {
        pEffect->setCallBack(this,callfuncO_selector(Pve::showResultEnd));
//    }
    Game::sharedGame()->playEffect(pEffect,true);
	CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, SFX_PVE_EXPLORE);
//    displayWonders();
}

void Pve::displayWonders(uint32_t idx)
{
// 	for (int i = 1; i <= 5; i++)
// 	{
// 		WonderDrop *pWonder = WonderDrop::create();
// 		settlement::WonderInfo *pData = settlement::WonderInfo::create();
// 		pData->wonderType = settlement::WonderType(i);
// 		if (i == 3)
// 		{
// 			pData->productId = 34;
// 		}
// 		else if (i == 4)
// 		{
// 			pData->productId = 65;
// 		}
// 		pData->itemID = 9001;
// 		
// 		pWonder->setData(pData);
// 		Game::sharedGame()->openPopup(pWonder);
// 	}
// 	

    settlement::SettlementInfoPve *pSettlementInfo = DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve();
	if(idx == pSettlementInfo->m_pWonders->count())
	{
		closePopup();
		if(m_pPopup->getOpacity()==0) m_pPopup->setOpacity(180);
	} else if(idx == 0)
	{
		m_pPopup->setVisible(true);
		m_pPopup->setOpacity(0);
		addChild(m_pPopup);
	}
    if(pSettlementInfo->isHasWonder() && idx < pSettlementInfo->m_pWonders->count())
    {
		//探索奇遇时播放音效
		DM_GET_SOUND_MODEL->playCommonEffectSound(SFX_PVE_EXPLORE_N, 3);
		WonderDrop *pWonder = WonderDrop::create();
		pWonder->setData((settlement::WonderInfo*)pSettlementInfo->m_pWonders->objectAtIndex(idx));
		pWonder->setIndex(idx);
		pWonder->setDelegate(this);
		m_pPopup->removeAllChildren();
		m_pPopup->addChild(pWonder);
// 		for (uint32_t i=0; i<pSettlementInfo->m_pWonders->count(); ++i) {
// 			WonderDrop *pWonder = WonderDrop::create();
// 			pWonder->setData((settlement::WonderInfo*)pSettlementInfo->m_pWonders->objectAtIndex(i));
// 			addChild(pWonder);
// 			//Game::sharedGame()->openPopup(pWonder);
// 		}
//         Game::sharedGame()->setPopupVisible(false);
//         for (uint32_t i=0; i<pSettlementInfo->m_pWonders->count(); ++i) {
//             DropWonderEffect* pEffect = DropWonderEffect::create((settlement::WonderInfo*)pSettlementInfo->m_pWonders->objectAtIndex(i));
//             if(i==pSettlementInfo->m_pWonders->count()-1)  
//             {
//                 pEffect->setCallBack(this,callfuncO_selector(Pve::showResultEnd));
//             }
//             Game::sharedGame()->playEffect(pEffect);
//         }
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_GET_WONDER);
    }
	else if (m_bRushResult)
	{
		if(m_pRushSettle==NULL)
		{
			m_pRushSettle = RushSettle::create();
			//popup自适应
			PL_MOVE_NODE_ABOVE_CENTER_Y(m_pRushSettle);
			m_pRushSettle->setPopupDelegate(this);
			CC_SAFE_RETAIN(m_pRushSettle);
		}
		m_pRushSettle->setInfo(DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve());
		Game::sharedGame()->openPopup(m_pRushSettle);
		m_bRushResult = false;
		closePopup();
	}
}

void Pve::displayExploreBattle(pve::MapInfo* info)
{
//     ExploreBattleEffect* pEffect = ExploreBattleEffect::create(info);
//     pEffect->setCallBack(this,callfuncO_selector(Pve::exploreBattleEnd));
// 	Game::sharedGame()->playEffect(pEffect);
// 	CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, SFX_PVE_BATTLE);

	uint32_t enemyID = info->enemy_id;
	settlement::SettlementInfoPve *pSettlementInfo = DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve();
	bool win = pSettlementInfo->win;

	if((info->iTimes-1)%info->fight_open == info->fight_open-1 && win)
	{
		//胜利
		enemyID = info->enemy_id;
	}
	else if(info->iTimes%info->fight_open == info->fight_open-1 && !win)
	{
		//失败
		enemyID = info->enemy_id;
	}
	else
	{
		enemyID = info->enemyLittle_id;
	}

	LoadingLayer *loading = LoadingLayer::sharedLoadingLayer();
	loading->setWarriorIds(DM_GET_FORMATION_MODEL->getFormationLeader(), enemyID);
	exploreBattleEnd(this);
}

void Pve::showRushResultEnd(float dt)
{
    showResultEnd(NULL);
}

void Pve::showResultEnd(cocos2d::CCObject *exploreNode)
{
    settlement::SettlementInfoPve *pSettlementInfo = DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve();
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(pSettlementInfo->dropId));
    Game::sharedGame()->setPopupVisible(true);
	if (m_pMapInfo)
	{
		m_pMapInfo->hideResult();
	}
	displayWonders();
}

void Pve::exploreBattleEnd(cocos2d::CCObject *exploreNode)
{
    settlement::SettlementInfoPve *pSettlementInfo = DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve();
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE,
                                                                     CCInteger::create(pSettlementInfo->getCanSkipBattle()?battle::kSkipBattleTypeSkip:battle::kSkipBattleTypePVE));
	m_bAfterBattle = true;
}

void Pve::replayDialog(pve::ChapterInfo *pInfo)
{
	if (!DM_GET_PVE_MODEL->getDialog(pInfo->getCid()))
	{
		Tips_Alert(D_LOCAL_STRING("To_Be_Continued"));
		return;
	}
	
	m_nLastShowChapterID = pInfo->getCid();
	m_nLastShowMapID = pInfo->lastMid;
	m_bReplayDialog = true;
	updateView();
}

void Pve::onBackKey()
{
	if (m_pPopup && m_pPopup->isVisible() && m_pPopup->getChildrenCount())
	{
		closePopup();
		return;
	}
	if (!m_pPveModel->getShowMap() && !m_pPveModel->getShowChapter())
	{
		int cid = m_pPveModel->getLastShowChapterID();
		showMapView(cid);
		return;
	}
	else if (!m_pPveModel->getShowChapter())
	{
		int cid = m_pPveModel->getLastShowChapterID();
		showChapterView(cid, true);
		return;
	}
	
	
	BackKeyInterface::onBackKey();
}