//
//  Game.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Game.h"
#include "components/components.h"
#include "managers/GameManager.h"
#include "AppEventType.h"
#include "VisibleRect.h"
#include "managers/SoundManager.h"
#include "managers/DataManager.h"
#include "managers/TipsManager.h"
#include "managers/ResourceManager.h"
#include "managers/PlatformManager.h"
#include "managers/AnalyticsManager.h"
#include "managers/ShareManager.h"

#include "navigation/Navigation.h"
#include "equip/Equip.h"
#include "inventory/Inventory.h"
#include "inventory/BuyPopup.h"
#include "battle/Battle.h"
#include "home/Home.h"
#include "pve/Pve.h"
#include "warrior/Warrior.h"
#include "warrior/WarriorInfo.h"
#include "formation/Formation.h"
#include "match/Match.h"
#include "pvp/Pvp.h"
#include "soulhunter/SoulHunter.h"
#include "wonder/Wonder.h"
#include "destiny/Destiny.h"
#include "activeness/Activeness.h"
#include "mall/Mall.h"
#include "lottery/Lottery.h"
#include "lottery/SpecialLottery.h"
#include "account/Account.h"
#include "activity/Activity.h"
#include "tower/Tower.h"
#include "loginReward/LoginReward.h"
#include "settings/Setting.h"
#include "user/UserLevelup.h"
#include "mail/Mail.h"
#include "common/AwardListView.h"
#include "common/AwardItemView.h"
#include "tutorials/Tutorial.h"
#include "wish/Wish.h"
#include "burn/Burn.h"
#include "account/LowBalance.h"
#include "account/FirstRecharge.h"
#include "prize/Prize.h"
//#include "prize/ChargePrize.h"
//#include "announce/Announce.h"
#include "announce/GonggaolanView.h"
#include "utils/TimerUtil.h"
#include "strategy/Strategy.h"
#include "guild/Guild.h"
#include "guildboss/GuildBoss.h"
#include "friends/Friends.h"
#include "friend/Friend.h"
#include "friends/FriendsMessageView.h"
#include "team/Team.h"
#include "model/VersionModel.h"
#include "chat/ChatButton.h"
#include "account/AwardShareItemView.h"
#include "home/FooterMenu.h"
#include "achievement/Achievement.h"
#include "signin/Signin.h"
#include "rank/Rank.h"
#include "burn/RebirthView.h"
#include "home/Welcome.h"
#include "utils/FileUtil.h"
#include "equip/EquipBurn.h"
#include "weekly/Weekly.h"

#ifdef DEBUG_VERSION
#include "debug/DebugLayer.h"
#endif
USING_NS_CC;

using namespace navigation;

#define  NOTICE_BEGIN_X			0

static Game *s_SharedGame = NULL;

Game* Game::sharedGame(void)
{
	if (!s_SharedGame)
	{
		s_SharedGame = new Game();
		s_SharedGame->init();
	}
    
	return s_SharedGame;
}

Game::Game()
:m_pPopupLayer(NULL)
,m_pNavigation(NULL)
,m_pMainLayer(NULL)
,m_pEffectLayer(NULL)
,m_pBattleLayer(NULL)
,m_pTouchLayer(NULL)
,m_pTipsLayer(NULL)
,currentPage(navigation::kNavNIL)
,m_pFocusLayer(NULL)
,m_pNoticeLayer(NULL)
,m_pTutorialLayer(NULL)
,m_pActivityPopupLayer(NULL)
,m_pRechargeLayer(NULL)
,m_pWonderRankLayer(NULL)
,m_pDebugInfo(NULL)
,m_pFloatLayer(NULL)
,m_pActivityGameLayer(NULL)
,m_showedAnnounce(false)
,m_bBackKeyEnabled(true)
{}

Game::~Game()
{
    UNBINDING_ALL(DM_GET_NAVIGATION_MODEL, this);
}

bool Game::init()
{
    if(!BaseScenes::init())
    {
        return false;
    }
    
    initLayers();
    addNotifyHandler();
    //FileUtil::checkLogFileSize();
    
	BINDING_EXEC(DM_GET_NAVIGATION_MODEL, this, Game::addChatButton, D_EVENT_NAV_LOCK_STATUS_UPDATE);
	checkTutorial();
    if(m_pFocusLayer==NULL) NavigateTo(navigation::kNavHome);
	 startUpdateCountDown();
    return true;
}

#define SPECIAL_LOTTERY_TAG 21
void Game::onLotteryFromLua( int cardtype , int cost)
{
	//lua回调 先把页面拉出来
	/************************************************************************/
	/* 防止页面被加载两次 设置tag=21                                                         */
	/************************************************************************/

	//create new view
	SpecialLottery* specialLotteryView = SpecialLottery::create();
	specialLotteryView->setTag(SPECIAL_LOTTERY_TAG);

	//remove old view if exist
	SpecialLottery* oldview = NULL;	
	CCNode*	oldnode = getChildByTag(SPECIAL_LOTTERY_TAG);
	if(oldnode)
	{
		oldview = dynamic_cast<SpecialLottery*>(oldnode);
		if(oldview)
		{
			oldview->removeFromParent();
		}
	}

	//add new view
	addChild(specialLotteryView);
	//specialLotteryView->showAnimation(card::CardType(cardtype));
	specialLotteryView->drawCard(card::CardType(cardtype) , cost);
}

void Game::onConfirmFromLua(int wonderID , int wonderType , int buy , int price)
{
	//奇遇中，3不期而遇4云游商人 目前只处理这两种情况
	const char* m = wonderType == 3 ? "warrior" : "trader";
	if(buy)
	{
		//买
		CCString* pNoticeTrader = CCString::create(D_LOCAL_STRING("$qiyu_yunyoushangren" , price));
		CCString* pNoticeWarrior = CCString::create(D_LOCAL_STRING("$qiyu_buqieryu" , price));
		CCString* pShow = wonderType == 3 ? pNoticeWarrior : pNoticeTrader;
		//show confirm
		DMessageBox* pPBox = DMessageBox::showMessageBox(pShow->getCString(), this, messageBox_callfunc_selector(Game::onMessageBoxComfirm));
		pPBox->setUserObject(CCInteger::create(wonderID*10+wonderType));
	}
	else
	{
		//不买
		cocos2d::CCString* pStr = CCString::createWithFormat("{\"id\":%d,\"buy\":%d}" , wonderID , 0);
		HTTP_CLIENT->requestFromLua(NULL , "Wonder" , m , pStr->getCString());
	}
}

