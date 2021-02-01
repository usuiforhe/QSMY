//副本精英
#include "PveElite.h"
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

#define PVE_ELITE_SCROLL_SIZE CCSizeMake(640, 670)
#define PVE_ELITE_CELL_MIN_SIZE	CCSizeMake(574 ,64)
#define PVE_ELITE_CELL_MAX_SIZE	CCSizeMake(600 ,255)
#define PVE_ELITE_CELL_REW_SIZE   CCSizeMake(600, 100)

#define EXT_CELL_COUNT 1

PveElite::PveElite()
:m_pDelegate(NULL)
,m_pView(NULL)
,m_pPveModel(NULL)
,m_pUserModel(NULL)
,m_pNodeChild(NULL)
,m_pLblPlan(NULL)
,m_pButtonAnimationNode(NULL)
//,m_pRewardItemBox(NULL)
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
,m_pBtnRush(NULL)
,m_pBtnElite(NULL)
,m_pCoinTip(NULL)
,m_pNodeQiyu(NULL)
,m_pEliteCoin(NULL)
,m_pEliteGold(NULL)
,m_pEliteFirst(NULL)
,m_pArrCell(NULL)
{

}

PveElite::~PveElite()
{
	CC_SAFE_RELEASE(m_pNodeChild);
	CC_SAFE_RELEASE(m_pLblPlan);
	CC_SAFE_RELEASE(m_pButtonAnimationNode);
	//CC_SAFE_RELEASE(m_pRewardItemBox);
	CC_SAFE_RELEASE(m_pMapCellData);
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
	CC_SAFE_RELEASE(m_pBtnRush);
	CC_SAFE_RELEASE(m_pBtnElite);
	CC_SAFE_RELEASE(m_pCoinTip);
	CC_SAFE_RELEASE(m_pNodeQiyu);
	CC_SAFE_RELEASE(m_pEliteCoin);
	CC_SAFE_RELEASE(m_pEliteGold);
	CC_SAFE_RELEASE(m_pEliteFirst);
	CC_SAFE_RELEASE(m_pArrCell);
	UNBINDING_ALL(m_pUserModel,this);
}

bool PveElite::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pPveModel = DM_GET_PVE_MODEL;
	m_pUserModel = DM_GET_USER_MODEL;
	m_dropItems = CCArray::create();
	m_pArrCell = CCDictionary::create();
	CC_SAFE_RETAIN(m_pArrCell);
	CC_SAFE_RETAIN(m_dropItems);
	CC_SAFE_RETAIN(m_pUserModel);

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("RewardItemBox", RewardItemBoxLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_map/PL_map",pNodeLoaderLibrary);

	m_pMapCellData = readCCData("PL_ui/ccb/pl_map/PL_map_wheel_cell");
	CC_SAFE_RETAIN(m_pMapCellData);

	m_pView = PLWheelView::create(this, PVE_ELITE_SCROLL_SIZE);
	m_pView->setDelegate(this);
	m_pView->setWheelDelegate(this);
	m_pNodeChild->addChild(m_pView);

	m_pLblPlan->setString("0");

	//m_pRewardItemBox->getTouchEvent()->setTarget(this, menu_selector(PveElite::onTouchEquip));

	m_pBtnElite->setEnabled(false);
	m_pBtnElite->selected();
	m_pBtnWonder->setTarget(this, menu_selector(PveElite::callBackWonder));
	m_pBtnPve->setTarget(this, menu_selector(PveElite::callBackPve));

	m_ptOrigin = m_pNodeBg->getPosition();
	m_pArmatureCell = ResourceManager::sharedResourceManager()->getUIAnimate("pve_cell_ani");
	CC_SAFE_RETAIN(m_pArmatureCell);
	m_RushTime->setVisible(false);
	m_pBtnRush->setVisible(false);
	m_pNodeAllStar->setVisible(false);
	m_pNodeFame->setVisible(false);
	m_pNodeBack->setVisible(false);
	m_pNodeQiyu->setVisible(false);
	m_pCoinTip->setVisible(true);
	return true;
}

