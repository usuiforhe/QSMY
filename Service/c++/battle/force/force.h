//
//  force.h
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__force__
#define __battle__force__

#include "battle_prefix.h"
#include "base/base_force.h"
#include "user.h"
#include "game/position.h"

namespace battle{
	typedef enum add_property_type{
		add_pro_type_attack = 1,
		add_pro_type_defense = 2,
		add_pro_type_hp = 3
	} add_unit_property_type;

	class scene;
	class force : public base_force{
        friend class force_builder;
	public:
		force(uint8_t id, scene * scene):
            base_force(id),
            scene_(scene),
            is_mob_(false),
            name_(""),
            total_speed_(0),
            max_speed_(0),
            is_winner_(false),
            alive_num_(0)
        {
            
		}
        
		virtual ~force();
        
        virtual void init();
        
        force * get_opponent() const;
        
        virtual void add_unit(base_unit * unit);
        
        void add_position(position * position);
        
        unit * get_position_unit(uint8_t position_id);
        
        unit * get_hp_min();

        unit * get_hp_max();

        void win();
        
        CONST_GETTER(scene*, scene);
        
        GETTER(const std::string &, name);
        
        GETTER(uint32_t, total_speed);
        
        GETTER(uint32_t, max_speed);
        
        GETTER(bool, is_winner);
        
        GETTER(const std::vector<user*> &, users);
        
        GETTER(uint8_t, alive_num);
        
        inline bool is_dead(){
            return alive_num_ == 0;
        }
        
        void unit_die(unit * unit);
        
        inline void unit_revive(unit * unit){
            ++alive_num_;
        }
        
        inline const std::map<uint8_t, unit*> & get_unit_map(){
            return unit_map_;
        }
        
        bool is_mob() const;
        
        double get_fight();
        
        double get_max_fight();
        
        lua_object get_unit_list_table(bool shuffle);
        unit * get_unit_by_sid(uint32_t sid);
        
        lua_object get_unit_list_table_all(bool shuffle);
        unit * get_unit_by_sid_all(uint32_t sid);
        
        void add_unit_property();
        void del_unit_property();

        typedef std::map<uint8_t, unit *> unit_map_t;
        
	protected:
		scene * scene_;
        bool is_mob_;
        
        std::string name_;
        
        uint32_t total_speed_;
        uint32_t max_speed_;
        
        bool is_winner_;
        
        std::vector<user*> users_;
        
        unit_map_t unit_map_;
        uint8_t alive_num_;
        
        typedef boost::unordered_map<uint8_t, position*> position_map_t;
        position_map_t positions_;
	};
}

#endif /* defined(__battle__force__) */