void Game::onMessageBoxComfirm(MessageType type,DMessageBox*target)
{
	if(type == kMessageTypeOk)
	{
		CCInteger* pInt = dynamic_cast<CCInteger*>(target->getUserObject());
		int wonderID = pInt->getValue()/10;
		int wonderType = pInt->getValue()%10;
		//奇遇中，3不期而遇4云游商人 目前只处理这两种情况
		const char* m = wonderType == 3 ? "warrior" : "trader";
		cocos2d::CCString* pStr = CCString::createWithFormat("{\"id\":%d,\"buy\":%d}" , wonderID , 1);
		HTTP_CLIENT->requestFromLua(NULL , "Wonder" , m , pStr->getCString());
	}
}

void Game::startUpdateCountDown()
{
    uint32_t lastResponseTime = (uint32_t)HTTP_CLIENT->getHttpHandler()->getLastResponseTime();
    uint32_t time = GameManager::sharedGameManager()->getServerTime();
    if(TimeUtil::getStartTimeOfToday(time)!=TimeUtil::getStartTimeOfToday(lastResponseTime))
    {
        updateGame(0.f);
        return;
    }
    
    TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(Game::updateGame), this, TimeUtil::getStartTimeOfTomorrow(time)-time+int(CCRANDOM_0_1()*D_CONFIG_INT_FOR_KEY(UPDATE_DELAY))); //增加延迟刷新减轻服务器压力
}

void Game::updateGame(float dt)
{
    VersionModel::sharedVersionModel()->checkResVersion(); //检查资源版本没问题再刷新 否则直接跳更新页面不需要刷新信息了
	m_showedAnnounce = false;
}

void Game::doUpdateGame(CCObject* obj)
{
    CCBool *pNeedUpdate = dynamic_cast<CCBool*>(obj);
    CCAssert(pNeedUpdate, "error info");
    if(pNeedUpdate->getValue())
    {
        forceLogout(CCStringMake(D_LOCAL_STRING("NeedUpdateRes")));
        return;
    }
    
	DataManager::sharedDataManager()->resetData();
	AnalyticsManager::sharedAnalyticsManager()->setSysInited(false);
    HTTP_CLIENT->initData();
    HTTP_CLIENT->login(PlatformManager::sharedPlatformManager()->getLoginParam().c_str());
    HTTP_CLIENT->getSpecialActivity();
    
    FileUtil::checkLogFileSize();
    
    //if(dynamic_cast<Match*>(m_pFocusLayer)!=NULL) HTTP_CLIENT->initCup();    杯赛自己处理
    
    //if(dynamic_cast<Tower*>(m_pFocusLayer) != NULL) HTTP_CLIENT->towerGet(); 千层塔自己处理
    
    if(dynamic_cast<Pve*>(m_pFocusLayer) != NULL)
    {
        if(DM_GET_PVE_MODEL->getLastShowChapterID()!=-1)
        {
            HTTP_CLIENT->getMaps(DM_GET_PVE_MODEL->getLastShowChapterID());
        }
    }
    if(dynamic_cast<Pvp*>(m_pFocusLayer) != NULL) HTTP_CLIENT->initPvpInfo();
    if(dynamic_cast<Lottery*>(m_pFocusLayer) != NULL) HTTP_CLIENT->getCardInfo();
    
	if(dynamic_cast<HomeView*>(m_pFocusLayer)!= NULL) 
		checkAnnounce();
		//checkLoginReward();
    
	if (dynamic_cast<Guild*>(m_pFocusLayer) != NULL)
	{
		HTTP_CLIENT->guildGet();
	}
    
	if (dynamic_cast<Team*>(m_pFocusLayer) != NULL)
	{
		HTTP_CLIENT->FriendDungeon();
	}
    startUpdateCountDown();
}

void Game::addNotifyHandler()
{
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Game::endBattle),D_EVENT_END_BATTLE);
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::startBattle),D_EVENT_START_BATTLE);
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Game::UserLevelUpHandler),D_EVENT_USER_LEVELUP);
	NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::showDropsHandler),D_EVENT_DROPS_SHOW);
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::removeDropsHandler),D_EVENT_DROPS_REMOVE);
    
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Game::notEnoughPoint),D_EVENT_NOT_ENOUGH_POINT);
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Game::notEnoughCoin),D_EVENT_NOT_ENOUGH_COIN);
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::notEnoughItems),D_EVENT_NOT_ENOUGH_ITEMS);
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::notEnoughVipLv),D_EVENT_NOT_ENOUGH_VIPLV);
    
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Game::showRecharge),D_EVENT_SHOW_RECHARGE);
    
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Game::userLogout),D_EVENT_LOGOUT_SUC);

	NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Game::showRechargeLoading),D_EVENT_PAY_START);
	NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Game::hideRechargeLoading),D_EVENT_PAY_COMPLETE);
    
    
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::forceLogout),D_EVENT_FORCE_LOGOUT);
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::forceLogout),D_EVENT_SYSTEM_MAINTAIN);
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Game::requestFailed),D_EVENT_REQUEST_FAILD);
    
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Game::activityReset),D_EVENT_ACTIVITY_RESET);
    
    //NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::chapterAfterDialog),D_EVENT_AFTER_DIALOG);


	NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::showEditMessage),D_EVENT_EDIT_MESSAGE);
    
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::shareMsg), D_EVENT_SHARE);
	NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Game::shareMsgSuc), D_EVENT_SHARED_SUC);
    
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::doUpdateGame), D_EVENT_CHECK_RES_VERSION_COMPLETE);

	NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfuncO_selector(Game::strategyShowCallback), D_STRATEGY_SHOW);
	NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfuncO_selector(Game::strategyHideCallback), D_STRATEGY_HIDE);

	NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Game::showAchievementFlash), D_EVENT_ACHIEVEMENT_FLASH);

	NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfuncO_selector(Game::chatCheckNew),D_EVENT_CHAT_CHECK_NEW);
}

