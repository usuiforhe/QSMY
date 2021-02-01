//
//  HTTP_API.h
//  通讯协议定义
//
//  Created by wanghejun on 13-4-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef QSMY_HTTP_API_h
#define QSMY_HTTP_API_h

#include "AppInclude.h"

typedef void (cocos2d::CCObject::*SEL_CallFuncJson)(const Json::Value &,const Json::Value &);
#define callfuncJson_selector(_SELECTOR) (SEL_CallFuncJson)(&_SELECTOR)


#define SERVICE_METHOD "sm"
#define ACTION "a"
#define UUID "uuid" //client

static std::string GET_SERVICE_METHOD(const char* s,const char* m)
{
    return cocos2d::CCString::createWithFormat("%s_%s",s,m)->getCString();
}

/************************************************************************/
/*           HTTP请求-服务名与方法对应                                  */
/************************************************************************/

#pragma mark - 阵型 -

/*阵型*/
#define FORMATION_GET           "Formation","get"
#define FORMATION_JOIN          "Formation","join"
#define FORMATION_CHANGE        "Formation","change"
#define FORMATION_EQUIP         "Formation","equip"
#define FORMATION_QUICKEQUIP    "Formation","quickEquip"

#define FORMATION_GETOTHER      "Formation","getOtherFormation"		//查看其它玩家阵容

#pragma mark - 活跃度 -
/*活跃度*/
#define ACTIVENESS_GET            "Activeness","get"
#define ACTIVENESS_GETAWARD       "Activeness","getReward"

#pragma mark - 天命 -
/*天命*/
#define DESTINY_GET             "Destiny","get"
#define DESTINY_IMPROVE         "Destiny","improve"
#define DESTINY_GETREWARD       "Destiny","getReward"

#pragma mark - 技能 -
/*技能*/
#define SKILL_LEVELUP           "Warrior","sLvUp"			//弟子技能升级

#pragma mark - 弟子 -
/*弟子*/
#define WARRIOR_GET             "Warrior","get"				//获取弟子列表
#define WARRIOR_GETONE          "Warrior","getOne"          //获取弟子信息
#define WARRIOR_UP              "Warrior","up"				//弟子突破
#define WARRIOR_DEV             "Warrior","develop"			//弟子培养
#define WARRIOR_INH             "Warrior","inherit"			//弟子传功
#define WARRIOR_DEV_NOT         "Warrior","confirmDev"      //确认或取消弟子培养
#define WARRIOR_SET_DRESS       "Warrior","setDress"        //穿脱时装
#define WARRIOR_UPGRADE			 "Warrior","enhance"        //弟子强化

#pragma mark - 魂魄 -
/*魂魄*/
#define SOUL_GET                "Soul","get"				//获取魂魄列表
#define SOUL_GETONE             "Soul","getOne"				//获取魂魄信息
#define SOUL_RECRUIT            "Soul","recruit"			//弟子招募  这个服务名应该是warrior吧

#pragma mark - 装备 -

/*装备*/
#define EQUIP_GETEQUIPS         "Equip","getEquips"         //获得装备列表
#define EQUIP_GETEQUIP          "Equip","getEquip"          //获得装备信息
#define EQUIP_ENHANCE           "Equip","strengthen"        //强化装备
#define EQUIP_AUTOENHANCE       "Equip","auto"              //自动强化
#define EQUIP_SELLEQUIP         "Equip","batchSell"              //出售装备
#define EQUIP_REFINE			"Equip","refine"			//装备精炼
#define EQUIP_DECOMPOSE			"Equip","decompose"			//装备分解
#define TREASURE_BURN			"Equip","treasureDevour"	//宝物吞噬

#pragma mark - 道具 -
/*道具*/
#define ITEM_GETITEMS           "Item","getItems"           //获得道具
#define ITEM_GETITEM            "Item","getItem"            //获得道具信息
#define ITEM_USEITEM           "Item","useItem"            //使用道具

