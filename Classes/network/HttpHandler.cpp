//
//  HttpHandler.cpp
//  QSMY
//
//  Created by wanghejun on 13-4-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "HttpHandler.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "managers/AnalyticsManager.h"
#include "managers/PlatformManager.h"
#include "utils/SystemUtil.h"
#include "utils/TimerUtil.h"
#include "managers/AdsManager.h"
#include "HttpClient.h"

USING_NS_CC;

HttpHandler::HttpHandler()
:last_response_time(0)
{
    
}

bool HttpHandler::init()
{
    
    errorFuns[USER_NOT_LOGON] = callfuncJson_selector(HttpHandler::userNotLogin);
    errorFuns[SYSTEM_MAINTAIN] = callfuncJson_selector(HttpHandler::systemMaintain);
    errorFuns[TOWER_OUT_OF_DATE] = callfuncJson_selector(HttpHandler::theRoundEnded);
    errorFuns[SOUL_NOT_ENOUGH] = callfuncJson_selector(HttpHandler::soulNotEnough);
	errorFuns[SOUL_HUNTER_NOT_EXIST] = callfuncJson_selector(HttpHandler::soulHunterNotExist);
	errorFuns[SOUL_HUNTER_NO_MORE_SNATCH] = callfuncJson_selector(HttpHandler::soulHunterNoMoreSnatch);
	errorFuns[GUILD_NO_GUILD] = callfuncJson_selector(HttpHandler::guildNoGuild);
	errorFuns[GUILD_HAS_GUILD] = callfuncJson_selector(HttpHandler::guildHasGuild);
	errorFuns[GUILDBOSS_ERROR_USER_RESCUED] = callfuncJson_selector(HttpHandler::guildBossErrorUserRescued);
	errorFuns[GUILD_PERMISSION_DENIED] = callfuncJson_selector(HttpHandler::guildPermissionDenied);
	errorFuns[GUILDBOSS_NEW_JOIN] = callfuncJson_selector(HttpHandler::guildBossNewJoin);
	errorFuns[GUILDBOSS_NO_DATA] = callfuncJson_selector(HttpHandler::guildBossNoData);
	errorFuns[FRIENDS_NOT_EXIST] = callfuncJson_selector(HttpHandler::friendsNotExist);
	errorFuns[FRIENDS_TARGET_FULL] = callfuncJson_selector(HttpHandler::friendsTargetFull);
	errorFuns[FRIENDS_REQUEST_NOT_EXIST] = callfuncJson_selector(HttpHandler::friendsRequestsNeedRefresh);
	errorFuns[FRIENDS_GIFT_FULL] = callfuncJson_selector(HttpHandler::friendsGiftFull);
	errorFuns[FRIENDS_WARRIOR_NOT_EXIST] = callfuncJson_selector(HttpHandler::friendsWarriorNotExist);
#pragma mark -
    
	parseFuns["user"] = callfuncJson_selector(HttpHandler::user);
    parseFuns["formations"] = callfuncJson_selector(HttpHandler::formations);
	parseFuns["otherFormation"] = callfuncJson_selector(HttpHandler::otherFormation);
    parseFuns["destinies"] = callfuncJson_selector(HttpHandler::destinies);
    parseFuns["equip"] = callfuncJson_selector(HttpHandler::equip);
    parseFuns["equips"] = callfuncJson_selector(HttpHandler::equips);
    parseFuns["item"] = callfuncJson_selector(HttpHandler::item);
    parseFuns["items"] = callfuncJson_selector(HttpHandler::items);
    parseFuns["item_change"] = callfuncJson_selector(HttpHandler::item_change);
    parseFuns["items_change"] = callfuncJson_selector(HttpHandler::items_change);
	parseFuns["products"] = callfuncJson_selector(HttpHandler::products);
	parseFuns["activeness"] = callfuncJson_selector(HttpHandler::activeness);
    parseFuns["actvns_reward"] = callfuncJson_selector(HttpHandler::getActivenessAward);

	//成就
	parseFuns["tasks"] = callfuncJson_selector(HttpHandler::tasks);
	parseFuns["achievement_msg"] = callfuncJson_selector(HttpHandler::achievement_msg);
	
	parseFuns["loginAward"] = callfuncJson_selector(HttpHandler::loginAward);

	//parseFuns["mails"] = callfuncJson_selector(HttpHandler::mails);
	parseFuns["newMail"] = callfuncJson_selector(HttpHandler::getNewMails);

    parseFuns["drops"] = callfuncJson_selector(HttpHandler::drops);
	parseFuns["drop"] = callfuncJson_selector(HttpHandler::drop);
	parseFuns["coin_change"] = callfuncJson_selector(HttpHandler::coinChanged);
    parseFuns["point"] = callfuncJson_selector(HttpHandler::getPoint);
    
    
	parseFuns["warrior"] = callfuncJson_selector(HttpHandler::warrior);
	parseFuns["warriors"] = callfuncJson_selector(HttpHandler::warriors);
	parseFuns["potBuffs"] = callfuncJson_selector(HttpHandler::potBuffs);
	parseFuns["potBuff"] = callfuncJson_selector(HttpHandler::potBuff);
	parseFuns["soul"] = callfuncJson_selector(HttpHandler::soul);
	parseFuns["souls"] = callfuncJson_selector(HttpHandler::souls);
    
    parseFuns["explores"] = callfuncJson_selector(HttpHandler::explores);
    parseFuns["explore"] = callfuncJson_selector(HttpHandler::explore);
    parseFuns["chapter"] = callfuncJson_selector(HttpHandler::chapter);
    parseFuns["chapters"] = callfuncJson_selector(HttpHandler::chapters);
    //    parseFuns["cookie"] = callfuncJson_selector(HttpHandler::cookie);
    
	parseFuns["cup"] = callfuncJson_selector(HttpHandler::cup);
	parseFuns["process"] = callfuncJson_selector(HttpHandler::process);
	parseFuns["mTable"] = callfuncJson_selector(HttpHandler::mTable);
	parseFuns["bet"] = callfuncJson_selector(HttpHandler::bet);
	parseFuns["bets"] = callfuncJson_selector(HttpHandler::bets);

	parseFuns["tower"] = callfuncJson_selector(HttpHandler::tower_get);
	parseFuns["tower_add"] = callfuncJson_selector(HttpHandler::tower_add);
    
    parseFuns["card"] = callfuncJson_selector(HttpHandler::card);
    parseFuns["cards"] = callfuncJson_selector(HttpHandler::cards);
    
    parseFuns["notice"] = callfuncJson_selector(HttpHandler::parseNotice);
	//parseFuns["chSettle"] = callfuncJson_selector(HttpHandler::chSettle);
    
    parseFuns["activities"] = callfuncJson_selector(HttpHandler::activityGetChargeAward);
    
	parseFuns["privileges"] = callfuncJson_selector(HttpHandler::privileges);
    parseFuns["dresses"] = callfuncJson_selector(HttpHandler::dresses);
    
	parseFuns["soulExchange"] = callfuncJson_selector(HttpHandler::soulExchange);

	parseFuns["pvpSettlement"] = callfuncJson_selector(HttpHandler::pvpSettlement);

	parseFuns["HardDrawNum"] = callfuncJson_selector(HttpHandler::hardDrawNum);

	parseFuns["ActiveDrawNum"] = callfuncJson_selector(HttpHandler::activeDrawNum);
	//签到初始化解析 从exe改至parse
	parseFuns["signin"] = callfuncJson_selector(HttpHandler::signinGet);	
	//在线奖励
	parseFuns["onlineGift"] = callfuncJson_selector(HttpHandler::onlineAward);	
#pragma mark -小伙伴-
	parseFuns["gundamFormation"] = callfuncJson_selector(HttpHandler::getGundam);
	//friend
	//parseFuns["friendRequests"] = callfuncJson_selector(HttpHandler::friendGetRequests);

	parseFuns["equipRefineryItemNum"] = callfuncJson_selector(HttpHandler::equipRefineryItemNum);
#pragma mark -周活动-
	parseFuns["weeklyActivities"] = callfuncJson_selector(HttpHandler::weeklyActivities);
	parseFuns["weeklyActivity"] = callfuncJson_selector(HttpHandler::weeklyActivity);

#pragma mark -

	//添加抽卡的响应回调
	exeFuns[GET_SERVICE_METHOD(CARD_DRAW)] = callfuncJson_selector(HttpHandler::onDrawCard);
	exeFuns[GET_SERVICE_METHOD(TASK_GETAWARD)] = callfuncJson_selector(HttpHandler::taskGetAward);
	exeFuns[GET_SERVICE_METHOD(TASK_GETSCOREAWARD)] = callfuncJson_selector(HttpHandler::taskGetScoreAward);
    
	exeFuns[GET_SERVICE_METHOD(USER_PRIVILEGES)] = callfuncJson_selector(HttpHandler::userPrivileges);

    exeFuns[GET_SERVICE_METHOD(SOUL_GET)] = callfuncJson_selector(HttpHandler::getSouls);
    
    exeFuns[GET_SERVICE_METHOD(FORMATION_JOIN)] = callfuncJson_selector(HttpHandler::joinFormation);
    exeFuns[GET_SERVICE_METHOD(FORMATION_CHANGE)] = callfuncJson_selector(HttpHandler::changeFormation);
	exeFuns[GET_SERVICE_METHOD(WARRIOR_INH)] = callfuncJson_selector(HttpHandler::inheritWarrior);
	//exeFuns[GET_SERVICE_METHOD(WARRIOR_DEV)] = callfuncJson_selector(HttpHandler::developWarrior);
	exeFuns[GET_SERVICE_METHOD(WARRIOR_DEV_NOT)] = callfuncJson_selector(HttpHandler::confirmDevWarrior);
	exeFuns[GET_SERVICE_METHOD(WARRIOR_UP)] = callfuncJson_selector(HttpHandler::promoteWarrior);
	exeFuns[GET_SERVICE_METHOD(SOUL_RECRUIT)] = callfuncJson_selector(HttpHandler::recruitSoul);
    
	exeFuns[GET_SERVICE_METHOD(ITEM_USEITEM)] = callfuncJson_selector(HttpHandler::useItem);
	exeFuns[GET_SERVICE_METHOD(EQUIP_AUTOENHANCE)] = callfuncJson_selector(HttpHandler::enhanceEquip);
	exeFuns[GET_SERVICE_METHOD(EQUIP_ENHANCE)] = callfuncJson_selector(HttpHandler::enhanceEquip);
	exeFuns[GET_SERVICE_METHOD(EQUIP_SELLEQUIP)] = callfuncJson_selector(HttpHandler::sellEquips);
	exeFuns[GET_SERVICE_METHOD(EQUIP_REFINE)] = callfuncJson_selector(HttpHandler::refineEquip);
    exeFuns[GET_SERVICE_METHOD(TREASURE_BURN)] = callfuncJson_selector(HttpHandler::treasureBurn);
    
    
    exeFuns[GET_SERVICE_METHOD(PVP_INIT)] = callfuncJson_selector(HttpHandler::pvpInit);
    exeFuns[GET_SERVICE_METHOD(PVP_GETINFO)] = callfuncJson_selector(HttpHandler::pvpInfo);
    exeFuns[GET_SERVICE_METHOD(PVP_GETLIST)] = callfuncJson_selector(HttpHandler::pvpTargets);
    exeFuns[GET_SERVICE_METHOD(PVP_CHPVP)] = callfuncJson_selector(HttpHandler::chPvpTarget);  
	exeFuns[GET_SERVICE_METHOD(PVP_CHPVPLOW)] = callfuncJson_selector(HttpHandler::chPvpTarget);
    exeFuns[GET_SERVICE_METHOD(PVP_CHNPC)] = callfuncJson_selector(HttpHandler::chPvpNPC);
    exeFuns[GET_SERVICE_METHOD(PVP_CHBOSS)] = callfuncJson_selector(HttpHandler::chPvpBoss);
	exeFuns[GET_SERVICE_METHOD(PVP_GETREWARD)] = callfuncJson_selector(HttpHandler::getPvpAward);
	exeFuns[GET_SERVICE_METHOD(PVP_EXCHANGE)] = callfuncJson_selector(HttpHandler::challengeExchange);
	exeFuns[GET_SERVICE_METHOD(PVP_REFRESH)] = callfuncJson_selector(HttpHandler::challengeRefresh);
    
	exeFuns[GET_SERVICE_METHOD(CUP_INIT)] = callfuncJson_selector(HttpHandler::cupInit);
    exeFuns[GET_SERVICE_METHOD(DESTINY_IMPROVE)] = callfuncJson_selector(HttpHandler::improveDestiny);
    exeFuns[GET_SERVICE_METHOD(DESTINY_GETREWARD)] = callfuncJson_selector(HttpHandler::rewardDestiny);
    
    exeFuns[GET_SERVICE_METHOD(CUP_SIGNUP)] = callfuncJson_selector(HttpHandler::signUp);
    exeFuns[GET_SERVICE_METHOD(CUP_MREWARD)] = callfuncJson_selector(HttpHandler::cupMReward);
    exeFuns[GET_SERVICE_METHOD(CUP_MREP)] = callfuncJson_selector(HttpHandler::cupMRep);
    exeFuns[GET_SERVICE_METHOD(CUP_BET)] = callfuncJson_selector(HttpHandler::cupBet);
    
	exeFuns[GET_SERVICE_METHOD(TOWER_FIGHT)] = callfuncJson_selector(HttpHandler::FightResult);
    exeFuns[GET_SERVICE_METHOD(TOWER_RANK)] = callfuncJson_selector(HttpHandler::towerRank);
	exeFuns[GET_SERVICE_METHOD(TOWER_OLD_RANK)] = callfuncJson_selector(HttpHandler::towerOldRank);
	exeFuns[GET_SERVICE_METHOD(TOWER_REWARD)] = callfuncJson_selector(HttpHandler::tower_reward);
    
    
	exeFuns[GET_SERVICE_METHOD(MAIL_PICKUP)] = callfuncJson_selector(HttpHandler::getMailAwards);
	exeFuns[GET_SERVICE_METHOD(MAIL_GETMAILS)] = callfuncJson_selector(HttpHandler::mails);
	exeFuns[GET_SERVICE_METHOD(MAIL_DELETE_MAIL)] = callfuncJson_selector(HttpHandler::mailDeleteMail);
	exeFuns[GET_SERVICE_METHOD(MAIL_RECIVE_ALL_ITEM)] = callfuncJson_selector(HttpHandler::getAllMailAwards);
    
    
    exeFuns[GET_SERVICE_METHOD(CHAPTER_COLLECT_BOX)] = callfuncJson_selector(HttpHandler::chapterCollectBox);
    exeFuns[GET_SERVICE_METHOD(PVE_EXEC)]   =   callfuncJson_selector(HttpHandler::startExplore);
	exeFuns[GET_SERVICE_METHOD(PVE_RUSH)] = callfuncJson_selector(HttpHandler::rush);
    exeFuns[GET_SERVICE_METHOD(PVE_ONCEMORE)] = callfuncJson_selector(HttpHandler::onceMore);
	//exeFuns[GET_SERVICE_METHOD(PVE_ACTIVITYAWARD)] = callfuncJson_selector(HttpHandler::pveActivityAward);
    
	exeFuns[GET_SERVICE_METHOD(WISH_GET)] = callfuncJson_selector(HttpHandler::getWishes);
	exeFuns[GET_SERVICE_METHOD(WISH_WISH)] = callfuncJson_selector(HttpHandler::doWish);
	exeFuns[GET_SERVICE_METHOD(WISH_CHANGEWARRIOR)] = callfuncJson_selector(HttpHandler::wishChangeWarrior);

	//exeFuns[GET_SERVICE_METHOD(BURN_BURN)] = callfuncJson_selector(HttpHandler::burn);
	//exeFuns[GET_SERVICE_METHOD(BURN_AUTOLIST)] = callfuncJson_selector(HttpHandler::getAutoList);
    
    exeFuns[GET_SERVICE_METHOD(ROOKIE_GET)] = callfuncJson_selector(HttpHandler::getRookieInfo);
	exeFuns[GET_SERVICE_METHOD(ROOKIE_FINISH)] = callfuncJson_selector(HttpHandler::finishRookie);
    
    exeFuns[GET_SERVICE_METHOD(ACTIVITY_GET_SEPCIAL)] = callfuncJson_selector(HttpHandler::activityGetSpecial);
	exeFuns[GET_SERVICE_METHOD(ACTIVITY_GET_AWARD)] = callfuncJson_selector(HttpHandler::activityGetAward);
	exeFuns[GET_SERVICE_METHOD(ACTIVITY_TLOGIN_AWARD)] = callfuncJson_selector(HttpHandler::activityTLoginAward);
	exeFuns[GET_SERVICE_METHOD(ACTIVITY_LVUP_AWARD)] = callfuncJson_selector(HttpHandler::activityLvUpAward);
    
	exeFuns[GET_SERVICE_METHOD(ACTIVITY_CHARGE_AWARD)] = callfuncJson_selector(HttpHandler::activityChargeAward);
    
    exeFuns[GET_SERVICE_METHOD(SYSTEM_INIT)] = callfuncJson_selector(HttpHandler::initData);
    exeFuns[GET_SERVICE_METHOD(SYSTEM_NOTICE)] = callfuncJson_selector(HttpHandler::getNotices);
    
    exeFuns[GET_SERVICE_METHOD(CHARGE_GET_RESULT)] = callfuncJson_selector(HttpHandler::getChargeResult);

#pragma mark - 公会 -
	exeFuns[GET_SERVICE_METHOD(GUILD_CREATE)] = callfuncJson_selector(HttpHandler::guildCreate);
	exeFuns[GET_SERVICE_METHOD(GUILD_APPLY)] = callfuncJson_selector(HttpHandler::guildApply);
	exeFuns[GET_SERVICE_METHOD(GUILD_DEAL_APPLY)] = callfuncJson_selector(HttpHandler::guildDealApply);
	exeFuns[GET_SERVICE_METHOD(GUILD_GET)] = callfuncJson_selector(HttpHandler::guildGet);
	exeFuns[GET_SERVICE_METHOD(GUILD_GET_LIST)] = callfuncJson_selector(HttpHandler::guildGetList);
	exeFuns[GET_SERVICE_METHOD(GUILD_SEARCH)] = callfuncJson_selector(HttpHandler::guildSearch);
	exeFuns[GET_SERVICE_METHOD(GUILD_POSITION)] = callfuncJson_selector(HttpHandler::guildPosition);
	exeFuns[GET_SERVICE_METHOD(GUILD_QUIT)] = callfuncJson_selector(HttpHandler::guildQuit);
	exeFuns[GET_SERVICE_METHOD(GUILD_SIGN)] = callfuncJson_selector(HttpHandler::guildSign);
	exeFuns[GET_SERVICE_METHOD(GUILD_QUES)] = callfuncJson_selector(HttpHandler::guildQues);
	exeFuns[GET_SERVICE_METHOD(GUILD_DETAILS)] = callfuncJson_selector(HttpHandler::guildDetails);
	exeFuns[GET_SERVICE_METHOD(GUILD_UPDATE)] = callfuncJson_selector(HttpHandler::guildUpdate);
	exeFuns[GET_SERVICE_METHOD(GUILD_APPLYLIST)] = callfuncJson_selector(HttpHandler::guildApplyList);
	exeFuns[GET_SERVICE_METHOD(GUILD_UPGRADE_TREE)] = callfuncJson_selector(HttpHandler::guildUpgradeTree);
	exeFuns[GET_SERVICE_METHOD(GUILD_GET_NOTICE)] = callfuncJson_selector(HttpHandler::guildGetNotice);
	exeFuns[GET_SERVICE_METHOD(GUILD_MSG)] = callfuncJson_selector(HttpHandler::guildMsg);
	exeFuns[GET_SERVICE_METHOD(GUILD_MALL_REFRESH)] = callfuncJson_selector(HttpHandler::guildMallRefresh);
	exeFuns[GET_SERVICE_METHOD(GUILD_MALL_EXCHANGE)] = callfuncJson_selector(HttpHandler::guildMallExchange);
	exeFuns[GET_SERVICE_METHOD(GUILD_SUBSTITUTE)] = callfuncJson_selector(HttpHandler::guildSubstitute);
	exeFuns[GET_SERVICE_METHOD(GUILD_LOG)] = callfuncJson_selector(HttpHandler::guildLog);

#pragma mark - 公会战 -
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_GET)] = callfuncJson_selector(HttpHandler::guildBossGet);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_DECLARE_WAR)] = callfuncJson_selector(HttpHandler::guildBossDeclareWar);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_DECLARE_TARGETS)] = callfuncJson_selector(HttpHandler::guildBossDeclareTargets);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_START)] = callfuncJson_selector(HttpHandler::guildBossStart);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_GET_WOUNDED_LIST)] = callfuncJson_selector(HttpHandler::guildBossGetWoundedList);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_MEMBER_RANK)] = callfuncJson_selector(HttpHandler::guildBossMemberRank);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_GUILD_RANK)] = callfuncJson_selector(HttpHandler::guildBossGuildRank);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_GET_TARGET_LIST)] = callfuncJson_selector(HttpHandler::guildBossGetTargetList);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_GET_HATRED_LIST)] = callfuncJson_selector(HttpHandler::guildBossGetHatredList);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_ATTACK_BOSS)] = callfuncJson_selector(HttpHandler::guildBossAttackBoss);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_AUTO_ATTACK_BOSS)] = callfuncJson_selector(HttpHandler::guildBossAutoAttackBoss);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_ROB)] = callfuncJson_selector(HttpHandler::guildBossRob);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_ROB_SETTLE)] = callfuncJson_selector(HttpHandler::guildBossRobSettle);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_RESCUE)] = callfuncJson_selector(HttpHandler::guildBossRescue);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_GET_NOTICE)] = callfuncJson_selector(HttpHandler::guildBossGetNotice);
	exeFuns[GET_SERVICE_METHOD(GUILDBOSS_DAILY_SETTLE)] = callfuncJson_selector(HttpHandler::guildBossDailySettle);