void Game::showAchievementFlash(CCObject* obj)
{
	CCInteger * i = dynamic_cast<CCInteger*>(obj);
	m_pNavigation->updateAchievement(i->getValue());
	/************************************************************************/
	/* 暂不增加动画效果                                                                     */
	/************************************************************************/
	//Tips_Alert("New Achievement!");
	//AchievementEffect* pEffect = AchievementEffect::create();
	//m_pTipsLayer->addChild(pEffect,0,0);
}
void Game::chapterAfterDialog(CCObject *obj)
{
    CCInteger *cid = dynamic_cast<CCInteger*>(obj);
    if(cid->getValue()==1){
        PlatformManager::sharedPlatformManager()->showBindAccount();
    }
}


void Game::removeNotifyHandler()
{
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void Game::purgeSharedGame()
{
    DMessageBox::setContainer(NULL);
    Tutorial::purgeTutorial();
    removeNotifyHandler();
    removeAllChildren();
    SoundManager::purgeSoundManager();
    HTTP_CLIENT->reset();
    
    CC_SAFE_RELEASE(m_pNavigation);
    CC_SAFE_RELEASE(m_pPopupLayer);
    CC_SAFE_RELEASE(m_pMainLayer);
    CC_SAFE_RELEASE(m_pEffectLayer);
    CC_SAFE_RELEASE(m_pBattleLayer);
    CC_SAFE_RELEASE(m_pBusyLayer);
    CC_SAFE_RELEASE(m_pTipsLayer);
    CC_SAFE_RELEASE(m_pTouchLayer);
    CC_SAFE_RELEASE(m_pNoticeLayer);
    CC_SAFE_RELEASE(m_pTutorialLayer);
    CC_SAFE_RELEASE(m_pActivityPopupLayer);
    CC_SAFE_RELEASE(m_pRechargeLayer);
    CC_SAFE_RELEASE(m_pWonderRankLayer);
	CC_SAFE_RELEASE(m_pFloatLayer);
    CC_SAFE_RELEASE(m_pActivityGameLayer);
    CC_SAFE_RELEASE_NULL(m_pDebugInfo);
    UserInfoView::sharedUserInfoView()->removeFromParent();
    UserInfoView::purgeUserInfoView();
    TimerUtil::purgeTimerUtil();
    
    CC_SAFE_RELEASE(this);
    s_SharedGame = NULL;
    
    DataManager::purgeSharedDataManager();
    CCScriptEngineManager::purgeSharedManager();
    GameManager::sharedGameManager()->initScriptEngine();
    SceneManager::goScene(SceneManager::kSceneTagLogin);
	AnalyticsManager::sharedAnalyticsManager()->setSysInited(false);
	user::UserInfo *pUser = DM_GET_USER_MODEL->getUserInfo();
	AnalyticsManager::sharedAnalyticsManager()->logout(CCString::createWithFormat("%u", pUser->getUserId())->getCString(), pUser->getUserLv());
}


void Game::userLogout()
{
    purgeSharedGame();
}


void Game::requestFailed()
{
    DMessageBox::showMessageBox(D_LOCAL_STRING("checkNetworkRetry").c_str(), this, messageBox_callfunc_selector(Game::requestFailedCallBack));
}

void Game::requestFailedCallBack(MessageType type,DMessageBox*pBox)
{
    switch (type)
    {
        case kMessageTypeOk:
            HTTP_CLIENT->retryFailedRequest();
            break;
        case kMessageTypeCancel:
            userLogout();
            break;
        default:
            break;
    }
    
}

void Game::forceLogout(CCObject* obj)
{
    if(!DM_GET_USER_MODEL->getLogin()) return;
    DM_GET_USER_MODEL->setLogin(false);
    CCString* pReason = dynamic_cast<CCString*>(obj);
    if(pReason)
    {
        DMessageBox::showMessageBox(pReason->getCString(), this, messageBox_callfunc_selector(Game::forceLogoutCallBack));
    }else
    {
        userLogout();
    }
}

void Game::forceLogoutCallBack(MessageType,DMessageBox*)
{
    userLogout();
}

void Game::UserLevelUpHandler()
{
	DM_GET_NAVIGATION_MODEL->updateNavigationUnlockStatus();
    //玩家升级提示
	openPopup(UserLevelup::create());
}


void Game::notEnoughPoint()
{
    if(DM_GET_USER_MODEL->getUserInfo()->isPayed())
    {
        //showLowBalance(D_LOCAL_STRING("PointNotEnoughGoMall").c_str());
		showLowBalance();
        return;
    }
    //showLowBalanceFirst(D_LOCAL_STRING("PointNotEnoughGoMall").c_str());
	showLowBalanceFirst();
}

void Game::notEnoughCoin()
{
    /*if(DM_GET_USER_MODEL->getUserInfo()->isPayed())
    {
        Tips_Warning(D_LOCAL_STRING("CoinNotEnoughGoMall").c_str());
        return;
    }
    showLowBalanceFirst(D_LOCAL_STRING("CoinNotEnoughFirstCharge").c_str());*/

	//现在直接谈提示语 不会再首充
	Tips_Warning(D_LOCAL_STRING("CoinNotEnoughGoMall").c_str());
}

void Game::notEnoughVipLv(cocos2d::CCObject* obj)
{
    CCString* pTips = dynamic_cast<CCString*>(obj);
    if(DM_GET_USER_MODEL->getUserInfo()->isPayed())
    {
        //showLowBalance(pTips==NULL?D_LOCAL_STRING("VIPLVlow").c_str():pTips->getCString());
		showLowBalance(true);
        return;
    }
    //showLowBalanceFirst(pTips==NULL?D_LOCAL_STRING("VIPLVlow").c_str():pTips->getCString());
	showLowBalanceFirst(true);
}

void Game::notEnoughItems(cocos2d::CCObject* obj)
{
    //如果是商城道具提示购买 不然提示不够
    CCInteger* pItemId = dynamic_cast<CCInteger*>(obj);
	uint32_t itemId = 0;
    if(pItemId)
    {
        itemId = pItemId->getValue();
//         if(DM_GET_MALL_MODEL->isItemInMall(itemId))
//         {
//             Tips_Warning(D_LOCAL_STRING("ItemNotEnoughGoMall").c_str()); //FIXME: 跳确认框？
//             return;
//         }
    }
	item::ItemUnenoughInfo *pInfo = DM_GET_ITEM_MODEL->getUnenoughInfo(itemId);
	if (pInfo->getAlertType() == item::UnenoughAlertTypeTips)
	{
		Tips_Warning(pInfo->getContent().c_str());
	}
	else
	{
		DM_GET_MALL_MODEL->setUnenoughItemID(itemId);
		//DMessageBox::showMessageBox(pInfo->getContent().c_str(), this, messageBox_callfunc_selector(Game::callBackUnenough));
		BuyPopup* dp = BuyPopup::create();
		openPopup(dp);
	}
	
    //Tips_Warning(D_LOCAL_STRING("ItemNotEnough").c_str());
}

void Game::callBackUnenough(MessageType eType,DMessageBox* pBox)
{
	if (eType == kMessageTypeCancel)
	{
		return;
	}
	
	DPopup *pop = m_pPopupLayer->getPopingUp();
	if (pop)
	{
		pop->closePopup();
	}
	
	NavigateTo(kNavMall);
}

#define CHAT_BUTTON_TAG 101

void Game::addChatButton()
{
	uint32_t lv = DM_GET_USER_MODEL->getUserInfo()->getUserLv();
	if(lv<10) return;

	ChatButton* pChatBtn = (ChatButton*)m_pFloatLayer->getChildByTag(CHAT_BUTTON_TAG);
	// 特殊处理聊天按钮，工会功能开启之后显示
	navigation::NavigationNode* pNavigationNode = DM_GET_NAVIGATION_MODEL->getNavigationNodeByNavType(navigation::kNavGuild);
	//if (pNavigationNode && pNavigationNode->isUnlocked() && !pChatBtn)
	if (!pChatBtn)
	{
		pChatBtn = ChatButton::create();
		const CCSize& winSize = CCDirector::sharedDirector()->getWinSize();
		const CCSize& size = pChatBtn->getContentSize();
		float minX = (winSize.width - DESIGN_SIZE.width) * 0.5f;
		float maxX = minX + DESIGN_SIZE.width - size.width;
		float minY = (winSize.height - DESIGN_SIZE.height) * 0.5f + PL_MAX_HEIGHT_DIFF/2;
		float maxY = minY +DESIGN_SIZE.height -size.height - PL_MAX_HEIGHT_DIFF;
		float centerY = winSize.height/2;
		pChatBtn->setPosition(ccp(maxX, centerY+50));
		m_pFloatLayer->addChild(pChatBtn, 0, CHAT_BUTTON_TAG);

		// 开启聊天新消息检测
		DM_GET_CHAT_MODEL->openCheckNew();
	}
}

void Game::showLowBalanceFirst(const char* tips)
{
    //LowBalance* pLowBalance = LowBalanceFirst::create();
    //pLowBalance->setTips(tips);
    //openPopup(pLowBalance);

	showRecharge();
	//NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHOW_RECHARGE);
}

void Game::showLowBalance(const char* tips)
{
    LowBalance* pLowBalance = LowBalance::create();
    pLowBalance->setTips(tips);
    openPopup(pLowBalance);
}

void Game::showLowBalanceFirst(bool isVip)
{
	LowBalanceFirst* pLowBalance = LowBalanceFirst::create();
	if(isVip) pLowBalance->setVipNotEnough();
	openPopup(pLowBalance);
}

void Game::showLowBalance(bool isVip)
{
	LowBalance* pLowBalance = LowBalance::create();
	if(isVip) pLowBalance->setVipNotEnough();
	openPopup(pLowBalance);
}

void Game::showRecharge()
{
	//if(DM_GET_USER_MODEL->getUserInfo()->isPayed())
	//{
	//	NavigateTo(kNavAccount);
	//}
	//else{
	//	//first blood
	//	FirstRecharge* first = FirstRecharge::create();
	//	openPopup(first);
	//}

	//不弹首充，直接进充值
	NavigateTo(kNavAccount);
    
}
void Game::showRecharge(bool value)
{
	if(value)
	{
		NavigateTo(kNavAccount);
		//m_pRechargeLayer->openPopup(Account::create());
	}
}

void Game::showSetting()
{
	 Setting* settingLayer = Setting::create(this);
	 if (settingLayer)
	 {
		 openPopup(settingLayer);
	 }
}

void Game::removeDropsHandler(CCObject *obj)
{
    CCInteger* pInt = dynamic_cast<CCInteger*>(obj);
	CCAssert(pInt, "show drop error!");
    DM_GET_DROP_MODEL->removeDrop(pInt->getValue());
}

void Game::showDropsHandler(CCObject *obj)
{
	CCInteger* pInt = dynamic_cast<CCInteger*>(obj);
	CCAssert(pInt, "show drop error!");
	//显示掉落UI
	DropModel *pModel = DM_GET_DROP_MODEL;
	cocos2d::CCArray *pAwardList = pModel->getDropArray(pInt->getValue());
    
	if(!pAwardList || pAwardList->count() == 0) return;
	openPopup(AwardViewFactory::create(pAwardList));
}


void Game::initLayers()
{
    
    m_pRechargeLayer = DPopupLayer::create();
    m_pRechargeLayer->retain();
    m_pRechargeLayer->setPosition(D_APP_CENTER_POINT);
    
    m_pWonderRankLayer = DLayer::create();
    m_pWonderRankLayer->retain();
    m_pWonderRankLayer->setPosition(D_APP_CENTER_POINT);
    
    m_pPopupLayer = DPopupLayer::create();
    m_pPopupLayer->retain();
    m_pPopupLayer->setPosition(D_APP_CENTER_POINT);
    
    m_pActivityPopupLayer = DPopupLayer::create();
    m_pActivityPopupLayer->retain();
    m_pActivityPopupLayer->setPosition(D_APP_CENTER_POINT);
    
    m_pMainLayer = DLayer::create();
    m_pMainLayer->retain();
    m_pMainLayer->setPosition(D_APP_CENTER_POINT);
    
    m_pEffectLayer = DEffectLayer::create();
    m_pEffectLayer->retain();
    m_pEffectLayer->setPosition(D_APP_CENTER_POINT);
    
    
    m_pBattleLayer = DLayer::create();
    m_pBattleLayer->retain();
    m_pBattleLayer->setPosition(D_APP_CENTER_POINT);
    
    
	//m_pNavigation = Navigation::create();
	m_pNavigation = FooterMenuView::create();
	//m_pNavigation->setDelegate(this);
	m_pNavigation ->retain();
	m_pNavigation->setPositionX(D_APP_CENTER_POINT.x);
	
    
    m_pNoticeLayer = DLayer::create();
    m_pNoticeLayer->retain();
    m_pNoticeLayer->setPosition(D_APP_CENTER_POINT);
    
    m_pTutorialLayer = DLayer::create();
    m_pTutorialLayer->retain();
    m_pTutorialLayer->setPosition(D_APP_CENTER_POINT);
    
    m_pMessageBoxLayer = DLayer::create();
    m_pMessageBoxLayer->retain();
    m_pMessageBoxLayer->setPosition(D_APP_CENTER_POINT);
    DMessageBox::setContainer(m_pMessageBoxLayer);
    
    m_pDebugInfo = CCLabelTTF::create("",D_FONT_DEFAULT,D_FONT_SIZE_11);
    m_pDebugInfo->retain();
    m_pDebugInfo->setAnchorPoint(ccp(0,1));
    m_pDebugInfo->setPosition(VisibleRect::leftTop());
    
    m_pActivityGameLayer = DLayer::create();
    m_pActivityGameLayer->retain();
    m_pActivityGameLayer->setPosition(D_APP_CENTER_POINT);
    
    
	//notice board
    CCSprite * notice = CCSprite::create("PL_ui/home/gonggaolan.png");
    notice->setAnchorPoint(D_ANCHOR_POINT_TOP);
	notice->setPosition(D_DESIGN_POINT_TOP);
	PL_MOVE_NODE_ABOVE_CENTER_Y(notice);
    m_pNoticeLayer->addChild(notice);
    
    
	DMarqueeView *pMarquee = DMarqueeView::create(DESIGN_SIZE.width-(NOTICE_BEGIN_X<<1), ccc3(0xff, 0xff, 0xff), D_FONT_DEFAULT, NOTICE_FONT_SIZE);
    //	pMarquee->addString("We Make Wonders!");
	pMarquee->setPosition(ccp(NOTICE_BEGIN_X,DESIGN_SIZE.height - D_NODE_HEIGHT_HALF(notice) - NOTICE_FONT_SIZE-2));
    pMarquee->setDataSource(this);
	PL_MOVE_NODE_ABOVE_CENTER_Y(pMarquee);
	m_pNoticeLayer->addChild(pMarquee);
    
    
    
    m_pBusyLayer = CCNode::create();
    HTTP_CLIENT->getBusyLayer()->removeFromParent();
    m_pBusyLayer ->addChild(HTTP_CLIENT->getBusyLayer());
    m_pBusyLayer ->retain();
    m_pBusyLayer->setPosition(D_APP_CENTER_POINT);
    
    
    m_pTouchLayer = TouchLayer::create();
    m_pTouchLayer ->retain();
    m_pTouchLayer->setVisible(false);
    m_pTouchLayer->setPosition(D_APP_CENTER_POINT);
    
    m_pTipsLayer = DLayer::create();
    m_pTipsLayer ->setContentSize(DESIGN_SIZE);
    m_pTipsLayer->retain();
    m_pTipsLayer->setPosition(D_APP_CENTER_POINT);

	m_pFloatLayer = DLayer::create();
	m_pFloatLayer->retain();
    
    addChild(m_pNavigation,kNavigateLayerZorder);
    addChild(m_pMainLayer,kMainLayerZorder);
    addChild(m_pPopupLayer,kPopupZorder);
    addChild(m_pRechargeLayer,kRechargePopupZorder);
    addChild(m_pWonderRankLayer, kWonderRankZorder);
    addChild(m_pActivityPopupLayer,kActivityPopupZorder);
    addChild(m_pEffectLayer,kEffectLayerZorder);
    addChild(m_pTouchLayer,kTouchLayerZorder);
    addChild(m_pNoticeLayer,kNoticeLayerZoder);
    addChild(m_pTipsLayer, kTipsLayerZorder);
    addChild(m_pTutorialLayer,kTutorialLayerZorder);
    addChild(m_pBusyLayer,kBusyLayerZorder);
    addChild(m_pMessageBoxLayer,kMessageBoxZorder);
	addChild(m_pFloatLayer,kFloatLayerZoder);
    addChild(m_pActivityGameLayer, kActivityGameLayerZorder);
    TIPSMANAGER->setParent(m_pTipsLayer);
    
    
#ifdef DEBUG_VERSION
    DebugButton* pDebugButton = DebugButton::create();
    pDebugButton->setPosition(D_APP_CENTER_POINT);
    addChild(pDebugButton,kPopupZorder-1);
#endif
    
    if(CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename("debug")))
    {
        m_pDebugInfo->setColor(ccc3(255, 0, 0));
        addChild(m_pDebugInfo,kBoderZoder+1);
        schedule(schedule_selector(Game::updateDebugInfo));
    }
    

    CCNode *pNode = NULL;
    CCB_READER("PL_ui/ccb/pl_kuozhankuang/PL_kuozhankuang",pNode);
    pNode->setPosition(D_APP_CENTER_POINT);
    if(pNode) addChild(pNode,kBoderZoder);
}


