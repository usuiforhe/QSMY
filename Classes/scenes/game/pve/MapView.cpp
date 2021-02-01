//
//  MapView.cpp
//  QSMY
//
//  Created by wanghejun on 13-4-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MapView.h"
#include "Pve.h"
#include "managers/ResourceManager.h"
#include "utils/TimerUtil.h"
#include "managers/TipsManager.h"
#include "../inventory/ItemInfoView.h"
#include "../equip/EquipInfoView.h"
#include "../warrior/WarriorInfo.h"

#include "RewardItemBox.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PVE_MAP_SCROLL_SIZE CCSizeMake(640, 670)
#define PVE_MAP_CELL_MIN_SIZE	CCSizeMake(574 ,64)
#define PVE_MAP_CELL_MAX_SIZE	CCSizeMake(600 ,255)
#define PVE_MAP_CELL_REW_SIZE   CCSizeMake(600, 100)

#define EXT_CELL_COUNT 1

MapView::MapView()
:m_pDelegate(NULL)
,m_pView(NULL)
,m_pPveModel(NULL)
,m_pUserModel(NULL)
,m_pNodeChild(NULL)
,m_pChapterInfo(NULL)
// ,m_pNormalStarSprite(NULL)
// ,m_pLightStarSprite(NULL)
// ,m_ChapterName(NULL)
,m_pLblPlan(NULL)
,m_pCurrentProgress(NULL)
,m_pButtonImageNode(NULL)
,m_pButtonAnimationNode(NULL)
,m_pButtonAnimationSW(NULL)
// ,m_pArrow(NULL)
,m_pRewardItemBox(NULL)
,m_pNodeAllStar(NULL)
,m_pBtnAllStar(NULL)
,m_pNodeBg(NULL)
,m_pSprBg(NULL)
,m_pNodeRotation(NULL)
, m_pNodeDraggingDown(NULL)
, m_pNodeDraggingUp(NULL)
, m_pBtnPve(NULL)
, m_pBtnWonder(NULL)
, m_pBoxDrop0(NULL)
, m_pBoxDrop1(NULL)
,m_pMapCellData(NULL)
//,m_pAwardCellData(NULL)
,m_nSelectedIndex(EXT_CELL_COUNT)
,m_uCenterIdx(0)
,m_ptOrigin(0, 0)
,m_bFromChapter(false)
,m_bTouching(false)
,m_fMaxPct(0.f)
,m_fMinPct(0.f)
,m_bWheeling(false)
,m_pArmatureCell(NULL)
,m_pNodeFame(NULL)
,m_pNodeBack(NULL)
,m_pNodeDrop(NULL)
,m_pNodeJindu(NULL)
,m_pNodeWheel(NULL)
,m_pQiyuNode(NULL)
,m_pJuqingNode(NULL)
,m_RushTime(NULL)
,m_dropItems(NULL)
,m_pDropBg(NULL)
,m_pDropBgBoss(NULL)
,m_pDropTitel(NULL)
,m_nRushTimer(0.f)
,m_pCoinTip(NULL)
,m_pBtnElite(NULL)
,m_pNodeQiyu(NULL)
,m_bFromMapInfo(false)
{

}

MapView::~MapView()
{
    setChapterInfo(NULL);
    
	CC_SAFE_RELEASE(m_pNodeChild);
// 	CC_SAFE_RELEASE(m_ChapterName);
 	CC_SAFE_RELEASE(m_pLblPlan);
	CC_SAFE_RELEASE(m_pCurrentProgress);
	CC_SAFE_RELEASE(m_pButtonImageNode);
	CC_SAFE_RELEASE(m_pButtonAnimationNode);
// 	CC_SAFE_RELEASE(m_pArrow);
 	CC_SAFE_RELEASE(m_pRewardItemBox);
	CC_SAFE_RELEASE(m_pMapCellData);
//    CC_SAFE_RELEASE(m_pAwardCellData);
	CC_SAFE_RELEASE(m_pChapterInfo);
// 	CC_SAFE_RELEASE(m_pNormalStarSprite);
// 	CC_SAFE_RELEASE(m_pLightStarSprite);
	CC_SAFE_RELEASE(m_pNodeAllStar);
	CC_SAFE_RELEASE(m_pBtnAllStar);
	CC_SAFE_RELEASE(m_pNodeBg);
	CC_SAFE_RELEASE(m_pNodeRotation);
	CC_SAFE_RELEASE(m_pNodeDraggingDown);
	CC_SAFE_RELEASE(m_pNodeDraggingUp);
	CC_SAFE_RELEASE(m_pBtnPve);
	CC_SAFE_RELEASE(m_pBtnWonder);
	CC_SAFE_RELEASE(m_pBoxDrop0);
	CC_SAFE_RELEASE(m_pBoxDrop1);
	CC_SAFE_RELEASE(m_pArmatureCell);
	CC_SAFE_RELEASE(m_pNodeFame);
	CC_SAFE_RELEASE(m_pNodeBack);
	CC_SAFE_RELEASE(m_pNodeDrop);
	CC_SAFE_RELEASE(m_pNodeJindu);
	CC_SAFE_RELEASE(m_pNodeWheel);
	CC_SAFE_RELEASE(m_pQiyuNode);
	CC_SAFE_RELEASE(m_pJuqingNode);
	CC_SAFE_RELEASE(m_RushTime);
	CC_SAFE_RELEASE(m_dropItems);
	CC_SAFE_RELEASE(m_pDropBg);
	CC_SAFE_RELEASE(m_pDropBgBoss);
	CC_SAFE_RELEASE(m_pDropTitel);
	CC_SAFE_RELEASE(m_pButtonAnimationSW);
	CC_SAFE_RELEASE(m_pBtnElite);
	CC_SAFE_RELEASE(m_pNodeQiyu);
	UNBINDING_ALL(m_pUserModel,this);
	CC_SAFE_RELEASE(m_pCoinTip);
}

bool MapView::init()
{
    if(!DLayer::init())
    {
        return false;
    }
    
    m_pPveModel = DM_GET_PVE_MODEL;
    m_pUserModel = DM_GET_USER_MODEL;
    
	m_dropItems = CCArray::create();
	CC_SAFE_RETAIN(m_dropItems);
	CC_SAFE_RETAIN(m_pUserModel);

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("RewardItemBox", RewardItemBoxLoader::loader());
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_map/PL_map", pNodeLoaderLibrary);
    
	m_pMapCellData = readCCData("PL_ui/ccb/pl_map/PL_map_wheel_cell");
	CC_SAFE_RETAIN(m_pMapCellData);
    
//     m_pAwardCellData = readCCData("ui/ccb/pve/pvePowerRewardNode");
//     CC_SAFE_RETAIN(m_pAwardCellData);
    
    m_pView = PLWheelView::create(this, PVE_MAP_SCROLL_SIZE);
    m_pView->setDelegate(this);
	m_pView->setWheelDelegate(this);
	m_pNodeChild->addChild(m_pView);

//	m_pNodeAllStar->setVisible(false);
	m_pLblPlan->setString("0");
    
 	m_pRewardItemBox->getTouchEvent()->setTarget(this, menu_selector(MapView::onTouchEquip));

	m_pBtnPve->setEnabled(false);
	m_pBtnPve->selected();
	m_pBtnWonder->setTarget(this, menu_selector(MapView::callBackWonder));
	m_pBtnElite->setTarget(this, menu_selector(MapView::touchElite));
//     
//     m_pNormalStarSprite = CCSprite::create("ui/pve/img_star_n.png");
//     m_pLightStarSprite = CCSprite::create("ui/pve/img_star_h.png");
//     
//     CC_SAFE_RETAIN(m_pNormalStarSprite);
//     CC_SAFE_RETAIN(m_pLightStarSprite);

	m_ptOrigin = m_pNodeBg->getPosition();
	m_pArmatureCell = ResourceManager::sharedResourceManager()->getUIAnimate("pve_cell_ani");
	CC_SAFE_RETAIN(m_pArmatureCell);
	m_RushTime->setVisible(false);
	m_pCoinTip->setVisible(false);
    return true;
}

