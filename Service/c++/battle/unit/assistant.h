//
//  assistant.h
//  battle
//
//  Created by 汪燕军 on 14-7-2.
//  Copyright (c) 2014年 汪燕军. All rights reserved.
//

#ifndef __battle__assistant__
#define __battle__assistant__

#include "unit.h"
namespace battle
{
    class assistant:public unit
    {
    public:
        assistant(scene * scene, force * force, user * user, uint8_t id, uint32_t sid);
        
        virtual ~assistant();
        
        virtual bool is_alive(){
            return true;
        }
        
        virtual void init();
        
		virtual bool action(bool inc_self_frames_);       
        
    };
}

#endif /* defined(__battle__assistant__) */
