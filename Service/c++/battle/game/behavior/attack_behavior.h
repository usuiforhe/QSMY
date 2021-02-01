#ifndef __battle__attack_behavior__
#define __battle__attack_behavior__

#include "battle_prefix.h"
#include "base/base_behavior.h"
#include "behavior_enums.h"

namespace battle {
    class attack_behavior:public base_behavior{
    public:
        explicit attack_behavior(base_unit * actor):base_behavior(actor){
            
        }
        
        ~attack_behavior(){
            
        }
        
        virtual Json::Value to_json();        
        
        virtual void append_to_bin(std::vector<char> & bin);
        
    private:
        
    };
}

#endif // __battle__attack_behavior__