#pragma mark - 商品 -
/*商品*/
#define PRODUCT_GETS             "Mall","get"               //获得商品列表
#define PRODUCT_BUY              "Mall","buy"               //购买商品
#define BUY_POWER                "Mall","buyPow"            //购买体力
#define BUY_200_RED_PACKET       "Activity","twoHundredBonus"   //购买200红包内的商品

#pragma mark - 玩家信息 -
/*玩家信息*/
#define USER_GET                    "User","get"               //获取信息
#define USER_RAND_NAME              "User","randName"           //随机名字
#define USER_PRIVILEGES				"User","privileges"			//获取特权信息
#define USER_REGISTERTOPLAT         "User","registerToPlat"     //注册玩家登陆过的服务器

#pragma mark - PVE -
/*PVE*/
#define PVE_GET             "Explore","get"                 //获取PVE探索列表
#define PVE_EXEC            "Explore","exec"                //PVE探索执行
#define CHAPTER_GET         "Explore","getC"                //获取PVE章节列表信息
#define CHAPTER_COLLECT_BOX "Explore","cAward"              //获取PVE章节列表信息
#define PVE_ELITE_GET       "Explore","getHero"             //获取副本精英战列表

#define	PVE_RUSH_CLEARCD	"Explore","clearRushCD"			//清除扫荡CD
#define PVE_RUSH            "Explore","rush"                 //扫荡(“mid”:101)
#define PVE_ONCEMORE		"Explore","onceMore"			//购买一次探索机会
#define PVE_GRADEAWARD      "Explore","gradeAward"          //获取章节三星奖励
#define PVE_ACTIVITYAWARD   "Explore","getExploreActivityAward"  //获取活动副本通关奖励
 
#pragma mark - 杯赛 -
/*杯赛*/
#define CUP_INIT        "Cup","init"                        //杯赛初始化
#define CUP_GET         "Cup","get"                         //获取杯赛个人信息
#define CUP_PROCESS     "Cup","process"                     //获取杯赛个人历史
#define CUP_MTABLE      "Cup","mTable"                      //获取8强比赛列表
#define CUP_BETS        "Cup","bets"                        //获取杯赛竞猜记录
#define CUP_MREP        "Cup","mRep"                        //获取比赛战报(含二进制数据)
#define CUP_SIGNUP      "Cup","signUp"                      //杯赛报名
#define CUP_MREWARD     "Cup","mReward"                     //领取比赛奖励
#define CUP_BET         "Cup","bet"                         //竞猜
#define CUP_BETPRIZE    "Cup","betReward"                   //领取竞猜奖励

#pragma mark - 论剑 -
/*论剑*/
#define PVP_INIT		"Challenge","init"                  //初始化论剑信息
#define PVP_GETINFO		"Challenge","info"                  //更新玩家论剑信息
#define PVP_GETLIST		"Challenge","targets"               //获得对手列表
#define PVP_CHPVP		"Challenge","challenge"             //挑战对手
#define PVP_CHPVPLOW	"Challenge","challengeLoser"        //挑战排名比自己低对手
#define PVP_CHNPC		"Challenge","challengeNpc"          //获得试炼木人
#define PVP_CHBOSS		"Challenge","challengeBoss"         //挑战东皇太一
#define PVP_GETREWARD	"Challenge","getReward"             //获得奖励
#define PVP_REFRESH		"Challenge","refresh"
#define PVP_EXCHANGE	"Challenge","exchange"

#pragma mark - 千层塔 -
/*千层塔*/
#define TOWER_GET		"Tower","get"						//【千机楼】获得玩家千机楼信息
#define TOWER_FIGHT		"Tower","fight"						//【千机楼】挑战某层楼
#define TOWER_START		"Tower","start"						//【千机楼】开始进入
#define TOWER_SETBUFF	"Tower","setBuff"					//【千机楼】给某单位加buff
#define TOWER_GOON		"Tower","goOn"						//【千机楼】复活挑战
#define TOWER_END		"Tower","end"						//【千机楼】结束本轮
#define TOWER_RANK		"Tower","rank"						//【千机楼】排行榜
#define TOWER_OLD_RANK	"Tower","old_rank"					//【千机楼】昨日排行榜
#define TOWER_REWARD	"Tower","reward"					//【千机楼】兑换