#pragma mark - 皇陵 -
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_GETSEARCH)] = callfuncJson_selector(HttpHandler::dragonvalleyGetsearch);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_GETOCCUPY)] = callfuncJson_selector(HttpHandler::dragonvalleyGetoccupy);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_GETOCCUPYLIST)] = callfuncJson_selector(HttpHandler::dragonvalleyGetoccupylist);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_SEARCH)] = callfuncJson_selector(HttpHandler::dragonvalleySearch);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_SPEEDUP)] = callfuncJson_selector(HttpHandler::dragonvalleySpeedup);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_OCCUPYFINISH)] = callfuncJson_selector(HttpHandler::dragonvalleyOccupyfinish);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_GETASSISTBONUSLIST)] = callfuncJson_selector(HttpHandler::dragonvalleyGetassistbonuslist);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_GETASSISTBONUS)] = callfuncJson_selector(HttpHandler::dragonvalleyGetassistbonus);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_GETALLASSISTBONUS)] = callfuncJson_selector(HttpHandler::dragonvalleyGetallassistbonus);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_BATTLESTART)] = callfuncJson_selector(HttpHandler::dragonvalleyBattlestart);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_GETLOG)] = callfuncJson_selector(HttpHandler::dragonvalleyGetlog);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_USERRANK)] = callfuncJson_selector(HttpHandler::dragonvalleyUserRank);
	exeFuns[GET_SERVICE_METHOD(DRAGONVALLEY_GUILDRANK)] = callfuncJson_selector(HttpHandler::dragonvalleyGuildRank);
	
#pragma mark - 狩魂 -
	exeFuns[GET_SERVICE_METHOD(SOUL_HUNTER_GET)] = callfuncJson_selector(HttpHandler::huntGet);
	exeFuns[GET_SERVICE_METHOD(SOUL_HUNTER_TARGETS)] = callfuncJson_selector(HttpHandler::huntTargets);
	exeFuns[GET_SERVICE_METHOD(SOUL_HUNTER_ATTACK)] = callfuncJson_selector(HttpHandler::huntAttack);
    
#pragma mark 商城
    exeFuns[GET_SERVICE_METHOD(PRODUCT_BUY)] = callfuncJson_selector(HttpHandler::productBuy);
    exeFuns[GET_SERVICE_METHOD(BUY_200_RED_PACKET)] = callfuncJson_selector(HttpHandler::redPacketBuy);

#pragma mark 装备
	exeFuns[GET_SERVICE_METHOD(EQUIP_DECOMPOSE)] = callfuncJson_selector(HttpHandler::decomposeEquip);

