//
//  HomeView.h
//  主页
//
//  Created by cll on 13-3-22.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__HomeView__
#define __QSMY__HomeView__

#include "AppInclude.h"
#include "../Game.h"
#include "HeroShowView.h"

#define MENU_COUNT 4
#define PVE_TAG 0
#define GUILD_TAG 5


enum HomeTag
{
	kTagActivity	= 0,
	kTagCharge		= 1,
	kTagSign		= 2,
	kTagMail		= 3,
	kTagMallLottery = 4,
	kTagOnlineAward = 5,
    kTagSpecialActivity = 6,
	kTagActiveness = 7,
};

class NavigationDelegate;

class MenuTouchHelper
:public cocos2d::CCObject
,public cocos2d::CCTouchDelegate
{
    CC_SYNTHESIZE_PASS_BY_REF(cocos2d::CCRect, m_rect, Rect);
public:
    
    MenuTouchHelper()
    :m_bEnabled(false)
    {
        
    }
    
    virtual ~MenuTouchHelper()
    {
        setEnabled(false);
    }
    
    CREATE_FUNC(MenuTouchHelper);
    D_INIT_FUNC();
    virtual void setEnabled(bool val)
    {
        CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        if(!pDispatcher) return;
        if(m_bEnabled==val) return;
        m_bEnabled = val;
        if(val) pDispatcher->addTargetedDelegate(this, -1,false);
        else if(pDispatcher) pDispatcher->removeDelegate(this);
    }
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    bool        m_bEnabled;
};

class HomeView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
public:
	enum
	{
		kHomeWarrior,			//弟子
		kHomeEquip,				//装备
		kHomeLadder,			//论剑
		kHomeWill,				//意志
		kHomeLottery,			//抽卡
		kHomeTrain				//练武
        
	};
public:
	HomeView();
	virtual ~HomeView();
    
    //virtual bool init();
    //CREATE_FUNC(HomeView);
    
    static HomeView* create(NavigationDelegate* pDelegate);
    virtual bool init(NavigationDelegate* pDelegate);
    
    virtual void onEnter();
    virtual void onExit();
    
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
private:
	void menuCallback(cocos2d::CCObject *sender);
    void onActivenessUpdate();
    void onUpdateNewMailCount();
    void onUpdateNewRemainCount();
    void onUpdateUserInfo();
    
    void updateMenuDisplay(bool animate);
    void createHomeMenus();
    void getAwardCallBack(cocos2d::CCObject *sender);
    void reChargeCallBack(cocos2d::CCObject *sender);
    void showAnnounceCallBack(cocos2d::CCObject*);
    void onPrizeUpdate();
    
    //void chargeAwardCallBack(cocos2d::CCObject *sender);
    //void onChargePrizeUpdate();
    
    void menuToggleCallBack(cocos2d::CCObject * param);
    
	//处理招募角标
    void showLotteryDing(float dt);
	//处理签到角标
	void onUpdateNewSign();
	//处理在线奖励
	void onUpdateOnlineAward();
	//更新在线奖励倒计时
	void refreshOnlineAward(float dt);
    
    void toggleNav(cocos2d::CCObject *sender);
    void showNav(bool val);
    void hideNav();
    void changeSpecialVisible();
    
    
    void layout();

	bool isGuildBossRunning();


	void callBackHome(cocos2d::CCObject *sender);
    void createWeeklyMenu();

private:
    
    NavigationDelegate              *m_pDelegate;
	ActivenessModel                 *m_pActivenessModel;
    MailModel                       *m_pMailModel;
    LotteryModel                    *m_pLotteryModel;
	ActivityModel                    *m_pActivityModel;
    NavigationModel                 *m_pNavigationModel;
    static bool                     m_bMenuShowing;
    
    /** ccb bind**/
    cocos2d::CCLabelBMFont          *m_pActiveness;
    cocos2d::CCNode                 *m_pHeroContainer;
    cocos2d::CCMenuItemImage        *m_pSettingMenuItem;
	cocos2d::CCMenuItemImage        *m_pAnnounceMenuItem;
    cocos2d::CCMenuItemImage        *m_pMailMenuItem;
    cocos2d::CCMenuItemImage        *m_pSepcialActivityMenuItem;
    cocos2d::CCMenuItemImage        *m_pToggleMenuItem;
    cocos2d::CCMenuItem             *m_pStrategyMenuItem;
    cocos2d::CCLayerGradient        *m_pMenuBg;

    cocos2d::CCNode                 *m_pRemindContainer;
    cocos2d::CCLabelBMFont          *m_pQualifiedPrizeNum;
    
    cocos2d::CCNode                 *m_pMenuContainer[MENU_COUNT];
    
    cocos2d::CCNode                 *m_pSpecialText;
    cocos2d::CCLabelBMFont		*m_pNewMailLabel;
    cocos2d::CCNode                 *m_pNewMailBg;
	cocos2d::CCLabelBMFont		*m_pFreeCardLabel;
	cocos2d::CCNode                 *m_pFreeCardBg;
    cocos2d::CCNode                 *m_pNewRemindBg;
    cocos2d::CCLabelBMFont		*m_pNewRemindLabel;
	cocos2d::CCNode                 *m_pNewSignBg;
	cocos2d::CCNode					*m_pSignFlash;
	cocos2d::CCNode					*m_pSignImageNode;
	cocos2d::CCNode					*m_pOnlineFlash;
	cocos2d::CCNode					*m_pOnlineImageNode;

	cocos2d::CCLabelBMFont			*m_pCountdown;
	cocos2d::CCNode						*m_pOnlineNode;
	cocos2d::CCMenuItemImage	*m_pOnlineImage;
	cocos2d::CCMenuItem				*m_pOnlineMenu;
    
    //cocos2d::CCNode                 *m_pChargeAward;
    //cocos2d::CCNode                 *m_pChargeRemindContainer;
    //cocos2d::CCLabelBMFont          *m_pChargeQualifiedPrizeNum;
    
    cocos2d::CCNode                 *m_pNavRect;
    cocos2d::CCNode                 *m_pDialogEffect;
    cocos2d::CCNode                 *m_pEffectNode;
    
    cocos2d::CCNode                 *m_pActivenessDing;
    cocos2d::CCNode                 *m_pLotteryDing;
    
    
    cocos2d::CCNode                 *m_pGetaward;
    cocos2d::CCNode                 *m_pReCharge;
    cocos2d::CCNode                 *m_pLeftIconsContainer;
	cocos2d::CCNode                 *m_pWeeklyMenu;      //周活动按钮容器
   
    /** //ccb bind**/
    
    
    MenuTouchHelper                 *m_pMenuTouchHelper;

	sp::Armature					*m_pArmaturePve;
	sp::Armature					*m_pArmatureExplore;
	sp::Armature					*m_pArmatureGuild;
	sp::Armature					*m_pArmatureBoss;
	CCNode							*m_pPveEffect;
	CCNode							*m_pExploreEffect;
	CCNode							*m_pGuildEffect;
	CCNode							*m_pBossEffect;
    CCNode							*m_pNodeXueNv;
	CCNode *m_pNodeFirst;
	CCNode *m_pNodeCharged;

	HeroShowView* m_pHeroShowView;
};

#endif /* defined(__QSMY__HomeView__) */
