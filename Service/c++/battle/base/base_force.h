#ifndef __battle__base_force__
#define __battle__base_force__

#include "battle_prefix.h"
#include "base_unit.h"
namespace battle
{
	class base_force
	{
	public:
		base_force(uint8_t id):id_(id){

		}

		virtual ~base_force(){

		}

		CONST_GETTER(uint8_t, id);
        
        virtual void init() = 0;

		virtual void add_unit(base_unit * unit){
			if(unit_map_.count(unit->get_id()) == 0){
				unit_map_[unit->get_id()] = unit;
			}
		}

		virtual base_unit * get_unit(uint8_t id){
			if(unit_map_.count(id) == 0){
				return NULL;
			}
			return unit_map_[id];
		}
	protected:
		uint8_t id_;
		boost::unordered_map<uint8_t, base_unit*> unit_map_;
	};
}

#endif // __battle__base_force__
