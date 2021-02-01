//
//  FriendListView.cpp
//  商城列表
//
//  Created by cll on 13-6-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "FriendListView.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  FRIEND_LIST_CELL_SIZE		CCSizeMake(611, 150)



/************************ Mall view class begin*******************/

FriendListView::FriendListView()
:m_pListView(NULL)
,m_pModel(NULL)
,m_pData(NULL)
,m_pDelegate(NULL)
{
    
}

FriendListView::~FriendListView()
{
    CC_SAFE_RELEASE(m_pData);
}

FriendListView* FriendListView::create(FriendListDelegate* pDelegate , const cocos2d::CCSize& size , const cocos2d::CCPoint& pos)
{
	FriendListView *pRet = new FriendListView();
	if (pRet && pRet->init(pDelegate , size , pos))
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

bool FriendListView::init(FriendListDelegate* pDelegate , const cocos2d::CCSize& size , const cocos2d::CCPoint& pos)
{
	if(!DLayer::init())
	{
		return false;
	}
	m_pDelegate = pDelegate;
	m_pModel    = DM_GET_FRIEND_MODEL;

	m_pData = readCCData("PL_ui/ccb/pl_haoyou/pl_haoyou_cell_wode");
	CC_SAFE_RETAIN(m_pData);
    
	m_pListView = DTableView::create(this, size);
	m_pListView->setFlyInStyle(kCCTableViewFlyInOnEnter);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	addChild(m_pListView);

	return true;
}

void FriendListView::onEnter()
{
	BINDING_EXEC(m_pModel,this,FriendListView::updateFriendListView,D_EVENT_FRIEND_LIST);
    DLayer::onEnter();
}

void FriendListView::onExit()
{
	m_pDelegate->toggleView(false);
	UNBINDING_ALL(m_pModel,this);    
	DLayer::onExit();
}



void FriendListView::updateFriendListView()
{
	m_pModel->getFriendList(&m_pDataList);	
	m_pDelegate->toggleView(m_pDataList.count() == 0);
	m_pListView->reloadData();
}


void FriendListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
	CCLOG("Mall cell touched at index: %i", cell->getIdx());
    
}

CCSize FriendListView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return FRIEND_LIST_CELL_SIZE;
}

CCTableViewCell* FriendListView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	FriendListCell *cell = (FriendListCell*)table->dequeueCell();
	if (!cell)
	{
		cell = FriendListCell::create(m_pData);
	}
	partner::FriendInfo * pInfo = (partner::FriendInfo *)m_pDataList.objectAtIndex(idx);
	cell->setFriendInfo(pInfo);
	cell->setDelegate(m_pDelegate);
	return cell;
    
}

uint32_t FriendListView::numberOfCellsInTableView(CCTableView *table)
{
	return m_pDataList.count();
}



/************************ Mall view class end*******************/










/************************ Mall cell class begin**********************/


FriendListCell::FriendListCell()
	:m_pName(NULL)
	,m_pTime(NULL)
	,m_pFight(NULL)
	,m_pLevel(NULL)
	,m_pMenu(NULL)
	,m_pGift(NULL)
	,m_pDelegate(NULL)
	,m_uFriendID(0)
	,m_pFriendInfo(NULL)
	,m_pCanSend(NULL)
	,m_pCantSend(NULL)
{
    
}

FriendListCell::~FriendListCell()
{
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pTime);
	CC_SAFE_RELEASE(m_pFight);
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pMenu);
	CC_SAFE_RELEASE(m_pGift);
	SAFE_RELEASE_UNBINDING_ALL(m_pFriendInfo , this);
	CC_SAFE_RELEASE(m_pCanSend);
	CC_SAFE_RELEASE(m_pCantSend);
}

bool FriendListCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTime", CCRichLabelTTF *, this->m_pTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCLabelBMFont *, this->m_pFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGift", CCMenuItemImage *, this->m_pGift);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenu", CCMenuItemImage *, this->m_pMenu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCanSend", CCNode *, this->m_pCanSend);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCantSend", CCNode *, this->m_pCantSend);
    return false;
}

FriendListCell* FriendListCell::create(cocos2d::extension::CCData* pData)
{
    FriendListCell* pRet = new FriendListCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool FriendListCell::init(cocos2d::extension::CCData* pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);

	m_pMenu->setTarget(this, menu_selector(FriendListCell::onMenu));
	m_pGift->setTarget(this, menu_selector(FriendListCell::sendGift));
	return true;
}

void FriendListCell::onMenu(cocos2d::CCObject*)
{
	//show menu
	if(m_pDelegate)
		m_pDelegate->showMenu(m_uFriendID);
}

void FriendListCell::sendGift(cocos2d::CCObject*)
{
	HTTP_CLIENT->friendSendGift(m_uFriendID);
}

void FriendListCell::updateCell()
{
	m_pName->setString(m_pFriendInfo->getName().c_str());
	m_pLevel->setString(D_CSTRING_FROM_UINT(m_pFriendInfo->getLevel()));
	m_pFight->setString(D_CSTRING_FROM_UINT(m_pFriendInfo->getFight()));
	//m_pTime->setString(TimeUtil::getSeconds2String(m_pFriendInfo->getOffTime() , "%D:%H:%M" , ":" , "%02d", true));
	m_pTime->setString(DM_GET_FRIEND_MODEL->getStateString(m_pFriendInfo->getOffTime()));
	//耐力是否可以赠送
	uint32_t uNow = GameManager::sharedGameManager()->getServerTime();
	uint32_t uStart = TimeUtil::getStartTimeOfToday(uNow);
	bool bEnable = m_pFriendInfo->getPresentTs() < uStart;
	m_pGift->setEnabled(bEnable);
	m_pCanSend->setVisible(bEnable);
	m_pCantSend->setVisible(!bEnable);
}

void FriendListCell::setFriendInfo(partner::FriendInfo* pInfo)
{
	m_uFriendID = pInfo->getFriendID();

	SAFE_RELEASE_UNBINDING_ALL(m_pFriendInfo , this);
	CC_SAFE_RETAIN(pInfo);
	m_pFriendInfo = pInfo;
	BINDING_EXEC(m_pFriendInfo,this,FriendListCell::updateCell,D_EVENT_FRIEND_INFO_REFRESH);
	


	//save friend id
	/*m_nFriendID = pBasic->getFriendID();

	SAFE_RELEASE_UNBINDING_ALL(m_pBasic , this);
	CC_SAFE_RETAIN(pBasic);
	m_pBasic = pBasic;
	BINDING_EXEC(m_pBasic,this,FriendRecommendCell::updateCell,D_EVENT_FRIEND_BASIC_REFRESH);*/
}

/************************ Mall view cell class end**********************/




