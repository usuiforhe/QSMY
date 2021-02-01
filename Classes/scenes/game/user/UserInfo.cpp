//  UserInfo.cpp
//
//  Created by cll on 13-4-1.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#include "UserInfo.h"
#include "../Game.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "../account/VipView.h"
#include "../inventory/ChangeName.h"

#include "../account/Account.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace user;


using namespace sp;

// singleton stuff
static UserInfoView *s_SharedUserInfoView = NULL;

UserInfoView* UserInfoView::sharedUserInfoView(void)
{
    if (!s_SharedUserInfoView)
    {
        s_SharedUserInfoView = new UserInfoView();
        s_SharedUserInfoView->init();
    }
    return s_SharedUserInfoView;
}

void UserInfoView::purgeUserInfoView(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedUserInfoView);
}

UserInfoView::UserInfoView()
:userInfo(NULL)
,m_eDisplayType(user::kUserInfoFull)
,m_pFullInfoCCBNode(NULL)
,m_pSimpleCCBNode(NULL)
,username_simple(NULL)
,points_simple(NULL)
,coins_simple(NULL)
,username(NULL)
,power(NULL)
,points(NULL)
,coins(NULL)
,exp(NULL)
,level(NULL)
,vipLevel(NULL)
,expline(NULL)
,m_pPBPower(NULL)
,m_pPBEndu(NULL)
,m_pBMFEndu(NULL)
,m_pBMFTime(NULL)
,m_pDiffs(NULL)
,m_pEffectNode(NULL)
,m_pPrivileges(NULL)
,m_pPowerNode(NULL)
,m_pEnduranceNode(NULL)
{
    
}

void UserInfoView::onEnter()
{
	BINDING_EXEC(userInfo,this,UserInfoView::onUserInfoUpdated,D_EVENT_USERINFO_UPDATE);

	BINDING_ONLY(DM_GET_ITEM_MODEL,this,UserInfoView::addPow,D_EVENT_ADD_POW);
	BINDING_ONLY(DM_GET_ITEM_MODEL,this,UserInfoView::addEndu,D_EVENT_ADD_ENDU);

	//BINDING_EXEC(DM_GET_USER_MODEL,this,UserInfoView::updatePrivileges,D_EVENT_GET_PRIVILEGES);
    DLayer::onEnter();
}


void UserInfoView::onExit()
{
	UNBINDING_ALL(userInfo, this);
	UNBINDING_ALL(DM_GET_ITEM_MODEL, this);
    DLayer::onExit();
}

UserInfoView::~UserInfoView()
{
	CC_SAFE_RELEASE(username_simple);
	CC_SAFE_RELEASE(coins_simple);
	CC_SAFE_RELEASE(points_simple);
    CC_SAFE_RELEASE(username);
    CC_SAFE_RELEASE(power);
    CC_SAFE_RELEASE(points);
    CC_SAFE_RELEASE(coins);
    CC_SAFE_RELEASE(exp);
    CC_SAFE_RELEASE(level);
    CC_SAFE_RELEASE(vipLevel);
    CC_SAFE_RELEASE(expline);
    CC_SAFE_RELEASE(m_pPBPower);
	CC_SAFE_RELEASE(m_pPBEndu);
	CC_SAFE_RELEASE(m_pBMFEndu);
	CC_SAFE_RELEASE(m_pBMFTime);
    CC_SAFE_RELEASE(m_pDiffs);
	CC_SAFE_RELEASE(m_pEffectNode);
	CC_SAFE_RELEASE(m_pPrivileges);
	CC_SAFE_RELEASE(m_pPowerNode);
	CC_SAFE_RELEASE(m_pEnduranceNode);
}

bool UserInfoView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "username_simple", CCLabelTTF *, this->username_simple);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "coins_simple", CCLabelBMFont *, this->coins_simple);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "points_simple", CCLabelBMFont *, this->points_simple);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "username", CCLabelTTF *, this->username);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "power", CCLabelBMFont *, this->power);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "points", CCLabelBMFont *, this->points);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "coins", CCLabelBMFont *, this->coins);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "exp", CCLabelBMFont *, this->exp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "level", CCLabelBMFont *, this->level);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "vipLevel", CCLabelBMFont *, this->vipLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "expline", DProgressBar *, this->expline);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPBPower", DProgressBar *, this->m_pPBPower);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPBEndu", DProgressBar *, this->m_pPBEndu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFEndu", CCLabelBMFont *, this->m_pBMFEndu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFTime", CCLabelBMFont *, this->m_pBMFTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPowerNode", CCNode *, this->m_pPowerNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnduranceNode", CCNode *, this->m_pEnduranceNode);
    
    return false;
}

SEL_MenuHandler UserInfoView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallBack", UserInfoView::menuCallBack);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "userDetailsCallBack", UserInfoView::userDetailsCallBack);
    return NULL;
}

