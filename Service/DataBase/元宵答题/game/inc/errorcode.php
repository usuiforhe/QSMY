<?php
//操作成功
define('OPT_SUCCESS', '0000');

/*-- 掉落 --*/
define('DROP_NOT_EXISTS', '1001');
define('LIST_NOT_EXISTS', '1002');
define('ITEM_NOT_EXISTS', '1003');
/*--掉落end--*/
/*PVE*/
define('POWER_NOT_ENOUGH','1101');
define('ALREADY_GET_CHAPTER_AWARD','1102');
define('PRES_NOT_ENOUGH','1103');
define('MAP_NOT_UNLOCKED', '1104');//关卡未解锁
define('EXPLORE_REACH_MAX', '1105'); //探索次数达到上限
define('MAP_CANNOT_RUSH', '1106'); //关卡不能扫荡
define('EXPLORE_NOT_REACH_MAX', '1107'); //关卡探索次数未达到上限
define('RUSH_IN_CD', '1108'); //关卡扫荡在CD中
define('RUSH_NOT_IN_CD', '1109'); //关卡扫荡不在CD中
define('NOT_ALL_FULL_GRADE','1110');//该章节还没有全三星
define('ALREADY_GET_FULL_GRADE_AWARD','1111');//该章节已经领取三星奖励
define('CHAPTER_NOT_PASSED','1112');//该章节还未通关
define('ALREADY_GET_VIDEO_AWARD','1113');//该章节视频奖励已经领取

//用户
define('WRONG_TIME_TO_EAT','1201');//没到吃鸡时间
define('NAME_LENGTH_ERROR', '1202');
define('USER_ALREADY_EXISTS', '1203');
define('USER_NOT_EXISTS', '1204');
define('YOU_ALREADY_EAT','1205');
define('POWER_ALREADY_MAX','1206');
define('USER_NAME_EXISTS', '1207');
define('USER_ALREADY_FROZEN','1208');//用户已经被冻结
define('USER_NAME_FORBIDDEN','1209');//该用户名禁用
define('BATTLE_CD', '1210');        //战斗CD中
define('ENDURANCE_NOT_ENOUGH','1211');
define('USER_POINT_NOT_ENOUGH','1212');//元宝不足

//WARRIOR 弟子
define('WARRIOR_INFO_ERROR', '1301');
define('WARRIOR_LV_LOW', '1302');
define('WARRIOR_LV_ERROR', '1303');
define('SOUL_INFO_ERROR', '1304');
define('WARRIOR_EXIST', '1305');
define('SOUL_NOT_ENOUGH', '1306');
define('WARRIOR_NOT_EXISTS','1307');
define('POTENTIAL_NOT_ENOUGH','1308');
define('WARRIOR_CAN_NOT_BE_UP','1309');
define('WARRIOR_UP_ALREADY_TOP_LV','1310');
define('WARRIOR_LV_TOO_HIGH','1311');//弟子等级已经太高
define('INVALID_SKILL_EXP','1312');//弟子技能经验有误
//ITEM PACKAGE
define('NOT_ENOUGH_ITEM', '1401');
define('TRIGGER_NOT_ENOUGH','1402');//需要配合的玩意儿不够
define('CAN_NOT_EAT_MORE','1403');//今天吃烤鸡次数到上限
define('ITEM_NOT_AVAILABLE','1404');//道具未开放使用
define('RECRUIT_NOT_ENOUGH_ITEM','1405');//招募令不足
define('BREAK_NOT_ENOUGH_ITEM','1406');//进阶丹不足
define('ITEM_NOT_TO_USE_TIME','1407');//未到使用时间
//FORMATION
define('POS_NOT_OPEN', '1501');
define('POS_ALREADY_OPEN','1502');
define('POS_ERROR','1503');
define('WARRIOR_ALREADY_IN_FORMATION', '1504');
define('ALREADY_LEADER','1505');//已经是队长了
define('WARRIOR_NOT_IN_POS','1506');//此位置没有弟子
//WONDER奇遇
define('POINT_NOT_ENOUGH','1601');
define('WONDER_ALREADY_DONE','1602');
define('NO_SUCH_WONDER','1603');
define('WONDER_TIME_LIMIT','1604');//奇遇时间还没开始
define('WONDER_BOSS_NO_KILL','1605');//BOSS还活的好好的
define('WONDER_BOSS_ALREADY_AWARD','1606');//已经领取恶鬼罗刹奖励
define('WONDER_BOSS_ALREADY_HELP','1607');//已经求助过
define('WONDER_BOSS_ALREADY_KILL','1608');//恶鬼已经被干死啦，还要鞭尸啊
define('WONDER_POINT_NOT_ENOUGH','1609');//奇遇积分不足
define('WONDER_POINT_ALREADY_GIVE_REWARD','1610');//奇遇积分奖励已领取