#pragma mark - 系统 -
/*系统*/
#define SYSTEM_SYNCTIME "System","syncTime"                 //同步后台时间

#pragma mark - 邮件 -
/*邮件*/
#define MAIL_GETMAILS   "Mail","getMails"                   //获取邮件列表
#define MAIL_PICKUP     "Mail","pickup"                     //提取邮件附件
#define MAIL_GETNEWS    "Mail","getNew"                     //获取新邮件数
#define MAIL_DELETE_MAIL	"Mail","deleteMail"				//删除邮件
#define MAIL_RECIVE_ALL_ITEM "Mail","pickupAll"						//一键提取附件


#pragma mark - 抽卡 -
/*抽卡*/
#define CARD_GET        "Card","get"                        //获取抽卡信息
#define CARD_DRAW       "Card","draw"                       //抽卡

#pragma mark - 许愿 -
/*许愿*/
#define WISH_GET        "Wish","get"                        //获取许愿信息
#define WISH_WISH       "Wish","wish"                       //执行许愿
#define WISH_CHANGEWARRIOR	"Wish","changeWarrior"			//修改弟子

#pragma mark - 炼化 -
/*炼化*/
#define BURN_BURN        "RefiningNew","burned"                        //burn
#define BURN_AUTOLIST       "RefiningNew","auto"                       //get auto burn list

#pragma mark - 重生 -
#define  REBIRTH_DROP         "RefiningNew","rebornDrop"      //物品掉落
#define  REBIRTH_REBORN         "RefiningNew","reborn"          //英雄重生

#pragma mark - 新手引导 -
/*新手引导*/
#define ROOKIE_GET      "Rookie","get"                      //获取最后完成的步骤
#define ROOKIE_FINISH   "Rookie","finish"                   //完成引导步骤

#pragma mark - 活动
/*活动*/
#define ACTIVITY_GET_SEPCIAL        "Activity","getSpecial" //获取是否开启特殊活动入口
#define GETLOGINAWARD_INFO			"Activity","getLoginAward"
#define LOGIN_REWARD			    "Activity","loginAward"

#define ACTIVITY_GET_AWARD    "Activity","getAward"          //获取奖励页面信息
#define ACTIVITY_TLOGIN_AWARD   "Activity","tLoginAward"     //领取累计登录奖励
#define ACTIVITY_LVUP_AWARD   "Activity","lvUpAward"         //领取升级奖励

#define ACTIVITY_CHARGE_AWARD   "Activity","rookieTChargeReward" //领取累计充值奖励
#define ACTIVITY_GET			"Activity","get"				//活动信息
#define ACTIVITY_SHARE          "Activity","share"				//分享领奖

#pragma mark - 系统 -
/*系统*/
#define SYSTEM_INIT         "System","init"                     //初始化数据
#define SYSTEM_ANNOUNCE     "System","announce"                 //获取公告(登陆后使用)
#define SYSTEM_NOTICE       "System","notice"                   //获取走马灯列表

#pragma mark - 充值 -
/*充值*/
#define CHARGE_GET_ORDER    "Charge"                        //,"getOrderForPP" 获取订单
#define CHARGE_GET_RESULT   "Charge","getResult"            //获取充值结果


#pragma mark - 设置 -
#define ADD_FEEDBACK        "FeedBack","add"                //反馈

#pragma mark - 兑换 -
#define EX_CODE             "ExCode","exchange"             //兑换

