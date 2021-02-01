<?php
/**
 * yuyang
 */

class clsAnswerMeta
{
    /**
     * @var 编号
     */
    var $id;
    /**
     * @var 正确答案
     */
    var $rightId;
    /**
     * @var 题目
     */
    var $question;
    /**
     * @var 答案1
     */
    var $a1;
    /**
     * @var 答案2
     */
    var $a2;
    /**
     * @var 答案3
     */
    var $a3;
    /**
     * @var 答案4
     */
    var $a4;

    /**
     * 构造函数
     */
    function __construct($id, $rightId, $question, $a1, $a2, $a3, $a4)
    {
        $this->id = $id;
        $this->rightId = $rightId;
        $this->question = $question;
        $this->a1 = $a1;
        $this->a2 = $a2;
        $this->a3 = $a3;
        $this->a4 = $a4;
    }

    /**
     * @param $id
     * @return clsAnswerMeta
     */
    static function get($id)
    {
        $key = 'g_answer_' . $id;
        if (!array_key_exists($key, $GLOBALS)) {
            include(INFO_PATH . 'activity/answer/'.$id.'.php');
        }
        return $GLOBALS[$key];
    }
}