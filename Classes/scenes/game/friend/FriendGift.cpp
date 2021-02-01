//
//  FriendGift.cpp
//  商城列表
//
//  Created by cll on 13-6-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "FriendGift.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  FRIEND_RECOMMEND_CELL_SIZE		CCSizeMake(611, 150)



/************************ Mall view class begin*******************/

FriendGift::FriendGift()
:m_pListView(NULL)
,m_pModel(NULL)
,m_pData(NULL)
,m_pReceiveAll(NULL)
,m_pTimes(NULL)
,m_pPageContainer(NULL)
{
    
}


FriendGift::~FriendGift()
{
    CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pReceiveAll);
	CC_SAFE_RELEASE(m_pTimes);
	CC_SAFE_RELEASE(m_pPageContainer);
}


FriendGift* FriendGift::create(const cocos2d::CCSize& size)
{
	FriendGift *pRet = new FriendGift();
	if (pRet && pRet->init(size))
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

bool FriendGift::init(const cocos2d::CCSize& size)
{
	if(!DLayer::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_haoyou/pl_haoyou_lingqu");

	m_pData = readCCData("PL_ui/ccb/pl_haoyou/pl_haoyou_tuijianlingqu_cell");
	CC_SAFE_RETAIN(m_pData);
    
	m_pModel = DM_GET_FRIEND_MODEL;
    
	m_pListView = DTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pPageContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pListView->setFlyInStyle(kCCTableViewFlyInOnEnter);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pPageContainer->addChild(m_pListView);

	m_pReceiveAll->setTarget(this, menu_selector(FriendGift::revieveAllWithGift));
    
    
	return true;
}


bool FriendGift::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReceiveAll", CCMenuItemImage *, this->m_pReceiveAll);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTimes", CCLabelBMFont *, this->m_pTimes);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPageContainer", CCNode *, this->m_pPageContainer);
	
	return false;
}

void FriendGift::onEnter()
{
	BINDING_EXEC(m_pModel,this,FriendGift::updateView,D_EVENT_FRIEND_GIFT_LIST);
    DLayer::onEnter();
}

void FriendGift::onExit()
{
	UNBINDING_ALL(m_pModel,this); 
	DLayer::onExit();
}

void FriendGift::revieveAllWithGift(CCObject*)
{
	HTTP_CLIENT->receiveAndSendGiftAll();
}

void FriendGift::updateView()
{
	m_pModel->getGiftList(&m_pDataList);	
	m_pListView->reloadData();
}


void FriendGift::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
	CCLOG("Mall cell touched at index: %i", cell->getIdx());
    
}

CCSize FriendGift::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return FRIEND_RECOMMEND_CELL_SIZE;
}

CCTableViewCell* FriendGift::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	FriendRecommendCell *cell = (FriendRecommendCell*)table->dequeueCell();
	if (!cell)
	{
		cell = FriendRecommendCell::create(m_pData);
	}
    cell->m_type = kFriendCellGift;
    partner::FriendBasic * pBasic = (partner::FriendBasic *)m_pDataList.objectAtIndex(idx);
    cell->setFriendBasic(pBasic);
	return cell;
    
}

uint32_t FriendGift::numberOfCellsInTableView(CCTableView *table)
{
	return m_pDataList.count();
}


/************************ Mall view class end*******************/


