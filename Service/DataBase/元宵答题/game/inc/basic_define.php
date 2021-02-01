<?php
/**
 * Created by JetBrains PhpStorm.
 * User: pengya
 * Date: 13-3-30
 * Time: 下午3:58
 * To change this template use File | Settings | File Templates.
 */
define('TIME_NOW', $_SERVER['REQUEST_TIME']);
define('TIME_TODAY', strtotime('today', TIME_NOW));

//-------------平台配置-------------
define('PLAT_SELF', 1); //机器码
define('PLAT_PP', 3); //PP平台
define('PLAT_WEGAME', 25); //WEGAME平台

//-------------金额配置-------------
define('POINT_PER_MONEY_UNIT_ZH', 10);
define('POINT_PER_MONEY_UNIT_TW', 1.5);

//-------------平台配置-------------

/*机关兽*/
define('GUNDAM_USER_LV',18);

/*弟子*/
define('NORMAL_INHER_ITEM', 9010);
define('HIGH_INHER_ITEM', 9009);
define('WARRIOR_MAX_LEVEL', 180);
define('SKILL_MAX_LEVEL', 10);
/*单位--end*/

/*弟子*/
define('DEVELOP_MIN_POINT', 4);
define('DEVELOP_MAX_POINT', 6);
/*弟子 ---end*/

/*千机楼*/
define('MAX_ROUND', 1);
define('TOWER_LIMIT_LV', 13); //正常限制是13级
define('CONTINUE_LIMIT', 10); //continue最大值

/*千机楼--end*/
/*玩家*/
define('MAX_POWER', 10);
define('MAX_ENDURANCE', 10);
define('SECONDS_PER_POWER', 360);
define('SECONDS_PER_ENDU', 900);
define('EAT_CHICKEN', 20);
define('USER_MAX_LEVEL', 60);
/*玩家--end*/


/*LOG OPERATE START*/
define('LOG_OP_ADD_ITEM_START', 1001); //添加道具
define('LOG_OP_DEL_ITEM_START', 1003); //删除道具
define('LOG_OP_ITEM_USE', 1005); //开宝箱


define('LOG_OP_ADD_EQUIP_START', 1101); //添加装备
define('LOG_OP_UPD_EQUIP_START', 1103); //更新装备
define('LOG_OP_DEL_EQUIP_START', 1105); //删除装备
//define('LOG_OP_ENHANCE_EQUIP_START', 1107); //强化装备
//define('LOG_OP_EQUIP_SELL',1109);//装备售出

define('LOG_OP_ACTIVENESS_REWARD_START', 1201); //活跃度领奖开始
define('LOG_OP_ACTIVENESS_SAVE', 1203); //活跃度保存

define('LOG_OP_CHALLENGE', 1301); //挑战结束
define('LOG_OP_CHALLENGE_REWARD_START', 1302); //论剑领奖开始
define('LOG_OP_CHALLENGE_UPDATE', 1304); //论剑保存

define('LOG_OP_CUP_SIGN_UP', 1401); //杯赛报名
define('LOG_OP_CUP_BET_SAVE', 1403); //保存杯赛竞猜
define('LOG_OP_CUP_MATCH_REWARD_START', 1406); //杯赛比赛奖励领奖开始
define('LOG_OP_CUP_MATCH_SAVE', 1408); //保存比赛

define('LOG_OP_SAVE_DESTINY_START', 1501); //保存天命开始
define('LOG_OP_IMPROVE_DESTINY_START', 1503); //提升天命开始
define('LOG_OP_DESTINY_REWARD_START', 1507); //天命领奖开始

define('LOG_OP_PAY_POINT', 1601); //点券消费
//弟子相关
define('LOG_OP_WARRIOR_INHERIT', 1701); //弟子传功
define('LOG_OP_WARRIOR_DEVELOP', 1702); //弟子培养
define('LOG_OP_WARRIOR_DEVELOP_OK', 1703); //弟子培养确认
define('LOG_OP_WARRIOR_DEVELOP_CANCEL', 1704); //弟子培养取消
define('LOG_OP_WARRIOR_UP', 1705); //弟子突破
define('LOG_OP_WARRIOR_ADD_START', 1706); //添加弟子开始

define('LOG_OP_WARRIOR_DELETE_START', 1708); //删除弟子
//define('LOG_OP_WARRIOR_ADD_EXP',1710);//经验增加
define('LOG_OP_WARRIOR_ADD_SKILL_EXP', 1712); //技能经验增加
define('LOG_OP_WARRIOR_DECOMPOSE_START', 1714); //弟子分解
define('LOG_OP_WARRIOR_RECRUIT', 1716); //弟子招募
define('LOG_OP_WARRIOR_UPDATE_START', 1717); //弟子更新开始

