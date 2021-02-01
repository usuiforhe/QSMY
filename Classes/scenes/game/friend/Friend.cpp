//
//  Friend.h
//  好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#include "Friend.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "FriendListView.h"
#include "FriendRecommend.h"
#include "FriendGift.h"
#include "../user/UserInfo.h"
#include "FriendFormation.h"


Friend::Friend()
	:m_pDTabView(NULL)
	, m_pFocusLayer(NULL)
	, m_pPopupLayer(NULL)
	, m_pModel(NULL)
	, m_pContainer(NULL)
	, m_pContainerPos(NULL)
	, m_pCloseButton(NULL)
	,m_pChallenge(NULL)
	,m_pView(NULL)
	,m_pDelete(NULL)
	,m_pBtnClose(NULL)
	,m_pPopup(NULL)
	,m_pConfirmButton(NULL)
	,m_pFriendless(NULL)
	,m_uCurrentFriendID(0)
{
}

Friend::~Friend()
{
	CC_SAFE_RELEASE(m_pDTabView);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pContainerPos);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pChallenge);
	CC_SAFE_RELEASE(m_pView);
	CC_SAFE_RELEASE(m_pDelete);
	CC_SAFE_RELEASE(m_pBtnClose);
	CC_SAFE_RELEASE(m_pPopup);
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pFriendless);
}

Friend* Friend::create()
{
	Friend *pRet = new Friend();
	if (pRet && pRet->init())
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

bool Friend::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pModel = DM_GET_FRIEND_MODEL;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_haoyou/pl_haoyou");

	m_pPopup->setVisible(false);

	m_pFocusLayer = CCNode::create();
	addChild(m_pFocusLayer, 1);
	m_pPopupLayer = CCNode::create();
	addChild(m_pPopupLayer, 2);

	m_pFriendless->setVisible(false);

	//事件绑定
	m_pBtnClose->getButton()->setTarget(this, menu_selector(Friend::onMenuClose));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(Friend::closeCallBack));
	m_pConfirmButton->getButton()->setTarget(this, menu_selector(Friend::onMenuClose));
	m_pDelete->setTarget(this, menu_selector(Friend::onMenu));
	m_pView->setTarget(this, menu_selector(Friend::onMenu));
	m_pChallenge->setTarget(this, menu_selector(Friend::onMenu));
	

	float posY = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainerPos->getPosition().y, PL_MAX_HEIGHT_DIFF / 2);
	float height = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainerPos->getContentSize().height+8, -PL_MAX_HEIGHT_DIFF);
	m_pDTabView = DTabView::create(CCRectMake(m_pContainerPos->getPosition().x, posY, m_pContainerPos->getContentSize().width, height),5);
	m_pDTabView->retain();

	m_pDTabView->addTab(FriendListView::create(this , PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainerPos->getContentSize(), -PL_MAX_HEIGHT_DIFF) , m_pContainerPos->getPosition()),"","PL_ui/all/all_btn_fenye_lan.png","PL_ui/all/all_btn_fenye_lv.png",NULL,"PL_ui/haoyou/haoyou_btn_txt_wode.png");
	m_pDTabView->addTab(FriendRecommend::create(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainerPos->getContentSize(), -PL_MAX_HEIGHT_DIFF)),"","PL_ui/all/all_btn_fenye_lan.png","PL_ui/all/all_btn_fenye_lv.png",NULL,"PL_ui/haoyou/haoyou_btn_txt_tuijian.png");
	m_pDTabView->addTab(FriendGift::create(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainerPos->getContentSize(), -PL_MAX_HEIGHT_DIFF)),"","PL_ui/all/all_btn_fenye_lan.png","PL_ui/all/all_btn_fenye_lv.png",NULL,"PL_ui/haoyou/haoyou_btn_txt_naili.png");
	
	m_pContainer->addChild(m_pDTabView);

	return true;
}

bool Friend::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainerPos", CCNode *, this->m_pContainerPos);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	/*CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBottom", CCNode *, this->m_pBottom);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableNode", CCNode *, this->m_pTableNode);*/
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChallenge", CCMenuItemImage *, this->m_pChallenge);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pView", CCMenuItemImage *, this->m_pView);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDelete", CCMenuItemImage *, this->m_pDelete);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnClose", DButton *, this->m_pBtnClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, this->m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPopup", CCNode *, this->m_pPopup);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFriendless", CCNode *, this->m_pFriendless);
	return false;
}

void Friend::closeCallBack(CCObject* obj)
{
	removeFromParent();
	Game::sharedGame()->NavigateTo(navigation::kNavHome);
}

void Friend::onEnter()
{
	m_pModel->onEnter();
	updateRequest(0);
	DLayer::onEnter();
}

void Friend::onExit()
{
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
	DLayer::onExit();
	//FriendNavigateTo(kFriendNavHome);
}

void Friend::onBackKey()
{
	//if (m_ePopingNav != kFriendNavNoPop)
	//{
	//	FriendNavigateTo(kFriendNavNoPop);
	//	return;
	//}
	//if (m_eShowingNav != kFriendNavHome)
	//{
	//	FriendNavigateTo(kFriendNavHome);
	//	return;
	//}
	BackKeyInterface::onBackKey();
}


void Friend::updateRequest(float dt)
{
	HTTP_CLIENT->friendGetNewRequest(this, callfuncJson_selector(Friend::callBackRequest));
}

void Friend::callBackRequest(const Json::Value &request, const Json::Value &response)
{
	if(response["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
	TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(Friend::updateRequest), this, D_CONFIG_UINT_FOR_KEY(FRIENDS_REQUEST_REFRESH_INTERVAL));
}

void Friend::onMenuClose(CCObject*)
{
	m_pPopup->setVisible(false);
}

/************************************************************************/
/* delegate                                                                     */
/************************************************************************/
void Friend::showMenu(unsigned int fid)
{
	m_uCurrentFriendID = fid;
	m_pPopup->setVisible(true);
}
void Friend::onMenu(CCObject* obj)
{
	if(!m_uCurrentFriendID) return;
	//确保有正确的fid

	CCNode * menunode = dynamic_cast<CCNode *>(obj);
	Friend::FriendMenuType menutype = (Friend::FriendMenuType)menunode->getTag();
	switch(menutype){
	case kFriendMenuView:
		{
			//查看别人阵容
			FriendFormation* pFormation = FriendFormation::create();
			pFormation->setInfo(m_uCurrentFriendID);
			m_pPopupLayer->addChild(pFormation);
			break;
		}
	case kFriendMenuChallenge:
		{
			HTTP_CLIENT->getOtherFormation(m_uCurrentFriendID);
			DM_GET_OTHERFORMATION_MODEL->setCurrentWarriorId(m_uCurrentFriendID);
			HTTP_CLIENT->challengeFriend(m_uCurrentFriendID);
			break;
		}
	case kFriendMenuDelete:
		{
			//目前只绑定了删除
			HTTP_CLIENT->deleteFriend(m_uCurrentFriendID);			
			break;
		}
	}
	//点完之后，要把弹框关了
	m_pPopup->setVisible(false);
}