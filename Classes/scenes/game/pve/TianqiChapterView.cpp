//  霹雳天启 活动副本
//  ChapterView.cpp
//  QSMY
//

#include "TianqiChapterView.h"
#include "Pve.h"
#include "managers/ResourceManager.h"

#include "managers/DataManager.h"
#include "managers/GameManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PVE_ACTIVITY_CHAPTER_SCROLL_SIZE CCSizeMake(640, 670)
#define PVE_ACTIVITY_CHAPTER_CELL_SIZE CCSizeMake(574, 64)
#define PVE_ACTIVITY_BACK_OFFSET CCSizeMake(-120, 0)

#define PVE_ACTIVITY_NOACTION_DURATION	3.f
#define PVE_ACTIVITY_BACK_MIN_SCALE	0.6f
#define PVE_ACTIVITY_BACK_MAX_SCALE	1.5f
#define PVE_ACTIVITY_BACK_EXIT_SCALE	2.0f
//#define PVE_ACTION_INTERVAL	0.2f
//活动章节起步系数
#define PVE_ACTIVITY_CHAPTER_NUM 10000  

#define PVE_ACTIVITY_ARROW_POS_PLUS	ccp(0, -20)

TianqiChapterView::TianqiChapterView()
:m_pDelegate(NULL)
, m_pContainer(NULL)
, m_pLblPlan(NULL)
, m_pView(NULL)
, m_pPveModel(NULL)
, m_pCCData(NULL)
, m_nCid(0)
//, m_pScrBack(NULL)
//, m_pCCBBack(NULL)
//, m_pSprBack(NULL)
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
//, m_ptDeltaPos(0, 0)
//, m_fDeltaScale(0)
,m_bTouching(false)
//,m_pArmatureBuilding(NULL)
,m_pArrCell(NULL)
,m_fMaxPct(0.f)
,m_fMinPct(0.f)
,m_bWheeling(false)
,m_pArmatureCell(NULL)
,m_pNodeWheel(NULL)
,m_bFromActivityMapList(false)
,m_pArmatureEnv(NULL)
,m_RushTime(NULL)
,m_pCoinTip(NULL)
,m_pBtnElite(NULL)
,m_pTianqiBox(NULL)
,m_pTianqiBtn(NULL)
,m_pChapterNormal(NULL)
,m_pChapterActivity(NULL)
,m_pActivityAward(NULL)
,m_pBtnActivityAward(NULL)
,m_pActivityAwardAni(NULL)
{
	//D_RESET_C_ARRAY(m_pMapBuild, CHAPTER_MAP_MAX);
	//D_RESET_C_ARRAY(m_pMapName, CHAPTER_MAP_MAX);
	//D_RESET_C_ARRAY(m_pMapArrow, CHAPTER_MAP_MAX);
}

TianqiChapterView::~TianqiChapterView()
{
	unscheduleAllSelectors();
    removeAllChildren();
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pLblPlan);
	CC_SAFE_RELEASE(m_pCCData);
	//CC_SAFE_RELEASE(m_pScrBack);
	//CC_SAFE_RELEASE(m_pCCBBack);
	//CC_SAFE_RELEASE(m_pSprBack);
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
	//CC_SAFE_RELEASE(m_pArmatureBuilding);
	//D_SAFE_RELEASE_C_ARRAY(m_pMapBuild, CHAPTER_MAP_MAX);
	//D_SAFE_RELEASE_C_ARRAY(m_pMapName, CHAPTER_MAP_MAX);
	//D_SAFE_RELEASE_C_ARRAY(m_pMapArrow, CHAPTER_MAP_MAX);
	CC_SAFE_RELEASE(m_pArrCell);
	CC_SAFE_RELEASE(m_pArmatureCell);
	CC_SAFE_RELEASE(m_pNodeWheel);
	CC_SAFE_RELEASE(m_RushTime);
	CC_SAFE_RELEASE(m_pCoinTip);
	CC_SAFE_RELEASE(m_pBtnElite);
	CC_SAFE_RELEASE(m_pTianqiBox);
	CC_SAFE_RELEASE(m_pTianqiBtn);
	CC_SAFE_RELEASE(m_pChapterNormal);
	CC_SAFE_RELEASE(m_pChapterActivity);
	CC_SAFE_RELEASE(m_pActivityAward);
	CC_SAFE_RELEASE(m_pBtnActivityAward);
	CC_SAFE_RELEASE(m_pActivityAwardAni);
}