#pragma mark - 好友 - 组队副本 -
	exeFuns[GET_SERVICE_METHOD(FRIEND_CHALLENGE)] = callfuncJson_selector(HttpHandler::friendChallenge);
	exeFuns[GET_SERVICE_METHOD(FRIEND_RECOMMEND)] = callfuncJson_selector(HttpHandler::friendRecommend);
	exeFuns[GET_SERVICE_METHOD(FRIEND_SEARCH)] = callfuncJson_selector(HttpHandler::friendSearch);
	exeFuns[GET_SERVICE_METHOD(FRIEND_SEND_REQUEST)] = callfuncJson_selector(HttpHandler::friendSendRequest);
	exeFuns[GET_SERVICE_METHOD(FRIEND_ACCEPT)] = callfuncJson_selector(HttpHandler::friendAccept);
	exeFuns[GET_SERVICE_METHOD(FRIEND_REFUSE)] = callfuncJson_selector(HttpHandler::friendRefuse);
	exeFuns[GET_SERVICE_METHOD(FRIEND_REFUSE_ALL)] = callfuncJson_selector(HttpHandler::friendRefuseAll);
	exeFuns[GET_SERVICE_METHOD(FRIEND_DELETE_FRIEND)] = callfuncJson_selector(HttpHandler::friendDeleteFriend);
	//exeFuns[GET_SERVICE_METHOD(FRIEND_GET_REQUESTS)] = callfuncJson_selector(HttpHandler::friendGetRequests);
	exeFuns[GET_SERVICE_METHOD(FRIEND_SEND_MAIL)] = callfuncJson_selector(HttpHandler::friendSendMail);
	exeFuns[GET_SERVICE_METHOD(FRIEND_DELETE_MAIL)] = callfuncJson_selector(HttpHandler::friendDeleteMail);
	exeFuns[GET_SERVICE_METHOD(FRIEND_SEND_GIFT)] = callfuncJson_selector(HttpHandler::friendSendGit);
	exeFuns[GET_SERVICE_METHOD(FRIEND_RECEIVE_GIFT)] = callfuncJson_selector(HttpHandler::friendReceiveGift);
	exeFuns[GET_SERVICE_METHOD(FRIEND_GET_NEW_REQUEST)] = callfuncJson_selector(HttpHandler::friendGetNewRequest);
	exeFuns[GET_SERVICE_METHOD(FRIEND_GETFRIENDS)] = callfuncJson_selector(HttpHandler::friendGetFriends);
	exeFuns[GET_SERVICE_METHOD(FRIEND_DUNGEON)] = callfuncJson_selector(HttpHandler::friendDungeon);
	exeFuns[GET_SERVICE_METHOD(FRIEND_RESET_DUNGEON)] = callfuncJson_selector(HttpHandler::friendResetDungeon);
	exeFuns[GET_SERVICE_METHOD(FRIEND_GO)] = callfuncJson_selector(HttpHandler::friendGo);
	exeFuns[GET_SERVICE_METHOD(FRIEND_RUSH)] = callfuncJson_selector(HttpHandler::friendRush);
	exeFuns[GET_SERVICE_METHOD(FRIEND_RECEIVE_AND_SEND)] = callfuncJson_selector(HttpHandler::receiveAndSendGiftAll);
	

#pragma mark - 聊天 -
	exeFuns[GET_SERVICE_METHOD(CHAT_SEND_GUILD)] = callfuncJson_selector(HttpHandler::chatSendGuild);
	exeFuns[GET_SERVICE_METHOD(CHAT_GET_GUILD)] = callfuncJson_selector(HttpHandler::chatGetGuild);
	exeFuns[GET_SERVICE_METHOD(CHAT_SEND_WORLD)] = callfuncJson_selector(HttpHandler::chatSendWorld);
	exeFuns[GET_SERVICE_METHOD(CHAT_GET_WORLD)] = callfuncJson_selector(HttpHandler::chatGetWorld);
    
#pragma mark - 分享领奖 －
    
    exeFuns[GET_SERVICE_METHOD(ACTIVITY_SHARE)] = callfuncJson_selector(HttpHandler::activityShareAward);

#pragma mark - 奇遇 -
	exeFuns[GET_SERVICE_METHOD(WONDER_TRADER)] = callfuncJson_selector(HttpHandler::wonderTrader);
	exeFuns[GET_SERVICE_METHOD(WONDER_TREASURE)] = callfuncJson_selector(HttpHandler::wonderTreasure);
	exeFuns[GET_SERVICE_METHOD(WONDER_SUPERIOR)] = callfuncJson_selector(HttpHandler::wonderSuperior);
	exeFuns[GET_SERVICE_METHOD(WONDER_FIGHT)] = callfuncJson_selector(HttpHandler::wonderFight);
    exeFuns[GET_SERVICE_METHOD(WONDER_FIGHT_BOSS)] = callfuncJson_selector(HttpHandler::wonderFightBoss);
	exeFuns[GET_SERVICE_METHOD(WONDER_WARRIOR)] = callfuncJson_selector(HttpHandler::wonderWarrior);
	exeFuns[GET_SERVICE_METHOD(WONDER_BOX)] = callfuncJson_selector(HttpHandler::wonderBox);
#pragma mark - 签到 -
	//获取签到信息 改放在parse中 增加对初始化签到信息的解析
	//exeFuns[GET_SERVICE_METHOD(SIGNIN_GET)] = callfuncJson_selector(HttpHandler::signinGet);
	exeFuns[GET_SERVICE_METHOD(SIGNIN_SIGNIN)] = callfuncJson_selector(HttpHandler::signinSignin);
#pragma mark -小伙伴-
	//exeFuns[GET_SERVICE_METHOD(FORMATION_JOINMASTER)] = callfuncJson_selector(HttpHandler::joinGundam);
#pragma mark -活跃度-
	exeFuns[GET_SERVICE_METHOD(ACTIVENESS_GETAWARD)] = callfuncJson_selector(HttpHandler::rewardGet);
    return true;
}

void HttpHandler::user(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_USER_MODEL->parseUserInfo(responseData);
	DM_GET_NAVIGATION_MODEL->updateNavigationUnlockStatus();
    DM_GET_FORMATION_MODEL->updateLockStatus(DM_GET_USER_MODEL->getUserInfo()->getUserLv());
}


void HttpHandler::useItem(const Json::Value &requestData,const Json::Value &responseData)
{
	uint32_t itemId = requestData["id"].asUInt();
	item::ItemInfo * pItemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(itemId);

    uint32_t useNum = 1;
    if(responseData.isMember("useNum"))
	{
		useNum = responseData["useNum"].asUInt();
	}
	//else{
	//	useNum = pItemInfo->getBaseInfo()->getUseItemNum();
	//}

	switch (pItemInfo->getBaseInfo()->getItemType())
	{
	case item::kItemTypeVipPk:
	case item::kItemTypeBox:
	case item::kItemTypeReward:
		
		break;
	case item::kItemTypePrivilege:
		DM_GET_USER_MODEL->resetPrivileges();
		HTTP_CLIENT->userPrivileges();
		break;
	case item::kItemTypeSpecialPrivilege:
		break;
	case item::kItemTypePow:
		DM_GET_ITEM_MODEL->addPow( pItemInfo );
		break;
	case item::kItemTypeEndu:
		DM_GET_ITEM_MODEL->addEndu( pItemInfo );
		break;
	case item::kItemTypeNormal:
	case item::kItemTypeUsage:
	default:;
	};
	
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));

    AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
    pAnalytics->useItem(pItemInfo->getBaseInfo()->getItemName().c_str(), useNum);
    
	DM_GET_ITEM_MODEL->reduceItemNumByID(itemId,useNum);
	
	//更新碎片信息
	DM_GET_ITEM_MODEL->postNotification(D_EVENT_AVAILABLE_EQUIP);
}

void HttpHandler::drops(const Json::Value &requestData,const Json::Value &responseData)
{
	//十连抽掉落特殊处理
	bool noSort = false;
	if(requestData.isMember("noSort") && requestData["noSort"].asBool() == true) noSort = true;

	DM_GET_DROP_MODEL->addDrops(responseData,requestData[UUID].asUInt() , noSort);

	uint32_t nSize = responseData.size();
	for (uint32_t i=0; i<nSize; ++i)
	{
		saveDrops(requestData, responseData[i]);
	}
}

void HttpHandler::drop(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_DROP_MODEL->addDrop(responseData,requestData[UUID].asUInt());
    saveDrops(requestData,responseData);
}

void HttpHandler::saveDrops(const Json::Value &requestData,const Json::Value &responseData)
{
    AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
    drop::DropType dropType = (drop::DropType)responseData["type"].asUInt();
    const char *itemName = NULL;
    int num = 1;
    
    switch (dropType) {
        case drop::kDropTypeAvatar:
        {
            avatar::AvatarInfo *pAvatarInfo = DM_GET_AVATAR_MODEL->parseDress(responseData["dress"]);
            DM_GET_AVATAR_MODEL->addAvatar(pAvatarInfo);
            itemName = pAvatarInfo->getBaseInfo()->getName().c_str();
            break;
        }
        case drop::kDropTypeItem:
        case drop::kDropTypePkg:
        {
            item::ItemInfo* pItem = DM_GET_ITEM_MODEL->parseItem(responseData["item"]);
			DM_GET_ITEM_MODEL->addItemNumByID(pItem->getItemId(),pItem->getNum());
            itemName = pItem->getBaseInfo()->getItemName().c_str();
            num = pItem->getNum();
            break;
        }
        case drop::kDropTypeEquip:
        {
            equip::EquipCurInfo *pEquip = DM_GET_EQUIP_MODEL->parseEquip(responseData["equip"]);
            DM_GET_EQUIP_MODEL->addEquip(pEquip);
            itemName = pEquip->getBaseInfo()->getEquipName().c_str();
            break;
        }
        case drop::kDropTypeSoul:
        {
             warrior::SoulInfo* pSoulInfo = DM_GET_WARRIOR_MODEL->parseSoul(responseData["soul"]);
            DM_GET_WARRIOR_MODEL->addSoul(pSoulInfo);
            itemName = CCString::createWithFormat("soul_%s", pSoulInfo->getBaseInfo()->getWarriorName().c_str())->getCString();//pSoulInfo->getBaseInfo()->getWarriorName().c_str();
            num = pSoulInfo->getNum();
            break;
        }
        case drop::kDropTypeWarrior:
        {
             warrior::WarriorCurInfo *pWarriorCurInfo = DM_GET_WARRIOR_MODEL->parseWarrior(responseData["warrior"]);
             DM_GET_WARRIOR_MODEL->addWarrior(pWarriorCurInfo);
             pWarriorCurInfo->computeWarriorProps();
            itemName = pWarriorCurInfo->getBaseInfo()->getWarriorName().c_str();
            break;
            
        }
        case drop::kDropTypeCoin:
            if(responseData.isMember("coin")) DM_GET_USER_MODEL->getUserInfo()->changeCoins(responseData["coin"].asInt());
            itemName = "coin";
            num = responseData["coin"].asInt();
            
            break;
            
        case drop::kDropTypePoint:
            if(responseData.isMember("point")) DM_GET_USER_MODEL->getUserInfo()->changePoints(responseData["point"].asInt());
			itemName = "point";
            num = responseData["point"].asInt();
            break;
        case drop::kDropTypeVip:
        {
            if(responseData.isMember("vip"))
            {
                uint32_t vip = responseData["vip"].asInt();
                user::UserInfo * pUserInfo = DM_GET_USER_MODEL->getUserInfo();
                if(vip > pUserInfo->getVipLevel())
                {
                    pUserInfo->setVipLevel(vip);
                }
                itemName = "vip";
                num = vip;
            }
            break;
        }
        case drop::kDropTypePower:
            if(responseData.isMember("power")) DM_GET_USER_MODEL->getUserInfo()->changePower(responseData["power"].asInt());
            num = responseData["power"].asInt();
			itemName = "power";
            break;
        default:
            break;
    }
    pAnalytics->dropItem(itemName, num);
}

void HttpHandler::getPoint(const Json::Value &requestData,const Json::Value &responseData)
{
    int point = DM_GET_USER_MODEL->getUserInfo()->getPoints();
	DM_GET_USER_MODEL->getUserInfo()->updatePoints(responseData.asUInt());
    point -= responseData.asUInt();
    AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
    if (point == 0) {
        return;
    }
    else if (point > 0)
    {
        pAnalytics->useItem("point", point);
    }
    else
    {
        pAnalytics->dropItem("point", -point);
    }
}

void HttpHandler::coinChanged(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_USER_MODEL->getUserInfo()->changeCoins(responseData.asUInt());
    AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
    pAnalytics->dropItem("coin", responseData.asUInt());
}


void HttpHandler::formations(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_FORMATION_MODEL->parseFomations(responseData);
}

void HttpHandler::otherFormation(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_OTHERFORMATION_MODEL->parseOtherFomations(responseData,last_response_time);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("OtherFormation", "show");
}

void HttpHandler::destinies(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_DESTINY_MODEL->updateDestinys(responseData);
}

void HttpHandler::equip(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_EQUIP_MODEL->parseOneEquip(responseData);

}

void HttpHandler::equips(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_EQUIP_MODEL->parseEquips(responseData);
}

void HttpHandler::tasks(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ACHIEVEMENT_MODEL->parseTasks(responseData);
}

