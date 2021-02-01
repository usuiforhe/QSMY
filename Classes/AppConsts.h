//
//  AppConsts.h
//  游戏一些全局常量
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef QSMY_AppConsts_h
#define QSMY_AppConsts_h

#include "AppInclude.h"
#include <limits>

static const cocos2d::CCSize DESIGN_SIZE = cocos2d::CCSizeMake(640, 1136);



#ifndef INT8_MAX
#define INT8_MAX         std::numeric_limits<int8_t>::max()
#endif

#ifndef INT16_MAX
#define INT16_MAX        std::numeric_limits<int16_t>::max()
#endif

#ifndef INT32_MAX
#define INT32_MAX        std::numeric_limits<int32_t>::max()
#endif

#ifndef INT64_MAX
#define INT64_MAX        std::numeric_limits<int64_t>::max()
#endif

#ifndef INT8_MIN
#define INT8_MIN          std::numeric_limits<int8_t>::min()
#endif

#ifndef INT16_MIN
#define INT16_MIN         std::numeric_limits<int16_t>::min()
#endif

#ifndef INT32_MIN
#define INT32_MIN        std::numeric_limits<int32_t>::min()
#endif

#ifndef INT64_MIN
#define INT64_MIN        std::numeric_limits<int64_t>::min()
#endif

#ifndef UINT8_MAX
#define UINT8_MAX         std::numeric_limits<uint8_t>::max()
#endif

#ifndef UINT16_MAX
#define UINT16_MAX        std::numeric_limits<uint16_t>::max()
#endif

#ifndef UINT32_MAX
#define UINT32_MAX        std::numeric_limits<uint32_t>::max()
#endif

#ifndef UINT64_MAX
#define UINT64_MAX        std::numeric_limits<uint64_t>::max()
#endif



#define API_INTERFACE   "interface.php"     //游戏通讯接口
#define API_MY_SERVERS_LIST "servers.php"   //最近服务器列表
#define API_LOGIN       "login.php"         //游戏登陆接口
#define API_CHECKMAINT  "checkMaint.php"    //检查维护接口
#define API_LOING_CB    "login_cb.php"      //获取平台登陆回调信息
#define GUESS_LOGIN     "guess_login.php" //游戏游客登陆接口

#define API_REFRESH_TOKEN "refresh.php"     //刷新授权信息
#define API_MAINT       "maint.html"        //维护页面
#define API_CHECK_VERSION "version.php"     //检查版本
#define API_SERVER_LIST    "server.php"     //服务器列表
#define  API_ANNOUNCE		"announce.php"	//公告(未登陆前使用)

#define API_RESET_PASS "resetpw.php"  //重新设置密码
#define API_REGISTER "register.php"  //注册

#define COOKIE_PATH cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+"cookie"

//高于此值加万字
#define MAX_SHOW_COIN	100000

#define USER_EQUIP_LEVEL_RATIO		3										//玩家级别和装备级别的关系系数
#define USER_WARRIOR_LEVEL_RATIO	3										//玩家级别和弟子级别的关系系数 传功时约束


#define  WARRIOR_GROW_RATIO		1.746228                                    //弟子属性成长系数
#define  WARRIOR_UP_RATIO		0.025                                       //突破成长系数
#define  WARRIOR_UP_LV_MAX		10                                          //弟子突破等级上限
#define  WARRIOR_UP_LV_MIN		0                                           //弟子突破等级上限
#define  WARRIOR_LV_MAX         180                                         //弟子等级上限

#define BUY_POWER_ONCE          20                                          //单次购买体力值

#define  WARRIOR_DEV_LV_MIN 20

#define FORMATION_POS_MAX		7                                           //阵型一方最大位置数

#define SKILL_LV_MAX		10                                              //技能等级上限

#define MAILLIST_REFRESH_INTERVAL	300                                     //5min	 邮件列表刷新间隔

#define TOWER_RANK_REFRESH_INTERVAL	600                                     //10min	 千机楼排行榜刷新

#define OTHER_FORMATION_REFRESH_INTERVAL 3600                               //一笑

