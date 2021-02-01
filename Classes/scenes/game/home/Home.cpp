//  HomeView.cpp
//
//  Created by cll on 13-3-22.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#include "Home.h"
#include "network/HttpHandler.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "../user/UserInfo.h"
#include "HomeMenu.h"
#include "utils/TimerUtil.h"
#include "FooterMenu.h"
#include "../signin/Signin.h"

#include "../account/FirstRecharge.h"
#include "OnlineAward.h"
#include "WeeklyMenu.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace navigation;

#define D_EVENT_HOME_MENU "home menu nav"
#define HOME_DIALOG_POSITION ccp(343,420)

bool HomeView::m_bMenuShowing = false;

HomeView::HomeView()
:m_pDelegate(NULL)
,m_pActivenessModel(NULL)
,m_pNavigationModel(NULL)
,m_pHeroContainer(NULL)
,m_pActiveness(NULL)
,m_pSettingMenuItem(NULL)
,m_pAnnounceMenuItem(NULL)
,m_pMailMenuItem(NULL)
,m_pSepcialActivityMenuItem(NULL)
,m_pToggleMenuItem(NULL)
,m_pMenuBg(NULL)
,m_pNewMailLabel(NULL)
,m_pNewMailBg(NULL)
,m_pRemindContainer(NULL)
,m_pQualifiedPrizeNum(NULL)
,m_pMailModel(NULL)
,m_pActivityModel(NULL)
//,m_bMenuShowing(false)
,m_pNewRemindBg(NULL)
,m_pNewRemindLabel(NULL)
,m_pGetaward(NULL)
//,m_pChargeAward(NULL)
//,m_pChargeRemindContainer(NULL)
//,m_pChargeQualifiedPrizeNum(NULL)
,m_pDialogEffect(NULL)
,m_pMenuTouchHelper(NULL)
,m_pNavRect(NULL)
,m_pStrategyMenuItem(NULL)
,m_pEffectNode(NULL)
,m_pActivenessDing(NULL)
,m_pLotteryDing(NULL)
,m_pLotteryModel(NULL)
,m_pReCharge(NULL)
,m_pLeftIconsContainer(NULL)
,m_pArmaturePve(NULL)
,m_pArmatureExplore(NULL)
,m_pArmatureGuild(NULL)
,m_pArmatureBoss(NULL)
,m_pPveEffect(NULL)
,m_pExploreEffect(NULL)
,m_pGuildEffect(NULL)
,m_pBossEffect(NULL)
,m_pNodeXueNv(NULL)
,m_pNodeFirst(NULL)
,m_pNodeCharged(NULL)
,m_pFreeCardBg(NULL)
,m_pSpecialText(NULL)
,m_pFreeCardLabel(NULL)
,m_pNewSignBg(NULL)
,m_pSignFlash(NULL)
,m_pOnlineFlash(NULL)
,m_pSignImageNode(NULL)
,m_pOnlineImageNode(NULL)
,m_pCountdown(NULL)
,m_pOnlineNode(NULL)
,m_pOnlineImage(NULL)
,m_pOnlineMenu(NULL)
,m_pHeroShowView(NULL)
,m_pWeeklyMenu(NULL)
{
    D_RESET_C_ARRAY(m_pMenuContainer, MENU_COUNT);
}