void Game::updateDebugInfo(float dt)
{
    if(m_pDebugInfo) m_pDebugInfo->setString(CCString::createWithFormat("Av:%d,Us:%d",(int)SystemUtil::availableMemory(),(int)SystemUtil::usedMemory())->getCString());
}

void Game::showRechargeLoading()
{
    HTTP_CLIENT->getBusyLayer()->showBusy();
    //m_pBusyLayer->addChild(RechargeLoadingLayer::sharedRechargeLoadingLayer());
}

void Game::hideRechargeLoading()
{
    HTTP_CLIENT->getBusyLayer()->hideBusy();
    //RechargeLoadingLayer::sharedRechargeLoadingLayer()->removeFromParent();
}


DPopup* Game::openPopup(DPopup* popup)
{
    if(!popup) return NULL;
    m_pPopupLayer->openPopup(popup);
    return popup;
}

void Game::showAnnounce()
{
	if(DM_GET_ANNOUNCE_MODEL->getAnnounceNewList()->count()>0)
	{
		openActivityPopup(GonggaolanView::create());
	}
}

DPopup* Game::openActivityPopup(DPopup* popup)
{
    if(!popup) return NULL;
    m_pActivityPopupLayer->openPopup(popup);
    return popup;
}

void Game::setPopupVisible(bool val)
{
    if(m_pPopupLayer->isVisible()==val) return;
    m_pPopupLayer->setVisible(val);
}