bool PveElite::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeContainer",	CCNode *,			m_pNodeChild);
	// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ChapterName",	CCLabelBMFont *,	m_ChapterName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLblPlan",		CCLabelBMFont *,	m_pLblPlan);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonAnimationNode",	CCNode *,			m_pButtonAnimationNode);

	// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pArrow",		CCMenuItemImage *,	m_pArrow);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxPlan",		RewardItemBox *,         m_pRewardItemBox);

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
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRush" , CCNode*, m_pBtnRush);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnElite" , CCMenuItem*, m_pBtnElite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCoinTip" , CCNode*,m_pCoinTip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeQiyu" , CCNode*, m_pNodeQiyu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEliteCoin" , CCLabelBMFont*,m_pEliteCoin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEliteGold" , CCLabelBMFont*,m_pEliteGold);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEliteFirst" , CCNode*, m_pEliteFirst);
	return false;
}

SEL_MenuHandler PveElite::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchArrow", PveElite::onTouchArrow);
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackRush", PveElite::callBackRush);
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackAllStar", PveElite::callBackAllStar);

	return NULL;
}

void PveElite::onTouchCell(cocos2d::extension::CCTableViewCell* cell)
{

}
void PveElite::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	CCLOG("map cell touched at index: %i", cell->getIdx());
}

CCSize PveElite::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return PVE_ELITE_CELL_MIN_SIZE;
}