//用户相关
define('LOG_OP_USER_LOGIN', 1801); //用户登录
define('LOG_OP_USER_REGISTER', 1802); //用户注册
define('LOG_OP_USER_POWER_ADD', 1803); //用户行动力增加
define('LOG_OP_USER_ADD_COIN', 1811); //增加金币
define('LOG_OP_USER_DEDUCT_COIN', 1812); //减少金币
define('LOG_OP_USER_ADD_BONUS', 1813); //增加礼券
define('LOG_OP_USER_GET_VIP', 1814); //获得vip等级
define('LOG_OP_USER_ADD_POINT', 1815); //获得点券
define('LOG_OP_BIND_ACCOUNT', 1816); //绑定账号
define('LOG_OP_USER_ENDURANCE_ADD', 1817); //用户耐力增加
define('LOG_OP_USER_CHALLENGE_POINT_ADD', 1818); //用户竞技场积分增加
//activity
define('LOG_OP_ACTIVITY_EAT', 1901); //吃鸡
define('LOG_OP_ACTIVITY_LOGIN_AWARD', 1902); //领取登录奖励
define('LOG_OP_ACTIVITY_SIGN_IN', 1903); //签到
define('LOG_OP_ACTIVITY_LV_UP_AWARD', 1904); //领取升级奖励
define('LOG_OP_ACTIVITY_TOTAL_LOGIN_AWARD', 1905); //领取累计登录奖励
define('LOG_OP_ACTIVITY_MONTHCARD_TOTALCHARGE_AWARD',1906);//领取月卡累计充值奖励
define('LOG_OP_ACTIVITY_SPRINT_LEVEL_AWARD',1907);//领取冲级奖励
define('LOG_OP_ACTIVITY_SPECIAL_SIGN_IN_AWARD',1908);//领取特殊签到奖励
define('LOG_OP_ACTIVITY_GROWTH_FUND_AWARD',1909);//领取成长基金奖励
define('LOG_OP_ACTIVITY_PILI_MOVIE_AWARD',1910);//领取奇人密码奖励
define('LOG_OP_ACTIVITY_COLLECT_LOTTERY_DRAW',1911);//抽取收集抽奖道具-新春快乐
define('LOG_OP_ACTIVITY_FEED_WEALTH_GOD',1912);//喂财神-元宵节活动
//集市
define('LOG_OP_CARD_DRAW', 2001); //集市抽卡
define('LOG_OP_LIMIT_TIME_DRAW_CARD', 2002); //限时英雄抽卡

define('LOG_OP_MALL_BUY', 2201); //商城购买

define('LOG_OP_WONDER_DEL_START', 2305); //删除奇遇开始
define('LOG_OP_WONDER_ADD_POINT', 2306); //添加奇遇积分

define('LOG_OP_SOUL_ADD_START', 2401); //新增魂魄开始
define('LOG_OP_SOUL_DELETE_START', 2403); //删除魂魄开始

define('LOG_OP_CHAPTER_GET_AWARD', 2505); //领取章节礼物
define('LOG_OP_CHAPTER_UPDATE_START', 2506); //章节更新开始

define('LOG_OP_CHARGE_START', 2601); //充值开始

define('LOG_OP_ADD_DRESS', 2701);   //添加时装
define('LOG_OP_UPDATE_DRESS', 2702);   //更新时装
define('LOG_OP_DEL_DRESS', 2703);   //删除时装

define('LOG_OP_SOUL_EXCHANGE_ADD_POINT', 2801); //添加灵元
define('LOG_OP_SOUL_EXCHANGE_DEDUCT_POINT', 2802); //扣除灵元
define('LOG_OP_SOUL_EXCHANGE_EXCHANGE', 2803); //兑换魂魄

define('LOG_OP_MAIL_GET_ATTACHMENT',2900);//领取邮件附件

define('LOG_OP_REFINING', 2901); //炼化
define('LOG_OP_REFINING_REBORN', 2902); //重生

define('LOG_OP_EXCHANGE_REFRESH', 3001);//禁地寻宝刷新
define('LOG_OP_EXCHANGE_EXCHANGE', 3002);//禁地寻宝兑换
define('LOG_OP_EXCHANGE_CONTR_REWARD', 3003);//禁地寻宝领取贡献奖励
define('LOG_OP_EXCHANGE_GLOBAL_REWARD', 3004);//禁地寻宝领取全服奖励