void Game::setNavigatorVisable(bool value)
{
	m_pNavigation->setVisible(value);
}

void Game::playEffect(DEffectNode* pEffect,bool showImm)
{
	removeEffct();
    m_pEffectLayer->addNode(pEffect,showImm);
}

void Game::removeEffct()
{
	if(m_pEffectLayer && m_pEffectLayer->isPlaying())
	{
		m_pEffectLayer->removeAllNode();
	}
}

void Game::playSound(navigation::NavigationType val)
{
    switch (val) {
        case navigation::kNavLadder:
        case navigation::kNavMatch:
        case navigation::kNavTower:
            SoundManager::sharedSoundManager()->playChallengeSound();
            break;
        default:
            SoundManager::sharedSoundManager()->playMainUISound();
            break;
    }
    
}


#define CONTENT_TAG 9999

void Game::NavigateTo(navigation::NavigationType value ,bool force,CCObject *obj)
{
    if(currentPage==value && !force) return;
    navigation::NavigationNode* pNode = DM_GET_NAVIGATION_MODEL->getNavigationNodeByNavType(value);
    if(pNode && !pNode->isUnlocked())
    {
		Tips_Alert(D_LOCAL_STRING("NavIsLock%d",pNode->getUnlockLevel()).c_str());
		return;
    }
    
    playSound(value);
    
    navigation::NavigationType cPage = currentPage;
    currentPage = value;
	m_pFocusLayer = NULL;
    
    m_pNavigation->setNavigateDisplay(value);
	if (DM_GET_WEEKLY_MODEL->isWeekly(value))
	{
		if (!obj)
		{
			obj = CCInteger::create(value);
		}

		m_pFocusLayer = Weekly::create();
	}
	else
	{
		switch (value) {
		case kNavHome:          //首页
			m_pFocusLayer = HomeView::create(this);
			// checkLoginReward();
			checkWelcome();
			checkAnnounce();
			break;
		case kNavPve:           //pve
			m_pFocusLayer = Pve::create();
			break;
		case kNavElite:
			m_pFocusLayer = Pve::create(pve::PveFightElite);
			break;
		case kNavFormation:     //阵容
			{
				if(cPage==kNavChangeFormation)
				{
					m_pFocusLayer = m_pMainLayer->getChildByTag(CONTENT_TAG);
					FormationDelegate* pFormation = dynamic_cast<FormationDelegate*>(m_pFocusLayer);
					if(pFormation) pFormation->showFormationView();
				}else
				{
					m_pFocusLayer = Formation::create();
				}
				break;
			}
		case kNavChangeFormation:   //布阵
			{
				if(cPage==kNavFormation)
				{
					m_pFocusLayer = m_pMainLayer->getChildByTag(CONTENT_TAG);
				}else
				{
					m_pFocusLayer = Formation::create();
				}

				FormationDelegate* pFormation = dynamic_cast<FormationDelegate*>(m_pFocusLayer);
				if(pFormation) pFormation->showChangeFormationView();
				break;
			}
		case kNavExperience:
			m_pFocusLayer = Pvp::create(this);
			break;
		case kNavWonder:		//奇遇（活动）
			m_pFocusLayer = Wonder::create();
			break;
		case kNavMall:           //市集（商城）
			m_pFocusLayer = Mall::create();
			break;
		case kNavMallLottery:           //招募进商城 抽卡页签
			m_pFocusLayer = Mall::create(true);
			break;			
		case kNavLadder:         //论剑 pvp
			m_pFocusLayer = Pvp::create(this);
			break;
		case kNavInventory:      //背包
			m_pFocusLayer = Inventory::create();
			break;
			//case kNavBook:           //武林谱（图鉴）
			//    m_pFocusLayer = HomeView::create(this);
			//    break;
			//case kNavAnnounce:         //公告
			//    m_pFocusLayer = Announce::create();
			//    break;
		case kNavStrategy:		//掌中宝
			m_pFocusLayer = Strategy::create();
			break;
		case kNavWarrior:        //弟子
			m_pFocusLayer = Warrior::create();
			break;
		case kNavBurn:        //炼化
			m_pFocusLayer = Burn::create();
			break;
		case kNavEquipBurn:        //炼化
			m_pFocusLayer = EquipBurn::create();
			break;			
		case kNavRebirth:
			m_pFocusLayer = RebirthView::create();
			break;
		case kNavFriends:		//好友
			m_pFocusLayer = Friend::create();
			break;
		case kNavWish:			 //许愿
			m_pFocusLayer = Wish::create();
			break;
		case kNavSoulHunter:	 //狩魄
			m_pFocusLayer = SoulHunter::create(this);
			break;
		case kNavEquip:          //装备
			m_pFocusLayer = Equip::create();
			break;
		case kNavTreasure:       //心法
			m_pFocusLayer = HomeView::create(this);
			break;
		case kNavDestiny:        //天命
			m_pFocusLayer = Destiny::create();
			break;
		case kNavLottery:        //抽卡
			m_pFocusLayer = Lottery::create(NULL);
			break;
		case kNavMatch:          //杯赛
			m_pFocusLayer = Match::create();
			break;
		case kNavActiveness:    //活跃度
			m_pFocusLayer = Activeness::create();
			break;
		case kNavActivity:		//活动
			m_pFocusLayer = Activity::create();
			break;
		case kNavMail:
			m_pFocusLayer = Mail::create(this);
			break;
		case kNavTower:			//千机楼
			m_pFocusLayer = Tower::create();
			break;
		case kNavPrize:			//奖励
			m_pFocusLayer = Prize::create();
			break;
		case kNavChargePrize:			//奖励
			//m_pFocusLayer = ChargePrize::create();
			break;
		case kNavGuild:
			m_pFocusLayer = Guild::create(kGuildPageGuild);
			break;
		case kNavGuildBoss:
			m_pFocusLayer = Guild::create(kGuildPageBoss);
			break;
		case kNavGuildTomb:
			m_pFocusLayer = Guild::create(kGuildPageTomb);
			break;			
		case kNavTeam:
			m_pFocusLayer = Team::create();
			break;
		case kNavAchievement:
			m_pFocusLayer = Achievement::create();
			break;
		case kNavSignin:
			m_pFocusLayer = Signin::create();
			break;
		case kNavAccount:
			m_pFocusLayer = Account::create();
			//dynamic_cast<Account*>(m_pFocusLayer)->showInfo(kAccountTypeVipView);
			break;
		case kNavVip:
			m_pFocusLayer = Account::create();
			dynamic_cast<Account*>(m_pFocusLayer)->showInfo(kAccountTypeVipView);
			break;
		case kNavRank:
			m_pFocusLayer = Rank::shareRank();
			break;
		default:
			m_pFocusLayer = HomeView::create(this);
			break;
		}
	}
    
    if(m_pFocusLayer)
    {
        m_pFocusLayer->setTag(CONTENT_TAG);
        m_pFocusLayer->setUserObject(obj);
        
        if(!m_pFocusLayer->getParent())
        {
            m_pMainLayer->removeAllChildren();
        }
    }else
    {
        m_pFocusLayer = HomeView::create(this);
        m_pMainLayer->removeAllChildren();
    }
    
    if(!m_pFocusLayer->getParent())
    {
		//PL_MOVE_NODE_BELOW_CENTER_Y(m_pFocusLayer);
        m_pMainLayer->addChild(m_pFocusLayer);
    }
    
	m_pPopupLayer->removeAllChildrenWithCleanup(true);
    scheduleOnce(schedule_selector(Game::clearCache), 0.f);
    
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_GAME_VIEW_CHANGED, 0.1f);
    
    AnalyticsManager::sharedAnalyticsManager()->viewEnd(D_CSTRING_FROM_INT((int)cPage));
    AnalyticsManager::sharedAnalyticsManager()->viewBegin(D_CSTRING_FROM_INT((int)value));
}