CCTableViewCell* PveElite::tableCellAtIndex(CCTableView *table, uint32_t idx)
{

	//PveEliteCell * cell = dynamic_cast<PveEliteCell*>(table->dequeueCell());
	PveEliteCell * cell = dynamic_cast<PveEliteCell*>(m_pArrCell->objectForKey(idx));
	if (cell==NULL)
	{
		cell = PveEliteCell::create(m_pMapCellData, m_pDelegate, this);
		m_pArrCell->setObject(cell, idx);
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

uint32_t PveElite::numberOfCellsInTableView(CCTableView *table)
{
	return maps.count();
}

void PveElite::onTouchArrow(CCObject *obj)
{
}

void PveElite::onTouchEquip(cocos2d::CCObject *obj)
{
	if(m_pDelegate)
	{
		//m_pDelegate->showTouchItem(m_pChapterInfo->getCid());
	}
}

void PveElite::callBackRush(CCObject *obj)
{
}

void PveElite::callBackAllStar(cocos2d::CCObject *obj)
{
	if (m_pDelegate)
	{
		//m_pDelegate->getGradeAward(m_pChapterInfo->getCid());
	}

}

void PveElite::onEnter()
{
    DLayer::onEnter();

	BINDING_EXEC(m_pPveModel, this, PveElite::onMapsUpdated, D_EVENT_PVE_ELITE_UPDATED);
	this->prepareToEnter();
	
}

void PveElite::onExit()
{
	UNBINDING_ALL(m_pPveModel, this);

    m_nSelectedIndex = 0;
    maps.removeAllObjects();
    DLayer::onExit();
}

void PveElite::onRushUpdated()
{

}

void PveElite::update(float dt)
{

}

void PveElite::onMapsUpdated()
{
    updateView();
    
	if(m_pPveModel->getLaseEliteID()>0)
	{
		
		rotateTo(m_pPveModel->getLaseEliteID());
		m_pView->reloadData();
	}
	else
	{
		m_pView->setContentOffset(m_pView->minContainerOffset(), false);
	}
}

void PveElite::updateView()
{

	m_pPveModel->getUnlockedElite(maps);
    maps.reverseObjects();
    
    
    pve::MapInfo * pMapInfo = NULL;
    if(maps.count()>0) 
	{
		pMapInfo = static_cast<pve::MapInfo *>(maps.objectAtIndex(0));
	}
	else
	{
		return;
	}

    if(!pMapInfo || pMapInfo->iTimes==0)
    {
        m_nSelectedIndex = 0;
    }
    
    m_pView->reloadData();

	pMapInfo = (pve::MapInfo *)maps.objectAtIndex(m_uCenterIdx);
	if (pMapInfo)
	{
		updateDropInfo(pMapInfo);
	}

	/*m_pNodeBg->removeAllChildrenWithCleanup(true);
	m_pSprBg = ResourceManager::sharedResourceManager()->getChapterIcon(m_pPveModel->getChapterExtraInfo(pMapInfo->chapterInfo->getCid())->res);
	m_pNodeBg->addChild(m_pSprBg, 1, 1);

	m_pNodeBg->setPosition(ccpSub(m_ptOrigin, ccp(pMapInfo->posX, pMapInfo->posY)));
	m_pNodeBg->setScale(pMapInfo->scale);*/
}

void PveElite::prepareToEnter()
{
	/*m_pNodeChild->setPositionX(m_pNodeChild->getPositionX() + PVE_ACTION_MOVE_DIS);
	m_pNodeChild->runAction(CCMoveBy::create(PVE_ACTION_MOVE_DUR_ENTER, ccp(-PVE_ACTION_MOVE_DIS, 0)));

	pve::MapInfo *pMapInfo = DM_GET_PVE_MODEL->getMapInfo(m_pPveModel->getLastShowMapID());
	CCNode *pNode = ResourceManager::sharedResourceManager()->getSceneIcon(pMapInfo->chapter_id);
	pNode->setPosition(ccpAdd(ccp(0, PVE_BATTLE_BACK_Y_POS), ccp(pMapInfo->posX, pMapInfo->posY)));
	m_pNodeBg->addChild(pNode);
	m_pSprBg->stopAllActions();
	m_pSprBg->setScale(1.5);
	m_pSprBg->setOpacity(0.f);
	m_pSprBg->runAction(CCScaleTo::create(PVE_ACTION_MOVE_DUR_ENTER, 1.0));
	m_pSprBg->runAction(CCFadeIn::create(PVE_ACTION_MOVE_DUR_ENTER));*/

	m_pNodeWheel->setPositionX(m_pNodeWheel->getPositionX() + PVE_ACTION_MOVE_DIS);
	m_pNodeWheel->runAction(CCMoveBy::create(PVE_ACTION_MOVE_DUR_ENTER, ccp(-PVE_ACTION_MOVE_DIS, 0)));
	
}

void PveElite::updateCells(float dt)
{
	CCPoint pt = ccpAdd(m_pView->getContentOffset(), ccp(0, 5));
	if (pt.y > m_pView->minContainerOffset().y)
	{
		unschedule(schedule_selector(PveElite::updateCells));
	}
	
	m_pView->setContentOffset(pt);
}

void PveElite::prepareToExit()
{
	m_pNodeChild->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(PVE_ACTION_MOVE_DUR_EXIT, ccp(PVE_ACTION_MOVE_DIS, 0)), 
		CCCallFuncO::create(this, callfuncO_selector(PveElite::actionFinished), CCInteger::create(0))));

	//进度       
	//m_pNodeJindu->stopAllActions();
	//m_pNodeJindu->       runAction(CCMoveTo::create(0.2f,ccp(-300,0)));
	//m_pButtonImageNode->stopAllActions();
	//m_pButtonImageNode-> runAction(CCMoveTo::create(0.2f,ccp(-300,0)));
	//声望
	//m_pNodeFame->stopAllActions();
	//m_pNodeFame->        runAction(CCMoveTo::create(0.2f,ccp(0,300)));
	//返回
	//m_pNodeBack->stopAllActions();
	//m_pNodeBack->        runAction(CCMoveTo::create(0.2f,ccp(300,0)));
	//掉落
	m_pNodeDrop->stopAllActions();
	m_pNodeDrop->        runAction(CCMoveTo::create(0.2f,ccp(0,-1500)));
	//剧情按钮
	m_pJuqingNode->stopAllActions();
	m_pJuqingNode->      runAction(CCMoveTo::create(0.2F,ccp(0,200)));
	//奇遇按钮
	m_pQiyuNode->stopAllActions();
	m_pQiyuNode->        runAction(CCMoveTo::create(0.2F,ccp(0,200)));
	
	m_pCoinTip->stopAllActions();
	m_pCoinTip->        runAction(CCMoveTo::create(0.2F,ccp(0,-820)));

}

void PveElite::prepareToExitForMap(unsigned int mid)
{

}

void PveElite::actionFinished(CCObject *pObj)
{
	if(m_pDelegate)
	{
		m_pDelegate->showChapterView();
	}
}