void UserInfoView::menuCallBack(cocos2d::CCObject* obj)
{
    CCMenuItem* pItem = (CCMenuItem*)obj;
    int tag = pItem->getTag();
    if(tag==-1)
    {
        CCLOG("error nav");
        return;
    }
    Game::sharedGame()->NavigateTo((navigation::NavigationType)tag);
}

void UserInfoView::onUserInfoUpdated()
{
    updateUserInfo();
}

void UserInfoView::removeFromParentAndCleanup(bool cleanup)
{
    DLayer::removeFromParentAndCleanup(cleanup);
	setDisplayType(user::kUserInfoFull);
    setZOrder(0);//重置zorder
}

bool UserInfoView::init()
{
    
    if ( !DLayer::init() )
    {
        return false;
    }

    userInfo = DM_GET_USER_MODEL->getUserInfo();
    CCB_READER("PL_ui/ccb/pl_home/PL_Home_shang", m_pFullInfoCCBNode);
	if (m_pFullInfoCCBNode)
	{
		addChild(m_pFullInfoCCBNode);
	}

	CCB_READER("PL_ui/ccb/pl_common/pl_qianghua_top", m_pSimpleCCBNode);
	if (m_pSimpleCCBNode)
	{
		addChild(m_pSimpleCCBNode);
		m_pSimpleCCBNode->setVisible(false);
	}
    


    CC_SAFE_RELEASE(m_pDiffs);
    m_pDiffs = CCArray::create();
    CC_SAFE_RETAIN(m_pDiffs);
	/*
	Armature *pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("iconmall",sp::ZORDER);
	m_pEffectNode->addChild(pArmature);
	*/

	PL_MOVE_NODE_BELOW_CENTER_Y(this);

    return true;
}

void UserInfoView::addPow()
{
	m_pPowerNode->removeAllChildren();
	sp::Armature* m_pEnhance =  ResourceManager::sharedResourceManager()->getUIAnimate("adding",sp::ZORDER);
	m_pEnhance->getAnimation()->playByIndex(0);
	m_pPowerNode->addChild(m_pEnhance);
}

void UserInfoView::addEndu()
{
	m_pEnduranceNode->removeAllChildren();
	sp::Armature* m_pEnhance =  ResourceManager::sharedResourceManager()->getUIAnimate("adding",sp::ZORDER);
	m_pEnhance->getAnimation()->playByIndex(0);
	m_pEnduranceNode->addChild(m_pEnhance);
}

void UserInfoView::updateUserInfo()
{
    if(!userInfo || userInfo->getUserLv()==0 || userInfo->getLocked()) return;

	switch(m_eDisplayType)
	{
	case kUserInfoFull:
		updateFullInfo();
		break;
	case kUserInfoSimple:
		updateSimpleInfo();
		break;
	default:
		CCLOG("error userinfo m_eDisplayType");
		return;
	}

    
}

void UserInfoView::updatePrivileges()
{
	CCArray *pArr = CCArray::create();
	DM_GET_USER_MODEL->getPrivileges(pArr);
	m_pPrivileges->setVisible(pArr->count() > 0);
}

void UserInfoView::diffValue(user::UserInfoDiff* pInfo)
{
    if(m_pDiffs->count()<=0) return;
    UserInfoDiff* pDiff = NULL;
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(m_pDiffs, obj)
    {
        pDiff = static_cast<UserInfoDiff*>(obj);
        pInfo->iCoins -= pDiff->iCoins;
        pInfo->iPower -= pDiff->iPower;
        pInfo->iUserExp -= pDiff->iUserExp;
        pInfo->iPoints -= pDiff->iPoints;
    }
    
}

void UserInfoView::addDiff(user::UserInfoDiff* pInfo)
{
    if(pInfo==NULL) return;
    m_pDiffs->addObject(pInfo);
    updateUserInfo();
}

void UserInfoView::removeDiff(user::UserInfoDiff* pInfo)
{
    if(pInfo==NULL) return;
    m_pDiffs->removeObject(pInfo);
    updateUserInfo();
}

void UserInfoView::userDetailsCallBack(cocos2d::CCObject* obj)
{
	UserDetail *pDetailView = UserDetail::create();
	pDetailView->setDelegate(this);
	Game::sharedGame()->openPopup(pDetailView);
}

void UserInfoView::willShowVipView(UserDetail *pDetailView)
{
// 	Account *pAccount = Account::create();
// 	pAccount->showInfo(kAccountTypeVipView);
// 	pAccount->setPopupDelegate(this);
// 	Game::sharedGame()->openPopup(pAccount);
}

void UserInfoView::onPopupClose(DPopup* pPopup)
{
	if (dynamic_cast<Account *>(pPopup))
	{
		userDetailsCallBack(NULL);
	}
}