// cocos2d::CCSpriteFrame* MapView::getNormalStarDisplayFrame()
// {
//     if (m_pNormalStarSprite)
//     {
//         return m_pNormalStarSprite->displayFrame();
//     }
//     
//     return NULL;
// }
// 
// 
// cocos2d::CCSpriteFrame* MapView::getLightStarDisplayFrame()
// {
//     if (m_pLightStarSprite)
//     {
//         return m_pLightStarSprite->displayFrame();
//     }
//     
//     return NULL;
// }


bool MapView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeContainer",	CCNode *,			m_pNodeChild);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ChapterName",	CCLabelBMFont *,	m_ChapterName);
 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLblPlan",		CCLabelBMFont *,	m_pLblPlan);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurrentProgress",		CCLabelBMFont *,	m_pCurrentProgress);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonImageNode",	CCNode *,			m_pButtonImageNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonAnimationNode",	CCNode *,			m_pButtonAnimationNode);
	
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pArrow",		CCMenuItemImage *,	m_pArrow);
 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxPlan",		RewardItemBox *,         m_pRewardItemBox);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAllStar",	CCNode *,			m_pNodeAllStar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAllStar",	CCMenuItem *,			m_pBtnAllStar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeBg",	CCNode *,			m_pNodeBg);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRotation", CCNode *, m_pNodeRotation);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDraggingDown", CCNode *, m_pNodeDraggingDown);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDraggingUp", CCNode *, m_pNodeDraggingUp);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnPve", CCMenuItem *, m_pBtnPve);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnWonder", CCMenuItem *, m_pBtnWonder);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxDrop0", ItemBox *, m_pBoxDrop0);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxDrop1", ItemBox *, m_pBoxDrop1);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFame", CCNode * , m_pNodeFame);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeBack", CCNode * , m_pNodeBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDrop", CCNode * , m_pNodeDrop);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeJindu", CCNode* , m_pNodeJindu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeWheel", CCNode *, m_pNodeWheel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pQiyuNode",  CCNode* , m_pQiyuNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pJuqingNode",  CCNode* , m_pJuqingNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_RushTimeT",  CCLabelBMFont* , m_RushTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDropBg" , CCNode* , m_pDropBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDropBgBoss" , CCNode* , m_pDropBgBoss);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDropTitel" , CCNode* , m_pDropTitel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonAnimationSW", CCNode*, m_pButtonAnimationSW);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCoinTip" ,CCNode*, m_pCoinTip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnElite" , CCMenuItem*, m_pBtnElite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pNodeQiyu", CCNode*,m_pNodeQiyu);
	return false;
}

SEL_MenuHandler MapView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchArrow", MapView::onTouchArrow);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackRush", MapView::callBackRush);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackAllStar", MapView::callBackAllStar);
    
	return NULL;
}

void MapView::onTouchCell(cocos2d::extension::CCTableViewCell* cell)
{
//     if(m_nSelectedIndex == (int32_t)cell->getIdx())
//     {
//         m_nSelectedIndex = - 1;
//         m_pView->updateCellPositions(cell);
//         m_pView->reloadData();
//     }
//     else
//     {
//         m_nSelectedIndex = cell->getIdx();
//         m_pView->updateCellPositions(cell);
//         m_pView->reloadData();
//     }
}

void MapView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	
    CCLOG("map cell touched at index: %i", cell->getIdx());
}

CCSize MapView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
//     if (idx == 0) {
//         return PVE_MAP_CELL_REW_SIZE;
//     }
//     else if (idx == m_nSelectedIndex && m_nSelectedIndex!=0)
//     {
//         return PVE_MAP_CELL_MAX_SIZE;
//     }
    return PVE_MAP_CELL_MIN_SIZE;
}

CCTableViewCell* MapView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
//     if(idx==0)
//     {
//         AllStarAwardCell *cell = dynamic_cast<AllStarAwardCell*>(table->dequeueCell());
//         if (cell==NULL)
//         {
//             cell = AllStarAwardCell::create(m_pAwardCellData, m_pDelegate);
//         }
//         cell->setChapterInfo(m_pChapterInfo);
//         return cell;
//     }
    
    MapViewCell * cell = dynamic_cast<MapViewCell*>(table->dequeueCell());
    if (cell==NULL)
	{
        cell = MapViewCell::create(m_pMapCellData, m_pDelegate, this);
	}
    cell->updateCell((pve::MapInfo *)maps.objectAtIndex(idx), idx==m_nSelectedIndex);
	cell->setIsCenter(m_uCenterIdx == idx);
	cell->showTouch(m_uCenterIdx == idx && !m_bTouching);
	if (m_uCenterIdx == idx)
	{
		addArmatureToCell(m_pArmatureCell, cell);
	}
    return cell;
}

uint32_t MapView::numberOfCellsInTableView(CCTableView *table)
{
    return maps.count();
}

void MapView::onTouchArrow(CCObject *obj)
{
	prepareToExit(m_pChapterInfo->getCid());
}

void MapView::onTouchEquip(cocos2d::CCObject *obj)
{
	if(m_pDelegate)
	{
		m_pDelegate->showTouchItem(m_pChapterInfo->getCid());
	}
}

void MapView::callBackRush(CCObject *obj)
{

	pve::MapInfo *pMapInfo = (pve::MapInfo *)maps.objectAtIndex(m_pView->getCenterIdx());
	if (pMapInfo->iScores <= 0)
	{
		Tips_Alert(D_LOCAL_STRING("MapScoreZero").c_str());
		return;
	}
	

 	user::UserInfo* pInfo = m_pUserModel->getUserInfo();

	if (pInfo->getVipLevel() < D_CONFIG_UINT_FOR_KEY(PVE_RUSH_MIN_VIPLEV) && !m_pUserModel->checkPrivilegeType(user::PrivilegeTypeRush))
	{
    //NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHOW_RECHARGE);
		//这里修改为VIP不足提示
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV);
	
		return;
	}
// 
// 
// 	if (m_nRushTimer < 0.f)
// 	{
// 		m_nRushTimer = 0.f;
// 	}
// 
	float m_nRushTimer = pInfo->getRushCD();
	if (m_nRushTimer > 0.f)
	{
		if (pInfo->getVipLevel() < D_CONFIG_UINT_FOR_KEY(PVE_RUSH_MIN_USEPOINT_VIPLEV))
		{
			std::string strTips = D_LOCAL_STRING("VIP%dUserCanUse%dMoneyClearRushCD", D_CONFIG_UINT_FOR_KEY(PVE_RUSH_MIN_USEPOINT_VIPLEV), pInfo->getRushCDCost()).c_str();
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV,ccs(strTips));
			return;
		}
		else
		{
			if (pInfo->getPoints() < pInfo->getRushCDCost())
			{
				NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
				return;
			}
			else
			{
				if(m_pDelegate) m_pDelegate->clearRushCD();
				return;
			}
		}
	}

	if (pMapInfo)
	{
		if(m_pDelegate) m_pDelegate->rushMap(pMapInfo->getMapId());
	}
}


void MapView::callBackAllStar(cocos2d::CCObject *obj)
{
	if (m_pDelegate)
	{
		m_pDelegate->getGradeAward(m_pChapterInfo->getCid());
	}
	
}

