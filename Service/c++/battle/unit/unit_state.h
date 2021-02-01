//
//  unit_state.h
//  battle
//
//  Created by wangyanjun on 13-3-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__unit_state__
#define __battle__unit_state__

#include "battle_prefix.h"
namespace battle{
    class unit_state{
    public:
        unit_state():disable_action_(0), disable_attack_(0), disable_cast_skill_(0), disable_block_(0), disable_dodge_(0), disable_critical_(0), disable_be_attacked_(0), break_action_(false){
            
        }
        
        ~unit_state(){
            
        }
        
        bool get_can_action(){
            return disable_action_ == 0;
        }
        
        inline void set_can_action(bool val){
            if(val){
                if(disable_action_ > 0){
                    --disable_action_;
                }
            }
            else{
                ++disable_action_;
            }
        }
        
        bool get_can_attack(){
            return disable_attack_ == 0;
        }
        
        void set_can_attack(bool val){
            if(val){
                if(disable_attack_ > 0){
                    --disable_attack_;
                }
            }
            else{
                ++disable_attack_;
            }
        }
        
        bool get_can_cast_skill(){
            return disable_cast_skill_ == 0;
        }
        
        void set_can_cast_skill(bool val){
            if(val){
                if(disable_cast_skill_ > 0){
                    --disable_cast_skill_;
                }
            }
            else{
                ++disable_cast_skill_;
            }
        }
        
        bool get_can_block(){
            return disable_block_ == 0;
        }
        
        void set_can_block(bool val){
            if(val){
                if(disable_block_ > 0){
                    --disable_block_;
                }
            }
            else{
                ++disable_block_;
            }
        }
        
        bool get_can_dodge(){
            return disable_dodge_ == 0;
        }
        
        void set_can_dodge(bool val){
            if(val){
                if(disable_dodge_ > 0){
                    --disable_dodge_;
                }
            }
            else{
                ++disable_dodge_;
            }
        }
        
        bool get_can_critical(){
            return disable_critical_ == 0;
        }
        
        void set_can_critical(bool val){
            if(val){
                if(disable_critical_ > 0){
                    --disable_critical_;
                }
            }
            else{
                ++disable_critical_;
            }
        }
        
        bool get_can_be_attacked(){
            return disable_be_attacked_ == 0;
        }
        
        void set_can_be_attacked(bool val){
            if(val){
                if(disable_be_attacked_ > 0){
                    --disable_be_attacked_;
                }
            }
            else{
                ++disable_be_attacked_;
            }
        }
        
        GETTER(bool, break_action);
        SETTER(bool, break_action);
        
        void reset(){
            disable_action_ = 0;
            disable_attack_ = 0;
            disable_cast_skill_ = 0;
            disable_block_ = 0;
            disable_dodge_ = 0;
            disable_critical_ = 0;
            disable_be_attacked_ = 0;
            break_action_ = false;
        }
        
    private:
        uint8_t disable_action_;
        uint8_t disable_attack_;
        uint8_t disable_cast_skill_;
        uint8_t disable_block_;
        uint8_t disable_dodge_;
        uint8_t disable_critical_;
        uint8_t disable_be_attacked_;
        bool break_action_;
    };
}

#endif /* defined(__battle__unit_state__) */
