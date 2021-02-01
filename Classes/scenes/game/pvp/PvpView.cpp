//
//  PvpView.cpp
//  论剑列表
//
//  Created by cll on 13-4-20
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PvpView.h"
#include "Pvp.h"
#include "managers/ResourceManager.h"
#include "model/UserModel.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"
#include "PvpTargetCell.h"
#include "RankUser.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  PVP_LIST_RECT		CCRectMake(0, 0, 640, 560)

#define	PVP_CELL_SIZE_OUT	CCSizeMake(640, 643)

#define  PVP_CELL_SIZE		CCSizeMake(640, 214)

#define CLOUD_INDEX	10

#pragma mark --------------pvp view----------------
/************************ pvp select class begin*******************/

PvpView::PvpView()
:m_pListView(NULL)
,m_pDelegate(NULL)
// ,m_pBoss(NULL)
// ,m_pRobot(NULL)
// ,m_pListContainer(NULL)
,m_pContainer(NULL)
,m_pCell(NULL)
,m_pData(NULL)
,m_pTargets(NULL)
,m_pRanks(NULL)
,m_pBMFRank(NULL)
,m_pBMFPvpPoints(NULL)
,m_pTTFCountDown(NULL)
,m_pCloud(NULL)
,doSlide(true)
{
};

PvpView::~PvpView()
{
//     CC_SAFE_RELEASE(m_pBoss);
//     CC_SAFE_RELEASE(m_pRobot);
//     CC_SAFE_RELEASE(m_pListContainer);
	CC_SAFE_RELEASE(m_pContainer);
	//CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pData);
    CC_SAFE_RELEASE(m_pTargets);
	CC_SAFE_RELEASE(m_pRanks);
	CC_SAFE_RELEASE(m_pBMFRank);
	CC_SAFE_RELEASE(m_pBMFPvpPoints);
	CC_SAFE_RELEASE(m_pTTFCountDown);
}

bool PvpView::init()
{
	if(!DLayer::init())
	{
		return false;
	}
    
    m_pTargets = CCArray::create();
    CC_SAFE_RETAIN(m_pTargets);

	m_pRanks = CCDictionary::create();
	CC_SAFE_RETAIN(m_pRanks);
    
//     cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
//     CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
//     pNodeLoaderLibrary->registerCCNodeLoader("PvpBossView", PvpBossViewLoader::loader());
//     pNodeLoaderLibrary->registerCCNodeLoader("PvpRobotView", PvpRobotViewLoader::loader());
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_lunjian/PL_lunjian_paihang");
    
    m_pData = readCCData("PL_ui/ccb/pl_lunjian/PL_lunjian_paihang_cell");
    CC_SAFE_RETAIN(m_pData);

	//cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	//CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	//pNodeLoaderLibrary->registerCCNodeLoader("RankUser", RankUserLoader::loader());
	//CCB_READER_DATA_BY_CUSTOM_LOADER(m_pData, m_pCell, pNodeLoaderLibrary);
	//CC_SAFE_RETAIN(m_pCell);
//     
	m_pListView = DTableView::create(this, m_pContainer->getContentSize());
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setAnchorPoint(CCPointZero);
	m_pListView->setPosition(ccp(0,0));
	m_pContainer->addChild(m_pListView);


	m_pCloud = CCSprite::create("PL_ui/lunjian/PL_lunjian_yun.png");
	m_pCloud->setAnchorPoint(ccp(0, 0));
	m_pListView->addChild(m_pCloud, 9999, 1);
// 
//     setVisible(false);
//     
	return true;
    
}

void PvpView::onEnter()
{
	scheduleUpdate();
    DLayer::onEnter();
	BINDING_EXEC(DM_GET_PVP_MODEL, this, PvpView::updateView, D_EVENT_PVP_INIT);
	BINDING_ONLY(DM_GET_PVP_MODEL, this, PvpView::updateView, D_EVENT_PVPLIST_UPDATE);
	BINDING_ONLY(DM_GET_PVP_MODEL, this, PvpView::updateView, D_EVENT_PVPINFO_UPDATE);

// 	m_pBoss->setDelegate(m_pDelegate);
// 	m_pRobot->setDelegate(m_pDelegate);
}

void PvpView::onExit()
{
	unscheduleUpdate();
	UNBINDING_ALL(DM_GET_PVP_MODEL,this);
	DLayer::onExit();
//     m_pTargets->removeAllObjects();
//     m_pListView->reloadData();
//     m_pListView->releaseCellsFreed();
}

