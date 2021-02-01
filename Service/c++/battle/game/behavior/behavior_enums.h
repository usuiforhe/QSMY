//
//  behavior_enums.h
//  battle
//
//  Created by wangyanjun on 13-3-28.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef battle_behavior_enums_h
#define battle_behavior_enums_h

namespace battle {
    typedef enum behavior_type{
        BEHAVIOR_ATTACK = 0x01,
        BEHAVIOR_SKILL = 0x02,
        BEHAVIOR_BUFF_EFFECT = 0x03
    } behavior_type;
    
    typedef enum behavior_result_type{
        BEHAVIOR_RESULT_DAMAGE = 0x01,
        BEHAVIOR_RESULT_ADD_BUFF = 0x02,
        BEHAVIOR_RESULT_REMOVE_BUFF = 0x03,
        BEHAVIOR_RESULT_HEAL = 0x04,
        BEHAVIOR_RESULT_SHOW = 0x05,
        BEHAVIOR_RESULT_HIDE = 0x06,
        BEHAVIOR_RESULT_DIALOG = 0x07
    } behavior_result_type;
}

#endif