HomeView::~HomeView()
{
    m_bMenuShowing = false;
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
	CC_SAFE_RELEASE(m_pActiveness);
    CC_SAFE_RELEASE(m_pHeroContainer);
    CC_SAFE_RELEASE(m_pSettingMenuItem);
    CC_SAFE_RELEASE(m_pAnnounceMenuItem);
    CC_SAFE_RELEASE(m_pMailMenuItem);
    CC_SAFE_RELEASE(m_pSepcialActivityMenuItem);
    CC_SAFE_RELEASE(m_pToggleMenuItem);
    CC_SAFE_RELEASE(m_pMenuBg);
    CC_SAFE_RELEASE(m_pNewMailLabel);
    CC_SAFE_RELEASE(m_pNewMailBg);
    CC_SAFE_RELEASE(m_pRemindContainer);
    CC_SAFE_RELEASE(m_pQualifiedPrizeNum);
    CC_SAFE_RELEASE(m_pNewRemindLabel);
    CC_SAFE_RELEASE(m_pNewRemindBg);
    D_SAFE_RELEASE_C_ARRAY(m_pMenuContainer, MENU_COUNT);
    CC_SAFE_RELEASE(m_pGetaward);
    
    CC_SAFE_RELEASE(m_pReCharge);
    CC_SAFE_RELEASE(m_pLeftIconsContainer);
    //CC_SAFE_RELEASE(m_pChargeAward);
    //CC_SAFE_RELEASE(m_pChargeRemindContainer);
    //CC_SAFE_RELEASE(m_pChargeQualifiedPrizeNum);
    CC_SAFE_RELEASE(m_pMenuTouchHelper);
    CC_SAFE_RELEASE(m_pNavRect);
    CC_SAFE_RELEASE(m_pStrategyMenuItem);
    CC_SAFE_RELEASE(m_pEffectNode);
    
    CC_SAFE_RELEASE(m_pActivenessDing);
    CC_SAFE_RELEASE(m_pLotteryDing);
    
	CC_SAFE_RELEASE(m_pPveEffect);
	CC_SAFE_RELEASE(m_pExploreEffect);
	CC_SAFE_RELEASE(m_pGuildEffect);
	CC_SAFE_RELEASE(m_pBossEffect);
	CC_SAFE_RELEASE(m_pNodeXueNv);
	CC_SAFE_RELEASE(m_pNodeFirst);
	CC_SAFE_RELEASE(m_pNodeCharged);
	CC_SAFE_RELEASE(m_pFreeCardBg);
    CC_SAFE_RELEASE(m_pSpecialText);
	CC_SAFE_RELEASE(m_pFreeCardLabel);
	CC_SAFE_RELEASE(m_pNewSignBg);
	CC_SAFE_RELEASE(m_pSignFlash);
	CC_SAFE_RELEASE(m_pOnlineFlash);
	CC_SAFE_RELEASE(m_pSignImageNode);
	CC_SAFE_RELEASE(m_pOnlineImageNode);

	CC_SAFE_RELEASE(m_pCountdown);
	CC_SAFE_RELEASE(m_pOnlineNode);
	CC_SAFE_RELEASE(m_pOnlineImage);
	CC_SAFE_RELEASE(m_pOnlineMenu);
	CC_SAFE_RELEASE(m_pWeeklyMenu);
}


HomeView* HomeView::create(NavigationDelegate* pDelegate)
{
	HomeView *pRet = new HomeView();
    if (pRet && pRet->init(pDelegate))
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

bool HomeView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHeroContainer", CCNode *, this->m_pHeroContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNewMailLabel", CCLabelBMFont *, this->m_pNewMailLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNewMailBg", CCNode *, this->m_pNewMailBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFreeCardLabel", CCLabelBMFont *, this->m_pFreeCardLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFreeCardBg", CCNode *, this->m_pFreeCardBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpecialText", CCNode*, this->m_pSpecialText);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNewSignBg", CCNode *, this->m_pNewSignBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSignFlash", CCNode *, this->m_pSignFlash);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOnlineFlash", CCNode *, this->m_pOnlineFlash);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSignImageNode", CCNode *, this->m_pSignImageNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOnlineImageNode", CCNode *, this->m_pOnlineImageNode);
	
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCountdown", CCLabelBMFont *, this->m_pCountdown);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOnlineNode", CCNode *, this->m_pOnlineNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOnlineImage", CCMenuItemImage *, this->m_pOnlineImage);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOnlineMenu", CCMenuItem *, this->m_pOnlineMenu);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSepcialActivityMenuItem", CCMenuItemImage*, this->m_pSepcialActivityMenuItem);
    /*
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pActiveness", CCLabelBMFont *, this->m_pActiveness);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSettingMenuItem", CCMenuItemImage *, this->m_pSettingMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnnounceMenuItem", CCMenuItemImage *, this->m_pAnnounceMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMailMenuItem", CCMenuItemImage *, this->m_pMailMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pToggleMenuItem", CCMenuItemImage *, this->m_pToggleMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStrategyMenuItem", CCMenuItem*, this->m_pStrategyMenuItem);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuBg", CCLayerGradient *, this->m_pMenuBg);
    
   
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNewRemindLabel", CCLabelBMFont *, this->m_pNewRemindLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNewRemindBg", CCNode *, this->m_pNewRemindBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRemindContainer", CCNode *, this->m_pRemindContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pQualifiedPrizeNum", CCLabelBMFont *, this->m_pQualifiedPrizeNum);
	*/
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuContainer1", CCNode *, this->m_pMenuContainer[0]);
    
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuContainer2", CCNode *, this->m_pMenuContainer[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuContainer3", CCNode *, this->m_pMenuContainer[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuContainer4", CCNode *, this->m_pMenuContainer[3]);

    /*
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGetaward", CCNode *, this->m_pGetaward);
    
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChargeAward", CCNode *, this->m_pChargeAward);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChargeRemindContainer", CCNode *, this->m_pChargeRemindContainer);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChargeQualifiedPrizeNum", CCLabelBMFont *, this->m_pChargeQualifiedPrizeNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDialogEffect", CCNode *, this->m_pDialogEffect);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNavRect", CCNode *, this->m_pNavRect);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode *, this->m_pEffectNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pActivenessDing", CCNode *, this->m_pActivenessDing);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLotteryDing", CCNode *, this->m_pLotteryDing);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReCharge", CCNode *, this->m_pReCharge);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLeftIconsContainer", CCNode *, this->m_pLeftIconsContainer);
        
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPveEffect", CCNode *, this->m_pPveEffect);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExploreEffect", CCNode *, this->m_pExploreEffect);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuildEffect", CCNode *, this->m_pGuildEffect);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBossEffect", CCNode *, this->m_pBossEffect);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeXuenv", CCNode *, this->m_pNodeXueNv);
    */

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFirst", CCNode *, m_pNodeFirst);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeCharged", CCNode *, m_pNodeCharged);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWeeklyMenu",CCNode*,m_pWeeklyMenu);
    return false;
}

