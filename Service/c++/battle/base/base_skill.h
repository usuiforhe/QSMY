#ifndef __battle__base_skill__
#define __battle__base_skill__

#include "battle_prefix.h"
namespace battle{
	class base_skill
	{
	public:
		base_skill(uint32_t sid, uint32_t cd, lua_object lua_object):sid_(sid), cd_(cd), lua_object_(lua_object){

		}
		virtual ~base_skill(){};

		GETTER(uint32_t, sid);

		GETTER(uint32_t, cd);
        
		SETTER(uint32_t, cd);
        
        virtual bool is_enabled() = 0;

		virtual bool is_trigger(const lua_object & args) = 0;

		virtual void effect() = 0;

	protected:
		uint32_t sid_;
		uint32_t cd_;
        
        lua_object lua_object_;
	};
}

#endif // __battle__base_skill__
