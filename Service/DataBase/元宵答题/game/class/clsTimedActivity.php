<?php
/**
 * Desc:
 * User: wangyanjun
 * Date: 13-6-28
 * Time: 下午5:30
 */

class clsTimedActivity
{
    const REDIS_NAMESPACE = 'TIMED_ACTIVITY';

    /**
     * 后台控制活动的开始时间
     */
    const TTIMED_ACTIVITY_START = 950400;//1468800
    
    /*
     * 新服默认开启列表
     */
    static $DEFAULT_OPEN_LIST = array(
        10000001 => array(clsActivity::TYPE_EXCHANGE_MALL, 0, 864000),
        10000002 => array(clsActivity::TYPE_LIMIT_WARRIOR, 604800, 950400)
    );
    /*
     * 指定活动配置ID
     */
    static $LOAD_CONF_LIST = array(
        10000001 => 1,
        10000002 => 1
    );
    
    /**
     * @var 活动编号
     */
    var $id;
    /**
     * @var 活动类型
     */
    var $type;
    /**
     * @var 开始时间
     */
    var $startTs;
    /**
     * @var 结束时间
     */
    var $endTs;
    /**
     * @var 是否在保护期内开启
     */
    var $isNewServer;

    /**
     * @var 活动列表
     */
    private static $list;

    function __construct($id, $type, $startTs, $endTs, $isNewServer = 0)
    {
        $this->id = $id;
        $this->type = $type;
        $this->startTs = $startTs;
        $this->endTs = $endTs;
        $this->isNewServer = $isNewServer;
    }

    function save()
    {
        self::loadRedis();
        $this->update();
        c_redis_db::setHashValue(
            self::REDIS_NAMESPACE,
            '',
            $this->id,
            $this
        );
    }

