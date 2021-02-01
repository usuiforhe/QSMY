//
//  ChapterView.cpp
//  QSMY
//
//  Created by wanghejun on 13-4-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ChapterView.h"
#include "Pve.h"
#include "managers/ResourceManager.h"

#include "managers/DataManager.h"
#include "managers/GameManager.h"
//#include "utils/TimeUtil.h"
#include "utils/TimerUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PVE_CHAPTER_SCROLL_SIZE CCSizeMake(640, 670)
#define PVE_CHAPTER_CELL_SIZE CCSizeMake(574, 64)
#define PVE_BACK_OFFSET CCSizeMake(-120, 0)

#define PVE_NOACTION_DURATION	3.f
#define PVE_BACK_MIN_SCALE	0.6f
#define PVE_BACK_MAX_SCALE	1.5f
#define PVE_BACK_EXIT_SCALE	2.0f
#define PVE_ACTION_INTERVAL	0.2f

#define PVE_MAP_BUILDING_TAG	1
#define PVE_MAP_ARROW_TAG	2
#define PVE_MAP_NAME_TAG	3

#define PVE_ARROW_POS_PLUS	ccp(0, -20)

ChapterView::ChapterView()
:m_pDelegate(NULL)
, m_pContainer(NULL)
, m_pLblPlan(NULL)
, m_pView(NULL)
, m_pPveModel(NULL)
, m_pCCData(NULL)
, m_nCid(0)
, m_pScrBack(NULL)
, m_pCCBBack(NULL)
, m_pSprBack(NULL)
, m_pNodeAllStar(NULL)
, m_pBoxPlan(NULL)
, m_pNodeBack(NULL)
, m_pNodeBg(NULL)
, m_pNodeDrop(NULL)
, m_pBtnRush(NULL)
, m_pNodeRotation(NULL)
, m_pNodeDraggingDown(NULL)
, m_pNodeDraggingUp(NULL)
, m_pBtnPve(NULL)
, m_pBtnWonder(NULL)
, m_pNodeFame(NULL)
, m_pBackContainer(NULL)
, m_uCenterIdx(0)
, m_bNeedUpdatePos(false)
, m_ptDeltaPos(0, 0)
, m_fDeltaScale(0)
,m_bTouching(false)
,m_pArmatureBuilding(NULL)
,m_pArrCell(NULL)
,m_fMaxPct(0.f)
,m_fMinPct(0.f)
,m_bWheeling(false)
,m_pArmatureCell(NULL)
,m_pNodeWheel(NULL)
,m_bFromMapList(false)
,m_pArmatureEnv(NULL)
,m_RushTime(NULL)
,m_pCoinTip(NULL)
,m_pBtnElite(NULL)
,m_pTianqiBox(NULL)
,m_pTianqiBtn(NULL)
,m_pActivityTime(NULL)
,m_ActivityEndT(0)
{
	D_RESET_C_ARRAY(m_pMapBuild, CHAPTER_MAP_MAX);
	D_RESET_C_ARRAY(m_pMapName, CHAPTER_MAP_MAX);
	D_RESET_C_ARRAY(m_pMapArrow, CHAPTER_MAP_MAX);
}

ChapterView::~ChapterView()
{
	unscheduleAllSelectors();
    removeAllChildren();
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pLblPlan);
	CC_SAFE_RELEASE(m_pCCData);
	CC_SAFE_RELEASE(m_pScrBack);
	CC_SAFE_RELEASE(m_pCCBBack);
	CC_SAFE_RELEASE(m_pSprBack);
	CC_SAFE_RELEASE(m_pNodeAllStar);
	CC_SAFE_RELEASE(m_pNodeBack);
	CC_SAFE_RELEASE(m_pNodeBg);
	CC_SAFE_RELEASE(m_pNodeDrop);
	CC_SAFE_RELEASE(m_pBtnRush);
	CC_SAFE_RELEASE(m_pNodeRotation);
	CC_SAFE_RELEASE(m_pNodeDraggingDown);
	CC_SAFE_RELEASE(m_pNodeDraggingUp);
	CC_SAFE_RELEASE(m_pBtnPve);
	CC_SAFE_RELEASE(m_pBtnWonder);
	CC_SAFE_RELEASE(m_pNodeFame);
	CC_SAFE_RELEASE(m_pBackContainer);
	CC_SAFE_RELEASE(m_pArmatureBuilding);
	D_SAFE_RELEASE_C_ARRAY(m_pMapBuild, CHAPTER_MAP_MAX);
	D_SAFE_RELEASE_C_ARRAY(m_pMapName, CHAPTER_MAP_MAX);
	D_SAFE_RELEASE_C_ARRAY(m_pMapArrow, CHAPTER_MAP_MAX);
	CC_SAFE_RELEASE(m_pArrCell);
	CC_SAFE_RELEASE(m_pArmatureCell);
	CC_SAFE_RELEASE(m_pNodeWheel);
	CC_SAFE_RELEASE(m_RushTime);
	CC_SAFE_RELEASE(m_pCoinTip);
	CC_SAFE_RELEASE(m_pBtnElite);
	CC_SAFE_RELEASE(m_pTianqiBox);
	CC_SAFE_RELEASE(m_pTianqiBtn);
	CC_SAFE_RELEASE(m_pActivityTime);
}