SEL_MenuHandler HomeView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallback", HomeView::menuCallback);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "toggleNav", HomeView::toggleNav);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "getAwardCallBack", HomeView::getAwardCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "reChargeCallBack", HomeView::reChargeCallBack);
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "chargeAwardCallBack", HomeView::chargeAwardCallBack);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackHome", HomeView::callBackHome);
    return NULL;
}

void HomeView::showNav(bool val)
{
    m_bMenuShowing = val;
    updateMenuDisplay(true);
}

void HomeView::hideNav()
{
    showNav(false);
}

void HomeView::changeSpecialVisible()
{
    if (m_pSepcialActivityMenuItem)
    {
        m_pSepcialActivityMenuItem->setVisible(DM_GET_ACTIVITY_MODEL->getSpecialActivityType() > 0);
        
        if (m_pSpecialText)
        {
            m_pSpecialText->setVisible(DM_GET_ACTIVITY_MODEL->getSpecialActivityType() > 0);
        }
    }
}

void HomeView::toggleNav(cocos2d::CCObject *sender)
{
    m_pStrategyMenuItem->setVisible(!D_CONFIG_INT_FOR_KEY(DISABLE_ZZB) && !DM_GET_PLATFORM_MODEL->getConfig().getStrategyHidden());
    showNav(!m_bMenuShowing);
}

void HomeView::menuCallback(CCObject * obj)
{
	CCMenuItemImage * pMenuItem = (CCMenuItemImage*) obj;
	int tag = pMenuItem->getTag();
    if(tag==-1)
    {
        CCLOG("home error nav id");
        return;
    }
	if(m_pDelegate)
		m_pDelegate->NavigateTo((NavigationType)tag);
}



