//
//  HttpHandler.h
//  通讯回调处理
//
//  Created by wanghejun on 13-4-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__HttpHandler__
#define __QSMY__HttpHandler__
#include "AppInclude.h"
#include "json/json.h"
#include "ErrorCode.h"
#include "HTTP_API.h"

class GameManager;
class DataManager;

class HttpHandler: public cocos2d::CCObject{
    
    CC_SYNTHESIZE(time_t , last_response_time, LastResponseTime); //与服务器最后通讯时间
    
public:
    HttpHandler();
    CREATE_FUNC(HttpHandler);
    virtual bool init();
    void parseResult(const Json::Value &requestData,const Json::Value &responseData,bool autoSave);
public:
    
    void onBattleData(std::vector<char>& vec);
    
    void startExplore(const Json::Value &requestData,const Json::Value &responseData);

	//pvp挑战
    void chPvpBoss(const Json::Value &requestData,const Json::Value &responseData);
	void chPvpNPC(const Json::Value &requestData,const Json::Value &responseData);
	void chPvpTarget(const Json::Value &requestData,const Json::Value &responseData);

private:
    
    void parseRequest(const Json::Value &requestData,const Json::Value &responseData);
    void parseData(const Json::Value &requestData,const Json::Value &responseData);
    void parseError(const Json::Value &requestData,const Json::Value &responseData);
    
private:
    std::map<std::string, SEL_CallFuncJson> parseFuns;      //处理返回数据(根据返回数据key处理的写这里)
    std::map<std::string, SEL_CallFuncJson> errorFuns;      //处理错误数据
    std::map<std::string, SEL_CallFuncJson> exeFuns;        //处理请求数据(根据请求方法名处理的写这里)


private:

	void user(const Json::Value &requestData,const Json::Value &responseData);          //玩家信息
    void formations(const Json::Value &requestData,const Json::Value &responseData);    //阵型整体信息
    void destinies(const Json::Value &requestData,const Json::Value &responseData);      //天命信息

    void otherFormation(const Json::Value &requestData,const Json::Value &responseData); //其它玩家阵型整体信息

	void drops(const Json::Value &requestData,const Json::Value &responseData);			//掉落
	void drop(const Json::Value &requestData,const Json::Value &responseData);			//掉落
    void saveDrops(const Json::Value &requestData,const Json::Value &responseData);     //保存掉落数据

	void coinChanged(const Json::Value &requestData,const Json::Value &responseData);	//得到金币变化（增（减）量）
    void getPoint(const Json::Value &requestData,const Json::Value &responseData);		//刷新点券

    void equip(const Json::Value &requestData,const Json::Value &responseData);         //单个装备
    void equips(const Json::Value &requestData,const Json::Value &responseData);        //装备列表
	void enhanceEquip(const Json::Value &requestData,const Json::Value &responseData);	//强化装备
	void sellEquips(const Json::Value &requestData,const Json::Value &responseData);	//出售装备
	void treasureBurn(const Json::Value &requestData,const Json::Value &responseData);	//出售装备
	void refineEquip(const Json::Value &requestData,const Json::Value &responseData);	//精炼装备

    void item(const Json::Value &requestData,const Json::Value &responseData);          //单个道具信息
    void items(const Json::Value &requestData,const Json::Value &responseData);         //道具列表
    void item_change(const Json::Value &requestData,const Json::Value &responseData);   //道具数量变化
    void items_change(const Json::Value &requestData,const Json::Value &responseData);   //道具数量变化
	void useItem(const Json::Value &requestData,const Json::Value &responseData);		//使用道具

	void products(const Json::Value &requestData,const Json::Value &responseData);      //商品列表

	void warrior(const Json::Value &requestData,const Json::Value &responseData);       //单个弟子信息
	void warriors(const Json::Value &requestData,const Json::Value &responseData);      //弟子列表
	void inheritWarrior(const Json::Value &requestData,const Json::Value &responseData);//传功
	//void developWarrior(const Json::Value &requestData,const Json::Value &responseData);//培养
	void confirmDevWarrior(const Json::Value &requestData,const Json::Value &responseData);//确认or取消培养
	void promoteWarrior(const Json::Value &requestData,const Json::Value &responseData);//突破
	void recruitSoul(const Json::Value &requestData,const Json::Value &responseData);	//魂魄招募

	void potBuffs(const Json::Value &requestData,const Json::Value &responseData);		//培养加成
	void potBuff(const Json::Value &requestData,const Json::Value &responseData);		//培养加成

	void soul(const Json::Value &requestData,const Json::Value &responseData);			//单个魂魄信息
	void souls(const Json::Value &requestData,const Json::Value &responseData);			//魂魄列表
	