bool ChapterView::init()
{
    if(!DLayer::init())
    {
        return false;
    }

//	m_pSprBack = CCSprite::create("PL_ui/map/chapter_map.png");

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("RewardItemBox", RewardItemBoxLoader::loader());
//	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_map/PL_map", pNodeLoaderLibrary);

	cocos2d::CCNode *__pNode__ = NULL;   
	cocos2d::extension::CCBReader * ___ccbReader__ = new DCCBReader(pNodeLoaderLibrary);
	__pNode__ = ___ccbReader__->readNodeGraphFromFile("PL_ui/ccb/pl_map/PL_map", this);
	___ccbReader__->release();
	if(__pNode__) addChild(__pNode__, 2);


	CCNode *pBack = NULL;
	CCB_READER("PL_ui/ccb/pl_map/PL_map_bg", pBack);
	m_pCCBBack = pBack;//dynamic_cast<CCSprite *>(pBack);
	CC_SAFE_RETAIN(m_pCCBBack);
	m_pScrBack = CCScrollView::create(m_pBackContainer->getContentSize(), m_pCCBBack);
	m_pScrBack->setPosition(m_pBackContainer->getPosition());
	m_pScrBack->setTouchEnabled(false);
	addChild(m_pScrBack, 1);
	CC_SAFE_RETAIN(m_pScrBack);

	m_pCCData = readCCData("PL_ui/ccb/pl_map/PL_map_wheel_cell");
	CC_SAFE_RETAIN(m_pCCData);
	    
    m_pPveModel = DM_GET_PVE_MODEL;
    
	m_pArrCell = CCDictionary::create();
	CC_SAFE_RETAIN(m_pArrCell);

    m_pView = PLWheelView::create(this, PVE_CHAPTER_SCROLL_SIZE);
	m_pView->cocos2d::extension::CCTableView::setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pView->setWheelDelegate(this);
    m_pView->setDelegate(this);
    m_pContainer->addChild(m_pView);

	m_pNodeAllStar->setVisible(false);

	m_pBoxPlan->getTouchEvent()->setTarget(this, menu_selector(ChapterView::onTouchEquip));

	m_pNodeBack->setVisible(false);

	m_pNodeDrop->setVisible(false);

	m_pBtnRush->setEnabled(false);
	m_RushTime->setVisible(false);

	m_pNodeDraggingDown->setVisible(false);
	m_pNodeDraggingUp->setVisible(false);

	m_pBtnPve->setEnabled(false);
	m_pBtnPve->selected();
	m_pBtnWonder->setTarget(this, menu_selector(ChapterView::callBackWonder));

	//精英赛
	m_pBtnElite->setTarget(this,menu_selector(ChapterView::touchElite));

	m_pNodeFame->setVisible(false);

	for (int i = 0; i < CHAPTER_MAP_MAX; i++)
	{
		m_pMapBuild[i]->setVisible(false);
		m_pMapName[i]->setVisible(false);
		m_pMapArrow[i]->setVisible(false);
	}
	m_pScrBack->setContentOffset(PVE_BACK_OFFSET);

	m_pArmatureBuilding = ResourceManager::sharedResourceManager()->getUIAnimate("pve_building_center");
	CC_SAFE_RETAIN(m_pArmatureBuilding);
	m_pArmatureCell = ResourceManager::sharedResourceManager()->getUIAnimate("pve_cell_ani");
	CC_SAFE_RETAIN(m_pArmatureCell);

	m_pCoinTip->setVisible(false);
	m_pTianqiBtn->setTarget(this,menu_selector(ChapterView::onOpenTianqi));
    return true;
}

bool ChapterView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeContainer",	CCNode *,	m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLblPlan", CCLabelBMFont *, m_pLblPlan);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAllStar",	CCNode *,			m_pNodeAllStar);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxPlan", RewardItemBox *, m_pBoxPlan);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeBack",	CCNode *,			m_pNodeBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeBg",	CCNode *,			m_pNodeBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDrop",	CCNode *,			m_pNodeDrop);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRush",	CCMenuItem *,			m_pBtnRush);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_RushTimeT", CCLabelBMFont* ,   m_RushTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRotation", CCNode *, m_pNodeRotation);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDraggingDown", CCNode *, m_pNodeDraggingDown);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDraggingUp", CCNode *, m_pNodeDraggingUp);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnPve", CCMenuItem *, m_pBtnPve);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnWonder", CCMenuItem *, m_pBtnWonder);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFame", CCNode *, m_pNodeFame);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackContainer", CCNode *, m_pBackContainer);

	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pMapBuild", CCNode *, m_pMapBuild, CHAPTER_MAP_MAX);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pMapName", CCNode *, m_pMapName, CHAPTER_MAP_MAX);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pNodeArrow", CCNode *, m_pMapArrow, CHAPTER_MAP_MAX);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSprBack", CCSprite *, m_pSprBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeWheel", CCNode *, m_pNodeWheel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCoinTip" , CCNode*, m_pCoinTip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnElite" , CCMenuItem*, m_pBtnElite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTianqiBox" , CCNode*, m_pTianqiBox);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTianqiBtn" , CCMenuItemImage*, m_pTianqiBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pActivityTime", CCLabelTTF* , m_pActivityTime);
	return false;
}

SEL_MenuHandler ChapterView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	return NULL;
}

void ChapterView::onEnter()
{
    DLayer::onEnter();
	//副本活动入口
	m_pTianqiBox->setVisible(m_pPveModel->isActivityShow());
	if(m_pPveModel->isActivityShow())
	{
		m_ActivityEndT = m_pPveModel->getActivityDownTime();
		m_pActivityTime->setString(TimeUtil::getSeconds2String(m_ActivityEndT, "%D:%H:%M:%S",":","%02d",true));
		TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
		TimerUtil::sharedTimerUtil()->schedule(schedule_selector(ChapterView::updateActivityTime), this , 1);
	}

	prepareToEnter();
    BINDING_EXEC(m_pPveModel, this, ChapterView::onChaptersUpdated, D_EVENT_PVE_CHAPTERS_UPDATED);
}

