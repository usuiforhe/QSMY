//
//  normal_end_checker.h
//  battle
//
//  Created by 汪燕军 on 13-5-3.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__normal_end_checker__
#define __battle__normal_end_checker__

#include "game/end_checker/i_end_checker.h"

namespace battle {
    class normal_end_checker:public i_end_checker{
    public:
        virtual void check(scene * scene);
        
    private:
        void calculate_grade(double percent);
    };
    
#define S_NORMAL_END_CHECKER latte::singleton<normal_end_checker>::instance()
}

#endif /* defined(__battle__normal_end_checker__) */