#pragma mark - 公会 -
/*公会*/
#define GUILD_CREATE		"Guild","create"				//创建
#define GUILD_APPLY			"Guild","apply"					//申请
#define GUILD_DEAL_APPLY	"Guild","dealApply"				//处理申请
#define GUILD_GET			"Guild","get"					//公会信息
#define GUILD_GET_LIST		"Guild","getList"				//公会列表
#define GUILD_SEARCH		"Guild","search"				//搜索公会
#define GUILD_POSITION		"Guild","position"				//任职
#define	GUILD_QUIT			"Guild","quit"					//推出（解散）公会
#define GUILD_SIGN			"Guild","sign"					//签到
#define GUILD_QUES			"Guild","ques"					//问答
#define GUILD_DETAILS		"Guild","details"				//详情
#define GUILD_UPDATE		"Guild","update"				//修改公告
#define GUILD_APPLYLIST		"Guild","applyList"				//申请列表
#define GUILD_UPGRADE_TREE	"Guild","upgradeTree"			//升级天工树
#define GUILD_GET_NOTICE	"Guild","getNotice"				//获得跑马灯
#define GUILD_MSG		"Guild","msg"					//留言
#define GUILD_MALL_REFRESH	"GuildMall","refresh"				//【帮会商店】刷新商店
#define GUILD_MALL_EXCHANGE	"GuildMall","exchange"				//【帮会商店】兑换物品
#define GUILD_SUBSTITUTE	"Guild","substitute"			//取代会长
#define GUILD_LOG			"Guild","getLogs"					//日志

#pragma mark - 公会战 -
#define GUILDBOSS_GET				"GuildBoss","get"
#define GUILDBOSS_DECLARE_WAR		"GuildBoss","declareWar"
#define GUILDBOSS_DECLARE_TARGETS	"GuildBoss","declareTargets"
#define GUILDBOSS_START				"GuildBoss","start"
#define GUILDBOSS_GET_WOUNDED_LIST	"GuildBoss","getWoundedList"
#define GUILDBOSS_MEMBER_RANK		"GuildBoss","memberRank"
#define GUILDBOSS_GUILD_RANK		"GuildBoss","guildRank"
#define GUILDBOSS_GET_TARGET_LIST	"GuildBoss","getTargetList"
#define GUILDBOSS_GET_HATRED_LIST	"GuildBoss","getHatredList"
#define GUILDBOSS_ATTACK_BOSS		"GuildBoss","attackBoss"
#define GUILDBOSS_AUTO_ATTACK_BOSS	"GuildBoss","autoAttackBoss"
#define GUILDBOSS_ROB				"GuildBoss","rob"
#define GUILDBOSS_ROB_SETTLE		"GuildBoss","robSettle"
#define GUILDBOSS_RESCUE			"GuildBoss","rescue"
#define GUILDBOSS_GET_NOTICE		"GuildBoss","getNotice"
#define GUILDBOSS_DAILY_SETTLE		"GuildBoss","dailySettle"

#pragma mark -皇陵寻宝-
#define DRAGONVALLEY_GETSEARCH		"DragonValley","getSearch"			//获取宝库信息
#define DRAGONVALLEY_GETOCCUPY		"DragonValley","getOccupy"			//获取宝库信息
#define DRAGONVALLEY_GETOCCUPYLIST   "DragonValley","getOccupyList"  //获取宝库列表信息
#define DRAGONVALLEY_SEARCH    "DragonValley","search"           //搜索
#define DRAGONVALLEY_SPEEDUP  "DragonValley","speedUp"           //加速
#define DRAGONVALLEY_OCCUPYFINISH  "DragonValley","occupyFinish" //皇陵寻宝获取占领结束或者被抢占的宝库的奖励
#define DRAGONVALLEY_GETASSISTBONUSLIST  "DragonValley","getAssistBonusList"  //获取助战奖励列表
#define DRAGONVALLEY_GETASSISTBONUS  "DragonValley","getAssistBonus"          //获取助战奖励
#define DRAGONVALLEY_GETALLASSISTBONUS  "DragonValley","getAllAssistBonus"    //获取所有助战奖励
#define DRAGONVALLEY_BATTLESTART  "DragonValley","battleStart" //抢夺
#define DRAGONVALLEY_GETLOG  "DragonValley","getLog"           //获取采矿完成或者被抢的记录
#define DRAGONVALLEY_BATTLESTART  "DragonValley","battleStart" //抢夺
#define DRAGONVALLEY_USERRANK  "DragonValley","getMyGuildRank" //帮内成员排行
#define DRAGONVALLEY_GUILDRANK  "DragonValley","getGuildRank" //帮派排行

