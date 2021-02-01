//
//  warrior.h
//  cache
//
//  Created by 汪燕军 on 13-5-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__warrior__
#define __cache__warrior__

#include "cache_prefix.h"
#include "protocol/warrior.pb.h"
#include "base/async_update_object.h"

namespace cache {
    class warrior:public async_update_object{
    public:
        warrior(uint64_t id,
                uint32_t sid,
                uint16_t level,
                uint32_t exp,
                uint8_t up,
                uint32_t attack_add,
                uint32_t defense_add,
                uint32_t hp_add,
                uint32_t potential,
                uint32_t skill_exp,
                uint32_t skill_lv,
                uint8_t grade,
                uint32_t develop_item_num,
                uint32_t dress_id
                ):
        async_update_object(),
        id_(id),
        sid_(sid),
        level_(level),
        exp_(exp),
        up_(up),
        attack_add_(attack_add),
        defense_add_(defense_add),
        hp_add_(hp_add),
        potential_(potential),
        skill_exp_(skill_exp),
        skill_lv_(skill_lv),
        grade_(grade),
        develop_item_num_(develop_item_num),
        dress_id_(dress_id)
        {
            
        }
        
        explicit warrior(const proWarrior & warrior):
        async_update_object(),
        id_(0),
        sid_(warrior.sid()),
        level_(static_cast<uint16_t>(warrior.level())),
        exp_(warrior.exp()),
        up_(static_cast<uint8_t>(warrior.up())),
        attack_add_(warrior.attack_add()),
        defense_add_(warrior.defense_add()),
        hp_add_(warrior.hp_add()),
        potential_(warrior.potential()),
        skill_exp_(warrior.skill_exp()),
        skill_lv_(static_cast<uint16_t>(warrior.skill_lv())),
        grade_(static_cast<uint8_t>(warrior.grade())),
        develop_item_num_(warrior.develop_item_num()),
        dress_id_(warrior.dress_id())
        {
            
        }
        
        inline GETTER(uint64_t, id);
        inline SETTER(uint64_t, id);
        inline GETTER(uint32_t, sid);
        inline GETTER(uint16_t, level);
        inline GETTER(uint32_t, exp);
        inline GETTER(uint8_t, up);
        inline GETTER(uint32_t, attack_add);
        inline GETTER(uint32_t, defense_add);
        inline GETTER(uint32_t, hp_add);
        inline GETTER(uint32_t, potential);
        inline GETTER(uint32_t, skill_exp);
        inline GETTER(uint16_t, skill_lv);
        inline GETTER(uint8_t, grade);
        inline GETTER(uint32_t, develop_item_num);
        inline GETTER(uint32_t, dress_id);
        
        inline GETTER(boost::shared_mutex &, mutex);
        
        virtual ~warrior(){}
        
        inline std::auto_ptr<proWarrior> to_proto(){
            std::auto_ptr<proWarrior> proto(new proWarrior);
            READ_LOCK(lock, mutex_);
            
            proto->set_sid(sid_);
            proto->set_level(level_);
            proto->set_exp(exp_);
            proto->set_up(up_);
            proto->set_attack_add(attack_add_);
            proto->set_defense_add(defense_add_);
            proto->set_hp_add(hp_add_);
            proto->set_potential(potential_);
            proto->set_skill_exp(skill_exp_);
            proto->set_skill_lv(skill_lv_);
            proto->set_grade(grade_);
            proto->set_develop_item_num(develop_item_num_);
            proto->set_dress_id(dress_id_);
            
            return proto;
        }
        
        inline void copy_from(const warrior & source){
            level_ = source.level_;
            exp_ = source.exp_;
            up_ = source.up_;
            attack_add_ = source.attack_add_;
            defense_add_ = source.defense_add_;
            hp_add_ = source.hp_add_;
            potential_ = source.potential_;
            skill_exp_ = source.skill_exp_;
            skill_lv_ = source.skill_lv_;
            grade_ = source.grade_;
            develop_item_num_ = source.develop_item_num_;
            dress_id_ = source.dress_id_;
        }
        
        inline void copy_from(const proWarrior & source){
            level_ = static_cast<uint16_t>(source.level());
            exp_ = source.exp();
            up_ = static_cast<uint8_t>(source.up());
            attack_add_ = source.attack_add();
            defense_add_ = source.defense_add();
            hp_add_ = source.hp_add();
            potential_ = source.potential();
            skill_exp_ = source.skill_exp();
            skill_lv_ = static_cast<uint16_t>(source.skill_lv());
            grade_ = static_cast<uint8_t>(source.grade());
            develop_item_num_ = source.develop_item_num();
            dress_id_ = source.dress_id();
        }
        
        inline void copy_to_proto(proWarrior * proto){
            READ_LOCK(lock, mutex_);
            
            proto->set_sid(sid_);
            proto->set_level(level_);
            proto->set_exp(exp_);
            proto->set_up(up_);
            proto->set_attack_add(attack_add_);
            proto->set_defense_add(defense_add_);
            proto->set_hp_add(hp_add_);
            proto->set_potential(potential_);
            proto->set_skill_exp(skill_exp_);
            proto->set_skill_lv(skill_lv_);
            proto->set_grade(grade_);
            proto->set_develop_item_num(develop_item_num_);
            proto->set_dress_id(dress_id_);
        }
        
        inline void copy_to_warrior_res(pro_warrior_res_ptr res){
            READ_LOCK(lock, mutex_);
            
            proWarrior * proto = new proWarrior;
            res->set_allocated_warrior(proto);
            proto->set_sid(sid_);
            proto->set_level(level_);
            proto->set_exp(exp_);
            proto->set_up(up_);
            proto->set_attack_add(attack_add_);
            proto->set_defense_add(defense_add_);
            proto->set_hp_add(hp_add_);
            proto->set_potential(potential_);
            proto->set_skill_exp(skill_exp_);
            proto->set_skill_lv(skill_lv_);
            proto->set_grade(grade_);
            proto->set_develop_item_num(develop_item_num_);
            proto->set_dress_id(dress_id_);
        }
        
    private:
        uint64_t id_;
        uint32_t sid_;
        uint16_t level_;
        uint32_t exp_;
        uint8_t up_;
        uint32_t attack_add_;
        uint32_t defense_add_;
        uint32_t hp_add_;
        uint32_t potential_;
        uint32_t skill_exp_;
        uint16_t skill_lv_;
        uint8_t grade_;
        uint32_t develop_item_num_;
        uint32_t dress_id_;
        
        boost::shared_mutex mutex_;
    };
}

#endif /* defined(__cache__warrior__) */