void PveElite::PLWheelViewBouncePercent(PLWheelView *pView, float pct)
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

void PveElite::PLWheelViewTouched(PLWheelView *pView)
{
	m_bTouching = true;
	m_fMaxPct = 0.f;
	m_fMinPct = 0.f;
	m_pNodeRotation->stopAllActions();
	m_bWheeling = false;
	PveEliteCell *pCell = (PveEliteCell *)pView->cellAtIndex(m_uCenterIdx);
	pCell->showTouch(false);
}

void PveElite::PLWheelViewStartWheeling(PLWheelView *pView, unsigned int centerIdx)
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

void PveElite::PLWheelViewStopWheeling(PLWheelView *pView, unsigned int centerIdx)
{
	m_bWheeling = false;
	m_bTouching = false;
	PveEliteCell *pCell = (PveEliteCell *)pView->cellAtIndex(m_uCenterIdx);
	pCell->showTouch(true);
}

void PveElite::PLWheelViewCenterIdxChanged(PLWheelView *pView, unsigned int centerIdx)
{
	CCLOG("centerId %d.", centerIdx);
	PveEliteCell *pCell = (PveEliteCell *)m_pView->cellAtIndex(m_uCenterIdx);
	if (pCell)
	{
		pCell->setIsCenter(false);
	}
	m_uCenterIdx = centerIdx;
	pCell = (PveEliteCell *)m_pView->cellAtIndex(m_uCenterIdx);
	if (pCell)
	{
		pCell->setIsCenter(true);
		addArmatureToCell(m_pArmatureCell, pCell);
	}

	pve::MapInfo * pMapInfo = (pve::MapInfo *)maps.objectAtIndex(m_uCenterIdx);
	if (pMapInfo)
	{
		updateDropInfo(pMapInfo);
	}

	/*m_pNodeBg->stopAllActions();
	pve::MapInfo *pInfo = (pve::MapInfo *)maps.objectAtIndex(m_uCenterIdx);
	m_pNodeBg->runAction(CCMoveTo::create(0.5, ccpSub(m_ptOrigin, ccp(pInfo->posX, pInfo->posY))));
	m_pNodeBg->runAction(CCScaleTo::create(0.5, pInfo->scale));*/
}

void PveElite::callBackWonder(CCObject *pObj)
{
	Game::sharedGame()->NavigateTo(navigation::kNavWonder);
}

void PveElite::callBackPve(CCObject *pObj)
{
	prepareToExit();
}

void PveElite::addArmatureToCell(sp::Armature *pArmature, CCTableViewCell *pCell)
{
	if (!pArmature || !pCell)
	{
		return;
	}
	pArmature->removeFromParent();
	pArmature->setPosition(ccp(260, 64));
	pCell->addChild(pArmature);
}