void ChapterView::onExit()
{
    UNBINDING_ALL(m_pPveModel, this);
	//TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
    DLayer::onExit();
}

void ChapterView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	
	return;
}

CCSize ChapterView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return PVE_CHAPTER_CELL_SIZE;
}

CCTableViewCell* ChapterView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
    ChapterViewCell *cell = (ChapterViewCell*)m_pArrCell->objectForKey(idx);
    if(!cell)
    {
        cell = ChapterViewCell::createChapterViewCell(this, m_pCCData , idx+1 == m_pPveModel->getUnlockChartersCount());
		m_pArrCell->setObject(cell, idx);
    } 
    
    pve::ChapterInfo* pChapterInfo = m_pPveModel->getChapterInfoByIndex(m_pPveModel->getUnlockChartersCount()-idx-1);
	cell->updateCell(pChapterInfo);
	cell->setIsCenter(m_uCenterIdx == idx);
	cell->showTouch(m_uCenterIdx == idx && !m_bTouching);
	if (m_uCenterIdx == idx)
	{
		addArmatureToCell(m_pArmatureCell, cell);
	}
    return cell;
}

uint32_t ChapterView::numberOfCellsInTableView(CCTableView *table)
{
    return m_pPveModel->getUnlockChartersCount();
}

void ChapterView::updateView()
{
    m_pView->reloadData();
    rotateTo(m_nCid);
}

void ChapterView::onChaptersUpdated()
{
    if(!m_pPveModel->isSynced)
	{
		return;
	}

    updateView();
}

void ChapterView::onTouchEquip(CCObject *obj)
{
	CCMenuItem *pItem = static_cast<CCMenuItem *>(obj);
	if (m_pDelegate)
	{
		m_pDelegate->showTouchItem(pItem->getTag());
	}
}

void ChapterView::touchChapterCell(const uint32_t &chapterID)
{
    if(m_pDelegate)
    {
		m_pPveModel->setFightType(pve::PveFightNormal);
        pve::ChapterInfo *pChapterInfo = m_pPveModel->getChapterInfo(chapterID);
        if(pChapterInfo->chapter_unlock)
        {
            m_pDelegate->showMapView(pChapterInfo->getCid(), true);
        }
        else
        {
            CCLOG("未解锁的关卡信息");
        }
    }
}

void ChapterView::touchElite(CCObject *obj)
{
	if(m_pDelegate)
	{
		m_pDelegate->showEliteView();
	}
}

void ChapterView::setDelegate(PveDelegate *pDelegate)
{
    m_pDelegate = pDelegate;
}

void ChapterView::rotateTo(const int &cid)
{
    m_nCid = cid;
	CCPoint offset = ccp(0,-(cid-1)*PVE_CHAPTER_CELL_SIZE.height);
    offset.y = MIN(m_pView->maxContainerOffset().y, offset.y);
    offset.y = MAX(m_pView->minContainerOffset().y, offset.y);
    
	m_pView->setContentOffset(offset);
	hideOtherAndName();
	ChapterViewCell *pCell = (ChapterViewCell *)m_pView->cellAtIndex(cid - 1);
	if (pCell)
	{
		//pCell->showTouch(true);
	}
	
// 	pve::ChapterInfo* pInfo = m_pPveModel->getChapterInfoByIndex(cid - 1);
// 	pve::ChapterExtraInfo *pExtra = m_pPveModel->getChapterExtraInfo(pInfo->getCid());
// 	CCPoint backOffset = ccpSub(ccp(0, 0), ccpMult(pExtra->pt, 0.5 * m_pSprBack->getScale()));//ccpSub(ccp(DESIGN_SIZE.width, DESIGN_SIZE.height), ccpMult(pExtra->pt, 0.5 * m_pSprBack->getScale()));
// 	if (backOffset.x > m_pScrBack->maxContainerOffset().x)
// 	{
// 		backOffset.x = m_pScrBack->maxContainerOffset().x;
// 	}
// 	else if (backOffset.x < m_pScrBack->minContainerOffset().x)
// 	{
// 		backOffset.x = m_pScrBack->minContainerOffset().x;
// 	}
// 
// 	if (backOffset.y > m_pScrBack->maxContainerOffset().y)
// 	{
// 		backOffset.y = m_pScrBack->maxContainerOffset().y;
// 	}
// 	else if (backOffset.y < m_pScrBack->minContainerOffset().y)
// 	{
// 		backOffset.y = m_pScrBack->minContainerOffset().y;
// 	}
// 	m_pScrBack->setContentOffset(backOffset);
// 	for (int i = 0; i < CHAPTER_MAP_MAX; i++)
// 	{
// 		m_pNodeMap[i]->setVisible(i == pExtra->section);
// 	}
	//m_pScrBack->setScale(1.25);
}

void ChapterView::PLWheelViewTouched(PLWheelView *pView)
{
	unscheduleAllSelectors();
	m_bTouching = true;
	m_fMaxPct = 0.f;
	m_fMinPct = 0.f;
	m_pNodeRotation->stopAllActions();
	m_bWheeling = false;
	ChapterViewCell *pCell = (ChapterViewCell *)pView->cellAtIndex(m_uCenterIdx);
	pCell->showTouch(false);
}

