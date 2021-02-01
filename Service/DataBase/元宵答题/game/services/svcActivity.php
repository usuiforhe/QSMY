<?php
/**
 * Created by JetBrains PhpStorm.
 * User: pengya
 * Date: 13-5-2
 * Time: 下午2:45
 * To change this template use File | Settings | File Templates.
 */

class svcActivity
{
    public $iUserId = 0;

    function __construct($iUserId)
    {
        $this->iUserId = $iUserId;
    }

    /**
     * 获取玩家活动信息
     */
    function get()
    {
        $activities = array();
        $act = clsActivity::getInstance($this->iUserId);
        $act->updateLoginTimes();

        $activities[] = $act->toClientEat();
        
        //迎财神
        if (clsMammon::isAvailable($this->iUserId)) {
            $activities[] = clsMammon::get($this->iUserId)->toClientObj();
        }
        
        //4-7天充值返现
        if (clsRookieChargeReturn::isAvailable($this->iUserId)) {
            $activities[] = clsRookieChargeReturn::get($this->iUserId)->toClientObj();
        }
        
        //新账号第8天起7天内累计充值送礼
        if (clsRookieTotalCharge::isAvailable($this->iUserId)) {
            $activities[] = clsRookieTotalCharge::get($this->iUserId)->toClientObj();
        }
        
       //新手30天内冲级奖励
        if (clsSprintLevel::isAvailable($this->iUserId)) {
            $activities[] = clsSprintLevel::get($this->iUserId)->toClientObj();
        }
        
        if (!defined('DISABLE_INVITATION') || !DISABLE_INVITATION) {
            $invitation = clsInvitation::get($this->iUserId, auth_module::$plat, auth_module::$mCode);
            if ($invitation->invitee_award < clsInvitation::MIN_INVITE_LV || $invitation->inviter_award != clsInvitation::GET_ALL_INVITEE_AWARD) {
                $activities[] = $invitation->toClientObj();
            }
        }
        
//        $mobile = clsMobile::get($this->iUserId);
//        if ($mobile->status != clsMobile::STATUS_DRAWN) {
//            $activities[] = $mobile->toClientObj();
//        }
//        
//        $activities[] = clsRiddles::getInstance($this->iUserId)->toClientObj();
//        if (clsLRPractise::isAvailable()) {
//            $activities[] = clsLRPractise::get($this->iUserId)->toClientObj();
//        }
//        if (clsFacebookInvite::isAvailable($this->iUserId)) {
//            $activities[] = clsFacebookInvite::get($this->iUserId)->toActivityObj();
//        }

        //月卡
        $activities[] = clsMonthCard::getInstance($this->iUserId)->toClientObj();
        
//        $activities[] = clsUserRequestShare::get($this->iUserId)->toClientObj();

        //23 24 活动，开服1服
        if(SERVER_ID == 1 || SERVER_ID == 999){
            if(strtotime(date('Y-m-d')) - strtotime(SERVER_START_TS) < 7 * 86400){
                $u = clsUser::get($this->iUserId);
                if($u){
                    $tmp = clsKFChargeReturn::get($u->pid);
                    if($tmp->id != 0 && $tmp->state == 0){
                        $activities[] = $tmp->toClientObj($this->iUserId);
                    }

                    $tmp = clsKFLevelReturn::get($u->pid);
                    if($tmp->id != 0 && $tmp->state == 0){
                        $activities[] = $tmp->toClientObj();
                    }
                }
            }
        }
        
        //月签到
        if (clsCalendar::isAvailable()) {
            $activities[] = clsCalendar::get($this->iUserId)->toClientObj();
        }

        //求学
        $study = clsStudy::get($this->iUserId);
        $study->refresh();
        $activities[] = $study->toClientObj();
        unset($study);
        
        
        //成长基金----------------------------------------------------------------------------------------------------------------------------------
        $objGrowthFund = new stdClass();
        foreach(array_keys(clsGrowthFund::$arrType) as $k => $v){
            if($growthFund = clsGrowthFund::get($this->iUserId, $v))
                $objGrowthFund->{'growthFund_'.$v} = $growthFund->toClientObj();
        }
        if( (clsGrowthFund::isAvailable($this->iUserId) && ($objGrowthFund->growthFund_1->isBuy == FALSE || $objGrowthFund->growthFund_2->isBuy == FALSE))
            ||    
            ($objGrowthFund->growthFund_1->isBuy == TRUE && $objGrowthFund->growthFund_1->rewards < clsGrowthFund::$arrGiveMaxNum[1]) 
            || 
            ($objGrowthFund->growthFund_2->isBuy == TRUE && $objGrowthFund->growthFund_2->rewards < clsGrowthFund::$arrGiveMaxNum[2])
            ){
            $objGrowthFund->type = clsActivity::TYPE_GROWTH_FUND;
            $objGrowthFund->endTs = clsGrowthFund::get($this->iUserId, 1)->regDate + (clsGrowthFund::START_DAY + clsGrowthFund::LAST_DAYS) * 86400;
            $activities[] = $objGrowthFund;
        }
        unset($objGrowthFund);
        //-------------------------------------------------------------------------------------------------------------------------------------------
        
        //奇人密码
        if (clsPiliMovie::isAvailable($this->iUserId)) {
            $activities[] = clsPiliMovie::get($this->iUserId)->toClientObj();
        }
        
        //GM工具控制的实效类活动
        $list = clsTimedActivity::getList();
        foreach ($list as $activity) {
            if (!$activity->isAvailable()) continue;
            $obj = $activity->toClientObj($this->iUserId);
            if (!is_null($obj)) {
                $activities[] = $obj;
            }
        }
        
        ret::setData('activities', $activities);
        ret::setSuccess();
    }
    