void UserInfoView::onPopupBack(DPopup* pPopup)
{
	if (dynamic_cast<Account *>(pPopup))
	{
		userDetailsCallBack(NULL);
	}
	pPopup->removeFromParent();
}

void UserInfoView::setDisplayType( user::UserInfoDisplayType type )
{
	if (m_eDisplayType == type)
	{
		return;
	}
	
	switch(type)
	{
	case kUserInfoFull:
		m_pFullInfoCCBNode->setVisible(true);
		m_pSimpleCCBNode->setVisible(false);
		break;
	case kUserInfoSimple:
		m_pSimpleCCBNode->setVisible(true);
		m_pFullInfoCCBNode->setVisible(false);
		break;
	default:
		CCLOG("error UserInfoDisplayType,%d ", type);
		return;
	}

	m_eDisplayType = type;
	updateUserInfo();
}

void UserInfoView::updateFullInfo()
{
	user::UserBase * pUserBase = DM_GET_USER_MODEL->getUserBaseByLv(userInfo->getUserLv());

	CCAssert(pUserBase!=NULL, "用户配置表没有对应等级");

	UserInfoDiff* pInfo = UserInfoDiff::create();
	pInfo->iCoins = userInfo->getCoins();
	pInfo->iPower = userInfo->getPower();
	pInfo->iUserExp = userInfo->getUserExp();
	pInfo->iPoints = userInfo->getPoints();

	diffValue(pInfo);

	CCString * str_power = CCString::createWithFormat("%d/%d", pInfo->iPower, pUserBase->getMaxPower());
	CCString * str_endu = CCString::createWithFormat("%d/%d", userInfo->getEndurance(), pUserBase->getMaxEndu());
	CCString * str_exp = CCString::createWithFormat("%d/%d",userInfo->getUserExp()-pUserBase->getTotalExp(),pUserBase->getMaxExp());

	CCString * str_points = CCString::createWithFormat("%d",pInfo->iPoints);
	CCString * str_coins = CCString::createWithFormat("%d",pInfo->iCoins);
	CCString * str_level = CCString::createWithFormat("%d",userInfo->getUserLv());
	CCString * str_vipLevel = CCString::createWithFormat("%d",userInfo->getVipLevel());

	if(username)
	{
		username->setString(userInfo->getUserName().c_str());
	}

	if(power)
	{
		power->setString(str_power->getCString());
	}

	if(m_pBMFEndu)
	{
		m_pBMFEndu->setString(str_endu->getCString());
	}

	if(points)
	{
		points->setString(str_points->getCString());
	}

	if (m_pBMFTime)
	{
		m_pBMFTime->setString(TimeUtil::formatTime(GameManager::sharedGameManager()->getServerTime(), "%H:%M"));
	}
	
	if (coins)
	{
		coins->setString(str_coins->getCString());
	}
	
	//exp->setString(str_exp->getCString());
	if (level)
	{
		level->setString(str_level->getCString());
	}

	if (vipLevel)
	{
		vipLevel->setString(str_vipLevel->getCString());
	}

	float percent = 0.0f;
	static float delay= 0.f;
	if (expline)
	{
		percent = (float)(pInfo->iUserExp-pUserBase->getTotalExp())/pUserBase->getMaxExp();
		expline->setPercent(percent,delay);
	}

	if (m_pPBPower)
	{
		percent = (float)pInfo->iPower/pUserBase->getMaxPower();
		if (percent > 1.0)
		{
			percent = 1.0;
		}
		m_pPBPower->setPercent(percent, delay);
	}

	if (m_pPBEndu)
	{
		percent = (float)userInfo->getEndurance() / pUserBase->getMaxEndu();
		if (percent > 1.0)
		{
			percent = 1.0;
		}
		m_pPBEndu->setPercent(percent, delay);
	}

	D_FLOAT_EQUALS(delay, 0.f)?delay=0.3f:0.f;
}

void UserInfoView::updateSimpleInfo()
{
	user::UserBase * pUserBase = DM_GET_USER_MODEL->getUserBaseByLv(userInfo->getUserLv());

	CCAssert(pUserBase!=NULL, "用户配置表没有对应等级");

	UserInfoDiff* pInfo = UserInfoDiff::create();
	pInfo->iCoins = userInfo->getCoins();
	pInfo->iPower = userInfo->getPower();
	pInfo->iUserExp = userInfo->getUserExp();
	pInfo->iPoints = userInfo->getPoints();

	diffValue(pInfo);

	CCString * str_points = CCString::createWithFormat("%d",pInfo->iPoints);
	CCString * str_coins = CCString::createWithFormat("%d",pInfo->iCoins);

	if (username_simple)
	{
		username_simple->setString(userInfo->getUserName().c_str());
	}

	if (coins_simple)
	{
		coins_simple->setString(str_coins->getCString());
	}

	if (points_simple)
	{
		points_simple->setString(str_points->getCString());
	}
}