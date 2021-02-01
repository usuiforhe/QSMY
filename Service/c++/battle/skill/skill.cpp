#include "skill.h"
#include "unit/unit.h"
#include "game/scene.h"
#include "util/script_loader.h"
#include "game/behavior/skill_behavior.h"
#include "event/args/unit_event_args.h"
#include "util.h"

using namespace battle;

bool skill::is_enabled()
{
    if(trigger_frames_ == 0){
        return true;
    }
    
    return owner_->get_self_frames() >= trigger_frames_ + cd_;
}

bool skill::is_trigger(const lua_object & args){
    coactors_.clear();
    attacker_ = NULL;
    
    if(!is_enabled()){
        return false;
    }
    
	if(this->skill_type_ == 1){
	    if(!latte::prd::is_achieved(trigger_rate_, prd_n_)){
	        ++prd_n_;
	        return false;
	    }
	}else if(this->skill_type_ == 2){
		if(owner_->up < this->unit_up_ || owner_->is_first_cast == true){
		    if(!latte::prd::is_achieved(trigger_rate_, prd_n_)){
		        ++prd_n_;
		        return false;
		    }
		}else {
			owner_->is_first_cast = true;
		}
	}else if(this->skill_type_ == 3){
		if(owner_->up < this->unit_up_ ){
			if(!latte::prd::is_achieved(trigger_rate_, prd_n_)){
					++prd_n_;
					return false;
			}
		}else{
			if(!latte::prd::is_achieved(trigger_rate_ +10 , prd_n_)){
					++prd_n_;
					return false;
			}
		}
	}
    
    if (luabind::type(lua_object_["is_trigger"]) != LUA_TNIL)
    {
        lua_object result = luabind::call_function<lua_object>(lua_object_["is_trigger"], owner_->get_self_frames(), level_, owner_, args);
        if(result.is_valid() && luabind::type(result) != LUA_TNIL){
            bool is_trigger = luabind::object_cast<bool>(result["trigger"]);
            if(!is_trigger){
                return false;
            }
            
            position_ = luabind::object_cast<uint8_t>(result["position"]);
            target_ = luabind::object_cast<uint8_t>(result["target"]);
            
            lua_object coactors = result["coactors"];
            for(luabind::iterator iter(coactors), end; iter != end; ++iter){
                coactors_.push_back(luabind::object_cast<uint8_t>(*iter));
            }

            uint16_t event_id = luabind::object_cast<uint16_t>(args["event_id"]);
            if(event_id == UNIT_AFTER_DAMAGE){
                uint32_t unit_id = luabind::object_cast<uint32_t>(args["source_id"]);
                attacker_ = owner_->get_scene()->get_unit(unit_id);
            }

            return true;
        }
    }
    
    return false;
}

void skill::effect(){
    if(!owner_->is_alive())
    {
        return;
    }
    
    if(visible_){
        skill_behavior * behavior = new skill_behavior(owner_, sid_, position_, target_);
        BOOST_FOREACH(uint8_t unit_id, coactors_){
            behavior->add_coactor(unit_id);
        }
        owner_->get_scene()->get_report()->push_behavior(behavior);
    }
    
    if (luabind::type(lua_object_["effect"]) != LUA_TNIL)
    {
        lua_object result = luabind::call_function<lua_object>(lua_object_["effect"], owner_->get_self_frames(), level_, owner_, attacker_);
    }
    
    if(visible_){
        owner_->get_scene()->get_report()->pop_behavior();
    }
    
    trigger_frames_ = owner_->get_self_frames();
    prd_n_ = 1;
}

void skill::refresh(){
    trigger_frames_ = 0;
}