//Tower 千机楼
define('SCORES_NOT_ENOUGH','1701');//能量值不够，无法领取buff
define('ROUND_OVER','1702');//当天次数已经用尽
define('CONTINUE_TIMES_LIMIT','1703');//continue次数已经用尽
define('COINS_NOT_ENOUGH','1704');//continue所需金币不够
define('VIP_NOT_ENOUGH','1705');
define('THE_ROUND_ENDED','1706');//本轮已结束
define('TOWER_OUT_OF_DATE','1707');
define('TOWER_POINTS_NOT_ENOUGH','1708');//积分不够兑换
//Activity 活动
define('ALREADY_GET_LOGIN_GIFT','1801');
define('NOT_ENOUGH_LOGIN_TIMES','1802');
define('TODAY_ALREADY_SIGN_IN','1803');
define('MAMMON_TIMES_LIMIT','1804');//迎财神次数到上限
define('ACTIVITY_NOT_AVAILABLE', '1805');//活动不可用
define('ALREADY_GET_TOTAL_CHARGE_GIFT','1806');//已经领取过该累计充值奖励
define('NOT_ENOUGH_CHARGE_AMOUNT','1807');//累计充值金额未达到领取奖励的要求
define('ALREADY_GET_MONTHCARD_CHARGE_GIFT','1808');//该月卡充值金额奖励已经被领取
define('NOT_MONTH_CARD_USER','1809');//没有资格领取该奖励
define('TODAY_SHARE_GIFT_LIMIT','1810');//今天分享奖励已经领完
define('ALREADY_GET_SPRINT_LEVEL_GIFT','1811');//已领取过该等级的奖励
define('ONLINE_GIFT_NOT_IN_TIME','1812');//在线礼包时间未到
define('TODAY_SHARE_NOT_TO_TIME','1813');//分享时间间隔未到
define('MONTH_CARD_NO_BY_TIMES','1814');//【月卡商城】该道具无购买次数
define('USER_ALREADY_REQUEST','1815');//用户已邀请
define('USER_REQUEST_ALREADY_GIVE_REWARD','1816');//用户邀请奖励已领取
define('SPECIAL_SIGN_IN_ALREADY_GIVE_REWARD','1817');//签到奖励已领取
define('SPECIAL_SIGN_IN_NOT_GIVE_REWARD','1818');//签到奖励资格不符合不可获取
define('TODAY_SHARE_ALREADY_GIVE','1819');//分享奖励已领取
define('CALENDAR_ALREADY_SIGNED', '1820'); //已签到
define('CALENDAR_MAX_RE_SIGN_TIMES', '1821'); //签到达到最大次数
define('CALENDAR_EXPIRED', '1822'); //签到日期已过
define('CALENDAR_FUTURE', '1823'); //签到日期未到
define('ALREADY_GET_GIFT','1824');//已领取过该奖励
define('PILI_MOVIE_NOT_TO_OPEN_TIME','1825');//开启奇人密码宝箱时间未到
define('NOT_REACH_TASK','1826');//未达成任务目标
define('TODAY_NUMBER_OF_FULL','1827');//今日次数已满
define('KILL_LOVERS_TIMES_FINISH','1828');//甜蜜挑战次数用完
define('ANSWER_NOT_GET_AWARD','1829');//领取答题奖励失败
define('ANSWER_IS_NOT_IN_DATE','1830');//不在答题时间内
define('ANSWER_ALREADY_END','1831');//已经答题完毕
define('ANSWER_ALREADY_GET_AWARD','1832');//答题奖励已领取

//活跃度
define('ALREADY_GET_ACTIVENESS_REWARD', '1901');        //活跃奖励已领取
define('NOT_ENOUGH_ACTIVENESS_POINT', '1902');          //活跃点数不足

