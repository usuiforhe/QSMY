#include "unit.h"
#include "game/scene.h"
#include "force/force.h"
#include "rule/target_rule.h"
#include "random/random_helper.h"
#include "game/behavior/attack_behavior.h"
#include "game/behavior/damage_behavior_result.h"
#include "game/behavior/heal_behavior_result.h"
#include "game/behavior/display_behavior_result.h"
#include "game/behavior/dialog_behavior_result.h"
#include "util.h"
#include "skill/skill.h"
#include "log/log.h"
#include "event/args/unit_event_args.h"
#include "game/position.h"
#include "builder/skill_builder.h"
#include "util/script_loader.h"
#include "game/behavior/skill_behavior.h"
using namespace battle;

unit::unit(scene * scene, force * force, user * user, uint8_t id, uint32_t sid, unit_type type)
:base_unit(id, sid, type),
level(0),
hp(0),
max_hp(0),
base_attack_point(0),
attack_point(0),
base_defense(0),
defense(0),
base_speed(0),
speed(0),
base_dodge_rate(0),
dodge_rate(0),
base_critical_rate(0),
critical_rate(0),
base_block_rate(0),
block_rate(0),
size(0),
up(0),
dress_id(0),
related_pos_id(0),
skill_fight(0),
is_boss(false),
is_show(true),
critical_multiplier(CRITICAL_MULTIPLIER),
block_multiplier(BLOCK_MULTIPLIER),
base_hit_rate(0),
hit_rate(0),
event_hurt(0),
event_injury(0),
first_turn(false),
base_heal_rate(0),
heal_rate(0),
base_break_rate(0),
break_rate(0),
base_tough_rate(0),
tough_rate(0),
base_criticaldam_rate(0),
criticaldam_rate(0),
locationg(0),
skill_spec(false),
attack_spec(""),
immu_debuff(0),
double_atk(0),
atk_heal(0),
highhp_dam(0),
highhp_def(0),
fieldbuff(""),
heal_debuff(false),
self_frames_(0),
fight_factor_(0),
scene_(scene),
force_(force),
user_(user),
skill_event_handler_(this),
is_first_cast(false),
skill_resist(0),
init_buff("")
{
    init_events();
}

unit::~unit(){
    BOOST_FOREACH(buff_list_map_t::value_type value, buff_list_map_){
        BOOST_FOREACH(unit_buff * buff, *(value.second)){
            delete buff;
        }
        
        delete value.second;
    }
}



uint8_t unit::get_position_id(){
    return position_->get_id();
}

void unit::init(){
    init_hp = hp;
    
    double dodge = static_cast<double>(base_dodge_rate) / 100;
    double block = static_cast<double>(base_block_rate) / 100;
    double critical = static_cast<double>(base_critical_rate) / 100;
    fight_factor_ =  (static_cast<double>(base_attack_point) * (1 +(CRITICAL_MULTIPLIER - 1) * critical) * (1 + skill_fight) * base_speed / 500 + base_attack_point * (1 - dodge) * block * BLOCK_DMG_MULTIPLIER + base_defense) / ((1 - dodge) * (1 - ( 1 - dodge) * block * (1 - BLOCK_MULTIPLIER))) / 1000;
    
    
    if(!is_alive())
    {
        return;
    }

    battle::scheduler & scheduler = scene_->get_scheduler();
    scheduler.add_task(new unit_action_task(this));
    
    std::list<uint8_t> * protectors = S_TARGET_RULE.get_protector_list(position_->get_id());
    if(protectors != NULL){        
        BOOST_FOREACH(uint8_t protector, *protectors){
            battle::unit * unit = force_->get_position_unit(protector);
            if(unit != NULL && unit->is_alive()){
                state_.set_can_be_attacked(false);
            }
        }
    }
    
    //初始化攻击目标优先级列表
    battle::force * target_force = force_->get_opponent();
    std::list<uint8_t> * targets = S_TARGET_RULE.get_target_list(position_->get_id());
    
    BOOST_FOREACH(uint8_t target, *targets){
        battle::unit * unit = target_force->get_position_unit(target);
        if(unit != NULL && unit->is_alive()){
            targets_.push_back(unit);
        }
    }

    if(!this->init_buff.empty() && this->init_buff.size() > 1){
        std::vector<uint32_t> pro;
        latte::split(pro, this->init_buff, ",");

        if(latte::probability_achieved(pro[1])){

            lua_object obj = S_SCRIPT_LOADER.load_skill(pro[0], this->get_scene()->get_lua_state());
            if(!obj.is_valid() || luabind::type(obj) == LUA_TNIL){

            }else{
                lua_object buffs = obj["buffs"];

    			for(luabind::iterator pos(buffs), end; pos != end; ++pos){
    				lua_object buff_ = *pos;
    				uint32_t buf_id = luabind::object_cast<uint32_t>(buff_["buff_id"]);
    				this->add_buff_noresult(buf_id,id_,pro[0],buff_);
    			}
            }
        }
    }

    unit_init_event_args args(this);
    on_init_(this, args);
}