    function getSpecial(){
        ret::setData('isOpen', FALSE);
        ret::setData('activityId', clsActivity::TYPE_EAT);
        ret::setSuccess();
    }

    /**
     * 连续登录奖励界面
     */
    function getLoginAward()
    {
        $act = clsActivity::getInstance($this->iUserId);
        $act->updateLoginTimes();
        ret::setData('loginAward', $act->toClientLoginAward());
        ret::setSuccess();
    }

    /**
     * 领奖界面
     */
    function getAward()
    {
        $act = clsActivity::getInstance($this->iUserId);
        $act->updateLoginTimes();
        ret::setData('award', $act->toClientAward());
        ret::setSuccess();
    }

    /**
     * 吃鸡，定时恢复体力
     */
    function eat()
    {
        $act = clsActivity::getInstance($this->iUserId);
        //判断时间点以及是否吃过
        $eatId = $act->chkEat();
        $user = clsUser::get($this->iUserId);
        //更新数据
        $act->eat_times = $eatId;
        $act->eatTs = TIME_NOW;
        $act->update();
        $user->addPowerValue(EAT_CHICKEN, $logs);
        $user->updateUser();
        //日志
        $logs[] = new clsLog(clsLog::CATEGORY_NORMAL, $this->iUserId, '吃鸡', '', LOG_OP_ACTIVITY_EAT, $user->pow);
        clsLog::saveLogs($logs);
        //事件
        clsEventManager::getInstance()->addEvent(new clsEvent($this->iUserId, clsEvent::EVENT_ID_EAT_CHICKEN));
        //返回
        ret::setData('user', array('pow' => $user->getPower()));
        ret::setSuccess();
    }


    /** 领取连续登录奖励
     * @param $args
     */
    function loginAward($args)
    {
        $id = intval($args->id);
        //判断输入参数
        if ($id < 1 || $id > 3) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $act = clsActivity::getInstance($this->iUserId);
        //判断是否可以领取该奖励,如果可以则更新状态
        if ($act->chkLoginAward($id)) {
            //领取奖励
            $act->getLoginAward($id);
        }
        //返回信息
        ret::setSuccess();
    }


    /**
     * 获取签到信息
     */
    function getSignIn(){
        $act = clsActivity::getInstance($this->iUserId);
        $act->updateLoginTimes();
        $activities = $act->toClientSignIn();
        ret::setData('signin', $activities);
        ret::setSuccess();
    }

    /**
     * 签到 以及 签到奖励(自动发放)
     */
    function signIn()
    {
        $act = clsActivity::getInstance($this->iUserId);
        $act->signIn();
        clsLog::saveLog(new clsLog(clsLog::CATEGORY_NORMAL, $this->iUserId, '签到', '', LOG_OP_ACTIVITY_SIGN_IN));
        clsEventManager::getInstance()->addEvent(new clsEvent($this->iUserId, clsEvent::EVENT_ID_SIGN_UP_NEW));
        ret::setSuccess();
    }

    /** 领取升级奖励
     * @param $args
     */
    function lvUpAward($args)
    {
        $lv = intval($args->lv);
        $user = clsUser::get($this->iUserId);
        if ($user->lv < $lv) {
            ret::throwError(USER_lV_LOW);
        }
        $activity = clsActivity::getInstance($this->iUserId);
        $activity->getLvUpAward($lv);
        ret::setSuccess();
    }

    /**领取累计登录奖励
     * @param $args
     */
    function tLoginAward($args)
    {
        $days = intval($args->d);
        $activity = clsActivity::getInstance($this->iUserId);
        if ($activity->tLoginTimes < $days) {
            ret::throwError(NOT_ENOUGH_LOGIN_TIMES);
        }
        $activity->getTotalLoginAward($days);
        ret::setSuccess();
    }

    /**
     *  迎财神
     */
    function mammon()
    {
        if (!clsMammon::isAvailable($this->iUserId)) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $mammon = clsMammon::get($this->iUserId);
        if ($mammon->chkMammon()) {
            $mammon->getMammon();
        }
        ret::setSuccess();
    }


    /**
     * 获得限时活动列表
     * @param $args
     */
    function getTimedList($args)
    {
        $list = clsTimedActivity::getList();
        $out = array();
        foreach ($list as $activity) {
            if (!$activity->isAvailable()) continue;
            $obj = $activity->toClientObj($this->iUserId);
            if (!is_null($obj)) {
                $out[] = $obj;
            }
        }
        ret::setData('timedActivities', $out);
        ret::setSuccess();
    }

    /**
     * 获得充值返现信息
     */
    function getChargeReturn()
    {
        $chargeReturn = clsRookieChargeReturn::get($this->iUserId)->toClientObj()->amount;
        ret::setData('chargeReturn', $chargeReturn);
        ret::setSuccess();
    }


