<?php
/**
 * Created by JetBrains PhpStorm.
 * User: pengya
 * Date: 13-5-2
 * Time: 下午5:16
 * To change this template use File | Settings | File Templates.
 */

class clsActivity
{

    CONST REDIS_NAMESPACE = 'Activity';
    //吃鸡
    const EAT_ROUND_ONE_START = ' 12:00:00';
    const EAT_ROUND_ONE_END = ' 14:00:00';
    const EAT_ROUND_TWO_START = ' 17:00:00';
    const EAT_ROUND_TWO_END = ' 19:00:00';
    const EAT_ROUND_THREE_START = ' 21:00:00';
    const EAT_ROUND_THREE_END = ' 24:00:00';
    //连续登录奖励
    const FIRST_DAY_DROP_ID = 100001;
    const SECOND_DAY_DROP_ID = 100002;
    const THIRD_DAY_DROP_ID = 100003;
    //签到
    const MAX_SIGN_IN_ROUND_NUM = 7;

    /**
     * 吃鸡
     */
    const TYPE_EAT = 1;
    /**
     * 签到
     */
    const TYPE_SIGN_IN = 2;
    /**
     * 迎财神
     */
    const TYPE_MAMMON = 3;
    /**
     * 充值送礼/累计储值 注册7天内累充 4-7天领奖
     */
    const TYPE_ROOKIE_TOTAL_CHARGE = 4;
    /**
     * 充值返现 注册4-7天
     */
    const TYPE_ROOKIE_CHARGE_RETURN = 5;
    /**
     * 手机验证码
     */
    const TYPE_MOBILE = 6;
    /**
     * 邀请
     */
    const TYPE_INVITE = 7;
    /**
     * 兑换商店(管理端开关型)
     */
    const TYPE_EXCHANGE_MALL = 8;
    /**
     * 消费返现(管理端开关型)
     */
    const TYPE_CONSUME_RETURN = 9;
    /**
     * 充值返现(管理端开关型)
     */
    const TYPE_CHARGE_RETURN = 10;
    /**
     * 累计充值
     */
    const TYPE_TOTAL_CHARGE = 11;

    /**
     *  月卡
     */
    const TYPE_MONTH_CARD = 12;
    /**
     * 猜灯谜
     */
    const TYPE_RIDDLE = 13;

    /**
     * 月卡累充
     */
    const TYPE_MONTH_CARD_TOTAL_CHARGE = 14;
    /**
     * 主角修行
     */
    const TYPE_LR_PRACTISE = 15;

    /**
     * 魂魄兑换商店
     */
    const TYPE_SOUL_EXCHANGE = 16;

    /**
     * 双倍充值
     */
    const TYPE_DOUBLE_CHARGE = 17;
    /**
     * facebook邀请
     */
    const TYPE_FACEBOOK_INVITE = 18;
    /**
     * 视频欣赏
     */
    const TYPE_VIDEO = 19;
    /**
     * 冲级奖励
     */
    const TYPE_SPRINT_LEVEL = 20;
    /**
     * 限时英雄
     */
    const TYPE_LIMIT_WARRIOR = 21;
    /**
     * 好友邀请与分享
     */
    const TYPE_REQUEST_SHARE= 22;

    /**
     * 开服等级返回
     */
    const TYPE_KF_LEVEL_RETURN = 23;
    
    /**
     * 开服充值返还
     */
    const TYPE_KF_CHARGE_RETURN = 24;
    
    /**
     * 日历
     */
    const TYPE_CALENDAR = 26;
    
    /**
     *  月卡商城
     */
    const TYPE_MONTH_CARD_MALL = 30;
    