//杯赛
define('CUP_ALREADY_SIGN_UP', '2101');
define('CUP_BET_MAX', '2102');          //杯赛竞猜次数达到上限
define('CUP_ALREADY_BET_MATCH', '2103');      //已竞猜该比赛
define('CUP_MATCH_NOT_EXIST', '2104');  //杯赛比赛不存在
define('CUP_BET_PRIZE_NOT_AVL', '2105');    //杯赛竞猜奖励不可用
define('CUP_MATCH_PRIZE_DRAWN', '2106');    //比赛奖励已领取
define('CUP_MATCH_NO_RESULT', '2107');  //杯赛比赛暂无结果
define('CUP_MATCH_RESULT_PUBLIC', '2108');  //杯赛比赛结果已公布
define('CUP_CANNOT_SIGN_UP', '2109');// 当前不能报名杯赛

//论剑
define('CHALLENGE_RANK_CHANGED', '2201');   //目标名次已发生变化
define('NOT_ENOUGH_CH_POINT', '2202');   //论剑积分不足
define('NOT_QUALIFIED_CH_BOSS', '2203');   //没有挑战boss的资格
define('NOT_ENOUGH_CH_TIMES', '2204');   //挑战次数不足
define('NOT_ENOUGH_BOSS_TIMES', '2205');   //挑战boss的机会已用完
define('NOT_QUALIFIED_TO_CH', '2206');   //没有资格挑战该玩家
define('CHALLENGE_REWARD_NOT_AVL', '2207');   //奖励不可用
define('TARGET_IN_CHALLENGE', '2208');  //目标正在被挑战中
define('BEING_CHALLENGED', '2209');  //正在被挑战中
define('NOT_ENOUGH_ENDURANCE', '2210');   //耐力不足
define('CHALLENGE_NO_SUCH_EXCHANGE_ITEM', '2211'); //兑换项不存在
define('CHALLENGE_ITEM_EXCHANGEED', '2212'); //兑换项已兑换
define('CHALLENGE_ITEM_NO_STORE', '2213'); //兑换项无库存
define('CHALLENGE_REQUIRE_USER_NAME', '2214'); //论剑需要角色名，你还没有角色名

//天命
define('DESTINY_SOUL_NOT_SATISFIED', '2301');   //魂魄不符合需求
define('DESTINY_REWARD_NOT_AVL', '2302');   //奖励不可用
define('DESTINY_MAX_LEVEL', '2303');   //天命等级达到上限
define('DESTINY_NOT_EXIST', '2304');    //天命不存在
define('DESTINY_GET_REWARD_FIRST', '2305');    //请先领取天命奖励

//装备
define('ALREADY_HAS_EQUIP', '2401');
define('NO_SUCH_EQUIP', '2402');
define('USER_lV_LOW','2403');//玩家等级不够
define('TREASURE_CANT_BE_STRENGHTEN','2404');//典籍不能被强化
define('EQUIP_IN_FORMATION','2405');//装备在玩家身上
define('EQUIP_ALREADY_MAX_ENHANCE_LV','2406');//装备已经强化到最大级别
define('EQUIP_TREASURE_ALREADY_MAX_ENHANCE_LV','2407');//宝物装备已经强化到最大级别
define('EQUIP_IS_NOT_TREASURE','2408');//装备不是一个宝物

//mall
define('PRODUCT_NOT_AVAILABLE','2501');//商品未开放
define('NO_SUCH_PRODUCT','2502');
define('NOT_ENOUGH_BUY_POW_TIMES','2503');//购买体力次数不够
define('ALREADY_BUY_PRODUCT','2504');//道具已购买
define('NOT_BUY_PRODUCT','2505');//尚未购买该道具
//skill
define('ITEM_NOT_ENOUGH','2601');//技能道具不够
define('SKILL_LV_TOP','2602');//技能到了顶级

//邮件mail
define('NO_ATTACHMENT','2701');//没有附件
define('ATTACH_INFO_ERROR','2702');//附件信息错误
define('MAIL_NOT_EXISTS','2703');//邮件不存在
define('THE_MAIL_CAN_NOT_BE_DELETED','2704');//这种邮件不能被删

//许愿
define('INVALID_WISH_NUM', '2801'); //许愿弟子数量错误
define('NOT_ENOUGH_LANTERNS', '2802'); //天灯不足