#pragma mark - 狩魂 -
/*狩魂*/
#define SOUL_HUNTER_GET     "Hunt","get"					//获取玩家狩魂信息
#define SOUL_HUNTER_TARGETS "Hunt","targets"				//获取目标列表
#define SOUL_HUNTER_ATTACK  "Hunt","attack"					//攻击目标


#pragma mark - 好友 -
#define FRIEND_CHALLENGE      "Friend","challenge"			//挑战好友
#define FRIEND_RECOMMEND        "Friend","recommend"            //推荐好友
#define FRIEND_SEARCH           "Friend","search"               //查找好友
#define FRIEND_SEND_REQUEST     "Friend","sendRequest"          //发送邀请
#define FRIEND_ACCEPT           "Friend","accept"               //接收邀请
#define FRIEND_REFUSE           "Friend","refuse"               //拒绝邀请
#define FRIEND_REFUSE_ALL		"Friend","refuseAll"			//拒绝所有请求
#define FRIEND_DELETE_FRIEND    "Friend","deleteFriends"         //删除好友
#define FRIEND_GET_REQUESTS		"Friend","getRequests"			//获取申请列表
#define FRIEND_SEND_MAIL		"Friend","sendMail"				//发邮件
#define FRIEND_DELETE_MAIL		"Friend","deleteMail"			//删除邮件
#define FRIEND_SEND_GIFT		"Friend","sendGift"				//送礼
#define	FRIEND_RECEIVE_GIFT		"Friend","receiveGift"			//收礼
#define FRIEND_GET_NEW_REQUEST	"Friend","getNewRequest"		//申请数量
#define FRIEND_GETFRIENDS       "Friend","getFriends"           //获取好友信息
#define FRIEND_DUNGEON          "Friend","dungeon"              //获取好友副本信息
#define FRIEND_RESET_DUNGEON    "Friend","resetDungeon"         //重置好友副本
#define FRIEND_GO               "Friend","go"                   //搞起好友副本
#define FRIEND_RUSH             "Friend","rush"                 //扫荡好友副本
#define FRIEND_RECEIVE_AND_SEND             "Friend","receiveAndSendGiftAll"                 //领取所有耐力并回赠

#pragma mark -聊天-
#define CHAT_SEND_GUILD			"Chat","sendGuild"				//发送工会聊天信息
#define CHAT_GET_GUILD			"Chat","getGuild"				//获取工会聊天信息
#define CHAT_SEND_WORLD			"Chat","sendWorld"				//发送世界聊天信息
#define CHAT_GET_WORLD			"Chat","getWorld"				//获取世界聊天信息
#define CHAT_CHK_NEW			"Chat","chkNew"					//检测新消息


#pragma mark -奇遇-
#define WONDER_TRADER			"Wonder","trader"
#define WONDER_SUPERIOR			"Wonder","superior"
#define WONDER_TREASURE			"Wonder","treasure"
#define WONDER_WARRIOR			"Wonder","warrior"
#define WONDER_FIGHT			"Wonder","fight"
#define WONDER_BOX              "Wonder","box"
#define WONDER_FIGHT_BOSS       "Wonder","boss"


#pragma mark -签到-
#define SIGNIN_GET	"Activity","getSignIn"
#define SIGNIN_SIGNIN	"Activity","signIn"

#define ACTIVITY_ONLINEAWARD "Activity","onlineGift"

#pragma mark -成就-
#define TASK_GETAWARD  "Task","getAward"
#define TASK_GETSCOREAWARD  "Task","getScoreAward"
#define TASK_GETLIST  "Task","lists"

#pragma mark -login-
#define LOGIN_REGISTER  "UserRegister","regWegame"
#define LOGIN_RESETPW   "UserRegister","resetWegamePassword"
#define BIND_ACCOUNT  "UserRegister","bindAccount"

#pragma mark --Rank---
#define RANK_GETINFO "Rank","lists"

#define USER_MYSERVETLIT "UserServer","get"

#pragma mark -小伙伴-
#define FORMATION_GETGUNDAM "Formation","getGundam"
#define FORMATION_JOINMASTER "Formation","joinMaster"
#define FORMATION_CHANGEMASTER "Formation","changeMaster"

#endif
