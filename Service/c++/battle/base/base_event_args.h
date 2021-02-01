#ifndef __battle__base_event_args__
#define __battle__base_event_args__

#include "battle_prefix.h"
namespace battle{
	struct base_event_args{
	public:
		explicit base_event_args(uint16_t id):id(id){

		}

		virtual ~base_event_args(){

		}
        
        //
        // 将事件参数转化为luabind::object
        //
        virtual lua_object to_lua(lua_State* L) const
		{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
			return obj;
		}
        
        uint16_t id;
	};
}

#endif // __battle__base_event_args__
