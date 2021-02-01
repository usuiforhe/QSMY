//
//  Welcome.h
//  登陆奖励
//
//  Created by ldr123 on 13-07-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Welcome.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "model/LoginRewardModel.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
using namespace sp;

Welcome::Welcome()
{
	m_pMenuItem = NULL;
	m_pIconContainer = NULL;
	m_pContent = NULL;
}

Welcome::~Welcome()
{
	CC_SAFE_RELEASE(m_pMenuItem);
	CC_SAFE_RELEASE(m_pIconContainer);
	CC_SAFE_RELEASE(m_pContent);
}

bool Welcome::init()
{
	if (!DPopup::init(true))
	{
		return false;
	}

    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_huanying/PL_huanying");
	m_pMenuItem->setTarget(this,menu_selector(Welcome::onTouchBack));

	//读表
	loginreward::WelcomeData* pData = DM_GET_LOGIN_REWARD_MODEL->getWelcomeData();
	if(pData)
	{
		m_pContent->setString(pData->m_content.c_str());
		CCString* path = CCString::createWithFormat("welcome/%s" , pData->m_soundPath.c_str());
		CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, path);

		CCString* iconpath = CCString::createWithFormat("PL_ui/tutorial/%d.png" , pData->m_imgIndex);
		CCSprite* icon = CCSprite::create(iconpath->getCString());
		if(icon)m_pIconContainer->addChild(icon);
	}
	return true;
}

bool Welcome::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuItem",			CCMenuItem *,	m_pMenuItem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer",				CCNode *,	m_pIconContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContent",				CCRichLabelTTF *,	m_pContent);
	return false;
}

void Welcome::onTouchBack(cocos2d::CCObject* p)
{
    if (m_pMenuItem->isVisible()) {
        closePopup();
    }
}

void Welcome::onBackKey()
{
	closePopup();
}