void HomeView::updateMenuDisplay(bool animate)
{
    //m_pToggleMenuItem->setRotation(m_bMenuShowing?180:0);
    m_pToggleMenuItem->setEnabled(!m_bMenuShowing);
    changeSpecialVisible();
    if(!animate)
    {
        m_pSettingMenuItem->setScale(m_bMenuShowing?1:0);
        m_pAnnounceMenuItem->setScale(m_bMenuShowing?1:0);
        m_pStrategyMenuItem->setScale(m_bMenuShowing?1:0); 
        m_pMailMenuItem->setScale(m_bMenuShowing?1:0);
        m_pSepcialActivityMenuItem->setScale(m_bMenuShowing?1:0);
        m_pMenuBg->setScale(m_bMenuShowing?1:0);
        m_pNewMailBg->setScale(m_bMenuShowing?1:0);
        m_pNewRemindBg->setScale(m_bMenuShowing?0:1);
        
        
    }else
    {
        if(m_bMenuShowing)
        {
            m_pMenuTouchHelper->setEnabled(true);
            CCActionInterval* pAction = CCScaleTo::create(0.18f,1.f);

            m_pSettingMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.01f),CCEaseBackOut::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pStrategyMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.01f),CCEaseBackOut::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pAnnounceMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.02f),CCEaseBackOut::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pMailMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.03f),CCEaseBackOut::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pSepcialActivityMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.03f),CCEaseBackOut::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pMenuBg->runAction(CCSequence::create(CCDelayTime::create(0.f),D_CCCA(pAction),NULL));
            
            m_pNewMailBg->runAction(CCSequence::create(CCDelayTime::create(0.03f),CCEaseBackOut::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pNewRemindBg->runAction(CCSequence::create(CCDelayTime::create(0.03f),CCEaseBackOut::create(CCScaleTo::create(0.18f,0.f)),NULL));
            m_pToggleMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.03f),CCEaseBackOut::create(CCScaleTo::create(0.10f,0.f)),NULL));
        }else
        {
            m_pMenuTouchHelper->setEnabled(false);

            CCActionInterval* pAction = CCScaleTo::create(0.18f,0.f);
            
            m_pSettingMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.03f),CCEaseBackIn::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pStrategyMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.03f),CCEaseBackIn::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pAnnounceMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.02f),CCEaseBackIn::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pMailMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.01f),CCEaseBackIn::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pSepcialActivityMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.01f),CCEaseBackIn::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pMenuBg->runAction(CCSequence::create(CCDelayTime::create(0.03f),CCScaleTo::create(0.1f,0.f,1.f),NULL));
            
            m_pNewMailBg->runAction(CCSequence::create(CCDelayTime::create(0.03f),CCEaseBackIn::create((CCActionInterval*)D_CCCA(pAction)),NULL));
            m_pNewRemindBg->runAction(CCSequence::create(CCDelayTime::create(0.03f),CCEaseBackIn::create(CCScaleTo::create(0.18f,1.f)),NULL));
            m_pToggleMenuItem->runAction(CCSequence::create(CCDelayTime::create(0.03f),CCEaseBackIn::create(CCScaleTo::create(0.10f,1.f)),NULL));
        }
    }
}


bool MenuTouchHelper::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_rect.containsPoint(pTouch->getLocation())) return false;
    setEnabled(false);
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_HOME_MENU);
    return false;
}