bool unit::action(bool inc_self_frames){
    if(inc_self_frames) {
        ++self_frames_;
    }

    update_buffs();
    
    if(!is_alive()){
        return false;
    }

    if(!state_.get_can_action() || state_.get_break_action()){
        state_.set_break_action(false);
        return true;
    }
    
    unit_main_action_init_event_args init_args(this);
    on_main_action_init_(this, init_args);
    
    if(!is_alive()){
        return true;
    }
    
    if(!state_.get_can_action() || state_.get_break_action()){
        state_.set_break_action(false);
        return true;
    }
    
    if(state_.get_can_attack()){
        unit * target = get_target();
        if(target != NULL){

        	uint8_t atk_times = 1;
        	if(latte::probability_achieved(this->double_atk)){
        		atk_times = 2;
        	}

        	for(uint8_t i = 0; i < atk_times; i++){
                attack(target, UNIT_ATTACK_NORMAL);
        	}
        }
        else{
            return true;
        }
    }
    
    unit_main_action_end_event_args end_args(this);
    on_main_action_end_(this, end_args);
    
    if(state_.get_break_action()){
        state_.set_break_action(false);
    }
    
    return true;
}

unit * unit::get_target(){
    std::list<unit*>::iterator iter = targets_.begin();
    while(iter != targets_.end()){
        unit * target = *iter;
        if(target->is_alive()){
            if(target->state_.get_can_be_attacked())
            {
                return target;
            }
            else{
                ++iter;
            }
        }
        else{
            targets_.erase(iter++);
        }
    }
    
    return NULL;
}

void unit::attack(unit * target, unit_attack_type type){

    unit_before_attack_event_args before_args(this, target, type);
    on_before_attack_(this, before_args);
    
    if(!is_alive() || !target->is_alive()){
        return;
    }
    //TODO

    base_behavior * behavior = new attack_behavior(this);
    scene_->get_report()->push_behavior(behavior);

    if(!this->attack_spec.empty() && this->attack_spec.size() > 1){
        std::vector<uint32_t> pro;
        latte::split(pro, this->attack_spec, ",");

        if(latte::probability_achieved(pro[1])){

            lua_object obj = S_SCRIPT_LOADER.load_skill(pro[0], this->get_scene()->get_lua_state());
            if(!obj.is_valid() || luabind::type(obj) == LUA_TNIL){

            }else{
                lua_object buffs = obj["buffs"];

    			for(luabind::iterator pos(buffs), end; pos != end; ++pos){
    				lua_object buff_ = *pos;
    				uint32_t buf_id = luabind::object_cast<uint32_t>(buff_["buff_id"]);
    				target->add_buff(buf_id,id_,pro[0],buff_);
    			}
            }
        }
    }
    
    int32_t dmg = attack_point;

    bool is_critical = this->is_critical(target->tough_rate);
    if(is_critical){
        dmg = static_cast<int32_t>(dmg * critical_multiplier);
    }

    if(target->is_dodge(this->hit_rate)){
        base_behavior_result * result = new damage_behavior_result(target, 0, false, true, false, hp);
        scene_->get_report()->add_behavior_result(result);
    }
    else{
        bool is_block = type == UNIT_ATTACK_NORMAL && target->is_block();
        if(is_block){
            dmg = static_cast<int32_t>(dmg * (1 - block_multiplier));
        }
        
        //伤害公式
        dmg = dmg + this->event_hurt - target->event_injury - target->defense;

        //TODO
        if(this->hp > target->hp){
            //自身生命高于目标时，对目标伤害 +XX%
        	dmg += static_cast<uint16_t>(dmg * (this->highhp_dam / 100.0));
        }

        target->taking_damage(this, dmg, is_critical, type, is_block);

        //TODO 普通攻击的吸血比例
        uint16_t add_hp = static_cast<uint16_t>(dmg * (this->atk_heal / 100.0));
        if(add_hp > 0){
            this->hp += add_hp;
            if(this->hp > this->max_hp){
                this->hp = this->max_hp;
            }
            base_behavior_result * result = new heal_behavior_result(this, add_hp, false, hp);
            scene_->get_report()->add_behavior_result(result);
        }
        //end
    }
    scene_->get_report()->pop_behavior();

    unit_after_attack_event_args after_args(this, target, type);
    on_after_attack_(this, after_args);
}