    /**
     *  特殊签到（圣诞签到等）
     */
    const TYPE_SPECIAL_SIGN_IN = 31;
    /**
     *  成长礼包活动
     */
    const TYPE_GROWTH_FUND = 32;
    /**
     * 红包
     */
    const TYPE_RED_PAPER = 33;
    /**
     *  奇人密码
     */
    const TYPE_PILI_MOVIE = 34;
    /**
     *  收集领奖-新春快乐
     */
    const TYPE_COLLECT_LOTTERY_DRAW = 35;
    /**
     * 新年充值
     */
    const TYPE_NEW_YEAR_CHARGE = 36;
    /**
     *  喂财神-元宵活动
     */
    const TYPE_FEED_WEALTH_GOD = 37;
    /**
     * 消费抽奖
     */
    const  TYPE_CONSUME_LOTTERY = 38;
    /**
     * 挑战情侣活动-情人节活动
     */
    const  TYPE_KILL_LOVERS = 39;
    /**
     * 福袋活动-新春福袋
     */
    const  TYPE_LUCKY_POCKET = 40;
    /**
     * 答题活动-元宵答题
     */
    const  TYPE_ANSWER = 41;
    /**
     * 累充持续秒数 7*24*3600--从注册当天0点开始
     */
    const TOTAL_CHARGE_LAST_SECONDS = 604800;
    /**
     * 迎财神持续秒数 4*24*3600--从注册当天0点开始
     */
    const MAMMON_LAST_SECONDS = 345600;
    /**
     * @var 自增id
     */
    var $id;
    /**
     * @var 用户id
     */
    var $uid;
    /**
     * @var 吃鸡的次数
     */
    var $eat_times;
    /**
     * @var 上次登录时间
     */
    var $loginTs;
    /**
     * @var 连续登录次数
     */
    var $loginTimes;
    /**
     * @var 登录奖励 0,0,0
     */
    var $loginAward;
    /**
     * @var 签到时间戳
     */
    var $signTs;
    /**
     * @var 此轮已经签到次数
     */
    var $signTimes;
    /**
     * @var 签到轮数
     */
    var $signRound;
    /**
     * @var 签到是否是首轮
     */
    var $signFirstRound;
    /**
     * @var 签到限制次数
     */
    var $signLimit;
    /**
     * @var int 累计登录奖励领取情况
     */
    var $tLoginAward;
    /**
     * @var int 升级奖励领取情况
     */
    var $lvAward;
    /**
     * @var int 累计登录次数
     */
    var $tLoginTimes;
    /** 上次吃鸡时间
     * @var int
     */
    var $eatTs;
    /**
     * @var int 累计充值奖励领取情况
     */
    var $tChargeAward;
    /** 
     * 在线礼包领取次数
     * @var int
     */
    var $onlineGift;
    /**
     * 在线礼包上次领取时间
     * @var int
     */
    var $onlineGiftTs;
    
    /** 
     * 特殊签到活动签到次数
     * @var int
     */
    var $specialSignInTimes;
    /**
     * 特殊签到活动上次签到时间
     * @var int
     */
    var $specialSignInTs;
    /**
     * 特殊签到活动领取奖励状态
     * @var int
     */
    var $specialSignInAward;

    private static $instances = array();

    function __construct($userId, $eat_times = 0, $loginTs = TIME_NOW, $loginTimes = 1, $loginAward = 0, $signTs = 0, $signTimes = 0, $signRound = 1, $tLoginAward = 0, $lvAward = 0, $tLoginTimes = 1, $signLimit = 1, $eatTs = 0, $tChargeAward = 0,$onlineGift = 0 ,$onlineGiftTs = 0, $specialSignInTimes = 0, $specialSignInTs = 0, $specialSignInAward = 0, $id = 0, $signFirstRound = 0)
    {
        $this->uid = $userId;
        $this->loginTimes = $loginTimes;
        $this->loginTs = $loginTs;
        $this->loginAward = $loginAward;
        $this->signTs = $signTs;
        $this->signTimes = $signTimes;
        $this->signRound = $signRound;
        $this->tLoginAward = $tLoginAward;
        $this->lvAward = $lvAward;
        $this->eat_times = $eat_times;
        $this->signLimit = $signLimit;
        $this->tLoginTimes = $tLoginTimes;
        $this->eatTs = $eatTs;
        $this->tChargeAward = $tChargeAward;
        $this->onlineGift = $onlineGift;
        $this->onlineGiftTs = $onlineGiftTs;
        $this->specialSignInTimes = $specialSignInTimes;
        $this->specialSignInTs = $specialSignInTs;
        $this->specialSignInAward = $specialSignInAward;
        $this->id = $id;
        $this->signFirstRound = $signFirstRound;
    }

    static function handleEvent(clsEvent $event)
    {
        if ($event->id == clsEvent::EVENT_ID_LOGIN) {
            clsActivity::getInstance($event->userId)->updateLoginTimes();
        }
    }