void MapView::setChapterInfo(pve::ChapterInfo* pChapterInfo)
{
    
    CC_SAFE_RETAIN(pChapterInfo);
    SAFE_RELEASE_UNBINDING_ALL(m_pChapterInfo, this);
    
    m_pChapterInfo = pChapterInfo;
    
    if(m_pChapterInfo)
    {
		if(m_pChapterInfo->chapterType == m_pPveModel->getActivityID())
		{
			 m_pPveModel->setLastShowActivityChapterID(m_pChapterInfo->getCid());
		}
		else
		{
			 m_pPveModel->setLastShowChapterID(m_pChapterInfo->getCid());
		}
       
        BINDING_EXEC(m_pChapterInfo, this, MapView::onMapsUpdated, D_EVENT_PVE_CHAPTER_UPDATED);
    }
}

void MapView::onEnter()
{
    DLayer::onEnter();

	BINDING_ONLY(m_pPveModel, this, MapView::onMapsUpdated, D_EVENT_PVE_CHAPTERS_UPDATED);
    BINDING_EXEC(m_pUserModel, this, MapView::onRushUpdated, D_EVENT_PVE_RUSH_TIME);
    
    if(m_pChapterInfo)
    {
        m_pPveModel->getMaps(m_pChapterInfo->getCid());
        BINDING_ONLY(m_pChapterInfo, this, MapView::onMapsUpdated, D_EVENT_PVE_CHAPTER_UPDATED);
		if(m_pChapterInfo->chapterType == m_pPveModel->getActivityID())
		{
			m_pBtnElite->setEnabled(false);
			//m_pBtnElite->selected();
		}
		else
		{
			m_pBtnElite->setEnabled(true);
			//m_pBtnElite->unselected();
		}
    }
	updateView();
	
	this->prepareToEnter();
}

void MapView::onExit()
{
	UNBINDING_ALL(m_pPveModel, this);
    if(m_pChapterInfo)
	{
		UNBINDING_ALL(m_pChapterInfo, this);
	}
	UNBINDING_ALL(m_pUserModel,this);
    m_nSelectedIndex = 0;
    maps.removeAllObjects();
    DLayer::onExit();
}

void MapView::onRushUpdated()
{
	user::UserInfo* pInfo = m_pUserModel->getUserInfo();
	m_nRushTimer = pInfo->getRushCD();
	if(m_nRushTimer>0.f) TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
	update(0.f);
}

void MapView::update(float dt)
{
	m_nRushTimer-=dt;
	if (m_nRushTimer < 0.f)
	{
		m_nRushTimer = 0.f;
	}

	if (D_FLOAT_EQUALS(m_nRushTimer, 0.f))
	{
		m_RushTime->setVisible(false);
		// m_pRushTimeBack->setVisible(false);
		TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
	}
	else
	{
		m_RushTime->setVisible(true);
		//m_pRushTimeBack->setVisible(true);
		m_RushTime->setString(TimeUtil::getSeconds2String(m_nRushTimer, "%M:%S"));
	}
}

void MapView::rotateTo(const int &mid)
{
	CCPoint offset = ccp(0,-(mid-1)*PVE_MAP_CELL_MIN_SIZE.height);
	offset.y = MIN(m_pView->maxContainerOffset().y, offset.y);
	offset.y = MAX(m_pView->minContainerOffset().y, offset.y);

	m_pView->setContentOffset(offset);
}

void MapView::onMapsUpdated()
{
    updateView();
	if(m_bFromMapInfo)
	{
		//CCLog(CCString::createWithFormat("%d",m_pPveModel->getLastShowMapID())->getCString());
		int id = 0;
		int lastMapId = 0;
		if(m_pChapterInfo->chapterType == m_pPveModel->getActivityID())
		{
			lastMapId = m_pPveModel->getLastShowActivityMapID();
		}
		else
		{
			lastMapId = m_pPveModel->getLastShowMapID();
		}
		for(uint32_t i=0;i<maps.count();i++)
		{
			pve::MapInfo* info = static_cast<pve::MapInfo*>(maps.objectAtIndex(i));

			if(info->getMapId() == lastMapId)
			{
				id = i;
				break;
			}
		}
		id = maps.count()-id;
		rotateTo(id);
	}
	else
	{
		m_pView->setContentOffset(m_pView->minContainerOffset(), false);
	}
}

void MapView::updateView()
{
	if (!m_pChapterInfo)
	{
		return;
	}

	m_pNodeBg->removeAllChildrenWithCleanup(true);
	unsigned int res = 0;
	if(m_pChapterInfo->chapterType == m_pPveModel->getActivityID())
	{
		res = m_pChapterInfo->getCid();
	}
	else
	{
		res = m_pPveModel->getChapterExtraInfo(m_pChapterInfo->getCid())->res;
	}
	m_pSprBg = ResourceManager::sharedResourceManager()->getChapterIcon(res);
	m_pNodeBg->addChild(m_pSprBg, 1, 1);

	if (!m_pChapterInfo->isSynced)
	{
		return;
	}
	

    m_pPveModel->getUnlockedMapsByChapterId(m_pChapterInfo->getCid(),maps);
    maps.reverseObjects();
    
    
    pve::MapInfo * pMapInfo = NULL;
    if(maps.count()>0) pMapInfo = static_cast<pve::MapInfo *>(maps.objectAtIndex(0));
    if(!pMapInfo || pMapInfo->iTimes==0)
    {
        m_nSelectedIndex = 0;
    }
    
    m_pView->reloadData();
    
	int nCid = m_pChapterInfo->getCid();
	pve::ChapterInfo* pChapterInfo = m_pPveModel->getChapterInfo(nCid);
	if (pChapterInfo)
	{
		m_pLblPlan->setString(CCString::createWithFormat("%d/%d",pChapterInfo->pres,pChapterInfo->getMaxPrestige())->getCString());
		m_pCurrentProgress->setString(CCString::createWithFormat("%d/%d",pChapterInfo->great_num , pChapterInfo->lastMid - pChapterInfo->firstMid + 1)->getCString());
		m_pBtnAllStar->setEnabled(m_pChapterInfo->canGetAllStarReward());

		if(m_pChapterInfo->grade_award)
		{
			//領過了
			m_pNodeAllStar->setVisible(false);
		}
		//是否可以领取声望奖励
		m_pButtonAnimationSW->setVisible(m_pChapterInfo->canGetPrestigeReward());

		m_pButtonImageNode->setVisible(true);
		m_pButtonAnimationNode->setVisible(m_pChapterInfo->canGetAllStarReward());
		if (m_pChapterInfo->canGetAllStarReward())
		{
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_PVE_GET_3_STAR);
		}
		

//		m_pLblPlan->setString(CCString::createWithFormat("%d/%d",pChapterInfo->pres, pChapterInfo->getMaxPrestige())->getCString());

        pve::PrestigeReward* pPrestigeReward = pChapterInfo->getAwardToShow();
        if(pPrestigeReward)
        {
            m_pRewardItemBox->setInfo(pChapterInfo, pPrestigeReward->prestige_reward_need_viplv);
        }
//         m_ChapterName->setString(pChapterInfo->chapter_name.c_str());
	}

	pMapInfo = (pve::MapInfo *)maps.objectAtIndex(m_uCenterIdx);
	if (pMapInfo)
	{
		updateDropInfo(pMapInfo->getMapId());
	}

	m_pNodeBg->setPosition(ccpSub(m_ptOrigin, ccp(pMapInfo->posX, pMapInfo->posY)));
	m_pNodeBg->setScale(pMapInfo->scale);
}