int unit::skill_taking_damage(unit * source, int32_t val, bool disable_critical){
    bool is_critical = !disable_critical && source->is_critical(this->tough_rate);
    if(is_critical){
        val = static_cast<int32_t>(val * source->critical_multiplier);
    }
    return taking_damage(source, val, is_critical, UNIT_ATTACK_SKILL, false);
}

int unit::taking_damage(battle::unit *source, int32_t val, bool is_critical, unit_attack_type type, bool is_block){
    //val -= defense;
    
    if(type == UNIT_ATTACK_BLOCK){
        val *= BLOCK_DMG_MULTIPLIER;
    }

	if(this->hp > source->hp){
		//自身生命高于目标时，被攻击时伤害减少 XX%
		val -= static_cast<uint16_t>(val * (this->highhp_def / 100.0));
	}

    if(val <= 0) {
        val = 1;
    }
    
    unit_before_damage_event_args before_damage_args(this, source, val);
    on_before_damage_(this, before_damage_args);
    
    if(!source->is_alive() || !is_alive()){
        return 0;
    }
    
    hp -= val;
    
    if(hp <= 0) {
        hp = 0;
        is_block = false;
    }
    
    base_behavior_result * result = new damage_behavior_result(this, val, is_critical, false, is_block, hp);
    scene_->get_report()->add_behavior_result(result);
    
    unit_after_damage_event_args after_damage_args(this, source, val);
    on_after_damage_(this, after_damage_args);

    unit_after_target_damage_event_args after_target_damage_args(source, this, val);
    on_after_target_damage_(source, after_target_damage_args);
    
    if(!is_alive()) {
        unit_before_die_event_args before_die_args(this, source);
        on_before_die_(this, before_die_args);
        if(!is_alive()){
            die();
            unit_after_die_event_args after_die_args(this, source);
            on_after_die_(this, after_die_args);
            
            unit_kill_event_args kill_args(source, this);
            on_kill_(source, kill_args);
        }
    }
    else if(is_block){
        attack(source, UNIT_ATTACK_BLOCK);
    }
    return val;
}

void unit::heal(battle::unit *source, int32_t val, bool disable_critical){
    if(val <= 0){
        return;
    }
    
    if(!is_alive()){
        return;
    }
    
    bool is_critical = !disable_critical && source->is_critical();
    if(is_critical){
        val *= source->critical_multiplier;
    }
    unit_before_heal_event_args before_heal_args(this, source, val);
    on_before_heal_(this, before_heal_args);
    
    if(!is_alive()){
        return;
    }
    hp += val;
    if(hp > max_hp){
        hp = max_hp;
    }
    
    base_behavior_result * result = new heal_behavior_result(this, val, is_critical, hp);
    scene_->get_report()->add_behavior_result(result);
    
    unit_after_heal_event_args after_heal_args(this, source, val);
    on_after_heal_(this, after_heal_args);
}

bool unit::is_critical(){
    return state_.get_can_critical() && latte::probability_achieved(critical_rate);
}

bool unit::is_critical(uint16_t target_tough_rate){
    return state_.get_can_critical() && latte::probability_achieved(std::max(0 , critical_rate - target_tough_rate));
}

bool unit::is_dodge(){
    return state_.get_can_dodge() && latte::probability_achieved(dodge_rate);
}

bool unit::is_dodge(uint16_t hit_rate){

	int32_t a = static_cast<int32_t>(hit_rate);
	int32_t b = static_cast<int32_t>(dodge_rate);
    int16_t dodge_rate_new = std::max<int16_t>(0, b - a);

    return state_.get_can_dodge() && latte::probability_achieved(dodge_rate_new);
}

bool unit::is_block(){
    return state_.get_can_block() && latte::probability_achieved(block_rate);
}

void unit::die(){
    if(size == 1){
        std::list<uint8_t> * protectees = S_TARGET_RULE.get_protectee_list(position_->get_id());
        if(protectees != NULL){
            BOOST_FOREACH(uint8_t protectee, *protectees){
                unit * u = force_->get_position_unit(protectee);
                if(u != NULL && u != this){
                    u->state_.set_can_be_attacked(true);
                }
            }
        }
    }
    else{
        std::set<uint8_t> s;
        uint8_t lines = (size + 1) / 2;
        for(uint8_t i = 0; i < lines; ++i){
            uint8_t pos = position_->get_id() - 2 * i;
            std::list<uint8_t> * protectees = S_TARGET_RULE.get_protectee_list(pos);
            
            if(protectees != NULL){
                BOOST_FOREACH(uint8_t protectee, *protectees){
                    if(s.find(protectee) == s.end()){
                        s.insert(protectee);
                    }
                }
            }
        }
        
        BOOST_FOREACH(uint8_t protectee, s){
            unit * u = force_->get_position_unit(protectee);
            if(u != NULL && u != this){
                u->state_.set_can_be_attacked(true);
            }
        }
    }
    
    force_->unit_die(this);
    this->get_force()->del_unit_property();
    
#if DEBUG
    std::stringstream & ss = latte::fast_stringstream::ss();
    ss << "unit is die. id:" << static_cast<uint32_t>(id_) << ", sid:" << sid_;
    s_log.out_debug(ss.str());
#endif
}

