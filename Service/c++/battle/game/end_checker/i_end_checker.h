//
//  i_end_checker.h
//  battle
//
//  Created by 汪燕军 on 13-5-3.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__i_end_checker__
#define __battle__i_end_checker__

#include "battle_prefix.h"

namespace battle {
    class scene;
    struct i_end_checker{
    public:
        i_end_checker(){
            
        }
        
        virtual ~i_end_checker(){
            
        }
        
        virtual void check(scene * scene) = 0;
    };
}

#endif /* defined(__battle__i_end_checker__) */