    void explores(const Json::Value &requestData,const Json::Value &responseData);      //玩家PVE探索信息列表
    void explore(const Json::Value &requestData,const Json::Value &responseData);       //玩家PVE探索关卡详细信息
    void chapterCollectBox(const Json::Value &requestData,const Json::Value &responseData);    //pve领奖
    void pveActivityAward(const Json::Value &requestData,const Json::Value &responseData);    //副本活动通关奖励
    
    void chapters(const Json::Value &requestData,const Json::Value &responseData);      //章节列表信息
    void chapter(const Json::Value &requestData,const Json::Value &responseData);       //某一章节信息
	void rush(const Json::Value &requestData,const Json::Value &responseData);			//扫荡
    void onceMore(const Json::Value &requestData,const Json::Value &responseData);      //强行探索

    //void cookie(const Json::Value &requestData,const Json::Value &responseData);      //登陆信息

#pragma mark 成就
	void tasks(const Json::Value &requestData,const Json::Value &responseData);        //成就列表
	void achievement_msg(const Json::Value &requestData,const Json::Value &responseData);        //成就更新響應    
	void taskGetAward(const Json::Value &requestData, const Json::Value &responseData);	//成就单个奖励
	void taskGetScoreAward(const Json::Value &requestData, const Json::Value &responseData);	//成就积分奖励
    
    void privileges(const Json::Value &requestData,const Json::Value &responseData);
    void dresses(const Json::Value &requestData,const Json::Value &responseData);      //时装
    
	void soulExchange(const Json::Value &requestData,const Json::Value &responseData);	//魂魄兑换
#pragma mark 获取特权信息
	void userPrivileges(const Json::Value &requestData,const Json::Value &responseData);
#pragma mark 魂魄
    void getSouls(const Json::Value &requestData,const Json::Value &responseData);		//获取魂魄列表

#pragma mark 杯赛
	void cup(const Json::Value &requestData,const Json::Value &responseData);			//报名信息
	void process(const Json::Value &requestData,const Json::Value &responseData);       //比赛历史列表
	void mTable(const Json::Value &requestData,const Json::Value &responseData);        //比赛对阵列表
	void bet(const Json::Value &requestData,const Json::Value &responseData);			//竞猜记录
	void bets(const Json::Value &requestData,const Json::Value &responseData);			//竞猜记录列表
    void signUp(const Json::Value &requestData,const Json::Value &responseData);        //报名
    void cupMReward(const Json::Value &requestData,const Json::Value &responseData);    //领取杯赛比赛奖励
    void cupMRep(const Json::Value &requestData,const Json::Value &responseData);       //获得比赛战报(含二进制数据)
    void cupBet(const Json::Value &requestData,const Json::Value &responseData);        //竞猜
    void cupInit(const Json::Value &requestData,const Json::Value &responseData);		//杯赛初始化

#pragma mark 论剑
    void pvpInit(const Json::Value &requestData,const Json::Value &responseData);       //论剑初始化
    void pvpInfo(const Json::Value &requestData,const Json::Value &responseData);       //获取论剑信息
    void pvpTargets(const Json::Value &requestData,const Json::Value &responseData);    //获取论剑目标列表
	void challenge(const Json::Value &requestData,const Json::Value &responseData);		//论剑信息
	void chRanks(const Json::Value &requestData,const Json::Value &responseData);		//论剑top10列表
	void chTargets(const Json::Value &requestData,const Json::Value &responseData);		//可看到的玩家列表
	void getPvpAward(const Json::Value &requestData,const Json::Value &responseData);	//领取论剑奖励

	void pvpSettlement(const Json::Value &requestData,const Json::Value &responseData);       //論劍結算

    ///////////////////////////////////////////////////////////////////////////////////
    
    void joinFormation(const Json::Value &requestData,const Json::Value &responseData);     //加入阵容
    void changeFormation(const Json::Value &requestData,const Json::Value &responseData);   //变换阵容
    
    void improveDestiny(const Json::Value &requestData,const Json::Value &responseData);    //提升天命
    void rewardDestiny(const Json::Value &requestData,const Json::Value &responseData);		//领取奖励
    
    void activeness(const Json::Value &requestData,const Json::Value &responseData);			//获取活跃度
	void getActivenessAward(const Json::Value &requestData,const Json::Value &responseData);  //获取活跃度奖励


	void challengeRefresh(const Json::Value &requestData,const Json::Value &responseData);
	void challengeExchange(const Json::Value &requestData,const Json::Value &responseData);
#pragma mark 千层塔
    void towerRank(const Json::Value &requestData,const Json::Value &responseData);
	void FightResult(const Json::Value &requestData,const Json::Value &responseData);
	void ChooseResult(const Json::Value &requestData, const Json::Value &responseData);