void MapView::prepareToEnter()
{
	m_pNodeChild->setPositionX(m_pNodeChild->getPositionX() + PVE_ACTION_MOVE_DIS);
	m_pNodeChild->runAction(CCMoveBy::create(PVE_ACTION_MOVE_DUR_ENTER, ccp(-PVE_ACTION_MOVE_DIS, 0)));
	if (!m_bFromChapter)
	{
		int lastMapId = 0;
		if(m_pChapterInfo->chapterType == m_pPveModel->getActivityID())
		{
			lastMapId = m_pPveModel->getLastShowActivityMapID();
		}
		else
		{
			lastMapId = m_pPveModel->getLastShowMapID();
		}
		pve::MapInfo *pMapInfo = DM_GET_PVE_MODEL->getMapInfo(lastMapId);
		CCNode *pNode = ResourceManager::sharedResourceManager()->getSceneIcon(pMapInfo->chapter_id);
		pNode->setPosition(ccpAdd(ccp(0, PVE_BATTLE_BACK_Y_POS), ccp(pMapInfo->posX, pMapInfo->posY)));
		m_pNodeBg->addChild(pNode);
		m_pSprBg->stopAllActions();
		m_pSprBg->setScale(1.5);
		m_pSprBg->setOpacity(0.f);
		m_pSprBg->runAction(CCScaleTo::create(PVE_ACTION_MOVE_DUR_ENTER, 1.0));
		m_pSprBg->runAction(CCFadeIn::create(PVE_ACTION_MOVE_DUR_ENTER));

		m_pNodeWheel->setPositionX(m_pNodeWheel->getPositionX() + PVE_ACTION_MOVE_DIS);
		m_pNodeWheel->runAction(CCMoveBy::create(PVE_ACTION_MOVE_DUR_ENTER, ccp(-PVE_ACTION_MOVE_DIS, 0)));
	}
	
//	m_pNodeChild->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(0.0, ccp(640, 0)), 
//		CCMoveBy::create(0.5, ccp(-640, 0))));
		//CCCallFuncO::create(this, callfuncO_selector(ChapterView::actionFinished), CCInteger::create(cid))));
}

void MapView::updateCells(float dt)
{
	CCPoint pt = ccpAdd(m_pView->getContentOffset(), ccp(0, 5));
	if (pt.y > m_pView->minContainerOffset().y)
	{
		unschedule(schedule_selector(MapView::updateCells));
	}
	
	m_pView->setContentOffset(pt);
}

void MapView::prepareToExit(unsigned int cid)
{
	m_pNodeChild->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(PVE_ACTION_MOVE_DUR_EXIT, ccp(PVE_ACTION_MOVE_DIS, 0)), 
		CCCallFuncO::create(this, callfuncO_selector(MapView::actionFinished), CCInteger::create(cid))));

	//进度       
	m_pNodeJindu->stopAllActions();
	m_pNodeJindu->       runAction(CCMoveTo::create(0.2f,ccp(-300,0)));
	m_pButtonImageNode->stopAllActions();
	m_pButtonImageNode-> runAction(CCMoveTo::create(0.2f,ccp(-300,0)));
	//声望
	m_pNodeFame->stopAllActions();
	m_pNodeFame->        runAction(CCMoveTo::create(0.2f,ccp(0,300)));
	//返回
	m_pNodeBack->stopAllActions();
	m_pNodeBack->        runAction(CCMoveTo::create(0.2f,ccp(300,0)));
	//掉落
	m_pNodeDrop->stopAllActions();
	m_pNodeDrop->        runAction(CCMoveTo::create(0.2f,ccp(0,-1500)));
	//剧情按钮
	m_pJuqingNode->stopAllActions();
	m_pJuqingNode->      runAction(CCMoveTo::create(0.2F,ccp(0,200)));
	//奇遇按钮
	m_pQiyuNode->stopAllActions();
	m_pQiyuNode->        runAction(CCMoveTo::create(0.2F,ccp(0,200)));
	m_pNodeQiyu->stopAllActions();
	m_pNodeQiyu->        runAction(CCMoveTo::create(0.2F,ccp(300,0)));

}

void MapView::prepareToExitForMap(unsigned int mid)
{
	m_pNodeChild->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(PVE_ACTION_MOVE_DUR_EXIT, ccp(PVE_ACTION_MOVE_DIS, 0)), 
		CCCallFuncO::create(this, callfuncO_selector(MapView::actionFinishedForMap), CCInteger::create(mid))));
	m_pNodeWheel->runAction(CCMoveBy::create(PVE_ACTION_MOVE_DUR_EXIT, ccp(PVE_ACTION_MOVE_DIS, 0)));
	pve::MapInfo *pMapInfo = DM_GET_PVE_MODEL->getMapInfo(mid);
	CCNode *pNode = ResourceManager::sharedResourceManager()->getSceneIcon(pMapInfo->chapter_id);
 	pNode->setPosition(ccpMult(ccpAdd(ccp(0, PVE_BATTLE_BACK_Y_POS), ccp(pMapInfo->posX, pMapInfo->posY)), 1.f/m_pNodeBg->getScale()));
 	pNode->setScale(1.f / m_pNodeBg->getScale());
	m_pNodeBg->addChild(pNode);
	m_pSprBg->stopAllActions();
	m_pSprBg->runAction(CCScaleTo::create(PVE_ACTION_MOVE_DUR_EXIT, 1.5));
	m_pSprBg->runAction(CCFadeOut::create(PVE_ACTION_MOVE_DUR_EXIT));
}

void MapView::actionFinished(CCObject *pObj)
{
	CCInteger *pCid = static_cast<CCInteger *>(pObj);
	if(m_pDelegate)
	{
		if(m_pChapterInfo->chapterType == m_pPveModel->getActivityID())
		{
			m_pDelegate->showTianqiChapterView(pCid->getValue(), true);
		}
		else
		{
			m_pDelegate->showChapterView(pCid->getValue(), true);
		}
	}
}

void MapView::actionFinishedForMap(CCObject *pObj)
{
	CCInteger *pMid = static_cast<CCInteger *>(pObj);
	if(m_pDelegate)
	{
		m_pDelegate->showMapInfoView(pMid->getValue());
	}
	m_pNodeBg->removeAllChildren();
}

void MapView::PLWheelViewBouncePercent(PLWheelView *pView, float pct)
{
	if (!m_bWheeling)
	{
		m_pNodeRotation->setRotation(pct * MAX_ROTATION);
	}
	static bool showingAction = false;
	if (pct <= 0.1 && pct >= -0.1)
	{
// 		m_pNodeDraggingDown->stopAllActions();
// 		m_pNodeDraggingUp->stopAllActions();
		m_pNodeDraggingDown->setVisible(true);
		m_pNodeDraggingUp->setVisible(true);
		showingAction = false;
	}
	else if (!showingAction)
	{
		showingAction = true;
		if (pct > 0.1)
		{
//			m_pNodeDraggingUp->stopAllActions();
			m_pNodeDraggingUp->setVisible(false);
			m_pNodeDraggingDown->setVisible(true);
//			m_pNodeDraggingDown->runAction(CCRepeatForever::create(CCBlink::create(0.5, 1)));
		}
		else
		{
//			m_pNodeDraggingDown->stopAllActions();
			m_pNodeDraggingDown->setVisible(false);
			m_pNodeDraggingUp->setVisible(true);
//			m_pNodeDraggingUp->runAction(CCRepeatForever::create(CCBlink::create(0.5, 1)));
		}
	}
	if (pct > m_fMaxPct)
	{
		m_fMaxPct = pct;
		m_fMinPct = 0.f;
	}
	if (pct < m_fMinPct)
	{
		m_fMinPct = pct;
		m_fMaxPct = 0.f;
	}
}

void MapView::PLWheelViewTouched(PLWheelView *pView)
{
	m_bTouching = true;
	m_fMaxPct = 0.f;
	m_fMinPct = 0.f;
	m_pNodeRotation->stopAllActions();
	m_bWheeling = false;
	MapViewCell *pCell = (MapViewCell *)pView->cellAtIndex(m_uCenterIdx);
	pCell->showTouch(false);
}