void ChapterView::PLWheelViewCenterIdxChanged(PLWheelView *pView, unsigned int centerIdx)
{
	ChapterViewCell *pCell = (ChapterViewCell *)m_pView->cellAtIndex(m_uCenterIdx);
	if (pCell)
	{
		pCell->setIsCenter(false);
	}
	m_uCenterIdx = centerIdx;
	pCell = (ChapterViewCell *)m_pView->cellAtIndex(m_uCenterIdx);
	if (pCell)
	{
		pCell->setIsCenter(true);
		addArmatureToCell(m_pArmatureCell, pCell);
	}
//	pve::ChapterInfo* pChapterInfo = m_pPveModel->getChapterInfoByIndex(m_pPveModel->getUnlockChartersCount()-centerIdx-1);
//	this->updateChapter(pChapterInfo);
//	m_nCid = m_pPveModel->getUnlockChartersCount()-centerIdx;
//	updateMapPos(0.f);
	m_bNeedUpdatePos = true;
}

void ChapterView::updateChapter(pve::ChapterInfo *pInfo)
{
	m_pLblPlan->setString(CCString::createWithFormat("%d/%d",pInfo->pres,pInfo->getMaxPrestige())->getCString());
	//m_pScrBack->setContentOffset(ccpSub(ccp(DESIGN_SIZE.width, DESIGN_SIZE.height), m_pPveModel->getChapterPos(pInfo->getCid())));
//	m_pSprBack->setScale(1.0);
	pve::PrestigeReward* pPrestigeReward = pInfo->getAwardToShow();
	m_pBoxPlan->setInfo(pInfo, pPrestigeReward->prestige_reward_need_viplv);
}

void ChapterView::PLWheelViewStartWheeling(PLWheelView *pView, unsigned int centerIdx)
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
	m_nCid = m_pPveModel->getUnlockChartersCount()-centerIdx;
	updateMapPos(PVE_ACTION_INTERVAL);
// 	pve::ChapterInfo* pInfo = m_pPveModel->getChapterInfoByIndex(m_pPveModel->getUnlockChartersCount()-centerIdx-1);
// 	pve::ChapterExtraInfo *pExtra = m_pPveModel->getChapterExtraInfo(pInfo->getCid());
// 	CCPoint offset = ccpSub(ccp(0, 0), ccpMult(pExtra->pt, 0.5 * m_pSprBack->getScale()));//ccpSub(ccp(DESIGN_SIZE.width / 2, DESIGN_SIZE.height / 2), ccpMult(pExtra->pt, 0.5 * m_pSprBack->getScale()));
// 	CCLOG("%f,%f,%f,%f", m_pScrBack->maxContainerOffset().x, m_pScrBack->maxContainerOffset().y, m_pScrBack->minContainerOffset().x, m_pScrBack->minContainerOffset().y);
// 	
// 	if (offset.x > m_pScrBack->maxContainerOffset().x)
// 	{
// 		offset.x = m_pScrBack->maxContainerOffset().x;
// 	}
// 	else if (offset.x < m_pScrBack->minContainerOffset().x)
// 	{
// 		offset.x = m_pScrBack->minContainerOffset().x;
// 	}
// 
// 	if (offset.y > m_pScrBack->maxContainerOffset().y)
// 	{
// 		offset.y = m_pScrBack->maxContainerOffset().y;
// 	}
// 	else if (offset.y < m_pScrBack->minContainerOffset().y)
// 	{
// 		offset.y = m_pScrBack->minContainerOffset().y;
// 	}
// 	
// 	m_pScrBack->setContentOffsetInDuration(offset, 0.2);
// 	for (int i = 0; i < CHAPTER_MAP_MAX; i++)
// 	{
// 		m_pNodeMap[i]->setVisible(i == pExtra->section);
// 	}
	//m_pScrBack->setScale(1.25);
}

void ChapterView::PLWheelViewStopWheeling(PLWheelView *pView, unsigned int centerIdx)
{
	m_bWheeling = false;
// 	pve::ChapterInfo* pInfo = m_pPveModel->getChapterInfoByIndex(m_pPveModel->getUnlockChartersCount()-centerIdx-1);
// 	m_pScrBack->setContentOffsetInDuration(ccpSub(ccp(DESIGN_SIZE.width, DESIGN_SIZE.height), m_pPveModel->getChapterPos(pInfo->getCid())), 0.2);
//	m_pSprBack->setScale(PVE_BACK_MAX_SCALE);
	scheduleOnce(schedule_selector(ChapterView::scaleToMin), PVE_NOACTION_DURATION);
	m_bTouching = false;
	ChapterViewCell *pCell = (ChapterViewCell *)pView->cellAtIndex(m_uCenterIdx);
	pCell->showTouch(true);

	if (!m_bNeedUpdatePos)
	{
		return;
	}

	m_bNeedUpdatePos = false;

	CCLOG("%f,%f,%f,%f", m_pScrBack->maxContainerOffset().x, m_pScrBack->maxContainerOffset().y, m_pScrBack->minContainerOffset().x, m_pScrBack->minContainerOffset().y);
	m_nCid = m_pPveModel->getUnlockChartersCount()-centerIdx;
	schedule(schedule_selector(ChapterView::updateScale));
	showAllWithName();
	

//	m_pCCBBack->stopAllActions();
//	m_pCCBBack->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.2f, PVE_BACK_MAX_SCALE), CCCallFunc::create(this, callfunc_selector(ChapterView::afterScale))));
}

