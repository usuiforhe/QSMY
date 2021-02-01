//
//  AppEventType.h
//  消息定义
//
//  Created by wanghejun on 13-3-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef QSMY_AppEventType_h
#define QSMY_AppEventType_h

/************************************************************************/
/*                    events definition                                 */
/************************************************************************/

#define D_EVENT_END_BATTLE          "end battle"
#define D_EVENT_START_BATTLE        "start battle"

#define D_EVENT_INIT_MODEL          "init model"            //model初始化完成

#define D_EVENT_NOT_ENOUGH_COIN     "not enough coin"       //银币不足
#define D_EVENT_NOT_ENOUGH_POINT    "not enough point"      //点券不足
#define D_EVENT_NOT_ENOUGH_ITEMS    "not enough items"      //道具不足
#define D_EVENT_NOT_ENOUGH_VIPLV    "not enough vip level"  //vip等级不足

#define D_EVENT_SHOW_RECHARGE       "show recharge"         //显示充值界面

#define D_EVENT_USER_CHARGE_SUC     "user charge suc"       //用户充值成功


#define D_EVENT_BUY_MONTHCARD       "buy monthcard"         //购买月卡

#define D_EVENT_EDIT_MESSAGE		"edit message"			//编辑留言

//////////////////////////////////////////////////////////////////////////

#pragma mark - tutorial -

#define D_EVENT_SHOW_PVE_SETTLEMENT    "show pveSettlement"  //显示pve战后结算
#define D_EVENT_CLOSE_LEVELUP_POPUP    "hide levelup"        //关闭玩家升级结算
#define D_EVENT_PVE_GET_3_STAR	"pve get 3 star"					//可以获得3星奖励
#define D_EVNET_PVE_CAN_GET_REPUTATION	"pve can get reputation"	//可以获得声望奖励
#define D_EVENT_GET_WONDER			"pve get wonder" //触发了奇遇
#define D_EVENT_GAME_VIEW_CHANGED		"game view changed"	//游戏主界面切换（签到需要在游戏主界面触发）



//////////////////////////////////////////////////////////////////////////
#pragma mark - platform -

#define D_EVENT_LOGIN_SUC       "login Succeed"                 //登陆成功
#define D_EVENT_LOGOUT_SUC      "logout Succeed"                //登出成功
#define D_EVENT_FETCHING_FACEBOOK_EMAIL "fetching facebook email"   //开始获取facebook用户的邮箱
#define D_EVENT_FETCH_FACEBOOK_EMAIL_DONE   "fetch facebook email done" //获取facebook用户邮箱结束
#define D_EVENT_PAY_START		"pay start"						//开始支付
#define D_EVENT_PAY_COMPLETE	"pay complete"					//支付结束


//////////////////////////////////////////////////////////////////////////
#pragma mark - system -

#define D_EVENT_FORCE_LOGOUT        "user not login"                //强制踢到登陆界面
#define D_EVENT_SYSTEM_MAINTAIN     "system maintain"               //系统维护中
#define D_EVENT_REQUEST_FAILD       "request failed"                //发送请求失败
#define D_EVENT_LOGIN_FAILD       "login failed"                //登录失败
#define D_EVENT_SPECIAL_ACTIVI_CHANGED  "special active changed"    //特殊活动

#pragma mark - tower -
#define D_EVENT_TOWER_TOWER_OUT_OF_DATE   "the round ended"           //千层塔本轮结束

#pragma mark - pve -
#define D_EVENT_AFTER_DIALOG "after_dialog"	//剧情结束

//////////////////////////////////////////////////////////////////////////
#pragma mark - version -
#define D_EVENT_CHECK_RES_VERSION_COMPLETE "check res version complete" //检查资源版本完成

//////////////////////////////////////////////////////////////////////////
#pragma mark - share -
#define D_EVENT_SHARE "share msg" //分享
#define D_EVENT_SHARED_SUC "shared msg suc" //分享成功
#define D_EVENT_INVITE_SUC  "invite msg suc" //FB邀请成功

#pragma mark - 掌中宝 -
#define D_STRATEGY_SHOW	"strategy show"
#define D_STRATEGY_HIDE	"strategy hide"

#pragma mark - 设置 -
#define D_EVENT_CLOSE_SETTING	"close setting"

#define D_EVENT_CHAT_CHECK_NEW	"ChatCheckNew"

#pragma mark - weekly -

#define D_EVENT_WEEKLY_UPDATE "weekly update"
#define D_EVENT_WEEKLY_ONBACK	"weekly onback"

#endif