    function toClientObj($userId)
    {
        $obj = new stdClass();
        $obj->id = $this->id;
        $obj->type = $this->type;
        $obj->startTs = $this->startTs;
        $obj->endTs = $this->endTs;
        switch ($this->type) {
            case clsActivity::TYPE_CHARGE_RETURN:
                $obj->amount = clsChargeReturn::get($this->id, $userId)->amount;
                break;
            case clsActivity::TYPE_CONSUME_RETURN:
                $consume = clsConsumeReturn::get($this->id, $userId);
                $obj->amount = $consume->points + $consume->bonus;
                break;
            case clsActivity::TYPE_EXCHANGE_MALL:
                $exchange = clsExchange::get($this->id, $userId);
                $obj->conf = self::getConfigId($this->id);
                $obj->contr = $exchange->contribute;
                $obj->tContr = $exchange->totalContribute;
                $obj->gReward = $exchange->globalReward;
                $obj->refreshTs = $exchange->refreshTs;
                $obj->exItems = $exchange->getExItemsToClient();
                $mall = clsExchangeMall::get($this->id);
                $obj->gContr = $mall->getContribute();
                $obj->firstEndTs = $mall->getFirstStageEndTs($this);
                break;
            case clsActivity::TYPE_TOTAL_CHARGE:
                $totalCharge = clsTotalCharge::get($userId, $this->id);
                $obj->amount = $totalCharge->amount;
                $obj->status = $totalCharge->getStatus();
                break;
            case clsActivity::TYPE_MONTH_CARD_TOTAL_CHARGE:
                $monthTotalCharge = clsMonthCardTotalCharge::get($userId, $this->id);
                $monthcard = clsMonthCard::getInstance($userId);
                $obj->monthcard = intval($monthcard->isMonthCardUser());
                $obj->amount = $monthTotalCharge->amount;
                $obj->reward = $monthTotalCharge->reward;
                break;
            case clsActivity::TYPE_SOUL_EXCHANGE:
                $exchange = clsSoulExchange::get($userId);
                $exchange->autoRefresh($this->id);
                $obj->conf = self::getConfigId($this->id);
                $obj->recommend = clsSoulExchangeMeta::getRecommendWarriors($this->id);
                $obj->point = $exchange->point;
                $obj->refreshTs = $exchange->refreshTs;
                $obj->items = $exchange->items;
                break;
            case clsActivity::TYPE_LIMIT_WARRIOR://限时英雄
                $obj->conf = clsTimedActivity::getConfigId( $this->id );
                foreach( clsLimitTimeDrawCard::get($this->id, $userId)->toClientObj() as $k =>$v )
                    $obj->{$k} = $v;
                break;
            case clsActivity::TYPE_REQUEST_SHARE://好友邀请
                foreach(clsUserRequestShare::get($userId)->toClientObj() as $k =>$v )
                    $obj->{$k} = $v;
                break;
            case clsActivity::TYPE_MONTH_CARD_MALL://月卡商城
                $obj->monthcard = intval(clsMonthCard::getInstance($userId)->isMonthCardUser());
                break;
            case clsActivity::TYPE_SPECIAL_SIGN_IN://特殊签到（圣诞签到等）
                $act = clsActivity::getInstance($userId);
                if( $this->startTs > $act->specialSignInTs ){
                    $act->initSpecialSign();
                }
                $obj->specialSignInTs = $act->specialSignInTs;
                $obj->specialSignInTimes = $act->specialSignInTimes;
                $obj->specialSignInAward = $act->specialSignInAward;
                break;
            case clsActivity::TYPE_RED_PAPER:
                $money = clsCharge::chargeSum($userId, date('Y-m-d H:i:s', $this->startTs), date('Y-m-d H:i:s', $this->endTs));
                $obj->money = $money;

                $info = clsHongbao::get($userId);
                $obj->username = $info->username;
                $obj->areanumber = $info->areanumber;
                $obj->phone = $info->phone;
                $obj->address = $info->address;
                $obj->state = $info->state;;
                break;
            case clsActivity::TYPE_NEW_YEAR_CHARGE:
                $new_year_info = clsNewyearCharge::get($this->id, $userId);
                if($new_year_info->before_money == 0){
                    $money = clsCharge::chargeSum($userId, '2014-11-27', date('Y-m-d H:i:s', $this->startTs));
                    $new_year_info->before_money = $money + 1;//计算过不论是否充值过都加1防止在计算一次
                }
                $new_year_info->save();

                if($new_year_info->before_money >= 1){
                    $new_year_info->before_money -= 1;
                }
                $obj->score = $new_year_info->getScore();
                $obj->reward_record = $new_year_info->reward_record;
                break;
            case clsActivity::TYPE_COLLECT_LOTTERY_DRAW://收集抽奖 - 新春快乐！！！！！哈哈哈哈啊啊啊
                $act = clsCollectLotteryDraw::get($this->id, $userId, TIME_TODAY);
                foreach( $act->toClientObj() as $k =>$v )
                        $obj->{$k} = $v;
                break;
            case clsActivity::TYPE_FEED_WEALTH_GOD://喂财神 - 元宵快乐！！！！！哈哈哈哈啊啊啊
                $act = clsFeedWealthGod::get($this->id, $userId);
                foreach( $act->toClientObj() as $k =>$v )
                    $obj->{$k} = $v;
                break;
            case clsActivity::TYPE_CONSUME_LOTTERY:
                $lotteryInfo = clsConsumeLottery::get($this->id, $userId);
                $obj->point = $lotteryInfo->point;//消费的总元宝
                $obj->number = $lotteryInfo->getNumber();//计算剩余次数
                break;
            case clsActivity::TYPE_KILL_LOVERS://挑战情侣-情人节活动
                $act = clsKillLovers::get($this->id, $userId);
                foreach( $act->toClientObj() as $k =>$v )
                    $obj->{$k} = $v;
                break;
            case clsActivity::TYPE_ANSWER://答题活动-元宵答题
                $act = clsAnswer::get($this->id, $userId,TIME_TODAY);
                foreach( $act->toClientObj() as $k =>$v )
                    $obj->{$k} = $v;
                break;
            default:
                break;
        }
        return $obj;
    }