void unit::dialog(uint32_t dialog_id)
{
    base_behavior_result * result = new dialog_behavior_result(this, dialog_id);
    scene_->get_report()->add_behavior_result(result);
}

void unit::show()
{
    is_show = true;
    base_behavior_result * result = new display_behavior_result(this, true);
    scene_->get_report()->add_behavior_result(result);
}

void unit::hide()
{
    is_show = false;
    base_behavior_result * result = new display_behavior_result(this, false);
    scene_->get_report()->add_behavior_result(result);
}

//------------------------------skill start -------------------------------------

void unit::add_skill(uint32_t skill_id, uint8_t level){
    if(skill_map_.find(skill_id) == skill_map_.end()){
        S_SKILL_BUILDER.build(this, skill_id, level);
    }
}

void unit::remove_skill(uint32_t skill_id){
    BOOST_AUTO(pos,skill_map_.find(skill_id));
    if(pos != skill_map_.end()){
        boost::shared_ptr<skill> skill = pos->second;
        skill_map_.erase(pos);
    }
}

boost::shared_ptr<skill> unit::get_skill(uint32_t sid){
    BOOST_AUTO(pos, skill_map_.find(sid));
    if(pos != skill_map_.end()){
        return pos->second;
    }
    else{
        return boost::shared_ptr<skill>();
    }
}

skill * unit::get_skill_ptr(uint32_t sid){
    return get_skill(sid).get();
}

void unit::bind_skill(uint32_t event_id, boost::shared_ptr<skill> skill){
    skill_event_handler_.bind_skill(event_id, skill);
    skill_map_[skill->get_sid()] = skill;
}

void unit::bind_event_handler(uint32_t event_id, i_event_handler<unit*> * handler){
    BOOST_AUTO(pos, event_map_.find(event_id));
    if(pos != event_map_.end()){
        pos->second->bind(handler);
    }
}

void unit::cast_skill(boost::shared_ptr<skill> skill){    
    unit_before_skill_event_args before_args(this, skill);
    on_before_skill_(this, before_args);
#if DEBUG
    s_log.out_debug("before cast skill:"+boost::lexical_cast<std::string>(skill->get_sid()));
#endif
    if(is_alive()){
        skill->effect();
        if(is_alive()){
            unit_after_skill_event_args after_args(this, skill);
            on_after_skill_(this, after_args);
        }
#if DEBUG
        s_log.out_debug("after cast skill:"+boost::lexical_cast<std::string>(skill->get_sid()));
#endif
    }
}

//------------------------------skill end -------------------------------------

//------------------------------protected methods--------------------------------

void unit::init_events(){
#define ADD_EVENT(event_id, name) event_map_.insert(std::make_pair(event_id, &name))
    
    ADD_EVENT(UNIT_INIT, on_init_);
    ADD_EVENT(UNIT_MAIN_ACTION_INIT, on_main_action_init_);
    ADD_EVENT(UNIT_BEFORE_ATTACK, on_before_attack_);
    ADD_EVENT(UNIT_AFTER_ATTACK, on_after_attack_);
    ADD_EVENT(UNIT_BEFORE_BE_ATTACKED, on_before_be_attacked_);
    ADD_EVENT(UNIT_AFTER_BE_ATTACKED, on_after_be_attacked_);
    ADD_EVENT(UNIT_BEFORE_SKILL, on_before_skill_);
    ADD_EVENT(UNIT_AFTER_SKILL, on_after_skill_);
    ADD_EVENT(UNIT_BEFORE_DAMAGE, on_before_damage_);
    ADD_EVENT(UNIT_AFTER_DAMAGE, on_after_damage_);
    ADD_EVENT(UNIT_AFTER_TARGET_DAMAGE, on_after_target_damage_);
    ADD_EVENT(UNIT_BEFORE_BUFF_ADDED, on_before_buff_added_);
    ADD_EVENT(UNIT_AFTER_BUFF_ADDED, on_after_buff_added_);
    ADD_EVENT(UNIT_BEFORE_BUFF_REMOVED, on_before_buff_removed_);
    ADD_EVENT(UNIT_AFTER_BUFF_REMOVED, on_after_buff_removed_);
    ADD_EVENT(UNIT_BEFORE_DIE, on_before_die_);
    ADD_EVENT(UNIT_AFTER_DIE, on_after_die_);
    ADD_EVENT(UNIT_KILL, on_kill_);
}