void Game::clearCache(float dt)
{
	if(m_pBattleLayer->getParent()!=NULL)
	{
		//加载战斗资源时clearCache会清除已加载的资源，导致战斗中找不到需要的资源
		return;
	}
    ResourceManager::sharedResourceManager()->clearCache();
}

CCString * Game::getMarqueeText()
{
    CCString * pText = DM_GET_NOTICE_MODEL->getNextNoticeText(notice::kNoticeListNormal);
    if(pText != NULL)
    {
        return pText;
    }
    else
    {
        return DM_GET_NOTICE_MODEL->getTipsInfo()->getDialog();
    }
}


void Game::startBattle(CCObject* obj)
{
    if(m_pBattleLayer->getParent()!=NULL)
    {
        CCLOG("%s","已在战斗中！"); //测试的时候读本地数据进战斗会导致重复进入战斗
        return;
    }
    
    SoundManager::sharedSoundManager()->stopBackgroundMusic();
    
    addChild(m_pBattleLayer,kBattleLayerZorder);
    
    m_pPopupLayer->removeFromParentAndCleanup(false);
    m_pRechargeLayer->removeFromParentAndCleanup(false);
    m_pWonderRankLayer->removeFromParentAndCleanup(false);
    m_pActivityPopupLayer->removeFromParentAndCleanup(false);
    //m_pMainLayer->setVisible(false);
    m_pMainLayer->removeFromParentAndCleanup(false);
    m_pNavigation->removeFromParent();
    m_pEffectLayer->removeFromParentAndCleanup(false);
    m_pMessageBoxLayer->removeFromParentAndCleanup(false);
    m_pNoticeLayer->removeFromParentAndCleanup(false);
    m_pActivityGameLayer->removeFromParentAndCleanup(false);
    if(DM_GET_TUTORIAL_MODEL->isNeedHide()) m_pTutorialLayer->removeFromParentAndCleanup(false);
    
    Battle* pBattle = Battle::create();
    CCInteger* pSkipType = dynamic_cast<CCInteger*>(obj);
    if(pSkipType!=NULL)
    {
        pBattle->setSkipType((battle::SkipBattleType)pSkipType->getValue());
    }
    m_pBattleLayer->addChild(pBattle);
}