//兑换商店
define('NO_SUCH_EXCHANGE_ITEM', '2901'); //兑换的项目不存在
define('EXCHANGE_REFRESHED', '2902'); //兑换项已刷新
define('NOT_ENOUGH_EXCHANGE_CONTRIBUTE', '2903'); //兑换商城贡献值不足
define('EQUIP_NOT_SATISFIED', '2904'); //兑换的装备不满足需求
define('GLOBAL_TARGET_NOT_SATISFIED', '2905'); //兑换商城未达到总贡献值目标
define('EXCHANGE_GLOBAL_REWARD_DRAWN', '2906'); //兑换商城全局奖励已领取
define('NO_SUCH_EXCHANGE_REWARD', '2907'); //兑换商城奖励不存在
define('ALREADY_EXCHANGED','2908');//已经兑换

//新手引导
define('ROOKIE_STEP_INVALID', '3001');  //该步骤暂时不能完成
define('ROOKIE_STEP_FINISHED', '3002');  //该步骤已完成
//邀请码
define('CAN_NOT_USE_YOURS','3101');// 不能用自己的哦
define('NOT_ENOUGH_INVITEES','3102');//没有邀请那么多人哦
define('ALREADY_GET_AWARD','3103');//已经领过奖了
define('NOT_BE_INVITED','3104');//还没有被邀请
define('NOT_RIGHT_INVITEES_CODE','3105');//请输入正确的邀请码

//用户反馈
define('FEEDBACK_NUM_LIMIT','3201');//当天反馈次数到达上限
define('FEEDBACK_WORD_LENGTH_WRONG','3202');//反馈内容字数不对

//兑换码
define('EX_CODE_USED_TIMES_LIMIT','3301');//兑换码使用次数到达上限

//手机验证
define('MOBILE_VALIDATED', '3401');//已验证手机号
define('INCORRECT_MOBILE_CODE', '3402');//手机验证码错误
define('INVALID_MOBILE', '3403');//手机号码错误
define('SMS_IN_CD', '3404'); //短信CD
define('ALREADY_GET_MOBILE_REWARD', '3405'); //手机验证奖励已领取
//公会
define('USER_ALREADY_IN_GUILD','3501');//用户已经在公会中
define('GUILD_NAME_ALREADY_USED','3502');//公会名已经被使用
define('GUILD_NAME_FORBIDDEN','3503');//公会名被禁止
define('USER_NOT_IN_GUILD','3504');//用户不在公会中
define('LEADER_CAN_NOT_QUIT_GUILD','3505');//会长不能擅自退出公会
define('GUILD_MEMBER_LIMIT','3506');//人数已满
define('ALREADY_APPLY_THE_GUILD','3507');//已经申请了该公会
define('GUILD_APPLY_NUM_LIMIT','3508');//申请数量限制
define('APPLY_NOT_EXISTS','3509');//申请不存在
define('AUTHORITY_LIMIT','3510');//没有权限
define('NO_SUCH_GUILD','3511');//公会不存在
define('ASSISTANT_NUM_LIMIT','3512');//公会助理满员
define('ALREADY_SIGN_IN_GUILD','3513');//已经签到过
define('ACCEPT_NUM_LIMIT','3514');//接受申请已经达到上限
define('KING_MAX_NUM_LIMIT','3515');//今天捐献王道次数没有了
define('KNIGHT_MAX_NUM_LIMIT','3516');//今天捐献侠道道具个数够了
define('USER_IS_ALREADY_ASSISTANT','3517');//已经是助手了
define('ANSWER_IS_WRONG','3518');//答案错误
define('GUILD_ANNOUNCE_FORBIDDEN','3519');//门派公告输入错误
define('NOT_DECLARE_TIME','3520');//不是宣战时期
define('ALREADY_DECLARED','3521');//今天已经宣战了
define('PAID_TIME_LIMIT','3522');//付费次数限制
define('NOT_BOSS_TIME','3523');//不是弑神回合
define('NOT_ROB_TIME','3524');//不是争夺回合
define('BOSS_SETTLE_TIME_LIMIT','3525');//公会战时间，不允许该操作
define('NOT_GUILD_BOSS_TIME','3526');//公会战还未开始
define('GUILD_NOT_IN_BATTLE','3527');//您的公会未参加公会战
define('CAN_NOT_JOIN_BATTLE','3528');//您的公会不能参加公会战
define('NOT_IN_THE_SAME_GUILD','3529');//不在同一个公会
define('CAN_NOT_RESCUR_YOURSELF','3530');//不能救助自己
define('USER_NOT_WOUNDED','3531');//用户没受伤
define('BEING_RESCUED', '3532'); //正在被救助
define('OTHER_RESCUING', '3533'); //其他人正在救助
define('BEING_ROBBED', '3534'); //正在被抢劫
define('ROB_DATA_OUT_OF_DATE','3535');//可被抢夺星尘数据过时
define('NOT_IN_BATTLE_YESTERDAY','3536');//您昨天没有参加公会战
define('MSG_LENGTH_LIMIT','3537');//公会消息过长
define('MASTER_ON_LINE_IN_7_DAYS','3538');//掌门连续7日未登陆方可发起取代
define('NOT_ENOUGH_CON_LV','3539');//贡献等级不足
define('ALREADY_BE_MASTER','3540');//您就是掌门哦
define('FIRST_GUILD_USER_NOT_IN_BATTLE','3541');//公会首日加入的用户不可参加公会战
define('NEW_SERVER_GUILD_NOT_IN_BATTLE','3542');//新服未开放公会战
//region 狩魂
define('HUNT_NOT_ENOUGH_POWER', '3601');//狩魂体力不足
define('HUNT_WARRIOR_REQUIRED', '3602');//自己没有该弟子，不能抢夺
define('HUNT_TOO_LATE', '3603');//您来迟了……
define('HUNT_MAX_DAILY_GOT', '3604');//抢夺获得该魂魄的次数达到每日上限
//endregion
//月卡
define('MONTH_CARD_OUT_OF_DATE','3701');//月卡已经过期
define('MONTH_CARD_ALREADY_GET_AWARD','3702');//今天已经领取该月卡奖励
//灯谜
define('NOT_RIDDLE_TIME','3801');//不是猜灯谜的合适时间
define('ALREADY_RIDDLE_GUESSED','3802');//您今天已经猜过啦