	void tower_get(const Json::Value &requestData,const Json::Value &responseData);
	void tower_add(const Json::Value &requestData,const Json::Value &responseData);	

	void towerOldRank(const Json::Value &requestData,const Json::Value &responseData);
	void tower_reward(const Json::Value &requestData,const Json::Value &responseData);

#pragma mark 登陆奖励
	void loginAward(const Json::Value &requestData,const Json::Value &responseData);

#pragma mark 邮件列表
	void mails(const Json::Value &requestData,const Json::Value &responseData);
	void getMailAwards(const Json::Value &requestData,const Json::Value &responseData);
	//一键领取的响应 触发掉落弹框
	void getAllMailAwards(const Json::Value &requestData,const Json::Value &responseData);
	void getNewMails(const Json::Value &requestData,const Json::Value &responseData);
	void mailDeleteMail(const Json::Value &requestData,const Json::Value &responseData);

#pragma mark 获得奇遇
	void getWonder(const Json::Value &requestData,const Json::Value &responseData);
 
#pragma mark 抽卡
    void card(const Json::Value &requestData,const Json::Value &responseData);
    void cards(const Json::Value &requestData,const Json::Value &responseData);
	void hardDrawNum(const Json::Value &requestData,const Json::Value &responseData);
	//响应限时英雄的 橙色次数
	void activeDrawNum(const Json::Value &requestData,const Json::Value &responseData);
	//响应抽卡结果
	void onDrawCard(const Json::Value &requestData,const Json::Value &responseData);
    
#pragma mark 许愿
    /**
     *	@brief	获取许愿信息
     *
     *	@param 	requestData 	请求数据
     *	@param 	responseData 	响应数据
     */
    void getWishes(const Json::Value &requestData,const Json::Value &responseData);
    
    /**
     *	@brief	执行许愿
     *
     *	@param 	requestData 	请求数据
     *	@param 	responseData 	响应数据
     */
    void doWish(const Json::Value &requestData,const Json::Value &responseData);
	void wishChangeWarrior(const Json::Value &requestData, const Json::Value &responseData);

#pragma mark 炼化
	//burn result
    void burn(const Json::Value &requestData,const Json::Value &responseData);
	//auto list result
	void getAutoList(const Json::Value &requestData, const Json::Value &responseData);
    
#pragma mark 新手引导
    void getRookieInfo(const Json::Value &requestData,const Json::Value &responseData);
    void finishRookie(const Json::Value &requestData,const Json::Value &responseData);
    
#pragma mark 奖励页面
    /**
     *	@brief	是否开启特殊活动入口
     *
     *	@param 	requestData 	请求
     *	@param 	responseData 	响应
     */
    void activityGetSpecial(const Json::Value &requestData,const Json::Value &responseData);

    /**
     *	@brief	获取页面数据
     *
     *	@param 	requestData 	请求
     *	@param 	responseData 	响应
     */
    void activityGetAward(const Json::Value &requestData,const Json::Value &responseData);

    /**
     *	@brief	领取累计登录奖励
     *
     *	@param 	requestData 	请求数据
     *	@param 	responseData 	响应数据
     */
    void activityTLoginAward(const Json::Value &requestData,const Json::Value &responseData);
    
    /**
     *	@brief	领取等级奖励
     *
     *	@param 	requestData 	请求数据
     *	@param 	responseData 	响应数据
     */
    void activityLvUpAward(const Json::Value &requestData,const Json::Value &responseData);

#pragma mark 分享领奖
    
    /**
     *	@brief	分享领奖
     *
     *	@param 	requestData 	请求数据
     *	@param 	responseData 	响应数据
     */
    void activityShareAward(const Json::Value &requestData,const Json::Value &responseData);
    
#pragma mark 累充页面（临时）
    /**
     *	@brief	获取累充页面数据
     *
     *	@param 	requestData 	请求
     *	@param 	responseData 	响应
     */
    void activityGetChargeAward(const Json::Value &requestData,const Json::Value &responseData);
    
    /**
     *	@brief	领取累计充值奖励
     *
     *	@param 	requestData 	请求数据
     *	@param 	responseData 	响应数据
     */
    void activityChargeAward(const Json::Value &requestData,const Json::Value &responseData);

#pragma mark 系统
    /**
     *	@brief	初始化数据
     *
     *	@param 	requestData 	请求数据
     *	@param 	responseData 	响应数据
     */
    void initData(const Json::Value &requestData,const Json::Value &responseData);
    
