//
//  FriendRecommend.cpp
//  商城列表
//
//  Created by cll on 13-6-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "FriendRecommend.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  FRIEND_RECOMMEND_CELL_SIZE		CCSizeMake(611, 150)



/************************ Mall view class begin*******************/

FriendRecommend::FriendRecommend()
:m_pListView(NULL)
,m_pModel(NULL)
,m_pData(NULL)
,m_pMenuItemMore(NULL)
,m_pMenuItemIgnore(NULL)
,m_pBtnSearch(NULL)
,m_pPageContainer(NULL)
,m_pBoxContainer(NULL)
,m_pMyID(NULL)
,m_pEditBox(NULL)
{
    
}


FriendRecommend::~FriendRecommend()
{
    CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pMenuItemMore);
	CC_SAFE_RELEASE(m_pMenuItemIgnore);
	CC_SAFE_RELEASE(m_pBtnSearch);
	CC_SAFE_RELEASE(m_pPageContainer);
	CC_SAFE_RELEASE(m_pBoxContainer);
	CC_SAFE_RELEASE(m_pMyID);
}


FriendRecommend* FriendRecommend::create(const cocos2d::CCSize& size)
{
	FriendRecommend *pRet = new FriendRecommend();
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

bool FriendRecommend::init(const cocos2d::CCSize& size)
{
	if(!DLayer::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_haoyou/pl_haoyou_tuijian");

	m_pData = readCCData("PL_ui/ccb/pl_haoyou/pl_haoyou_tuijianlingqu_cell");
	CC_SAFE_RETAIN(m_pData);
    
	m_pModel = DM_GET_FRIEND_MODEL;
    	
	m_pListView = DTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pPageContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pListView->setFlyInStyle(kCCTableViewFlyInOnEnter);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
    //m_pListView->setPositionX(10);
	m_pPageContainer->addChild(m_pListView);

	m_pMenuItemMore->getButton()->setTarget(this, menu_selector(FriendRecommend::doMore));
	m_pMenuItemIgnore->getButton()->setTarget(this, menu_selector(FriendRecommend::doIgnoreAll));
	m_pBtnSearch->getButton()->setTarget(this, menu_selector(FriendRecommend::doSearch));
    
	initEditBox();
    
	return true;
}

void FriendRecommend::initEditBox()
{

	m_pEditBox = CCEditBox::create(m_pBoxContainer->getContentSize(), CCScale9Sprite::create());
	m_pEditBox->setFont(D_FONT_DEFAULT,D_FONT_SIZE_14);
	m_pEditBox->setFontColor(ccWHITE);
	m_pEditBox->setPlaceHolder(D_LOCAL_STRING(FRIEND_TXT_PLACEHOLDER).c_str());
	m_pEditBox->setPlaceholderFontColor(ccc3(117, 117, 117));
	m_pEditBox->setMaxLength(15);
	m_pEditBox->setReturnType(kKeyboardReturnTypeDone);
	m_pEditBox->setDelegate(this);
	m_pEditBox->setAnchorPoint(CCPointZero);

	m_pBoxContainer->addChild(m_pEditBox);
}

bool FriendRecommend::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuItemMore", DButton *, this->m_pMenuItemMore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuItemIgnore", DButton *, this->m_pMenuItemIgnore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnSearch", DButton *, this->m_pBtnSearch);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPageContainer", CCNode *, this->m_pPageContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxContainer", CCNode *, this->m_pBoxContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMyID", CCLabelTTF *, this->m_pMyID);
	return false;
}

void FriendRecommend::onEnter()
{
	BINDING_EXEC(m_pModel,this,FriendRecommend::updateView,D_EVENT_FRIEND_RECOMMEND);
    DLayer::onEnter();
}

void FriendRecommend::onExit()
{
	UNBINDING_ALL(m_pModel,this);
    
	DLayer::onExit();
}

void FriendRecommend::doMore(CCObject*)
{
	//推荐4个，从哪个开始推荐
	unsigned int times = m_pModel->getRecommendTimes();
	HTTP_CLIENT->recommendFriend(times*4);
}

