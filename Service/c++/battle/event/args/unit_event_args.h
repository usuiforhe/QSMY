//
//  unit_event_args.h
//  battle
//
//  Created by wangyanjun on 13-3-15.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef battle_unit_event_args_h
#define battle_unit_event_args_h

#include "battle_prefix.h"
#include "base/base_event_args.h"
#include "event/event_defines.h"
#include "skill/skill.h"
namespace battle {
    class unit;
    struct unit_event_args : public base_event_args{
        unit_event_args(uint16_t id, unit * owner):base_event_args(id), owner(owner){
            
        }
        
        virtual ~unit_event_args(){
            
        }
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;

			return obj;
        }
        
        unit * owner;
    };
    
    
    //
    // 单位初始化事件参数
    //
    struct unit_init_event_args : public unit_event_args{
    public:
        unit_init_event_args(unit * owner):unit_event_args(UNIT_INIT, owner){
            
        }
        
        ~unit_init_event_args(){}
    };
    
    //
    // 单位主动回合初始化事件参数
    //
    struct unit_main_action_init_event_args:public unit_event_args{
    public:
        unit_main_action_init_event_args(unit * owner):unit_event_args(UNIT_MAIN_ACTION_INIT, owner){
            
        }
        
        ~unit_main_action_init_event_args(){}
    };
    
    //
    // 单位主动回合结束化事件参数
    //
    struct unit_main_action_end_event_args:public unit_event_args{
    public:
        unit_main_action_end_event_args(unit * owner):unit_event_args(UNIT_MAIN_ACTION_END, owner){
            
        }
        
        ~unit_main_action_end_event_args(){}
    };
    
    //
    // 攻击前事件参数
    //
    struct unit_before_attack_event_args : public unit_event_args{
        unit_before_attack_event_args(unit * owner, unit * target, uint8_t type):unit_event_args(UNIT_BEFORE_ATTACK, owner), target(target), type(type){
            
        }
        
        ~unit_before_attack_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["type"] = type;
            obj["target_id"] = target->get_id();
            
			return obj;
        }
        
        unit * target;
        uint8_t type;
    };
    
    //
    // 攻击后事件参数
    //
    struct unit_after_attack_event_args : public unit_event_args{
        unit_after_attack_event_args(unit * owner, unit * target, uint8_t type):unit_event_args(UNIT_AFTER_ATTACK, owner), target(target), type(type){
            
        }
        
        ~unit_after_attack_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["type"] = type;
            obj["target_id"] = target->get_id();
            
			return obj;
        }
        
        unit * target;
        uint8_t type;
    };
    
    //
    // 被攻击前事件参数
    //
    struct unit_before_be_attacked_event_args : public unit_event_args{
        unit_before_be_attacked_event_args(unit * owner, unit * attacker, uint8_t type):unit_event_args(UNIT_BEFORE_BE_ATTACKED, owner), attacker(attacker), type(type){
            
        }
        
        ~unit_before_be_attacked_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["attacker_id"] = attacker->get_id();
            obj["type"] = type;
            
			return obj;
        }
        
        unit * attacker;
        uint8_t type;
    };

    //
    // 被攻击后事件参数
    //
    struct unit_after_be_attacked_event_args : public unit_event_args{
        unit_after_be_attacked_event_args(unit * owner, unit * attacker, uint8_t type):unit_event_args(UNIT_AFTER_BE_ATTACKED, owner), attacker(attacker), type(type){
            
        }
        
        ~unit_after_be_attacked_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["attacker_id"] = attacker->get_id();
            obj["type"] = type;
            
			return obj;
        }
        
        unit * attacker;
        uint8_t type;
    };
    
    //
    // 释放技能前事件参数
    //
    struct unit_before_skill_event_args : public unit_event_args{
        unit_before_skill_event_args(unit * owner, boost::shared_ptr<battle::skill> p_skill):unit_event_args(UNIT_BEFORE_SKILL, owner), skill(p_skill){
            
        }
        
        ~unit_before_skill_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["skill_id"] = skill->get_sid();
            obj["skill_level"] = skill->get_level();
            
			return obj;
        }
        
        boost::shared_ptr<battle::skill> skill;
    };
    
    //
    // 释放技能后事件参数
    //
    struct unit_after_skill_event_args : public unit_event_args{
        unit_after_skill_event_args(unit * owner, boost::shared_ptr<battle::skill> p_skill):unit_event_args(UNIT_AFTER_SKILL, owner), skill(p_skill){
            
        }
        
        ~unit_after_skill_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["skill_id"] = skill->get_sid();
            obj["skill_level"] = skill->get_level();
            
			return obj;
        }
        
        boost::shared_ptr<battle::skill> skill;
    };
    
    //
    // 受到伤害前事件参数
    //
    struct unit_before_damage_event_args : public unit_event_args{
        unit_before_damage_event_args(unit * owner, unit * source, uint32_t damage): unit_event_args(UNIT_BEFORE_DAMAGE, owner), source(source), damage(damage){
            
        }
        
        ~unit_before_damage_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["source_id"] = source->get_id();
            obj["damage"] = damage;
            
			return obj;
        }
        
        unit * source;
        uint32_t damage;
    };
    
    //
    // 受到伤害后事件参数
    //
    struct unit_after_damage_event_args : public unit_event_args{
        unit_after_damage_event_args(unit * owner, unit * source, uint32_t damage): unit_event_args(UNIT_AFTER_DAMAGE, owner), source(source), damage(damage){
            
        }
        
        ~unit_after_damage_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["source_id"] = source->get_id();
            obj["damage"] = damage;
            
			return obj;
        }
        
        unit * source;
        uint32_t damage;
    };
    
    //
    // 目标受到伤害后事件参数
    //
    struct unit_after_target_damage_event_args : public unit_event_args{
        unit_after_target_damage_event_args(unit * owner, unit * target, uint32_t damage): unit_event_args(UNIT_AFTER_TARGET_DAMAGE, owner), target(target), damage(damage){
            
        }
        
        ~unit_after_target_damage_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["target_id"] = target->get_id();
            obj["damage"] = damage;
            
			return obj;
        }
        
        unit * target;
        uint32_t damage;
    };
    
    //
    // 被添加buff前事件参数
    //
    struct unit_before_buff_added_event_args : public unit_event_args{
        unit_before_buff_added_event_args(unit * owner, uint32_t buff_id, uint32_t source_unit_id, uint32_t source_skill_id):
        unit_event_args(UNIT_BEFORE_BUFF_ADDED, owner),
        buff_id(buff_id),
        source_unit_id(source_unit_id),
        source_skill_id(source_skill_id){
            
        }
        
        ~unit_before_buff_added_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["buff_id"] = buff_id;
            obj["source_unit_id"] = source_unit_id;
            obj["source_skill_id"] = source_skill_id;
            
			return obj;
        }
        
        uint32_t buff_id;
        uint32_t source_unit_id;
        uint32_t source_skill_id;
    };
    
    //
    // 被添加buff后事件参数
    //
    struct unit_after_buff_added_event_args : public unit_event_args{
        unit_after_buff_added_event_args(unit * owner, uint32_t buff_id, uint32_t source_unit_id, uint32_t source_skill_id):
        unit_event_args(UNIT_AFTER_BUFF_ADDED, owner),
        buff_id(buff_id),
        source_unit_id(source_unit_id),
        source_skill_id(source_skill_id){
            
        }
        
        ~unit_after_buff_added_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["buff_id"] = buff_id;
            obj["source_unit_id"] = source_unit_id;
            obj["source_skill_id"] = source_skill_id;
            
			return obj;
        }        
        
        uint32_t buff_id;
        uint32_t source_unit_id;
        uint32_t source_skill_id;
    };
    
    //
    // 被移除buff前事件参数
    //
    struct unit_before_buff_removed_event_args : public unit_event_args{
        unit_before_buff_removed_event_args(unit * owner, uint32_t buff_id, uint32_t source_unit_id, uint32_t source_skill_id, bool is_expired):
        unit_event_args(UNIT_BEFORE_BUFF_REMOVED, owner),
        buff_id(buff_id),
        source_unit_id(source_unit_id),
        source_skill_id(source_skill_id),
        is_expired(is_expired){
            
        }
        
        ~unit_before_buff_removed_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["buff_id"] = buff_id;
            obj["source_unit_id"] = source_unit_id;
            obj["source_skill_id"] = source_skill_id;
            obj["is_expired"] = is_expired;
            
			return obj;
        }
        
        uint32_t buff_id;
        uint32_t source_unit_id;
        uint32_t source_skill_id;
        bool is_expired;
    };
    
    //
    // 被移除buff后事件参数
    //
    struct unit_after_buff_removed_event_args : public unit_event_args{
        unit_after_buff_removed_event_args(unit * owner, uint32_t buff_id, uint32_t source_unit_id, uint32_t source_skill_id, bool is_expired):
        unit_event_args(UNIT_AFTER_BUFF_REMOVED, owner),
        buff_id(buff_id),
        source_unit_id(source_unit_id),
        source_skill_id(source_skill_id),
        is_expired(is_expired){
            
        }
        
        ~unit_after_buff_removed_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["buff_id"] = buff_id;
            obj["source_unit_id"] = source_unit_id;
            obj["source_skill_id"] = source_skill_id;
            obj["is_expired"] = is_expired;
            
			return obj;
        }
        
        
        uint32_t buff_id;
        uint32_t source_unit_id;
        uint32_t source_skill_id;
        bool is_expired;
    };
    
    //
    // 死亡前事件参数
    //
    struct unit_before_die_event_args : public unit_event_args{
        unit_before_die_event_args(unit * owner, unit * killer):unit_event_args(UNIT_BEFORE_DIE, owner), killer(killer){
            
        }
        
        ~unit_before_die_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["killer_id"] = killer->get_id();
            
			return obj;
        }
        
        unit * killer;
    };
    
    //
    // 死亡后事件参数
    //
    struct unit_after_die_event_args : public unit_event_args{
        unit_after_die_event_args(unit * owner, unit * killer):unit_event_args(UNIT_AFTER_DIE, owner), killer(killer){
            
        }
        
        ~unit_after_die_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["killer_id"] = killer->get_id();
            
			return obj;
        }
        
        unit * killer;
    };
    
    //
    // 击杀事件参数
    //
    struct unit_kill_event_args : public unit_event_args{
        unit_kill_event_args(unit * owner, unit * dead): unit_event_args(UNIT_KILL, owner),dead(dead){
            
        }
        
        ~unit_kill_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["dead_id"] = dead->get_id();
            
			return obj;
        }
        
        unit * dead;
    };
    
    //
    // 受到伤害前事件参数
    //
    struct unit_before_heal_event_args : public unit_event_args{
        unit_before_heal_event_args(unit * owner, unit * source, uint32_t heal): unit_event_args(UNIT_BEFORE_HEAL, owner), source(source), heal(heal){
            
        }
        
        ~unit_before_heal_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["heal"] = heal;
            
			return obj;
        }
        
        unit * source;
        uint32_t heal;
    };
    
    //
    // 受到伤害后事件参数
    //
    struct unit_after_heal_event_args : public unit_event_args{
        unit_after_heal_event_args(unit * owner, unit * source, uint32_t heal): unit_event_args(UNIT_AFTER_HEAL, owner), source(source), heal(heal){
            
        }
        
        ~unit_after_heal_event_args(){}
        
        virtual lua_object to_lua(lua_State * L) const{
			lua_object obj = luabind::newtable(L);
            obj["event_id"] = id;
            obj["heal"] = heal;
            
			return obj;
        }
        
        unit * source;
        uint32_t heal;
    };
}

#endif
