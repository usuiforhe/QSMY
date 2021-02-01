#ifndef __battle__skill__
#define __battle__skill__

#include "battle_prefix.h"
#include "base/base_skill.h"
#include "base/i_event_handler.h"
namespace battle{
    typedef enum skill_type{
        SKILL_TYPE_ACTIVE,
        SKILL_TYPE_PASSIVE
    } skill_type;
    
    class unit;
	class skill:public base_skill{
        friend class skill_builder;
	public:
		skill(uint32_t id, uint32_t cd, uint16_t level, lua_object lua_object, unit * owner, uint8_t trigger_rate, bool visible = true):
        base_skill(id, cd, lua_object),
        level_(level),
        owner_(owner),
        trigger_rate_(trigger_rate),
        visible_(visible),
        type_(SKILL_TYPE_ACTIVE),
        trigger_frames_(0),
        prd_n_(1),
        priority_(0),
        position_(0),
        target_(0),
        skill_type_(1),
        unit_up_(100),
        attacker_(NULL){
            
		}

		virtual ~skill(){

		}
        
        virtual bool is_enabled();
        
        virtual bool is_trigger(const lua_object & args);
        
        virtual void effect();
        
        void refresh();
        
        GETTER(unit * const, owner);
        GETTER(uint16_t, level);
        GETTER(uint16_t, trigger_rate);
        GETTER(uint8_t, priority);

        inline void set_trigger_rate(uint16_t trigger_rate){
            trigger_rate_ = static_cast<uint8_t>(trigger_rate);
        }

        inline GETTER(skill_type, type);
        
        inline SETTER(uint16_t, level);
        
	protected:
        uint16_t level_;
		unit * owner_;
        uint8_t trigger_rate_;
		bool visible_;
        skill_type type_;
        uint32_t trigger_frames_;
        uint16_t prd_n_;
        uint8_t priority_;
        
        uint8_t position_;
        uint8_t target_;
        std::vector<uint8_t> coactors_;

        uint8_t skill_type_;
        uint8_t unit_up_;

        //攻击者
        unit * attacker_;
	};
}

#endif // __battle__skill__
