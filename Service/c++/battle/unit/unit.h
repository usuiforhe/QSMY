#ifndef __battle__unit__
#define __battle__unit__

#include "battle_prefix.h"
#include "base/base_unit.h"
#include "base/event.h"
#include "unit_state.h"
#include "event/event_defines.h"
#include "event/handler/unit_event_handler.h"
#include "buff/unit_buff.h"

namespace battle{
	class scene;
	class force;
    class user;
    class round;
	class position;
    class skill;
	class unit:public base_unit{
        
        typedef event<unit*> unit_event;
        friend class unit_builder;
        
	public:
        uint16_t level;
		int32_t hp;
        int32_t max_hp;
        int32_t init_hp;

        int32_t base_attack_point;
		int32_t attack_point;

        int32_t base_defense;
		int32_t defense;

		uint32_t base_speed;
		uint32_t speed;

        uint16_t base_dodge_rate;
		uint16_t dodge_rate;

        uint16_t base_critical_rate;
		uint16_t critical_rate;

        uint16_t base_block_rate;
        uint16_t block_rate;

        uint8_t size;

        uint8_t up;

        uint32_t dress_id;

        uint8_t related_pos_id;

        float skill_fight;
        bool is_boss;
        bool is_show;
        
        double critical_multiplier;
        double block_multiplier;

        //TODO
        uint16_t base_hit_rate;//基础命中
        uint16_t hit_rate;// 命中计算

        uint16_t event_hurt;//最终伤害
        uint16_t event_injury;//最终免伤

        bool first_turn;//上场一次攻击必释放技能

        uint16_t base_heal_rate;//基础治疗
        uint16_t heal_rate;//治疗计算

        uint16_t base_break_rate;//破击
        uint16_t break_rate;

        uint16_t base_tough_rate;//抗暴
        uint16_t tough_rate;

        uint16_t base_criticaldam_rate;//暴击伤害率
        uint16_t criticaldam_rate;

        uint8_t locationg;//英雄类型
        bool skill_spec;//技能特殊效果1,0
        std::string attack_spec;//该英雄普通攻击时有10%附带1011DEBUFF 如buffid,概率
        uint16_t immu_debuff;//免疫DEBUFF如免疫10000
        uint8_t double_atk;//该英雄普通攻击时有10%额外再攻击一次
        uint8_t atk_heal;//普通攻击的吸血比例
        uint8_t highhp_dam;//自身生命高于目标时，对目标伤害 +XX%
        uint8_t highhp_def;//自身生命高于目标时，被攻击时伤害减少 XX%
        std::string fieldbuff;//该单位在场上，增加所有 XX 类型英雄 XX% 的 XX属性;如:类型,属性,值
        bool heal_debuff;//治疗DEBUFF
        bool is_first_cast;//是否是首次是否技能
        uint16_t skill_resist;//技能免伤
        std::string init_buff;//初始化buff
        //TODO
        
		unit(scene * scene, force * force, user * user, uint8_t id, uint32_t sid, unit_type type);

		virtual ~unit();
        
        GETTER(scene *, scene);
        
        GETTER(force *, force);
        
        GETTER(position *, position);
        
        GETTER(unit_state &, state);
        
        GETTER(uint32_t, self_frames);
        
        unit_state * get_state_ptr(){
            return &state_;
        }
        
        virtual bool is_alive(){
            return hp > 0;
        }
        
        uint8_t get_position_id();
        
        inline GETTER(const std::vector<position*> &, extra_positions);

        inline double get_fight(){
            return hp * fight_factor_;
        }
        
        inline double get_max_fight(){
            return max_hp * fight_factor_;
        }
        
        inline GETTER(double, fight_factor);
        
        void init();
        
		bool action(bool inc_self_frames_);
        
        unit * get_target();
        
        void attack(unit * target, unit_attack_type type = UNIT_ATTACK_NORMAL);
        
        int skill_taking_damage(unit * source, int32_t val, bool disable_critical);
        
        int taking_damage(unit * source, int32_t val, bool is_critical, unit_attack_type type, bool is_block = false);
        
        void heal(unit * source, int32_t val, bool disable_critical);
        
        bool is_critical();
        
        bool is_critical(uint16_t target_tough_rate);

        bool is_dodge();
        
        bool is_dodge(uint16_t hit_rate);
        
        bool is_block();
        
        void die();
        
        void dialog(uint32_t dialog_id);
        
        void show();
        
        void hide();
        
        inline int16_t getId(){
        	return static_cast<int16_t>(this->id_);
        }

        //-----------------------------skill start-------------------------------------
        
        void add_skill(uint32_t skill_id, uint8_t level);
        
        void remove_skill(uint32_t skill_id);
        
        boost::shared_ptr<skill> get_skill(uint32_t sid);
        
        skill * get_skill_ptr(uint32_t sid);
        
        void bind_skill(uint32_t event_id, boost::shared_ptr<skill> skill);
        
        void bind_event_handler(uint32_t event_id, i_event_handler<unit*> * handler);
        
        void cast_skill(boost::shared_ptr<skill> skill);

        //-----------------------------skill end--------------------------------------
        
        //-----------------------------buff start-------------------------------------
               
        
        void add_buff(uint32_t buff_id, uint8_t source_unit_id, uint32_t source_skill_id, const lua_object & meta_data);

        void add_buff_noresult(uint32_t buff_id, uint8_t source_unit_id, uint32_t source_skill_id, const lua_object & meta_data);
        
        void remove_buff(uint32_t buff_id, uint8_t source_unit_id = 0, uint32_t source_skill_id = 0);
        
        void remove_buff_layer(uint32_t buff_id, uint8_t layer, uint8_t source_unit_id = 0, uint32_t source_skill_id = 0);
        
        uint16_t get_buff_layer(uint32_t buff_id, uint8_t source_unit_id = 0, uint32_t source_skill_id = 0);
        
        bool is_buff_exist(uint32_t buff_id);
        
        void update_buffs();
        
        typedef std::map<uint32_t, std::list<unit_buff*>*> buff_list_map_t;
        buff_list_map_t buff_list_map_;
        
        //-----------------------------buff end --------------------------------------
	protected:
        unit_state state_;
        uint32_t self_frames_;
        double fight_factor_;

		scene * scene_;
		force * force_;
        user * user_;
		position * position_;
        std::vector<position *>  extra_positions_;
        std::list<unit * > targets_;
        
        typedef std::map<uint32_t, boost::shared_ptr<battle::skill> > skill_map_t;
        std::map<uint32_t, boost::shared_ptr<battle::skill> > skill_map_;
        
        boost::unordered_map<uint16_t, unit_event*> event_map_;
        skill_event_handler skill_event_handler_;
     
//------------------------------事件-------------------------------
        unit_event on_init_;
        unit_event on_main_action_init_;
        unit_event on_main_action_end_;
        unit_event on_before_attack_;
        unit_event on_after_attack_;
        unit_event on_before_be_attacked_;
        unit_event on_after_be_attacked_;
        unit_event on_before_skill_;
        unit_event on_after_skill_;
        unit_event on_before_damage_;
        unit_event on_after_damage_;
        unit_event on_after_target_damage_;
        unit_event on_before_buff_added_;
        unit_event on_after_buff_added_;
        unit_event on_before_buff_removed_;
        unit_event on_after_buff_removed_;
        unit_event on_before_die_;
        unit_event on_after_die_;
        unit_event on_kill_;
        unit_event on_before_heal_;
        unit_event on_after_heal_;
        
//-------------------------------方法--------------------------------
        void init_events();
	};
}

#endif // __battle__unit__
