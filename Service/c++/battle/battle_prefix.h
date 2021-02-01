#ifndef __battle__battle_prefix__
#define __battle__battle_prefix__

#include "latte_prefix.h"

#include <luabind/lua_include.hpp>
#include <luabind/luabind.hpp>
#include <json/json.h>
#include <boost/foreach.hpp>

extern "C"
{
    #include <lualib.h>
}

#include "log/log.h"
#include "util.h"
#include "defines.h"

typedef luabind::adl::object lua_object;

#endif // __battle__battle_prefix__