bool HomeView::init(NavigationDelegate* pDelegate)
{
    
    if ( !DLayer::init() )
    {
        return false;
    }
    
    m_pMenuTouchHelper = MenuTouchHelper::create();
    CC_SAFE_RETAIN(m_pMenuTouchHelper);
    
    m_pActivenessModel = DM_GET_ACTIVENESS_MODEL;
    m_pNavigationModel = DM_GET_NAVIGATION_MODEL;
    m_pMailModel       = DM_GET_MAIL_MODEL;
    m_pLotteryModel    = DM_GET_LOTTERY_MODEL;
    m_pActivityModel = DM_GET_ACTIVITY_MODEL;
    m_pDelegate = pDelegate;

    
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_home/home");
	
	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView());	//添加用户基本信息

	//FooterMenuView::sharedFooterMenuView()->removeFromParentAndCleanup(false);
	//addChild(FooterMenuView::sharedFooterMenuView());	//添加页脚菜单

	//英雄对话节点
	m_pDialogEffect = CCNode::create();
	m_pDialogEffect->setPosition(HOME_DIALOG_POSITION);
	PL_MOVE_NODE_BELOW_CENTER_Y(m_pDialogEffect);
	m_pDialogEffect->setZOrder(100);
	addChild(m_pDialogEffect);

    m_pHeroShowView = HeroShowView::create();
    m_pHeroShowView->setDelegate(pDelegate);
    m_pHeroShowView->setDialogEffect(m_pDialogEffect);
    m_pHeroContainer->addChild(m_pHeroShowView);
    
    
    NavigationCat navs[] = {KNavCatPve,KNavCatPvp,KNavCatWarrior,KNavCatUnion};
    for(uint8_t i = 0;i < MENU_COUNT;++i)
    {
        m_pMenuContainer[i]->setTag(navs[i]);
    }
    
    //updateMenuDisplay(false);
    
    createHomeMenus();
    createWeeklyMenu();
    //m_pAnnounceMenuItem->setTarget(this, menu_selector(HomeView::showAnnounceCallBack));
    
    NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(HomeView::hideNav), D_EVENT_HOME_MENU);
    NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(HomeView::changeSpecialVisible), D_EVENT_SPECIAL_ACTIVI_CHANGED);
  
	/*
    m_pEffectNode->addChild(ResourceManager::sharedResourceManager()->getEffect(rand()%2==1?3:4));
    
    m_pActivenessDing->setVisible(false);
	sp::Armature *pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("activity_prompt");
	m_pActivenessDing->addChild(pArmature);

	m_pLotteryDing->setVisible(false);
	pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("activity_prompt");
	m_pLotteryDing->addChild(pArmature);

	m_pArmaturePve = ResourceManager::sharedResourceManager()->getUIAnimate("activeness");
	m_pArmaturePve->setPosition(m_pPveEffect->getPosition());
	m_pArmaturePve->setScaleX(m_pPveEffect->getScaleX());
	m_pArmaturePve->setScaleY(m_pPveEffect->getScaleY());
	m_pArmaturePve->setOpacity(0);
	addChild(m_pArmaturePve);


	m_pArmatureExplore = ResourceManager::sharedResourceManager()->getUIAnimate("activeness");
	m_pArmatureExplore->setPosition(m_pExploreEffect->getPosition());
	m_pArmatureExplore->setScaleX(m_pExploreEffect->getScaleX());
	m_pArmatureExplore->setScaleY(m_pExploreEffect->getScaleY());
    m_pArmatureExplore->setOpacity(0);
	addChild(m_pArmatureExplore);


	m_pArmatureGuild = ResourceManager::sharedResourceManager()->getUIAnimate("activeness");
	m_pArmatureGuild->setPosition(m_pGuildEffect->getPosition());
	m_pArmatureGuild->setScaleX(m_pGuildEffect->getScaleX());
	m_pArmatureGuild->setScaleY(m_pGuildEffect->getScaleY());
	m_pArmatureGuild->setOpacity(0);
	addChild(m_pArmatureGuild);


	m_pArmatureBoss = ResourceManager::sharedResourceManager()->getUIAnimate("activeness");
	m_pArmatureBoss->setPosition(m_pBossEffect->getPosition());
	m_pArmatureBoss->setScaleX(m_pBossEffect->getScaleX());
	m_pArmatureBoss->setScaleY(m_pBossEffect->getScaleY());
	m_pArmatureBoss->setOpacity(0);
	addChild(m_pArmatureBoss);

	if (isGuildBossRunning())
	{
		m_pArmatureGuild->setOpacity(255);
	}
	else
	{
		m_pArmaturePve->setOpacity(255);
	}
	*/
    changeSpecialVisible();
    return true;

}

void HomeView::showAnnounceCallBack(cocos2d::CCObject*)
{
    m_pMenuTouchHelper->setEnabled(false);
    toggleNav(NULL);
    if(m_pDelegate) m_pDelegate->showAnnounce();
}


void HomeView::createHomeMenus()
{
    for(uint8_t i = 0;i < MENU_COUNT;++i)
    {
        HomeMenu* pHomeMenu = HomeMenu::create(m_pNavigationModel->getNavigationNodeById(m_pMenuContainer[i]->getTag()));
        pHomeMenu->setDelegate(m_pDelegate);
        addChild(pHomeMenu);//保证所有按钮在同一容器下 方便调整zorder
		pHomeMenu->setTag(i);
        pHomeMenu->setPosition(m_pMenuContainer[i]->getPosition()); //编辑器的ccnode只用来确定位置 不用来作为容器
        pHomeMenu->setToggleCallBack(this, callfuncO_selector(HomeView::menuToggleCallBack));
    }
}

void HomeView::createWeeklyMenu()
{
	//CCMenuItemImage *img = CCMenuItemImage::create()

	WeeklyMenu* pMenu = WeeklyMenu::create();
	pMenu->setDelegate(m_pDelegate);
	pMenu->setPosition(CCPointZero);
	m_pWeeklyMenu->addChild(pMenu);

}

