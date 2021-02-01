//
//  UserDetail.cpp
//  用户详情
//
//  Created by Eci on 13-12-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "UserDetail.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "managers/TipsManager.h"

#include "../inventory/ChangeName.h"
#include "utils/TimerUtil.h"
#include "../../../model/NavigationModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

UserDetail::UserDetail()
:m_pLabelUserName(NULL)
,m_pLabelLevel(NULL)
,m_pLabelPoints(NULL)
,m_pLabelCoins(NULL)
,m_pLabelPower(NULL)
,m_pLabelVip(NULL)
,m_pLabelPowerFull(NULL)
,m_pLabelPowerRecover(NULL)
,m_pCloseButton(NULL)
,m_pButtonRename(NULL)
,m_pButtonVip(NULL)
,m_pContainer(NULL)
,m_pDelegate(NULL)
,m_pUserInfo(NULL)
,m_pPrivileges(NULL)
,m_pTableView(NULL)
,m_pNodePrivileges(NULL)
,m_pLabelEndu(NULL)
,m_pLabelEnduFull(NULL)
,m_pLabelEnduRecover(NULL)
{
	
}

UserDetail::~UserDetail()
{
	CC_SAFE_RELEASE(m_pLabelUserName);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelPoints);
	CC_SAFE_RELEASE(m_pLabelCoins);
	CC_SAFE_RELEASE(m_pLabelPower);
    CC_SAFE_RELEASE(m_pLabelVip);
	CC_SAFE_RELEASE(m_pLabelPowerFull); 
	CC_SAFE_RELEASE(m_pLabelPowerRecover);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pButtonRename);
	CC_SAFE_RELEASE(m_pButtonVip); 
	CC_SAFE_RELEASE(m_pContainer); 
	CC_SAFE_RELEASE(m_pPrivileges);
	CC_SAFE_RELEASE(m_pTableView);
	CC_SAFE_RELEASE(m_pNodePrivileges);
	CC_SAFE_RELEASE(m_pLabelEndu);
	CC_SAFE_RELEASE(m_pLabelEnduFull);
	CC_SAFE_RELEASE(m_pLabelEnduRecover);
}

bool UserDetail::init()
{
	if (!DPopup::init())
	{
		return false;
	}
    
	//CCB_READER_AND_ADDCHILD("ui/ccb/userDetail");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_wanjiaxinxi/wanjiaxinxi");
	m_pPrivileges = CCArray::create();
	CC_SAFE_RETAIN(m_pPrivileges);

	m_pCloseButton->getButton()->setTarget(this, menu_selector(UserDetail::callBackClose));
	m_pButtonRename->setTarget(this, menu_selector(UserDetail::callBackRename));
	m_pButtonVip->setTarget(this, menu_selector(UserDetail::callBackVip));
	m_pUserInfo = DM_GET_USER_MODEL->getUserInfo();

	//m_pTableView = DTableView::create(this, m_pContainer->getContentSize());
	//m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	//m_pTableView->setVerticalFillOrder(kCCTableViewFillBottomUp);
	//m_pTableView->setDelegate(this);
	//CC_SAFE_RETAIN(m_pTableView);
	//m_pContainer->addChild(m_pTableView);

	if (!DM_GET_USER_MODEL->getPrivilegesSynced())
	{
		HTTP_CLIENT->userPrivileges();
	}

	updateInfo();
    return true;
}

void UserDetail::onEnter()
{
	TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
	TimerUtil::sharedTimerUtil()->schedule(schedule_selector(UserDetail::updatePrivileges), this, 60);
	TimerUtil::sharedTimerUtil()->schedule(schedule_selector(UserDetail::countDownUpdate), this, 1);
	//BINDING_ONLY(m_pUserInfo, this, UserDetail::updateInfo, D_EVENT_USERINFO_UPDATE);
	BINDING_EXEC(DM_GET_USER_MODEL, this, UserDetail::updatePrivileges, D_EVENT_GET_PRIVILEGES);
	DPopup::onEnter();
}

void UserDetail::onExit()
{
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
	//UNBINDING(m_pUserInfo, this, D_EVENT_USERINFO_UPDATE);
	UNBINDING(DM_GET_USER_MODEL, this, D_EVENT_GET_PRIVILEGES);
	DPopup::onExit();
}

void UserDetail::updatePrivileges()
{
	//DM_GET_USER_MODEL->getPrivileges(m_pPrivileges);
	//updatePrivileges(0);
}

void UserDetail::updatePrivileges(float dt)
{
	//m_pTableView->reloadData();
	//m_pNodePrivileges->setVisible(m_pPrivileges->count());
}

bool UserDetail::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelUserName", CCLabelTTF *, m_pLabelUserName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelLevel", CCLabelBMFont *, m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelPoints", CCLabelBMFont *, m_pLabelPoints);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelCoins", CCLabelBMFont *, m_pLabelCoins);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelPower", CCLabelBMFont*, m_pLabelPower);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelVip", CCLabelBMFont*, m_pLabelVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelPowerFull", CCLabelBMFont *, m_pLabelPowerFull);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelPowerRecover", CCLabelBMFont*, m_pLabelPowerRecover);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton*, m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonRename", CCMenuItem *, m_pButtonRename);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonVip", CCMenuItem *, m_pButtonVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodePrivileges", CCNode *, m_pNodePrivileges);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelEndu", CCLabelBMFont*, m_pLabelEndu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelEnduFull", CCLabelBMFont*, m_pLabelEnduFull);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelEnduRecover", CCLabelBMFont*, m_pLabelEnduRecover);
	
	return false;
}

