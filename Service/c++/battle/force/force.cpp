//
//  force.cpp
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "force.h"
#include "user.h"
#include "unit/unit.h"
#include "game/scene.h"
using namespace battle;

force::~force(){
    BOOST_FOREACH(position_map_t::value_type val, positions_){
        delete val.second;
    }
    
    BOOST_FOREACH(user * user, users_){
        delete user;
    }
}

void force::init(){
    BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
        val.second->init();
    }
}

force * force::get_opponent() const{
    if(id_ == LEFT_FORCE_ID){
        return scene_->get_right_force();
    }
    else{
        return scene_->get_left_force();
    }
}

void force::add_unit(base_unit * unit){
    if(unit_map_.count(unit->get_id()) == 0){
        unit_map_.insert(std::make_pair(unit->get_id(), dynamic_cast<battle::unit*>(unit)));
        if(unit->is_alive() && unit->get_type() == unit_type_normal)
        {
            ++alive_num_;
        }
    }
}

void force::add_position(battle::position *position){
    if(positions_.count(position->get_id()) == 0){
        positions_.insert(std::make_pair(position->get_id(), position));
    }
}

unit * force::get_position_unit(uint8_t position_id){    
    BOOST_AUTO(pos, positions_.find(position_id));
    if(pos != positions_.end()){
        return pos->second->get_unit();
    }
    
    return NULL;
}

unit * force::get_hp_min(){
	uint16_t hp = 0XFFFF;
	unit * target_unit = NULL;
	BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
		if(val.second->hp < hp){
			target_unit = val.second;
		}
	}

	return target_unit;
}

unit * force::get_hp_max(){
	uint16_t hp = 0X00;
	unit * target_unit = NULL;
	BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
		if(val.second->hp > hp){
			target_unit = val.second;
		}
	}

	return target_unit;
}

void force::add_unit_property(){
	BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
		std::string fieldbuff = val.second->fieldbuff;
		if(!fieldbuff.empty() && fieldbuff.size() > 1){
            std::vector<uint16_t> pro;
            latte::split(pro, fieldbuff, ",");
			BOOST_FOREACH(unit_map_t::value_type u, unit_map_){
				if(u.second->locationg == pro[0]){
					if(pro[1] == add_pro_type_attack){
						u.second->attack_point += static_cast<int32_t>(u.second->attack_point * (pro[2] / 100.0));
					}else if(pro[1] == add_pro_type_defense){
						u.second->defense += static_cast<int32_t>(u.second->defense * (pro[2] / 100.0));
					}else if(pro[1] == add_pro_type_hp){
						u.second->hp += static_cast<int32_t>(u.second->hp * (pro[2] / 100.0));
					}
				}
			}
		}
	}
}

void force::del_unit_property(){
	BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
		uint16_t id = val.second->get_id();
		std::string fieldbuff = val.second->fieldbuff;
		if(!fieldbuff.empty() && fieldbuff.size() > 1){
            std::vector<uint16_t> pro;
            latte::split(pro, fieldbuff, ",");
			BOOST_FOREACH(unit_map_t::value_type u, unit_map_){
				if(id != u.second->get_id()){
					if(u.second->locationg == pro[0]){
						if(pro[1] == add_pro_type_attack){
							u.second->attack_point -= static_cast<int32_t>(u.second->attack_point * (pro[2] / 100.0));
						}else if(pro[1] == add_pro_type_defense){
							u.second->defense -= static_cast<int32_t>(u.second->defense * (pro[2] / 100.0));
						}else if(pro[1] == add_pro_type_hp){
							u.second->hp -= static_cast<int32_t>(u.second->hp * (pro[2] / 100.0));
						}
					}
				}
			}
		}
	}
}

void force::win(){
    is_winner_ = true;
    double percent = get_fight() / get_max_fight();
    uint8_t grade(1); 
    if(percent >= THREE_STAR_THRESHOLD){
        grade = 3;
    }
    else if(percent >= TWO_STAR_THRESHOLD){
        grade = 2;
    }
    
    scene_->over(grade);
}

void force::unit_die(battle::unit *unit)
{
    --alive_num_;
    scene_->unit_die(unit);
    
#if DEBUG
    std::stringstream & ss = latte::fast_stringstream::ss();
    ss << "alive num:" << static_cast<uint32_t>(alive_num_);
    s_log.out_debug(ss.str());
#endif
}

bool force::is_mob() const{
    return is_mob_;
}

double force::get_fight(){
    double fight(0);
    BOOST_FOREACH(user * user, users_){
        fight += user->get_fight();
    }
    
    return fight;
}

double force::get_max_fight(){
    double fight(0);
    BOOST_FOREACH(user * user, users_){
        fight += user->get_max_fight();
    }
    
    return fight;
}

lua_object force::get_unit_list_table(bool shuffle){
    if (unit_map_.size() > 0)
    {
        luabind::object table = luabind::newtable(scene_->get_lua_state());
        if(shuffle){              
            std::vector<unit*> units;
            units.reserve(unit_map_.size());

            BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
                if(val.second->is_alive() && val.second->get_type() == unit_type_normal){
                    units.push_back(val.second);
                }
            }
            std::random_shuffle(units.begin(), units.end());
            int i(0);
            BOOST_FOREACH(unit* u, units){
                table[++i] = u;
            }
        }
        else{            
            int i(0);
            BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
                if(val.second->is_alive() && val.second->get_type() == unit_type_normal){
                    table[++i] = val.second;
                }
            }
        }
        
        return table;
    }
    else
    {
        return luabind::newtable(scene_->get_lua_state());
    }
}

unit * force::get_unit_by_sid(uint32_t sid){    
    
    BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
        if(val.second->get_sid() == sid && val.second->get_type() == unit_type_normal){
            return val.second;
        }
    }
    return NULL;
}

lua_object force::get_unit_list_table_all(bool shuffle){
    if (unit_map_.size() > 0)
    {
        luabind::object table = luabind::newtable(scene_->get_lua_state());
        if(shuffle){
            std::vector<unit*> units;
            units.reserve(unit_map_.size());

            BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
                if(val.second->is_alive()){
                    units.push_back(val.second);
                }
            }
            std::random_shuffle(units.begin(), units.end());
            int i(0);
            BOOST_FOREACH(unit* u, units){
                table[++i] = u;
            }
        }
        else{
            int i(0);
            BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
                if(val.second->is_alive()){
                    table[++i] = val.second;
                }
            }
        }

        return table;
    }
    else
    {
        return luabind::newtable(scene_->get_lua_state());
    }
}

unit * force::get_unit_by_sid_all(uint32_t sid){
    BOOST_FOREACH(unit_map_t::value_type val, unit_map_){
        if(val.second->get_sid() == sid){
            return val.second;
        }
    }
    return NULL;
}