void ChapterView::updateScale(float dt)
{
	if (m_pCCBBack->getScale() >= PVE_BACK_MAX_SCALE)
	{
		unschedule(schedule_selector(ChapterView::updateScale));
		
		return;
	}
	m_pCCBBack->setScale(m_pCCBBack->getScale() + 0.05);
	updateMapPos(0.f);	
}

void ChapterView::scaleToMin(float dt)
{
	unscheduleAllSelectors();

	m_ptDeltaPos = ccpSub(PVE_BACK_OFFSET, m_pScrBack->getContentOffset());
	m_fDeltaScale = PVE_BACK_MIN_SCALE - m_pCCBBack->getScale();
	schedule(schedule_selector(ChapterView::updateScaleMin));
// 	m_pCCBBack->runAction(CCScaleTo::create(PVE_ACTION_MOVE_DUR_ENTER, PVE_BACK_MIN_SCALE));
// 	m_pScrBack->setContentOffsetInDuration(PVE_BACK_OFFSET, PVE_ACTION_MOVE_DUR_EXIT);
}

void ChapterView::updateScaleMin(float dt)
{
	if (m_pCCBBack->getScale() <= PVE_BACK_MIN_SCALE)
	{
		m_pCCBBack->setScale(PVE_BACK_MIN_SCALE);
		m_pScrBack->setContentOffset(PVE_BACK_OFFSET);
		unschedule(schedule_selector(ChapterView::updateScaleMin));
		hideOtherAndName();
		return;
	}
	float dtScale = m_fDeltaScale * dt / PVE_ACTION_INTERVAL;
	CCPoint dtPos = ccpMult(m_ptDeltaPos, dt / PVE_ACTION_INTERVAL);
	m_pCCBBack->setScale(m_pCCBBack->getScale() + dtScale);
	m_pScrBack->setContentOffset(ccpAdd(m_pScrBack->getContentOffset(), dtPos));
	
}

void ChapterView::updateMapPos(float dur)
{
	pve::ChapterInfo* pInfo = m_pPveModel->getChapterInfoByIndex(m_nCid - 1);
	pve::ChapterExtraInfo *pExtra = m_pPveModel->getChapterExtraInfo(pInfo->getCid());
	CCPoint backOffset = ccpSub(ccp(320, 640), ccpMult(pExtra->pt, 0.5 * m_pCCBBack->getScale()));//ccpSub(ccp(DESIGN_SIZE.width, DESIGN_SIZE.height), ccpMult(pExtra->pt, 0.5 * m_pSprBack->getScale()));
	if (backOffset.x > m_pScrBack->maxContainerOffset().x)
	{
		backOffset.x = m_pScrBack->maxContainerOffset().x;
	}
	else if (backOffset.x < m_pScrBack->minContainerOffset().x)
	{
		backOffset.x = m_pScrBack->minContainerOffset().x;
	}

	if (backOffset.y > m_pScrBack->maxContainerOffset().y)
	{
		backOffset.y = m_pScrBack->maxContainerOffset().y;
	}
	else if (backOffset.y < m_pScrBack->minContainerOffset().y)
	{
		backOffset.y = m_pScrBack->minContainerOffset().y;
	}
	if (D_FLOAT_EQUALS(dur, 0.f))
	{
		m_pScrBack->setContentOffset(backOffset);
	}
	else
	{
		m_pScrBack->setContentOffsetInDuration(backOffset, dur);
	}
// 	for (int i = 0; i < CHAPTER_MAP_MAX; i++)
// 	{
// 		m_pNodeMap[i]->setVisible(i + 1 == pExtra->section);
// 	}
}

void ChapterView::showAllWithName()
{
	for (int i = 0; i < CHAPTER_MAP_MAX; i++)
	{
		pve::ChapterInfo *pChapter = m_pPveModel->getChapterInfo(i + 1);
		pve::ChapterExtraInfo *pExtra = m_pPveModel->getChapterExtraInfo(m_nCid);
		m_pMapBuild[i]->setVisible(pChapter->chapter_unlock);
		if (i + 1 == pExtra->section)
		{
			m_pArmatureBuilding->removeFromParent();
			m_pArmatureBuilding->setPosition(ccpAdd(m_pMapArrow[i]->getPosition(), PVE_ARROW_POS_PLUS));
			m_pMapArrow[i]->getParent()->addChild(m_pArmatureBuilding);
		}
		
//		m_pNodeMap[i]->getChildByTag(PVE_MAP_ARROW_TAG)->setVisible(i + 1 == m_nCid);
		m_pMapName[i]->setVisible(i + 1 == pExtra->section);
		
	}
	if (m_nCid > 0)
	{
		if (NULL != m_pArmatureEnv)
		{
			m_pArmatureEnv->removeFromParent();
		}
		pve::ChapterExtraInfo *pExtra = m_pPveModel->getChapterExtraInfo(m_nCid);
		m_pArmatureEnv = ResourceManager::sharedResourceManager()->getUIAnimate(CCString::createWithFormat("chapter_ani_%d", pExtra->ani)->getCString());
		m_pArmatureEnv->setScale(0.8f);
		if (!m_pArmatureEnv)
		{
			return;
		}
		
		m_pArmatureEnv->setPosition(m_pMapBuild[pExtra->section-1]->getPosition());
		m_pMapBuild[pExtra->section-1]->getParent()->addChild(m_pArmatureEnv);
	}
}