//#define MIN_PAY_VIP_LEVEL 2                                                 //最小收费vip等级

#define BATTLE_GRADE_COUNT 3                                                //战斗最大星数

#define  NOTICE_FONT_SIZE		11

static const unsigned int G_FIRST_BATTLE_ID = 500;                          //新手引导第一场战斗的id

/****************** user default key *******************/
#pragma mark - user default key -

#define KEY_WITH_USER(key)	\
	CCString::createWithFormat("%s_%s",key,PlatformManager::sharedPlatformManager()->getUserId().c_str())->getCString()

#define KEY_GUESS_USER_NAME       "KEY_GUESS_USER_NAME"                                                     //绑定的游客用户名
#define KEY_lOGINED_USERS       "KEY_lOGINED_USERS"                                                     //上次登陆用户名
#define KEY_USER_NAME       "KEY_USER_NAME"                                                     //上次登陆用户名
#define KEY_USER_PLATID       "KEY_USER_PLATID"                                                     //上次登陆用户平台唯一Id
#define KEY_PASSWORD      "KEY_PASSWORD"                                                     //上次登陆密码
#define KEY_FACEBOOK_NAME   "KEY_FACEBOOK_NAME"                                             //上次登录的Facebook名称
#define KEY_USER_EMAIL       "KEY_USER_EMAIL"                                                     //上次登陆email
#define KEY_USER_UNIQUEID       "KEY_USER_UNIQUEID"                                                     //上次登陆唯一用户id
#define KEY_LOGIN_TYPE       "KEY_LOGIN_TYPE"                                                     //上次登陆使用方式
#define KEY_MUSIC_STATUS    "KEY_MUSIC_STATUS"                                                  //音乐状态
#define KEY_SFX_STATUS      "KEY_SFX_STATUS"                                                    //音效状态
#define KEY_VIDEO_SKIP      "KEY_VIDEO_SKIP"                                                    //是否可以跳过视频

#define KEY_SERVERID        "KEY_SERVERID"                                                      //最后登陆的服务器
#define KEY_TOKKEN          KEY_WITH_USER("KEY_TOKKEN")                                         //登陆tokken
#define KEY_TUTORIAL_SUBSTEP		KEY_WITH_USER_SERVER("KEY_TUTORIAL_SUBSTEP")


#define KEY_WITH_USER_SERVER(key)	\
    CCString::createWithFormat("%s_%s_%d",key,PlatformManager::sharedPlatformManager()->getUserId().c_str(),cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_SERVERID,-1))->getCString()

#define KEY_BATTLE_SPEEDUP          KEY_WITH_USER_SERVER("KEY_BATTLE_SPEEDUP")                         //战斗是否加速播放
#define KEY_VS_MATCH_WATCHED		KEY_WITH_USER_SERVER("KEY_VS_MATCH_WATCHED")
#define KEY_MATCH_WATCHED_TIME		KEY_WITH_USER_SERVER("KEY_MATCH_WATCHED_TIME")
#define KEY_MATCH_WATCHED			KEY_WITH_USER_SERVER("KEY_MATCH_WATCHED")

#define KEY_START_TIME_OF_DAY			KEY_WITH_USER_SERVER("KEY_START_TIME_OF_DAY")


/******************  Touch Priority *******************/
#pragma mark - Touch Priority -

#define DTOUCH_EFFECT_PRIORITY          -130
#define DTOUCH_TUTORIAL_PRIORITY        -120
#define DTOUCH_ALERT_PRIORITY           -900
#define DTOUCH_DISABLE_TOUCH_PRIORITY   -999

//为了先响应触摸事件关掉功能菜单弹出框。不会截取触摸事件。
#define DTOUCH_FOOTER_MENU_VIEW         -130


/******************  POPUP Priority *******************/
#define DPOPUP_RUSH_SETTLE_PRIORITY    -200
#define DPOPUP_USER_LEVELUP_PRIORITY    100
#define DPOPUP_WONDER_DROP_PRIORITY		-100