    /**
     * @param $iUserId
     * @return clsActivity
     */
    static function getFromDB($iUserId)
    {
        $sSql = 'select * from T_USER_ACTIVITY where user_id=' . $iUserId;
        $row = c_mysql_db::DBConnect()->getRow($sSql);
        if (!empty($row)) {
            $result = new clsActivity($iUserId, intval($row['eat_times']), intval($row['login_ts']), intval($row['login_times']), intval($row['login_award']), intval($row['sign_in_ts']), intval($row['sign_in_times']), intval($row['sign_round']), intval($row['total_login_award']), intval($row['lv_up_award']), intval($row['total_login_times']), intval($row['sign_limit']), intval($row['eat_ts']), intval($row['total_charge_award']), intval($row['online_gift']), intval($row['online_gift_ts']), intval($row['special_sign_in_times']), intval($row['special_sign_in_ts']), intval($row['special_sign_in_award']), intval($row['id']),intval($row['signFirstRound']));
        } else {
            $result = new clsActivity($iUserId);
            $result->signLimit = clsSignAwardMeta::get(1)->times;
            $result->save();
        }
        return $result;
    }

    /**
     * @param $iUserId
     * @return clsActivity
     */
    static function getInstance($iUserId)
    {
        if (array_key_exists($iUserId, self::$instances)) {
            return self::$instances[$iUserId];
        } else {
            $instance = c_redis_db::get(
                self::REDIS_NAMESPACE,
                $iUserId,
                function () use ($iUserId) {
                    return clsActivity::getFromDB($iUserId);
                }
            );
            $isCli = defined('IS_CLI') && IS_CLI;
            if (!$isCli) {
                self::$instances[$iUserId] = $instance;
            }
            return $instance;
        }
    }