void ChapterView::hideOtherAndName()
{
	for (int i = 0; i < CHAPTER_MAP_MAX; i++)
	{
		pve::ChapterExtraInfo *pExtra = m_pPveModel->getChapterExtraInfo(m_nCid);
		m_pMapBuild[i]->setVisible(i + 1 == pExtra->section);
		if (i + 1 == pExtra->section)
		{
			m_pArmatureBuilding->removeFromParent();
			m_pArmatureBuilding->setPosition(ccpAdd(m_pMapArrow[i]->getPosition(), PVE_ARROW_POS_PLUS));
			m_pMapArrow[i]->getParent()->addChild(m_pArmatureBuilding);
		}
//		m_pNodeMap[i]->getChildByTag(PVE_MAP_ARROW_TAG)->setVisible(i + 1 == m_nCid);
		m_pMapName[i]->setVisible(false);
	}
}

void ChapterView::PLWheelViewBouncePercent(PLWheelView *pView, float pct)
{
	if (!m_bWheeling)
	{
		m_pNodeRotation->setRotation(pct * MAX_ROTATION);
	}
	if (pct <= 0.1 && pct >= -0.1)
	{
// 		m_pNodeDraggingDown->stopAllActions();
// 		m_pNodeDraggingUp->stopAllActions();
		m_pNodeDraggingDown->setVisible(true);
		m_pNodeDraggingUp->setVisible(true);
	}
	else
	{
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

void ChapterView::prepareToEnter()
{
	m_pContainer->setPositionX(m_pContainer->getPositionX() + PVE_ACTION_MOVE_DIS);
	if (!m_bFromMapList)
	{
		m_pNodeWheel->setPositionX(m_pNodeWheel->getPositionX() + PVE_ACTION_MOVE_DIS);
	}
	
	CCSprite *pScene = ResourceManager::sharedResourceManager()->getChapterIcon(m_pPveModel->getChapterExtraInfo(m_nCid)->res);
	pScene->setPosition(m_pNodeBg->getPosition());
	this->addChild(pScene);
	m_pCCBBack->setScale(PVE_BACK_EXIT_SCALE);
	m_pCCBBack->stopAllActions();
	m_pCCBBack->runAction(CCScaleTo::create(PVE_ACTION_MOVE_DUR_ENTER, PVE_BACK_MIN_SCALE));
	m_pSprBack->setOpacity(0);
	m_pSprBack->stopAllActions();
	m_pSprBack->runAction(CCSequence::createWithTwoActions(CCFadeTo::create(PVE_ACTION_MOVE_DUR_ENTER, 255),
		CCCallFunc::create(this, callfunc_selector(ChapterView::enterActionFinished))));
}

void ChapterView::enterActionFinished()
{
	m_pContainer->stopAllActions();
	m_pContainer->runAction(CCMoveBy::create(PVE_ACTION_MOVE_DUR_EXIT, ccp(-PVE_ACTION_MOVE_DIS, 0)));
	if (!m_bFromMapList)
	{
		m_pNodeWheel->stopAllActions();
		m_pNodeWheel->runAction(CCMoveBy::create(PVE_ACTION_MOVE_DUR_EXIT / 2, ccp(-PVE_ACTION_MOVE_DIS, 0)));
	}
	
	rotateTo(m_nCid);
}

void ChapterView::prepareToExit(unsigned int cid)
{
	m_pContainer->stopAllActions();
	m_pContainer->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(PVE_ACTION_MOVE_DUR_EXIT, ccp(PVE_ACTION_MOVE_DIS, 0)), 
		CCCallFuncO::create(this, callfuncO_selector(ChapterView::actionFinished), CCInteger::create(cid))));
// 	m_pNodeBg->removeAllChildrenWithCleanup(true);

	CCSprite *pIcon = ResourceManager::sharedResourceManager()->getChapterIcon(m_pPveModel->getChapterExtraInfo(m_nCid)->res);
	pIcon->setPosition(m_pNodeBg->getPosition());
	if (m_pPveModel->getChapterInfo(m_nCid)->isSynced)
	{
		cocos2d::CCArray maps;
		m_pPveModel->getUnlockedMapsByChapterId(m_nCid,maps);
		pve::MapInfo *pMapInfo = (pve::MapInfo *)maps.lastObject();
		pIcon->setScale(pMapInfo->scale);
		pIcon->setPosition(ccpSub(m_pNodeBg->getPosition(), ccp(pMapInfo->posX, pMapInfo->posY)));
	}
	
	this->addChild(pIcon);
	m_pCCBBack->stopAllActions();
	m_pCCBBack->runAction(CCScaleTo::create(PVE_ACTION_MOVE_DUR_EXIT, PVE_BACK_EXIT_SCALE));
	m_pSprBack->stopAllActions();
	m_pSprBack->runAction(CCFadeTo::create(PVE_ACTION_MOVE_DUR_EXIT, 0));
	for (int i = 0; i < CHAPTER_MAP_MAX; i++)
	{
		m_pMapBuild[i]->stopAllActions();
		if (m_pMapBuild[i]->isVisible())
		{
			m_pMapBuild[i]->runAction(CCFadeTo::create(PVE_ACTION_MOVE_DUR_EXIT, 0));
//			m_pNodeMap[i]->getChildByTag(PVE_MAP_ARROW_TAG)->runAction(CCFadeTo::create(PVE_ACTION_MOVE_DUR_EXIT, 0));
			m_pArmatureBuilding->runAction(CCFadeTo::create(PVE_ACTION_MOVE_DUR_EXIT, 0));
		}
		
	}
	
	//m_pSprBack->runAction(CCTintTo::create(PVE_ACTION_MOVE_DUR_EXIT, 255, 255, 255));
}

void ChapterView::actionFinished(CCObject *pObj)
{
	CCInteger *pCid = static_cast<CCInteger *>(pObj);
	touchChapterCell(pCid->getValue());
}

