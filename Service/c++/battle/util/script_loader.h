#ifndef __battle__script_loader__
#define __battle__script_loader__

#include "battle_prefix.h"
#include "singleton.h"

namespace battle {
    class script_loader{
    public:
        script_loader(){}
        
        ~script_loader(){}
        
        lua_object load_scene(uint32_t sid, lua_State * L);
        
        lua_object load_skill(uint32_t id, lua_State * L);
        
        lua_object load_buff(uint32_t id, lua_State * L);
        
        lua_object load_bot(uint32_t id, lua_State * L);
        
        lua_object load_bot_name(lua_State * L);
        
        bool load_script(lua_State * L, const std::string & name);
    };
}

#define S_SCRIPT_LOADER latte::singleton<script_loader>::instance()


#endif // __battle__script_loader__