void HttpHandler::achievement_msg(const Json::Value &requestData,const Json::Value &responseData)
{
	
	//在model中更新
	DM_GET_ACHIEVEMENT_MODEL->updateCurrentAchievement(responseData);
}

void HttpHandler::equipRefineryItemNum(const Json::Value &requestData,const Json::Value &responseData)
{
	//在model中更新
	DM_GET_BURN_MODEL->setEquipRefineryItemNum(responseData.asUInt());
}

void HttpHandler::enhanceEquip(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_USER_MODEL->getUserInfo()->updateCoins(responseData["user"]["coin"].asUInt(), 0.5f);
    DM_GET_EQUIP_MODEL->parseOneEquip(responseData["equip"],true,0.5f);
    AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
	pAnalytics->useItem("coin", responseData["user"]["coin"].asUInt());
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Equip", "enhance");
}

void HttpHandler::sellEquips(const Json::Value &requestData,const Json::Value &responseData)
{
	//DM_GET_EQUIP_MODEL->removeEquip(requestData["id"].asUInt());
	//AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Equip", "sell");

	// 本地删除装备
	const Json::Value& equis = requestData["ids"];
	for (Json::Value::iterator iter = equis.begin(); iter != equis.end(); ++iter)
	{
		DM_GET_EQUIP_MODEL->removeEquip((*iter).asInt());
	}
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Equip", "sell");

	//炼化重置
	DM_GET_BURN_MODEL->reset();
}

void HttpHandler::refineEquip(const Json::Value &requestData,const Json::Value &responseData)
{
	//精炼
}


void HttpHandler::treasureBurn(const Json::Value &requestData,const Json::Value &responseData)
{
	//删装备
	DM_GET_BURN_MODEL->parseTreasureBurn(requestData,responseData);
}

void HttpHandler::item(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_ITEM_MODEL->parseOneItem(responseData);

}

void HttpHandler::items(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ITEM_MODEL->parseItems(responseData);
}

void HttpHandler::item_change(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ITEM_MODEL->changeItemNumByID(responseData["itemId"].asUInt(), responseData["num"].asInt());

	item::ItemInfo *pItem = DM_GET_ITEM_MODEL->getItemInfoByID(responseData["itemId"].asUInt());
	if (!pItem)
	{
		return;
	}
	
	const char *itemName = pItem->getBaseInfo()->getItemName().c_str();
	int num = responseData["num"].asInt();
	AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
	if(num > 0)
	{
		pAnalytics->dropItem(itemName, num);
	}
	else if (num < 0)
	{
		pAnalytics->useItem(itemName, num);
	}
}

void HttpHandler::items_change(const Json::Value &requestData,const Json::Value &responseData)
{
    for (uint32_t i =0; i<responseData.size(); ++i)
    {
        item_change(requestData,responseData[i]);
    }
}

void HttpHandler::products(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MALL_MODEL->parseProducts(responseData);
}


void HttpHandler::warrior(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_WARRIOR_MODEL->parseOneWarrior(responseData);
}

void HttpHandler::warriors(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_WARRIOR_MODEL->parseWarriors(responseData);
}

void HttpHandler::privileges(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_USER_MODEL->parsePrivileges(responseData);
}

void HttpHandler::userPrivileges(const Json::Value &requestData,const Json::Value &responseData)
{
	//DM_GET_USER_MODEL->parsePrivileges(responseData["privileges"]);
}

void HttpHandler::getSouls(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_WARRIOR_MODEL->updateSouls(responseData["souls"]);
}

void HttpHandler::inheritWarrior(const Json::Value &requestData,const Json::Value &responseData)
{
	uint32_t wid = requestData["fid"].asUInt();
	warrior::WarriorCurInfo *pWarriorInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wid);
    
	if (pWarriorInfo)
	{
		pWarriorInfo->resetWarriorCurInfo();
        DM_GET_WARRIOR_MODEL->checkWarriorOrder(pWarriorInfo);
	}
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Warrior", "inherit");
}

void HttpHandler::confirmDevWarrior(const Json::Value &requestData,const Json::Value &responseData)
{
	uint32_t wid = requestData["wid"].asUInt();
	uint32_t act = requestData["act"].asUInt();

	warrior::WarriorCurInfo *pWarriorInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wid);

	if (pWarriorInfo)
	{
		if (act == WARRIOR_DEV_CONFIRM)	//确认
			DM_GET_WARRIOR_MODEL->dealWithPotBuff(pWarriorInfo);
		else	//取消
			DM_GET_WARRIOR_MODEL->dealWithPotBuff(pWarriorInfo,false);
	}
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Warrior", "develop");
}

void HttpHandler::promoteWarrior(const Json::Value &requestData,const Json::Value &responseData)
{
	uint32_t wid = requestData["wid"].asUInt();

	warrior::WarriorCurInfo *pWarriorInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wid);
	
	//pSoulInfo->autorelease();

	if (pWarriorInfo)
	{
        warrior::SoulInfo *pSoulInfo = warrior::SoulInfo::create(wid,DM_GET_WARRIOR_MODEL->getWarriorUpSoul(pWarriorInfo));
		DM_GET_WARRIOR_MODEL->removeSoul(pSoulInfo);
		pWarriorInfo->addUpLevel();
	}
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Warrior", "promote");
}

void HttpHandler::recruitSoul(const Json::Value &requestData,const Json::Value &responseData)
{
	unsigned int wid = requestData["wid"].asUInt();
    WarriorModel * pWarriorModel = DM_GET_WARRIOR_MODEL;
    
    
	//服务器未返回弟子数据需客户端添加
	warrior::WarriorCurInfo *pWarriorCurInfo = warrior::WarriorCurInfo::create(wid);
	pWarriorModel->addWarrior(pWarriorCurInfo);
    
    
	warrior::SoulInfo *pSoulInfo = warrior::SoulInfo::create(wid,pWarriorModel->getWarriorGrowInfoByGrade(pWarriorCurInfo->getBaseInfo()->getRealGrade())->getRctSoul());
	pWarriorModel->removeSoul(pSoulInfo);
	pWarriorModel->postNotification(D_EVENT_WARRIOR_FINISH_RECRUIT, pWarriorCurInfo);

	AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
	pAnalytics->useItem(CCString::createWithFormat("soul_%s", pSoulInfo->getBaseInfo()->getWarriorName().c_str())->getCString(), pSoulInfo->getNum());

	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Soul", "recruit");
}

void HttpHandler::potBuffs(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_WARRIOR_MODEL->parsePotBuffs(responseData);
}

void HttpHandler::potBuff(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_WARRIOR_MODEL->parseOnePotBuff(responseData);
}

void HttpHandler::soul(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_WARRIOR_MODEL->parseOneSoul(responseData);
}

void HttpHandler::souls(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_WARRIOR_MODEL->parseSouls(responseData);
}


void HttpHandler::explores(const Json::Value &requestData,const Json::Value &responseData)
{
    CCAssert(requestData.isMember("cid"), "");
	DM_GET_PVE_MODEL->updateMaps(requestData["cid"].asUInt(),responseData);
}


void HttpHandler::explore(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_PVE_MODEL->updateMap(responseData);    
}

void HttpHandler::chapterCollectBox(const Json::Value &requestData,const Json::Value &responseData)
{
	pve::ChapterInfo* pChapterInfo = DM_GET_PVE_MODEL->getChapterInfo(requestData["cid"].asUInt());
	if (pChapterInfo)
	{
        pChapterInfo->setChpterAwardGotten(requestData["vip"].asUInt());
	}
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asInt()));
}

void HttpHandler::pveActivityAward(const Json::Value &requestData,const Json::Value &responseData)
{
	//NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asInt()));
}

void HttpHandler::chapters(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_PVE_MODEL->updateChapters(responseData);
}


void HttpHandler::chapter(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_PVE_MODEL->updateChapter(responseData);
    
}

/*杯赛*/

void HttpHandler::cupMReward(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_MATCH_MODEL->mReward(requestData["finalNum"].asUInt(), requestData["mid"].asUInt());
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asInt()));
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Cup", "reward");
}

void HttpHandler::cupMRep(const Json::Value &requestData,const Json::Value &responseData)
{
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE,cocos2d::CCInteger::create(battle::kSkipBattleTypeMatch));
	DM_GET_MATCH_MODEL->watchReplay(requestData["finalNum"].asUInt(), requestData["mid"].asUInt());
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Cup", "replay");
    DM_GET_BATTLE_MODEL->setSettlementInfo(DM_GET_SETTLEMENT_MODEL->getSettlementInfoMatch());
    DM_GET_BATTLE_MODEL->setIsShowResult(false);
}

void HttpHandler::cupBet(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MATCH_MODEL->updateMatchRate(requestData["finalNum"].asUInt(), requestData["mid"].asUInt(),responseData["rate1"].asUInt(),responseData["rate2"].asUInt());
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Cup", "bet");
}

void HttpHandler::signUp(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MATCH_MODEL->signedUp();
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Cup", "signUp");
}

void HttpHandler::cup(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MATCH_MODEL->parseSignUp(responseData);
}

void HttpHandler::process(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MATCH_MODEL->parseMatchs(responseData,last_response_time);
}

void HttpHandler::mTable(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MATCH_MODEL->parseVSMatchs(responseData);
}

void HttpHandler::bet(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MATCH_MODEL->parseBet(responseData);
}

void HttpHandler::bets(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MATCH_MODEL->parseBets(responseData);
}

void HttpHandler::cupInit(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MATCH_MODEL->inited();
}

#pragma mark --------------论剑-----------------

/*论剑*/
void HttpHandler::pvpInit(const Json::Value &requestData,const Json::Value &responseData)
{
    const Json::Value & jsonInfo = responseData["challenge"];
	//const Json::Value & jsonRanks = responseData["chRanks"];
	DM_GET_PVP_MODEL->onInitComplete(responseData);
}

//获取玩家论剑信息
void HttpHandler::pvpInfo(const Json::Value &requestData,const Json::Value &responseData)
{
    const Json::Value & jsonInfo = responseData["challenge"];
    DM_GET_PVP_MODEL->onInfoComplete(jsonInfo);
}

//获取论剑目标列表
void HttpHandler::pvpTargets(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_PVP_MODEL->onTargetsComplete(responseData);
}

//获取论剑奖励
void HttpHandler::getPvpAward(const Json::Value &requestData,const Json::Value &responseData)
{
	uint32_t awardID = requestData["reward"].asUInt();

	pvp::AwardInfo * pAwardInfo = DM_GET_PVP_MODEL->getAward(awardID);
	DM_GET_PVP_MODEL->drawAward(requestData[UUID].asUInt(), pAwardInfo);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("PVP", "award");
}

//論劍結算
void HttpHandler::pvpSettlement(const Json::Value &requestData,const Json::Value &responseData)
{
	unsigned int tid = 0;
	if(requestData.isMember("tid")) tid = requestData["tid"].asUInt();

	DM_GET_SETTLEMENT_MODEL->setSettlementInfoPvp(responseData , tid);
	DM_GET_BATTLE_MODEL->setSettlementInfo(DM_GET_SETTLEMENT_MODEL->getSettlementInfoPvp());

}

//挑战东皇太一
void HttpHandler::chPvpBoss(const Json::Value &requestData,const Json::Value &responseData)
{
    uint16_t grade = responseData["grade"].asUInt();
	DM_GET_PVP_MODEL->setGrade(grade);
	DM_GET_PVP_MODEL->onChallengeBossComplete(requestData[UUID].asUInt());
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("PVP", "boss");
} 

//挑战试炼木人
void HttpHandler::chPvpNPC(const Json::Value &requestData,const Json::Value &responseData)
{
    uint16_t grade = responseData["grade"].asUInt();
	DM_GET_PVP_MODEL->setGrade(grade);
	DM_GET_PVP_MODEL->onChallengeNpcComplete(requestData[UUID].asUInt());
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("PVP", "npc");
} 

//挑战排名
void HttpHandler::chPvpTarget(const Json::Value &requestData,const Json::Value &responseData)
{
    uint16_t grade = responseData["grade"].asUInt();
   
	DM_GET_PVP_MODEL->setGrade(grade);
    if(grade > 0)
    {
        const Json::Value & jsonInfo = responseData["challenge"];
        //const Json::Value & jsonTargets = responseData["chTargets"];
        
        DM_GET_PVP_MODEL->onChallengeTargetComplete(requestData[UUID].asUInt(), jsonInfo, responseData);
    }
    else
    {
        Json::Value jsonInfo, jsonTargets;
        DM_GET_PVP_MODEL->onChallengeTargetComplete(requestData[UUID].asUInt(), jsonInfo, responseData);
	}
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("PVP", "target");
    
}

