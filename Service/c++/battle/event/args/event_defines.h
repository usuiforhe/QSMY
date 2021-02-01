//
//  event_defines.h
//  battle
//
//  Created by wangyanjun on 13-3-15.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef battle_event_defines_h
#define battle_event_defines_h

//--------------------- 场景事件 ---------------------

#define SCENE_INIT                  1001        //场景初始化

//--------------------- 势力事件 ---------------------

#define FORCE_INIT                  2001        //势力初始化
#define FORCE_BEFORE_DIE            2002        //势力死亡前
#define FORCE_AFTER_DIE             2003        //势力死亡后

//--------------------- 玩家事件 ---------------------


#define USER_INIT                   3001        //玩家初始化
#define USER_BEFORE_DIE             3002        //玩家死亡前
#define USER_AFTER_DIE              3003        //玩家死亡后


//--------------------- 单位事件 ---------------------

#define UNIT_INIT                   4001        //单位初始化
#define UNIT_MAIN_ACTION_INIT       4002        //单位主行动初始化
#define UNIT_MAIN_ACTION_END        4003        //单位主行动结束
#define UNIT_BEFORE_ATTACK          4004        //单位攻击前
#define UNIT_AFTER_ATTACK           4005        //单位攻击后
#define UNIT_BEFORE_BE_ATTACKED     4006        //单位被攻击前
#define UNIT_AFTER_BE_ATTACKED      4007        //单位被攻击后
#define UNIT_BEFORE_SKILL           4008        //单位释放技能前
#define UNIT_AFTER_SKILL            4009        //单位释放技能后
#define UNIT_BEFORE_DAMAGE          4010        //单位受到伤害前
#define UNIT_AFTER_DAMAGE           4011        //单位受到伤害后
#define UNIT_BEFORE_DIE             4012        //单位死亡前
#define UNIT_AFTER_DIE              4013        //单位死亡后
#define UNIT_KILL                   4014        //单位击杀
#define UNIT_BEFORE_BUFF_ADDED      4015        //单位buff被添加前
#define UNIT_AFTER_BUFF_ADDED       4016        //单位buff被添加后
#define UNIT_BEFORE_BUFF_REMOVED    4017        //单位buff被移除前
#define UNIT_AFTER_BUFF_REMOVED     4018        //单位buff被移除后

typedef enum unit_attack_type{
    //普通攻击
    UNIT_ATTACK_NORMAL = 1,
    //格挡攻击
    UNIT_ATTACK_BLOCK = 2
} unit_attack_type;

#endif
