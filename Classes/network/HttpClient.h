//
//  HttpClient.h
//  通信模块
//
//  Created by wanghejun on 13-4-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__HttpClient__
#define __QSMY__HttpClient__

#include "AppInclude.h"
#include "utils/json_serializer.h"
#include "HTTP_API.h"
#include "CCLuaEngine.h"
#include "managers/DataManager.h"
#include "BusyLayer.h"
#include "ErrorCode.h"
#include "HttpHandler.h"

#define HTTP_CLIENT GameManager::sharedGameManager()->getHttpClient()


class HttpRequest;

class HttpClient : public cocos2d::CCObject {

    CC_SYNTHESIZE_PASS_BY_REF(std::string, _url, URL);
    
private:
    cocos2d::extension::CCHttpClient    *http;
    HttpHandler                         *m_pHttpHandler;
    int                                 m_nScriptHandler;
    CC_SYNTHESIZE_READONLY(BusyLayer*, m_pBusyLayer, BusyLayer);
    
    cocos2d::CCDictionary               *m_pFailedRequestList;
    
private:
    
    void onHttpRequestCompleted(cocos2d::extension::CCHttpClient *sender, CCHttpResponse *response);
    
    // void attachCookie(cocos2d::extension::CCHttpRequest*);
    void onBattleRequestCompleted(cocos2d::extension::CCHttpClient *sender, CCHttpResponse *response);
    
    void onHttpDataRequestCompleted(cocos2d::extension::CCHttpClient *sender, CCHttpResponse *response);
   
public:
    
    HttpClient();
    virtual ~HttpClient();
    
    CREATE_FUNC(HttpClient);
    virtual bool init();
    
    void setHttpHandler(HttpHandler *var);
	HttpHandler* getHttpHandler()
	{
		return m_pHttpHandler;
	};

    virtual void setHttpHandler(int nHandler);
    
    void retryFailedRequest();
    
    void reset();
    
    std::string getShareSign(const char* parameter = NULL);
public:
    void login(const char* parma,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL);
	void resetPassTW(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector, json_serializer &a);
	void registerUserTW(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector, json_serializer &a);
	void bindAccount(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector, json_serializer &a);
	void resetPass(const char* parma,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL);
	void registerUser(const char* param,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL);
	void loginSetTokken(const char* parma,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL);
    void getServerList(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector);
	void getAnnounce(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector);
    void getMyServerList(const char* param,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector);
    void getVersion(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector);
    void getFileVersion(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,const char* version_url);
public:
    void doRequset(HttpRequest* request);
    void doRequset(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData,const char* s,const char* m,json_serializer &a,bool showBusy = true);
    void doRequset(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData,const char* s,const char* m,bool showBusy = true);
    void doBattleRequset(cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData,const char* s,const char* m,json_serializer &a,bool showBusy = true);
public:
    
    void requestFromLua(cocos2d::LUA_FUNCTION listener,
                        const char* s,const char* m,const char* a, bool inBattle = false, bool autoSaveData = true,bool showBusy = true);
   
#pragma mark----------------成就系統----------------------
	void getTaskAward(uint32_t task_id , cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);
	void getTaskScoreAward(uint32_t index , cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);
	void getTaskList(cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);

#pragma mark----------------用户信息----------------------
	/******************  user info ***********************/
	
	/*获取用户信息*/
	void getUserInfo(cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);
    
    void registerToPlat(cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);
    
	void randUserName(cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);
    
	void userPrivileges(cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);

#pragma mark----------------阵容----------------------
    /******************  formation ***********************/
   