    /**
     *  更新数据
     */
    function update()
    {
        if ($this->id < 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        //$this->loadRedis();
        $this->save();
        c_redis_db::set(
            self::REDIS_NAMESPACE,
            $this->uid,
            $this
        );
    }

    function save()
    {
        if ($this->id == 0) {
            $sSql = '
insert into T_USER_ACTIVITY (user_id, eat_ts, eat_times, login_times,
total_login_times, login_ts, login_award, sign_in_ts, sign_in_times,
sign_round, sign_limit, total_login_award, lv_up_award, total_charge_award,
online_gift,online_gift_ts,special_sign_in_times,special_sign_in_ts,special_sign_in_award) values( '
                . $this->uid . ',' . $this->eatTs . ',' . $this->eat_times . ','
                . $this->loginTimes . ',' . $this->tLoginTimes . ',' . $this->loginTs . ','
                . $this->loginAward . ',' . $this->signTs . ',' . $this->signTimes . ','
                . $this->signRound . ',' . $this->signLimit . ',' . $this->tLoginAward . ','
                . $this->lvAward . ',' . $this->tChargeAward . ','
                . $this->onlineGift . ',' . $this->onlineGiftTs . ','
                . $this->specialSignInTimes . ',' . $this->specialSignInTs. ',' . $this->specialSignInAward . ')';
            $id = c_mysql_db::DBConnect()->insert($sSql);
            $this->id = $id;
        } else {
            $sSql = 'update T_USER_ACTIVITY
                  set eat_ts=' . $this->eatTs .
                ',eat_times = ' . $this->eat_times .
                ',login_times=' . $this->loginTimes .
                ',total_login_times = ' . $this->tLoginTimes .
                ',login_ts=' . $this->loginTs .
                ',login_award=' . $this->loginAward .
                ',sign_in_ts=' . $this->signTs .
                ',sign_in_times=' . $this->signTimes .
                ',sign_round=' . $this->signRound .
                ',sign_limit = ' . $this->signLimit .
                ',total_login_award=' . $this->tLoginAward .
                ',lv_up_award=' . $this->lvAward .
                ',total_charge_award=' . $this->tChargeAward . 
                ',online_gift=' . $this->onlineGift .
                ',online_gift_ts=' . $this->onlineGiftTs .
                ',special_sign_in_times=' . $this->specialSignInTimes .
                ',special_sign_in_ts=' . $this->specialSignInTs .
                ',special_sign_in_award=' . $this->specialSignInAward .
                ',signFirstRound=' . $this->signFirstRound .
                ' where id=' . $this->id;
            $updater = new clsAsyncUpdater(
                self::REDIS_NAMESPACE,
                $this->uid,
                $sSql
            );
            $updater->update();
        }
    }

    /**
     * 更新登录时间
     */
    function updateLoginTimes()
    {
        $loginDay = date('Y-m-d', $this->loginTs);
        $nowDay = date('Y-m-d');
        $yesDay = date('Y-m-d', strtotime('-1 day'));
        if ($loginDay < $nowDay) {
            if ($loginDay == $yesDay) {
                $this->loginTimes += 1;
            } else {
                $this->loginTimes = 1;
            }
            $this->tLoginTimes += 1;
            $this->loginTs = TIME_NOW;
            $this->initLoginAward();
            $this->initEat();
            $this->initSign();
        }

        $this->update();
    }
    
    /**
     * 更新在线礼包时间
     */
    function updateOnlineGift( $time = 0 )
    {
        if( $time > 0 ){
            $this->onlineGiftTs = $time;
            $this->update();
        }
        else{
            if( $this->onlineGiftTs <= 0 ){
                $user = clsUser::get($this->uid);
                $this->onlineGift = 0;
                $this->onlineGiftTs = $user->last_login_ts;
                $this->update();
            }
        }
    }

    /**判断连续登录奖励是否可以领取
     * @param $id
     * @return bool
     */
    function chkLoginAward($id)
    {
        if ($this->loginTimes < $id) {
            ret::throwError(NOT_ENOUGH_LOGIN_TIMES);
        }
        if (chkAwardStatus($this->loginAward, $id)) {
            ret::throwError(ALREADY_GET_LOGIN_GIFT);
        }
        return true;
    }

    /**获取连续登录奖励
     * @param $id
     * @return stdClass
     */
    function getLoginAward($id)
    {
        $award = array();
        switch ($id) {
            case 1:
                $award = clsLoot::roll($this->uid, self::FIRST_DAY_DROP_ID);
                break;
            case 2:
                $award = clsLoot::roll($this->uid, self::SECOND_DAY_DROP_ID);
                break;
            case 3:
                $award = clsLoot::roll($this->uid, self::THIRD_DAY_DROP_ID);
                break;
        }
        if (empty($award)) {
            ret::throwError(INVALID_ARGUMENT);
        }
        if ($award->type == clsLootItem::TYPE_COIN) {
            $user = clsUser::get($this->uid);
            $fRate = intval(pow(1.12, (3 * $user->lv - 1) / 3));
            $award = new clsLootItem(0, $award->type, 0, $award->num * $fRate);
        }
        $this->loginAward = updateAwardStatus($this->loginAward, $id);
        $this->update();
        clsLog::saveLog(new clsLog(clsLog::CATEGORY_NORMAL, $this->uid, '领取登录奖励', '', LOG_OP_ACTIVITY_LOGIN_AWARD));
        $award = $award->addToUser($this->uid, SYS_LOGIN);
        ret::setData('drop', $award);

        if ($id == 1) {
            $this->activityLoginReward();
        }
    }

    /**
     * 活动登录奖励
     */
    private function activityLoginReward()
    {
        $today = strtotime('today', TIME_NOW);
        if ($today == strtotime('2014-04-16 00:00:00') && $this->loginTimes >= 3) {
            clsMail::sendMail(
                $this->uid,
                '',
                '恭喜您获得连续登录奖励。元宝×388，烤山鸡×3，培养丹×5，修行秘籍×3',
                array(
                    new clsRewardItem(clsGoods::TYPE_BONUS, 0, 388),
                    new clsRewardItem(clsGoods::TYPE_ITEM, 9033, 3),
                    new clsRewardItem(clsGoods::TYPE_ITEM, 9011, 5),
                    new clsRewardItem(clsGoods::TYPE_ITEM, 9089, 3),
                )
            );
        }

        if ($today == strtotime('2014-04-24 00:00:00')) {
            clsMail::sendMail(
                $this->uid,
                '',
                '恭喜您获得活动登陆奖励：七海宝箱×1',
                array(
                    new clsRewardItem(clsGoods::TYPE_ITEM, 9090, 1)
                )
            );
        }

        if ($today >= strtotime('2014-04-25 00:00:00') && $today < strtotime('2014-04-28 00:00:00')) {
            clsMail::sendMail(
                $this->uid,
                '',
                '恭喜您获得活动登陆奖励：七海宝箱×1',
                array(
                    new clsRewardItem(clsGoods::TYPE_ITEM, 9091, 1)
                )
            );
        }
    }

    /**
     * 每天初始化连续登录领奖信息
     */
    function initLoginAward()
    {
        $this->loginAward = 0;
    }

    /**
     * 每天初始化签到信息
     */
    function initSign()
    {
//        $yesDay = date('Y-m-d', strtotime('-1 day'));
//        if (date('Y-m-d', $this->signTs) != $yesDay) {
//            $this->signTimes = 0;
//        }
    }
    
    /**
     * 初始化特殊签到信息
     */
    function initSpecialSign()
    {
        $this->specialSignInTimes = 0;
        $this->specialSignInAward = 0;
    }

    /**
     * 当天开始，初始化吃鸡
     */
    function initEat()
    {
        $this->eat_times = 0;
    }

    /**
     *  签到
     */
    function signIn()
    {
        $out = array();
        $old = date('Y-m-d', $this->signTs);
        $today = date('Y-m-d');

        if ($old == $today) { //判断是否签到过了
            ret::throwError(TODAY_ALREADY_SIGN_IN);
        }else {
            $this->signTimes += 1;
        }

        $signRound =  $this->signRound;//签到轮次

        $signItemMeta = clsSignIn::getInstance($signRound, $this->signTimes);
        $signItemMeta->confirmItemNum( );

        $out[] = $signItemMeta->itemid1->addToUser($this->uid, SYS_SIGN_IN);
        $out[] = $signItemMeta->itemid2->addToUser($this->uid, SYS_SIGN_IN);

        if($this->signTimes == self::MAX_SIGN_IN_ROUND_NUM){
            $this->signTimes = 0;
            $this->signRound += 1;
        }

        if($this->signFirstRound == 0){
            if($this->signRound == 5){
                $this->signRound = 2;
                $this->signFirstRound = 1;
            }
        }else{
            if($this->signRound == 6){
                $this->signRound = 2;
            }
        }

        $this->signTs = TIME_NOW;
        $this->update();

        ret::setData('drops', $out);
        ret::setData('signIn', $this->toSignInClient());
    }

    /** 返回签到数据
     * @return stdClass
     */
    function toSignInClient()
    {
        $obj = new stdClass();
        $obj->signTs = $this->signTs;
        $obj->signTimes = $this->signTimes;
        $obj->signRound = $this->signRound;
        return $obj;
    }

    /** 领取升级奖励
     * @param $lv
     */
    function getLvUpAward($lv)
    {
        $meta = clsActAwardMeta::get(clsActAwardMeta::LV_UP_AWARD, $lv);
        if (!$meta) {
            ret::throwError(INVALID_ARGUMENT);
        }
        if (chkAwardStatus($this->lvAward, $lv)) {
            ret::throwError(ALREADY_GET_LOGIN_GIFT);
        }
        $this->lvAward = updateAwardStatus($this->lvAward, $lv);
        $this->update();
        $award = $meta->reward->addToUser($this->uid, SYS_LV_UP_AWARD);
        clsLog::saveLog(new clsLog(clsLog::CATEGORY_NORMAL, $this->uid, '领取升级奖励', '', LOG_OP_ACTIVITY_LV_UP_AWARD, $lv));
        ret::setData('drop', $award);
    }

    /**领取累计登录奖励
     * @param $days
     */
    function getTotalLoginAward($days)
    {
        $meta = clsActAwardMeta::get(clsActAwardMeta::LOGIN_AWARD, $days);
        if (!$meta) {
            ret::throwError(INVALID_ARGUMENT);
        }
        if (chkAwardStatus($this->tLoginAward, $days)) {
            ret::throwError(ALREADY_GET_LOGIN_GIFT);
        }
        $this->tLoginAward = updateAwardStatus($this->tLoginAward, $days);
        $this->update();
        $award = $meta->reward->addToUser($this->uid, SYS_TOTAL_LOGIN_AWARD);
        clsLog::saveLog(new clsLog(clsLog::CATEGORY_NORMAL, $this->uid, '领取累计登录奖励', '', LOG_OP_ACTIVITY_TOTAL_LOGIN_AWARD, $days));
        ret::setData('drop', $award);
    }

    /**检查现在是否可以吃鸡
     * @return bool
     */
    function chkEat()
    {
        $iStart1 = strtotime(date('Y-m-d') . self::EAT_ROUND_ONE_START);
        $iEnd1 = strtotime(date('Y-m-d') . self::EAT_ROUND_ONE_END);
        $iStart2 = strtotime(date('Y-m-d') . self::EAT_ROUND_TWO_START);
        $iEnd2 = strtotime(date('Y-m-d') . self::EAT_ROUND_TWO_END);
        $iStart3 = strtotime(date('Y-m-d') . self::EAT_ROUND_THREE_START);
        $iEnd3 = strtotime(date('Y-m-d') . self::EAT_ROUND_THREE_END);
        $iNow = TIME_NOW;
        $iFlag = 0;
        if($this->eatTs < TIME_TODAY){
            $this->eat_times = 0;
        }
        if ($iNow >= $iStart1 && $iNow <= $iEnd1) {
            $iFlag = 1;
        } elseif ($iNow >= $iStart2 && $iNow <= $iEnd2) {
            $iFlag = 2;
        } elseif ($iNow >= $iStart3 && $iNow <= $iEnd3) {
            $iFlag = 3;
        }
        if ($iFlag == 0) //时间不对
        {
            ret::throwError(WRONG_TIME_TO_EAT);
        }
        if (($iFlag == 1 && $this->eat_times > 0) || 
                ($iFlag == 2 && $this->eat_times > 1) || 
                ($iFlag == 3 && $this->eat_times > 2)) {
            ret::throwError(YOU_ALREADY_EAT);
        }
        return $iFlag;
    }
    
    /**
     *  特殊签到（圣诞签到）
     *  $sDate 签到开始时间
     */
    function specialSignIn( $sDate )
    {
        if( $sDate > $this->specialSignInTs ){
            $this->initSpecialSign();
        }
        
        $old = date('Y-m-d', $this->specialSignInTs);
        $today = date('Y-m-d');
        
        if ($old == $today) { //判断是否签到过了
            ret::throwError(TODAY_ALREADY_SIGN_IN);
        }else {
            $this->specialSignInTimes += 1;
        }
        $this->specialSignInTs = TIME_NOW;
        $this->update();
    }
    
    /** 领取特殊签到奖励
     * @param $times
     */
    function getSpecialSignIn($times)
    {
        $specialSignInMeta = clsSpecialSignInMeta::get($times);
        if( !$specialSignInMeta ){
            ret::throwError(INVALID_ARGUMENT);
        }
        if( chkAwardStatus($this->specialSignInAward, $times) ){
            ret::throwError(SPECIAL_SIGN_IN_ALREADY_GIVE_REWARD);
        }
        /*
         * 1.0.7版本新春活动新增月卡用户双倍
         */
        $isMonthCardUser = clsMonthCard::getInstance($this->uid)->isMonthCardUser();
        
        $drops = array();
        foreach( $specialSignInMeta->awards as $k => $v ){
            if($isMonthCardUser)
                $v->num *= 2;
            $drops[] = $v->addToUser($this->uid,SYS_SPECIAL_SIGN_IN);
        }
        $this->specialSignInAward = updateAwardStatus($this->specialSignInAward, $times);
        $this->update();
        clsLog::saveLog(new clsLog(clsLog::CATEGORY_NORMAL, $this->uid, '领取特殊签到奖励', '', LOG_OP_ACTIVITY_SPECIAL_SIGN_IN_AWARD, $times));
        ret::setData('drops', $drops);
    }

    /**
     * @return clsActivity
     */
    function toClientObj()
    {
        $obj = new stdClass();
        $obj->eat_times = $this->eat_times;
        $obj->eatTs = $this->eatTs;
        $obj->signRound = $this->signRound;
        $obj->signTimes = $this->signTimes;
        $obj->signTs = $this->signTs;
        return $obj;
    }

    /**
     * 吃饭数据
     * @return stdClass
     */
    function toClientEat()
    {
        $obj = new stdClass();
        $obj->type = self::TYPE_EAT;
        $obj->eat_times = $this->eat_times;
        $obj->eatTs = $this->eatTs;
        return $obj;
    }

    /**
     * 签到数据
     * @return stdClass
     */
    function toClientSignIn()
    {
        $obj = new stdClass();
        $obj->type = self::TYPE_SIGN_IN;
        $obj->signRound = $this->signRound;
        $obj->signTimes = $this->signTimes;
        $obj->signTs = $this->signTs;
        return $obj;
    }

    /** 连续登陆奖励数据
     * @return stdClass
     */
    function toClientLoginAward()
    {
        $obj = new stdClass();
        $obj->loginTimes = $this->loginTimes;
        $obj->loginAward = $this->loginAward;
        return $obj;
    }
    
    /** 在线礼包数据
     * @return stdClass
     */
    function toClientOnlineGift()
    {
        $obj = new stdClass();
        $obj->onlineGift = $this->onlineGift;
        $obj->onlineGiftTs = $this->onlineGiftTs;
        return $obj;
    }

    /**领奖界面
     * @return stdClass
     */
    function toClientAward()
    {
        $obj = new stdClass();
        $obj->tLoginAward = $this->tLoginAward;
        $obj->lvAward = $this->lvAward;
        $obj->tLoginTimes = $this->tLoginTimes;
        return $obj;
    }
}