void ChapterView::callBackWonder(CCObject *pObj)
{
	Game::sharedGame()->NavigateTo(navigation::kNavWonder);
}

void ChapterView::addArmatureToCell(sp::Armature *pArmature, CCTableViewCell *pCell)
{
	if (!pArmature || !pCell)
	{
		return;
	}
	pArmature->removeFromParent();
	pArmature->setPosition(ccp(290, 65));
	pCell->addChild(pArmature);
}

void ChapterView::onOpenTianqi(CCObject *obj)
{
	m_pDelegate->showTianqiChapterView();
}

void ChapterView::updateActivityTime(float dt)
{
	m_ActivityEndT--;
	if(m_ActivityEndT<=0)
	{
		m_ActivityEndT = 0;
		TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
		m_pTianqiBox->setVisible(false);
	}
	m_pActivityTime->setString(TimeUtil::getSeconds2String(m_ActivityEndT, "%D:%H:%M:%S",":","%02d",true));
}

ChapterViewCell::ChapterViewCell()
// :m_pBackground(NULL)
// ,m_pLblPlan(NULL)
// ,m_pArrow(NULL)
// ,m_ChapterName(NULL)
// ,m_pRewardItemBox(NULL)
:m_pChapterInfo(NULL)
// ,m_pThreeStarEat(NULL)
// ,m_pThreeStar(NULL)
,m_pNodeChapter(NULL)
,m_pNodeMapInfo(NULL)
,m_pBMFMapName(NULL)
,m_pBMFChapterName(NULL)
,m_pBtnChapter(NULL)
,m_pImageNode(NULL)
,m_pAnimationNode(NULL)
,bCurrentChapter(false)
,m_pAnimateTouch(NULL)
{
	D_RESET_C_ARRAY(m_images, ANIMATION_COUNT);
	D_RESET_C_ARRAY(m_animations, ANIMATION_COUNT);
}
ChapterViewCell::~ChapterViewCell()
{
    removeAllChildren();
// 	CC_SAFE_RELEASE(m_pBackground);
// 	CC_SAFE_RELEASE(m_pLblPlan);
// 	CC_SAFE_RELEASE(m_pArrow);
// 	CC_SAFE_RELEASE(m_ChapterName);
// 	CC_SAFE_RELEASE(m_pRewardItemBox);
//     CC_SAFE_RELEASE(m_pThreeStarEat);
//     CC_SAFE_RELEASE(m_pThreeStar);
	CC_SAFE_RELEASE(m_pNodeMapInfo);
	CC_SAFE_RELEASE(m_pNodeChapter);
	CC_SAFE_RELEASE(m_pBMFMapName);
	CC_SAFE_RELEASE(m_pBMFChapterName);
	CC_SAFE_RELEASE(m_pBtnChapter);

	CC_SAFE_RELEASE(m_pImageNode);
	CC_SAFE_RELEASE(m_pAnimationNode);
	D_SAFE_RELEASE_C_ARRAY(m_images, ANIMATION_COUNT);
	D_SAFE_RELEASE_C_ARRAY(m_animations, ANIMATION_COUNT);

	SAFE_RELEASE_UNBINDING_ALL(m_pChapterInfo, this);
	CC_SAFE_RELEASE(m_pAnimateTouch);
}

ChapterViewCell * ChapterViewCell::createChapterViewCell(ChapterView *chapterView, cocos2d::extension::CCData* pData , bool bCurrentChapter)
{
    ChapterViewCell *pChapterViewCell = new ChapterViewCell();
	pChapterViewCell->bCurrentChapter = bCurrentChapter;

    if(pChapterViewCell && pChapterViewCell->init(chapterView, pData))
    {
        pChapterViewCell->autorelease();
        return pChapterViewCell;
    }
    else
    {
        CC_SAFE_DELETE(pChapterViewCell);
        return pChapterViewCell;
    }
}

bool ChapterViewCell::init(ChapterView *chapterView, cocos2d::extension::CCData* pData)
{
    m_pChapterView = chapterView;

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("RewardItemBox", RewardItemBoxLoader::loader());

	CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pNodeLoaderLibrary);

// 	cocos2d::CCNode *__pNode__ = NULL;   
// 		cocos2d::extension::CCBReader * ___ccbReader__ = new DCCBReader(pNodeLoaderLibrary);
// 		__pNode__ = ___ccbReader__->readNodeGraphFromData(pData, this, CCDirector::sharedDirector()->getWinSize());
// 		___ccbReader__->release();
// 		if(__pNode__) addChild(__pNode__);
// 			__pNode__->setAnchorPoint(ccp(0.5, 0.5));
	//m_pRewardItemBox->getTouchEvent()->setTarget(this, menu_selector(ChapterViewCell::onTouchEquip));

	m_pNodeChapter->setVisible(true);
	m_pNodeMapInfo->setVisible(false);

    return true;
}