define('LOG_OP_EX_CODE_EXCHANGE', 4001);//兑换码兑换

define('LOG_OP_LR_PRACTISE', 4101);//主角修行

define('LOG_OP_WISH', 4201);//许愿

define('LOG_OP_FRIEND_DUNGEON_GO', 4301); //好友副本出战
define('LOG_OP_FRIEND_DUNGEON_RUSH', 4302); //好友副本扫荡
define('LOG_OP_FRIEND_DUNGEON_RESET', 4303); //好友副本重置
define('LOG_OP_FRIEND_DUNGEON_REWARD',4304);//好友副本奖励
define('LOG_OP_HUNT_ATTACK', 4401); //狩魂-攻击
define('LOG_OP_HUNT_ATTACK_NPC', 4402); //狩魂-攻击NPC

define('LOG_OP_GUILD_SIGN_IN',4501);//公会签到
define('LOG_OP_GUILD_KING_TREE',4502);//捐献王道树
define('LOG_OP_GUILD_KNIGHT_TREE',4503);//捐献侠道树
define('LOG_OP_GUILD_ANSWER_QUESTION',4504);//捐献侠道树

define('LOG_OP_GUILD_BOSS_RESCUE',4601);//公会战救助
define('LOG_OP_GUILD_BOSS_PAID_ATTACK_BOSS',4602);//公会战付费弑龙
define('LOG_OP_GUILD_BOSS_PAID_ROB',4603);//公会战付费纷争
define('LOG_OP_FRIEND_GET_GIFT',4701);//好友收礼

define('LOG_OP_ADD_ALCHEMYPILL',4801);//添加内丹
define('LOG_OP_UPDATE_ALCHEMYPILL',4802);//更新内丹
define('LOG_OP_DEL_ALCHEMYPILL',4803);//删除内丹
define('LOG_OP_STRENGTHEN_ALCHEMYPILL',4804);//强化内丹
define('LOG_OP_DECOMPOSE_ALCHEMYPILL',4805);//分解内丹
define('LOG_OP_ADD_CHI',4806);//增加真气
define('LOG_OP_DEDUCT_CHI',4807);//消耗真气

define('LOG_OP_WEEKLY_MIRAGE',4901);//蜃楼东渡
//墨攻棋阵
define('LOG_OP_WEEKLY_CHESS_POWER_ADD',5001);//墨攻棋阵购买体力
define('LOG_OP_WEEKLY_CHESS_POINT_ADD',5002);//墨攻棋阵添加积分
define('LOG_OP_WEEKLY_CHESS_OPEN_BOARD',5003);//墨攻棋阵开启棋盘
define('LOG_OP_WEEKLY_CHESS_CHANGE_CHECKER',5004);//墨攻棋阵改变棋格
define('LOG_OP_WEEKLY_CHESS_PICKUP_POINTS',5005);//墨攻棋阵领取积分奖励
define('LOG_OP_WEEKLY_CHESS_PHASE_REWARD',5006);//墨攻棋阵领取全服积分奖励
define('LOG_OP_WEEKLY_CHESS_ACHIEVE_REWARD',5007);//墨攻棋阵领取成就奖励
define('LOG_OP_WEEKLY_CHESS_ADD_KEYS',5008);//墨攻棋阵获取钥匙
define('LOG_OP_WEEKLY_CHESS_DEL_KEYS',5009);//墨攻棋阵消耗钥匙
define('LOG_OP_WEEKLY_CHESS_OCCUPY_CHECKER',5010);//墨攻棋阵占领棋格
define('LOG_OP_WEEKLY_CHESS_UNLOCK_CHECKER',5011);//墨攻棋阵占领棋格

define('LOG_OP_GUILD_CHARGE_DRAW',5101);//公会充值领取奖励
define('LOG_OP_GUILD_CHARGE',5102);//帮会充值

define('LOG_OP_GUILD_MALL_DEDUCT_CON_VALUE',5201);//帮会商城扣除帮贡
define('LOG_OP_GUILD_MALL_EXCHANGE_ITEM',5202);//帮会商城获得物品

define('LOG_OP_STUDY_DRAW_REWARDS',5301);//求学领取奖励
define('LOG_OP_STUDY_LOCK_GRID',5302);//求学多顶格子