void HomeView::onEnter()
{
	DLayer::onEnter();
	BINDING_EXEC(DM_GET_USER_MODEL->getUserInfo(),this,HomeView::onUpdateUserInfo,D_EVENT_USERINFO_UPDATE);
	BINDING_EXEC(m_pMailModel,this,HomeView::onUpdateNewMailCount,D_EVENT_MAIL_NEW_COUNT_UPDATE);
	BINDING_EXEC(m_pMailModel,this,HomeView::onUpdateNewMailCount,D_EVENT_MAIL_NEW_COUNT_SYNC);
	BINDING_EXEC(m_pActivityModel,this,HomeView::onUpdateOnlineAward,D_EVENT_ACTIVITY_ONLINE_AWARD);
	BINDING_EXEC(DM_GET_SIGNIN_MODEL,this,HomeView::onUpdateNewSign,D_EVENT_SIGNIN_UPDATE);
	

	//新增免费抽卡角标 五秒刷新一次
	showLotteryDing(0);
	TimerUtil::sharedTimerUtil()->schedule(schedule_selector(HomeView::showLotteryDing), this, 5); 
	
	/*
    BINDING_EXEC(m_pActivenessModel,this,HomeView::onActivenessUpdate,D_EVENT_ACTIVENESS_UPDATE);
	BINDING_EXEC(m_pMailModel,this,HomeView::onUpdateNewMailCount,D_EVENT_MAIL_NEW_COUNT_UPDATE);
	BINDING_EXEC(m_pMailModel,this,HomeView::onUpdateNewMailCount,D_EVENT_MAIL_NEW_COUNT_SYNC);
    BINDING_EXEC(DM_GET_USER_MODEL->getUserInfo(),this,HomeView::onUpdateUserInfo,D_EVENT_USERINFO_UPDATE);
    BINDING_EXEC(DM_GET_PRIZE_MODEL,this,HomeView::onPrizeUpdate,D_EVENT_PRIZE_INIT);
    
    BINDING_ONLY(DM_GET_PRIZE_MODEL,this,HomeView::onPrizeUpdate,D_EVENT_PRIZE_UPDATE);
   
    //BINDING_EXEC(DM_GET_CHARGE_PRIZE_MODEL,this,HomeView::onChargePrizeUpdate,D_EVENT_CHARGE_PRIZE_INIT);
    //BINDING_ONLY(DM_GET_CHARGE_PRIZE_MODEL,this,HomeView::onChargePrizeUpdate,D_EVENT_CHARGE_PRIZE_UPDATE);
    showLotteryDing(0);
    
    DLayer::onEnter();
    
    CCRect rect;
    rect.origin = m_pNavRect->convertToWorldSpace(CCPointZero);
    rect.size = m_pNavRect->getContentSize();
    
    m_pMenuTouchHelper->setRect(rect);
	*/
}

void HomeView::onExit()
{
    TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
    UNBINDING_ALL(m_pActivenessModel, this);
    UNBINDING_ALL(m_pMailModel, this);
	UNBINDING_ALL(m_pActivityModel, this);
    UNBINDING_ALL(DM_GET_PRIZE_MODEL, this);
    UNBINDING_ALL(DM_GET_USER_MODEL->getUserInfo(), this);
	UNBINDING_ALL(DM_GET_SIGNIN_MODEL, this);
    //UNBINDING_ALL(DM_GET_CHARGE_PRIZE_MODEL, this);
    DLayer::onExit();
}

void HomeView::layout()
{
    CCObject* obj = NULL;
    CCNode* pNode = NULL;
    float vy = 0.f;
    CCARRAY_FOREACH(m_pLeftIconsContainer->getChildren(), obj)
    {
        pNode = static_cast<CCNode*>(obj);
        if(pNode && pNode->isVisible())
        {
            pNode->setPosition(ccp(0, vy));
            vy-=pNode->getContentSize().height;
        }
    }
}

void HomeView::onUpdateNewSign()
{
	//处理签到角标
	m_pNewSignBg->setVisible(false);
	m_pSignFlash->setVisible(!DM_GET_SIGNIN_MODEL->isSignedToday());
	m_pSignImageNode->setVisible(DM_GET_SIGNIN_MODEL->isSignedToday());
}

void HomeView::onUpdateOnlineAward()
{
	//确认同步
	if(!m_pActivityModel->isOnlineAwardSynced) return;
	//先关闭
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(HomeView::refreshOnlineAward), this);

	if(m_pActivityModel && m_pActivityModel->showAward())
	{
		unsigned int countdown = m_pActivityModel->getOnlineAwardCountdown();
		
		//显示显示领奖
		m_pOnlineMenu->setVisible(true);
		m_pOnlineFlash->setVisible(countdown == 0);
		m_pOnlineImageNode->setVisible(!m_pOnlineFlash->isVisible());
		if(countdown == 0)
		{
			//当前就可以领取
			m_pCountdown->setString(TimeUtil::getSeconds2String(countdown));
		}
		else{
			//倒计时
			m_pCountdown->setString(TimeUtil::getSeconds2String(countdown));
			TimerUtil::sharedTimerUtil()->schedule(schedule_selector(HomeView::refreshOnlineAward), this , 1);
		}
	}
	else{
		//不显示
		//隐藏图片动画和menu
		m_pOnlineFlash->setVisible(false);
		m_pOnlineImageNode->setVisible(false);
		m_pOnlineMenu->setVisible(false);
	}
}

