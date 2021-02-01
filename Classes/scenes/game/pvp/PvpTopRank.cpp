//
//  PvpTopRankView.cpp
//  论剑Top10列表
//
//  Created by cll on 13-6-18
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PvpTopRank.h"
#include "Pvp.h"
#include "managers/ResourceManager.h"
#include "model/UserModel.h"
#include "managers/GameManager.h"
#include "PvpTargetCell.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  PVP_LIST_RECT		CCRectMake(0, 0, 640, 672)

#define  PVP_CELL_SIZE		CCSizeMake(640, 112)

#define  PVP_LIST_OFFSET_X	8

#define  PVP_LIST_OFFSET_Y  -27





/************************ 东皇老爷子 *******************/

class PvpTopRank0Cell:public CCTableViewCell
{
public:
    PvpTopRank0Cell()
    {
        
    }
    
    virtual ~PvpTopRank0Cell()
    {
        
    }
    static PvpTopRank0Cell* create(CCData* data)
    {
        PvpTopRank0Cell* pRet = new PvpTopRank0Cell();
        if(pRet && pRet->init(data))
        {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
    
    bool init(CCData* data)
    {
        CCB_READER_DATA_AND_ADDCHILD(data);
        return true;
    }
};


/************************ pvp select class begin*******************/



PvpTopRankView::PvpTopRankView()
: m_pListView(NULL)
, m_pDelegate(NULL)
, m_pData(NULL)
, m_pData0(NULL)
, m_pRanks(NULL)
{
};

PvpTopRankView::~PvpTopRankView()
{
    CC_SAFE_RELEASE(m_pData);
    CC_SAFE_RELEASE(m_pData0);
    CC_SAFE_RELEASE(m_pRanks);
}


bool PvpTopRankView::init()
{
	if(!DLayer::init())
	{
		return false;
	}
    
    m_pData = readCCData("ui/ccb/pvp/pvpPlayerCell");
    CC_SAFE_RETAIN(m_pData);
    
    m_pData0= readCCData("ui/ccb/pvp/pvpRankNo1Cell");
    CC_SAFE_RETAIN(m_pData0);
    
    m_pRanks = CCDictionary::create();
    CC_SAFE_RETAIN(m_pRanks);

	m_pListView = DTableView::create(this, CCSizeMake(PVP_LIST_RECT.size.width,PVP_LIST_RECT.size.height));
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setAnchorPoint(CCPointZero);
	m_pListView->setPosition(ccp(PVP_LIST_OFFSET_X, PVP_LIST_OFFSET_Y));
	addChild(m_pListView);

	return true;
}

void PvpTopRankView::onEnter()
{
    DLayer::onEnter();
    updateView();
}

void PvpTopRankView::onExit()
{
	DLayer::onExit();
    m_pRanks->removeAllObjects();
    m_pListView->reloadData();
    m_pListView->releaseCellsFreed();
    
}

void PvpTopRankView::updateView()
{
    if(!DM_GET_PVP_MODEL->isSynced)
    {
        return;
    }
    DM_GET_PVP_MODEL->getTopRanks(m_pRanks);
    m_pRanks->setObject(pvp::ChTargetInfo::create(), 0);
	m_pListView->reloadData();
    m_pListView->setContentOffset(m_pListView->minContainerOffset());
}


void PvpTopRankView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	uint32_t idx =  cell->getIdx();

	CCLOG("rank cell touched at index: %i", idx);

}

CCSize PvpTopRankView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return PVP_CELL_SIZE;
}

CCTableViewCell* PvpTopRankView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{	
	CCTableViewCell *cell = table->dequeueCell();
    
	if (idx==0 && dynamic_cast<PvpTopRank0Cell*>(cell)==NULL)
	{
		cell = PvpTopRank0Cell::create(m_pData0);
	}else
    {
        PvpTopRankCell* pRankCell = dynamic_cast<PvpTopRankCell*>(cell);
        if(pRankCell==NULL)
        {
            pRankCell = PvpTopRankCell::create(m_pData);
        }
        
        pRankCell->setDelegate(m_pDelegate);
        uint32_t rank = idx;
//         pvp::ChTargetInfo * target = (pvp::ChTargetInfo *)(m_pRanks->objectForKey(rank));
//         pRankCell->setTargetInfo(target);
        cell =  pRankCell;
    }
    

	return cell;
}

uint32_t PvpTopRankView::numberOfCellsInTableView(CCTableView *table)
{
	return m_pRanks->count();
}


/************************ pvp select class end*******************/