void HttpHandler::challengeExchange(const Json::Value &requestData,const Json::Value &responseData)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW, cocos2d::CCInteger::create(requestData[UUID].asUInt()));
	DM_GET_PVP_MODEL->exchanged(requestData, responseData);
}

void HttpHandler::challengeRefresh(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_PVP_MODEL->parseChMall(responseData["chMall"]);
}
//void HttpHandler::cookie(const Json::Value &requestData,const Json::Value &responseData)
//{
//	DM_GET_USER_MODEL->saveCookie(responseData);
//}
















void HttpHandler::joinFormation(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_FORMATION_MODEL->joinFormation(requestData);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Formation", "join");
}

void HttpHandler::changeFormation(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_FORMATION_MODEL->changeFormation(requestData);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Formation", "change");
}


void HttpHandler::improveDestiny(const Json::Value &requestData,const Json::Value &responseData)
{
    AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();

    //更新相应的天命信息
    Json::Value destiny = responseData["destiny"];
    DM_GET_DESTINY_MODEL->updateDestiny(destiny);
    
    //监听天命信息变化刷新弟子列表 后台返回数据先更新天命 后更新弟子  避免重复刷新错误
    
    //先删除提交给后台的弟子和魂魄
    //更新弟子
    Json::Value warriors = Json::arrayValue;
    warriors = requestData["warriors"];
    cocos2d::CCArray *pRemoveWarriors = cocos2d::CCArray::create();
    for(uint32_t i = 0 ; i < warriors.size() ; i++)
    {
        uint32_t nWarriorID = warriors[i].asUInt();
        warrior::WarriorCurInfo *pWarriorCurInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(nWarriorID);
        pRemoveWarriors->addObject(pWarriorCurInfo);
		pAnalytics->useItem(pWarriorCurInfo->getBaseInfo()->getWarriorName().c_str(), 1);
    }
    
    DM_GET_WARRIOR_MODEL->removeWarriors(pRemoveWarriors);
    
    //更新魂魄
    Json::Value souls = Json::arrayValue;
    souls = requestData["souls"];
    cocos2d::CCArray *pRemoveSouls = cocos2d::CCArray::create();
    for(uint32_t i = 0 ; i < souls.size() ; i++)
    {
        warrior::SoulInfo *pSoulInfo =DM_GET_WARRIOR_MODEL->parseSoul(souls[i]);
		pRemoveSouls->addObject(pSoulInfo);
		pAnalytics->useItem(CCString::createWithFormat("soul_%s", pSoulInfo->getBaseInfo()->getWarriorName().c_str())->getCString(), pSoulInfo->getNum());
    }
    DM_GET_WARRIOR_MODEL->removeSouls(pRemoveSouls);
    
    //更新后台返回的受影响的最新魂魄数量
    DM_GET_WARRIOR_MODEL->parseSouls(responseData["souls"]);
    
    
   
    
	activeness(requestData, responseData["activeness"]);


	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Destiny", "improve");
    
}

void HttpHandler::rewardDestiny(const Json::Value &requestData,const Json::Value &responseData)
{
    //更新天命奖励状态
    DM_GET_DESTINY_MODEL->rewardDestiny(requestData["id"].asUInt(), (uint32_t)last_response_time);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));

	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Destiny", "reward");
}

void HttpHandler::loginAward(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_LOGIN_REWARD_MODEL->ParseActivityInfo(responseData);
}


void HttpHandler::mails(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MAIL_MODEL->parseMails(responseData);
}

void HttpHandler::getMailAwards(const Json::Value &requestData,const Json::Value &responseData)
{
	uint32_t mailId = requestData["id"].asUInt();
    DM_GET_MAIL_MODEL->setMailAttAccepted(DM_GET_MAIL_MODEL->getMailByID(mailId));
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));

	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Mail", "award");
}

void HttpHandler::getAllMailAwards(const Json::Value &requestData,const Json::Value &responseData)
{
	//uint32_t mailId = requestData["id"].asUInt();
	//DM_GET_MAIL_MODEL->setMailAttAccepted(DM_GET_MAIL_MODEL->getMailByID(mailId));
	DM_GET_MAIL_MODEL->setAllMailAttAccepted(responseData);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));

	//AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Mail", "award");
}

void HttpHandler::getNewMails(const Json::Value &requestData,const Json::Value &responseData)
{
	uint32_t newNormalMailNum = responseData["normal"].asUInt();
	uint32_t newFriendMailNum = responseData["friend"].asUInt();
	uint32_t newNormalAttMailNum = responseData["normal_att"].asUInt();
	DM_GET_MAIL_MODEL->setNewMailsNum(newNormalMailNum, newFriendMailNum,newNormalAttMailNum);
}

void HttpHandler::mailDeleteMail(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MAIL_MODEL->mailDeleted(requestData);
}

void HttpHandler::activeness(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_ACTIVENESS_MODEL->parseActiveness(responseData);
}

void HttpHandler::getActivenessAward(const Json::Value &requestData,const Json::Value &responseData) 
{	
	AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
	
	//添加金币
	if (responseData.isMember("coin"))
	{
		 DM_GET_USER_MODEL->getUserInfo()->changeCoins(responseData["coin"].asUInt());
		 pAnalytics->dropItem("coin", responseData["coin"].asUInt());
	}

	//获得点券
	if (responseData.isMember("point"))
	{
		DM_GET_USER_MODEL->getUserInfo()->changePoints(responseData["point"].asUInt());
		pAnalytics->dropItem("point", responseData["point"].asUInt());
	}

	//获得道具
	if (responseData.isMember("items"))
	{
        ItemModel * pItemModel = DM_GET_ITEM_MODEL;
        const Json::Value & items = responseData["items"];
        for(Json::Value::iterator iter(items.begin()); iter != items.end(); ++iter)
        {
            item::ItemInfo * pItemInfo = pItemModel->parseItem(*iter);
			pItemModel->changeItemNumByID(pItemInfo->getItemId(), pItemInfo->getNum());
			pAnalytics->dropItem(pItemInfo->getBaseInfo()->getItemName().c_str(), pItemInfo->getNum());
        }
	}

	//更新已领取奖励列表
	uint32_t point = requestData["point"].asUInt();    
    DM_GET_ACTIVENESS_MODEL->onGetAwardComplete(point);


	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Activeness", "award");
}



void HttpHandler::onBattleData(std::vector<char> &vec)
{
	DM_GET_BATTLE_MODEL->parseInfo(vec);
}

void HttpHandler::onceMore(const Json::Value &requestData,const Json::Value &responseData)
{
    pve::MapInfo *pMapInfo = DM_GET_PVE_MODEL->getMapInfo(requestData["mid"].asUInt());
	if (pMapInfo)
	{
        pMapInfo->setTodayTimes(responseData["todayTimes"].asUInt());
	}
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("PVE", "onceMore");
}

void HttpHandler::rush(const Json::Value &requestData,const Json::Value &responseData)
{
	//保存结算信息
    DM_GET_SETTLEMENT_MODEL->setSettlementInfoPve(responseData["rush"],requestData[UUID].asInt(),responseData["wonders"], requestData["mid"].asUInt());
	settlement::SettlementInfoPve *pSettlementInfo = DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve();    
	DM_GET_USER_MODEL->getUserInfo()->addExp(pSettlementInfo->exp,0.1f);//延迟升级提示 先出结算
	DM_GET_FORMATION_MODEL->updateLockStatus(DM_GET_USER_MODEL->getUserInfo()->getUserLv());
	DM_GET_USER_MODEL->getUserInfo()->changeCoins(pSettlementInfo->coin,0.1f);
	DM_GET_USER_MODEL->getUserInfo()->setPower(responseData["rush"]["power"].asInt());
    DM_GET_PVE_MODEL->getMapInfo(requestData["mid"].asUInt())->updateRush(responseData["rush"]);
    
    //结算
    if (pSettlementInfo->wexp > 0)
    {
        FormationModel::PositionArray* pPosition = DM_GET_FORMATION_MODEL->getPositionsArray();
        if (pPosition)
        {
            CCObject* obj = NULL;
            formation::PositionInfo* pPositionInfo = NULL;
            
            CCARRAY_FOREACH(pPosition, obj)
            {
                pPositionInfo = (formation::PositionInfo*)obj;
                if (pPositionInfo)
                {
                    warrior::WarriorCurInfo *pCurInfo =  DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(pPositionInfo->iWarriorId);
                    bool isLevelUp = false;
                    if (pCurInfo)
                    {
                        pCurInfo->addExp(pSettlementInfo->wexp, 0.f,&isLevelUp);
                    }
                    pSettlementInfo->setWarriorisLevelup(pPositionInfo->iWarriorId, isLevelUp);
                }
            }
        }
	}
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("PVE", "rush");
}

void HttpHandler::startExplore(const Json::Value &requestData,const Json::Value &responseData)
{
    //保存结算信息
    DM_GET_SETTLEMENT_MODEL->setSettlementInfoPve(responseData["pve"],requestData[UUID].asInt(),responseData["wonder"], requestData["mid"].asUInt());
    
    pve::MapInfo* pInfo = DM_GET_PVE_MODEL->getMapInfo(requestData["mid"].asUInt());
    DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve()->setCanSkipBattle(pInfo->iScores>0);
    
    if(responseData.isMember("activeness")) activeness(requestData, responseData["activeness"]);
	if(pInfo && pInfo->fightType==0)  
	{
		//精英副本不执行
		explore(requestData, responseData["pve"]);
	}

    settlement::SettlementInfoPve *pSettlementInfo = DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve();

	/*pve::MapInfo *pMapInfo = DM_GET_PVE_MODEL->getMapInfo(DM_GET_PVE_MODEL->getLastShowMapID());
	CCAssert(pMapInfo != NULL, CCString::createWithFormat("error map id: %u", DM_GET_PVE_MODEL->getLastShowMapID())->getCString());
	if (pMapInfo)
	{
		DM_GET_USER_MODEL->getUserInfo()->changePower(pMapInfo->power_cost,false);
	}*/
	CCAssert(pInfo != NULL, CCString::createWithFormat("error map id: %u", pInfo->getMapId())->getCString());
	if(pInfo)
	{
		//结算体力
		DM_GET_USER_MODEL->getUserInfo()->changePower(pInfo->power_cost,false);
	}
    
	DM_GET_USER_MODEL->getUserInfo()->setLocked(true);
	if(responseData.isMember("drops")) 
	{
		drops(requestData, responseData["drops"]);
	}
    DM_GET_USER_MODEL->getUserInfo()->addExp(pSettlementInfo->exp, pSettlementInfo->inwar?5.f:2.f);
    DM_GET_FORMATION_MODEL->updateLockStatus(DM_GET_USER_MODEL->getUserInfo()->getUserLv());
    DM_GET_USER_MODEL->getUserInfo()->changeCoins(pSettlementInfo->coin, pSettlementInfo->inwar?5.f:2.f);
    if(pSettlementInfo->inwar) DM_GET_BATTLE_MODEL->setSettlementInfo(pSettlementInfo);
	
    
    //结算
    if (pSettlementInfo->wexp > 0)
    {
        FormationModel::PositionArray* pPosition = DM_GET_FORMATION_MODEL->getPositionsArray();
        if (pPosition)
        {
            CCObject* obj = NULL;
            formation::PositionInfo* pPositionInfo = NULL;
            
            CCARRAY_FOREACH(pPosition, obj)
            {
                pPositionInfo = (formation::PositionInfo*)obj;
                if (pPositionInfo)
                {
                    warrior::WarriorCurInfo *pCurInfo =  DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(pPositionInfo->iWarriorId);
                    bool isLevelUp = false;
                    if (pCurInfo)
                    {
                        pCurInfo->addExp(pSettlementInfo->wexp, pSettlementInfo->inwar?5.f:2.f,&isLevelUp);
                    }
                    pSettlementInfo->setWarriorisLevelup(pPositionInfo->iWarriorId, isLevelUp);
                }
            }
        }
	}
	DM_GET_USER_MODEL->getUserInfo()->setLocked(false, pSettlementInfo->inwar?5.f:2.f);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("PVE", "explore");
}

void HttpHandler::tower_get(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_TOWER_MODEL->tower_get(responseData);
}

void HttpHandler::tower_add(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_TOWER_MODEL->tower_add(responseData);
}

void HttpHandler::towerRank(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_TOWER_MODEL->updateTowerRanks(responseData["towerranks"],responseData["myrank"].asUInt(),last_response_time);
}

void HttpHandler::towerOldRank(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_TOWER_MODEL->updateTowerOldRanks(responseData["towerranks"]);
}

void HttpHandler::tower_reward(const Json::Value &requestData,const Json::Value &responseData)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asInt()));
}

