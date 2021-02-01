//
//  Game.h
//  游戏场景
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Game__
#define __QSMY__Game__

#include "AppInclude.h"
#include "components/components.h"
#include "model/UserModel.h"
#include "model/WarriorModel.h"
#include "model/NavigationModel.h"
#include "common/TouchLayer.h"
#include "../BaseScenes.h"


class DTipsComponets;
class Navigation;


class NavigationDelegate
{
public:
    virtual ~NavigationDelegate() {}
    virtual void NavigateTo(navigation::NavigationType value,bool force = false,cocos2d::CCObject *obj=NULL) = 0;   
    virtual DPopup* openPopup(DPopup* popup) = 0;               //打开一个弹出框
    virtual DPopup* openActivityPopup(DPopup* popup) = 0;       //一般用于系统自动弹出的弹出框（强制新手引导阶段会隐藏该层）
    virtual void showAnnounce() = 0;    //显示公告
};

class FooterMenuView;
class Game : public BaseScenes,public NavigationDelegate, public DMarqueeViewDataSouce
{
    
    CC_SYNTHESIZE_READONLY(FooterMenuView*, m_pNavigation, Navigation);
    CC_SYNTHESIZE_READONLY(CCNode *, m_pFocusLayer, FocusLayer); //当前激活的ui层
	CC_SYNTHESIZE(bool, m_bBackKeyEnabled, BackKeyEnabled);
	//CC_SYNTHESIZE(Warrior *, m_warrior, Warrior);
    
public:
	static Game * sharedGame(void);
    void purgeSharedGame();
    
    /**
     *	@brief	切换页面
     *
     *	@param 	value 	切换到页面
     *	@param 	false 	是否强制切换 即使已经在这个页面
     */
    virtual void NavigateTo(navigation::NavigationType value,bool force = false,CCObject *obj=NULL);
    
    virtual cocos2d::CCString * getMarqueeText();
    
	void setNavigatorVisable(bool value);
	navigation::NavigationType getCurrentPage(){return currentPage;};
    
    virtual DPopup* openPopup(DPopup* popup);
    void showRecharge(bool);	//由首充进入 充值页面
	void showSetting();
    
    /**
     *	@brief	活动弹窗（位于普通弹窗上面）
     *
     *	@param 	popup 	
     *
     *	@return	
     */
    virtual DPopup* openActivityPopup(DPopup* popup);
    
    void setPopupVisible(bool val);
    
    /**
     *	@brief	播放特效
     *
     *	@param 	pEffect 	特效内容
     *	@param 	false 	是否立刻显示（不进入队列 和正在显示的特效重叠显示）
     */
    void playEffect(DEffectNode* pEffect,bool showImm = false);
	void removeEffct();
    
    
    void appendTips(const char* strTips);
    
    /**
     *	@brief  禁止所有的触摸事件
     *
     *  @prama  true允许触摸 false禁止触摸
     *
     */
    void setTouchEnable(bool enable);
    
	//处理lua中的抽卡回调
	void onLotteryFromLua( int cardtype , int cost);
    //处理奇遇购买确认
	void onConfirmFromLua(int wonderID , int wonderType , int buy , int price);
    //弹框确认
	void onMessageBoxComfirm(MessageType,DMessageBox*);
private:
    
    Game();
    ~Game();
	virtual bool init();
    
protected:
    
    void showRechargeLoading();
    void hideRechargeLoading();

    
    void startUpdateCountDown();
    void updateGame(float dt);
    void doUpdateGame(CCObject* obj);
    void updateDebugInfo(float dt);
    
    void showAnnounce();
    
    void checkTutorial();
    void checkLoginReward();
	void checkAnnounce();
	void checkWelcome();//欢迎界面
	void openWelcome();//播放欢迎界面
    void LoginRewardRetFunc(const Json::Value &requestData, const Json::Value &responseData);
    
    void initLayers();
    
    void startBattle(CCObject*);
    void endBattle();
    void chapterAfterDialog(CCObject *obj);

    
    void addNotifyHandler();
    void removeNotifyHandler();