//换装
define('DRESS_EXIST', '3901');     //时装已存在
define('DRESS_NOT_EXIST', '3902'); //时装不存在

//主角修行
define('NO_REQUIRE_WARRIOR', '4001'); //需要的弟子不存在
define('NOT_ENOUGH_FREE_TIMES', '4002'); //免费次数不足
define('NOT_ENOUGH_PRACTISE_TIMES', '4003'); //修行次数不足
define('RECRUIT_FIRST', '4004'); //必须先招募主角
define('PRACTISE_IN_CD', '4005'); //修行CD中

//抽卡兑换
define('NOT_ENOUGH_CARD_EXCHANGE_POINT', '4101'); //抽卡兑换点数不足
define('NO_SUCH_SOUL_EXCHANGE_ITEM', '4102'); //魂魄兑换项不存在
define('SOUL_EXCHANGE_REFRESHED', '4103'); //魂魄兑换项已刷新
//装备精练
define('CAN_NOT_BE_REFINED','4201');//该品阶装备不能被精练
define('ALREADY_MAX_REFINED_LV','4202');//已经是精炼最大等级

//组队副本
define('FRIEND_DUNGEON_LOCKED', '4301'); //组队副本未解锁
define('FRIEND_DUNGEON_NOT_AVAILABLE', '4302'); //组队副本挑战次数已用完
define('FRIEND_DUNGEON_MAX_RESET', '4303'); //组队副本重置次数达到上限
define('FRIEND_DUNGEON_CANNOT_RUSH', '4304'); //该组队副本不能扫荡
define('FRIEND_DUNGEON_AVAILABLE', '4305'); //该组队副本不需要重置状态
define('FRIEND_DUNGEON_IN_PROGRESS', '4306'); //该组队副本正在进行中
define('FRIEND_DUNGEON_UNIT_ALIVE', '4307'); //该单位未阵亡
define('FRIEND_DUNGEON_UNIT_DEAD', '4308'); //该单位已阵亡
define('FRIEND_DUNGEON_FRIEND_ASSISTED', '4309'); //该好友已助阵
define('FRIEND_DUNGEON_BUFF_DISABLED', '4310'); //符文不可用
define('FRIEND_DUNGEON_SAME_WARRIOR', '4311'); //助阵弟子与队伍中弟子冲突
define('FRIEND_DUNGEON_WARRIOR_NOT_EXIST', '4312'); //助阵弟子不存在
define('FRIEND_DUNGEON_ASSIST_REQUIRED', '4313'); //请选择助阵好友
define('FRIEND_DUNGEON_TODAY_FRIEND','4314'); //新好友不允许助阵