void HttpHandler::FightResult(const Json::Value &requestData,const Json::Value &responseData)
{
    if(responseData.isMember("drops")) drops(requestData, responseData["drops"]);
    if(responseData.isMember("activeness")) activeness(requestData, responseData["activeness"]);
    tower::TowerInfo* pInfo = DM_GET_TOWER_MODEL->GetTowerInfo();
    uint32_t lastHard = pInfo->lastHard;

	uint32_t towerMid = 0;
	DJSON_UINT(towerMid,		responseData["tower"], "mid");
	bool canSkip = lastHard >= towerMid;
    
    DM_GET_TOWER_MODEL->tower_get(responseData["tower"],canSkip); //马上要进入战场再切出来会更新信息 所以不需要通知
    pInfo = DM_GET_TOWER_MODEL->GetTowerInfo();
    
	DM_GET_TOWER_MODEL->SetFightResult(responseData);
	if (!canSkip)
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE
		 ,cocos2d::CCInteger::create(canSkip?battle::kSkipBattleTypeSkip:battle::kSkipBattleTypeTower));
	}
	
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asInt()));

	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Tower", "fight");
}

void HttpHandler::cards(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_LOTTERY_MODEL->updateCards(responseData);
}

void HttpHandler::card(const Json::Value &requestData,const Json::Value &responseData)
{
    if(requestData["cost"].asBool() && requestData["type"].asUInt()==card::kCardTypeHard10)
    {
        DM_GET_LOTTERY_MODEL->getCardInfoByType(card::kCardTypeHard10)->setIsPaid(true);
    }
	DM_GET_LOTTERY_MODEL->updateCard(responseData);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Card", "draw");
}

void HttpHandler::hardDrawNum(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_LOTTERY_MODEL->setRestForOrange(responseData.asUInt());
}

void HttpHandler::activeDrawNum(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_LOTTERY_MODEL->setRestForSpecial(responseData.asUInt());
}

void HttpHandler::onDrawCard(const Json::Value &requestData,const Json::Value &responseData)
{
	//先仅处理限时抽卡的情况，type5，6的情况
	//if(responseData["card"]["type"] == )
	DM_GET_LOTTERY_MODEL->onDrawCard(requestData , responseData);
}

#pragma mark 许愿
/**
 *	@brief	获取许愿信息
 *
 *	@param 	requestData 	请求数据
 *	@param 	responseData 	响应数据
 */
void HttpHandler::getWishes(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_WISH_MODEL->wishesGet(requestData, responseData);
}

/**
 *	@brief	执行许愿
 *
 *	@param 	requestData 	请求数据
 *	@param 	responseData 	响应数据
 */
void HttpHandler::doWish(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_WISH_MODEL->doWishComplete(requestData, responseData);


	const Json::Value & res = responseData["wishRes"];
	item::ItemBase *pItem = DM_GET_ITEM_MODEL->getItemBaseByID(SKY_LIGHT_ID);
	int num = K_WISH_SKY_LIGHT_NUM_PER_TIME * res.size();
	AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
	pAnalytics->useItem(pItem->getItemName().c_str(), num);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Wish", "wish");
}

void HttpHandler::burn(const Json::Value &requestData,const Json::Value &responseData)
{
	//这里暂不处理，直接从model传入回调函数
}
void HttpHandler::getAutoList(const Json::Value &requestData,const Json::Value &responseData)
{
	//这里暂不处理，直接从model传入回调函数
}

void HttpHandler::wishChangeWarrior(const Json::Value &requestData, const Json::Value &responseData)
{
	//no data
}

void HttpHandler::getRookieInfo(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_TUTORIAL_MODEL->setLastStep(responseData["lastStep"].asUInt());
    
    std::vector<uint8_t> ids;
    DM_GET_TUTORIAL_MODEL->getLockedNavSteps(ids);
    //DM_GET_NAVIGATION_MODEL->setUnlockStatus(ids, false);
}

void HttpHandler::finishRookie(const Json::Value &requestData,const Json::Value &responseData)
{
    tutorial::StepInfo* pStep = DM_GET_TUTORIAL_MODEL->getNextStep();
    if(pStep && pStep->getNavId()>0) DM_GET_NAVIGATION_MODEL->setUnlockStatus(pStep->getNavId(), true);
    DM_GET_TUTORIAL_MODEL->setLastStep(requestData["id"].asUInt());

	AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
	pAnalytics->tutorialStep(requestData["id"].asUInt());
}



#pragma mark 奖励页面
/**
 *	@brief	是否开启特殊活动入口
 *
 *	@param 	requestData 	请求
 *	@param 	responseData 	响应
 */
void HttpHandler::activityGetSpecial(const Json::Value &requestData,const Json::Value &responseData)
{
    if (responseData.isMember("isOpen") && responseData.isMember("activityId"))
    {
        int activityId = 0;
        if(responseData["isOpen"].asBool() == true)
        {
            activityId = responseData["activityId"].asInt();
        }
        DM_GET_ACTIVITY_MODEL->setSpecialActivityType(activityId);
    }
}

/**
 *	@brief	获取页面数据
 *
 *	@param 	requestData 	请求
 *	@param 	responseData 	响应
 */
void HttpHandler::activityGetAward(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_PRIZE_MODEL->parsePrize(responseData["award"]);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Activity", "award");
}

/**
 *	@brief	领取累计登录奖励
 *
 *	@param 	requestData 	请求数据
 *	@param 	responseData 	响应数据
 */
void HttpHandler::activityTLoginAward(const Json::Value &requestData,const Json::Value &responseData)
{
    uint32_t days = requestData["d"].asUInt();
    DM_GET_PRIZE_MODEL->onTLoginPrizeDrawn(days);
    
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW, cocos2d::CCInteger::create(requestData[UUID].asUInt()));
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Activity", "loginAward");
}


/**
 *	@brief	领取等级奖励
 *
 *	@param 	requestData 	请求数据
 *	@param 	responseData 	响应数据
 */
void HttpHandler::activityLvUpAward(const Json::Value &requestData,const Json::Value &responseData)
{
    uint32_t level = requestData["lv"].asUInt();
    DM_GET_PRIZE_MODEL->onLevelPrizeDrawn(level);
    
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW, cocos2d::CCInteger::create(requestData[UUID].asUInt()));
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Activity", "levelAward");
}

#pragma mark 系统


#pragma mark 累充页面(临时）
/**
 *	@brief	获取页面数据
 *
 *	@param 	requestData 	请求
 *	@param 	responseData 	响应
 */
void HttpHandler::activityGetChargeAward(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ACTIVITY_MODEL->parseInfo(responseData);
}

/**
 *	@brief	领取累计登录奖励
 *
 *	@param 	requestData 	请求数据
 *	@param 	responseData 	响应数据
 */
void HttpHandler::activityChargeAward(const Json::Value &requestData,const Json::Value &responseData)
{
	//uint32_t id = requestData["id"].asUInt();
	//DM_GET_CHARGE_PRIZE_MODEL->onPrizeDrawn(id);
    
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW, cocos2d::CCInteger::create(requestData[UUID].asUInt()));
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Activity", "chargeAward");
}


/**
 *	@brief	初始化数据
 *
 *	@param 	requestData 	请求数据
 *	@param 	responseData 	响应数据
 */
void HttpHandler::initData(const Json::Value &requestData,const Json::Value &responseData)
{
    TimeUtil::init(responseData["timezone"].asCString());
    GameManager::sharedGameManager()->registerLocalNotification();
    activityGetAward(requestData, responseData);
    DM_GET_ANNOUNCE_MODEL->parseAnnounces(responseData["announces"]);
    DM_GET_SHARE_MODEL->setShareTimes(responseData["sharedTimes"].asUInt());
	
    UserModel *pUser = DM_GET_USER_MODEL;
	if (responseData.isMember("exploreActivity"))
	{
		pUser->parseExploreActivity(responseData["exploreActivity"]);
	}
	
	//HTTP_CLIENT->userPrivileges();
    PlatformModel *pPlatform = DM_GET_PLATFORM_MODEL;
        
    ConfigModel *pConfig = DM_GET_CONFIG_MODEL;
    pConfig->setConfigByKey(DISABLE_SETTING, responseData[DISABLE_SETTING].asCString());
    pConfig->setConfigByKey(DISABLE_ZZB, responseData[DISABLE_ZZB].asCString());

	GuildBossModel *pGuildBoss = DM_GET_GUILDBOSS_MODEL;
	pGuildBoss->parseOpenTs(responseData);

    AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
	pAnalytics->setSysInited(true);
	pAnalytics->login(CCString::createWithFormat("%u", pUser->getUserInfo()->getUserId())->getCString(), pUser->getUserInfo()->getUserLv());
    pAnalytics->initAccount(CCString::createWithFormat("%u", pUser->getUserInfo()->getUserId())->getCString());
	pAnalytics->setAccountType(kAccountType1);
    pAnalytics->setAccountName(pUser->getUserInfo()->getUserName().c_str());
    pAnalytics->setAccountLevel(pUser->getUserInfo()->getUserLv());
    pAnalytics->setGameServer(pPlatform->getCurrentServer()->getName().c_str());

	PlatformManager::sharedPlatformManager()->setServerInfoToPlugin();
    
    AdsManager::sharedAdsManager()->setEnableAd(responseData["ENABLE_AD"].asString().compare("0")!=0);
    AdsManager::sharedAdsManager()->showAds(10.0f);
    
    if (responseData.isMember("ENABLE_CDKEY")) {
        if (responseData["ENABLE_CDKEY"].asString().compare("0") !=0) {
            DM_GET_SETTING_MODEL->setEnableCDKey(true);
        }
    }
	if (responseData.isMember("m_SelectMode")) {
			if(responseData["m_SelectMode"].asBool())
				DM_GET_TUTORIAL_MODEL->setSelectMode(true);
			if(!responseData["m_SelectMode"].asBool())
				DM_GET_TUTORIAL_MODEL->setSelectMode(false);
	}
	
	//周活动
	WeeklyModel *pWeeklyModel = DM_GET_WEEKLY_MODEL;
	if(responseData.isMember("weeklyTypes"))
	{
		pWeeklyModel->parseInfo(responseData["weeklyTypes"]);
	}
	if(responseData.isMember("weeklyTypes"))
	{
		pWeeklyModel->setOpenTs(responseData["weeklyStartTs"].asUInt());
	}
}

/**
 *	@brief	获取通知
 *
 *	@param 	requestData 	请求数据
 *	@param 	responseData 	响应数据
 */
void HttpHandler::parseNotice(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_NOTICE_MODEL->parseNotice(notice::kNoticeListNormal, responseData);
}

/**
 *	@brief	获取通知
 *
 *	@param 	requestData 	请求数据
 *	@param 	responseData 	响应数据
 */
void HttpHandler::getNotices(const Json::Value &requestData,const Json::Value &responseData)
{
    notice::NoticeListType listType = (notice::NoticeListType)requestData["list"].asUInt();
    DM_GET_NOTICE_MODEL->parseNotices(listType, responseData["notices"]);
}


#pragma mark - 充值 -


void HttpHandler::getChargeResult(const Json::Value &requestData,const Json::Value &responseData)
{
	if (!responseData.isMember("cAmount") || 0 == responseData["cAmount"].asUInt()) return;
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_USER_CHARGE_SUC,cocos2d::CCInteger::create(responseData["cAmount"].asUInt()));
    CCLOG("ChargeResult post D_EVENT_USER_CHARGE_SUC, cAmount: %d", responseData["cAmount"].asUInt());
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW, cocos2d::CCInteger::create(requestData[UUID].asUInt()));
    
    AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
    pAnalytics->chargeSuccess();
    /*platform::RechargeInfo *pInfo = DM_GET_PLATFORM_MODEL->getRechargeInfo();
    pAnalytics->reward(pInfo->getPoint() * pInfo->getExtRate(), "chargeReward");*/
}




void HttpHandler::dresses(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_AVATAR_MODEL->parseDresses(responseData);
}

void HttpHandler::soulExchange(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ACTIVITY_MODEL->parseSoulExchange(responseData);
}

#pragma mark- 公会 -

void HttpHandler::guildCreate(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILD_MODEL->parse(responseData);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Guild", "create");
}

void HttpHandler::guildApply(const Json::Value &requestData, const Json::Value &responseData)
{
	uint32_t guildID = requestData["gid"].asUInt();
	uint32_t guildAction = requestData["act"].asUInt();
	DM_GET_GUILD_MODEL->changeSelfApplication(guildID, guildAction);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Guild", "apply");
}