    /**
     * 获得消费返现信息
     * @param $args
     */
    function getConsumeReturn($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $consumeReturn = clsConsumeReturn::get($activityId, $this->iUserId);
        ret::setData('consumeReturn', $consumeReturn->points + $consumeReturn->bonus);
        ret::setSuccess();
    }

    /**
     * 获得兑换商店兑换项列表
     * @param $args
     */
    function EMGetExItems($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $exchange = clsExchange::get($activityId, $this->iUserId);
        ret::setData('exItems', $exchange->getExItemsToClient());
        ret::setSuccess();
    }

    /**
     * 获得兑换商店全局贡献值
     * @param $args
     */
    function EMGlobalContr($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        ret::setData('contr', clsExchangeMall::get($activityId)->getContribute());
        ret::setSuccess();
    }

    /**
     * 查询兑换商店排行榜
     * @param $args
     */
    function EMRank($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        ret::setData('EMRank', clsExchangeMall::getRank($activityId));
        ret::setSuccess();
    }

    /**
     * 兑换商店刷新兑换列表
     * @param $args
     */
    function EMRefresh($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }

        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        $mall = clsExchangeMall::get($activityId);
        if ($mall->getStage() != clsExchangeMall::FIRST_STAGE) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        $exchange = clsExchange::get($activityId, $this->iUserId);
        $exchange->refresh();
        ret::setData('exItems', $exchange->getExItemsToClient());
        ret::setData('addContr', clsExchange::REFRESH_CONTRIBUTE);
        ret::setSuccess();
    }

    /**
     * 兑换商店进行兑换
     * @param $args
     */
    function EMExchange($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $index = intval($args->index);
        $exId = intval($args->exId);
        if ($exId <= 0 || $index <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $equipId = intval($args->eid);
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        $mall = clsExchangeMall::get($activityId);
        if ($mall->getStage() != clsExchangeMall::FIRST_STAGE) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        $exchange = clsExchange::get($activityId, $this->iUserId);
        $exchange->exchange($exId, $equipId, $index);
        ret::setSuccess();
    }

    /**
     * 兑换商店,使用贡献值兑换奖励
     * @param $args
     */
    function EMExReward($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }

        $rewardId = intval($args->rid);
        if ($rewardId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        $exchange = clsExchange::get($activityId, $this->iUserId);
        $reward = $exchange->exchangeReward($rewardId);
        ret::setData('contr', $exchange->contribute);
        ret::setData('drop', $reward);
        ret::setSuccess();
    }

    /**
     * 兑换商店，领取全局奖励
     * @param $args
     */
    function EMGlobalReward($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }

        $id = intval($args->id);
        if ($id <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }

        $phase = clsExchangePhase::getInstance($id);
        if (!$phase) {
            ret::throwError(INVALID_ARGUMENT);
        }

        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        $exchange = clsExchange::get($activityId, $this->iUserId);
        $reward = $exchange->drawGlobalReward($phase);
        ret::setData('drops', $reward);
        ret::setData('EMGReward', $exchange->globalReward);
        ret::setSuccess();
    }

    /** 领取累计充值奖励
     * @param $args
     */
    function getTotalCharge($args)
    {
        $activityId = intval($args->aid);
        $id = intval($args->id);
        if ($id <= 0 || $activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }

        $meta = clsTotalChargeAwardMeta::getTimed($id);
        if (!$meta) {
            ret::throwError(INVALID_ARGUMENT);
        }

        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $totalCharge = clsTotalCharge::get($this->iUserId, $activityId);
        $rewards = $totalCharge->getReward($meta);
        ret::setData('drops', $rewards);
        ret::setSuccess();
    }

    /**
     * 领取7天累充奖励
     * @param $args
     */
    function rookieTChargeReward($args)
    {
        $id = intval($args->id);
        if ($id <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }

        $meta = clsTotalChargeAwardMeta::get($id);
        if (!$meta) {
            ret::throwError(INVALID_ARGUMENT);
        }

        if (!clsRookieTotalCharge::isAvailable($this->iUserId)) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        $rookieTotalCharge = clsRookieTotalCharge::get($this->iUserId);
        ret::setData('drops', $rookieTotalCharge->getReward($meta));
        ret::setSuccess();
    }

    function getMC()
    {
        $monthcards = clsMonthCard::getInstance($this->iUserId)->toClientCardsObj();
        ret::setData('monthcards', $monthcards);
        ret::setSuccess();
    }

    function getMCAward($args)
    {
        $id = intval($args->index);
        $meta = clsMonthCardMeta::get($id);
        if (!$meta) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $monthcards = clsMonthCard::getInstance($this->iUserId);
        $monthcards->getAward($meta);
        ret::setSuccess();
    }


    function getMCTotalCharge($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        ret::setData('mcTotalCharge', clsMonthCardTotalCharge::get($this->iUserId, $activityId)->toClientObj());
        ret::setSuccess();
    }

    function getMCTotalChargeAward($args)
    {
        $activity_id = intval($args->aid);
        $id = intval($args->id);
        if ($id <= 0 || $activity_id <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activity_id);
        if (is_null($activity) || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $meta = clsMonthCardTotalChargeMeta::get($id);
        if (!$meta) {
            ret::throwError(INVALID_ARGUMENT);
        }
        if (!clsMonthCard::getInstance($this->iUserId)->isMonthCardUser()) {
            ret::throwError(NOT_MONTH_CARD_USER);
        }
        $clsMonthCardCharge = clsMonthCardTotalCharge::get($this->iUserId, $activity_id);
        $out = $clsMonthCardCharge->getReward($meta);
        ret::setData('mcTotalCharge', $clsMonthCardCharge->toClientObj());
        ret::setData('drop', $out);
        ret::setSuccess();
    }

    function getRiddles($args)
    {
        $riddle = clsRiddles::getInstance($this->iUserId);
        $riddle->init();
        ret::setData('riddle', $riddle->toClientRiddleObj());
        ret::setSuccess();
    }

    function guessRiddle($args)
    {
        $answer = intval($args->answer);
        if ($answer < 1 || $answer > 4) {
            ret::throwError(INVALID_ARGUMENT);
        }
        clsRiddles::getInstance($this->iUserId)->guess($answer);

        ret::setSuccess();
    }

    /**
     *  好友邀请-分享
     */
    function getShare($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_REQUEST_SHARE || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $shareTs = intval($args->ts);
        $sign = $args->sign;
        if(strcasecmp($sign,md5($this->iUserId.$shareTs.clsUserRequestShare::KEY)) != 0){
            ret::throwError(INVALID_ARGUMENT);
        }
        $share = clsUserRequestShare::get($this->iUserId);
        $rf = clsUserRequestShareMeta::get();
//        if( $shareTs - $share->ts <= $rf->shareCd || $share->ts > $shareTs ){
//            ret::throwError(TODAY_SHARE_NOT_TO_TIME);
//        }
        $share->share( $shareTs );
        ret::setSuccess();
    }
    
    /**
     *  好友邀请-分享领取奖励
     */
    function share($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_REQUEST_SHARE || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $share = clsUserRequestShare::get($this->iUserId);
        if( $share->ts == 0 || $share->share_award == 0 ){
            ret::throwError(TODAY_SHARE_ALREADY_GIVE);
        }
        $gifts = $share->getShareGift();
        ret::setData('drops',$gifts);
        ret::setSuccess();
    }
    
    /**
     *  好友邀请-用户邀请
     */
    function getUserRequest($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_REQUEST_SHARE || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $requestId = $args->requestId;
        sort($requestId);
        $strRequestId = '';
        foreach( $requestId as $k => $v ){
            $strRequestId .= $v;
        }
        $sign = $args->sign;
        if(strcasecmp($sign,md5($this->iUserId.$strRequestId.clsUserRequestShare::KEY)) != 0){
            ret::throwError(INVALID_ARGUMENT);
        }
        $requestId = array_unique($requestId);
        $arrUseRequest = clsUserRequest::searchFromDB($this->iUserId,$requestId);
        $arrUseRequest = array_flip($arrUseRequest);
        $i = 0;
        foreach($requestId as $k => $v){
            if(!array_key_exists($v, $arrUseRequest) ){
                (new clsUserRequest($this->iUserId, $v, TIME_NOW))->save();
                $i++;
            }
        }
        ret::setData('userRequestNum',clsUserRequest::getCount($this->iUserId));
        ret::setSuccess();
    }
    
    /**
     *  好友邀请-邀请领奖
     */
    function userRequest($args)
    {
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_REQUEST_SHARE || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $num = intval($args->num);
        $requestCount = clsUserRequest::getCount($this->iUserId);
        $rf = clsUserRequestShareMeta::get();
        if( $requestCount < $num || !array_key_exists($num, $rf->requestAwards)){
            ret::throwError(INVALID_ARGUMENT);
        }
        $share = clsUserRequestShare::get($this->iUserId);
        $arrAwards = explode(',', $share->request_award);
        foreach($arrAwards as $k => $v){
            if($v == $num)
                ret::throwError(USER_REQUEST_ALREADY_GIVE_REWARD);
        }
        $gifts = $share->getRequestGift($num);
        ret::setData('drops',$gifts);
        ret::setSuccess();
    }
    
    function sprintLevel($args){
        $id = intval($args->id);
        $meta = clsSprintLevelAwardMeta::get($id);
        if (!$meta) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $objSprintLevel  = clsSprintLevel::get($this->iUserId);
        ret::setData('drops', $objSprintLevel->getReward($meta));
        ret::setSuccess();
    }
    
    function limitWarriorRank($args){
        $num = intval($args->num);
        if( $num !== 50 ){
            ret::throwError(INVALID_ARGUMENT);
        }
        $activityId = clsLimitTimeDrawCard::getActivityId();
        if($activityId <= 0)
            ret::throwError(INVALID_ARGUMENT);
        
        ret::setData('LimitWarriorRank', clsLimitTimeDrawCard::rank($activityId, $num));
        ret::setSuccess();
    }
    
    function getOnlineGift($args){
        $act = clsActivity::getInstance($this->iUserId);
        $act->updateOnlineGift();
        ret::setData('onlineGift', $act->toClientOnlineGift());
        ret::setSuccess();
    }
    
    function onlineGift($args){
        $act = clsActivity::getInstance($this->iUserId);
        $onlineGiftMeta = clsOnlineGiftMeta::get( $act->onlineGift + 1 );
        if( ! $onlineGiftMeta ){
            ret::throwError(INVALID_ARGUMENT);
        }
        if( TIME_NOW - $act->onlineGiftTs < $onlineGiftMeta->intervalTime ){
            ret::throwError(ONLINE_GIFT_NOT_IN_TIME);
        }
        $drops = array();
        foreach( $onlineGiftMeta->awards as $k => $v ){
            $drops[] = $v->addToUser($this->iUserId,SYS_ONLINE_GIFT);
        }
        $act->onlineGift += 1;
        $act->updateOnlineGift( TIME_NOW );
        
        ret::setData('drops', $drops);
        ret::setData('onlineGift', $act->toClientOnlineGift());
        ret::setSuccess();
    }

    function kfChargeReturn($args){
        $u = clsUser::get($this->iUserId);
        if(!$u){
            ret::throwError(INVALID_ARGUMENT);
        }

        $tmp = clsKFChargeReturn::get($u->pid);
        if(!$tmp){
            ret::throwError(INVALID_ARGUMENT);
        }

        if($tmp->state == 1){
            ret::throwError(INVALID_ARGUMENT);
        }

        $userMoney = clsUserMoney::get($this->iUserId);
        $userMoney->tpoints += $tmp->vip_points;//VIP
        $userMoney->points += $tmp->total_points;//元宝

        $vip = clsVipMeta::getVipLvByCharge($userMoney->tpoints);
        $userMoney->addVipValue($vip, SYS_KF_CHARGE_RETURN, $logs);
        clsLog::saveLogs($logs);
        $userMoney->update(true);

        //保存
        $tmp->save();

        $monthCard = clsMonthCard::getInstance($this->iUserId);
        $obj = $u->toClientObj($userMoney,$monthCard);

        $obj->point-=$tmp->total_points;
        ret::setData('user', $obj);

        $drops = array();
        $drops[] = clsGoods::create(clsGoods::TYPE_BONUS, 0, $tmp->total_points)->toClientObj();
        ret::setData('drops', $drops);
    }

    function kfLevelReturn($args){
        $u = clsUser::get($this->iUserId);
        if(!$u){
            ret::throwError(INVALID_ARGUMENT);
        }

        $tmp = clsKFLevelReturn::get($u->pid);
        if(!$tmp){
            ret::throwError(INVALID_ARGUMENT);
        }

        if($tmp->state == 1){
            ret::throwError(INVALID_ARGUMENT);
        }

        //确认该弟子玩家是否拥有
        $bIn = clsWarriorPackage::getInstance($this->iUserId)->getWarrior(clsKFLevelReturn::WID);
        if($bIn)
        {
            $drops = array();
            clsSoulPackage::getInstance($this->iUserId)->addSoul(new clsSoul(clsKFLevelReturn::WID, 30));
            $drops[] = clsGoods::create(clsGoods::TYPE_SOUL, clsKFLevelReturn::WID, 30)->toClientObj();
            ret::setData('drops', $drops);

        }else{

            $warrior = new clsWarrior(clsKFLevelReturn::WID);
            clsWarriorPackage::getInstance($this->iUserId)->addWarrior($warrior);
            $drops = array();
            $drops[] = clsGoods::create(clsGoods::TYPE_WARRIOR, clsKFLevelReturn::WID, 1)->toClientObj();
            ret::setData('drops', $drops);
        }
        //保存
        $tmp->save();
    }
    
    /**
     * 月卡商城列表
     */
    function monthCardMall($args){
        $activityId = intval($args->aid);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_MONTH_CARD_MALL || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        ret::setData('mall_list', clsMonthCardMall::getList($this->iUserId, $activityId));
        ret::setSuccess();
    }

    /**
     * 月卡商城道具购买
     */
    function monthCardMallBuy($args)
    {    
        if( !clsMonthCard::getInstance($this->iUserId)->isMonthCardUser() ){
            ret::throwError(MONTH_CARD_OUT_OF_DATE);
        }
        $activityId = intval($args->aid);
        $mMId = intval($args->id);
        if ($activityId <= 0) {
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_MONTH_CARD_MALL || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $objMCMallMeta = clsMonthCardMallMeta::get($mMId);
        if( !$objMCMallMeta ){
            ret::throwError(INVALID_ARGUMENT);
        }
        $objMCMall = clsMonthCardMall::get($this->iUserId, $activityId, $mMId);
        if( ! $objMCMall )
            $objMCMall = new clsMonthCardMall ($this->iUserId, $activityId, $mMId);
        if( $objMCMallMeta->buyTimes == 0 || $objMCMallMeta->buyTimes <= $objMCMall->buyTimes ){
            ret::throwError(MONTH_CARD_NO_BY_TIMES);
        }
        $product = clsProduct::getInfo($objMCMallMeta->productId);
        $money = clsUserMoney::get($this->iUserId);
        if ($money->getTotal() < $product->unit_price) {
            ret::throwError(POINT_NOT_ENOUGH);
        }
        clsMall::buy($this->iUserId, $product, 1, SYS_MONTH_CARD_MALL);
        $objMCMall->buyTimes += 1;
        $objMCMall->save();
        ret::setData('mall_list', clsMonthCardMall::getList($this->iUserId, $activityId));
        ret::setSuccess();
    }
    
    /**
     * 特殊签到签到
     */
    function specialSignIn($args)
    {
        $activityId = intval($args->aid);
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_SPECIAL_SIGN_IN || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $act = clsActivity::getInstance($this->iUserId);
        $act->specialSignIn($activity->startTs);
        /*
         * 针对新春签到加入一个掉落库
         */
        $loot = clsLoot::noneSpRoll(400000);
        if($loot)
            $drop = $loot->addToUser($this->iUserId, SYS_SPECIAL_SIGN_IN);
        ret::setData('drop', $drop);
        clsLog::saveLog(new clsLog(clsLog::CATEGORY_NORMAL, $this->iUserId, '特殊签到', '', LOG_OP_ACTIVITY_SPECIAL_SIGN_IN_AWARD));
        ret::setData('specialSignIn', $activity->toClientObj($this->iUserId));
        ret::setSuccess();
    }
    
    /**
     * 获取特殊签到奖励
     */
    function getSpecialSignInAward($args){
        $activityId = intval($args->aid);
        $times = intval($args->times);
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_SPECIAL_SIGN_IN || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $act = clsActivity::getInstance($this->iUserId);
        if($act->specialSignInTimes < $times){
            ret::throwError(SPECIAL_SIGN_IN_NOT_GIVE_REWARD);
        }
        $act->getSpecialSignIn($times);
        
        ret::setData('specialSignIn', $activity->toClientObj($this->iUserId));
        ret::setSuccess();
    }
    
    function calendarSign($args)
    {
        if (!clsCalendar::isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        $id = intval($args->id);
        if ($id <= 0) ret::throwError(INVALID_ARGUMENT);

        $month = date('Ym', TIME_NOW);
        $meta = clsCalendarMeta::get($month, $id);

        if (!$meta) ret::throwError(INVALID_ARGUMENT);

        if ($meta->date > TIME_TODAY) {
            ret::throwError(CALENDAR_FUTURE);
        }

        $calendar = clsCalendar::get($this->iUserId);
        if ($meta->date < TIME_TODAY) {
            $loot = $calendar->reSign($meta);
        } else {
            $loot = $calendar->sign($meta);
        }

        ret::setData('drop', $loot);
        ret::setSuccess();
    }
    
    /*
     * 初始化成长基金
     */
    function initGrowthFund(){
        $creatTime = 0;
        $regDate = clsUser::get($this->iUserId)->reg_ts;
        if( $regDate >  strtotime('2015-01-14')){
            $creatTime = $regDate;
        }
        else{
            $creatTime = TIME_NOW;
        }
        foreach(clsGrowthFund::$arrType as $k => $v){
            if(!clsGrowthFund::get($this->iUserId, $k)){
                $growthFund = new clsGrowthFund($this->iUserId, $k, 0, 0, $creatTime);
                $growthFund->save();
            }
        }
    }
    
    /**
     * 成长基金
     */
    function buyGrowthFund($args)
    {
        $buyType = intval($args->buyType);
        if (!clsGrowthFund::isAvailable($this->iUserId)) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        if(!in_array($buyType, array(1,2))){
            ret::throwError(INVALID_ARGUMENT);
        }
        
        if(!clsGrowthFund::isCanBuy($this->iUserId, $buyType)){
            ret::throwError(VIP_NOT_ENOUGH);
        }
        
        $growthFund = clsGrowthFund::get($this->iUserId, $buyType);
        if(!$growthFund || $growthFund->regDate <= 0)
            ret::throwError(INVALID_ARGUMENT);
        
        if($growthFund->buyTime > 0)
            ret::throwError(ALREADY_BUY_PRODUCT);

        $product = clsProduct::getInfo(clsGrowthFund::$arrType[$buyType]);
        $money = clsUserMoney::get($this->iUserId);
        if ($money->getTotal() < $product->unit_price) {
            ret::throwError(USER_POINT_NOT_ENOUGH);
        }
        clsMall::buy($this->iUserId, $product, 1, SYS_GROWTH_FUND);
        $growthFund->buyTime = TIME_NOW;
        $growthFund->save();

        ret::setData('growthFund', $growthFund->toClientObj());
        clsLog::saveLog(new clsLog(clsLog::CATEGORY_NORMAL, $this->iUserId, '购买成长基金', '', LOG_OP_ACTIVITY_GROWTH_FUND_AWARD));
        ret::setSuccess();
    }
    
    /**
     * 获取成长基金奖励
     */
    function getGrowthFundAward($args){
        $buyType = intval($args->buyType);
        $id = intval($args->id);
        if(!in_array($buyType, array(1,2))){
            ret::throwError(INVALID_ARGUMENT);
        }
        $growthFund = clsGrowthFund::get($this->iUserId, $buyType);
        if(!$growthFund || $growthFund->buyTime <= 0)
            ret::throwError(NOT_BUY_PRODUCT);
        
        $growthFundMeta = clsGrowthFundMeta::get($buyType, $id);
        if(!$growthFundMeta)
            ret::throwError(INVALID_ARGUMENT);

        ret::setData('drop', $growthFund->getReward($growthFundMeta, clsUser::get($this->iUserId)->lv));
        ret::setData('growthFund', $growthFund->toClientObj());
        ret::setSuccess();
    }
    
    /**
     * 奇人密码信息
     */
    function getPiliMovie($args){
        $piliMovie = clsPiliMovie::get($this->iUserId);
        ret::setData('piliMovie', $piliMovie->toClientObj());
        ret::setSuccess();
    }
    
    /**
     * 领取奇人密码奖励
     */
    function getPiliMovieAward($args){
        $piliMovie = clsPiliMovie::get($this->iUserId);
        $drop = $piliMovie->getReward();
        ret::setData('drop', $drop);
        ret::setSuccess();
    }
    
    /**
     * 收集抽奖兑换道具-新春快乐
     */
    function collectLotteryDrawExchange($args){
        $activityId = intval($args->aid);
        $item1 = intval($args->item1);
        $item2 = intval($args->item2);
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_COLLECT_LOTTERY_DRAW || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        if(!in_array($item1, clsCollectLotteryDraw::$ITEMS) || !in_array($item2, clsCollectLotteryDraw::$ITEMS)){
            ret::throwError(INVALID_ARGUMENT);
        }
        $collectLotteryDraw = clsCollectLotteryDraw::get($activityId, $this->iUserId, TIME_TODAY);
        $collectLotteryDraw->exchange($item1, $item2);
        ret::setSuccess();
    }
    
    /**
     * 收集抽奖抽奖-新春快乐
     */
    function collectLotteryDraw($args){
        $activityId = intval($args->aid);
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_COLLECT_LOTTERY_DRAW || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $collectLotteryDraw = clsCollectLotteryDraw::get($activityId, $this->iUserId, TIME_TODAY);
        $drop = $collectLotteryDraw->getReward();

        foreach( $collectLotteryDraw->toClientObj() as $k =>$v )
            ret::setData($k, $v);
        ret::setData('drop', $drop);
        ret::setSuccess();
    }
    
    /**
     * 收集抽奖排行-新春快乐
     */
    function collectLotteryDrawRank($args){
        $activityId = intval($args->aid);
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_COLLECT_LOTTERY_DRAW || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $rank = clsCollectLotteryDraw::getRank($activityId, TIME_TODAY);
        ret::setData('rank', $rank);
        ret::setSuccess();
    }
    
    /**
     * 喂财神兑换-元宵活动
     */
    function feedWealthGodExchange($args){
        $activityId = intval($args->aid);
        $itemId = intval($args->itemId);
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_FEED_WEALTH_GOD || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        if(!in_array($itemId, clsFeedWealthGod::$ITEMS) || $itemId == clsFeedWealthGod::$ITEMS[4]){
            ret::throwError(INVALID_ARGUMENT);
        }
        $feedWealthGod = clsFeedWealthGod::get($activityId, $this->iUserId);
        $feedWealthGod->exchange($itemId);
        ret::setSuccess();
    }
    
    /**
     * 喂财神-元宵活动
     */
    function feedWealthGod($args){
        $activityId = intval($args->aid);
        $itemId = intval($args->itemId);
        $chk = (boolean)$args->chk;
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_FEED_WEALTH_GOD || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        if(!in_array($itemId, clsFeedWealthGod::$ITEMS)){
            ret::throwError(INVALID_ARGUMENT);
        }
        $feedWealthGodMeta = clsFeedWealthGodMeta::get($itemId);
        $item = clsItemPackage::getInstance($this->iUserId)->getItem($itemId);
        if($feedWealthGodMeta->useNum > $item->num){
            ret::throwError(TRIGGER_NOT_ENOUGH);
        }
        $feedWealthGod = clsFeedWealthGod::get($activityId, $this->iUserId);
        $allItems = array_flip(clsFeedWealthGod::$ITEMS);
        if($feedWealthGodMeta->evenydayNum != 0 && $feedWealthGod->data[$allItems[$itemId]] + 1 > $feedWealthGodMeta->evenydayNum){
            ret::throwError(TODAY_NUMBER_OF_FULL);
        }
        if(!$chk){
            $sign = $args->sign;
            $b = intval($args->b);
            $c = intval($args->c);
            $i = intval($args->i);
            if($b > $feedWealthGodMeta->bonus || $c > $feedWealthGodMeta->coins || $i > $feedWealthGodMeta->items){
                ret::throwError(INVALID_ARGUMENT);
            }
            if(strtoupper($sign) != strtoupper(MD5($this->iUserId.$activityId.$b.$c.$i.$itemId.$feedWealthGod->rand.clsUserRequestShare::KEY))){
                ret::throwError(INVALID_SIGN);
            }
            $drops = $feedWealthGod->getReward($feedWealthGodMeta, $b, $c, $i);
            
            foreach( $feedWealthGod->toClientObj() as $k =>$v )
                ret::setData($k, $v);
            ret::setData('drops', $drops);
        }
        else{
            ret::setData('signNum', $feedWealthGod->getRand());
        }
        ret::setSuccess();
    }
    
    /**
     * 喂财神排行-元宵活动
     */
    function feedWealthGodRank($args){
        $activityId = intval($args->aid);
        $num = intval($args->num);
        $limit = intval($args->limit);
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_FEED_WEALTH_GOD || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $rank = clsFeedWealthGod::getRank($activityId, $num, $limit);
        ret::setData('rank', $rank);
        ret::setSuccess();
    }

    /**
     * 新春充值
     * @param $args
     */
    function getNewYearChargeAward($args){
        $activityId = intval($args->aid);//活动ID
        $score = intval($args->score);//领取的积分档次

        //检查活动是否存在，活动时间是否正确
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_NEW_YEAR_CHARGE || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        //获取CONFIGID
        $allRewards = clsNewYearChargeAwardMeta::getAllTimedRewards();
        $configId = 0;
        foreach($allRewards as $k => $v){
            if($k == $score){
                $configId = $v;
                break;
            }
        }

        //参数错误
        if($configId  == 0){
            ret::throwError(INVALID_ARGUMENT);
        }

        $new_year = clsNewyearCharge::get($activity->id, $this->iUserId);

        if($new_year->getScore() < $score){
            ret::throwError(INVALID_ARGUMENT);
        }

        if($new_year->reward_record & (1 << $configId)){
            ret::throwError(INVALID_ARGUMENT);
        }
        //设置领取标示
        $new_year->reward_record = $new_year->reward_record | (1 << $configId);

        $award = clsNewYearChargeAwardMeta::get($configId);
        //添加道具
        $drops = array();
        foreach($award->reward as $item){
            $drops[] = $item->addToUser($this->iUserId,SYS_NEW_YEAR_CHARGE);
        }
        //保存
        $new_year->save();

        ret::setData('drops', $drops);

    }

    function lottery($args){
        $activityId = intval($args->aid);//活动ID
        $number = intval($args->number);//抽奖次数

        //检查活动是否存在，活动时间是否正确
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_CONSUME_LOTTERY || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        $lotteryInfo = clsConsumeLottery::get($activity->id, $this->iUserId);
        if($lotteryInfo->getNumber() < $number){
            ret::throwError(INVALID_ARGUMENT);
        }

        $droptemp = array();
        for($i = 0; $i < $number; ++$i){
            $lotteryInfo->dropItem($droptemp);
        }

        $lotteryInfo->number += $number;
        $lotteryInfo->save();

        ret::setData('number', $lotteryInfo->getNumber());
        ret::setData('drops', $droptemp);

    }

    function getLotteryInfo($args){
        $activityId = intval($args->aid);//活动ID

        //检查活动是否存在，活动时间是否正确
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_CONSUME_LOTTERY || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }

        $lotteryInfo = clsConsumeLottery::get($activity->id,$this->iUserId);
        ret::setData('lotteryInfo', $lotteryInfo->toClientObj());
    }
    
    /**
     * 去死去死团逆袭，干死情侣活动
     */
    function killLovers($args){
        ret::hasBin();
        $activityId = intval($args->aid);
        $id = intval($args->id);
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_KILL_LOVERS || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $killLoversMeta = clsKillLoversMeta::get($id);
        if(!$killLoversMeta){
            ret::throwError(INVALID_ARGUMENT);
        }
        $killLovers = clsKillLovers::get($activityId, $this->iUserId);
        $killLovers->init();
        
        if(!$killLovers->isKill($killLoversMeta)){
            ret::throwError(KILL_LOVERS_TIMES_FINISH);
        }
        $report = $killLovers->challeng($killLoversMeta);
        ret::setBin($report->getContent());
        ret::setSuccess();
    }
    
    /**
     * 福袋-新年福袋
     */
    function luckyPocket($args){
        $activityId = intval($args->aid);
        $num = intval($args->num);
        if(!in_array($num, array(1, 10))){
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_LUCKY_POCKET || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $luckyPocket = clsLuckyPocket::get($activityId, $this->iUserId);
        ret::setData('drops', $luckyPocket->draw($num));
        ret::setSuccess();
    }
    
    /**
     * 答题-元宵答题
     */
    function answer($args){
        $activityId = intval($args->aid);
        $qId = intval($args->qId);
        $rightId = intval($args->rightId);
        if(!in_array($qId, array(1, 2, 3, 4, 5))){
            ret::throwError(INVALID_ARGUMENT);
        }
        if(!in_array($rightId, array(1, 2, 3, 4))){
            ret::throwError(INVALID_ARGUMENT);
        }
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_ANSWER || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $answer = clsAnswer::get($activityId, $this->iUserId, TIME_TODAY);
        $answer->reply($qId, $rightId);
        ret::setSuccess();
    }
    
    /**
     * 答题领奖-元宵答题
     */
    function answerAward($args){
        $activityId = intval($args->aid);
        $activity = clsTimedActivity::get($activityId);
        if (is_null($activity) || $activity->type != clsActivity::TYPE_ANSWER || !$activity->isAvailable()) {
            ret::throwError(ACTIVITY_NOT_AVAILABLE);
        }
        $answer = clsAnswer::get($activityId, $this->iUserId, TIME_TODAY);
        ret::setData('drops', $answer->getReward());
        ret::setSuccess();
    }
}