void PveElite::updateDropInfo(pve::MapInfo *obj)
{
	//更新背景
	m_pNodeBg->removeAllChildrenWithCleanup(true);
	m_pSprBg = ResourceManager::sharedResourceManager()->getChapterIcon(m_pPveModel->getChapterExtraInfo(obj->chapterInfo->getCid())->res);
	m_pNodeBg->addChild(m_pSprBg, 1, 1);
	m_pSprBg->stopAllActions();
	m_pSprBg->runAction(CCFadeIn::create(0.2f));
	m_pNodeBg->setPosition(ccpSub(m_ptOrigin, ccp(obj->posX, obj->posY)));
	m_pNodeBg->setScale(obj->scale);

	m_pEliteFirst->setVisible(obj->iScores <=0);
	m_pBoxDrop0->setVisible(false);
	m_pBoxDrop1->setVisible(false);
	
	m_pEliteGold->setString(CCString::createWithFormat("%d",obj->eliteGold)->getCString());
	m_pEliteCoin->setString(CCString::createWithFormat("%d",obj->eliteCoin)->getCString());
	CCArray *pRew = m_pPveModel->getEliteDrop(obj->chapter_id);
	if (pRew && pRew->count() > 0)
	{
		pve::MapInfo *pInfo = NULL;
		CCObject* pObj = NULL;
		CCARRAY_FOREACH(pRew, pObj)
		{
			pInfo = dynamic_cast<pve::MapInfo *>(pObj);
			if (pInfo)
			{
				if ( pInfo->getMapId() == obj->getMapId())
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
			if(m_pDropTitel) m_pDropTitel->setVisible(false);
			if(m_pDropBgBoss) m_pDropBgBoss->setVisible(true);
			if(m_pDropBg) m_pDropBg->setVisible(false);
		}
		if (pInfo && pInfo->prizes_id.size() > 0 && pInfo->prizes_type.size() > 0)
		{
			drop::Drop *dropItem= drop::Drop::create((drop::DropType)pInfo->prizes_type[0], pInfo->prizes_id[0]);
			m_pBoxDrop0->updateDropItem(dropItem);
			m_pBoxDrop0->setVisible(true);
			m_pBoxDrop0->getTouchEvent()->setTag(0);
			m_pBoxDrop0->getTouchEvent()->setTarget(this,menu_selector(PveElite::dropTouchEventHandler));
			m_dropItems->insertObject(dropItem,0);
			if (pInfo->prizes_id.size() > 1 && pInfo->prizes_type.size() > 1)
			{
				dropItem = drop::Drop::create((drop::DropType)pInfo->prizes_type[1], pInfo->prizes_id[1]);
				m_pBoxDrop1->updateDropItem(dropItem);
				m_pBoxDrop1->setVisible(true);
				m_dropItems->insertObject(dropItem,1);
				m_pBoxDrop1->getTouchEvent()->setTag(1);
				m_pBoxDrop1->getTouchEvent()->setTarget(this,menu_selector(PveElite::dropTouchEventHandler));
			}
		}
	}
}

void PveElite::dropTouchEventHandler(CCObject* target)
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

void PveElite::rotateTo(const int &mid)
{
	CCPoint offset = ccp(0,-(mid-1)*PVE_ELITE_CELL_MIN_SIZE.height);
	offset.y = MIN(m_pView->maxContainerOffset().y, offset.y);
	offset.y = MAX(m_pView->minContainerOffset().y, offset.y);

	m_pView->setContentOffset(offset);
}

//MARK: - PveEliteCell -

PveEliteCell::PveEliteCell()
:m_Node(NULL)
,m_pMapInfo(NULL)
,m_pDelegate(NULL)
,m_pPveModel(NULL)
,m_pUserModel(NULL)
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
    D_RESET_C_ARRAY(m_star, ELITE_STARS);
	D_RESET_C_ARRAY(m_images, ELITE_STARS);
	D_RESET_C_ARRAY(m_animations, ELITE_STARS);
}

PveEliteCell::~PveEliteCell()
{
    
	//UNBINDING_ALL(m_pUserModel, this);
    SAFE_RELEASE_UNBINDING_ALL(m_pMapInfo, this);
     D_SAFE_RELEASE_C_ARRAY(m_star, ELITE_STARS);
	 D_SAFE_RELEASE_C_ARRAY(m_images, ELITE_STARS);
	 D_SAFE_RELEASE_C_ARRAY(m_animations, ELITE_STARS);

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

PveEliteCell* PveEliteCell::create(cocos2d::extension::CCData* pData, PveDelegate* pDelegate, PveElite* pMapView)
{
	PveEliteCell *pCell = new PveEliteCell();
	if (pCell && pCell->init(pData, pDelegate, pMapView))
	{
		pCell->autorelease();
		return pCell;
	}
    
	CC_SAFE_DELETE(pCell);
	return NULL;
}

bool PveEliteCell::init(cocos2d::extension::CCData* pData, PveDelegate* pDelegate, PveElite* pMapView)
{
	m_pPveModel		= DM_GET_PVE_MODEL;
	m_pUserModel	= DM_GET_USER_MODEL;
    
    CC_SAFE_RETAIN(m_pPveModel);
    CC_SAFE_RETAIN(m_pUserModel);
    
	m_pDelegate		= pDelegate;
    m_pEliteView      = pMapView;
    
    CCB_READER_DATA(pData,m_Node);
    if(m_Node) addChild(m_Node);


	m_pNodeChapter->setVisible(false);
	m_pNodeMapInfo->setVisible(true);

    return true;
}

void PveEliteCell::onEnter()
{
    CCTableViewCell::onEnter();
    //NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(PveEliteCell::updateRushStatus), D_EVENT_USER_VIP_LEVELUP);
	//BINDING_EXEC(m_pUserModel, this, PveEliteCell::updateRushStatus, D_EVENT_GET_PRIVILEGES);
}

void PveEliteCell::onExit()
{
    CCTableViewCell::onExit();
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    //UNBINDING_ALL(m_pUserModel, this);
    //TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
}

void PveEliteCell::onRushUpdated()
{

}

void PveEliteCell::update(float dt)
{
     
}

bool PveEliteCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
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

SEL_MenuHandler PveEliteCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchArrow", PveEliteCell::onExplore);
    
	return NULL;
}