    void UserLevelUpHandler();
	void showDropsHandler(CCObject *obj);
    void removeDropsHandler(CCObject *obj);
    
    void userLogout();
    
    void forceLogout(CCObject*);
    void forceLogoutCallBack(MessageType,DMessageBox*);
    
    
    void requestFailed();
    void requestFailedCallBack(MessageType,DMessageBox*);
    
    
    void clearCache(float dt);
    
    void showAchievementFlash(CCObject*);
    /*****************************************/
    /*               充值                     */
    /*****************************************/
    void notEnoughPoint(); //点券不足
    void notEnoughCoin();  //金币不足
    void notEnoughItems(cocos2d::CCObject*); //道具不足
    void notEnoughVipLv(cocos2d::CCObject*); //vip等级不足
    void showLowBalance(const char* tips); //提示货币不足
    void showLowBalanceFirst(const char* tips);//提示货币不足（首冲奖励）

	void showLowBalance(bool isVip = false);
	void showLowBalanceFirst(bool isVip = false);

    void showRecharge();        //充值页面

	void showEditMessage(cocos2d::CCObject* pObj);		//留言
    
    void shareMsg(cocos2d::CCObject* pObj);//分享
	void shareMsgSuc();//分享成功
	void onActivityShare(const Json::Value &requestData, const Json::Value &responseData);

	// 检测聊天新消息
	void chatCheckNew(cocos2d::CCObject* pObj);
    
    virtual void keyBackClicked();
protected:
    void playSound(navigation::NavigationType val);

private:
     
    enum
    {
        kZorder                 = 0,
        kMainLayerZorder        = 10,
		kNavigateLayerZorder    = 20,
        kNoticeLayerZoder       = 30,
		kFloatLayerZoder		= 35,		 // 悬浮层
        kActivityGameLayerZorder = 36,
        kRechargePopupZorder    = 40,
        kWonderRankZorder       = 41,
        kPopupZorder            = 42,
        kActivityPopupZorder    = 44,
        kBattleLayerZorder      = 50,
        kEffectLayerZorder      = 60,
        kTutorialLayerZorder    = 65,        //新手引导层
        kMessageBoxZorder       = 66,
        kTipsLayerZorder        = 70,        //提示层
        kBusyLayerZorder        = 80,
        kTouchLayerZorder       = 90,
        kBoderZoder             = 100,       //最外层遮罩
    };
    
    
    navigation::NavigationType currentPage;
    
    
    DPopupLayer         *m_pRechargeLayer;          //充值层
    DPopupLayer         *m_pPopupLayer;             //弹出框层
    DLayer              *m_pWonderRankLayer;        //奇遇排行层
    DPopupLayer         *m_pActivityPopupLayer;     //活动弹出框层
    DLayer              *m_pMainLayer;              //主ui层
    DLayer              *m_pNoticeLayer;            //走马灯层
    DEffectLayer        *m_pEffectLayer;            //播放动画效果层
    DLayer              *m_pBattleLayer;            //战斗层
    DLayer              *m_pTutorialLayer;          //新手引导层
    DLayer              *m_pTipsLayer;              //提示层
    DLayer              *m_pMessageBoxLayer;        //确认信息提示层
    CCNode              *m_pBusyLayer;              //忙碌层
	DLayer				*m_pFloatLayer;				//悬浮层
    DLayer              *m_pActivityGameLayer;      //活动小游戏（暂未使用）
    
    cocos2d::CCLabelTTF *m_pDebugInfo;              //调试信息
    
    TouchLayer          *m_pTouchLayer;             //禁止触摸层

	bool m_showedAnnounce;
private:
	void callBackUnenough(MessageType eType,DMessageBox* pBox);
	void addChatButton();
	void strategyShowCallback(cocos2d::CCObject* obj);
	void strategyHideCallback(cocos2d::CCObject* obj);

public:
	void setTutorialRunning(bool val);

	void activityReset();
};


class BackKeyInterface
{
public:
	virtual void onBackKey() {Game::sharedGame()->NavigateTo(navigation::kNavHome);}; //for android back key
};

#endif /* defined(__QSMY__Game__) */