void FriendRecommend::doIgnoreAll(CCObject*)
{
	//全部忽略
	if(m_pModel->getRequestList()->count() == 0)Tips_Alert(D_LOCAL_STRING("yijianhulvetips").c_str());
	else HTTP_CLIENT->refuseAllFriend();
}

void FriendRecommend::doSearch(CCObject*)
{
	if(strlen(m_pEditBox->getText()) == 0)
	{
		Tips_Alert(D_LOCAL_STRING(FRIEND_TXT_TYPEEMPTY).c_str());
	}
	else{
		int id = atoi(m_pEditBox->getText());
		if(id > 0)HTTP_CLIENT->searchFriend( id , this , callfuncJson_selector(FriendRecommend::onSearch));
		else Tips_Alert(D_LOCAL_STRING("haoyousousuotips").c_str());
	}
}

void FriendRecommend::onSearch(const Json::Value& requestData,const Json::Value& responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;

	partner::FriendBasic *pInfo = partner::FriendBasic::create();
	pInfo->parse(responseData["data"]["recommend"]);

	m_pDataList.removeAllObjects();
	m_pDataList.addObject(pInfo);

	m_pListView->reloadData();
}

void FriendRecommend::updateView()
{
	//CCArray *pRecommend = NULL;
	//CCArray *pRequest = NULL;

	m_pMyID->setString(D_CSTRING_FROM_UINT(DM_GET_USER_MODEL->getUserInfo()->getUserId()));

	m_pModel->getRecommend(&m_pDataList);	
	m_pListView->reloadData();
}


void FriendRecommend::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
	CCLOG("Mall cell touched at index: %i", cell->getIdx());
    
}

CCSize FriendRecommend::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return FRIEND_RECOMMEND_CELL_SIZE;
}

CCTableViewCell* FriendRecommend::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	FriendRecommendCell *cell = (FriendRecommendCell*)table->dequeueCell();
	if (!cell)
	{
		cell = FriendRecommendCell::create(m_pData);
        /*cell->setDelegate(m_pDelegate);
		cell->getBuyButton()->getButton()->setTarget(this,menu_selector(FriendRecommend::buyMenuCallback));*/
	}
    
    partner::FriendBasic * pBasic = (partner::FriendBasic *)m_pDataList.objectAtIndex(idx);
    cell->setFriendBasic(pBasic);
	return cell;
    
}

uint32_t FriendRecommend::numberOfCellsInTableView(CCTableView *table)
{
	return m_pDataList.count();
}


/************************ Mall view class end*******************/










/************************ Mall cell class begin**********************/


FriendRecommendCell::FriendRecommendCell()
	:m_pBasic(NULL)
	,m_pNodeApply(NULL)
	,m_pBtnApply(NULL)
	,m_pNodeAccept(NULL)
	,m_pBtnAccept(NULL)
	,m_pBtnIgnore(NULL)
	,m_pNodeAward(NULL)
	,m_pBtnAward(NULL)
	,m_pName(NULL)
	,m_pTime(NULL)
	,m_pFight(NULL)
	,m_pLevel(NULL)
	,m_nFriendID(0)
	,m_type(kFriendCellRecommend)
{
}

FriendRecommendCell::~FriendRecommendCell()
{
	SAFE_RELEASE_UNBINDING_ALL(m_pBasic, this);
	CC_SAFE_RELEASE(m_pNodeApply);
	CC_SAFE_RELEASE(m_pBtnApply);
	CC_SAFE_RELEASE(m_pNodeAccept);
	CC_SAFE_RELEASE(m_pBtnAccept);
	CC_SAFE_RELEASE(m_pBtnIgnore);
	CC_SAFE_RELEASE(m_pNodeAward);
	CC_SAFE_RELEASE(m_pBtnAward);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pTime);
	CC_SAFE_RELEASE(m_pFight);
	CC_SAFE_RELEASE(m_pLevel);
}