void HttpHandler::guildDealApply(const Json::Value &requestData, const Json::Value &responseData)
{
	uint32_t applicationID = requestData["id"].asUInt();
	uint32_t guildAction = requestData["act"].asUInt();
	DM_GET_GUILD_MODEL->dealGuildApplication(applicationID, guildAction);
	//接受申请时，重新读帮会列表
	DM_GET_GUILD_MODEL->parseGuildInfo(responseData["guild"]);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Guild", "dealApply");
}

void HttpHandler::guildGet(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILD_MODEL->parse(responseData);
}

void HttpHandler::guildGetList(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILD_MODEL->parseSelfApplication(responseData["guildApplys"]);
	uint32_t startNum = requestData["id"].asUInt();
	DM_GET_GUILD_MODEL->parseGuildList(responseData["guildBasics"], startNum);
}

void HttpHandler::guildSearch(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILD_MODEL->parseGuildList(responseData);
}

void HttpHandler::guildQuit(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILD_MODEL->quitGuild();
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Guild", "quit");
}

void HttpHandler::guildPosition(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILD_MODEL->changePosition(requestData, responseData);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Guild", "changePosition");
}

void HttpHandler::guildSign(const Json::Value &requestData, const Json::Value &responseData)
{
	GuildModel *pModel = DM_GET_GUILD_MODEL;
	guild::GuildUser *user = pModel->getSelfInfo();
	user->setLastSign(responseData["guildUser"]["sign_ts"].asUInt());
	user->setIsSigned(true);
	pModel->refreshContribution(responseData);

	pModel->addGuildNotice(responseData["notice"]);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Guild", "sign");
}

void HttpHandler::guildQues(const Json::Value &requestData, const Json::Value &responseData)
{
	GuildModel *pModel = DM_GET_GUILD_MODEL;
	int res = responseData["res"].asInt();
	pModel->getSelfInfo()->setQuestionID(0);
	pModel->getSelfInfo()->setLastAnswer(responseData["guildUser"]["ques_ts"].asUInt());
	if (1 == res)
	{
		pModel->refreshContribution(responseData);

		pModel->addGuildNotice(responseData["notice"]);
	}
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Guild", "answer");
}

void HttpHandler::guildDetails(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILD_MODEL->parseOtherGuildInfo(responseData["guild"]);
}

void HttpHandler::guildUpdate(const Json::Value &requestData, const Json::Value &responseData)
{
	std::string announcement = requestData["content"].asString();
	DM_GET_GUILD_MODEL->getGuildInfo()->setAnnouncement(announcement);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Guild", "announcement");
}

void HttpHandler::guildApplyList(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILD_MODEL->parseGuildApplication(responseData["guildApplys"]);
}

void HttpHandler::guildUpgradeTree(const Json::Value &requestData, const Json::Value &responseData)
{
	GuildModel *pModel = DM_GET_GUILD_MODEL;
	pModel->refreshContribution(responseData);
	if (responseData["guildUser"].isMember("king_times"))
	{
		//DM_GET_GUILD_MODEL->getSelfInfo()->setOverlord(responseData["guildUser"]["king_times"].asUInt());
		pModel->setOverlordTimes(responseData["guildUser"]["king_times"].asUInt());

		pModel->addGuildNotice(responseData["notice"]);
	}

	if (responseData["guildUser"].isMember("knight_times"))
	{
		pModel->getSelfInfo()->setChivalry(responseData["guildUser"]["knight_times"].asUInt());

		pModel->addGuildNotice(responseData["notice"]);
	}
	pModel->parseTree(responseData["guild"]);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Guild", "upgradeTree");
}

void HttpHandler::guildGetNotice(const Json::Value &requestData, const Json::Value &responseData)
{
	guild::GuildNoticeType iType = (guild::GuildNoticeType)requestData["type"].asUInt();
	DM_GET_GUILD_MODEL->parseGuildNotice(responseData["guildNotices"], iType);
}


void HttpHandler::guildMsg(const Json::Value &requestData, const Json::Value &responseData)
{
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Guild", "message");
}

void HttpHandler::guildMallRefresh(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILD_MODEL->parseGuildMall( responseData["guildMall"], 
		responseData["remainConValue"].asInt() );
}

void HttpHandler::guildMallExchange(const Json::Value &requestData, const Json::Value &responseData)
{
	guild::GuildMall *m_pGuildMall = DM_GET_GUILD_MODEL->getGuildMall() ;
	m_pGuildMall->addExchanged( requestData["exchangeId"].asUInt() ) ;
	DM_GET_GUILD_MODEL->parseGuildMall( responseData["guildMall"], 
		responseData["remainConValue"].asInt() );
}

void HttpHandler::guildSubstitute(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILD_MODEL->selfReplaceBoss(responseData["newConLv"].asUInt());
}

void HttpHandler::guildLog(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILD_MODEL->parseGuildLog(responseData["guildLogs"]);
}

#pragma mark- 公会战 -

void HttpHandler::guildBossGet(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseInformation(responseData);
}

void HttpHandler::guildBossDeclareWar(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseDeclaredTarget(requestData);
}

void HttpHandler::guildBossDeclareTargets(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseTargets(requestData, responseData["guildBossList"]);
}

void HttpHandler::guildBossStart(const Json::Value &requestData, const Json::Value &responseData)
{
	//DM_GET_GUILDBOSS_MODEL->getBoss()->parse(responseData["guildBossUserDetail"]);
	DM_GET_GUILDBOSS_MODEL->parseInformation(responseData);
	DM_GET_GUILDBOSS_MODEL->parseMemberRank(responseData["guildBossUserList"]);
}

void HttpHandler::guildBossGetWoundedList(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseWoundedUsers(responseData["woundeds"]);
}

void HttpHandler::guildBossMemberRank(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseMemberRank(responseData["guildBossUserList"]);
}

void HttpHandler::guildBossGuildRank(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseGuildRank(requestData, responseData["guildBossList"]);
}

void HttpHandler::guildBossGetTargetList(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseAttackTargets(responseData["guildBossUserList"]);
}

void HttpHandler::guildBossGetHatredList(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseHatredGuilds(responseData["hatred"]);
}

void HttpHandler::guildBossAttackBoss(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseDamage(responseData);
}

void HttpHandler::guildBossAutoAttackBoss(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseAutoDamage(responseData);
}

void HttpHandler::guildBossRob(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseRobDamage(responseData);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE);
}

void HttpHandler::guildBossRobSettle(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseRobResult(responseData);
}

void HttpHandler::guildBossRescue(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->removeWounded(requestData);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}

void HttpHandler::guildBossGetNotice(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseNotices(responseData["guildBossNotices"]);
}

void HttpHandler::guildBossDailySettle(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->parseAllResult(responseData);
}

#pragma mark - 皇陵 -

void HttpHandler::dragonvalleySearchFail(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ERRORTIP_MODEL->showErrorTips(KINGTOME_SEARCH_NOT_EXIST);
	DM_GET_KINGTOMB_MODEL->setStopAin(true);
}

void HttpHandler::dragonvalleyGetsearch(const Json::Value &requestData, const Json::Value &responseData)
{

}
void HttpHandler::dragonvalleyGetoccupy(const Json::Value &requestData, const Json::Value &responseData)
{

}
void HttpHandler::dragonvalleyGetoccupylist(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_KINGTOMB_MODEL->parseAssistUserIds(responseData["allAssistUserIds"]);
	DM_GET_KINGTOMB_MODEL->setSearchTimes(responseData["searchTimes"].asInt());
	DM_GET_KINGTOMB_MODEL->parseOccupyList(responseData["occupyList"]);
}
void HttpHandler::dragonvalleySpeedup(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_KINGTOMB_MODEL->changeKingTombInfo(responseData["occupy"]);
}
void HttpHandler::dragonvalleyOccupyfinish(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_KINGTOMB_MODEL->removeKingTombInfoByPos(DM_GET_KINGTOMB_MODEL->getRewordPos(),false);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}

void HttpHandler::dragonvalleySearch(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_KINGTOMB_MODEL->setSearchTimes(DM_GET_KINGTOMB_MODEL->getSearchTimes()+1);
	DM_GET_KINGTOMB_MODEL->changeKingTombInfo(responseData["occupy"],true);
}

void HttpHandler::dragonvalleyUserRank(const Json::Value &requestData, const Json::Value &responseData)
{
	//先处理 meetRare
	DM_GET_KINGTOMB_MODEL->parseMeetRare(responseData["meetRare"]);
	DM_GET_KINGTOMB_MODEL->parseRank(responseData["myGuildRank"] , true);
}

void HttpHandler::dragonvalleyGuildRank(const Json::Value &requestData, const Json::Value &responseData)
{
	//先处理 meetRare
	DM_GET_KINGTOMB_MODEL->parseMeetRare(responseData["meetRare"]);
	DM_GET_KINGTOMB_MODEL->parseRank(responseData["guildRank"] , false);
}


void HttpHandler::dragonvalleyGetassistbonuslist(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_KINGTOMB_MODEL->parseAssistBonusList(responseData["assistBonusList"]);
}
void HttpHandler::dragonvalleyGetassistbonus(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_KINGTOMB_MODEL->removeAssistBonusById(requestData["id"].asUInt());

	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}
void HttpHandler::dragonvalleyGetallassistbonus(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_KINGTOMB_MODEL->removeAllAssistBonus();
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}
void HttpHandler::dragonvalleyBattlestart(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_KINGTOMB_MODEL->addAssistUserId();
	DM_GET_KINGTOMB_MODEL->removeSeleteUse();
	DM_GET_KINGTOMB_MODEL->setDragonEnd(responseData["dragonEnd"].asBool());
	DM_GET_KINGTOMB_MODEL->setDragonWin(responseData["dragonWin"].asBool());
	DM_GET_KINGTOMB_MODEL->changeKingTombInfo(responseData["occupy"]);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE,cocos2d::CCInteger::create(battle::kSkipBattleTypeSkip));
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asInt()));
}
void HttpHandler::dragonvalleyGetlog(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_KINGTOMB_MODEL->parseLog(responseData["occupyLogList"]);
}

#pragma mark - 狩魂 -
/**
 *	@brief	获取玩家狩魂信息
 *
 *	@param 	requestData 	请求
 *	@param 	responseData 	响应
 */
void HttpHandler::huntGet(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_SOUL_HUNTER_MODEL->parseSoulHunterInfo(responseData["hunt"]);
}

/**
 *	@brief	获取目标列表
 *
 *	@param 	requestData 	请求数据
 *	@param 	responseData 	响应数据
 */
void HttpHandler::huntTargets(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_SOUL_HUNTER_MODEL->parseSoulHunterRivalsInfo(responseData["huntTargets"]);
}


/**
 *	@brief	攻击目标
 *
 *	@param 	requestData 	请求数据
 *	@param 	responseData 	响应数据
 */
void HttpHandler::huntAttack(const Json::Value &requestData,const Json::Value &responseData)
{	
	DM_GET_SOUL_HUNTER_MODEL->parseSoulHunterAwardInfo(requestData,responseData);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE);
	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("hunt", "attack");
}

void HttpHandler::chatSendGuild(const Json::Value& requestData, const Json::Value& responseData)
{
	DM_GET_CHAT_MODEL->parseSendGuild(requestData, responseData);
}

void HttpHandler::chatGetGuild(const Json::Value& requestData, const Json::Value& responseData)
{
	DM_GET_CHAT_MODEL->parseGuildInfo(requestData, responseData);
}

void HttpHandler::chatSendWorld(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_CHAT_MODEL->parseSendWorld(requestData, responseData);
}

void HttpHandler::chatGetWorld(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_CHAT_MODEL->parseWorldInfo(requestData, responseData);
}




void HttpHandler::productBuy(const Json::Value &requestData, const Json::Value &responseData)
{
    
    uint32_t itemID = 0;
	DJSON_UINT(itemID,requestData, "id");
    uint32_t num = 0;
    DJSON_UINT(num, requestData, "num");
    
    mall::ProductInfo* pItem = DM_GET_MALL_MODEL->getProductById(itemID);
    if(pItem && pItem->getBaseInfo()->getType()==mall::kProductTypeAvatar)
    {
        DM_GET_WARRIOR_MODEL->dressUp(DM_GET_AVATAR_MODEL->getAvatarInfoById(pItem->getBaseInfo()->getItemId()),false);
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
    }else
    {
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_REMOVE,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
    }
    AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
    pAnalytics->purchaseItem(pItem->getBaseInfo()->getProductName().c_str(),
                             num, pItem->getPrice());

	AnalyticsManager::sharedAnalyticsManager()->logEventWithMsg("Product", "buy");
}

void HttpHandler::redPacketBuy(const Json::Value &requestData, const Json::Value &responseData)
{
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}