define('LOG_OP_ADD_GUNDAM',5401);//添加机关兽
define('LOG_OP_DEL_GUNDAM',5402);//删除机关兽
define('LOG_OP_UPDATE_GUNDAM',5403);//更新机关兽
define('LOG_OP_GUNDAM_REFINING',5404);//机关兽进化
define('LOG_OP_GUNDAM_STRENGTHEN',5405);//机关兽强化
define('LOG_OP_GUNDAM_DECOMPOSE',5406);//机关兽分解
define('LOG_OP_GUNDAM_COMPOSE',5407);//机关兽合成
define('LOG_OP_GUNDAM_AUTO_STRENGTHEN',5408);//机关兽自动强化

define('LOG_OP_TURN_TABLE_TICKET',5501);//抽奖获得票
define('LOG_OP_TURN_TABLE_REWARD',5502);//抽奖获得物品

define('LOG_OP_GUNDAM_PIECE_ADD', 5601); //新增机关兽碎片开始
define('LOG_OP_GUNDAM_PIECE_DELETE', 5602); //删除机关兽碎片开始

define('LOG_OP_DRAGON_VALLEY_SPEEDUP', 5701); //皇陵寻宝加速
define('LOG_OP_DRAGON_VALLEY_SEARCH', 5702); //皇陵寻宝搜索
define('LOG_OP_DRAGON_VALLEY_FINISH', 5703); //皇陵寻宝领取奖励
define('LOG_OP_DRAGON_VALLEY_BONUS', 5704); //皇陵寻宝领取助战奖励
define('LOG_OP_DRAGON_VALLEY_ALL_BONUS', 5705); //皇陵寻宝领取所有助战奖励
define('LOG_OP_DRAGON_VALLEY_BATTLE', 5706); //皇陵寻宝战斗

define('LOG_OP_BLACK_MARKET_BUY_COIN',5801);//黑市购买商品
define('LOG_OP_BLACK_MARKET_DRAW',5802);//黑市领取全服奖励
define('LOG_OP_BLACK_MARKET_DRAW_COIN',5803);//黑市领取全服奖励银币

define('LOG_OP_CHARGE_ALTER_REWARD',5901);//充值有礼(选择奖励)领取物品

//楼兰探秘
define('LOG_OP_WEEKLY_LOULAN_POWER_ADD',6001);//楼兰探秘购买体力
define('LOG_OP_WEEKLY_LOULAN_POINT_ADD',6002);//楼兰探秘添加积分
define('LOG_OP_WEEKLY_LOULAN_PHASE_REWARD',6003);//楼兰全服奖励
define('LOG_OP_WEEKLY_LOULAN_ACHIEVE_REWARD',6004);//楼兰成就奖励

define('LOG_OP_CACHE_RES', 9001); //处理缓存结果
define('LOG_OP_FATAL_ERROR', 9002); //致命错误
define('LOG_OP_SYS_ERROR', 9003); //系统错误
define('LOG_OP_EVENT_HANDLE_ERROR', 9004); //事件处理错误

/*LOG OPERATE END*/