void HomeView::refreshOnlineAward(float dt)
{
	unsigned int countdown = m_pActivityModel->getOnlineAwardCountdown();
	m_pCountdown->setString(TimeUtil::getSeconds2String(countdown));
	if(countdown == 0) onUpdateOnlineAward();
}

void HomeView::showLotteryDing(float dt)
{
    //time_t cd = m_pLotteryModel->getMinCD();
	unsigned int times = m_pLotteryModel->getFreeCount();
    if(times==0)
    {
		m_pFreeCardBg->setVisible(false);
    }else
    {
		m_pFreeCardBg->setVisible(true);
		m_pFreeCardLabel->setString(D_CSTRING_FROM_UINT(times));
    }
	//TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(HomeView::showLotteryDing), this, 1); 
}

void HomeView::onUpdateUserInfo()
{
    m_pNodeFirst->setVisible(!DM_GET_USER_MODEL->getUserInfo()->isPayed());
	m_pNodeCharged->setVisible(!m_pNodeFirst->isVisible());
//    layout();
}

void HomeView::onUpdateNewMailCount()
{
    //显示新邮件数(仅当大于零时显示)
	int newMailsNum = m_pMailModel->getNewMailsNum();
	m_pNewMailBg->setVisible(newMailsNum>0);
	if(newMailsNum>0) m_pNewMailLabel->setString(D_CSTRING_FROM_INT(newMailsNum));
}

void HomeView::onUpdateNewRemainCount()
{
    //显示新邮件数
	uint32_t newMailsNum = m_pMailModel->getNewMailsNum(); //+ + + + + 公告 + + +  其他 +
	m_pNewRemindBg->setVisible(newMailsNum>0);
	if(newMailsNum>0) m_pNewRemindLabel->setString(CCString::createWithFormat("%d",newMailsNum)->getCString());
}

void HomeView::onActivenessUpdate()
{
    m_pActivenessDing->setVisible(m_pActivenessModel->hasAwardDrawable());
	m_pActiveness->setString(CCString::createWithFormat("%d",m_pActivenessModel->getPoint())->getCString());
}

void HomeView::getAwardCallBack(cocos2d::CCObject *sender)
{    
	if(m_pDelegate)
		m_pDelegate->NavigateTo(kNavPrize);
}

void HomeView::reChargeCallBack(cocos2d::CCObject *sender)
{
     NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHOW_RECHARGE);
}

void HomeView::onPrizeUpdate()
{
	m_pNodeXueNv->setVisible(!DM_GET_PRIZE_MODEL->isXueNvDrawn());
    m_pGetaward->setVisible(!DM_GET_PRIZE_MODEL->isDone());
    if(m_pGetaward->isVisible())
    {
        uint32_t qualifiedNum = DM_GET_PRIZE_MODEL->getQualifiedNum();
        m_pRemindContainer->setVisible(qualifiedNum > 0);
        m_pQualifiedPrizeNum->setString(CCString::createWithFormat("%d", qualifiedNum)->getCString());
    }
    layout();
}

//void HomeView::chargeAwardCallBack(cocos2d::CCObject *sender)
//{
//	if(m_pDelegate)
//		m_pDelegate->NavigateTo(kNavChargePrize);
//}

//void HomeView::onChargePrizeUpdate()
//{
//#define ani_tag 100
//    m_pChargeAward->setVisible(!DM_GET_CHARGE_PRIZE_MODEL->isDone());
//    m_pChargeAward->removeChildByTag(ani_tag);
//    if(m_pChargeAward->isVisible())
//    {
//        m_pChargeAward->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("getaward1"),-1,ani_tag);
//        uint32_t qualifiedNum = DM_GET_CHARGE_PRIZE_MODEL->getQualifiedNum();
//        m_pChargeRemindContainer->setVisible(qualifiedNum > 0);
//        m_pChargeQualifiedPrizeNum->setString(CCString::createWithFormat("%d", qualifiedNum)->getCString());
//    }
//}