void MapView::PLWheelViewStartWheeling(PLWheelView *pView, unsigned int centerIdx)
{
	m_bWheeling = true;
	if (!D_FLOAT_EQUALS(m_fMaxPct, 0.f))
	{
		m_pNodeRotation->runAction(CCSequence::createWithTwoActions(CCRotateTo::create(0.3f, -m_fMaxPct * MAX_ROTATION / 4), 
			CCSequence::createWithTwoActions(CCRotateTo::create(0.2f, m_fMaxPct * MAX_ROTATION / 4), CCRotateTo::create(0.1f, 0))));
		m_fMaxPct = 0.f;
	}
	if (!D_FLOAT_EQUALS(m_fMinPct, 0.f))
	{
		m_pNodeRotation->runAction(CCSequence::createWithTwoActions(CCRotateTo::create(0.3f, -m_fMinPct * MAX_ROTATION / 4), 
			CCSequence::createWithTwoActions(CCRotateTo::create(0.2f, m_fMinPct * MAX_ROTATION / 4), CCRotateTo::create(0.1f, 0))));
		m_fMinPct = 0.f;
	}
}

void MapView::PLWheelViewStopWheeling(PLWheelView *pView, unsigned int centerIdx)
{
	m_bWheeling = false;
	m_bTouching = false;
	MapViewCell *pCell = (MapViewCell *)pView->cellAtIndex(m_uCenterIdx);
	pCell->showTouch(true);
}

void MapView::PLWheelViewCenterIdxChanged(PLWheelView *pView, unsigned int centerIdx)
{
	CCLOG("centerId %d.", centerIdx);
	MapViewCell *pCell = (MapViewCell *)m_pView->cellAtIndex(m_uCenterIdx);
	if (pCell)
	{
		pCell->setIsCenter(false);
	}
	m_uCenterIdx = centerIdx;
	pCell = (MapViewCell *)m_pView->cellAtIndex(m_uCenterIdx);
	if (pCell)
	{
		pCell->setIsCenter(true);
		addArmatureToCell(m_pArmatureCell, pCell);
	}

	pve::MapInfo * pMapInfo = (pve::MapInfo *)maps.objectAtIndex(m_uCenterIdx);
	if (pMapInfo)
	{
		updateDropInfo(pMapInfo->getMapId());
	}

	m_pNodeBg->stopAllActions();
	pve::MapInfo *pInfo = (pve::MapInfo *)maps.objectAtIndex(m_uCenterIdx);
	m_pNodeBg->runAction(CCMoveTo::create(0.5, ccpSub(m_ptOrigin, ccp(pInfo->posX, pInfo->posY))));
	m_pNodeBg->runAction(CCScaleTo::create(0.5, pInfo->scale));
}

void MapView::callBackWonder(CCObject *pObj)
{
	Game::sharedGame()->NavigateTo(navigation::kNavWonder);
}

void MapView::touchElite(CCObject *obj)
{
	if(m_pDelegate)
	{
		m_pDelegate->showEliteView();
	}
}

void MapView::addArmatureToCell(sp::Armature *pArmature, CCTableViewCell *pCell)
{
	if (!pArmature || !pCell)
	{
		return;
	}
	pArmature->removeFromParent();
	pArmature->setPosition(ccp(290, 65));
	pCell->addChild(pArmature);
}

void MapView::updateDropInfo(unsigned int mapId)
{
	int nCid = m_pChapterInfo->getCid();
	m_pBoxDrop0->setVisible(false);
	m_pBoxDrop1->setVisible(false);
	CCArray *pRew = m_pPveModel->getChapterDrop(nCid);
	if (pRew && pRew->count() > 0)
	{
		pve::MapInfo *pInfo = NULL;
		CCObject* pObj = NULL;
		CCARRAY_FOREACH(pRew, pObj)
		{
			pInfo = dynamic_cast<pve::MapInfo *>(pObj);
			if (pInfo)
			{
				if ( pInfo->getMapId() == mapId)
				{
					break;
				}
				else
				{
					pInfo = NULL;
				}
			}		
		}
		if(pInfo)
		{
			if(pInfo->getMapId() == pInfo->chapterInfo->lastMid)
			{
				if(m_pDropTitel) m_pDropTitel->setVisible(false);
				if(m_pDropBgBoss) m_pDropBgBoss->setVisible(true);
				if(m_pDropBg) m_pDropBg->setVisible(false);
			}
			else
			{
				if(m_pDropTitel) m_pDropTitel->setVisible(true);
				if(m_pDropBgBoss) m_pDropBgBoss->setVisible(false);
				if(m_pDropBg) m_pDropBg->setVisible(true);
			}
		}
		if (pInfo && pInfo->prizes_id.size() > 0 && pInfo->prizes_type.size() > 0)
		{
			drop::Drop *dropItem= drop::Drop::create((drop::DropType)pInfo->prizes_type[0], pInfo->prizes_id[0]);
			m_pBoxDrop0->updateDropItem(dropItem);
			m_pBoxDrop0->setVisible(true);
			m_pBoxDrop0->getTouchEvent()->setTag(0);
			m_pBoxDrop0->getTouchEvent()->setTarget(this,menu_selector(MapView::dropTouchEventHandler));
			m_dropItems->insertObject(dropItem,0);
			if (pInfo->prizes_id.size() > 1 && pInfo->prizes_type.size() > 1)
			{
				dropItem = drop::Drop::create((drop::DropType)pInfo->prizes_type[1], pInfo->prizes_id[1]);
				m_pBoxDrop1->updateDropItem(dropItem);
				m_pBoxDrop1->setVisible(true);
				m_dropItems->insertObject(dropItem,1);
				m_pBoxDrop1->getTouchEvent()->setTag(1);
				m_pBoxDrop1->getTouchEvent()->setTarget(this,menu_selector(MapView::dropTouchEventHandler));
			}
		}
		/*if (pRew->count() > 1)
		{
		pve::MapInfo *pInfo = (pve::MapInfo *)pRew->objectAtIndex(1);
		m_pBoxDrop1->updateView(drop::Drop::create((drop::DropType)pInfo->prize_type, pInfo->prize_id));
		}*/
	}
}

void MapView::dropTouchEventHandler(CCObject* target)
{
	CCMenuItem *btn = static_cast<CCMenuItem*>(target);
	int tag = btn->getTag();

	if(m_dropItems->objectAtIndex(tag))
	{
		drop::Drop *dropItem = static_cast<drop::Drop*>(m_dropItems->objectAtIndex(tag));
		if(dropItem->getType() == drop::kDropTypeItem)
		{
			item::ItemBase* baseInfo = dropItem->getItem()->getBaseInfo();
			if(baseInfo)
			{
				ItemInfoView *m_pItemInfoView = ItemInfoView::create();
				m_pItemInfoView->setItemInfo(baseInfo);
				if(m_pDelegate)
				{
					m_pItemInfoView->setPveDelegate(m_pDelegate);
					m_pDelegate->showDropItemInfo(m_pItemInfoView);
				}
			}
		}
		else if(dropItem->getType() == drop::kDropTypeEquip)
		{
			equip::EquipBaseInfo* baseInfo = dropItem->getEquip()->getBaseInfo();
			if(baseInfo)
			{
				EquipInfoView *m_pEquipInfoView = EquipInfoView::create(EquipInfoView::kTypeViewOnly);
				PL_MOVE_NODE_ABOVE_CENTER_Y(m_pEquipInfoView);
				m_pEquipInfoView->setEquip(baseInfo);
				
				if(m_pDelegate)
				{
					m_pEquipInfoView->setPveDelegate(m_pDelegate);
					m_pDelegate->showDropItemInfo(m_pEquipInfoView);
				}
			}

		}
		else if(dropItem->getType() == drop::kDropTypeSoul)
		{
			warrior::WarriorBaseInfo* baseInfo = dropItem->getSoul()->getBaseInfo();
			if(baseInfo)
			{
				WarriorInfoView *m_pSoulInfoView = WarriorInfoView::create(WarriorInfoView::kTypeCloseOnly);
				m_pSoulInfoView->setWarrior(baseInfo);
				if(m_pDelegate)
				{
					m_pSoulInfoView->setPveDelegate(m_pDelegate);
					m_pDelegate->showDropItemInfo(m_pSoulInfoView,false);
				}
			}
		}
	}
}