bool TianqiChapterView::init()
{
    if(!DLayer::init())
    {
        return false;
    }

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("RewardItemBox", RewardItemBoxLoader::loader());

	cocos2d::CCNode *__pNode__ = NULL;   
	cocos2d::extension::CCBReader * ___ccbReader__ = new DCCBReader(pNodeLoaderLibrary);
	__pNode__ = ___ccbReader__->readNodeGraphFromFile("PL_ui/ccb/pl_map/PL_map", this);
	___ccbReader__->release();
	if(__pNode__) addChild(__pNode__, 2);


	/*CCNode *pBack = NULL;
	CCB_READER("PL_ui/ccb/pl_map/PL_map_bg", pBack);
	m_pCCBBack = pBack;
	CC_SAFE_RETAIN(m_pCCBBack);
	m_pScrBack = CCScrollView::create(m_pBackContainer->getContentSize(), m_pCCBBack);
	m_pScrBack->setPosition(m_pBackContainer->getPosition());
	m_pScrBack->setTouchEnabled(false);
	addChild(m_pScrBack, 1);
	CC_SAFE_RETAIN(m_pScrBack);*/

	m_pCCData = readCCData("PL_ui/ccb/pl_map/PL_map_wheel_cell");
	CC_SAFE_RETAIN(m_pCCData);
	    
    m_pPveModel = DM_GET_PVE_MODEL;
    
	m_pArrCell = CCDictionary::create();
	CC_SAFE_RETAIN(m_pArrCell);

    m_pView = PLWheelView::create(this, PVE_ACTIVITY_CHAPTER_SCROLL_SIZE);
	m_pView->cocos2d::extension::CCTableView::setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pView->setWheelDelegate(this);
    m_pView->setDelegate(this);
    m_pContainer->addChild(m_pView);

	m_pNodeAllStar->setVisible(false);

	m_pBoxPlan->getTouchEvent()->setTarget(this, menu_selector(TianqiChapterView::onTouchEquip));

	m_pNodeBack->setVisible(false);

	m_pNodeDrop->setVisible(false);

	m_pBtnRush->setEnabled(false);
	m_RushTime->setVisible(false);

	m_pNodeDraggingDown->setVisible(false);
	m_pNodeDraggingUp->setVisible(false);
	m_pChapterActivity->setVisible(true);
	m_pChapterNormal->setVisible(false);

	//m_pBtnPve->setEnabled(true);
	//m_pBtnPve->selected();
	m_pBtnWonder->setTarget(this, menu_selector(TianqiChapterView::callBackWonder));
	m_pBtnPve->setTarget(this, menu_selector(TianqiChapterView::callBackChapter));
	//精英赛
	m_pBtnElite->setEnabled(false);
	//m_pBtnElite->selected();
	//m_pBtnElite->setTarget(this,menu_selector(TianqiChapterView::touchElite));

	m_pNodeFame->setVisible(false);

	/*for (int i = 0; i < CHAPTER_MAP_MAX; i++)
	{
		m_pMapBuild[i]->setVisible(false);
		m_pMapName[i]->setVisible(false);
		m_pMapArrow[i]->setVisible(false);
	}*/
	//m_pScrBack->setContentOffset(PVE_ACTIVITY_BACK_OFFSET);

	//m_pArmatureBuilding = ResourceManager::sharedResourceManager()->getUIAnimate("pve_building_center");
	//CC_SAFE_RETAIN(m_pArmatureBuilding);
	m_pArmatureCell = ResourceManager::sharedResourceManager()->getUIAnimate("pve_cell_ani");
	CC_SAFE_RETAIN(m_pArmatureCell);

	m_pCoinTip->setVisible(false);
	m_pTianqiBox->setVisible(false);
	setActivityAwardBtn();
	m_pBtnActivityAward->setTarget(this, menu_selector(TianqiChapterView::getActivityAward));
    return true;
}