void PveEliteCell::onTouchNode(cocos2d::CCObject *p)
{
    if (m_pEliteView)
    {
        m_pEliteView->onTouchCell(this);
    }
}

void PveEliteCell::updateView()
{
    
	if (!m_pMapInfo)
	{
		return;
	}
    
	CCLog(m_pMapInfo->map_name.c_str());
	m_pBMFMapName->setString(m_pMapInfo->map_name.c_str());

	//pve::ChapterInfo *m_chapeterInfo = DM_GET_PVE_MODEL->getChapterInfo(m_pMapInfo->chapter_id);
	
	if(m_pMapInfo->mapType==2)
	{
		m_pBtnMapBoss->setVisible(true);
		m_pBtnMap->setVisible(false);
	}
	else
	{
		m_pBtnMapBoss->setVisible(false);
		m_pBtnMap->setVisible(true);
	}

    for (uint8_t i=0; i<ELITE_STARS; ++i)
    {
		m_star[i]->setVisible(i < m_pMapInfo->iScores);
    }
	m_pStarBack->setVisible(m_pMapInfo->iScores > 0);
	m_images[0]->setVisible(m_pMapInfo->iScores == 0);
	m_images[1]->setVisible(m_pMapInfo->iScores == 1 || m_pMapInfo->iScores == 2);
	m_images[2]->setVisible(m_pMapInfo->iScores == 3);
	m_animations[0]->setVisible(m_pMapInfo->iScores == 0);
	m_animations[1]->setVisible(m_pMapInfo->iScores == 1 || m_pMapInfo->iScores == 2);
	m_animations[2]->setVisible(m_pMapInfo->iScores == 3);


	m_pBMFNeedPower->setString(D_CSTRING_FROM_UINT(m_pMapInfo->power_cost));
	m_pBMFRest->setString(D_CSTRING_FROM_UINT(m_pMapInfo->remainTimes()));
}

void PveEliteCell::updateRushStatus()
{

}

void PveEliteCell::updateCell(pve::MapInfo *mapInfo, bool bExpand)
{
	if(m_pMapInfo==mapInfo) return;
	
	SAFE_RELEASE_UNBINDING_ALL(m_pMapInfo, this);
	CC_SAFE_RETAIN(mapInfo);
	m_pMapInfo = mapInfo;
	//BINDING_EXEC(m_pMapInfo, this, PveEliteCell::updateView, D_EVENT_PVE_MAP_UPDATED);
	updateView();
}

void PveEliteCell::onExplore(CCObject *p)
{
	if (m_pMapInfo == NULL)
	{
		return;
	}
	
	if (m_pMapInfo->remainTimes()==0)
	{
		//挑战次数已用完
		Tips_Alert(D_LOCAL_STRING("$errorcode2204").c_str());
	}
	else if(!m_pMapInfo->eliteUnlock)
	{
		//未解锁
		Tips_Alert(D_LOCAL_STRING("eliteInfoError2",m_pMapInfo->chapter_id).c_str());
	}
	else
	{
		m_pPveModel->setLaseEliteID(m_pMapInfo->chapter_id);
		if(m_pDelegate) m_pDelegate->startExploreRequest(m_pMapInfo->getMapId());
	}
}



void PveEliteCell::setIsCenter(bool isCenter)
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

void PveEliteCell::showTouch(bool bShow)
{
	m_pAnimateTouch->setVisible(bShow);
}