//好友
define('FRIEND_NOT_LV_TOO_LOW', '4401'); //好友等级过低
define('FRIEND_NOT_EXIST', '4402'); //好友不存在
define('YOUR_FRIEND_IS_MAX','4403');//您的好友已到上限
define('TARGET_FRIEND_IS_MAX','4404');//对方的好友已经到了上限
define('ALREADY_APPLY_THE_USER','4405');//您已经申请了这位玩家的好友
define('ALREADY_BE_HIS_FRIEND','4406');//你们已经是好友了
define('REQUEST_NOT_EXISTS','4407');//申请不存在
define('TARGET_REQUEST_NUM_LIMIT','4408');//对方申请已满
define('FRIEND_MAIL_CD_LIMIT','4409');//好友邮件cd中
define('FRIEND_MAIL_WORD_LIMIT','4410');//好友邮件字数限制100中文
define('YOU_ALREADY_SEND_GIFT','4411');//今天你已经送过他礼物了
define('HE_ALREADY_RECEIVED_GIFT_FROM_YOU','4412');//对方今天已经收过你的礼物了
define('YOU_ALREADY_RECEIVED_GIFT_FROM_HIM','4413');//您今天已经收过对方的礼物了
define('YOU_HAVE_NOT_GIFT_FROM_HIM','4414');//对方没有送礼给您
define('FRIEND_MAIL_NOT_EXISTS','4415');//好友邮件不存在
define('MAX_FRIEND_GIFT_RECEIVED','4416');//好友今天已经收够礼物了！
define('EMPTY_CONTENT','4417');//请不要发空信息
//facebook好友
define('FACEBOOK_FRIEDN_INSTALLED_NOT_ENOUGH', '4501');
define('ALREADY_GET_FACEBOOK_GIFT', '4502');
define('ALREADY_INVITE_FACEBOOK_OVER_30', '4503');
//vedio
define('VEDIO_CAN_NOT_RECEIVE', '4504');
define('VEDIO_ALREADY_RECEIVE', '4505');

//聊天
define('CHAT_IN_CD', '4601');//聊天CD中

//成就系统
define('TASK_ALREADY_GIVE', '4701');//该成就已经领取道具
define('TASK_UNFINISHED', '4702');//该成未完成

//求学悟道
define('STUDY_DAILY_MAX_COUNT','5201');//今日求学次数已用完
define('STUDY_NO_RESULT','5202');//没有求学
define('STUDY_LOCKED_GRIDS_MAX_COUNT','5203');//格子锁定数量达到最大
define('STUDY_GRID_LOCKED','5204');//格子已经被锁定
define('STUDY_NO_SIGN_REWARD','5205');//没有签到奖励

//炼化系统
define('REFINING_WARRIOR_ALREADY_BURNED', '4801');//弟子已经火化
define('REFINING_SOUL_NOT_ENOUGH', '4802');//炼化魂魄不足

//机关兽
define('MASTER_ALREADY_IN_FORMATION','5301');//机师已经在机关兽里啦
define('NO_SUCH_GUNDAM','5302');//机关兽不存在
define('ALREADY_HAS_GUNDAM','5303');//机关兽已存在
define('GUNDAM_NOT_ENOUGH','5304');//机关兽不够
define('ALREADY_MAX_GUNDAM_REFINED_LV','5305');//机关兽已经进化到顶级
define('THE_GUNDAM_GRADE_CANT_BE_REFINED','5306');//该品阶的机关兽不可进化
define('GUNDAM_ALREADY_MAX_LV','5307');//机关兽已经到了顶级
define('GUNDAM_ALREADY_IN_FORMATION','5308');//机关兽已经在阵上了
define('GUNDAM_IN_FORMATION_CANT_BE_DECOMPOSED','5309');//在阵上的机关兽不可被分解
define('GUNDAM_PIECE_NOT_ENOUGH','5310');//机关兽碎片不够
define('GUNDAM_NOT_IN_WARRIOR','5311');//机关兽还没有被佩戴