void HomeView::menuToggleCallBack(cocos2d::CCObject * param)
{
	//点击之后 就把说话关掉
	//if(m_pDialogEffect) m_pDialogEffect->removeAllChildrenWithCleanup(false);
	m_pHeroShowView->hideHeroTalk();

	return;
    HomeMenu * pMenu = dynamic_cast<HomeMenu*>(param);
    m_pDialogEffect->setVisible(!pMenu->isShowing());

	if (isGuildBossRunning())
	{
		m_pArmaturePve->setOpacity(0);
		m_pArmatureExplore->setOpacity(0);
		if (GUILD_TAG == pMenu->getTag())
		{
			CCNode *child = pMenu->getChildByTag(navigation::kNavGuildBoss);
			if (child && pMenu->isShowing())
			{
				reorderChild(m_pArmatureBoss, pMenu->getZOrder() + 1000);
				m_pArmatureGuild->setOpacity(0);
				m_pArmatureBoss->setOpacity(255);
			}
			else
			{
				m_pArmatureGuild->setOpacity(255);
				m_pArmatureBoss->setOpacity(0);
			}
		}
	} 
	else if (PVE_TAG == pMenu->getTag())
	{
		m_pArmatureGuild->setOpacity(0);
		m_pArmatureBoss->setOpacity(0);
		CCNode *child = pMenu->getChildByTag(navigation::kNavPve);
		if (child && pMenu->isShowing())
		{
			reorderChild(m_pArmatureExplore, pMenu->getZOrder() + 1000);
			m_pArmaturePve->setOpacity(0);
			m_pArmatureExplore->setOpacity(255);
		}
		else
		{
			m_pArmaturePve->setOpacity(255);
			m_pArmatureExplore->setOpacity(0);
		}
	}
}


bool HomeView::isGuildBossRunning()
{
	//int idx = DM_GET_GUILDBOSS_MODEL->getIndexOfNextPoint();
	guildboss::GuildBossState eState = DM_GET_GUILDBOSS_MODEL->getStateOfLivingPoint();
	navigation::NavigationNode *pNode = DM_GET_NAVIGATION_MODEL->getNavigationNodeById(navigation::kNavGuild);
	//navigation::NavigationNode *pChild = (navigation::NavigationNode *)pNode->getChildren()->objectAtIndex(1);
	return pNode && pNode->isUnlocked() && eState != guildboss::GuildBossStateClose;
}

void HomeView::callBackHome(cocos2d::CCObject *pObj)
{
	CCMenuItem *pItem = dynamic_cast<CCMenuItem *>(pObj);
	if (!pItem)
	{
		return;
	}
	switch(pItem->getTag())
	{
	case kTagActivity:
		Game::sharedGame()->NavigateTo(navigation::kNavActivity);
		break;
	case kTagCharge:
		//Game::sharedGame()->NavigateTo(navigation::kNavActivity, false, CCInteger::create(12));
		//go to recharge page , both when recharge or first recharge	


		//NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHOW_RECHARGE);
		if(DM_GET_USER_MODEL->getUserInfo()->isPayed()){
			//充值
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHOW_RECHARGE);
		}
		else{
			//首充
			FirstRecharge* first = FirstRecharge::create();
			Game::sharedGame()->openPopup(first);
		}

		break;
	case kTagSign:
//		Game::sharedGame()->NavigateTo(navigation::kNavSignin);
		Game::sharedGame()->openPopup(Signin::create());
		break;
	case kTagMail:
		Game::sharedGame()->NavigateTo(navigation::kNavMail);
		break;
	case kTagMallLottery:
		Game::sharedGame()->NavigateTo(navigation::kNavMallLottery);
		break;
	case kTagOnlineAward:
		{
			//弹出在线礼包界面
			OnlineAward* popup = OnlineAward::create();
			Game::sharedGame()->openPopup(popup);
			break;
		}
    case kTagSpecialActivity:
        {
            Game::sharedGame()->NavigateTo(kNavActivity, true, CCInteger::create(DM_GET_ACTIVITY_MODEL->getSpecialActivityType()));
            break;
        }
	case kTagActiveness:
		Game::sharedGame()->NavigateTo(navigation::kNavActiveness);
		break;
	default:
		;
	}
}