	void getFormations(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
	/**
     *	@brief	加入阵型
     *
     *	@param 	wid 	弟子ID
     *	@param 	id      阵型固定ID
     */
    void joinFormation(uint32_t wid,uint32_t id,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    
    /**
     *	@brief	更换阵型
     *
     *	@param 	id1 	阵型固定ID
     *	@param 	id2 	阵型固定ID
     */
    void changeFormation(uint32_t id1,uint32_t id2,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
	/**
     *	@brief	查看其他人阵型
     *
     *	@param 	othid 	其他人用户ID
     */
    void getOtherFormation(uint32_t othid,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    

    /**
     *	@brief	装备装配
     *
     *	@param 	fid 	阵型固定ID
     *	@param 	eid 	装备唯一ID
     */
    void equipWarrior(uint32_t fid,uint32_t eid,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/**
     *	@brief	批量装备装配
     *
     *	@param 	fid 	阵型固定ID
     *	@param 	ids 	装备唯一ID列表
     */
    void quickEquipWarrior(uint32_t fid,Json::Value& eids,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
	
	/**
     *	@brief	升级弟子技能
     *	@param 	id 	弟子ID
     */
	void upgradeSkill(uint32_t id,cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

#pragma mark----------------弟子----------------------
    /******************  warrior ***********************/

	/*获取弟子列表*/
	void getWarriors(cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
    
	/**
     *	@brief	获取弟子信息
     *	@param 	id 	弟子ID
     */
	void getWarrior(uint32_t id,cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

	/**
     *	@brief	弟子突破
     *	@param 	id 	弟子ID
     */
	void promoteWarrior(uint32_t id,cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

	/**
     *	@brief	弟子传功
     *	@param 	fid	传出弟子ID
	 *	@param 	tid	接受弟子id
	 *	@param 	tyep 普通传功还是高级传功 0 普通 1高级
     */
	void inheritWarrior(uint32_t fid,uint32_t tid, uint32_t type, cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

	/**
     *	@brief	弟子传功
     *	@param 	fid	传出弟子ID
	 *	@param 	tid	接受弟子id
	 *	@param 	tyep 普通传功还是高级传功 0 普通 1高级
     */
	void upgradeWarrior(uint32_t warriorId, uint32_t itemId, uint32_t itemCount, cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

	/**
     *	@brief	弟子培养
     *	@param 	id 	弟子ID
	 *	@param 	tyep 培养类型：1 普通培养 2 点券培养
     */
	void developWarrior(uint32_t id, uint32_t type, cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

	/**
     *	@brief	确认或取消弟子培养   
     *	@param 	id 	弟子ID
	 *	@param 	act 操作类型：1 确认 2 取消
     */
	void confDevelopWarrior(uint32_t id, uint32_t act, cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

    /**
     *	@brief	穿时装
     *	@param 	wid         弟子ID
     *  @param 	dressId 	时装编号 0表示脱掉时装
     */
    void setDress(uint32_t wid,uint32_t dressId,cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);
    
    /**
     *	@brief	脱时装
     *	@param 	wid         弟子ID
     */
    void unDress(uint32_t wid,cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);

#pragma mark----------------魂魄----------------------
	/**
     *	@brief	招募魂魄
     *	@param 	id 	弟子ID
     */
	void recruitSoul(uint32_t id,cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true);


	/*获取魂魄列表*/
	void getSouls(cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
    
	/**
     *	@brief	获取弟子的魂魄
     *	@param 	id 	弟子ID
     */
	void getSoul(uint32_t id,cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
    
    

#pragma mark----------------装备----------------------
    /******************  equip ***********************/
    
	/*获取装备列表*/
	void getEquips(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	
	/**
     *	@brief	获取装备信息
     *	@param 	id 	装备ID
     */
    void getEquip(uint32_t id,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
	/**
	 *	@brief	装备强化
	 *	@param 	id 	装备ID
	 */
	void enhanceEquip(uint32_t id,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/**
	 *	@brief	装备自动强化
	 *	@param 	id 	装备ID
	 */
	void autoEnhanceEquip(uint32_t id,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/**
	 *	@brief	装备强化
	 *	@param 	ids 	装备ID数组
	 */
	//void sellEquips(uint32_t ids,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void sellEquips(Json::Value& items,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	/**
	 *	@brief	装备精炼
	 *	@param 	ids 	装备ID
	 *  @param  items	材料数组
	 */
	void refineEquip(uint32_t id, uint32_t itemid,uint32_t num, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/**
	 *	@brief	装备分解
	 *  @param  items	装备数组
	 */
	void decomposeEquip(Json::Value& items, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/**
	 *	@brief	宝物吞噬
	 */
	void treasureBurn(unsigned int id , Json::Value& items,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

#pragma mark----------------背包----------------------  
    /******************  inventory ***********************/
    
	/*获取道具列表*/
	void getItems(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	/**
     *	@brief	获取道具信息
     *	@param 	id 	道具ID
     */
    void getItem(uint32_t id,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
	/**
     *	@brief	使用道具
     *	@param 	id 	道具ID
	 *  @param 	newName 用户新的名字 --仅用于更名状道具
     */
    void useItem(uint32_t id,const char* newName = NULL,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true,uint32_t num=1);
	void useItem(uint32_t id,uint32_t num=1,const char* newName = NULL);
    
#pragma mark----------------商城----------------------
	/******************  mall ***********************/

	/*获取商品列表*/
	void getProducts(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	
	/**
     *	@brief	购买商品
     *	@param 	id 	商品ID
	 *  @param  num 数量
     */
    void buyProduct(uint32_t id,uint32_t num, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    
    /**
     *	@brief	购买体力
     *
     *	@param 	num 	个数
     */
    void buyPow(uint32_t num, cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector,bool autoSaveData);
    
    /**
     *	@brief	购买红包内的商品
     *
     *	@param 	redPacketId 	红包id
     *	@param 	productId       商品id
     */
    void buy200RedPacketProduct(unsigned int redPacketId, unsigned int productId, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

    
#pragma mark----------------PVE----------------------    
    /******************  PVE ***********************/
    /**
     *	@brief	获取PVE探索列表
     *	@param 	cid 	章节ID
     */
    void getMaps(uint32_t cid,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

    /**
     *	@brief	执行探索
     *	@param 	cid 	关卡ID
     */
    void enterMap(uint32_t mid,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    void getChapters(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

    void getGradeAward(uint32_t cid,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void getElites(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	收取章节宝箱
     *	@param 	cid 	章节ID
     *	@param 	vip 	领取vip等级的奖励
     */
    void collectChapterBox(uint32_t cid,uint8_t vip);

	//扫荡
	void pve_rush(uint32_t mid, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void pve_rush_clearcd(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void pve_onceMore(uint32_t mid, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//副本活动通关奖励
	void getPveActivityAward(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

#pragma mark----------------杯赛----------------------
	/******************  CUP杯赛 ***********************/

	/*杯赛初始化*/
	void initCup(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
	/*获得杯赛个人信息*/
    void getCupInfo(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/*获得杯赛个人历史*/
	void getCupProcess(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/*获得杯赛对阵列表*/
	void getCupMatchTable(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/*获得杯赛竞猜记录*/
	void getCupBets(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/**
     *	@brief	获得比赛战报
     *	@param 	finalNum	几强赛(8/4/2)
	 *  @param  mid	比赛ID
     */
	void getCupMatchReplay(uint32_t finalNum, uint32_t mid, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/*报名杯赛*/
	void signUpCup(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

    /**
     *	@brief	获得比赛奖励
     *	@param 	finalNum	几强赛(8/4/2)
	 *  @param  mid	比赛ID
     */
	void getCupMatchPrize(uint32_t finalNum, uint32_t mid, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

    /**
     *	@brief	杯赛竞猜
     *	@param 	finalNum	几强赛(8/4/2)
	 *  @param  mid	比赛ID
	 *  @param  betUser	竞猜胜利方 1:一号玩家 2:二号玩家
     */
	void doCupBet(uint32_t finalNum, uint32_t mid, uint32_t betUser, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/**
     *	@brief	获得竞猜奖励
     *	@param 	id	竞猜ID
     */
	void getCupBetPrize(uint32_t id, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

#pragma mark----------------论剑---------------------- 
	/******************  论剑 ***********************/

	/*初始化*/
	void initPvpInfo(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
	/*获得论剑信息*/
    void getPvpInfo(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/**
     *	@brief	获得奖励
	 *  @param  rank    自己排名
     *	@param 	reward	奖励编号
     */
	void getPvpAward(uint32_t rank, uint32_t reward, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/*获得对手列表*/
	void getPvpList(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/**
     *	@brief	挑战对手
	 *  @param  tid     对手id
     *	@param 	tRank	对手排名
	 *  @param  sRank	自己排名
     */
	void chPvpTarget(uint32_t tid, uint32_t tRank,uint32_t sRank, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

    /**
     *	@brief	挑战试炼木人
	 *  @param  rank     自己排名
     */
	void chPvpNPC(uint32_t rank, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	 /**
     *	@brief	挑战东皇太一
	 *  @param  rank     自己排名
     */
	void chPvpBoss(uint32_t rank, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void challengeRefresh(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void challengeExchange(uint32_t idx, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

    
#pragma mark----------------千层塔----------------------
	/******************  千层塔 ***********************/
	//千层塔 begin
	void towerGet(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void towerFight(uint32_t, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL);
	void towerStart(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void towerSetBuff(uint32_t, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void towerGoon(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void towerEnd(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void towerRank(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void towerOldRank(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void towerReward(unsigned int id, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//end

#pragma mark----------------天命---------------------- 
	/******************  天命 ***********************/
    /**
     *	@brief	获取天命列表
     */
	void getDestinys(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	提升天命
     *
     *	@param 	did 	天命编号
     *	@param 	wid     消耗的弟子编号
     *	@param 	soul    消耗的魂魄编号
     */
    void improveDestinys(uint32_t did, Json::Value &warriors , Json::Value &souls , cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL);
    
    
    /**
     *	@brief	领取奖励
     *
     *	@param 	did 	天命编号
     */
    void destinyReward(uint32_t did,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

    
#pragma mark----------------活跃度----------------------     
    /******************  活跃度 ***********************/
    /**
     *  @breif  获取活跃度
     */
    void getActiveness(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	
	void getActivenessAward(uint32_t point,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

#pragma mark----------------登陆奖励---------------------- 
    /******************  登陆奖励 ***********************/
    /**
     *  @breif  
     */
	void getLoginRewardInfo(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void getLoginReward(uint32_t id, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

#pragma mark----------------邮件---------------------- 
	/******************  邮件 ***********************/
    /**
     *  @breif  获取邮件列表
     */
    void getMails(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	
	 /**
     *	@brief	获取邮件附件
     *
     *	@param 	id 	邮件id
     */
	void pickUpMail(uint32_t id,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/**
     *  @breif  获取新邮件数
     */
    void getNewMails(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true,bool showBusy = true);
	
		/**
     *  @breif  领取所有附件
     */
    void getAllMailsItem(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true,bool showBusy = true);
	
	 /**
     *	@brief	删除邮件
     *
     *	@param 	id 	邮件id
     */
	void deleteMail(cocos2d::CCArray *pMails,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
#pragma mark----------------抽卡----------------------    
    /******************  抽卡 ***********************/
    
    /**
     *  @breif  获取抽卡信息
     */
	void getCardInfo(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    
    /**
     *	@brief	抽卡
     *
     *	@param 	type 	抽卡类型 1:百里挑一 2: 千载难逢 3: 万众瞩目
     *	@param 	cost 	是否付费
     */
    void drawCard(card::CardType type,bool cost,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    
#pragma mark----------------许愿----------------------
    /******************  许愿 ***********************/
    
    /**
     *	@brief	获取许愿数据
     *
     *	@param 	NULL
     *	@param 	NULL
     *	@param 	true
     */
    void getWishesInfo(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	执行许愿
     *
     *	@param 	wishes 	许愿列表
     *	@param 	isTen 	是否十次
     */
    void wish(cocos2d::CCArray * wishes, bool isTen, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
	void wishChangeWarrior(cocos2d::CCArray *wishes, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

#pragma mark----------------炼化----------------------

	//获取自动炼化列表 参数为获取的个数
	void getAutoList(uint32_t num , cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);
	//炼化 传入warrior id数组
	void burn(cocos2d::CCArray * list ,cocos2d::CCArray* soulList, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true,bool showBusy = true);

#pragma mark----------------重生----------------------
	void getRebirthDrop(uint32_t wid,cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);
	void reBirth(uint32_t wid,cocos2d::CCObject* pSelectorTarget = NULL,SEL_CallFuncJson selector = NULL, bool autoSaveData = true,bool showBusy = true);

#pragma mark----------------新手引导----------------------
    /******************  新手引导 ***********************/
    /**
     *	@brief	获取新手数据
     *
     *	@param 	NULL
     *	@param 	NULL
     *	@param 	true
     */
    void getRookieInfo(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    
    /**
     *	@brief	完成新手
     *
     *	@param 	id 	新手编号
     *	@param 	wid 弟子编号//	选弟子步骤选择的弟子，仅该步需要此字段
     */
    void finishRookie(uint32_t id, uint32_t wid=0, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	完成新手
     *
     *	@param 	id 	新手编号
     *	@param 	wid 名字//	起名字，仅该步需要此字段
     */
    void finishRookie(uint32_t id, const char* name, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    
#pragma mark----------------奖励页面----------------------
    void activityGetAward(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	领取累计登录奖励
     *
     *	@param 	days 	登录天数
     */
    void activityTLoginAward(uint32_t days, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	领取等级奖励
     *
     *	@param 	lv 	等级
     */
    void activityLvUpAward(uint32_t lv, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
        
#pragma mark -------------------累计充值--------------------
    /**
     *	@brief	领取累充奖励领取
     *
     *	@param 	awardId 	累充奖励编号
     */
    void activityChargeAward(uint32_t awardId, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
#pragma mark - 活动信息 -
	
    /**
     *	@brief	初始化活动
     */
	void activityGet(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	分享领奖
     */
    void activityShare(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);


#pragma mark - 系统 -
    /**
     *	@brief	初始化数据
     */
    void initData(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	请求特殊活动编号
     */
    void getSpecialActivity(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *  @breif  同步服务端时间
     */
    void syncServerTime(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *  @breif  获取走马灯列表
     */
    void getNotices(uint32_t list, uint32_t time, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    void checkMaint();
    
#pragma mark - 充值 -
    
    
    /**
     *	@brief	获取PP订单
     *
     *	@param 	pid 	月卡编号
     *  @param  amount  充值金额
     */
    void getChargeOrder(const char* m,uint32_t pid, const char* amount, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);


    
    /**
     *	@brief	获取充值结果
     *
     */
    void getChargeResult(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    
#pragma mark - 设置 -
    
    /**
     *	@brief	反馈
     *
     */
    void addFeedBack(const char* content, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

    /**
     *	@brief	兑换码
     *
     */
    void exchangeCode(const char* code,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    



#pragma mark - 公会 -
	void guildCreate(const char *name, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildApply(uint32_t guildID, uint32_t action, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildDealApply(uint32_t appID, uint32_t action, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildGet(cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildGetList(uint32_t guildNum, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildSearch(const char *name, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildPosition(uint32_t userID, uint32_t positionID, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildQuit(cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildSign(cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildQues(uint32_t answerID, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildDetails(uint32_t guildID, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildUpdate(const char *content, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildApplyList(cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildUpgradeTree(uint32_t type, uint32_t num, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildGetNotice(uint32_t noticeType, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildMsg(uint32_t userID, const char *msg, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	// 【帮会商店】刷新商店
	void guildMallRefresh( cocos2d::CCObject *pSelectorTarget = NULL, 
		SEL_CallFuncJson selector = NULL, bool autoSaveData = true ) ;
	// 【帮会商店】兑换物品
	void guildMallExchange( uint32_t exchangeID, cocos2d::CCObject *pSelectorTarget = NULL, 
		SEL_CallFuncJson selector = NULL, bool autoSaveData = true ) ;
	//取代会长
	void guildSubstitute(cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildLog(cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

#pragma mark - 公会战 -
	void guildBossGet(cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossDeclareWar(uint32_t guildID, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossDeclareTargets(uint32_t selfGuildID, uint32_t guildID, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossStart(cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossGetWoundedList(uint32_t selfGuildID, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossMemberRank(uint32_t selfGuildID, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossGuildRank(uint32_t selfGuildID, uint32_t guildID, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossGetTargetList(uint32_t selfGuildID, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossGetHatredList(uint32_t selfGuildID, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossAttackBoss(bool isPayed, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossAutoAttackBoss(cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossRob(bool isPayed, uint32_t tid, uint32_t dmg, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossRobSettle(cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossRescue(uint32_t userID, bool isAll, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossGetNotice(uint32_t selfGuildID, uint32_t time, cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	void guildBossDailySettle(cocos2d::CCObject *pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

#pragma mark-----------------皇陵宝藏---------------------

	//【皇陵寻宝】获取搜索信息
	void getSearchDragonValley(unsigned int userId, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/);
	//皇陵寻宝】获取宝库信息
	void getOccupyDragonValley(unsigned int userId, cocos2d::CCObject* pSelectorTarget /*= NULL*/, SEL_CallFuncJson selector /*= NULL*/,bool autoSaveData /*= true*/);
	//【皇陵寻宝】获取宝库列表信息
	void getOccupyListDragonValley(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//【皇陵寻宝】搜索
	void searchDragonValley(unsigned int pos, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//皇陵寻宝】加速
	void speedUpDragonValley(unsigned int pos,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//【皇陵寻宝】获取占领结束或者被抢占的宝库的奖励
	void occupyFinishDragonValley(unsigned int pos, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//【皇陵寻宝】获取助战奖励列表
	void getAssistBonusListDragonValley(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//【皇陵寻宝】获取助战奖励
	void getAssistBonusDragonValley(unsigned int id, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//【皇陵寻宝】获取所有助战奖励
	void getAllAssistBonusDragonValley(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//【皇陵寻宝】抢夺
	void battleStartDragonValley(unsigned int pos,Json::Value& assist,  cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//【皇陵寻宝】获取采矿完成或者被抢的记录
	void getLogDragonValley(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//【皇陵寻宝】个人排行
	void getUserRank(unsigned int index = 1 , cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	//【皇陵寻宝】公会排行
	void getGuildRank(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	#pragma mark - 狩魂 -
    
    /**
     *	@brief	获取玩家狩魂信息
     *
     */
    void huntGet(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

    
    /**
     *	@brief	获取某一弟子的目标列表
     *
     */
    void huntTargets(uint32_t wId,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	/**
     *	@brief	攻击目标
     *
     */
    void huntAttack(uint32_t tId, uint32_t wId, uint8_t isNpc, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    #pragma mark - 好友 -
    
	/**
     *	@brief	挑战好友
     *
     */
    void challengeFriend(uint32_t friendId , cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	好友推荐
     *
     */
    void recommendFriend(unsigned int num = 0 , cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	查找好友
     *
     *	@param 	friendId 	用户编号
     */
    void searchFriend(uint32_t friendId,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	加好友
     *
     *	@param 	friendId 	用户编号
     */
    void addFriend(uint32_t friendId, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    
    /**
     *	@brief	同意加好友
     *
     *	@param 	friendId 	用户编号
     */
    void acceptFriend(uint32_t friendId, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    
    
    /**
     *	@brief	拒绝加好友
     *
     *	@param 	friendId 	用户编号
     */
    void refuseFriend(uint32_t friendId, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
	
    /**
     *	@brief	拒绝所有加好友
     *
     */
    void refuseAllFriend(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	删除好友
     *
     *	@param 	friendIds 	用户编号
     */
    void deleteFriend(unsigned int fid, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	发邮件
     *
	 *	@param 	friendId 	用户编号
	 *	@param 	msg 		邮件内容
     */
	void friendSendMail(uint32_t friendId, const char *msg, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	
	
	 /**
     *	@brief	删除邮件
     *
     *	@param 	id 	邮件id
     */
	void friendDeleteMail(uint32_t id, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	
    /**
     *	@brief	送礼
     *
	 *	@param 	friendId 	用户编号
     */
	void friendSendGift(uint32_t friendId, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

	
    /**
     *	@brief	收礼
     *
	 *	@param 	friendId 	用户编号
     */
	void friendReceiveGift(uint32_t friendId, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
	
    /**
     *	@brief	申请数量
     *
     */

	void friendGetNewRequest(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

    /**
     *	@brief	获取好友
     *
     */
    void getFriends(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	获取好友申请列表
     *
     */
    void getFriendRequests(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    /**
     *	@brief	获取好友副本
     *
     */
    void FriendDungeon(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    
    /**
     *	@brief	重置好友副本
     *
     *	@param 	id 	副本编号
     */
    void resetFriendDungeon(uint32_t dungeonId,cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

    
    /**
     *	@brief	进去好友副本战斗
     *
     *	@param 	dungeonId 	好友副本编号
     *	@param 	friendId 	用户编号
     *	@param 	friendWid 	好友弟子编号
     *
     */
    void goFriendDungeonBattle(uint32_t dungeonId,uint32_t friendId,uint32_t friendWid, cocos2d::CCDictionary *pWarriors, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
    
    /**
     *	@brief	扫荡好友副本
     *
     *	@param 	dungeonId 	好友副本编号
     *	@param 	friendId 	用户编号
     */
    void rushFriendDungeonBattle(uint32_t dungeonId,uint32_t friendId, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);
    
	/**
     *	@brief	接收所有的耐力并回赠（new）
     *
     */
	void receiveAndSendGiftAll(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL,bool autoSaveData = true);

#pragma mark----------------聊天----------------------  
	
	/**
     *	@brief	发送工会聊天信息
     *
     *	@param 	msg 	聊天内容
     */
	void chatSendGuild(const char* msg, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

	/**
     *	@brief	获取工会聊天信息
     *
     *	@param 	gid 	工会编号
     *	@param 	id 	请求的下一条消息编号
     */
	void chatGetGuild(unsigned int gid, unsigned int id, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

	/**
     *	@brief	发送世界聊天信息
     *
     *	@param 	msg 	聊天内容
     */
	void chatSendWorld(const char* msg, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

	/**
     *	@brief	获取世界聊天信息
     *
     *	@param 	id 	请求的下一条消息编号
     */
	void chatGetWorld(unsigned int id, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

	/**
     *	@brief	检测聊天信息
     *
     *	@param 	wLid 	请求的全服聊天下一条消息编号
	 *	@param  gLid	请求的公会聊天下一条消息编号
	 *	@param  gid		公会编号
     */
	void chatCheckNew(unsigned int wLid, unsigned int gLid, unsigned int gid, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);


#pragma mark----------------------奇遇-----------------
	void wonderTrader(unsigned int id, unsigned int buy, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
	void wonderSuperior(unsigned int id, unsigned int dbl, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
	void wonderTreasure(unsigned int id, unsigned int dbl, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
	void wonderWarrior(unsigned int id, unsigned int buy, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
	void wonderFight(unsigned int id, unsigned int gg, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
	void wonderBox(unsigned int id, unsigned int buy, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
    void wonderFightBoss(unsigned int id, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
#pragma mark----------------------签到-----------------
	void signinGet(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
	void signinSignin(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
#pragma mark----------------------在线奖励-----------------
	void getOnlineAward(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

#pragma mark ----------------排行榜------------------------
	void getRankInfo(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);

#pragma mark ----------------小伙伴------------------------
	void getGundam(cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true); 
	void joinGundam(uint32_t posId, uint32_t wId, cocos2d::CCObject* pSelectorTarget = NULL, SEL_CallFuncJson selector = NULL, bool autoSaveData = true);
	
};







class HttpRequest :public cocos2d::extension::CCHttpRequest {
public:
     
	HttpRequest()
    :m_pTarget(NULL)
    ,m_bAutoSaveData(true)
    ,m_listener(0)
    {
        static uint32_t uuid = 0;
        m_nUUID = ++uuid;
    };

    virtual ~HttpRequest()
    {
        CC_SAFE_RELEASE(m_pTarget);
    }
    
    /** Required field. You should set the callback selector function at ack the http request completed
     */
    inline void setCallback(CCObject* pTarget, SEL_CallFuncJson pSelector)
    {
        CC_SAFE_RETAIN(pTarget);
        CC_SAFE_RELEASE(m_pTarget);
        m_pTarget = pTarget;
        m_pSelector = pSelector;
    }
    
    
    
    /** Required field. You should set the callback selector function at ack the http request completed
     */
    inline void setCallback(cocos2d::LUA_FUNCTION listener)
    {
        m_listener =listener;
    }
    
    /**
     *	@brief	回调
     *
     *	@param 	val 	返回数据
     *
     *	@return	返回是否继续执行数据处理方法
     */
    void doCallback(Json::Value &requestData,Json::Value &responseDat)
    {
        if(m_pTarget)
        {
            (m_pTarget->*m_pSelector)(requestData,responseDat);
        }
	}

    void doCallbackLua(std::string& request,std::string &response)
    {
        if (m_listener)
        {
            //cocos2d::CCLuaValueDict dict;
            //dict["request"] = cocos2d::CCLuaValue::stringValue(request);
            //dict["response"] = cocos2d::CCLuaValue::stringValue(response);
            cocos2d::CCLuaStack* stack = cocos2d::CCLuaEngine::defaultEngine()->getLuaStack();
            stack->clean();
            //stack->pushCCLuaValueDict(dict);
            stack->pushString(request.c_str());
            stack->pushString(response.c_str());
            stack->pushBoolean(m_bAutoSaveData);
            stack->executeFunctionByHandler(m_listener, 3);
        }
    }
     
    CC_SYNTHESIZE(bool, m_bAutoSaveData, AutoSaveData);//是否自动保存返回值
    CC_SYNTHESIZE(bool, m_bShowBusy, ShowBusy);//是否显示忙碌状态
    
    /*获取请求唯一id*/
    uint32_t getUUID()
    {
        return m_nUUID;
    }
    
private:
    CCObject*          m_pTarget;    
    SEL_CallFuncJson     m_pSelector;
    cocos2d::LUA_FUNCTION m_listener;
    uint32_t            m_nUUID;
};

#endif /* defined(__QSMY__HttpClient__) */