//寻龙山谷
define('DRAGON_VALLEY_CAN_NOT_SPEED_UP','5601');//宝库不能加速
define('DRAGON_VALLEY_NO_TREASURE','5602');//没有搜索到宝藏
define('DRAGON_VALLEY_SEARCH_EXPIRE','5603');//搜索结果已经过期
define('DRAGON_VALLEY_OCCUPY_ALREADY','5604');//宝库已经被占领
define('DRAGON_VALLEY_ASSIST_NUM_LIMIT','5605');//抢夺队伍数量超过宝库防守队伍数量
define('DRAGON_VALLEY_ASSIST_RELATION_LIMIT','5606');//助战玩家必须为同一公会
define('DRAGON_VALLEY_ASSIST_ALREADY','5607');//不能邀请该玩家
define('DRAGON_VALLEY_IN_OCCUPY','5608');//宝库占领未结束

//平台错误
define('ALREADY_USED_ON_THE_MACHINE','8001');
define('ALREADY_BE_INVITED','8002');
define('INVALID_EXCHANGE_CODE','8004');//错误的兑换码
define('EXCHANGE_CODE_NOT_AVAILABLE','8005');//兑换码还未开启
define('EXCHANGE_CODE_OUT_OF_DATE','8006');//兑换码已过期
define('EXCHANGE_CODE_ALREADY_USED','8007');//兑换码已经被使用
define('INVALID_APP_RECEIPT', '8008');//苹果支付凭证无效
define('USED_APP_RECEIPT', '8009');//已使用的苹果支付凭证
define('INVALID_GOOGLE_SIG', '8010'); //谷歌支付签名不正确
define('USED_GOOGLE_ORDER', '8011');//已使用的谷歌支付凭证
define('INVALID_SIGN', '8012');//签名错误

define('INVITE_CODE_NOT_EXISTS','8201');//邀请码不存在
define('USER_NOT_BE_INVITED','8202');//玩家没有被邀请
define('INVALID_TOKEN', '8999');     //授权码错误

//MYCARD
define('MY_CARD_ALREADY_USED','8101');//该卡已经使用
define('MYCARD_ORDERID_ALREADY_USED', '8102');//MYCARD BILLING ORDER ID 已經儲值過
define('MYCARD_WAIT_20_MIN', '8103');  //以儲值,但 MYCARD　端尚未處理完成,請玩家等待２０分鐘

//系统错误
define('USER_NOT_LOGON', '9000');   //玩家未登录
define('REDIS_ERROR', '9001');      //REDIS异常
define('INVALID_ARGUMENT', '9002'); //参数错误
define('NO_PRIVILEGE', '9003');     //没有权限
define('DISTRIBUTE_LOCK_TIMEOUT', '9004');     //锁超时
define('SYSTEM_MAINTAIN', '9005');    //系统维护中
define('PLAT_ID_TIMEOUT', '9006');  //平台账号过期
define('REQUEST_TOO_BUSY', '9007'); //请求过于频繁
define('SYS_ERROR', '9999');        //系统错误

define('EX_CODE_ERROR', '9501'); //CDKEY错误
define('GUESS_MUST_MACHINE_CODE', '9502'); //无法获取到游客的机器码

//wegame 平台CODE  9101 - 9500  已经被占用
//平台登陆注册
define('LOGIN_ERROR', '9101'); //登陆失败，确认账号密码
define('USERNAME_ERROR', '9102'); //用户名错误
define('PASSWARD_ERROR', '9103'); //密码错误
define('PASSWARD_NOT_MATCH', '9104'); //密码与确认密码不匹配
define('EMAIL_ERROR', '9105'); //邮箱错误
define('REGISTER_USERNAME_EXIST', '9106'); //用户已存在
define('REGISTER_ERROR', '9107'); //注册失败
define('RESET_PASSWORD_ERROR', '9108'); //修改密码失败
define('OLD_PASSWORD_INVALID', '9109'); //老密码无效
define('WG_CHARGE_SERVER_ERROR', '9110'); //充值服务器不对
define('GUESS_ID_NOT_EXIST', '9111'); //游客ID不存在
define('SERVER_LIST_EMPTY', '9112');//服务器列表不为空
define('WEGAME_USERNAME_NOT_EXIST', '9113'); //用戶帳號不存在！
define('WEGAME_USERNAME_PASSWROD_ERROR', '9114'); //用戶密碼錯誤
define('BIND_ERROR', '9115'); //绑定用户失败
define('SET_ROLE_ERROR', '9116'); //设置角色名失败
define('USERNAME_LOCKED', '9117'); //用户账号被锁定
