#include "script_loader.h"
#include "util.h"
#include "config.h"

using namespace battle;

lua_object script_loader::load_scene(uint32_t sid, lua_State *L){
    std::string str_sid = boost::lexical_cast<std::string>(sid);
    if(load_script(L, "scene/" + str_sid)){
        return luabind::globals(L)["scene"];
    }
    else{
        return lua_object();
    }
}

lua_object script_loader::load_skill(uint32_t sid, lua_State *L){
    std::string str_sid = boost::lexical_cast<std::string>(sid);
    if(load_script(L, "skill/" + str_sid)){
        return luabind::globals(L)["g_skill_"+str_sid];
    }
    else{
        return lua_object();
    }    
}

lua_object script_loader::load_buff(uint32_t sid, lua_State *L){
    std::string str_sid = boost::lexical_cast<std::string>(sid);
    if(load_script(L, "buff/" + str_sid)){
        return luabind::globals(L)["g_buff_" + str_sid];
    }
    else{
        return lua_object();
    }
}

lua_object script_loader::load_bot(uint32_t sid, lua_State *L){
    std::string str_sid = boost::lexical_cast<std::string>(sid);
    if(load_script(L, "bot/" + str_sid)){
        return luabind::globals(L)["bot"];
    }
    else{
        return lua_object();
    }
}

lua_object script_loader::load_bot_name(lua_State *L){
    if(load_script(L, "bot/name/bot_names")){
        return luabind::globals(L)["bot_names"];
    }
    else{
        return lua_object();
    }
}

bool script_loader::load_script(lua_State *L, const std::string &name){
	std::stringstream &ss = latte::fast_stringstream::ss();
	ss << S_CONFIG.get_script_path() << name << S_CONFIG.get_lua_extension();
	std::string full_name = ss.str();
	
    try
    {
        if (luaL_dofile(L, full_name.c_str()) != 0)
        {
            std::stringstream &ss = latte::fast_stringstream::ss();
            ss << "open script file error. file name:" << full_name;
            ss << ", error:" << lua_tostring(L, -1);
            BOOST_THROW_EXCEPTION(std::runtime_error(ss.str()));
        }
        
        return true;
    }
    catch (boost::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "script_loader::load_script throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());
        return false;
    }
    catch (std::exception &e)
    {
        std::stringstream &ss = latte::fast_stringstream::ss();
        ss << "script_loader::load_script throws exceptions. what:" + boost::diagnostic_information(e);
        s_log.out_error(ss.str());
        return false;
    }
	
}