    function sendGifts($date)
    {
        switch ($this->type) {
            case clsActivity::TYPE_MAMMON:
                break;
            case clsActivity::TYPE_CHARGE_RETURN:
                clsChargeReturn::sendGifts($this, $date);
                break;
            case clsActivity::TYPE_CONSUME_RETURN:
                clsConsumeReturn::sendGifts($this, $date);
                break;
            case clsActivity::TYPE_EXCHANGE_MALL:
                clsExchangeMall::sendGifts($this, $date);
                break;
            case clsActivity::TYPE_TOTAL_CHARGE:
                clsTotalCharge::sendGifts($this, $date);
                break;
            default:
                break;
        }
    }

    /**
     * @return bool 是否在活动期间
     */
    function isAvailable()
    {
        return TIME_NOW >= $this->startTs && TIME_NOW < $this->endTs;
    }

    /*
     * 限时活动开启时间
     */
    static function getTimedActivityStartTime(){
        
        return strtotime(SERVER_START_TS) + self::TTIMED_ACTIVITY_START;
        
    }
    
    /*
     * 是否开启默认活动
     */
    static function isDefaultOpenEnabled(){
        return true;
    }
    
    /*
     * 是否为默认开启活动ID
     */
    static function isDefaultOpenActivity( $id ){
        if ( !self::isDefaultOpenEnabled() ) {
            return false;
        }
        return array_key_exists($id, self::$DEFAULT_OPEN_LIST);
    }
    
    /*
     * 添加默认开启活动
     * 例： 10000001 => array(clsActivity::TYPE_EXCHANGE_MALL, 0, 604800)
     */
    static function addDefaultOpenActivity( & $list = array( ), $date = TIME_NOW, $type = 0 ){
        if( !self::isDefaultOpenEnabled() ) return ;
        if( self::getTimedActivityStartTime() < $date ) return;
        $ServerTime = strtotime(SERVER_START_TS);
        foreach( self::$DEFAULT_OPEN_LIST as $k => $v){
            if( $ServerTime + $v[1] <= $date && $ServerTime + $v[2] >= $date ){
                if( $type > 0 && $type != $v[0] ) continue;
                $list[$k] = new clsTimedActivity(
                    $k,
                    intval($v[0]),
                    intval($ServerTime + $v[1]),
                    intval($ServerTime + $v[2])
                );
            }
        }
    }
    
    /*
     * 获取配置ID
     */
    static function getConfigId( $activity ){
        if( !self::isDefaultOpenActivity($activity) ){
            return  0;
        }
        return intval( self::$LOAD_CONF_LIST[$activity] );
    }
    
    /**
     * @param $id
     * @return clsTimedActivity
     */
    static function get($id)
    {
        return
            c_redis_db::getFromHash(
                self::REDIS_NAMESPACE,
                '',
                $id,
                function&() {
                    $list = clsTimedActivity::getFromDb(TIME_NOW);
                    return $list;
                }
            );
    }

    static function loadRedis()
    {
        c_redis_db::loadHash(
            self::REDIS_NAMESPACE,
            '',
            function&() {
                $list = clsTimedActivity::getFromDb(TIME_NOW);
                return $list;
            },
            1800
        );
    }

    /**
     * @return clsTimedActivity[]
     */
    static function getList()
    {
        if (!self::$list || !auth_module::$userId) {
            self::$list = c_redis_db::loadHashAndReturn(
                self::REDIS_NAMESPACE,
                '',
                function &() {
                    $list = clsTimedActivity::getFromDb(TIME_NOW);
                    return $list;
                },
                1800
            );
        }

        return self::$list;
    }