bool TianqiChapterView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
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

	//D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pMapBuild", CCNode *, m_pMapBuild, CHAPTER_MAP_MAX);
	//D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pMapName", CCNode *, m_pMapName, CHAPTER_MAP_MAX);
	//D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pNodeArrow", CCNode *, m_pMapArrow, CHAPTER_MAP_MAX);

	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSprBack", CCSprite *, m_pSprBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeWheel", CCNode *, m_pNodeWheel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCoinTip" , CCNode*, m_pCoinTip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnElite" , CCMenuItem*, m_pBtnElite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTianqiBox" , CCNode*, m_pTianqiBox);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTianqiBtn" , CCMenuItemImage*, m_pTianqiBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChapterNormal" , CCNode*,m_pChapterNormal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChapterActivity" , CCNode*,m_pChapterActivity);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pActivityAward", CCNode*,m_pActivityAward);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnActivityAward", CCMenuItemImage*,m_pBtnActivityAward);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pActivityAwardAni", UIAnimate*,m_pActivityAwardAni);
	return false;
}

SEL_MenuHandler TianqiChapterView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	return NULL;
}

void TianqiChapterView::onEnter()
{
    DLayer::onEnter();
	prepareToEnter();
    BINDING_EXEC(m_pPveModel, this, TianqiChapterView::onChaptersUpdated, D_EVENT_PVE_CHAPTERS_UPDATED);
}

void TianqiChapterView::onExit()
{
    UNBINDING_ALL(m_pPveModel, this);

    DLayer::onExit();
}

void TianqiChapterView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	
	return;
}

CCSize TianqiChapterView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return PVE_ACTIVITY_CHAPTER_CELL_SIZE;
}

CCTableViewCell* TianqiChapterView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
    TianqiChapterViewCell *cell = (TianqiChapterViewCell*)m_pArrCell->objectForKey(idx);
    if(!cell)
    {
        cell = TianqiChapterViewCell::createChapterViewCell(this, m_pCCData , idx+1 == m_pPveModel->getUnlockActivityChartersCount());
		m_pArrCell->setObject(cell, idx);
    } 
    
    pve::ChapterInfo* pChapterInfo = m_pPveModel->getActivityChapterInfoByIndex(m_pPveModel->getUnlockActivityChartersCount()-idx-1);
	cell->updateCell(pChapterInfo);
	cell->setIsCenter(m_uCenterIdx == idx);
	cell->showTouch(m_uCenterIdx == idx && !m_bTouching);
	if (m_uCenterIdx == idx)
	{
		addArmatureToCell(m_pArmatureCell, cell);
	}
    return cell;
}

uint32_t TianqiChapterView::numberOfCellsInTableView(CCTableView *table)
{
    return m_pPveModel->getUnlockActivityChartersCount();
}

void TianqiChapterView::updateView()
{
    m_pView->reloadData();
    rotateTo(m_nCid);
}

void TianqiChapterView::onChaptersUpdated()
{
    if(!m_pPveModel->isSynced)
	{
		return;
	}

    updateView();
}

void TianqiChapterView::onTouchEquip(CCObject *obj)
{
	CCMenuItem *pItem = static_cast<CCMenuItem *>(obj);
	if (m_pDelegate)
	{
		m_pDelegate->showTouchItem(pItem->getTag());
	}
}

