//
//  UserLevelup.h
//  用户升级弹出框
//
//  Created by cll on 13-07-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "UserLevelup.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "managers/TipsManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

UserLevelup::UserLevelup()
:m_award1Num(NULL)
,m_award2Num(NULL)
,m_maxPower(NULL)
,m_unlockWarriorNum(NULL)
,m_pConfirmNode(NULL)
//,m_pEffectNode(NULL)
//,m_pLightEffectNode(NULL)
,m_pLevel(NULL)
,m_pEndurance(NULL)
,m_pFormationNode(NULL)
,m_pConfirmButton(NULL)
,m_pFormationButton(NULL)
{
	
}

UserLevelup::~UserLevelup()
{
	CC_SAFE_RELEASE(m_award1Num);               //奖励元宝数	-- 对应用户iPoints
	CC_SAFE_RELEASE(m_award2Num);               //奖励银币数	-- 对应用户iCoins
	CC_SAFE_RELEASE(m_maxPower);				//最大体力
	CC_SAFE_RELEASE(m_unlockWarriorNum);		//最大上阵弟子
	CC_SAFE_RELEASE(m_pEndurance);

 //   CC_SAFE_RELEASE(m_pEffectNode);             //效果
    CC_SAFE_RELEASE(m_pConfirmNode);
 //   CC_SAFE_RELEASE(m_pLightEffectNode);        //光效果
    CC_SAFE_RELEASE(m_pLevel);

	CC_SAFE_RELEASE(m_pFormationNode);
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pFormationButton);
}

bool UserLevelup::init()
{
	if (!DPopup::init(true))
	{
		return false;
	}
    return true;
}

bool UserLevelup::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_award1Num",			CCLabelBMFont *,			m_award1Num);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_award2Num",			CCLabelBMFont *,			m_award2Num);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_maxPower",				CCLabelBMFont *,			m_maxPower);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_unlockWarriorNum",		CCLabelBMFont *,		m_unlockWarriorNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel",               CCLabelBMFont *,		m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEndurance", CCLabelBMFont*, m_pEndurance);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmNode" , CCNode*, m_pConfirmNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFormationNode" , CCNode*, m_pFormationNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton*, m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFormationButton", DButton*, m_pFormationButton);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmNode",			CCNode*,                m_pConfirmNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode",			CCNode *,                   m_pEffectNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLightEffectNode",		CCNode *,           m_pLightEffectNode);
    
	
	return false;
}

SEL_MenuHandler UserLevelup::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	/*CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onConfirmBut", UserLevelup::onConfirmBut);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onNavBut", UserLevelup::onNavBut);*/
	return NULL;
}

void UserLevelup::onEnter()
{
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_jssj/PL_jssj");
	updateInfo();

	//m_pEndurance->setString("40");
	m_pConfirmButton->getButton()->setTarget(this, menu_selector(UserLevelup::onConfirmBut));
	m_pFormationButton->getButton()->setTarget(this, menu_selector(UserLevelup::onNavBut));
	CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, DM_GET_CONFIG_MODEL->getConfigByKey(LEVEL_UP_SOUND_KEY));
    DPopup::onEnter();
}

void UserLevelup::updateInfo()
{
	user::UserInfo * pUserInfo = DM_GET_USER_MODEL->getUserInfo();
	user::UserBase * pUserBase = DM_GET_USER_MODEL->getUserBaseByLv(pUserInfo->getUserLv());
	user::UserUpAward* pUserUpAward = DM_GET_USER_MODEL->getUserUpAwardByLv(pUserInfo->getUserLv());
    
	if (!pUserUpAward)
	{
		Tips_Error("升级奖励信息配置错误");
		return;
	}
	
	m_award1Num->setString(CCString::createWithFormat("%d",pUserUpAward->getPoints())->getCString());
	m_award2Num->setString(CCString::createWithFormat("%d",pUserUpAward->getCoins())->getCString());
	m_maxPower->setString(CCString::createWithFormat("%d",pUserBase->getMaxPower())->getCString());
	m_pEndurance->setString(CCString::createWithFormat("%d",pUserBase->getMaxEndu())->getCString());

	m_unlockWarriorNum->setString(CCString::createWithFormat("%d",pUserUpAward->getUnlockPosNum())->getCString());
  /*  m_pEffectNode->removeAllChildren();
    m_pEffectNode->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("userlevelup"));*/
    
    m_pLevel->setString(CCString::createWithFormat("%d",pUserUpAward->getUserLv())->getCString());
    
 /*   m_pLightEffectNode->removeAllChildren();
    m_pLightEffectNode->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("levelup"));*/
    
    if(DM_GET_USER_MODEL->getUserUpAwardByLv(pUserInfo->getUserLv())->getUnlockPosNum()>DM_GET_USER_MODEL->getUserUpAwardByLv(pUserInfo->getUserLv()-1)->getUnlockPosNum())
    {
        m_pFormationNode->setVisible(true);
        m_pConfirmNode->setVisible(false);
    }else
    {
        m_pFormationNode->setVisible(false);
        m_pConfirmNode->setVisible(true);
    }
}

void UserLevelup::onNavBut(cocos2d::CCObject* obj)
{
    closePopup();
    Game::sharedGame()->NavigateTo(navigation::kNavChangeFormation);
}

void UserLevelup::onConfirmBut(cocos2d::CCObject* obj)
{
	closePopup();
}

void UserLevelup::closePopup()
{
	DPopup::closePopup();
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_CLOSE_LEVELUP_POPUP);
}


void UserLevelup::onBackKey()
{
	closePopup();
}