bool PvpView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoss", PvpBossView *, this->m_pBoss);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRobot", PvpRobotView *, this->m_pRobot);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pListContainer", cocos2d::CCNode *, this->m_pListContainer);
    
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", cocos2d::CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRank", CCLabelBMFont *, this->m_pBMFRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFPvpPoints", CCLabelBMFont *, this->m_pBMFPvpPoints);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFCountDown", CCLabelBMFont *, this->m_pTTFCountDown);
    return false;
}

void PvpView::updateView()
{
    if(!DM_GET_PVP_MODEL->isSynced)
    {
        return;
    }
    
	DM_GET_PVP_MODEL->getTarges(m_pTargets);
	DM_GET_PVP_MODEL->getTopRanks(m_pRanks);

	CCDictElement *pTmp = NULL;
	CCDictElement *pEle = NULL;


// 	CCDICT_FOREACH(m_pRanks, pEle)
// 	{
// 		pvp::ChTargetInfo * rankT = dynamic_cast<pvp::ChTargetInfo *>(pEle->getObject());
// 		CCObject *pObj = NULL;
// 		bool dup = false;
// 
// 		CCARRAY_FOREACH(m_pTargets, pObj)
// 		{
// 			pvp::ChTargetInfo * target = dynamic_cast<pvp::ChTargetInfo *>(pObj);
// 			if (target == NULL)
// 			{
// 				continue;
// 			}
// 			if (rankT->getRank() == target->getRank())
// 			{
// 				dup = true;
// 				break;
// 			}
// 		}
// 		if (!dup)
// 		{
// 			m_pTargets->insertObject(pEle->getObject(), pEle->getIntKey() - 1);
// 		}
// 		
// 	}

	int topRank = D_CONFIG_UINT_FOR_KEY(PVP_TOPLIST_NUM);
	for (int i = 1; i <= topRank; i++)
	{
		pvp::ChTargetInfo * rankT = dynamic_cast<pvp::ChTargetInfo *>(m_pRanks->objectForKey(i));
		CCObject *pObj = NULL;
		bool dup = false;

		CCARRAY_FOREACH(m_pTargets, pObj)
		{
			pvp::ChTargetInfo * target = dynamic_cast<pvp::ChTargetInfo *>(pObj);
			if (target == NULL)
			{
				continue;
			}
			if (rankT->getRank() == target->getRank())
			{
				dup = true;
				break;
			}
		}
		if (!dup)
		{
			m_pTargets->insertObject(rankT, i - 1);
		}
	}

//	m_pTargets->addObjectsFromArray(m_pRanks);
    //setVisible(true);
    pvp::ChallengeInfo * info = DM_GET_PVP_MODEL->getChallengeInfo();
	m_pBMFRank->setString(D_CSTRING_FROM_UINT(info->getRank()));
	m_pBMFPvpPoints->setString(D_CSTRING_FROM_UINT(info->getPoint()));
//     m_pRobot->updateView();

//     if(info->bossVisible())
//     {
//         m_pBoss->setVisible(true);
//         m_pBoss->updateView();
//         m_pListView->setViewSize(CCSizeMake(PVP_LIST_RECT.size.width,PVP_LIST_RECT.size.height - PVP_CELL_SIZE.height));
//     }
//     else
//     {
//         m_pBoss->setVisible(false);
//         m_pListView->setViewSize(CCSizeMake(PVP_LIST_RECT.size.width, PVP_LIST_RECT.size.height));
//     }
    


	CCObject *tmp = new CCObject();
	tmp->autorelease();

	int heightIdx  = m_pTargets->count() - m_pRanks->count();

	m_pTargets->insertObject(tmp, CLOUD_INDEX);
	m_pTargets->insertObject(tmp, CLOUD_INDEX);
	m_pTargets->insertObject(tmp, CLOUD_INDEX);

    
	m_pListView->reloadData();
	m_pCloud->setPositionY(PVP_CELL_SIZE_OUT.height / 3 * heightIdx);
	
	uint32_t myrank = DM_GET_PVP_MODEL->getChallengeInfo()->getRank();

    uint32_t offset = DM_GET_PVP_MODEL->getUserIndex();
	//这里因为前十名的缘故 所以先+10 还有云
	if(myrank > 10){
		offset += 13;
	}

	//新增一个offset 准确定位
	float fOffset = 0.0f;

    uint32_t cells = (uint32_t)(m_pListView->getViewSize().height / PVP_CELL_SIZE.height);
    if(offset + 1 >= cells)
    {
        offset = offset - cells + 3;
		fOffset = -0.7 * PVP_CELL_SIZE.height;
    }
    else{
        offset = 0;
    }
    //m_pListView->setContentOffset(ccpAdd(m_pListView->minContainerOffset(), ccp(0, offset * PVP_CELL_SIZE.height)));
	
	//滑不滑啊！
	if(doSlide){
		doSlide = false;
		m_pListView->setContentOffset(m_pListView->minContainerOffset());
		m_pListView->setContentOffsetInDuration(ccpAdd(m_pListView->minContainerOffset(), ccp(0, offset * PVP_CELL_SIZE.height + fOffset )) , 0.8f);
	}
	else{
		m_pListView->setContentOffset(ccpAdd(m_pListView->minContainerOffset(), ccp(0, offset * PVP_CELL_SIZE.height + fOffset )));
	}
	
}


