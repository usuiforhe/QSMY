//
//  OnlineAward.cpp
//  账户余额不足
//
//  Created by cll on 13-6-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "OnlineAward.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "../Game.h"
#include "../../../utils/TimeUtil.h"
#include "../equip/EquipInfoView.h"
#include "../inventory/ItemInfoView.h"
//#include "../warrior/WarriorInfo.h"
#include "utils/TimerUtil.h"
USING_NS_CC;


/************************ OnlineAward class begin*******************/


OnlineAward::OnlineAward()
	:m_pHeroContainer(NULL)
	,m_pGradeContainer(NULL)
	,m_pPopupLayer(NULL)
	,pWarriorBase(NULL)
	,m_pTime(NULL)
	,m_pTimeUp(NULL)
	,m_pCloseButton(NULL)
	,m_pConfirmButton(NULL)
	,m_pItemInfoView(NULL)
{
	for (uint8_t i =0; i<AWARD_COUNT; ++i)
	{
		m_pAward[i] = NULL;
	}
}

OnlineAward::~OnlineAward()
{
	CC_SAFE_RELEASE(m_pHeroContainer);
	CC_SAFE_RELEASE(m_pGradeContainer);
	CC_SAFE_RELEASE(m_pTime);
	CC_SAFE_RELEASE(m_pTimeUp);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pItemInfoView);

	for (uint8_t i =0; i<AWARD_COUNT; ++i)
	{
		CC_SAFE_RELEASE(m_pAward[i]);
	}
}

bool OnlineAward::init()
{
	if(!DPopup::init(true))
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zaixianlibao/pl_zaixianlibao");

	m_pCloseButton->getButton()->setTarget(this, menu_selector(OnlineAward::closeCallBack));
	m_pConfirmButton->getButton()->setTarget(this, menu_selector(OnlineAward::confirmCallBack));

	unsigned int timesToShow = DM_GET_ACTIVITY_MODEL->getOnlineAwardTimes() + 1;
	CCDictionary* pList = DM_GET_ACTIVITY_MODEL->getOnlineAwardList();
	activity::OnlineAwardInfo* pInfo = dynamic_cast<activity::OnlineAwardInfo*>(pList->objectForKey(timesToShow));
	CCArray* pAwardList = pInfo->getDropList();

	CCObject* obj = NULL;
	unsigned int index = 0;
	CCARRAY_FOREACH(pAwardList, obj)
	{
		drop::Drop* pDrop = dynamic_cast<drop::Drop*>(obj);
		m_pAward[index]->updateView(pDrop);
		index++;
	}

	m_pPopupLayer = CCNode::create();
	PL_MOVE_WITH_RESOLUTION_DIFF(m_pPopupLayer , -1);
	addChild(m_pPopupLayer);

	//处理倒计时
	unsigned int countdown = DM_GET_ACTIVITY_MODEL->getOnlineAwardCountdown();
	m_pConfirmButton->getButton()->setEnabled(countdown == 0);
	if(countdown == 0)
	{
		//当前就可以领取
		m_pTimeUp->setVisible(true);
		m_pTime->setVisible(!m_pTimeUp->isVisible());
	}
	else{
		m_pTimeUp->setVisible(false);
		m_pTime->setVisible(!m_pTimeUp->isVisible());
		//倒计时
		m_pTime->setString(TimeUtil::getSeconds2String(countdown , "%H:%M:%S",":", "%02d", true));
		TimerUtil::sharedTimerUtil()->schedule(schedule_selector(OnlineAward::refreshOnlineAward), this , 1);
	}

	return true;

}

void OnlineAward::onExit()
{
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
}

void OnlineAward::refreshOnlineAward(float dt)
{
	unsigned int countdown = DM_GET_ACTIVITY_MODEL->getOnlineAwardCountdown();
	m_pTime->setString(TimeUtil::getSeconds2String(countdown , "%H:%M:%S",":", "%02d", true));
	m_pConfirmButton->getButton()->setEnabled(countdown == 0);
	if(countdown == 0)
	{
		m_pTimeUp->setVisible(true);
		m_pTime->setVisible(!m_pTimeUp->isVisible());
		TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
	}
}

bool OnlineAward::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHeroContainer", CCNode *, this->m_pHeroContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeContainer", CCNode *, this->m_pGradeContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTime", CCLabelTTF *, this->m_pTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTimeUp", CCNode *, this->m_pTimeUp);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, this->m_pConfirmButton);

	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pAward",AwardItemSmall*,this->m_pAward,AWARD_COUNT);

	return false;
}

void OnlineAward::closeCallBack(cocos2d::CCObject *obj)
{
	closePopup();
}

void OnlineAward::confirmCallBack(cocos2d::CCObject *obj)
{
	HTTP_CLIENT->getOnlineAward(this,callfuncJson_selector(OnlineAward::onOnlineAward));
}

void OnlineAward::onOnlineAward(const Json::Value &requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
	closePopup();
}