    /**
     * @param $minEndTs
     * @return clsTimedActivity[]
     */
    static function getFromDb($minEndTs)
    {
        $list = array();
        $rows = c_mysql_db::DBConnect()->getAll('SELECT `id`, `type`, `is_new_server`, start_ts, end_ts
        FROM T_TIMED_ACTIVITY WHERE end_ts > ' . $minEndTs . ' and ( start_ts >= ' . self::getTimedActivityStartTime() . ' or is_new_server=1 )' );
        foreach ($rows as $row) {
            $id = intval($row['id']);
            $list[$id] = new clsTimedActivity(
                $id,
                intval($row['type']),
                intval($row['start_ts']),
                intval($row['end_ts']),
                intval($row['is_new_server'])
            );
        }
        unset($rows);
        self::addDefaultOpenActivity( $list, $minEndTs );
        return $list;
    }

    /**
     * @param $type
     * @param $date
     * @param $dateStart
     * @return clsTimedActivity[]
     */
    static function getByEndDate($type, $date, $dateStart = 0)
    {
        $endEnd = $date;
        $endStart = $dateStart ? $dateStart : $date - 86400;
        $rows = c_mysql_db::DBConnect()->getAll('SELECT `id`, `type`, start_ts, end_ts, is_new_server
        FROM T_TIMED_ACTIVITY WHERE type = ' . $type . ' AND end_ts <= ' . $endEnd . ' AND end_ts > ' . $endStart . ' and ( start_ts >= ' . self::getTimedActivityStartTime().' or is_new_server=1 )');
        $list = array();
        foreach ($rows as $row) {
            $id = intval($row['id']);
            $list[$id] = new clsTimedActivity(
                $id,
                intval($row['type']),
                intval($row['start_ts']),
                intval($row['end_ts']),
                intval($row['is_new_server'])
            );
        }
        self::addDefaultOpenActivity( $list, $endStart , $type );
        foreach( $list as $k => $v ){
            if( $v->endTs > $endEnd || $v->endTs <= $endStart ){
                unset($list[$k]);
            }
        }
        return $list;
    }

    private function update()
    {
        $db = c_mysql_db::DBConnect();
        $count = intval($db->getOne('SELECT COUNT(`id`) FROM T_TIMED_ACTIVITY WHERE `id` = ' . $this->id));
        if ($count > 0) {
            $db->update('UPDATE T_TIMED_ACTIVITY
                SET start_ts = ' . $this->startTs
                . ', end_ts = ' . $this->endTs
                . ', is_new_server = ' . $this->isNewServer
                . ' where `id`=' . $this->id);
        }
        else {
            $db->insert('INSERT INTO T_TIMED_ACTIVITY (`id`, `type`, `start_ts`, `end_ts`, `is_new_server`)
            VALUES (' . $this->id . ',' . $this->type . ',' . $this->startTs . ',' . $this->endTs . ',' . $this->isNewServer . ')');
        }
    }

    static function handleEvent(clsEvent $event)
    {
        $eventIds = array(
            clsEvent::EVENT_ID_CHARGE,
            clsEvent::EVENT_ID_CONSUME,
            clsEvent::EVENT_ID_DRAW_CARD_TYPE_ONLY,
            clsEvent::EVENT_ID_EXPLORE_BOSS,
            clsEvent::EVENT_ID_CHALLENGE,
            clsEvent::EVENT_ID_EXPLORE_RUSH
        );

        if (!in_array($event->id, $eventIds)) {
            return;
        }

        $list = self::getList();
        foreach ($list as $act) {
            if (!$act->isAvailable()) continue;

            switch ($act->type) {
                case clsActivity::TYPE_CHARGE_RETURN:
                    clsChargeReturn::handleEvent($act, $event);
                    break;
                case clsActivity::TYPE_CONSUME_RETURN:
                    clsConsumeReturn::handleEvent($act, $event);
                    break;
                case clsActivity::TYPE_TOTAL_CHARGE:
                    clsTotalCharge::handleEvent($act, $event);
                    break;
                case clsActivity::TYPE_EXCHANGE_MALL:
                    break;
                case clsActivity::TYPE_MONTH_CARD_TOTAL_CHARGE:
                    clsMonthCardTotalCharge::handleEvent($act, $event);
                    break;
                case clsActivity::TYPE_SOUL_EXCHANGE:
                    clsSoulExchange::handleEvent($act, $event);
                    break;
		case clsActivity::TYPE_DOUBLE_CHARGE:
                    clsChargeDouble::handleEvent($act, $event);
                    break;
                case clsActivity::TYPE_COLLECT_LOTTERY_DRAW:
                    clsCollectLotteryDraw::handleEvent($act, $event);
                    break;
                case clsActivity::TYPE_FEED_WEALTH_GOD:
                    clsFeedWealthGod::handleEvent($act, $event);
                    break;
                default:
                    break;
            }
        }
    }
}