void Game::endBattle()
{
    m_pBattleLayer->removeFromParent();
    m_pBattleLayer->removeAllChildren();
    
    addChild(m_pActivityPopupLayer,kActivityPopupZorder);
    //m_pMainLayer->setVisible(true);
    addChild(m_pMainLayer,kMainLayerZorder);
    addChild(m_pPopupLayer,kPopupZorder);
    addChild(m_pRechargeLayer,kRechargePopupZorder);
    addChild(m_pWonderRankLayer, kWonderRankZorder);
    addChild(m_pNavigation,kNavigateLayerZorder);
    addChild(m_pEffectLayer,kEffectLayerZorder);
    addChild(m_pNoticeLayer,kNoticeLayerZoder);
    addChild(m_pMessageBoxLayer,kMessageBoxZorder);
    addChild(m_pActivityGameLayer, kActivityGameLayerZorder);
    if(m_pTutorialLayer->getParent()==NULL) addChild(m_pTutorialLayer,kTutorialLayerZorder);
}


void Game::setTouchEnable(bool enable)
{
    m_pTouchLayer->setVisible(!enable);
}



void Game::checkTutorial()
{
    if(!DM_GET_TUTORIAL_MODEL->isAllStepComplete())
    {
        m_pTutorialLayer->addChild(Tutorial::sharedTutorial());
    }
}

void Game::checkWelcome()
{
	if(!DM_GET_LOGIN_REWARD_MODEL->isSynced && DM_GET_TUTORIAL_MODEL->isAllStepComplete() && DM_GET_LOGIN_REWARD_MODEL->getWelcomeOpen())
	{
		//新手引导时不弹 开关关闭时不弹
		DM_GET_LOGIN_REWARD_MODEL->isSynced = true;
		runAction(CCSequence::createWithTwoActions(CCDelayTime::create(1.0f) , CCCallFunc::create(this, callfunc_selector(Game::openWelcome))));
	}
}

void Game::openWelcome()
{
	openActivityPopup(Welcome::create());
}

void Game::checkLoginReward()
{
    if(!m_pActivityPopupLayer->isVisible()) return;
	if(!DM_GET_LOGIN_REWARD_MODEL->isSynced) HTTP_CLIENT->getLoginRewardInfo(this, callfuncJson_selector(Game::LoginRewardRetFunc));
}

