#ifndef __battle__base_buff__
#define __battle__base_buff__

#include "battle_prefix.h"
#include "i_buff_expire_strategy.h"
namespace battle{
	class base_buff{
	public:
		base_buff(uint32_t id, bool is_visible, lua_object script):
        id_(id),
        is_visible_(is_visible),
        layer_(1),
        effect_times_(0),
        expire_strategy_(NULL),
        script_(script){

		}
        
        virtual ~base_buff(){
            if(expire_strategy_ != NULL){
                delete expire_strategy_;
            }
        }

		inline uint32_t get_id(){
			return id_;
		}
        
        inline bool is_visible(){
            return is_visible_;
        }
                
        virtual void add_layer(){
            ++layer_;
        }
        
        virtual uint8_t get_layer(){
            return layer_;
        }
        
        virtual void remove_layer(uint8_t layer){
            layer_ -= layer;
        }
        
		virtual void enter() = 0;
        
        virtual bool is_trigger() = 0;
        
		virtual void effect(){
            ++effect_times_;
        };
        
        virtual bool is_expired(){
            return layer_ <= 0 || expire_strategy_->is_expired();
        }
        
		virtual void exit() = 0;
	protected:
		uint32_t id_;
        bool is_visible_;
        uint8_t layer_;
        uint32_t last_effect_frame_;
        uint8_t effect_times_;
        i_buff_expire_strategy * expire_strategy_;
        lua_object script_;
	};
}

#endif // __battle__base_buff__