void HttpHandler::decomposeEquip(const Json::Value &requestData, const Json::Value &responseData)
{
	// 本地删除装备
	const Json::Value& equis = requestData["ids"];
	for (Json::Value::iterator iter = equis.begin(); iter != equis.end(); ++iter)
	{
		DM_GET_EQUIP_MODEL->removeEquip((*iter).asInt());
	}

	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));

	//宝物炼化
	DM_GET_BURN_MODEL->parseBurnTreasure(requestData , responseData);
}

void HttpHandler::taskGetAward(const Json::Value &requestData, const Json::Value &responseData)
{
	//领奖 发掉落消息
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
	//领奖后服务器会重新发送新的 成就列表
}

void HttpHandler::taskGetScoreAward(const Json::Value &requestData, const Json::Value &responseData)
{
	//领奖 发掉落消息
	DM_GET_ACHIEVEMENT_MODEL->onGetScoreAward(requestData , responseData);
}

void HttpHandler::friendChallenge(const Json::Value &requestData, const Json::Value &responseData)
{
	//设置类型，设置可以跳过战斗
	DM_GET_BATTLE_MODEL->setBattleType(battle::kBattleTypeFriend);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE,CCInteger::create(battle::kSkipBattleTypeSkip));
}

void HttpHandler::friendRecommend(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIEND_MODEL->parseRecommend(responseData);
}

void HttpHandler::friendSearch(const Json::Value &requestData, const Json::Value &responseData)
{
	//DM_GET_FRIENDS_MODEL->parseFound(responseData["stranger"]);
}

void HttpHandler::friendSendRequest(const Json::Value &requestData, const Json::Value &responseData)
{
	//DM_GET_FRIENDS_MODEL->requestSent(requestData);
}

void HttpHandler::friendAccept(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIEND_MODEL->removeRequest(requestData["fid"].asUInt());
	DM_GET_FRIEND_MODEL->addFriend(responseData["friend"]);
}

void HttpHandler::friendRefuse(const Json::Value &requestData, const Json::Value &responseData)
{
	//DM_GET_FRIENDS_MODEL->removeRequest(requestData["targetId"].asUInt());
	DM_GET_FRIEND_MODEL->removeRequest(requestData["fid"].asUInt());
}

void HttpHandler::friendRefuseAll(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIEND_MODEL->clearRequest();
}

void HttpHandler::friendDeleteFriend(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIEND_MODEL->removeFriends(requestData);
}

void HttpHandler::friendGetRequests(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIEND_MODEL->parseRequest(responseData);
}

void HttpHandler::friendSendMail(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIENDS_MODEL->msgSent();
}


void HttpHandler::friendDeleteMail(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_MAIL_MODEL->friendMailDeleted(requestData);
}

void HttpHandler::friendSendGit(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIEND_MODEL->onSendGift(requestData , responseData);
}

void HttpHandler::friendReceiveGift(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIEND_MODEL->onReceiveGift(requestData);

	//DM_GET_FRIENDS_MODEL->giftReceived(requestData);
	//移除掉落
    //NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_REMOVE,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}

void HttpHandler::friendGetNewRequest(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIEND_MODEL->requestsCountGot(responseData);
}

void HttpHandler::friendGetFriends(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIEND_MODEL->parseFriend(responseData["friends"]);
}

void HttpHandler::friendDungeon(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIENDS_MODEL->parseDungeon(responseData["friendDungeon"]);
}

void HttpHandler::friendResetDungeon(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIENDS_MODEL->resetDungeon(requestData);
}

void HttpHandler::friendGo(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIENDS_MODEL->parseDungeon(responseData["friendDungeon"]);

	std::map<uint32_t, uint32_t> mBuffItemNum = DM_GET_FRIENDS_MODEL->getBuffItemNum();
	for (std::map<uint32_t, uint32_t>::iterator iter = mBuffItemNum.begin(); iter != mBuffItemNum.end(); iter++)
	{
		DM_GET_ITEM_MODEL->changeItemNumByID(iter->first, -iter->second);
	}
	mBuffItemNum.clear();
	DM_GET_FRIENDS_MODEL->setBuffItemNum(mBuffItemNum);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE,cocos2d::CCInteger::create(battle::kSkipBattleTypeTeam));
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}

void HttpHandler::friendRush(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIENDS_MODEL->parseDungeonRush(responseData["friendDungeon"]);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}

void HttpHandler::receiveAndSendGiftAll(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_FRIEND_MODEL->onReceiveAndSendGiftAll();
}

void HttpHandler::activityShareAward(const Json::Value& requestData, const Json::Value& responseData)
{
	DM_GET_SHARE_MODEL->setShareTimes(DM_GET_SHARE_MODEL->getShareTimes()+1);
    //NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}




////////////////////////////////////////////////////////////////////////////////////////




#pragma mark- 错误处理 -

void HttpHandler::userNotLogin(const Json::Value &requestData,const Json::Value &responseData)
{
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_FORCE_LOGOUT,CCStringMake(DM_GET_ERRORTIP_MODEL->getErrorTip(USER_NOT_LOGON)));
}

void HttpHandler::systemMaintain(const Json::Value &requestData,const Json::Value &responseData)
{
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SYSTEM_MAINTAIN,CCStringMake(DM_GET_ERRORTIP_MODEL->getErrorTip(SYSTEM_MAINTAIN)));
}

void HttpHandler::theRoundEnded(const Json::Value &requestData,const Json::Value &responseData)
{
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_TOWER_TOWER_OUT_OF_DATE,CCStringMake(DM_GET_ERRORTIP_MODEL->getErrorTip(TOWER_OUT_OF_DATE)));
}


void HttpHandler::soulNotEnough(const Json::Value &requestData,const Json::Value &responseData)
{
    DM_GET_ERRORTIP_MODEL->showErrorTips(SOUL_NOT_ENOUGH);
    HTTP_CLIENT->getSouls(NULL,NULL,false);
}

void HttpHandler::soulHunterNotExist(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_SOUL_HUNTER_MODEL->handleRivalNotExist();
	DM_GET_ERRORTIP_MODEL->showErrorTips(SOUL_HUNTER_NOT_EXIST);
}

void HttpHandler::soulHunterNoMoreSnatch(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_SOUL_HUNTER_MODEL->handleWarriorNoMoreSnatch();
	Tips_Error(D_LOCAL_STRING("SoulHunterNotWarrior"));
}

void HttpHandler::guildNoGuild(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->errorNoGuild();
	DM_GET_GUILD_MODEL->errorNoGuild();
}

void HttpHandler::guildHasGuild(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ERRORTIP_MODEL->showErrorTips(GUILD_HAS_GUILD);
	HTTP_CLIENT->guildGet();
}

void HttpHandler::guildBossErrorUserRescued(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ERRORTIP_MODEL->showErrorTips(GUILDBOSS_ERROR_USER_RESCUED);
	DM_GET_GUILDBOSS_MODEL->syncWounded(true);
}

void HttpHandler::guildPermissionDenied(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ERRORTIP_MODEL->showErrorTips(GUILD_PERMISSION_DENIED);
	HTTP_CLIENT->guildGet();
}

void HttpHandler::guildBossNewJoin(const Json::Value &requestData,const Json::Value &responseData)
{
}

void HttpHandler::guildBossNoData(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_GUILDBOSS_MODEL->setHasSettleData(false);
}

void HttpHandler::friendsNotExist(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ERRORTIP_MODEL->showErrorTips(FRIENDS_NOT_EXIST);
	DM_GET_FRIENDS_MODEL->selectFriend(NULL);
	HTTP_CLIENT->getFriends();
}

void HttpHandler::friendsRequestsNeedRefresh(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ERRORTIP_MODEL->showErrorTips(FRIENDS_REQUEST_NOT_EXIST);
	HTTP_CLIENT->friendGetNewRequest();
	HTTP_CLIENT->getFriendRequests();
}

void HttpHandler::friendsTargetFull(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ERRORTIP_MODEL->showErrorTips(FRIENDS_TARGET_FULL);
	HTTP_CLIENT->friendGetNewRequest();
	HTTP_CLIENT->getFriendRequests();
}

void HttpHandler::friendsGiftFull(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ERRORTIP_MODEL->showErrorTips(FRIENDS_GIFT_FULL);
	DM_GET_FRIENDS_MODEL->friendGiftFull(requestData[ACTION]);
}

void HttpHandler::friendsWarriorNotExist(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_ERRORTIP_MODEL->showErrorTips(FRIENDS_WARRIOR_NOT_EXIST);
	DM_GET_FRIENDS_MODEL->selectFriend(NULL);
	HTTP_CLIENT->getFriends();
}

void HttpHandler::wonderTrader(const Json::Value &requestData, const Json::Value &responseData)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}

void HttpHandler::wonderBox(const Json::Value &requestData, const Json::Value &responseData)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}
void HttpHandler::wonderSuperior(const Json::Value &requestData, const Json::Value &responseData)
{

}
void HttpHandler::wonderTreasure(const Json::Value &requestData, const Json::Value &responseData)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}
void HttpHandler::wonderWarrior(const Json::Value &requestData, const Json::Value &responseData)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}
void HttpHandler::wonderFight(const Json::Value &requestData, const Json::Value &responseData)
{
	if (requestData["gg"].asBool() == false)
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE
			,cocos2d::CCInteger::create(battle::kSkipBattleTypeSkip));
	}
	
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}

void HttpHandler::wonderFightBoss(const Json::Value &requestData, const Json::Value &responseData)
{
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE
                                                                     ,cocos2d::CCInteger::create(battle::kSkipBattleTypeSkip));
}

void HttpHandler::signinGet(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_SIGNIN_MODEL->parseData(responseData);
}

void HttpHandler::signinSignin(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_SIGNIN_MODEL->parseData(responseData["signIn"]);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));

}

void HttpHandler::onlineAward(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_ACTIVITY_MODEL->parseOnlineAward(requestData, responseData);
}

void HttpHandler::getGundam(const Json::Value &requestData, const Json::Value &responseData)
{
    DM_GET_GUNDAM_EF_MODEL->parseFomations(responseData);
}

void HttpHandler::joinGundam(const Json::Value &requestData, const Json::Value &responseData)
{
	DM_GET_GUNDAM_EF_MODEL->parseFomations(responseData["gundamFormation"]);
}

void HttpHandler::rewardGet(const Json::Value &requestData, const Json::Value &responseData)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}
////////////////////////////////////////////////////////////////////////////////////////

#pragma mark- 处理数据 -

void HttpHandler::parseResult(const Json::Value &requestData,const Json::Value &responseData,bool autoSave)
{
    last_response_time = (time_t)responseData["ts"].asUInt();
    if(responseData["code"].asString().compare(SUC_CODE)==0)
    {
        if(autoSave)
        {
            parseData(requestData,responseData);
        }
        parseRequest(requestData,responseData);
		if(responseData["data"].isMember("isShowFateAnimation")){
			DM_GET_BATTLE_MODEL->setShowFateAni(responseData["data"]["isShowFateAnimation"].asBool());
		}		
    }else
    {
        parseError(requestData,responseData);
    }
}

void HttpHandler::parseRequest(const Json::Value &requestData,const Json::Value &responseData)
{
    SEL_CallFuncJson fun = exeFuns[requestData[SERVICE_METHOD].asString()];
    if(fun!=NULL)
    {
        (this->*fun)(requestData[ACTION],responseData["data"]);
    }
}

void HttpHandler::parseData(const Json::Value &requestData,const Json::Value &responseData)
{
    SEL_CallFuncJson fun;
    Json::Value::Members members(responseData["data"].getMemberNames());
    for (Json::Value::Members::reverse_iterator it = members.rbegin(); it!=members.rend(); ++it) {
        const std::string &key =*it;
        fun = parseFuns[key];
        if(fun!=NULL)
        {
            (this->*fun)(requestData[ACTION],responseData["data"][key.c_str()]);
        }
    }
}

void HttpHandler::parseError(const Json::Value &requestData,const Json::Value &responseData)
{
    SEL_CallFuncJson fun = errorFuns[responseData["code"].asString()];
    if(fun!=NULL)
    {
        (this->*fun)(requestData,responseData);
    }else
    {
        DM_GET_ERRORTIP_MODEL->showErrorTips(responseData["code"].asString());
        //CCLOG("Error Code %s",getErrorTip(responseData["code"].asString().c_str()));
    }
    //AnalyticsManager::sharedAnalyticsManager()->logErrorWithMsg(responseData["code"].asString().c_str());
}

#pragma mark- 周活动 -
void HttpHandler::weeklyActivities(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_WEEKLY_MODEL->parseWeekly(responseData);
}

void HttpHandler::weeklyActivity(const Json::Value &requestData,const Json::Value &responseData)
{
	DM_GET_WEEKLY_MODEL->parseOneWeekly(responseData);
}