//MARK: - MapViewCell -

MapViewCell::MapViewCell():m_Node(NULL),m_pMapInfo(NULL),m_pDelegate(NULL),m_pPveModel(NULL), m_pUserModel(NULL),m_nRushTimer(0.f)
// 	m_extNode(NULL), m_MapBackDesc(NULL), m_pEquipBox1(NULL),
// m_pRushTimeBack(NULL),m_bNewMap(NULL),m_haveTimes(NULL),m_pMapDesc(NULL),m_pRichMapDesc(NULL),m_NeedMoney(NULL),m_NeedMoneyNum(NULL),
// m_pHaveTimesNode(NULL),m_pRushNode(NULL),
// m_pItemNone1(NULL), m_limitFight(NULL),m_bossBack(NULL),m_lbBack(NULL),m_normalBack(NULL),
// m_BossIcon(NULL),m_suggestLev(NULL),
// ,m_pNewMapEffectNode(NULL)
,m_pNodeChapter(NULL)
,m_pNodeMapInfo(NULL)
,m_pBMFMapName(NULL)
,m_pBMFChapterName(NULL)
,m_pBtnMap(NULL)
,m_pBtnMapBoss(NULL)
,m_pBMFNeedPower(NULL)
,m_pBMFRest(NULL)
,m_pImageNode(NULL)
,m_pAnimationNode(NULL)
,m_pAnimateTouch(NULL)
,m_pStarBack(NULL)
{
    D_RESET_C_ARRAY(m_star, STARS);
	D_RESET_C_ARRAY(m_images, STARS);
	D_RESET_C_ARRAY(m_animations, STARS);
}

MapViewCell::~MapViewCell()
{
    
	UNBINDING_ALL(m_pUserModel, this);
    SAFE_RELEASE_UNBINDING_ALL(m_pMapInfo, this);
// 	CC_SAFE_RELEASE(m_extNode);
// 	CC_SAFE_RELEASE(m_MapBackDesc);
// 	CC_SAFE_RELEASE(m_pEquipBox1);
// 	CC_SAFE_RELEASE(m_bNewMap);
// 	CC_SAFE_RELEASE(m_haveTimes);
// 	CC_SAFE_RELEASE(m_pMapDesc);
// 	CC_SAFE_RELEASE(m_pRichMapDesc);
// 	CC_SAFE_RELEASE(m_NeedMoney);
// 	CC_SAFE_RELEASE(m_NeedMoneyNum);
// 	CC_SAFE_RELEASE(m_pHaveTimesNode);
// 	CC_SAFE_RELEASE(m_pRushNode);
// 	CC_SAFE_RELEASE(m_pRushTimeBack);
//     
// 	CC_SAFE_RELEASE(m_pItemNone1);
// 	CC_SAFE_RELEASE(m_limitFight);
// 	CC_SAFE_RELEASE(m_bossBack);
// 	CC_SAFE_RELEASE(m_lbBack);
// 	CC_SAFE_RELEASE(m_normalBack);
// 
// 	CC_SAFE_RELEASE(m_BossIcon);
// 	CC_SAFE_RELEASE(m_suggestLev);
//     CC_SAFE_RELEASE(m_pNewMapEffectNode);
     D_SAFE_RELEASE_C_ARRAY(m_star, STARS);
	 D_SAFE_RELEASE_C_ARRAY(m_images, STARS);
	 D_SAFE_RELEASE_C_ARRAY(m_animations, STARS);

	CC_SAFE_RELEASE(m_pNodeMapInfo);
	CC_SAFE_RELEASE(m_pNodeChapter);
	CC_SAFE_RELEASE(m_pBMFMapName);
	CC_SAFE_RELEASE(m_pBMFChapterName);
	CC_SAFE_RELEASE(m_pBtnMap);
	CC_SAFE_RELEASE(m_pBMFNeedPower);
	CC_SAFE_RELEASE(m_pBMFRest);
    CC_SAFE_RELEASE(m_pPveModel);
    CC_SAFE_RELEASE(m_pUserModel);
	CC_SAFE_RELEASE(m_pImageNode);
	CC_SAFE_RELEASE(m_pAnimationNode);
	CC_SAFE_RELEASE(m_pAnimateTouch);
	CC_SAFE_RELEASE(m_pStarBack);
	CC_SAFE_RELEASE(m_pBtnMapBoss);
}

MapViewCell* MapViewCell::create(cocos2d::extension::CCData* pData, PveDelegate* pDelegate, MapView* pMapView)
{
	MapViewCell *pCell = new MapViewCell();
	if (pCell && pCell->init(pData, pDelegate, pMapView))
	{
		pCell->autorelease();
		return pCell;
	}
    
	CC_SAFE_DELETE(pCell);
	return NULL;
}

bool MapViewCell::init(cocos2d::extension::CCData* pData, PveDelegate* pDelegate, MapView* pMapView)
{
	m_pPveModel		= DM_GET_PVE_MODEL;
	m_pUserModel	= DM_GET_USER_MODEL;
    
    CC_SAFE_RETAIN(m_pPveModel);
    CC_SAFE_RETAIN(m_pUserModel);
    
	m_pDelegate		= pDelegate;
    m_pMapView      = pMapView;
    
    CCB_READER_DATA(pData,m_Node);
    if(m_Node) addChild(m_Node);


	m_pNodeChapter->setVisible(false);
	m_pNodeMapInfo->setVisible(true);

// 	if (NULL == m_pRichMapDesc)
// 	{
// 
// 		m_pRichMapDesc = CCRichLabelTTF::create();
// 
// 		CC_SAFE_RETAIN(m_pRichMapDesc);
// 
// 		replaceLabel(m_pMapDesc,m_pRichMapDesc);
// 		m_pMapDesc->setString("");
// 	}
    return true;
}

void MapViewCell::onEnter()
{
    CCTableViewCell::onEnter();
    NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(MapViewCell::updateRushStatus), D_EVENT_USER_VIP_LEVELUP);
	BINDING_EXEC(m_pUserModel, this, MapViewCell::updateRushStatus, D_EVENT_GET_PRIVILEGES);
}

void MapViewCell::onExit()
{
    CCTableViewCell::onExit();
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    UNBINDING_ALL(m_pUserModel, this);
    TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
}

void MapViewCell::onRushUpdated()
{
    /*user::UserInfo* pInfo = m_pUserModel->getUserInfo();
    m_nRushTimer = pInfo->getRushCD();
    if(m_nRushTimer>0.f) TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
    update(0.f);*/
}

void MapViewCell::update(float dt)
{
     
}