void TianqiChapterView::touchChapterCell(const uint32_t &chapterID)
{
    if(m_pDelegate)
    {
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

void TianqiChapterView::touchElite(CCObject *obj)
{
	if(m_pDelegate)
	{
		m_pDelegate->showEliteView();
	}
}

void TianqiChapterView::setDelegate(PveDelegate *pDelegate)
{
    m_pDelegate = pDelegate;
}

void TianqiChapterView::rotateTo(const int &cid)
{
    m_nCid = cid;
	CCPoint offset = ccp(0,-(cid-1)*PVE_ACTIVITY_CHAPTER_CELL_SIZE.height);
    offset.y = MIN(m_pView->maxContainerOffset().y, offset.y);
    offset.y = MAX(m_pView->minContainerOffset().y, offset.y);
    
	m_pView->setContentOffset(offset);
	//hideOtherAndName();
	TianqiChapterViewCell *pCell = (TianqiChapterViewCell *)m_pView->cellAtIndex(cid - 1);
	if (pCell)
	{
		//pCell->showTouch(true);
	}
}

void TianqiChapterView::PLWheelViewTouched(PLWheelView *pView)
{
	unscheduleAllSelectors();
	m_bTouching = true;
	m_fMaxPct = 0.f;
	m_fMinPct = 0.f;
	m_pNodeRotation->stopAllActions();
	m_bWheeling = false;
	TianqiChapterViewCell *pCell = (TianqiChapterViewCell *)pView->cellAtIndex(m_uCenterIdx);
	pCell->showTouch(false);
}

void TianqiChapterView::PLWheelViewCenterIdxChanged(PLWheelView *pView, unsigned int centerIdx)
{
	TianqiChapterViewCell *pCell = (TianqiChapterViewCell *)m_pView->cellAtIndex(m_uCenterIdx);
	if (pCell)
	{
		pCell->setIsCenter(false);
	}
	m_uCenterIdx = centerIdx;
	pCell = (TianqiChapterViewCell *)m_pView->cellAtIndex(m_uCenterIdx);
	if (pCell)
	{
		pCell->setIsCenter(true);
		addArmatureToCell(m_pArmatureCell, pCell);
	}
	m_bNeedUpdatePos = true;
}

void TianqiChapterView::updateChapter(pve::ChapterInfo *pInfo)
{
	m_pLblPlan->setString(CCString::createWithFormat("%d/%d",pInfo->pres,pInfo->getMaxPrestige())->getCString());
	pve::PrestigeReward* pPrestigeReward = pInfo->getAwardToShow();
	m_pBoxPlan->setInfo(pInfo, pPrestigeReward->prestige_reward_need_viplv);
}

void TianqiChapterView::PLWheelViewStartWheeling(PLWheelView *pView, unsigned int centerIdx)
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
	m_nCid = m_pPveModel->getUnlockActivityChartersCount()-centerIdx;
	//updateMapPos(PVE_ACTION_INTERVAL);
}

void TianqiChapterView::PLWheelViewStopWheeling(PLWheelView *pView, unsigned int centerIdx)
{
	m_bWheeling = false;
	//scheduleOnce(schedule_selector(TianqiChapterView::scaleToMin), PVE_ACTIVITY_NOACTION_DURATION);
	m_bTouching = false;
	TianqiChapterViewCell *pCell = (TianqiChapterViewCell *)pView->cellAtIndex(m_uCenterIdx);
	pCell->showTouch(true);

	if (!m_bNeedUpdatePos)
	{
		return;
	}

	m_bNeedUpdatePos = false;

	m_nCid = m_pPveModel->getUnlockActivityChartersCount()-centerIdx;
	//schedule(schedule_selector(TianqiChapterView::updateScale));
	//showAllWithName();
}

//void TianqiChapterView::updateScale(float dt)
//{
//	if (m_pCCBBack->getScale() >= PVE_ACTIVITY_BACK_MAX_SCALE)
//	{
//		unschedule(schedule_selector(TianqiChapterView::updateScale));
//		
//		return;
//	}
//	m_pCCBBack->setScale(m_pCCBBack->getScale() + 0.05);
//	updateMapPos(0.f);	
//}

//void TianqiChapterView::scaleToMin(float dt)
//{
//	unscheduleAllSelectors();
//
//	m_ptDeltaPos = ccpSub(PVE_ACTIVITY_BACK_OFFSET, m_pScrBack->getContentOffset());
//	m_fDeltaScale = PVE_ACTIVITY_BACK_MIN_SCALE - m_pCCBBack->getScale();
//	schedule(schedule_selector(TianqiChapterView::updateScaleMin));
//}

//void TianqiChapterView::updateScaleMin(float dt)
//{
//	if (m_pCCBBack->getScale() <= PVE_ACTIVITY_BACK_MIN_SCALE)
//	{
//		m_pCCBBack->setScale(PVE_ACTIVITY_BACK_MIN_SCALE);
//		m_pScrBack->setContentOffset(PVE_ACTIVITY_BACK_OFFSET);
//		unschedule(schedule_selector(TianqiChapterView::updateScaleMin));
//		hideOtherAndName();
//		return;
//	}
//	float dtScale = m_fDeltaScale * dt / PVE_ACTION_INTERVAL;
//	CCPoint dtPos = ccpMult(m_ptDeltaPos, dt / PVE_ACTION_INTERVAL);
//	m_pCCBBack->setScale(m_pCCBBack->getScale() + dtScale);
//	m_pScrBack->setContentOffset(ccpAdd(m_pScrBack->getContentOffset(), dtPos));
//	
//}

//void TianqiChapterView::updateMapPos(float dur)
//{
//	pve::ChapterInfo* pInfo = m_pPveModel->getActivityChapterInfoByIndex(m_nCid - 1);
//	pve::ChapterExtraInfo *pExtra = m_pPveModel->getChapterExtraInfo(pInfo->getCid());
//	CCPoint backOffset = ccpSub(ccp(320, 640), ccpMult(pExtra->pt, 0.5 * m_pCCBBack->getScale()));
//	if (backOffset.x > m_pScrBack->maxContainerOffset().x)
//	{
//		backOffset.x = m_pScrBack->maxContainerOffset().x;
//	}
//	else if (backOffset.x < m_pScrBack->minContainerOffset().x)
//	{
//		backOffset.x = m_pScrBack->minContainerOffset().x;
//	}
//
//	if (backOffset.y > m_pScrBack->maxContainerOffset().y)
//	{
//		backOffset.y = m_pScrBack->maxContainerOffset().y;
//	}
//	else if (backOffset.y < m_pScrBack->minContainerOffset().y)
//	{
//		backOffset.y = m_pScrBack->minContainerOffset().y;
//	}
//	if (D_FLOAT_EQUALS(dur, 0.f))
//	{
//		m_pScrBack->setContentOffset(backOffset);
//	}
//	else
//	{
//		m_pScrBack->setContentOffsetInDuration(backOffset, dur);
//	}
//}

//void TianqiChapterView::showAllWithName()
//{
//	for (int i = 0; i < CHAPTER_MAP_MAX; i++)
//	{
//		pve::ChapterInfo *pChapter = m_pPveModel->getChapterInfo(i + 1);
//		pve::ChapterExtraInfo *pExtra = m_pPveModel->getChapterExtraInfo(m_nCid);
//		m_pMapBuild[i]->setVisible(pChapter->chapter_unlock);
//		if (i + 1 == pExtra->section)
//		{
//			m_pArmatureBuilding->removeFromParent();
//			m_pArmatureBuilding->setPosition(ccpAdd(m_pMapArrow[i]->getPosition(), PVE_ACTIVITY_ARROW_POS_PLUS));
//			m_pMapArrow[i]->getParent()->addChild(m_pArmatureBuilding);
//		}
//		
//		m_pMapName[i]->setVisible(i + 1 == pExtra->section);
//		
//	}
//	if (m_nCid > 0)
//	{
//		if (NULL != m_pArmatureEnv)
//		{
//			m_pArmatureEnv->removeFromParent();
//		}
//		pve::ChapterExtraInfo *pExtra = m_pPveModel->getChapterExtraInfo(m_nCid);
//		m_pArmatureEnv = ResourceManager::sharedResourceManager()->getUIAnimate(CCString::createWithFormat("chapter_ani_%d", pExtra->ani)->getCString());
//		m_pArmatureEnv->setScale(0.8f);
//		if (!m_pArmatureEnv)
//		{
//			return;
//		}
//		
//		m_pArmatureEnv->setPosition(m_pMapBuild[pExtra->section-1]->getPosition());
//		m_pMapBuild[pExtra->section-1]->getParent()->addChild(m_pArmatureEnv);
//	}
//}

//void TianqiChapterView::hideOtherAndName()
//{
//	for (int i = 0; i < CHAPTER_MAP_MAX; i++)
//	{
//		pve::ChapterExtraInfo *pExtra = m_pPveModel->getChapterExtraInfo(m_nCid);
//		m_pMapBuild[i]->setVisible(i + 1 == pExtra->section);
//		if (i + 1 == pExtra->section)
//		{
//			m_pArmatureBuilding->removeFromParent();
//			m_pArmatureBuilding->setPosition(ccpAdd(m_pMapArrow[i]->getPosition(), PVE_ACTIVITY_ARROW_POS_PLUS));
//			m_pMapArrow[i]->getParent()->addChild(m_pArmatureBuilding);
//		}
//		m_pMapName[i]->setVisible(false);
//	}
//}

void TianqiChapterView::PLWheelViewBouncePercent(PLWheelView *pView, float pct)
{
	if (!m_bWheeling)
	{
		m_pNodeRotation->setRotation(pct * MAX_ROTATION);
	}
	if (pct <= 0.1 && pct >= -0.1)
	{
		m_pNodeDraggingDown->setVisible(true);
		m_pNodeDraggingUp->setVisible(true);
	}
	else
	{
		if (pct > 0.1)
		{
			m_pNodeDraggingUp->setVisible(false);
			m_pNodeDraggingDown->setVisible(true);
		}
		else
		{
			m_pNodeDraggingDown->setVisible(false);
			m_pNodeDraggingUp->setVisible(true);
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

void TianqiChapterView::prepareToEnter()
{
	m_pContainer->setPositionX(m_pContainer->getPositionX() + PVE_ACTION_MOVE_DIS);
	if (!m_bFromActivityMapList)
	{
		m_pNodeWheel->setPositionX(m_pNodeWheel->getPositionX() + PVE_ACTION_MOVE_DIS);
	}
	
	//背景
	m_pNodeBg->removeAllChildrenWithCleanup(true);
	CCString *res = CCString::createWithFormat("PL_ui/huodongfuben/pveActivity%d.png",m_pPveModel->getActivityID());
	CCSprite *pScene = CCSprite::create(res->getCString());
	m_pNodeBg->addChild(pScene);
	m_pNodeBg->setScale(PVE_ACTIVITY_BACK_EXIT_SCALE);
	m_pNodeBg->stopAllActions();
	m_pNodeBg->runAction(CCScaleTo::create(PVE_ACTION_MOVE_DUR_ENTER, 1));
	
	//CCSprite *pScene = ResourceManager::sharedResourceManager()->getChapterIcon(m_pPveModel->getChapterExtraInfo(m_nCid)->res);
	//pScene->setPosition(m_pNodeBg->getPosition());
	//this->addChild(pScene);

	/*m_pCCBBack->setScale(PVE_ACTIVITY_BACK_EXIT_SCALE);
	m_pCCBBack->stopAllActions();
	m_pCCBBack->runAction(CCScaleTo::create(PVE_ACTION_MOVE_DUR_ENTER, PVE_ACTIVITY_BACK_MIN_SCALE));
	m_pSprBack->setOpacity(0);
	m_pSprBack->stopAllActions();
	m_pSprBack->runAction(CCSequence::createWithTwoActions(CCFadeTo::create(PVE_ACTION_MOVE_DUR_ENTER, 255),
		CCCallFunc::create(this, callfunc_selector(TianqiChapterView::enterActionFinished))));*/
	enterActionFinished();
}

void TianqiChapterView::enterActionFinished()
{
	m_pContainer->stopAllActions();
	m_pContainer->runAction(CCMoveBy::create(PVE_ACTION_MOVE_DUR_EXIT, ccp(-PVE_ACTION_MOVE_DIS, 0)));
	if (!m_bFromActivityMapList)
	{
		m_pNodeWheel->stopAllActions();
		m_pNodeWheel->runAction(CCMoveBy::create(PVE_ACTION_MOVE_DUR_EXIT / 2, ccp(-PVE_ACTION_MOVE_DIS, 0)));
	}
	
	rotateTo(m_nCid);
}

void TianqiChapterView::prepareToExit(unsigned int cid)
{
	m_pContainer->stopAllActions();
	m_pContainer->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(PVE_ACTION_MOVE_DUR_EXIT, ccp(PVE_ACTION_MOVE_DIS, 0)), 
		CCCallFuncO::create(this, callfuncO_selector(TianqiChapterView::actionFinished), CCInteger::create(cid))));

	/*CCSprite *pIcon = ResourceManager::sharedResourceManager()->getChapterIcon(m_pPveModel->getChapterExtraInfo(m_nCid)->res);
	pIcon->setPosition(m_pNodeBg->getPosition());
	if (m_pPveModel->getChapterInfo(m_nCid)->isSynced)
	{
		cocos2d::CCArray maps;
		m_pPveModel->getUnlockedMapsByChapterId(m_nCid,maps);
		pve::MapInfo *pMapInfo = (pve::MapInfo *)maps.lastObject();
		pIcon->setScale(pMapInfo->scale);
		pIcon->setPosition(ccpSub(m_pNodeBg->getPosition(), ccp(pMapInfo->posX, pMapInfo->posY)));
	}
	
	this->addChild(pIcon);*/

	//m_pCCBBack->stopAllActions();
	//m_pCCBBack->runAction(CCScaleTo::create(PVE_ACTION_MOVE_DUR_EXIT, PVE_ACTIVITY_BACK_EXIT_SCALE));
	//m_pSprBack->stopAllActions();
	//m_pSprBack->runAction(CCFadeTo::create(PVE_ACTION_MOVE_DUR_EXIT, 0));
	/*for (int i = 0; i < CHAPTER_MAP_MAX; i++)
	{
		m_pMapBuild[i]->stopAllActions();
		if (m_pMapBuild[i]->isVisible())
		{
			m_pMapBuild[i]->runAction(CCFadeTo::create(PVE_ACTION_MOVE_DUR_EXIT, 0));
			m_pArmatureBuilding->runAction(CCFadeTo::create(PVE_ACTION_MOVE_DUR_EXIT, 0));
		}
		
	}*/
	
}

void TianqiChapterView::actionFinished(CCObject *pObj)
{
	CCInteger *pCid = static_cast<CCInteger *>(pObj);
	touchChapterCell(pCid->getValue());
}

void TianqiChapterView::callBackWonder(CCObject *pObj)
{
	Game::sharedGame()->NavigateTo(navigation::kNavWonder);
}

void TianqiChapterView::callBackChapter(CCObject *pObj)
{
	if(m_pDelegate)
	{
		m_pDelegate->showChapterView();
	}
};

void TianqiChapterView::addArmatureToCell(sp::Armature *pArmature, CCTableViewCell *pCell)
{
	if (!pArmature || !pCell)
	{
		return;
	}
	pArmature->removeFromParent();
	pArmature->setPosition(ccp(290, 65));
	pCell->addChild(pArmature);
}

void TianqiChapterView::getActivityAward(CCObject *pObj)
{
	//获取活动通关奖励
	HTTP_CLIENT->getPveActivityAward(this,callfuncJson_selector(TianqiChapterView::activityAwardCallBak));
}

void TianqiChapterView::activityAwardCallBak(const Json::Value & requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	m_pPveModel->setActivityAwardState(1);
	setActivityAwardBtn();
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asInt()));
}

