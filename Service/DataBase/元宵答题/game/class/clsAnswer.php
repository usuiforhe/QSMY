<?php

/**
 * @author yuyang
 */
class clsAnswer {
    
    const REDIS_NAMESPACE = 'ANSWER';

    /*
     * ID
     */
    var $id;
    /*
     * 活动ID
     */
    var $activityId;
    /*
     * 用户ID
     */
    var $userId;
    /*
     * 答题内容
     */
    var $answer;
    /*
     * 是否已领奖
     */
    var $isAward;
    /*
     * 创建时间
     */
    var $createTs;
    /*
     * 更新时间
     */
    var $updateTs;
    
    const LAST_ANSWER_DATE = 70800;
    const END_DATE = 71400;
    
    static $replyDate = array(
        array(68400, 69060),
        array(69000, 69660),
        array(69600, 70260),
        array(70200, 70860),
        array(70800, 71460),
    );
    
        //以下测试数据
//    const LAST_ANSWER_DATE = 68640;
//    const END_DATE = 68700;
//    
//    static $replyDate = array(
//        array(68400, 68460),
//        array(68460, 68520),
//        array(68520, 68580),
//        array(68580, 68640),
//        array(68640, 68700),
//    );

    function __construct( $activityId, $userId, $answer = '', $isAward = 0, $create_ts = 0, $update_ts = 0, $id = 0 )
    {
        $this->activityId = $activityId;
        $this->userId = $userId;
        $this->answer = $answer;
        $this->isAward = $isAward;
        $this->createTs = $create_ts;
        $this->updateTs = $update_ts;
        $this->id = $id;
    }

    function toClientObj()
    {
        $obj = new stdClass();
        $obj->answer = $this->answer;
        $obj->isAward = $this->isAward;
        return $obj;
    }

    function getReward()
    {
        if(TIME_NOW < TIME_TODAY + self::LAST_ANSWER_DATE || (TIME_NOW < TIME_TODAY + self::END_DATE && $this->answer[count(self::$replyDate) - 1] == 0)){//不是最后一道题目的答题时间
            ret::throwError(ANSWER_NOT_GET_AWARD);
        }
        
        if($this->isAward == 1){
            ret::throwError(ANSWER_ALREADY_GET_AWARD);
        }
        $awardList = self::getAnswerAward();
        $out = array();
        foreach($this->answer as $k => $v){
            if($v->state == 1){
                $out[] = $awardList[$k + 1]->addToUser($this->userId, SYS_ANSWER);
            }
        }
        $this->isAward = 1;
        $this->updateTs = TIME_NOW;
        $this->save();
        return $out;
    }
    
    function reply($qId, $rightId){
        if(TIME_NOW < clsAnswer::$replyDate[$qId - 1][0] + TIME_TODAY || TIME_NOW > clsAnswer::$replyDate[$qId - 1][1] + TIME_TODAY){
            ret::throwError(ANSWER_IS_NOT_IN_DATE);
        }
        if($this->answer[$qId - 1]->state != 0){
            ret::throwError(ANSWER_ALREADY_END);
        }
        $answer = clsAnswerMeta::get($this->answer[$qId - 1]->id);
        $answer->rightId == $rightId ? $this->answer[$qId - 1]->state = 1 : $this->answer[$qId - 1]->state = 2;
        $this->updateTs = TIME_NOW;
        $this->save();
    }
    
    function save( )
    {
        $this->saveDb( );
        c_redis_db::set(
            self::REDIS_NAMESPACE,
            $this->activityId . '_' . $this->userId . '_' . $this->createTs,
            $this
        );
    }

    private function saveDb( )
    {
        if ($this->id > 0) {
            $sql = 'UPDATE P_USER_ANSWER
                SET answer = \'' . addslashes(json_encode($this->answer)) . '\',
                    update_ts = ' . $this->updateTs . '
                WHERE `id` = ' . $this->id . ';';
            $updater = new clsAsyncUpdater(self::REDIS_NAMESPACE, $this->userId, $sql);
            $updater->update();
        }
        else {
            $this->id = c_mysql_db::DBConnect()->insert(
                'INSERT INTO P_USER_ANSWER
                (activity_id, user_id, answer, create_ts, update_ts)
                VALUES (' . $this->activityId
                    . ',' . $this->userId
                    . ',\'' . addslashes(json_encode($this->answer))
                    . '\',' . $this->createTs
                    . ',' . $this->updateTs.');'
            );
        }
    }

    /**
     * @param $activity
     * @param $userId
     * @return clsAnswer
     */
    static function get($activity, $userId, $date)
    {
        return c_redis_db::get(
            self::REDIS_NAMESPACE,
            $activity . '_' . $userId . '_' . $date,
            function () use ($activity, $userId, $date) {
                return clsAnswer::getFromDb($activity, $userId, $date);
            }
        );
    }

    static function getFromDb($activity, $userId, $date)
    {
        $row = c_mysql_db::DBConnect()->getRow('SELECT `id`, activity_id, user_id, answer, is_award, create_ts, update_ts
            FROM P_USER_ANSWER WHERE user_id = ' . $userId . ' and activity_id=' . $activity . ' and create_ts=' . $date);
        if (!$row) {
            $getAnswers = array();
            $answers = self::getAnswerConfig();
            foreach(array_rand(array_flip($answers),5) as $k => $v){
                $tmp = new stdClass();
                $tmp->id = $v;
                $tmp->state = 0;
                $getAnswers[] = $tmp;
            }
            $answer = new clsAnswer(
                    $activity,
                    $userId,
                    $getAnswers,
                    0,
                    $date,
                    TIME_NOW
            );
            $answer->save();
            return $answer;
        }
        else {
            return new clsAnswer(
                intval($row['activity_id']),
                intval($row['user_id']),
                json_decode($row['answer']),
                intval($row['is_award']),
                intval($row['create_ts']),
                intval($row['update_ts']),
                intval($row['id'])
            );
        }
    }
    
    /**
     * @return array
     */
    static function getAnswerConfig()
    {
        $key = 'g_answer_config';
        if (!array_key_exists($key, $GLOBALS)) {
            include(INFO_PATH . 'activity/answer/config.php');
        }
        return $GLOBALS[$key];
    }
    
    /**
     * @return array
     */
    static function getAnswerAward()
    {
        $key = 'g_answer_award';
        if (!array_key_exists($key, $GLOBALS)) {
            include(INFO_PATH . 'activity/answer/award.php');
        }
        return $GLOBALS[$key];
    }
}