#define SHARE_KEY "-.-==yonggekaigefennengegegelianglizhegeshiwangzongshuode-.-@@~~"


/******************  CRYPT  *******************/
#ifdef CRYPT

#define SUFFIX          ".dream"

#define UI_PASSWORD     "Aks4e#$&%(1~$&df"
#define DATA_PASSWORD   "Vk3se#_&%}+#~^c1"
#define LUA_PASSWORD    "#ks4e#$&%]P~@d+c"

#else
#define SUFFIX  ""
#endif

/****************** Notification  *******************/

#define FIND_LOCAL_STRING	"find_local_string"
#define D_EVENT_ACTIVITY_RESET	"activity_reset"


static const char*  LOW_QUALITY_PREFIX = "_low_quality";


/****************** Config path for CSVDecoder::create  *******************/
static const char*    RESOURCE_CONFIG_PATH = "data/resource/resource";
static const char*    GAME_CONFIG_PATH = "data/game/game_config";
static const char*    VERSION_CONFIG_PATH = "platform/version.txt";
static const char*    ACTIVENESS_CONFIG_PATH = "data/activeness/activeBounsValue";
static const char*    ACTIVENESS_AWARD_CONFIG_PATH = "data/activeness/activeBounsraward";
static const char*    SOUL_POINT_CONFIG_PATH = "data/soul_exchange/soul_point";
static const char*    ANNOUNCE_CONFIG_PATH = "data/notice/announce";
static const char*    DRESS_CONFIG_PATH = "data/dress/dress";
static const char*    SCENE_INFO_CONFIG_PATH = "data/scene/scene_info";
static const char*    BATTLE_DIALOGUE_CONFIG_PATH = "data/dialog/battle_dialogue";
static const char*    UNIT_DIALOGUE_CONFIG_PATH = "data/dialog/unit_dialogue";
static const char*    BUFF_CONFIG_PATH = "data/skill/buff";
static const char*    DESTINY_CONFIG_PATH = "data/destiny/destiny";
static const char*    DESTINY_COLOR_CONFIG_PATH = "data/destiny/destiny_color";
static const char*    EQUIP_COLOR_CONFIG_PATH = "data/equip/equip_color";
static const char*    EQUIPS_CONFIG_PATH = "data/equip/equips";
static const char*    EQUIP_ENHANCE_COST_CONFIG_PATH = "data/equip/equip_enhance_cost";
static const char*    REFINE_ITEM_CONFIG_PATH = "data/equip_refine/refine_level";
static const char*    REFINE_EXP_CONFIG_PATH = "data/equip_refine/refine_exp";
static const char*    ERROR_TIPS_CONFIG_PATH = "data/errorcode/error_tips";
static const char*    FILTER_CONFIG_PATH = "data/filter";
static const char*    FORMATION_UNLOCK_CONFIG_PATH = "data/formation/formation_unlock";
static const char*    TEAM_DROP_CONFIG_PATH = "data/friend_dungeon/team_drop";
static const char*    TEAM_BATTLE_CONFIG_PATH = "data/friend_dungeon/team_battle";
static const char*    TEAM_CHAPTER_CONFIG_PATH = "data/friend_dungeon/team_chapter";
static const char*    GUILDBOSS_DAMAGE_POSITION_CONFIG_PATH = "data/guide/guildboss_damage_position";
static const char*    GUILDBOSS_ANGER_COLOR_CONFIG_PATH = "data/guide/guildboss_anger_color";
static const char*    GUILD_LEVEL_CONFIG_PATH = "data/guide/guild_level";
static const char*    GUILD_KNIGHTHOOD_CONFIG_PATH = "data/guide/guild_knighthood";
static const char*    TREE_CHIVALRY_LEVEL_CONFIG_PATH = "data/guide/tree_chivalry_level";
static const char*    TREE_OVERLORD_LEVEL_CONFIG_PATH = "data/guide/tree_overlord_level";
static const char*    GUILD_COINS_CONFIG_PATH = "data/guide/guild_coins";
static const char*    TREE_CHIVALRY_SKILL_CONFIG_PATH = "data/guide/tree_chivalry_skill";
static const char*    TREE_OVERLORD_SKILL_CONFIG_PATH = "data/guide/tree_overlord_skill";
static const char*    GUILD_QUESTION_CONFIG_PATH = "data/guide/guild_question";
static const char*    GUILD_MARQUEE_CONFIG_PATH = "data/guide/guild_marquee";
static const char*    ITEM_COLOR_CONFIG_PATH = "data/item/item_color";
static const char*    ITEMS_CONFIG_PATH = "data/item/items";
static const char*    VIP_BOX_CONFIG_PATH = "data/item/vip_box";
static const char*    ITEM_UNENOUGH_CONFIG_PATH = "data/errorcode/item_unenough";
static const char*    LOGINREWARD_CONFIG_PATH = "data/activity/loginReward";
static const char*    LOTTERY_CONFIG_PATH = "data/lottery/lottery";
static const char*    MAIL_FORMAT_CONFIG_PATH = "data/mail/mail_format";
static const char*    RED_PACKET_200_CONFIG_PATH = "data/mall/200bonus";
static const char*    PRODUCTS_CONFIG_PATH = "data/mall/products";
static const char*    MATCH_CONFIG_CONFIG_PATH = "data/cup/match_config";
static const char*    MATCH_TIME_CONFIG_PATH = "data/cup/match_time";
static const char*    NAVIGATION_CONFIG_PATH = "data/navigation/navigation";
static const char*    TIPS_INFO_CONFIG_PATH = "data/dialog/tips_info";
static const char*    NOTICE_TEMPLATE_CONFIG_PATH = "data/notice/notice_template";
static const char*    PVP_ROBOT_FORMATION_CONFIG_PATH = "data/pvp/pvp_robot_formation";
static const char*    SERVERS_CONFIG_PATH = "platform/servers";
static const char*    RECHARGE_CONFIG_PATH = "platform/recharge";
static const char*    CONFIG_CONFIG_PATH = "platform/config";
static const char*    PARAMETER_CONFIG_PATH = "platform/parameter";
static const char*    ANALYTICS_CONFIG_PATH = "platform/analytics";
static const char*    ADS_CONFIG_PATH = "platform/ads";
static const char*    PRIZE_CONFIG_PATH = "data/activity/prize";
static const char*    CHAPTER_CONFIG_PATH = "data/explore/chapter";
static const char*    MAP_CONFIG_PATH = "data/explore/map";
static const char*    PVE_ELITE_PATH = "data/elitemap/elitemap";
static const char*    PVE_FAILURE_CONFIG_PATH = "data/errorcode/pve_failure";
static const char*    MAP_DIALOGUE_CONFIG_PATH = "data/dialog/map_dialogue";
static const char*    CHAPTER_EXTRA_INFO_CONFIG_PATH = "data/explore/chapter_extra_info";
static const char*    PVP_AWARD_INFO_CONFIG_PATH = "data/pvp/pvp_award_info";
static const char*    GUIDE_CONFIG_PATH = "data/guide/guide";
static const char*    SHARE_CONFIG_PATH = "platform/share";
static const char*    SIGNIN_CONFIG_PATH = "data/signin/signin";
static const char*    SKILL_CONFIG_PATH = "data/skill/skill";
static const char*    SKILL_LV_EXP_CONFIG_PATH = "data/skill_lv/skill_lv_exp";
static const char*    SKILL_COLOR_CONFIG_PATH = "data/skill/skill_color";
static const char*    SKILL_ZONE_CONFIG_PATH = "data/skill/skill_zone";
static const char*    UNITACTION_CONFIG_PATH = "data/voice/unitAction";
static const char*    WARRIORVOICE_CONFIG_PATH = "data/warrior/warrior_talk";
static const char*    SKILLEFFECT_CONFIG_PATH = "data/skill/skillEffect";
static const char*    SKILL_NO_SOUND_CONFIG_PATH = "data/skill/skill_no_sound";
static const char*    LOCAL_VOICE_CONFIG_PATH = "data/voice/local_voice";
static const char*    ANIMATIONSOUND_CONFIG_PATH = "data/voice/animationSound";
static const char*    SKILL_KILL_CONFIG_PATH = "data/skill/skill_kill";
static const char*    TOWERCONTINUE_CONFIG_PATH = "data/tower/towercontinue";
static const char*    TUTORIAL_CONFIG_PATH = "data/rookie/tutorial";
static const char*    FIRST_WARRIOR_INFO_CONFIG_PATH = "data/warrior/first_warrior_info";
static const char*    TUTORIAL_CONTENT_CONFIG_PATH = "data/rookie/tutorial_content";
static const char*    USER_LV_EXP_CONFIG_PATH = "data/user/user_lv_exp";
static const char*    USER_UP_AWARD_CONFIG_PATH = "data/user/user_up_award";
static const char*    VIPS_CONFIG_PATH = "data/user/vips";
static const char*    FIRST_PUNCH_CONFIG_PATH = "data/FirstPunch/FirstPunch";
static const char*    PRIVILEGES_CONFIG_PATH = "data/privilege/privileges";
static const char*    WARRIOR_FATE_CONFIG_PATH = "data/warrior/warrior_fate";
static const char*    WARRIOR_HANDBOOK_CONFIG_PATH = "data/warrior/warrior_handbook";
static const char*    WARRIOR_BASE_INFO_CONFIG_PATH = "data/warrior/warrior_base_info";
static const char*    WARRIOR_LV_EXP_CONFIG_PATH = "data/warrior/warrior_lv_exp";
static const char*    WARRIOR_GRADE_INFO_CONFIG_PATH = "data/warrior/warrior_grade_info";
static const char*    WARRIOR_UP_SOUL_CONFIG_PATH = "data/warrior/warrior_up_soul";
static const char*    WARRIOR_DIALOG_CONFIG_PATH = "data/dialog/warrior_dialog";
static const char*    VIDEO_CONFIG_PATH = "data/video/video";
static const char*    WARRIOR_ICONS_CONFIG_PATH = "data/warrior/warrior_icons_info";
static const char*    WARRIOR_UPGRADE_ITEM_CONFIG_PATH = "data/warrior/warrior_strengthen";
static const char*    WARRIOR_UPGRADE_ACCUMULATIION_CONFIG_PATH = "data/warrior/warrior_Strenthenspeed";
static const char*    WARRIOR_UPGRADE_CRITICAL_CONFIG_PATH = "data/warrior/warrior_Critical";
static const char*    WARRIOR_POTENTIAL_FACTOR_CONFIG_PATH = "data/warrior/warrior_up_soul_qianlidian";
static const char*    ACHIEVEMENT_CONFIG_PATH = "data/task/task";
static const char*    ACHIEVEMENT_AWARD_PATH = "data/task/score";
static const char*    ARENA_CONFIG_PATH = "data/pvp/arena_reward";
static const char*    TALENT_CONFIG_PATH = "data/talent/talent";
static const char*    WARRIOR_TALENT_CONFIG_PATH = "data/warrior/warriortalent";
static const char*    BURN_CONFIG_PATH = "data/warrior/warrior_lianhua";
static const char*    ONLINEAWARD_CONFIG_PATH = "data/activity/timeprize";
static const char*    BURN_EXP_CONFIG_PATH = "data/equip_decompose/decompose";
static const char*    PVP_MALL_LVL_UP_CONFIG_PATH = "data/pvp/shop_lv";
static const char*    WELCOME_CONFIG_PATH = "data/welcome/welcome";
static const char*    TREASURE_INIT_PATH = "data/treasure_level/BaseEatExp";
static const char*    TREASURE_LEVEL_PATH = "data/treasure_level/LevelUpExp";
static const char*    TREASURE_RESIST_PATH = "data/treasure_level/LevelUpGrow";
static const char*    CHAT_FORMAT_CONFIG_PATH = "data/sys_msg/sys_msg";
#endif //QSMY_AppConsts_h