void TianqiChapterView::setActivityAwardBtn()
{
	bool isActivityAward = static_cast<bool>(m_pPveModel->getActivityAwardState());
	m_pActivityAward->setVisible(!isActivityAward);
	//领奖提示特效
	if(m_pPveModel->isCanGetActivityAward())
	{
		m_pActivityAwardAni->setVisible(true);
		m_pActivityAwardAni->playAtIndex(0);
	}
	else
	{
		m_pActivityAwardAni->setVisible(false);
		m_pActivityAwardAni->stop();
	}
}

TianqiChapterViewCell::TianqiChapterViewCell()
:m_pChapterInfo(NULL)
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
TianqiChapterViewCell::~TianqiChapterViewCell()
{
    removeAllChildren();
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

TianqiChapterViewCell * TianqiChapterViewCell::createChapterViewCell(TianqiChapterView *chapterView, cocos2d::extension::CCData* pData , bool bCurrentChapter)
{
    TianqiChapterViewCell *pChapterViewCell = new TianqiChapterViewCell();
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

bool TianqiChapterViewCell::init(TianqiChapterView *chapterView, cocos2d::extension::CCData* pData)
{
    m_pChapterView = chapterView;

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("RewardItemBox", RewardItemBoxLoader::loader());

	CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pNodeLoaderLibrary);

	m_pNodeChapter->setVisible(true);
	m_pNodeMapInfo->setVisible(false);

    return true;
}

bool TianqiChapterViewCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
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

SEL_MenuHandler TianqiChapterViewCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchArrow", TianqiChapterViewCell::onTouchArrow);

	return NULL;
}

