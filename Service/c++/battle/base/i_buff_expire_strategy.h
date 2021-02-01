//
//  i_expire_strategy.h
//  battle
//
//  Created by wangyanjun on 13-3-18.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef battle_i_buff_expire_strategy_h
#define battle_i_buff_expire_strategy_h
namespace battle {
    class base_buff;
    class i_buff_expire_strategy{
    public:
        i_buff_expire_strategy(base_buff * buff){}
        
        virtual ~i_buff_expire_strategy(){}
        
        virtual void on_add() = 0;
                
        virtual bool is_expired() = 0;
        
        virtual void refresh() = 0;
        
    protected:
        base_buff * buff_;
    };
}

#endif