void PvpView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	uint32_t idx =  cell->getIdx();
	CCLOG("pvp cell touched at index: %i", idx);
}

CCSize PvpView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	if (idx == numberOfCellsInTableView(table) - 1)
	{
		unsigned int ct = m_pTargets->count() + 1;
		unsigned int modCount = ct % PVP_CELL_USER_COUNT;
		if (modCount == 0)
		{
			return PVP_CELL_SIZE_OUT;
		}
		
		return CCSizeMake(PVP_CELL_SIZE_OUT.width, PVP_CELL_SIZE_OUT.height / PVP_CELL_USER_COUNT * modCount);
	}
	
	return PVP_CELL_SIZE_OUT;
}

CCTableViewCell* PvpView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	PvpViewCell *cell = (PvpViewCell*)table->dequeueCell();
    
	if (!cell)
	{
		cell = PvpViewCell::create(m_pData);
	}
    cell->setDelegate(m_pDelegate);
    

	bool bBossVisible = false;
	pvp::ChTargetInfo * target0 = NULL;
	if (idx != 0)
	{
		target0 = dynamic_cast<pvp::ChTargetInfo*>(m_pTargets->objectAtIndex(idx * 3 - 1));
	}
	bool autoMove = idx * 3 >= m_pTargets->count() - 1;
	pvp::ChTargetInfo * target1 = NULL;
	if (idx * 3 < m_pTargets->count())
	{
		target1 = dynamic_cast<pvp::ChTargetInfo*>(m_pTargets->objectAtIndex(idx * 3));
	}
	pvp::ChTargetInfo * target2 = NULL;
	if (idx * 3 + 1 < m_pTargets->count())
	{
		target2 = dynamic_cast<pvp::ChTargetInfo*>(m_pTargets->objectAtIndex(idx * 3 + 1));
	}

	bBossVisible |= target0 && target0->getRank() == 1;
	bBossVisible |= target1 && target1->getRank() == 1;
	bBossVisible |= target2 && target2->getRank() == 1;

	cell->setTargetInfo(target0, target1, target2, autoMove);
	cell->setBossVisible(bBossVisible);
	cell->setZOrder(idx);
    
	return cell;
}

uint32_t PvpView::numberOfCellsInTableView(CCTableView *table)
{
	if(m_pListView && m_pDelegate)
	{
		int offset = m_pListView->getContentOffset().y;
		m_pDelegate->slideBackGround(offset/30);
	}
	

 	unsigned int ct = m_pTargets->count() + 1;
 	unsigned int cellCount = ct / PVP_CELL_USER_COUNT + (ct % PVP_CELL_USER_COUNT > 0);
 	return cellCount;
//	return m_pTargets->count() / 3;
}

void PvpView::update(float dt)
{
	uint32_t now = GameManager::sharedGameManager()->getServerTime();
	uint32_t st = TimeUtil::getStartTimeOfToday(now);
	uint32_t pt = DM_GET_PVP_MODEL->getPrizeTime();
	uint32_t cd = 0;
	if (now > st + pt)
	{
		st = TimeUtil::getStartTimeOfTomorrow(now);
	}
	m_pTTFCountDown->setString(TimeUtil::getSeconds2String(st + pt - now));
}

/************************ pvp select class end*******************/