    /**
     *	@brief	获取通知
     *
     *	@param 	requestData 	请求数据
     *	@param 	responseData 	响应数据
     */
    void parseNotice(const Json::Value &requestData,const Json::Value &responseData);
    
    /**
     *	@brief	获取通知
     *
     *	@param 	requestData 	请求数据
     *	@param 	responseData 	响应数据
     */
    void getNotices(const Json::Value &requestData,const Json::Value &responseData);
    
#pragma mark 充值

    /**
     *	@brief	获取PP订单
     *
     */
    void getChargeOrderPP(const Json::Value &requestData,const Json::Value &responseData);


    /**
     *	@brief	获取充值结果
     *
     */
    void getChargeResult(const Json::Value &requestData,const Json::Value &responseData);
    
#pragma  mark 公会
	void guildCreate(const Json::Value &requestData, const Json::Value &responseData);
	void guildApply(const Json::Value &requestData, const Json::Value &responseData);
	void guildDealApply(const Json::Value &requestData, const Json::Value &responseData);
	void guildGet(const Json::Value &requestData, const Json::Value &responseData);
	void guildGetList(const Json::Value &requestData, const Json::Value &responseData);
	void guildSearch(const Json::Value &requestData, const Json::Value &responseData);
	void guildPosition(const Json::Value &requestData, const Json::Value &responseData);
	void guildQuit(const Json::Value &requestData, const Json::Value &responseData);
	void guildSign(const Json::Value &requestData, const Json::Value &responseData);
	void guildQues(const Json::Value &requestData, const Json::Value &responseData);
	void guildDetails(const Json::Value &requestData, const Json::Value &responseData);
	void guildUpdate(const Json::Value &requestData, const Json::Value &responseData);
	void guildApplyList(const Json::Value &requestData, const Json::Value &responseData);
	void guildUpgradeTree(const Json::Value &requestData, const Json::Value &responseData);
	void guildGetNotice(const Json::Value &requestData, const Json::Value &responseData);
	void guildMsg(const Json::Value &requestData, const Json::Value &responseData);
	void guildMallRefresh(const Json::Value &requestData, const Json::Value &responseData);
	void guildMallExchange(const Json::Value &requestData, const Json::Value &responseData);
	void guildSubstitute(const Json::Value &requestData, const Json::Value &responseData);
	void guildLog(const Json::Value &requestData, const Json::Value &responseData);

#pragma  mark 公会战
	void guildBossGet(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossDeclareWar(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossDeclareTargets(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossStart(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossGetWoundedList(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossMemberRank(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossGuildRank(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossGetTargetList(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossGetHatredList(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossAttackBoss(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossAutoAttackBoss(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossRob(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossRobSettle(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossRescue(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossGetNotice(const Json::Value &requestData, const Json::Value &responseData);
	void guildBossDailySettle(const Json::Value &requestData, const Json::Value &responseData);

#pragma mark - 皇陵 -
	void dragonvalleySearchFail(const Json::Value &requestData,const Json::Value &responseData);
	void dragonvalleyGetsearch(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleyGetoccupy(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleyGetoccupylist(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleySpeedup(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleyOccupyfinish(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleyGetassistbonuslist(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleyGetassistbonus(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleyGetallassistbonus(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleyBattlestart(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleyGetlog(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleySearch(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleyUserRank(const Json::Value &requestData, const Json::Value &responseData);
	void dragonvalleyGuildRank(const Json::Value &requestData, const Json::Value &responseData);
#pragma mark 商城
    void productBuy(const Json::Value &requestData, const Json::Value &responseData);
    void redPacketBuy(const Json::Value &requestData, const Json::Value &responseData);

#pragma mark 装备
	void decomposeEquip(const Json::Value &requestData, const Json::Value &responseData);
	

#pragma mark - 好友 - 组队副本 -	
	void friendChallenge(const Json::Value &requestData, const Json::Value &responseData);
	void friendRecommend(const Json::Value &requestData, const Json::Value &responseData);
	void friendSearch(const Json::Value &requestData, const Json::Value &responseData);
	void friendSendRequest(const Json::Value &requestData, const Json::Value &responseData);
	void friendAccept(const Json::Value &requestData, const Json::Value &responseData);
	void friendRefuse(const Json::Value &requestData, const Json::Value &responseData);
	void friendRefuseAll(const Json::Value &requestData, const Json::Value &responseData);
	void friendDeleteFriend(const Json::Value &requestData, const Json::Value &responseData);
	void friendGetRequests(const Json::Value &requestData, const Json::Value &responseData);
	void friendSendMail(const Json::Value &requestData, const Json::Value &responseData);
	void friendDeleteMail(const Json::Value &requestData,const Json::Value &responseData);
	void friendSendGit(const Json::Value &requestData, const Json::Value &responseData);
	void friendReceiveGift(const Json::Value &requestData, const Json::Value &responseData);
	void friendGetNewRequest(const Json::Value &requestData, const Json::Value &responseData);
	void friendGetFriends(const Json::Value &requestData, const Json::Value &responseData);
	void friendDungeon(const Json::Value &requestData, const Json::Value &responseData);
	void friendResetDungeon(const Json::Value &requestData, const Json::Value &responseData);
	void friendGo(const Json::Value &requestData, const Json::Value &responseData);
	void friendRush(const Json::Value &requestData, const Json::Value &responseData);
	void receiveAndSendGiftAll(const Json::Value &requestData, const Json::Value &responseData);
	
    
#pragma mark- 错误处理 -
    
    void userNotLogin(const Json::Value &requestData,const Json::Value &responseData);
    void systemMaintain(const Json::Value &requestData,const Json::Value &responseData);
    void theRoundEnded(const Json::Value &requestData,const Json::Value &responseData);
    void soulNotEnough(const Json::Value &requestData,const Json::Value &responseData);
	void soulHunterNotExist(const Json::Value &requestData,const Json::Value &responseData);
	void soulHunterNoMoreSnatch(const Json::Value &requestData,const Json::Value &responseData);
	void guildNoGuild(const Json::Value &requestData,const Json::Value &responseData);
	void guildHasGuild(const Json::Value &requestData,const Json::Value &responseData);
	void guildBossErrorUserRescued(const Json::Value &requestData,const Json::Value &responseData);
	void guildPermissionDenied(const Json::Value &requestData,const Json::Value &responseData);
	void guildBossNewJoin(const Json::Value &requestData,const Json::Value &responseData);
	void guildBossNoData(const Json::Value &requestData,const Json::Value &responseData);
	void friendsNotExist(const Json::Value &requestData,const Json::Value &responseData);
	void friendsRequestsNeedRefresh(const Json::Value &requestData,const Json::Value &responseData);
	void friendsTargetFull(const Json::Value &requestData,const Json::Value &responseData);
	void friendsGiftFull(const Json::Value &requestData,const Json::Value &responseData);
	void friendsWarriorNotExist(const Json::Value &requestData,const Json::Value &responseData);

#pragma  mark -狩魂-
	void huntGet(const Json::Value &requestData, const Json::Value &responseData);
	void huntTargets(const Json::Value &requestData, const Json::Value &responseData);
	void huntAttack(const Json::Value &requestData, const Json::Value &responseData);
    
#pragma mark -聊天-
	void chatSendGuild(const Json::Value &requestData, const Json::Value &responseData);
	void chatGetGuild(const Json::Value &requestData, const Json::Value &responseData);
	void chatSendWorld(const Json::Value &requestData, const Json::Value &responseData);
	void chatGetWorld(const Json::Value &requestData, const Json::Value &responseData);

#pragma mark -奇遇-
	void wonderTrader(const Json::Value &requestData, const Json::Value &responseData);
	void wonderSuperior(const Json::Value &requestData, const Json::Value &responseData);
	void wonderTreasure(const Json::Value &requestData, const Json::Value &responseData);
	void wonderWarrior(const Json::Value &requestData, const Json::Value &responseData);
	void wonderFight(const Json::Value &requestData, const Json::Value &responseData);
    void wonderFightBoss(const Json::Value &requestData, const Json::Value &responseData);
	void wonderBox(const Json::Value &requestData, const Json::Value &responseData);
#pragma mark -奇遇-
	void signinGet(const Json::Value &requestData, const Json::Value &responseData);
	void signinSignin(const Json::Value &requestData, const Json::Value &responseData);
    
	//在线奖励
	void onlineAward(const Json::Value &requestData, const Json::Value &responseData);
 #pragma mark -小伙伴-   
    void getGundam(const Json::Value &requestData, const Json::Value &responseData);
	void joinGundam(const Json::Value &requestData, const Json::Value &responseData);
#pragma mark -活跃度-
	void rewardGet(const Json::Value &requestData, const Json::Value &responseData);

	void equipRefineryItemNum(const Json::Value &requestData,const Json::Value &responseData);//装备炼化石值
#pragma mark 周活动
	void weeklyActivities(const Json::Value &requestData,const Json::Value &responseData);
	void weeklyActivity(const Json::Value &requestData,const Json::Value &responseData);
};
#endif /* defined(__QSMY__HttpHandler__) */