void Game::checkAnnounce()
{
	//if(m_showedAnnounce || !m_pActivityPopupLayer->isVisible()) return;
	//m_showedAnnounce = true;
	//showAnnounce();

	if(DM_GET_TUTORIAL_MODEL->isAllStepComplete() && !DM_GET_ANNOUNCE_MODEL->isLoginShow)
	{
		DM_GET_ANNOUNCE_MODEL->isLoginShow = true;
		showAnnounce();
	}
	
}

void Game::LoginRewardRetFunc(const Json::Value &requestData, const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	/*LoginRewardModel *pModel = DM_GET_LOGIN_REWARD_MODEL;
	if (pModel)
	{
		pModel->ParseLoginRewardFromJson(responseData);
		if (pModel->IsHaveLoginAward())
		{
			openActivityPopup(LoginReward::create());
		}
	}
	*/
    //showAnnounce();
}


void Game::keyBackClicked()
{	
	if (!m_bBackKeyEnabled)
	{
		return;
	}
	
	if (m_pEffectLayer->isPlaying())
	{
		return;
	}
	if (m_pTouchLayer->isVisible())
	{
		return;
	}

	if (HTTP_CLIENT->getBusyLayer()->isShowing())
	{
		return;
	}

	if(m_pBattleLayer->getParent()!=NULL)
	{
		return;
	}
	if (DM_GET_TUTORIAL_MODEL->getIsRunning())
	{
		BaseScenes::keyBackClicked();
		return;
	}


	tutorial::StepInfo *pStepInfo = DM_GET_TUTORIAL_MODEL->getNextStep();
	if (pStepInfo && pStepInfo->isTrigger())
	{
		if (Tutorial::sharedTutorial() && Tutorial::sharedTutorial()->getChildrenCount())
		{
			StepPrompt *pStepView = dynamic_cast<StepPrompt *>(Tutorial::sharedTutorial()->getChildren()->lastObject());
			if (pStepView && pStepView->isVisible())
			{
				pStepView->closeStep();
				return;
			}
		}
	}
	if (m_pMessageBoxLayer->getChildrenCount())
	{
		DMessageBox *pBox = static_cast<DMessageBox *>(m_pMessageBoxLayer->getChildren()->lastObject());
		if (pBox)
		{
			pBox->onTouchCancel(NULL);
			return;
		}
	}
	DPopup *popUp = NULL;
	if((popUp = m_pActivityPopupLayer->getPopingUp()))
	{
// 		LoginReward *pReward = dynamic_cast<LoginReward *>(popUp);
// 		if (pReward && !pReward->isCompleted())
// 		{
// 			return;
// 		}
		
		popUp->onBackKey();
		return;
	}
	if((popUp = m_pPopupLayer->getPopingUp()))
	{
		popUp->onBackKey();
		return;
	}
	if((popUp = m_pRechargeLayer->getPopingUp()))
	{
		popUp->onBackKey();
		return;
	}
	BackKeyInterface *pInterface = dynamic_cast<BackKeyInterface *>(m_pFocusLayer);
	if (pInterface)
	{
		pInterface->onBackKey();
		return;
	}
	else if(dynamic_cast<HomeView*>(m_pFocusLayer) == NULL)
	{
		NavigateTo(kNavHome);
		return;
	}
	else
	{
        BaseScenes::keyBackClicked();
	}
}


void Game::setTutorialRunning(bool val)
{
	if (val != m_pActivityPopupLayer->isVisible())
	{
		return;
	}
	m_pActivityPopupLayer->setVisible(!val);
}

void Game::activityReset()
{
	DM_GET_ACTIVITY_MODEL->isSynced = false;
	if (dynamic_cast<Lottery*>(m_pFocusLayer) != NULL)
	{
		DM_GET_ACTIVITY_MODEL->syncFromServer();
	}
	
}

void Game::showEditMessage(cocos2d::CCObject* pObj)
{
	CCDictionary *pDict = dynamic_cast<CCDictionary *>(pObj);
	if (!pDict)
	{
		return;
	}
	
	CCString *pName = dynamic_cast<CCString *>(pDict->objectForKey("name"));
	if (!pName)
	{
		return;
	}

	CCInteger *pID = dynamic_cast<CCInteger *>(pDict->objectForKey("id"));
	if (!pID)
	{
		return;
	}
	
	CCInteger *pType = dynamic_cast<CCInteger *>(pDict->objectForKey("type"));
	if (!pType)
	{
		return;
	}
	

	FriendsMessageView* pView = FriendsMessageView::create();
	pView->setName(pName->getCString());
	pView->setUserID(pID->getValue());
	pView->setType(mail::MailType(pType->getValue()));
	openPopup(pView);
}

void Game::shareMsg(cocos2d::CCObject* pObj)
{
    CCString *pInfo = dynamic_cast<CCString *>(pObj);
    ShareManager* pShare = ShareManager::sharedShareManager();
    pShare->share(pInfo->getCString(), "");
}

void Game::shareMsgSuc()
{
    if(DM_GET_SHARE_MODEL->isCanReward())
    {
        HTTP_CLIENT->activityShare(this, callfuncJson_selector(Game::onActivityShare));
    }
}

void Game::onActivityShare(const Json::Value& requestData, const Json::Value& responseData)
{
	//显示掉落UI
	DropModel *pModel = DM_GET_DROP_MODEL;
	cocos2d::CCArray *pAwardList = pModel->getDropArray(requestData[UUID].asUInt());

	if(!pAwardList || pAwardList->count() == 0) return;
	openPopup(AwardShareItemView::create((drop::Drop*)pAwardList->objectAtIndex(0)));
}

void Game::strategyShowCallback(cocos2d::CCObject* obj)
{
	if (!m_pFloatLayer)
	{
		return;
	}
	
	m_pFloatLayer->setVisible(false);
}

void Game::strategyHideCallback(cocos2d::CCObject* obj)
{
	if (!m_pFloatLayer)
	{
		return;
	}

	m_pFloatLayer->setVisible(true);
}

void Game::chatCheckNew(cocos2d::CCObject* pObj)
{
	CCInteger* pType = dynamic_cast<CCInteger*>(pObj);
	if (!pType)
	{
		return;
	}

	ChatButton* pChatBtn = (ChatButton*)m_pFloatLayer->getChildByTag(CHAT_BUTTON_TAG);
	if (!pChatBtn)
	{
		return;
	}

	pChatBtn->setHaveNew(pType->getValue() != chat::kChatCheckTypeNone);
}