/*MOD*/
define('SYS_PVE', 1); //探索
define('SYS_WONDER', 2); //奇遇
define('SYS_TOWER', 3); //千层塔
define('SYS_CHALLENGE', 4); //论剑
define('SYS_SIGN_IN', 5); //签到
define('SYS_DRAW_CARD', 6); //抽卡
define('SYS_CUP', 7); //杯赛
define('SYS_WISH', 8); //许愿
define('SYS_EQUIP_ENHANCE', 9); //强化
define('SYS_WARRIOR_TRAIN', 10); //培养
define('SYS_EAT', 11); //吃鸡
define('SYS_ACTIVENESS', 12); //活跃度
define('SYS_DESTINY', 13); //天命
define('SYS_LOGIN', 14); //登录
define('SYS_EQUIP_SELL', 15); //出售
define('SYS_OPEN_BOX', 16); //开宝箱
define('SYS_MALL', 17); //商城
define('SYS_MAIL', 18); //邮件
define('SYS_CHARGE', 19); //充值奖励
define('SYS_LV_UP_AWARD', 20); //升级奖励
define('SYS_TOTAL_LOGIN_AWARD', 21); //累计登录奖励
define('SYS_MAMMON', 22); //迎财神
define('SYS_EXCHANGE_MALL', 23); //兑换商店
define('SYS_VIP_BOX', 24); //VIP礼包
define('SYS_TOTAL_CHARGE_AWARD', 25); //累计充值奖励
define('SYS_INVITE_AWARD', 26); //邀请奖励
define('SYS_EX_CODE', 27); //兑换码
define('SYS_MOBILE', 28); //手机验证
define('SYS_GUILD', 29); //公会
define('SYS_HUNT', 30); //狩魂
define('SYS_MONTH_CARD',31);//月卡
define('SYS_GUILD_BOSS',32);//公会战
define('SYS_RIDDLE',33);//灯谜
define('SYS_MONTHCARD_TOTAL_CHARGE_AWARD',34);//月卡累充奖励
define('SYS_LR_PRACTISE', 35); //主角修行
define('SYS_SOUL_EXCHANGE', 36); //魂魄兑换
define('SYS_EQUIP_FRAGMENT', 37); //装备碎片
define('SYS_EQUIP_DECOMPOSE',38);//装备分解
define('SYS_FRIEND_DUNGEON', 39); //好友副本
define('SYS_FRIEND',40);//好友系统
define('SYS_REQUEST_SHARE', 41); //邀请好友,分享（原分享，该功能包含分享）
define('SYS_CHAPTER',42);//章节
define('SYS_CHALLENGE_MALL',43);//竞技场商城
define('SYS_WARRIOR_ENHANCE', 44); //英雄强化
define('SYS_TASK', 45); //成就系统
define('SYS_REFINING', 46); //炼化系统
define('SYS_WARRIOR_BREAK', 47); //英雄进阶
define('SYS_LIMIT_TIME_DRAW_CARD', 48); //限时抽卡
define('SYS_SPRINT_LEVEL_AWARD', 49); //冲级奖励
define('SYS_ONLINE_GIFT', 50); //在线礼包
define('SYS_KF_CHARGE_RETURN', 51); //开服充值返回
define('SYS_MONTH_CARD_MALL', 52); //月卡商城
define('SYS_SPECIAL_SIGN_IN', 53); //特殊签到（圣诞签到等）
define('SYS_REFINING_REBORN', 54); //炼化重生
define('SYS_CALENDAR', 55); //月签到
define('SYS_DRAGON_VALLEY_SEARCH',56);//寻龙山谷宝库搜索
define('SYS_DRAGON_VALLEY_FINISH',57);//寻龙山谷宝库结算
define('SYS_GUNDAM',58);//机关兽系统
define('SYS_DRAGON_VALLEY_ATTACK',59);//寻龙山谷攻击结算
define('SYS_DRAGON_VALLEY_ASSIST',60);//寻龙山谷助战结算
define('SYS_DRAGON_VALLEY_DEFEND',61);//寻龙山谷防守失败结算
define('SYS_GUNDAM_COMPOSE',62);//机关兽合成
define('SYS_BLACK_MARKET',63);//黑市商城
define('SYS_TOWER_POINT',64);//千机楼积分兑换
define('SYS_GUNDAM_DECOMPOSE',65);//机关兽分解
define('SYS_CHARGE_ALTER_REWARD',66);//充值有礼(选择)
define('SYS_CONSUME_RETURN',67);//消费返现
define('SYS_OLD_PLAYER',68);//老玩家回归
define('SYS_OLD_PLAYER_INVITE',69);//老玩家回归邀请
define('SYS_ONETIME_REWARD', 70); //一次性奖励
define('SYS_LOULAN',71);//楼兰
define('SYS_CROSS_CUP', 72); //跨服杯赛
define('SYS_GROWTH_FUND', 73); //成长基金
define('SYS_PILI_MOVIE', 74); //奇人密码
define('SYS_COLLECT_LOTTERY_DRAW', 75); //收集抽奖-新春快乐
define('SYS_NEW_YEAR_CHARGE', 76); //新春储值
define('SYS_FEED_WEALTH_GOD', 77); //喂财神-元旦活动
define('SYS_CONSUME_LOTTERY', 78); //消费抽奖
define('SYS_KILL_LOVERS', 79); //挑战情侣-甜蜜挑战-情人节活动
define('SYS_KLUCKY_POCKET', 80); //福袋-新春福袋
define('SYS_ANSWER', 81); //答题活动-元宵答题

define('SYS_DEFAULT',999);//默认
/*MOD END*/

define('MAX_INT32', 2147483647);

/*language*/
define('LOWEST_VERSION', '1.0.7'); //最低版本
define('LANG_SYS_USER', '系统');
define('LANG_CHARGE_RETURN', '充值返现');
define('LANG_TOTAL_CHARGE_GIFT', '累计充值奖励');