bool MapViewCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_RushBtnInfo",	CCNode *,			m_pRushNode);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHaveTimesNode",CCNode *,			m_pHaveTimesNode);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_extNode",		CCNode *,			m_extNode);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_MapBackDesc",	CCLabelTTF *,		m_MapBackDesc);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipBox1",	ItemBox *,          m_pEquipBox1);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bNewMap",		CCSprite *,			m_bNewMap);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_haveTimes",	CCLabelBMFont *,	m_haveTimes);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_mapName",		CCLabelTTF *,       m_pMapDesc);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_NeedMoney",	CCNode *,			m_NeedMoney);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_NeedMoneyNum",	CCLabelBMFont *,	m_NeedMoneyNum);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRushTimeBack",CCScale9Sprite *,	m_pRushTimeBack);
//     
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemNone1",	CCSprite *,			m_pItemNone1);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_limitFight",	CCSprite *,			m_limitFight);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bossBack",		CCNode *,           m_bossBack);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_lbBack",		CCNode *,           m_lbBack);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_normalBack",	CCNode *,           m_normalBack);
// 
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_BossIcon",		CCSprite *,			m_BossIcon);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_suggestLev",	CCLabelBMFont *,	m_suggestLev);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeStar0",		CCSprite *,			m_star[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeStar1",		CCSprite *,			m_star[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeStar2",		CCSprite *,			m_star[2]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_image0",		CCNode *,			m_images[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_image1",		CCNode *,			m_images[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_image2",		CCNode *,			m_images[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_animation0",		CCNode *,			m_animations[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_animation1",		CCNode *,			m_animations[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_animation2",		CCNode *,			m_animations[2]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImageNode",		CCNode *,			m_pImageNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimationNode",		CCNode *,			m_pAnimationNode);
//     
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNewMapEffectNode",	CCNode *,	m_pNewMapEffectNode);
    

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeChapter", CCNode *, m_pNodeChapter);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeMapInfo", CCNode *, m_pNodeMapInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFChapterName", CCLabelBMFont *, m_pBMFChapterName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFMapName", CCLabelBMFont *, m_pBMFMapName); 
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnMap", CCMenuItem *, m_pBtnMap);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnMapBoss" , CCMenuItem*, m_pBtnMapBoss);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFNeedPower", CCLabelBMFont *, m_pBMFNeedPower);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRest", CCLabelBMFont *, m_pBMFRest);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateTouch", UIAnimate *, m_pAnimateTouch);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStarBack", CCNode *, m_pStarBack);

	return false;
}

SEL_MenuHandler MapViewCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
// 	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onRush", MapViewCell::onRush);
// 	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onExplore", MapViewCell::onExplore);
// 	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchNode", MapViewCell::onTouchNode);


	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchArrow", MapViewCell::onExplore);
    
	return NULL;
}

void MapViewCell::onTouchNode(cocos2d::CCObject *p)
{
    if (m_pMapView)
    {
        m_pMapView->onTouchCell(this);
    }
}

void MapViewCell::updateView()
{
    
	if (!m_pMapInfo)
	{
		return;
	}
    
	m_pBMFMapName->setString(m_pMapInfo->map_name.c_str());

// 	m_MapBackDesc->setString(m_pMapInfo->map_des.c_str());
// 	m_bNewMap->setVisible(m_pMapInfo->iTimes==0);
//     m_pNewMapEffectNode->setVisible(m_bNewMap->isVisible());
//     m_pNewMapEffectNode->removeAllChildren();
//     if(m_pNewMapEffectNode->isVisible())
//     {
//         m_pNewMapEffectNode->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("pve_new"));
//     }
//     m_pRichMapDesc->enableStroke(ccc3(255, 255, 255), 1);
// 	m_pRichMapDesc->setString(CCString::createWithFormat("%d.%s",m_pMapInfo->getMapId()%100, m_pMapInfo->map_name.c_str())->getCString());
//     m_suggestLev->setString(CCString::createWithFormat("%d", m_pMapInfo->mapLv)->getCString());
//     
//     m_limitFight->setVisible(false);
//     m_pHaveTimesNode->setVisible(false);
//     m_NeedMoney->setVisible(false);
// 
//     
//     if (m_pMapInfo->challenge_limit == 999)
//     {
//         m_limitFight->setVisible(true);
//     }
//     else
//     {
//         m_pHaveTimesNode->setVisible(true);
//         int nHaveTimes = m_pMapInfo->remainTimes();
//         m_haveTimes->setString(CCString::createWithFormat("%d", nHaveTimes)->getCString());
//         
//         if (nHaveTimes == 0)
//         {
//             m_NeedMoney->setVisible(true);
//             m_NeedMoneyNum->setString(CCString::createWithFormat("%d",DM_GET_MALL_MODEL->getProductPriceById(D_CONFIG_UINT_FOR_KEY(PRODUCT_PVE_EXEC_TIMES)))->getCString());
// 
//         }
//     }
//     
//     
//     m_pItemNone1->setVisible(false);
//     m_pEquipBox1->getGrade()->setVisible(true);
//     m_pEquipBox1->getContainer()->removeAllChildren();
//     
//     switch (m_pMapInfo->prize_type)
//     {
//         case pve::kPveRewardTypeEquip:
//         {
//             equip::EquipBaseInfo* pInfo = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(m_pMapInfo->prize_id);
//             m_pEquipBox1->getGrade()->setColor(DM_GET_EQUIP_MODEL->getEquipColorByGrade(pInfo->getEquipGrade()));
//             m_pEquipBox1->getContainer()->addChild(ResourceManager::sharedResourceManager()->getEquipIcon50P(m_pMapInfo->prize_id));
//             break;
//         }
//         case pve::kPveRewardTypeItem:
//         {
//             m_pEquipBox1->getGrade()->setVisible(false);
//             m_pEquipBox1->getContainer()->addChild(ResourceManager::sharedResourceManager()->getItemIcon50P(m_pMapInfo->prize_id));
//             break;
//         }
//         case pve::kPveRewardTypeSoul:
//         case pve::kPveRewardTypeWarrior:
//         {
//             m_pEquipBox1->getGrade()->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(m_pMapInfo->prize_id));
//             m_pEquipBox1->getContainer()->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon40P(m_pMapInfo->prize_id));
//             
//             break;
//         }
//         case pve::kPveRewardTypeNone:
//         default:
//             m_pItemNone1->setVisible(true);
//             m_pEquipBox1->getGrade()->setVisible(false);
//             break;
//     }
//         
// 	updateRushStatus();
//     
// 
//     m_bossBack->setVisible(false);
//     m_lbBack->setVisible(false);
//     m_normalBack->setVisible(false);
//     m_BossIcon->setVisible(false);
//     
//     int nType = m_pMapInfo->mapType;
//     if (nType == pve::eTypeNormal)
//     {
// 
//         m_normalBack->setVisible(true);
//     }
//     else if (nType == pve::eTypeLittleBoss)
//     {
// 
//         m_lbBack->setVisible(true);
//     }
//     else if (nType == pve::eTypeBoss)
//     {
// 
//         m_bossBack->setVisible(true);
//         m_BossIcon->setVisible(true);
//     }

	pve::ChapterInfo *m_chapeterInfo = DM_GET_PVE_MODEL->getChapterInfo(m_pMapInfo->chapter_id);
	if(m_pMapInfo->getMapId() == m_chapeterInfo->lastMid)
	{
		//龙头显示，否则隐藏
		m_pBtnMapBoss->setVisible(true);
		m_pBtnMap->setVisible(false);
	}
	else
	{
		m_pBtnMap->setVisible(true);
		m_pBtnMapBoss->setVisible(false);
	}

    for (uint8_t i=0; i<STARS; ++i)
    {
		m_star[i]->setVisible(i < m_pMapInfo->iScores);
//         if (i < m_pMapInfo->iScores)
//         {
//             m_star[i]->setDisplayFrame(m_pMapView->getLightStarDisplayFrame());
//         }
//         else
//         {
//             m_star[i]->setDisplayFrame(m_pMapView->getNormalStarDisplayFrame());
//         }
    }
	m_pStarBack->setVisible(m_pMapInfo->iScores > 0);
	m_images[0]->setVisible(m_pMapInfo->iScores == 0);
	m_images[1]->setVisible(m_pMapInfo->iScores == 1 || m_pMapInfo->iScores == 2);
	m_images[2]->setVisible(m_pMapInfo->iScores == 3);
	m_animations[0]->setVisible(m_pMapInfo->iScores == 0);
	m_animations[1]->setVisible(m_pMapInfo->iScores == 1 || m_pMapInfo->iScores == 2);
	m_animations[2]->setVisible(m_pMapInfo->iScores == 3);


	m_pBMFNeedPower->setString(D_CSTRING_FROM_UINT(m_pMapInfo->fight_open));
	m_pBMFRest->setString(D_CSTRING_FROM_UINT(m_pMapInfo->remainTimes()));
}