bool FriendRecommendCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeApply", CCNode *, this->m_pNodeApply);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnApply", DButton *, this->m_pBtnApply);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAccept", CCNode *, this->m_pNodeAccept);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAccept", DButton *, this->m_pBtnAccept);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnIgnore", DButton *, this->m_pBtnIgnore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAward", CCNode *, this->m_pNodeAward);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAward", DButton *, this->m_pBtnAward);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTime", CCRichLabelTTF *, this->m_pTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCLabelBMFont *, this->m_pFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    return false;
}

FriendRecommendCell* FriendRecommendCell::create(cocos2d::extension::CCData* pData)
{
    FriendRecommendCell* pRet = new FriendRecommendCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool FriendRecommendCell::init(cocos2d::extension::CCData* pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);
	return true;
}

void FriendRecommendCell::setFriendBasic(partner::FriendBasic* pBasic)
{
	//save friend id
	m_nFriendID = pBasic->getFriendID();

	SAFE_RELEASE_UNBINDING_ALL(m_pBasic , this);
	CC_SAFE_RETAIN(pBasic);
	m_pBasic = pBasic;
	BINDING_EXEC(m_pBasic,this,FriendRecommendCell::updateCell,D_EVENT_FRIEND_BASIC_REFRESH);
}

void FriendRecommendCell::updateCell()
{
	m_pName->setString(m_pBasic->getName().c_str());
	m_pLevel->setString(D_CSTRING_FROM_UINT(m_pBasic->getLevel()));
	m_pFight->setString(D_CSTRING_FROM_UINT(m_pBasic->getFight()));
	//m_pTime->setString(TimeUtil::getSeconds2String(m_pBasic->getOffTime() , "%D:%H:%M" , ":" , "%02d", true));
	m_pTime->setString(DM_GET_FRIEND_MODEL->getStateString(m_pBasic->getOffTime()));
	//绑定事件
	m_pBtnApply->getButton()->setEnabled(!(m_pBasic->getHasRequested()));
	m_pBtnApply->getButton()->setTarget(this, menu_selector(FriendRecommendCell::doApply));
	m_pBtnAccept->getButton()->setTarget(this, menu_selector(FriendRecommendCell::doAgree));
	m_pBtnIgnore->getButton()->setTarget(this, menu_selector(FriendRecommendCell::doRefuse));
	//显示隐藏
	if(m_type == kFriendCellRecommend)
	{
		//推荐页
		m_pNodeAccept->setVisible(m_pBasic->getNewRequest());
		m_pNodeApply->setVisible(!m_pBasic->getNewRequest());
		//如果搜索的好友是自己，不能让玩家点击申请
		m_pBtnApply->getButton()->setEnabled(DM_GET_USER_MODEL->getUserInfo()->getUserId() != m_pBasic->getFriendID());
	}
	else{
		//领耐力页
		m_pNodeAccept->setVisible(false);
		m_pNodeApply->setVisible(false);
		m_pNodeAward->setVisible(true);
		m_pBtnAward->getButton()->setTarget(this, menu_selector(FriendRecommendCell::doAward));
	}
	
}

void FriendRecommendCell::doApply(CCObject*)
{
	//check if they are already friend
	CCArray* friendlist = DM_GET_FRIEND_MODEL->getFriendList(CCArray::create());
	
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(friendlist, pObj)
	{
		partner::FriendBasic *pBasic = static_cast<partner::FriendBasic *>(pObj);
		if (pBasic->getFriendID() == m_nFriendID)
		{
			Tips_Alert("You are already friend!");
			return;
		}
	}

	//apply
	DM_GET_FRIEND_MODEL->doApply(m_nFriendID);

}
void FriendRecommendCell::doAgree(CCObject*)
{
	if(m_pBasic){
		HTTP_CLIENT->acceptFriend(m_pBasic->getFriendID());
	}
}
void FriendRecommendCell::doRefuse(CCObject*)
{
	if(m_pBasic){
		HTTP_CLIENT->refuseFriend(m_pBasic->getFriendID());
	}
}
void FriendRecommendCell::doAward(CCObject*)
{
	if(m_pBasic){
		HTTP_CLIENT->friendReceiveGift(m_pBasic->getFriendID());
		
		//DM_GET_FRIEND_MODEL->doAward(m_pBasic->getFriendID());
	}
}

/************************ Mall view cell class end**********************/




