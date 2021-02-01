//
//  unit_event_handler.h
//  battle
//
//  Created by wangyanjun on 13-3-16.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef battle_unit_event_handler_h
#define battle_unit_event_handler_h

#include "battle_prefix.h"
#include "base/i_event_handler.h"
#include "base/event.h"

namespace battle {
    class unit;
    class skill;
    
    class skill_event_handler : public i_event_handler<unit*>{
    public:
        explicit skill_event_handler(unit * owner):owner_(owner){}
        
        ~skill_event_handler(){
            BOOST_FOREACH(skill_map_t::value_type list, skill_map_){
                delete list.second;
            }
        }
        
        void bind_skill(uint32_t event_id, boost::shared_ptr<skill> skill);
        
        void unbind_skill(boost::shared_ptr<skill> skill);
        
        virtual void handle_event(unit * sender, base_event_args & args);
        
    private:
        typedef boost::unordered_map<uint32_t, std::list<boost::shared_ptr<skill> > * > skill_map_t;
        skill_map_t skill_map_;
        
        unit * owner_;
    };
    
#define UNIT_EVENT_HANDLER(T) class T:public i_event_handler<unit*>{\
    public:\
        virtual void handle_event(unit * sender, base_event_args & args);\
    }
}


#endif
