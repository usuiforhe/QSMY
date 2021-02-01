//
//  bot_name.h
//  battle
//
//  Created by 汪燕军 on 13-7-8.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef battle_bot_name_h
#define battle_bot_name_h

namespace battle {
    struct bot_name{
        uint32_t id;
        std::string name;
        
        bot_name(uint32_t id, std::string name):
        id(id), name(name){
            
        };
    };
}


#endif
