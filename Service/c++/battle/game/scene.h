#ifndef __battle__scene__
#define __battle__scene__

#include "battle_prefix.h"
#include "base/i_updatable.h"
#include "force/force.h"
#include "base/event.h"
#include "report.h"
#include "unit/unit.h"
#include "scheduler.h"
#include "end_checker/i_end_checker.h"

namespace battle{
	typedef enum scene_type{
        // PVE
		SCENE_TYPE_PVE = 1,
        // 杯赛
        SCENE_TYPE_CUP = 2,
        //千机楼
        SCENE_TYPE_TOWER = 3,
        //挑战赛
        SCENE_TYPE_CHALLENGE = 4,
        //狩魂
        SCENE_TYPE_HUNT = 5,
        //军争
        SCENE_TYPE_GUILD_ROB = 6
	} scene_type;
    
    class position;
    
	typedef event<scene*> scene_event;
    
	class scene{
        friend class scene_builder;
        
	public:
		scene(uint32_t sid, scene_type type);

		~scene();
        
        inline bool is_pve(){
            return type_ == SCENE_TYPE_PVE || type_ == SCENE_TYPE_TOWER || type_ == SCENE_TYPE_GUILD_ROB;
        }
        
        inline bool is_end(){
            return is_end_;
        }
        
        /**
         * 添加单位到场景中
         */
        void add_unit(unit * unit);
        /**
         * 获取场景中的道具
         */
        unit * get_unit(uint32_t id);
        /**
         * 执行战斗
         */
		void execute();
        /**
         * 单位死亡
         */
        void unit_die(unit * dier);
        
        void check_end(){
            end_checker_->check(this);
        }

        /**
         * 战斗结束
         */
		void over(uint8_t grade);
        /**
         * 获取战斗回合
         */
        uint32_t get_frames();
        
        CONST_GETTER(uint32_t, sid);
        
        CONST_GETTER(scene_type, type);
        
        CONST_GETTER(uint32_t, map_id);
        
		inline GETTER(report *, report);
        
        inline GETTER(lua_State *, lua_state);
        
        inline GETTER(scheduler &, scheduler);
        
        inline GETTER(force*, left_force);
        
        inline GETTER(force*, right_force);
        
        inline uint8_t next_unit_id(){
            return ++current_unit_id_;
        }

	private:        
        /**
         * 场景编号
         */
        uint32_t sid_;
        
		scene_type type_;//场景类型
        
        uint32_t map_id_;//关卡编号
        
        bool is_end_;//是否结束
        
        uint8_t current_unit_id_;//单位
        
        std::map<uint8_t, unit*> unit_map_;//场景单位
        
        force * left_force_;//左边势力
        
        force * right_force_;//右边势力
        
        lua_State * lua_state_;//LUA状态
        scheduler scheduler_;//排程
        i_end_checker * end_checker_;//结算检查
		report * report_;//战斗报告

		scene_event on_scene_init_;//场景初始化事件
		scene_event on_scene_over_;//场景结束事件
	};
}

#endif // __battle__scene__
