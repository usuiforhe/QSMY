#ifndef __battle__base_unit__
#define __battle__base_unit__

#include "battle_prefix.h"
#include "base_skill.h"
namespace battle{

	typedef enum unit_type
	{
		//普通单位
		unit_type_normal = 1,
		//机关兽
		unit_type_gundam = 2,
		//后援
		unit_type_assist = 3
	}
	unit_type;

	class base_force;
	class base_unit{
	public:
		base_unit(uint8_t id, uint32_t sid, unit_type type):id_(id), sid_(sid), type_(type){

		}

		virtual ~base_unit(){

		}
        
        virtual void init() = 0;
        
        virtual bool is_alive()
        {
            return true;
        }

		GETTER(uint8_t, id);

		GETTER(uint32_t,sid);

		GETTER(unit_type,type);

	protected:
		uint8_t id_;
		uint32_t sid_;
        unit_type type_;

		base_force * force_;
		std::vector<boost::shared_ptr<base_skill> > skills_;
	};
}

#endif // __battle__base_unit__
