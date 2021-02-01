//
//  unit_buff.h
//  battle
//
//  Created by wangyanjun on 13-3-18.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__unit_buff__
#define __battle__unit_buff__

#include "battle_prefix.h"
#include "base/base_buff.h"
namespace battle {
    class unit;
    class skill;
    class unit_buff:public base_buff{
        friend class buff_builder;
    public:
        unit_buff(uint32_t id, bool is_visible, lua_object script, unit * owner, uint32_t source_unit_id, uint32_t source_skill_id, const lua_object & meta_data, uint8_t interval);
        
        ~unit_buff(){
            
        }
        
        inline GETTER(unit *, owner);
        
        inline GETTER(unit *, source_unit);
        
        inline GETTER(uint32_t, source_skill_id);
        
        virtual void add_layer();
        
        virtual void remove_layer(uint8_t layer);
        
        virtual void enter();
        
        virtual bool is_trigger();
        
        virtual void effect();
        
        virtual void exit();
        
        bool is_match(uint8_t source_unit_id, uint32_t source_skill_id);
        
    private:
        unit * owner_;
        unit * source_unit_;
        uint32_t source_skill_id_;
        lua_object meta_data_;
        
        uint8_t interval_;
        uint32_t last_effect_frame_;
                
        luabind::object build_argument();
    };
}

#endif /* defined(__battle__unit_buff__) */