bool ChapterViewCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackground",	CCNode *,			m_pBackground);
//     
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLblPlan",		CCLabelBMFont *,	m_pLblPlan);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pArrow",		CCMenuItemImage *,	m_pArrow);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ChapterName",	CCLabelBMFont *,	m_ChapterName);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRewardItemBox",RewardItemBox *,   m_pRewardItemBox);
//     
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pThreeStar",CCNode *,   m_pThreeStar);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pThreeStarEat",CCNode *,   m_pThreeStarEat);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeChapter", CCNode *, m_pNodeChapter);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeMapInfo", CCNode *, m_pNodeMapInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFChapterName", CCLabelBMFont *, m_pBMFChapterName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFMapName", CCLabelBMFont *, m_pBMFMapName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnChapter", CCMenuItem *, m_pBtnChapter);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_image0",		CCNode *,			m_images[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_image1",		CCNode *,			m_images[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_image2",		CCNode *,			m_images[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_animation0",		CCNode *,			m_animations[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_animation1",		CCNode *,			m_animations[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_animation2",		CCNode *,			m_animations[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImageNode",		CCNode *,			m_pImageNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimationNode",		CCNode *,			m_pAnimationNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateTouch", UIAnimate *, m_pAnimateTouch);

	return false;
}

SEL_MenuHandler ChapterViewCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchArrow", ChapterViewCell::onTouchArrow);

	return NULL;
}

void ChapterViewCell::onTouchEquip(CCObject *obj)
{
//	m_pChapterView->onTouchEquip(m_pChapterInfo->getCid());
}

void ChapterViewCell::onTouchArrow(CCObject *obj)
{
	//m_pChapterView->touchChapterCell(m_pChapterInfo->getCid());
	m_pChapterView->prepareToExit(m_pChapterInfo->getCid());
}

void ChapterViewCell::updateView()
{
    if(!m_pChapterInfo) return;
     
	m_pBMFChapterName->setString(m_pChapterInfo->chapter_name.c_str());

	//uint32_t unlockConut = DM_GET_PVE_MODEL->getUnlockMapsCountByChapterId(m_pChapterInfo->getCid());	
	uint32_t totalCount = m_pChapterInfo->lastMid - m_pChapterInfo->firstMid + 1;	
	bool visible = (m_pChapterInfo->great_num >= totalCount);//bCurrentChapter;//(unlockConut == totalCount);

	m_images[0]->setVisible(false);
	m_images[1]->setVisible(!visible);
	m_images[2]->setVisible(visible);
	m_animations[0]->setVisible(false);
	m_animations[1]->setVisible(!visible);
	m_animations[2]->setVisible(visible);

	unsigned int unlocks = DM_GET_PVE_MODEL->getUnlockChartersCount();
	if(unlocks == m_pChapterInfo->getCid() && unlocks != DM_GET_PVE_MODEL->getChaptersCount()){
		//尚未全部解锁
		m_images[0]->setVisible(true);
		m_images[1]->setVisible(false);
		m_images[2]->setVisible(false);
		m_animations[0]->setVisible(true);
		m_animations[1]->setVisible(false);
		m_animations[2]->setVisible(false);
	}

	/*m_images[0]->setVisible(m_pChapterInfo->getProgress() == pve::eChapterNew);
	m_images[1]->setVisible(m_pChapterInfo->getProgress() == pve::eChapterCompete);
	m_images[2]->setVisible(m_pChapterInfo->getProgress() == pve::eChapterCleared);
	m_animations[0]->setVisible(m_pChapterInfo->getProgress() == pve::eChapterNew);
	m_animations[1]->setVisible(m_pChapterInfo->getProgress() == pve::eChapterCompete);
	m_animations[2]->setVisible(m_pChapterInfo->getProgress() == pve::eChapterCleared);*/

//     m_pBackground->removeAllChildren();
//     m_pBackground->addChild(ResourceManager::sharedResourceManager()->getSceneThumbIcon(m_pChapterInfo->getCid()));
//     
// 	m_ChapterName->setString(m_pChapterInfo->chapter_name.c_str());
//     
// 	m_pLblPlan->setString(CCString::createWithFormat("%d/%d",m_pChapterInfo->pres, m_pChapterInfo->getMaxPrestige())->getCString());
//     
//     m_pThreeStarEat->setVisible(m_pChapterInfo->canGetAllStarReward());
//     m_pThreeStar->setVisible(m_pChapterInfo->canGetAllStarReward() || m_pChapterInfo->grade_award);

    
//     pve::PrestigeReward* pPrestigeReward = m_pChapterInfo->getAwardToShow();
//     if (pPrestigeReward)
// 	{
// 		m_pRewardItemBox->setInfo(m_pChapterInfo, pPrestigeReward->prestige_reward_need_viplv);
// 	}
}

void ChapterViewCell::onEnter()
{
    CCTableViewCell::onEnter();
}

void ChapterViewCell::updateCell(pve::ChapterInfo* pChaterInfo)
{
	/*m_pImageNode->setVisible(!m_pBtnChapter->isEnabled());
	m_pAnimationNode->setVisible(m_pBtnChapter->isEnabled());*/

    if(m_pChapterInfo==pChaterInfo) return;

    SAFE_RELEASE_UNBINDING_ALL(m_pChapterInfo, this);
    CC_SAFE_RETAIN(pChaterInfo);
    m_pChapterInfo = pChaterInfo;
    BINDING_EXEC(m_pChapterInfo, this, ChapterViewCell::updateView, D_EVENT_PVE_CHAPTER_UPDATED);
}


void ChapterViewCell::setIsCenter(bool isCenter)
{
	m_pImageNode->setVisible(false);
	m_pAnimationNode->setVisible(isCenter);
	m_pBtnChapter->setEnabled(isCenter);
	if (isCenter)
	{
		m_pBtnChapter->selected();
	}
	else
	{
		m_pBtnChapter->unselected();
	}
}

void ChapterViewCell::showTouch(bool bShow)
{
	m_pAnimateTouch->setVisible(bShow);
	if (bShow)
	{
		m_pAnimateTouch->playAtIndex(0);
	}
	else
	{
		m_pAnimateTouch->stop();
	}
}