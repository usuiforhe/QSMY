//
//  unit_frames_buff_expire_strategy.h
//  battle
//
//  Created by wangyanjun on 13-3-19.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__unit_frames_buff_expire_strategy__
#define __battle__unit_frames_buff_expire_strategy__

#include "battle_prefix.h"
#include "base/base_buff.h"
#include "base/i_buff_expire_strategy.h"

namespace battle{
    class unit;
    class unit_frames_buff_expire_strategy : public i_buff_expire_strategy{
    public:
        unit_frames_buff_expire_strategy(base_buff * buff, unit * owner, uint32_t last_frames):
        i_buff_expire_strategy(buff), owner_(owner),last_frames_(last_frames), add_frames_(0){
            
        }
        
        ~unit_frames_buff_expire_strategy(){
            
        }
        
        
        virtual void on_add();
        
        virtual bool is_expired();
        
        virtual void refresh();
        
    private:
        unit * owner_;
        
        uint32_t last_frames_;
        
        uint32_t add_frames_;
    };
}

#endif /* defined(__battle__unit_frames_buff_expire_strategy__) */