void TianqiChapterViewCell::onTouchEquip(CCObject *obj)
{
//	m_pChapterView->onTouchEquip(m_pChapterInfo->getCid());
}

void TianqiChapterViewCell::onTouchArrow(CCObject *obj)
{
	m_pChapterView->prepareToExit(m_pChapterInfo->getCid());
}

void TianqiChapterViewCell::updateView()
{
    if(!m_pChapterInfo) return;
     
	m_pBMFChapterName->setString(m_pChapterInfo->chapter_name.c_str());

	uint32_t totalCount = m_pChapterInfo->lastMid - m_pChapterInfo->firstMid + 1;	
	bool visible = (m_pChapterInfo->great_num >= totalCount);//bCurrentChapter;//(unlockConut == totalCount);

	m_images[0]->setVisible(false);
	m_images[1]->setVisible(!visible);
	m_images[2]->setVisible(visible);
	m_animations[0]->setVisible(false);
	m_animations[1]->setVisible(!visible);
	m_animations[2]->setVisible(visible);

	unsigned int unlocks = DM_GET_PVE_MODEL->getUnlockActivityChartersCount();
	int chapterNum = (m_pChapterInfo->getCid()%PVE_ACTIVITY_CHAPTER_NUM);
	if(unlocks == chapterNum && unlocks != DM_GET_PVE_MODEL->getActivityChaptersCount()){
		//尚未全部解锁
		m_images[0]->setVisible(true);
		m_images[1]->setVisible(false);
		m_images[2]->setVisible(false);
		m_animations[0]->setVisible(true);
		m_animations[1]->setVisible(false);
		m_animations[2]->setVisible(false);
	}
}

void TianqiChapterViewCell::onEnter()
{
    CCTableViewCell::onEnter();
}

void TianqiChapterViewCell::updateCell(pve::ChapterInfo* pChaterInfo)
{
    if(m_pChapterInfo==pChaterInfo) return;

    SAFE_RELEASE_UNBINDING_ALL(m_pChapterInfo, this);
    CC_SAFE_RETAIN(pChaterInfo);
    m_pChapterInfo = pChaterInfo;
    BINDING_EXEC(m_pChapterInfo, this, TianqiChapterViewCell::updateView, D_EVENT_PVE_CHAPTER_UPDATED);
}


void TianqiChapterViewCell::setIsCenter(bool isCenter)
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

void TianqiChapterViewCell::showTouch(bool bShow)
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