#ifndef __battle__position__
#define __battle__position__

#include "battle_prefix.h"
#include "unit/unit.h"
namespace battle{
	class position{
	public:
		position(uint8_t id):id_(id), unit_(NULL){}
		~position(){}

		inline GETTER(uint8_t, id);
		inline GETTER(unit *, unit);
		inline SETTER(unit *, unit);
	private:
		uint8_t id_;
		unit * unit_;
	};
}

#endif // __battle__position__