void UserDetail::updateInfo()
{
	user::UserInfo * pUserInfo = DM_GET_USER_MODEL->getUserInfo();
	m_pLabelUserName->setString(pUserInfo->getUserName().c_str());
	m_pLabelLevel->setString(D_CSTRING_FROM_UINT(pUserInfo->getUserLv()));
	m_pLabelPoints->setString(D_CSTRING_FROM_UINT(pUserInfo->getPoints()));
	m_pLabelCoins->setString(D_CSTRING_FROM_UINT(pUserInfo->getCoins()));
	m_pLabelPower->setString(D_CSTRING_FROM_UINT(pUserInfo->getPower()));
	m_pLabelEndu->setString(D_CSTRING_FROM_UINT(pUserInfo->getEndurance()));
	m_pLabelVip->setString(D_CSTRING_FROM_UINT(pUserInfo->getVipLevel()));

	unsigned int rectime = pUserInfo->getPowerRecTime();
	unsigned int fulltime = pUserInfo->getPowerFullTime();

	m_pLabelPowerRecover->setString(D_CSTRING_TIME_FROM_UINT(rectime));	
	m_pLabelPowerFull->setString(D_CSTRING_TIME_FROM_UINT(fulltime));
	m_pLabelEnduRecover->setString(D_CSTRING_TIME_FROM_UINT(pUserInfo->getEnduRecTime()));
	m_pLabelEnduFull->setString(D_CSTRING_TIME_FROM_UINT(pUserInfo->getEnduFullTime()));
}

void UserDetail::closePopup()
{
    DPopup::closePopup();
    //NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_CLOSE_LEVELUP_POPUP);
}

void UserDetail::callBackClose(CCObject *sender)
{
	closePopup();
}

void UserDetail::callBackRename(CCObject *sender)
{
// 	if (m_pDelegate)
// 	{
// 		m_pDelegate->willShowRenameView(this);
// 	}
	if (0 == DM_GET_ITEM_MODEL->getItemNum(ITEM_CHANGE_NAME_ID))
	{
		closePopup();
		//Tips_Alert(D_LOCAL_STRING("ItemNotEnough"));
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_ITEMS,CCInteger::create(ITEM_CHANGE_NAME_ID));		
		return;
	}
	
	addChild(ChangeName::create());
}

void UserDetail::callBackVip(CCObject *sender)
{
	//addChild(VipView::create());
	/*if (m_pDelegate)
	{
		m_pDelegate->willShowVipView(this);
	}*/

	Game::sharedGame()->NavigateTo(navigation::kNavVip);
	closePopup();
}


void UserDetail::countDownUpdate(float dt)
{
	//每秒更新体力/耐力
	updateInfo();
}

unsigned int UserDetail::numberOfCellsInTableView(CCTableView *table)
{
	return m_pPrivileges->count();
}

CCTableViewCell *UserDetail::tableCellAtIndex(CCTableView *table, unsigned int idx)
{

	UserDetailPrivilegeCell *cell = (UserDetailPrivilegeCell*)table->dequeueCell();
	if (!cell)
	{
		cell = UserDetailPrivilegeCell::create();
	}
	user::Privilege *pData = (user::Privilege *)m_pPrivileges->objectAtIndex(idx);
	cell->setData(pData);
	return cell;
}

CCSize UserDetail::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return ccp(640, 70);
}

UserDetailPrivilegeCell::UserDetailPrivilegeCell()
	:m_pContents(NULL)
	,m_pEndTs(NULL)
	,m_pData(NULL)
{

}

UserDetailPrivilegeCell::~UserDetailPrivilegeCell()
{
	CC_SAFE_RELEASE(m_pContents);
	CC_SAFE_RELEASE(m_pEndTs);
	CC_SAFE_RELEASE(m_pData);
}

bool UserDetailPrivilegeCell::init()
{
	if (!CCTableViewCell::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("ui/ccb/userDetailPowerCell");
	return true;
}

void UserDetailPrivilegeCell::setData(user::Privilege *pData)
{
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
	m_pContents->setString(m_pData->getContent().c_str());
	uint32_t restTime = 0;
	if (pData->getEndTs() > GameManager::sharedGameManager()->getServerTime())
	{
		restTime = pData->getEndTs() - GameManager::sharedGameManager()->getServerTime();
	}
	//CCString *timeStr = CCString::createWithFormat("%3u%6.2u%6.2u", restTime / 86400, restTime % 86400 / 3600, restTime % 3600 / 60);
	m_pEndTs->setString(CCString::createWithFormat("%s", TimeUtil::getSeconds2String(restTime, "%D:%H:%M","    "))->getCString());
}

bool UserDetailPrivilegeCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContents", CCLabelTTF *, m_pContents);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEndTs", CCLabelBMFont *, m_pEndTs);
	return false;
}