void MapViewCell::updateRushStatus()
{
//     if(!m_pMapInfo) return;
//     if (m_pUserModel->getUserInfo()->getVipLevel() < D_CONFIG_UINT_FOR_KEY(PVE_RUSH_MIN_VIPLEV) && !DM_GET_USER_MODEL->checkPrivilegeType(user::PrivilegeTypeRush))
// 	{
// 		m_pRushNode->setVisible(false);
// 	}
// 	else
// 	{
// 		m_pRushNode->setVisible(m_pMapInfo->iScores>0);
// 	}
}

void MapViewCell::updateCell(pve::MapInfo *mapInfo, bool bExpand)
{
//     float fHeight = PVE_MAP_CELL_MAX_SIZE.height-PVE_MAP_CELL_MIN_SIZE.height;
// 	
// 	m_extNode->setVisible(bExpand);
// 	if (!bExpand)
// 	{
// 		m_Node->setPosition(0.f, -fHeight);
// 	}
// 	else
// 	{
// 		m_Node->setPosition(0.f, 0.f);
// 	}

	
	/*m_pImageNode->setVisible(!m_pBtnMap->isEnabled());
	m_pAnimationNode->setVisible(m_pBtnMap->isEnabled());*/

	if(m_pMapInfo==mapInfo) return;
	

	SAFE_RELEASE_UNBINDING_ALL(m_pMapInfo, this);
	CC_SAFE_RETAIN(mapInfo);
	m_pMapInfo = mapInfo;
	BINDING_EXEC(m_pMapInfo, this, MapViewCell::updateView, D_EVENT_PVE_MAP_UPDATED);
 	
}

void MapViewCell::onRush(CCObject *p)
{
	user::UserInfo* pInfo = m_pUserModel->getUserInfo();
    
	float m_nRushTimer = pInfo->getRushCD();
    if (m_nRushTimer < 0.f)
    {
       m_nRushTimer = 0.f;
    }
    
    if (m_nRushTimer > 0.f)
    {
        if (pInfo->getVipLevel() < D_CONFIG_UINT_FOR_KEY(PVE_RUSH_MIN_USEPOINT_VIPLEV))
        {
            std::string strTips = D_LOCAL_STRING("VIP%dUserCanUse%dMoneyClearRushCD", D_CONFIG_UINT_FOR_KEY(PVE_RUSH_MIN_USEPOINT_VIPLEV), pInfo->getRushCDCost()).c_str();
            NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV,ccs(strTips));
            return;
        }
        else
        {
            if (pInfo->getPoints() < pInfo->getRushCDCost())
            {
                NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
                return;
            }
            else
            {
                if(m_pDelegate) m_pDelegate->clearRushCD();
                return;
            }
        }
    }
    
	if (m_pMapInfo)
	{
        if(m_pDelegate) m_pDelegate->rushMap(m_pMapInfo->getMapId());
	}
}

void MapViewCell::onExplore(CCObject *p)
{
	if (m_pMapInfo == NULL)
	{
		return;
	}
	
	if (m_pMapInfo->remainTimes()==0)
	{
		if(m_pDelegate)
		{
			m_pDelegate->playOnceMore(m_pMapInfo->getMapId());
		}
	}
	else
	{
		if (m_pMapView)
		{
			m_pMapView->prepareToExitForMap(m_pMapInfo->getMapId());
		}
		
		//m_pDelegate->showMapInfoView(m_pMapInfo->getMapId());
	}
}



void MapViewCell::setIsCenter(bool isCenter)
{
	m_pBtnMap->setEnabled(isCenter);
	m_pBtnMapBoss->setEnabled(isCenter);
	m_pImageNode->setVisible(false);
	m_pAnimationNode->setVisible(isCenter);
	if (isCenter)
	{
		m_pBtnMap->selected();
		m_pBtnMapBoss->selected();
	}
	else
	{
		m_pBtnMap->unselected();
		m_pBtnMapBoss->unselected();
	}
}

void MapViewCell::showTouch(bool bShow)
{
	m_pAnimateTouch->setVisible(bShow);
}

//MARK: - AllStarAwardCell -

AllStarAwardCell::AllStarAwardCell()
:m_pReward(NULL)
,m_pChapterInfo(NULL)
,m_pGet(NULL)
,m_pGotten(NULL)
,m_pAnimate(NULL)
,m_pReplay(NULL)
{
   
}

AllStarAwardCell::~AllStarAwardCell()
{
    CC_SAFE_RELEASE(m_pReward);
    CC_SAFE_RELEASE(m_pGet);
	CC_SAFE_RELEASE(m_pGotten);
	CC_SAFE_RELEASE(m_pAnimate);
	CC_SAFE_RELEASE(m_pReplay);
    
    SAFE_RELEASE_UNBINDING_ALL(m_pChapterInfo, this);
}
    
bool AllStarAwardCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReward",	CCMenuItem *,			m_pReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGet",     CCNode *,			m_pGet);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGotten",	CCNode *,			m_pGotten);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimate", CCNode *, m_pAnimate);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReplay", CCMenuItem *, m_pReplay);
	return false;
};

AllStarAwardCell* AllStarAwardCell::create(cocos2d::extension::CCData* pData, PveDelegate* pDelegate)
{
    AllStarAwardCell *pCell = new AllStarAwardCell();
	if (pCell && pCell->init(pData, pDelegate))
	{
		pCell->autorelease();
		return pCell;
	}
    
	CC_SAFE_DELETE(pCell);
	return NULL;
}

bool AllStarAwardCell::init(cocos2d::extension::CCData* pData, PveDelegate* pDelegate)
{
   
    CCB_READER_DATA_AND_ADDCHILD(pData);
    
    m_pDelegate = pDelegate;
    
    m_pReward->setTarget(this, menu_selector(AllStarAwardCell::rewardCallBack));
    m_pReplay->setTarget(this, menu_selector(AllStarAwardCell::replayCallBack));

    return true;
}

void AllStarAwardCell::onEnter()
{
    CCTableViewCell::onEnter();
    BINDING_EXEC(m_pChapterInfo, this, AllStarAwardCell::updateView, D_EVENT_PVE_CHAPTER_UPDATED);
}

void AllStarAwardCell::onExit()
{
    CCTableViewCell::onExit();
    UNBINDING_ALL(m_pChapterInfo, this);
}

void AllStarAwardCell::rewardCallBack(cocos2d::CCObject* sender)
{
    if(m_pDelegate)
    {
        m_pDelegate->getGradeAward(m_pChapterInfo->getCid());
    }
}

void AllStarAwardCell::replayCallBack(cocos2d::CCObject* sender)
{
	if(m_pDelegate)
	{
		m_pDelegate->replayDialog(m_pChapterInfo);
	}
}

void AllStarAwardCell::updateView()
{
    if(!m_pChapterInfo)
    {
        setVisible(false);
        return;
    }
    setVisible(true);
    
    m_pGotten->setVisible(m_pChapterInfo->grade_award);
    
    m_pGet->setVisible(!m_pGotten->isVisible());
    
    m_pReward->setEnabled(m_pChapterInfo->canGetAllStarReward());

	m_pAnimate->setVisible(m_pChapterInfo->canGetAllStarReward());

	m_pReplay->setEnabled(DM_GET_PVE_MODEL->getMapInfo(m_pChapterInfo->lastMid)->iScores > 0);
    
}


