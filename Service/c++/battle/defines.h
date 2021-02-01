//
//  defines.h
//  battle
//
//  Created by wangyanjun on 13-3-28.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef battle_defines_h
#define battle_defines_h

namespace battle{

//-----------------------常量定义-----------------------
//左侧势力编号
#define LEFT_FORCE_ID 1
//右侧势力编号
#define RIGHT_FORCE_ID 2
//电脑玩家编号
#define MOB_USER_ID 0
//暴击伤害系数
#define CRITICAL_MULTIPLIER 1.5
//格挡伤害系数
#define BLOCK_MULTIPLIER 0.3
//格挡伤害系数
#define BLOCK_DMG_MULTIPLIER 0.6
//最大行动次数
#define MAX_ACTION_TIMES 100
//3星最低百分比
#define THREE_STAR_THRESHOLD 2.0/3
//2星最小百分比
#define TWO_STAR_THRESHOLD 1.0/3

//-----------------------常用定义---------------------------
#define GETTER(type, name)  type get_##name(){ return name##_; }
#define CONST_GETTER(type, name)  type get_##name() const { return name##_; }
#define SETTER(type, name)  void set_##name(type name){ name##_ = name; }
    
}

#endif
